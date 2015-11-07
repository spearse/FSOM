#include "../include/fsom/GrainStream.hpp"
#include "../include/fsom/Engine.hpp"
#include "../include/fsom/Session.hpp"
#include <algorithm>
#include <functional>
//#include <lo/lo_osc_types.h>
#include "../include/fsom/Table.hpp"
namespace fsom{

  
  
Grain::Grain(TablePtr window,   MultiTablePtr table,int dur,int position, float pitch,float amp):
  m_isDead(false),
  m_table(table),
  m_window(window),
  m_basePosition(position),
  m_basePitch(pitch),
  m_dur(dur),
  m_internalClock(0),
  m_phasor(44100),
  m_amp(amp)
{  
  //phasor freqency---- example ........ dur = 44100 .... 1... second  
  m_phasor.set_frequency(dur/44100.0f);
}

Grain::~Grain(){
  
}

bool Grain::is_dead()const {
  return m_isDead;
}
  
void Grain::kill(){  
    m_isDead=true;
//      fsom::DebugStream << "Killed grain"<<std::endl;
}
 
void Grain::process(float** outs, int start, int length){
    //TODO
    if(!m_isDead){
      float audioSize = m_dur * m_basePitch;
      for(int n = 0; n < length;++n){
	float phase =  phase_wrap(  m_phasor.get_phase() + float(n));
	float gain = m_window->linear_lookup(m_phasor.get_phase()* m_window->get_size());
	//MONO for time being...

	//TODO causing bombing on windows need alternative to at() default bounds checking
	float v = (m_table->at(0)->linear_lookup( m_basePosition+(m_internalClock *m_basePitch )) *   m_window->linear_lookup(m_phasor.get_phase() * m_window->get_size()  ))  * m_amp  ;
		
	  outs[0][n] += v ;
	  outs[1][n] += v;
	  m_phasor.tick();
	  m_internalClock +=1;
	  if(m_internalClock > m_dur)kill();	  
      }
      
    }
}
  
  /*
  

GrainStream::GrainStream():
  m_basePitch(1),
  m_density(2),
  m_basePosition(0),
  m_grainSize(44100),
  m_nextSpawn(44100),
  m_grainRate(1),
  m_window(TablePtr(new Table<float>(512))),
  m_filepath("")
{
  m_window->fill_hann();
//   spawn();
  
  TablePtr t1 = TablePtr(new Table<float>(44100));
  TablePtr t2 = TablePtr(new Table<float>(44100));
  MultiTablePtr mt(new MultiChannelBuffer(TablePtr(new Table<float>(44100)));
  mt->push_back(t1);
  mt->push_back(t2);
  MultiTablePtr mt;
  TablePtr t1 = TablePtr(new Table<float>(44100*5));
  TablePtr t2 = TablePtr(new Table<float>(44100*5));
  m_table = MultiTablePtr(new MultiTableBuffer());
  m_table->push_back(t1);
  m_table->push_back(t2);
}

GrainStream::~GrainStream()
{

}


void GrainStream::load_soundfile(std::string filepath){
    try{
	m_table = MultiTablePtr(new MultiTableBuffer( Engine::get_instance().get_active_session().load_file_to_table(filepath))) ;
	m_filepath = filepath;
//    m_tables.front()->print_table();
    }catch(...){
    
    }
}

std::string GrainStream::get_soundfile(){
	return m_filepath;
}

void GrainStream::reset(){
    m_internalClock = 0;
    m_nextSpawn = 0;
    kill_grains();
}

void GrainStream::set_basePitch(float in){
    m_basePitch = in;
}
void GrainStream::set_density(float in){
    m_density = in;
}
void GrainStream::set_basePosition(float in){
    m_basePosition = in* m_table->at(0)->get_size();
}
void GrainStream::set_grainSize(float in){
    m_grainSize = in;
}

void GrainStream::set_nextSpawn(int nextSpawn){
  m_nextSpawn = nextSpawn;
}

void GrainStream::set_grainRate(float rate){
  m_grainRate = rate;
}

void GrainStream::process(float** output, int channels, int frames){
  int remainder = frames;
  int start = 0;
  
  while(remainder > 0){
//        fsom::DebugStream << m_nextSpawn<<std::endl;
      kill_grains();
      while(m_nextSpawn <= 0){
	  spawn();  
      }
      
      if (remainder > m_nextSpawn) {
	
	  
			// partial process
			
			std::for_each(
				m_grains.begin(),m_grains.end(),
				std::bind(&Grain::process,_1,output,start,m_nextSpawn)
			);
			
			start += m_nextSpawn;
			remainder -= m_nextSpawn;
			m_nextSpawn = 0;
      } else {
			// remain process
			
			std::for_each(
				m_grains.begin(),m_grains.end(),
				std::bind(&Grain::process,_1,output,start,remainder)
			);
			m_nextSpawn -= remainder;
			remainder = 0;
      }
      
  }
  
}


void GrainStream::spawn(){
    float dur = 44100.0f/m_grainRate;
    m_nextSpawn = dur;
  
    m_grains.push_back(GrainPtr(new Grain(m_window,m_table,m_grainSize,m_basePosition,m_basePitch)));
//      fsom::DebugStream << "Spawned " << m_grains.size()<<std::endl; 
    
}

bool killPredicate(GrainPtr& grain){
//    fsom::DebugStream << "Pred "<< grain->is_dead()<<std::endl;
    return grain->is_dead();
}

void GrainStream::kill_grains(){
//     std::remove_if( m_grains.begin(),m_grains.end(),killPredicate);
    for(int n =0; n < m_grains.size();++n){
	if(m_grains.at(n)->is_dead()){
	    m_grains.at(n).reset();
	    m_grains.erase(m_grains.begin()+n);
	}
      
    }
//      fsom::DebugStream << m_grains.size() << "grains size " << std::endl;
//      fsom::DebugStream << "KILLING"<<std::endl;
    
}

*/


}