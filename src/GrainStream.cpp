#include "../include/fsom/GrainStream.hpp"
#include "../include/fsom/Engine.hpp"
#include "../include/fsom/Session.hpp"

namespace fsom{

  
  
Grain::Grain(fsom::MultiTablePtr table):m_isDead(false),m_table(table){  
  
}

Grain::~Grain(){
  
}

bool Grain::is_dead()const {
  return m_isDead;
}
  
void Grain::kill(){  
    m_isDead=true;
}
  
  
  
  
  

GrainStream::GrainStream()
{

}

GrainStream::~GrainStream()
{

}


void GrainStream::load_soundfile(std::string filepath){
    try{
	m_table = MultiTablePtr(new MultiTableBuffer( Engine::get_instance().get_active_session().load_file_to_table(filepath))) ;
//    m_tables.front()->print_table();
    }catch(...){
    
    }
}

void GrainStream::reset(){
    m_internalClock = 0;
}

void GrainStream::set_basePitch(float in){
    m_basePitch = in;
}
void GrainStream::set_density(float in){
    m_density = in;
}
void GrainStream::set_basePosition(float in){
    m_basePosition = in;
}
void GrainStream::set_grainSize(float in){
    m_grainSize = in;
}

void GrainStream::process(float** input,float** output, int channel,int frames){
  kill_grains();
   
}




}