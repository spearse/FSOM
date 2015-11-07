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
#include <fstream>
#include <cstdint>
#include <cmath>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>

#else
#include <unistd.h>
#endif

namespace fsom
{

const float SR = 44100.0f;
const float PI = 3.14159265359f;
const float TWOPI = 6.28318530718f;

/// audio objects should be measuring time in integer time values
/// use the following typedefs for time references.
// 32bit signed gives us 2^31/44100/60 = ~811 minutes
typedef int32_t SamplePosition;
typedef int32_t SampleLength;

static_assert(sizeof(SamplePosition) == 4, "Bad sample position size.");
static_assert(sizeof(SampleLength) == 4, "Bad sample length size.");

typedef std::vector<float> ChannelAmplitudes;

#ifdef DEBUG
#define dbgStream std::cerr
#define DEBUG_ONLY(x) x
#else
#define dbgStream g_debugLog
#define DEBUG_ONLY(x)
#endif

#ifdef _WIN32
#define DEBUG_OUTPUT(x) OutputDebugString(x);
//#define DEBUG_OUTPUT(x) std::cerr << x;
#else
#define DEBUG_OUTPUT(x) std::cerr << x;
#endif

#ifdef DEBUG
#define DebugOut(fmt, ...)          \
	char buf[256];                  \
	sprintf(buf, fmt, __VA_ARGS__); \
	DEBUG_OUTPUT(buf)

#else
#define DebugOut(fmt, ...)

#endif

// static std::ofstream g_debugLog("log.txt");

class DebugStreamBuf : public std::streambuf
{
	static const int SIZE = 8;
	char m_buf[SIZE];
	std::ofstream m_file;

  private:
	void doflush()
	{
		std::ptrdiff_t n = pptr() - pbase();
		pbump(-n);
//fsom::DebugStream << "|";
#ifdef DEBUG
		std::cerr.write(pbase(), n);
		m_file.write(pbase(), n);
#endif
		// OutputDebugString()
	}

  protected:
	virtual int overflow(int ch)
	{
		if (ch != traits_type::eof())
		{
			*pptr() = ch;
			pbump(1);
			doflush();
			return ch;
		}
		return traits_type::eof();
	}
	virtual int sync()
	{
		doflush();
		return 0;
	}

  public:
	DebugStreamBuf()
		: m_file("log.txt")
	{
		setp(m_buf, m_buf + SIZE - 1);
	}
};

extern std::ostream DebugStream;

//peak struct designed to store peak amp, the time of occurence and whether it has clipped..
struct PeakData
{
	float amp;
	fsom::SamplePosition position;
	bool clipped;
	PeakData()
		: amp(0.0f)
		, position(0)
		, clipped(false)
	{
	}
	void analyse(float** data, int chan, int blockSize, SamplePosition initialPos)
	{

		for (int n = 0; n < blockSize; ++n)
		{
			for (int c = 0; c < chan; ++c)
			{
				if (std::abs(data[c][n]) >= amp)
				{
					amp = std::abs(data[c][n]);
					position = initialPos + n;
					if (!clipped)
					{
						if (amp >= 1.0f)
						{
							clipped = true;
						}
					}
				}
			}
		}
	}
	void reset()
	{
		amp = 0;
		clipped = false;
	}
};

void generic_sleep(int t);

///Implements non copyable object inherit privately
class NonCopyable
{
  private:
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;

  public:
	NonCopyable() {}
};

/// Mutex Interface, platform specific versions needed
class Mutex
{
  protected:
	Mutex() {}
  public:
	virtual ~Mutex() {}
	static Mutex* create();
	static void destroy(Mutex* mutex);
	virtual void lock() = 0;
	virtual bool try_lock() = 0;
	virtual void unlock() = 0;
};

class ScopedMutexLock
{
	Mutex& m_mtx;

  public:
	ScopedMutexLock(Mutex& mtx)
		: m_mtx(mtx)
	{
		m_mtx.lock();
	}

