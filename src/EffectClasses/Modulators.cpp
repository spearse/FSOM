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



#include "../include/fsom/EffectClasses/Modulators.hpp"
#include <cmath>
#include <fsom/Region.hpp>
#include <fsom/Session.hpp>
#include <fsom/Engine.hpp>

//#include <iostream>

namespace fsom{
  
//High Amplitude Modulation Effect
HighAmpMod::HighAmpMod(dspCreationStruct data) :
	DSPEffect(data)
{
	set_effect_name("HighAmpMod");
	
	add_parameter("Frequency",10.0,15000.0,500.0); //add parameter for phasor frequency
	get_parameter("Frequency")->set_meta("GuiHint","soCustomFader");
	
	add_parameter("Amplitude",0.0,1.0,0.5); //add parameter for phasor amplitude
	get_parameter("Amplitude")->set_meta("GuiHint","soCustomFader");
	
	set_meta(get_tutId(),"link to html");
// 	get_parameter("Frequency")->set_meta("GuiHint","soCustomFader");
// 	get_parameter("Amplitude")->set_meta("GuiHint","soCustomFader");
	//set_implementation();
	m_phasor.set_frequency(get_parameter("Frequency")->get_value());
	set_implementation();
}

HighAmpMod::~HighAmpMod(){}


//Low Amplitude Modulation Process gets the phase at each sample (A-Rate)
void HighAmpMod::process(float** input, float** output, int frameSize, int channels) {
  
    SamplePosition samplesRead;
    
    Session& sess = fsom::Engine::get_instance().get_active_session();
    
    if(sess.get_preview_state() == false){
	samplesRead = get_creation_struct().attatchedRegion->get_sample_position();
	
    }else{
	samplesRead = sess.get_previed_playhead_value(); 
    }  
  
  
	if(!bypass_active()){
	  FSOM_ASSERT(channels == 2);
	  
	  for(int n = 0; n < frameSize; ++n){
		  m_phasor.set_frequency( get_parameter("Frequency")->get_value() );
		  float phasorAmp = get_parameter("Amplitude")->get_value();
	    
		  float v = sin(TWOPI*m_phasor.get_phase());
		  output[0][n] = (v*phasorAmp) * input[0][n];
		  output[1][n] = (v*phasorAmp) * input[1][n];
		  m_phasor.tick();
		  
		   for(ParameterList::const_iterator it = get_parameter_list().begin(); it != get_parameter_list().end();++it){
			(*it).second->tick(samplesRead);
		   }
		  
		   samplesRead++;
	  }

	  
	}else{
	    output[0] = input[0];
	    output[1] = input[1];
	}
	
}
  
  
//Low Amplitude Modulation Effect
LowAmpMod::LowAmpMod(dspCreationStruct data) :
	DSPEffect(data)
{
	set_effect_name("LowAmpMod");
	
	add_parameter("Frequency",0.01f, 20.0f, 1); //add parameter for phasor frequency
	get_parameter("Frequency")->set_meta("GuiHint","soCustomFader");
	
	add_parameter("Amplitude",0.0f, 1.0f, 0.5f); //add parameter for phasor amplitude
	get_parameter("Amplitude")->set_meta("GuiHint","soCustomFader");
	
	set_meta(get_tutId(),"link to html");
//  	get_parameter("Frequency")->set_meta("GuiHint","soCustomFader");
// 	get_parameter("Amplitude")->set_meta("GuiHint","soCustomFader");
	//set_implementation();
	m_phasor.set_frequency(get_parameter("Frequency")->get_value());
	
	set_implementation();
}

LowAmpMod::~LowAmpMod(){}


//Low Amplitude Modulation Process gets the phase at each sample (A-Rate)
void LowAmpMod::process(float** input, float** output, int frameSize, int channels) {
	    
  
	  SamplePosition samplesRead;
	    
	  Session& sess = fsom::Engine::get_instance().get_active_session();
	    
	  if(sess.get_preview_state() == false){
		samplesRead = get_creation_struct().attatchedRegion->get_sample_position();
	  }else{
		samplesRead = sess.get_previed_playhead_value(); 
	  }  
  
	  if(!bypass_active()){
	      FSOM_ASSERT(channels == 2);
	     
	      for(int n = 0; n < frameSize; ++n){
		       m_phasor.set_frequency( get_parameter("Frequency")->get_value() );
		      float phasorAmp = get_parameter("Amplitude")->get_value();
		
		      float v = sin(TWOPI*m_phasor.get_phase());
		      output[0][n] = (v*phasorAmp) * input[0][n];
		      output[1][n] = (v*phasorAmp) * input[1][n];
		      m_phasor.tick();
		      //get_parent_region().set_gain(v * input[0][n]);	
		      
		      for(ParameterList::const_iterator it = get_parameter_list().begin(); it != get_parameter_list().end();++it){
			    (*it).second->tick(samplesRead);
		      }
		      
		      samplesRead++;
	      }
	  }else{
	    output[0] = input[0];
	    output[1] = input[1];
	  }
	
}

///////////////////////////////////////////////////////////////  

//Tremelo Effect
Tremelo::Tremelo(dspCreationStruct data) :
	DSPEffect(data)
{
	set_effect_name("Tremelo");
	add_parameter("Frequency",0.0,10.0,5.0); //add parameter for phasor frequency
	set_meta(get_tutId(),"link to html");
	m_phasor.set_frequency(get_parameter("Frequency")->get_value());
// 	get_parameter("Freq")->set_meta("GuiHint","soCustomFader");
	//set_implementation();

}


Tremelo::~Tremelo(){}

//Tremelo Process gets the phase at each frame (K-Rate)
void Tremelo::process(float** input, float** output, int frameSize, int channels) {
	m_phasor.set_frequency(get_parameter("Frequency")->get_value());
	FSOM_ASSERT(channels == 2);
	for(int n = 0; n < frameSize; ++n){
		float v = sin(TWOPI*m_phasor.get_phase());
		output[0][n] = v * input[0][n];
		output[1][n] = v * input[1][n];
		m_phasor.tick();
	}		

}

////////////////////////////////////////////////////////////////////

//Ring Modulator Effect
RingMod::RingMod(dspCreationStruct data) :
	DSPEffect(data),
	m_table(512)
{
	set_effect_name("RingMod");
	add_parameter("Freq", 1.0f, 15000.0f, 500.0f); //add parameter for phasor frequency
	add_parameter("Multiplier", 0.01f, 2, 1);
	set_meta(get_tutId(),"link to html");
// 	get_parameter("Freq")->set_meta("GuiHint","soCustomFader");
	set_implementation();
	m_phasor.set_frequency(get_parameter("Freq")->get_value());
	m_table.fill_sine();
}

RingMod::~RingMod(){}


//Ring Mod Process gets the phase at each sample (A-Rate)
void RingMod::process(float** input, float** output, int frameSize, int channels) {
  
	SamplePosition samplesRead;
	  
	Session& sess = fsom::Engine::get_instance().get_active_session();
	  
	if(sess.get_preview_state() == false){
	      samplesRead = get_creation_struct().attatchedRegion->get_sample_position();
	}else{
	      samplesRead = sess.get_previed_playhead_value(); 
	}
  
	
	FSOM_ASSERT(channels == 2);

	for(int n = 0; n < frameSize; ++n){
		m_phasor.set_frequency(get_parameter("Freq")->get_value() * get_parameter("Multiplier")->get_value());
		float v = m_table.linear_lookup(m_phasor.get_phase() * m_table.get_size());
		output[0][n] = v * input[0][n];
		output[1][n] = v * input[1][n];
		m_phasor.tick();
		for(ParameterList::const_iterator it = get_parameter_list().begin(); it != get_parameter_list().end();++it){
			    (*it).second->tick(samplesRead);
		}
		      
		samplesRead++;
	}
	
}

/////////////////////////////////////////////////////////////////////////


HighFreqMod::HighFreqMod(dspCreationStruct data):
	DSPEffect(data)
{	  	
	set_effect_name("HighFreqMod");

}

HighFreqMod::~HighFreqMod()
{

}

void HighFreqMod::process(float** input, float** output, int frameSize, int channels){
	
}

///////////////////////////////////////////////////////////////////////////

LowFreqMod::LowFreqMod(dspCreationStruct data):
	DSPEffect(data)
{	  	
	set_effect_name("LowFreqMod");
}

LowFreqMod::~LowFreqMod()
{

}

void LowFreqMod::process(float** input, float** output, int frameSize, int channels){
	
}

}
