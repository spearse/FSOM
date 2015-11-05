#include "../../include/fsom/EffectClasses/FrequencyModulation.hpp"
#include <fsom/Session.hpp>
#include <fsom/Engine.hpp>
namespace fsom{

LowFrequencyModulation::LowFrequencyModulation(dspCreationStruct creationStruct) :
  DSPEffect(creationStruct),
  m_table(512),
  m_phasor(44100.0)
{
  m_phasor.set_frequency(1);
  set_effect_name("LowFrequencyModulation");

  add_parameter("Modulation Amount",0,12,1);  
  add_parameter("Modulation Speed",0.001f,10,0.1f);
  
  set_implementation();
}

LowFrequencyModulation::~LowFrequencyModulation(){
  
}


void LowFrequencyModulation::process(float** input, float** output, int frameSize, int channels){
  
     SamplePosition samplesRead;
      
    Session& sess = fsom::Engine::get_instance().get_active_session();
      
    if(sess.get_preview_state() == false){
	  samplesRead = get_creation_struct().attatchedRegion->get_sample_position();
    }else{
	  samplesRead = sess.get_previed_playhead_value(); 
    }
  
    if(!bypass_active()){
      float v =0 ;
      for(int n = 0; n < frameSize;++n){
	  m_phasor.set_frequency( get_parameter("Modulation Speed")->get_value() );
	  
	  v = m_table.linear_lookup( m_phasor.get_phase()*m_table.get_size()   )* get_parameter("Modulation Amount")->get_value(); 
	
	   m_transposeUnit.set_frequency(v);
	   m_transposeUnit.process_replace(input[0][n],input[1][n],output[0][n],output[1][n]);
	      
	   
	      for(ParameterList::const_iterator it = get_parameter_list().begin(); it != get_parameter_list().end();++it){
		  (*it).second->tick(samplesRead);
	      }
	m_phasor.tick();
	samplesRead++;
      }
      
      
    }else{
      output[0] = input[0];
      output[1] = input[1];
    } 
  
}
  
  
void LowFrequencyModulation::reset_effect(){
    m_transposeUnit.clear_delays();
}
  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  HighFrequencyModulation::HighFrequencyModulation(dspCreationStruct creationStruct) :
  DSPEffect(creationStruct),
  m_table(512),
  m_phasor(44100.0)
{
  m_phasor.set_frequency(1);
  set_effect_name("HighFrequencyModulation");

  add_parameter("Modulation Amount",0,100,50);  
  add_parameter("Modulation Speed",10,1000,200);
  
  set_implementation();
}

HighFrequencyModulation::~HighFrequencyModulation(){
  
}


void HighFrequencyModulation::process(float** input, float** output, int frameSize, int channels){
  
     SamplePosition samplesRead;
      
    Session& sess = fsom::Engine::get_instance().get_active_session();
      
    if(sess.get_preview_state() == false){
	  samplesRead = get_creation_struct().attatchedRegion->get_sample_position();
    }else{
	  samplesRead = sess.get_previed_playhead_value(); 
    }
  
    if(!bypass_active()){
      float v =0 ;
      for(int n = 0; n < frameSize;++n){
	  m_phasor.set_frequency( get_parameter("Modulation Speed")->get_value() );
	  
	  v = m_table.linear_lookup( m_phasor.get_phase()*m_table.get_size()   )* get_parameter("Modulation Amount")->get_value(); 
	
	   m_transposeUnit.set_frequency(v);
	   m_transposeUnit.process_replace(input[0][n],input[1][n],output[0][n],output[1][n]);
	      
	   
	      for(ParameterList::const_iterator it = get_parameter_list().begin(); it != get_parameter_list().end();++it){
		  (*it).second->tick(samplesRead);
	      }
	m_phasor.tick();
	samplesRead++;
      }
      
      
    }else{
      output[0] = input[0];
      output[1] = input[1];
    } 
  
}
  
  
void HighFrequencyModulation::reset_effect(){
    m_transposeUnit.clear_delays();
}
  
  
  
  
  
  
  
  
  
}