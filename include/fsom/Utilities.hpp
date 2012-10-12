/*
**  Copyright (C) 2011  Stephen Pearse and David Devaney
**
**  This file is part of FSOM (Free Sound Object Mixer).
**  FSOM is free software: you can redistribute it and/or modify
**  it under the terms of the GNU Lesser General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.

**  FSOM is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU Lesser General Public License for more details.

**  You should have received a copy of the GNU Lesser General Public License
**  along with FSOM.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef __UTILITIES__
#define __UTILITIES__

#include <cassert>
#include <cstring>

#include <iostream>
#include <vector>
#include <algorithm>

namespace fsom{
 
const float SR = 44100.0;
const double TWOPI = 2.0*3.14159;
const double PI = 3.14159;

/// audio objects should be measuring time in integer time values
/// use the following typedefs for time references.
typedef long int SamplePosition;
typedef long int SampleLength;

void generic_sleep(int t);

///Implements non copyable object inherit privately
class NonCopyable{
  private:
  	NonCopyable(const NonCopyable&); //This must not be implemented!!!
	NonCopyable& operator = (const NonCopyable&); //This Must Not Be Implemented either!!!
	
  public:
	NonCopyable(){}

};

/// Mutex Interface, platform specific versions needed
class Mutex{
  protected:
	Mutex(){}
  public:
	virtual ~Mutex(){}
	static Mutex* create();
	static void destroy(Mutex* mutex);
	virtual void lock() = 0;
	virtual bool try_lock() = 0;
	virtual void unlock() = 0;
};

class ScopedMutexLock{
	Mutex* m_mtx;
  public:
	ScopedMutexLock(Mutex* mtx) : m_mtx(mtx) { assert(m_mtx); m_mtx->lock(); }
	~ScopedMutexLock(){ assert(m_mtx); m_mtx->unlock();}
};


inline void interleave(const float** in,float* out, const int channels, const int frameCount){
    float *p=out;
    for(int frame = 0;frame < frameCount;++frame){
        for(int chan = 0;chan < channels;++chan){
            *p = in[chan][frame];
            ++p;
        }
    }
}

inline void deinterleave(const float* in,float** out,const int channels,const int frameCount){
	const float *p=in;
	for (int frame = 0;frame < frameCount;++frame){
	    for(int chan = 0; chan < channels;++chan){
		out[chan][frame]=*p;
			    ++p;
	    }
	}
}

/// a procedure for clearing the contents, setting to silence the contents of a buffer
inline void clear_multichannel_buffers(float** buffers, const int channels, const int frameCount){
	for(int n = 0;n < channels;++n){
		memset(buffers[n],0,frameCount*sizeof(float));
	}
}

inline float remap(float r1_a, float r1_b, float r2_a, float r2_b, float in){
  
  float d1 = r1_b - r1_a;
  float d2 = r2_b - r2_a;
  assert(r1_b > r1_a );
  assert(r2_b > r2_a); 
  float dv = in - r1_a;
 
  
  return dv/d1 *d2 + r2_a ;
}

/// a simple dsp phasor class for use in oscilator and control objects.
/// it provides a tick function that should be called once per sample of playback.
/// a get phase function returns the current phase of the phasor in the range 0.0f to 1.0f.
class Phasor{
	float m_phase;
	float m_sr;
	float m_delta;
	float m_frequency;
	bool m_isFirstTime;
public:
	Phasor(float sampleRate = 44100.0f, float frequency = 1.0f, float phase = 0.0f) :
		m_phase(phase),
		m_sr(sampleRate),
		m_delta(frequency/sampleRate),
		m_isFirstTime(true)
	{}
	float get_frequency() const { return m_frequency; }
	void set_frequency(float frequency){
		m_frequency = frequency;
		m_delta = frequency/m_sr;
	}	
	float get_phase() const { return m_phase; }
	void set_phase(float phase){
		m_phase = phase;
	}
	// call me to tick the phasor forward by one sample.
	void tick(){
		m_phase += m_delta;
		if(m_phase < 0.0f){
		  m_phase += 1.0f;
		}
		else if(m_phase >= 1.0f){
		  m_phase -= 1.0f;
		}
	}

};

inline float phase_wrap(float x){
  while(x > 1) x -= 1.0f;
  while(x < 0) x +=1.0f;
  return x;
}
//class designed to manage the creation, access to and destruction of multi-channel buffers
class MultiChannelBuffer{
float** m_buffers;
int m_channels;
size_t m_size; // size of individual buffers
public:
	MultiChannelBuffer(int channels, size_t size) :
		m_channels(channels),
		m_size(size)
	{
		m_buffers=new float* [channels];	
		for (int n=0;n < channels;n++){
			m_buffers[n]=new float[size];
		} 	
	}
	~MultiChannelBuffer(){
		for (int n=0;n < m_channels;n++){
			delete[] m_buffers[n];
		}	
	delete[] m_buffers;
	}
	float** get_buffers(){
		return m_buffers;
	}
	void clear(){
		clear_multichannel_buffers(m_buffers,m_channels,m_size);
	}
	size_t size() const { return m_size; }
};

/// This function is a helper to aid offseting into a multichannel buffers.
/// It uses a static array and is not thread safe and any new call will invalid a previous one.
/// However, it does not have to allocate or de-allocate so is good in the dsp call.
inline float** get_multichannel_offset_ptrs(float** buffers, int channels, size_t offset){
	static const int MAX_OUTPUT_CHANNELS = 64; // should be more than enough for stereo lol.
	assert(channels < MAX_OUTPUT_CHANNELS);
	static float* temp[MAX_OUTPUT_CHANNELS];
	for (int n=0;n < channels;n++){
		temp[n]=buffers[n]+offset;
	}
	return temp;
}

class FMinMax{
public:
	float fmin,fmax;
	FMinMax() : fmin(0),fmax(0){
	}
	FMinMax(float a,float b) : fmin(a),fmax(b){
	}
	FMinMax(const FMinMax& other): fmin(other.fmin),fmax(other.fmax){
	}

};

template<class T>
T wrap(T value){
	while(value > 1.0){
		value -= 1;
	}
	while(value < 0.0){
		value += 1;
	}
	return value;
}


//NEW BREAKPOINT SYSTEM

struct TVPair{
  float t_;
  float v_;
  
  TVPair() :
    t_(0),
    v_(0)
    {};
  TVPair(float t, float v) :
    t_(t),
    v_(v)
    {};
    ///copy constructor
   TVPair(const TVPair& old):t_(old.t_),v_(old.v_){}
   bool operator < (const TVPair& op) const {
     return t_ < op.t_;// && v_ < op.v_;
   }
   
};

inline bool operator < (const TVPair& op, const float& t) {
  return op.t_ < t;
}

inline bool operator < (const float& t, const TVPair& op) {
  return  t < op.t_;
}


class BreakPointUnit{
    typedef std::vector<TVPair> BPList;
    BPList bpList_;
  public:
    BreakPointUnit() {}
    ///Copy constructor
    BreakPointUnit(const BreakPointUnit& old){
	for(int n = 0; n < old.bpList_.size();++n){
	    bpList_.push_back(old.bpList_.at(n));
	}
      
    }
    void add_breakpoint( const TVPair& tvPair){
      bpList_.push_back(tvPair);
      std::sort( bpList_.begin(), bpList_.end() );
    }
    
    float get_value(float absT) const {
      assert( bpList_.size() > 0 );
      if(  absT <= bpList_.begin()->t_ ) return bpList_.begin()->v_;
      if(  absT >= bpList_.rbegin()->t_ ) return bpList_.rbegin()->v_;
      
      // find lower tv pair for input t value (TVPair A)
      // get next one too (TVPair B)
      // do linear interpolate based on A B and t;
     BPList::const_iterator itB = std::upper_bound(bpList_.begin(), bpList_.end(), absT);
     BPList::const_iterator itA = itB-1;
     
     float t = absT - itA->t_;
     float dt = itB->t_ - itA->t_;
     float r = t/dt;
   //  std::cout << "t = " << t << ": r=" << r << std::endl;
     return (1.0f-r)*itA->v_ + r * itB->v_;

    }
    
    const TVPair& get_pair(int index) const {return bpList_.at(index);}
    
    TVPair& get_pair(int index) {
      return bpList_.at(index);
    }
    
    void remove_pair(int index){
	bpList_.erase(bpList_.begin()+index);
	sort();
    }
    
    void remove_pair(fsom::TVPair pair){
      BPList::iterator it;
      for(it = bpList_.begin(); it != bpList_.end(); ++it){
	if((*it).t_ == pair.t_ && ((*it).v_ == pair.v_)){
	  break;
	}
      }
      bpList_.erase(it);
      sort();  
    }
      
    int get_list_size(){ return bpList_.size();}
    
  /*  TVPair get_closest_pair(float Time){
	    assert( bpList_.size() > 0 );
	//    if(  Time <= bpList_.begin()->t_ ) return bpList_.begin();
	//    if(  Time >= bpList_.rbegin()->t_ ) return bpList_.rbegin();
	    
	  BPList::iterator itB = std::upper_bound(bpList_.begin(), bpList_.end(), Time);
	  BPList::iterator itA = itB-1;
	  
	 std::cout << "time = " << Time << std::endl;
	 std::cout << "itB->t_ = " << itB->t_ << " : itA->t_ = " << itA->t_ << std::endl;
	 std::cout << "itB index = " << itB - bpList_.begin() << std::endl;	  
	 
	 if((itB->t_ - Time) < (Time - itA->t_)){
	    return (*itB);
	 }else{
	    return (*itA);  
	 }
    }*/
    
    void print_breakpoints(){
     /* 
       for( BPList::iterator it = bpList_.begin();it != bpList_.end();	++it){
		std::cout << " : time = " << it->t_ << " : val ="  << it->v_ << std::endl;
       } 
       */
    }
    
    void sort(){ std::sort(bpList_.begin(), bpList_.end());}
};


}//fsom

#endif