	~ScopedMutexLock()
	{
		m_mtx.unlock();
	}
};

inline void interleave(const float** in, float* out, const int channels, const int frameCount)
{
	float* p = out;
	for (int frame = 0; frame < frameCount; ++frame)
	{
		for (int chan = 0; chan < channels; ++chan)
		{
			*p = in[chan][frame];
			++p;
		}
	}
}

inline void deinterleave(const float* in, float** out, const int channels, const int frameCount)
{
	const float* p = in;
	for (int frame = 0; frame < frameCount; ++frame)
	{
		for (int chan = 0; chan < channels; ++chan)
		{
			out[chan][frame] = *p;
			++p;
		}
	}
}

/// a procedure for clearing the contents, setting to silence the contents of a buffer
inline void clear_multichannel_buffers(float** buffers, const int channels, const int frameCount)
{
	for (int n = 0; n < channels; ++n)
	{
		memset(buffers[n], 0, frameCount * sizeof(float));
	}
}

inline float remap(float r1_a, float r1_b, float r2_a, float r2_b, float in)
{

	float d1 = r1_b - r1_a;
	float d2 = r2_b - r2_a;
	assert(r1_b > r1_a);
	assert(r2_b > r2_a);
	float dv = in - r1_a;

	return dv / d1 * d2 + r2_a;
}

template <typename IntType, typename FloatType>
inline IntType truncate_to_integer(FloatType v)
{
	return static_cast<IntType>(std::floor(v));
}

/// a simple dsp phasor class for use in oscilator and control objects.
/// it provides a tick function that should be called once per sample of playback.
/// a get phase function returns the current phase of the phasor in the range 0.0f to 1.0f.
class Phasor
{
	float m_phase;
	float m_sr;
	float m_delta;
	float m_frequency;

  public:
	Phasor(float sampleRate = 44100.0f, float frequency = 1.0f, float phase = 0.0f)
		: m_phase(phase)
		, m_sr(sampleRate)
		, m_delta(frequency / sampleRate)
		, m_frequency(frequency)
	{
	}

	float get_frequency() const { return m_frequency; }

	void set_frequency(float frequency)
	{
		m_frequency = frequency;
		m_delta = frequency / m_sr;
	}
	float get_phase() const { return m_phase; }

	void set_phase(float phase)
	{
		m_phase = phase;
	}

	// call me to tick the phasor forward by one sample.
	void tick()
	{
		m_phase += m_delta;
		if (m_phase < 0.0f)
		{
			m_phase += 1.0f;
		}
		else if (m_phase >= 1.0f)
		{
			m_phase -= 1.0f;
		}
	}
};

inline float phase_wrap(float x)
{
	while (x > 1)
		x -= 1.0f;
	while (x < 0)
		x += 1.0f;
	return x;
}
//class designed to manage the creation, access to and destruction of multi-channel buffers
class MultiChannelBuffer
{
public:
	enum
	{
		kMaxChannels = 8
	};

	typedef std::vector<float> audio_data_block_t;
	typedef audio_data_block_t::size_type size_type;
	typedef uint8_t channel_index_type;

 public:
	 MultiChannelBuffer(channel_index_type channels, size_type size)
		: m_data(channels * size, 0.f)
		, m_channels(channels)
		, m_size(size)
	{
		assert(m_channels > 0 && m_channels <= kMaxChannels);
		std::fill(&m_channelPtrs[0], m_channelPtrs + kMaxChannels, nullptr);

		for (int n = 0; n < channels; n++)
		{
			m_channelPtrs[n] = &m_data[0] + (n * size);
		}
	}

	MultiChannelBuffer(const MultiChannelBuffer& op) = delete;

	MultiChannelBuffer& operator=(const MultiChannelBuffer& op) = delete;

	~MultiChannelBuffer()
	{
		std::fill(&m_channelPtrs[0], m_channelPtrs + kMaxChannels, nullptr);
		m_size = 0;
		m_channels = 0;
	}

	float** get_buffers()
	{
		return m_channelPtrs;
	}

	void clear()
	{
		std::fill(m_data.begin(), m_data.end(), 0.f);
	}

	size_type size() const { return m_size; }

	
private:
	audio_data_block_t m_data;
	size_type m_size; // size of individual buffers

	float* m_channelPtrs[kMaxChannels]; // offset pointers for each channel
	channel_index_type m_channels;
};

/// This function is a helper to aid offsetting into a multichannel buffers.
/// buffersIn is copied to offsetBuffersOut with the offset specified.
inline void get_multichannel_offset_ptrs(float* const buffersIn[], float* offsetBuffersOut[], uint8_t channels, size_t offset)
{
	for (int n = 0; n < channels; n++)
	{
		offsetBuffersOut[n] = buffersIn[n] + offset;
	}
}

class FMinMax
{
  public:
	float fmin, fmax;
	FMinMax()
		: fmin(0)
		, fmax(0)
	{
	}
	FMinMax(float a, float b)
		: fmin(a)
		, fmax(b)
	{
	}
	FMinMax(const FMinMax& other)
		: fmin(other.fmin)
		, fmax(other.fmax)
	{
	}
};

template <class T>
T wrap(T value)
{
	while (value > 1.0)
	{
		value -= 1;
	}
	while (value < 0.0)
	{
		value += 1;
	}
	return value;
}

//NEW BREAKPOINT SYSTEM

struct TVPair
{
	float t_;
	float v_;

	TVPair()
		: t_(0)
		, v_(0){};
	TVPair(float t, float v)
		: t_(t)
		, v_(v){};
	///copy constructor
	TVPair(const TVPair& old)
		: t_(old.t_)
		, v_(old.v_)
	{
	}
	bool operator<(const TVPair& op) const
	{
		return t_ < op.t_; // && v_ < op.v_;
	}
};

inline bool operator<(const TVPair& op, const float& t)
{
	return op.t_ < t;
}

inline bool operator<(const float& t, const TVPair& op)
{
	return t < op.t_;
}

class BreakPointUnit
{
	typedef std::vector<TVPair> BPList;
	BPList bpList_;

  public:
	BreakPointUnit() {}
	///Copy constructor
	BreakPointUnit(const BreakPointUnit& old)
	{
		for (BPList::size_type n = 0; n < old.bpList_.size(); ++n)
		{
			bpList_.push_back(old.bpList_.at(n));
		}
	}
	void add_breakpoint(const TVPair& tvPair)
	{
		bpList_.push_back(tvPair);
		std::sort(bpList_.begin(), bpList_.end());
	}

	float get_value(float absT) const
	{
		assert(bpList_.size() > 0);
		if (absT <= bpList_.begin()->t_)
			return bpList_.begin()->v_;
		if (absT >= bpList_.rbegin()->t_)
			return bpList_.rbegin()->v_;

		// find lower tv pair for input t value (TVPair A)
		// get next one too (TVPair B)
		// do linear interpolate based on A B and t;
		BPList::const_iterator itB = std::upper_bound(bpList_.begin(), bpList_.end(), absT);
		BPList::const_iterator itA = itB - 1;

		float t = absT - itA->t_;
		float dt = itB->t_ - itA->t_;
		float r = t / dt;
		//  fsom::DebugStream << "t = " << t << ": r=" << r << std::endl;
		return (1.0f - r) * itA->v_ + r * itB->v_;
	}

	const TVPair& get_pair(int index) const { return bpList_.at(index); }

	TVPair& get_pair(int index)
	{
		return bpList_.at(index);
	}

	void remove_pair(int index)
	{
		bpList_.erase(bpList_.begin() + index);
		sort();
	}

	void remove_pair(fsom::TVPair pair)
	{
		BPList::iterator it;
		for (it = bpList_.begin(); it != bpList_.end(); ++it)
		{
			if ((*it).t_ == pair.t_ && ((*it).v_ == pair.v_))
			{
				break;
			}
		}
		bpList_.erase(it);
		sort();
	}

	int get_list_size() { return bpList_.size(); }

	/*  TVPair get_closest_pair(float Time){
	    assert( bpList_.size() > 0 );
	//    if(  Time <= bpList_.begin()->t_ ) return bpList_.begin();
	//    if(  Time >= bpList_.rbegin()->t_ ) return bpList_.rbegin();
	    
	  BPList::iterator itB = std::upper_bound(bpList_.begin(), bpList_.end(), Time);
	  BPList::iterator itA = itB-1;
	  
	 fsom::DebugStream << "time = " << Time << std::endl;
	 fsom::DebugStream << "itB->t_ = " << itB->t_ << " : itA->t_ = " << itA->t_ << std::endl;
	 fsom::DebugStream << "itB index = " << itB - bpList_.begin() << std::endl;	  
	 
	 if((itB->t_ - Time) < (Time - itA->t_)){
	    return (*itB);
	 }else{
	    return (*itA);  
	 }
    }*/

	void print_breakpoints()
	{
		for (BPList::iterator it = bpList_.begin(); it != bpList_.end(); ++it)
		{
			fsom::DebugStream << " : time = " << it->t_ << " : val =" << it->v_ << std::endl;
		}
	}

	void sort() { std::sort(bpList_.begin(), bpList_.end()); }
	float get_last_breakpoint()
	{
		return bpList_.back().t_;
	}
	void duplicate_last_at_time(float t)
	{
		float v = bpList_.back().v_;
		TVPair pair(t, v);
		add_breakpoint(pair);
	}
};

} //fsom

#endif
