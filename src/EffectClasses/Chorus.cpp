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



#include "../../include/fsom/EffectClasses/Chorus.hpp"
#include <fsom/Session.hpp>
#include <fsom/Engine.hpp>




namespace fsom{
	
	Chorus::Chorus(dspCreationStruct data):
	DSPEffect(data),
	m_delayUnitL(44100),
	m_delayUnitR(44100),
	m_modTable(512),
	m_modPhasor(44100,1),
	m_samplePos(0),
	m_modDelayLeft(44100,44100,0.5),
	m_modDelayRight(44100,44100,0.5)
	{
		set_effect_name("Chorus");
		set_meta(get_tutId(),"link to html");
		
		m_modTable.fill_sine();
		add_parameter("Chorus Amount", 0.0f, 1.0f, 0.5f);
		get_parameter("Chorus Amount")->set_meta("GuiHint","soCustomFader");
		
		add_parameter("Depth", 0.0f, 1.0f, 0.5f);
		get_parameter("Depth")->set_meta("GuiHint","soCustomFader");
		
		add_parameter("Frequency", 0.00f, 5.0f, 1.0f);
		get_parameter("Frequency")->set_meta("GuiHint","soCustomFader");
		
		set_implementation();
		
		m_modDelayLeft.clear();
		m_modDelayRight.clear();
		
	}
	
	float frequencyFromSpeed(float speed){
		float maxFrequency = 5.0f;
		float minFrequency = 0.002f;
		
		return (speed * (maxFrequency - minFrequency)) + minFrequency;
	}
	
	float setupDepth(float depth){
		float max = 0.1f;
		float min = 0.005f;
		
		return depth * (max - min) + min;
	}

	
	Chorus::~Chorus()
	{
		
	}
	
	void Chorus::process(float** input, float** output, int frameSize, int channels){
		
		SamplePosition samplesRead;
		
		Session& sess = fsom::Engine::get_instance().get_active_session();
		
		if(sess.get_preview_state() == false){
			samplesRead = get_creation_struct().attatchedRegion->get_sample_position();
			
		}else{
			samplesRead = sess.get_previed_playhead_value();
		}
		
		if(!bypass_active()){
			float depth(0);
			float mix(0);
			float invmix(0);
			float frequency(0);
			float delayMs(10);
			float sampleRMS = 44.1;
			frequency = frequencyFromSpeed(get_parameter("Frequency")->get_value() /5.0f);
			depth = setupDepth(get_parameter("Depth")->get_value());
			m_modPhasor.set_frequency( 1 );
			
			float outL(0),outR(0);
			float wet(0.5);
			float dry(0.5);
			
			m_modDelayLeft.set_frequency(0.67);
			m_modDelayRight.set_frequency(0.67);
			m_modDelayLeft.setDepth(0.5);
			m_modDelayRight.setDepth(0.5);
			
			
			for(int n = 0; n < frameSize; ++n){
				
				m_delayUnitL.sum_sample(input[0][n]);
				m_delayUnitR.sum_sample(input[1][n]);
				
				m_modDelayLeft.write(input[0][n]);
				m_modDelayRight.write(input[1][n]);
				float wetL = m_modDelayLeft.modulatedDelay(20);
				float wetR = m_modDelayRight.modulatedDelay(20);
				
				//feedback
		//		m_modDelayLeft.sum(wetL );
		//		m_modDelayRight.sum(wetR);
				
				outL = wetL *wet;
				outR = wetR *wet;
				outL += input[0][n] * dry;
				outR += input[1][n] * dry;
				
				output[0][n] += outL;
				output[1][n] += outR;
				
				m_modDelayLeft.tick();
				m_modDelayRight.tick();
				

				
				/*
				 
				 depth = get_parameter("Depth")->get_value() * 441;
				 mix = get_parameter("Chorus Amount")->get_value();
				 invmix = 1.0f - mix;
				 frequency = get_parameter("Frequency")->get_value() * 0.8f ;
				 
			
				invmix = 0 ;
				mix = 0.5;
				depth = 441 ;
				
				frequency = frequencyFromSpeed(get_parameter("Frequency")->get_value() / 5.0f);
				depth = setupDepth(get_parameter("Depth")->get_value());
				
				//		  float fDt = (((m_modTable.linear_lookup( m_modPhasor.get_phase() * m_modTable.get_size()))*0.5)+0.5)    * depth  ;
				float phase =  (1.0f+  sin((6.28318531 * frequency * m_samplePos)/44100.0f))*0.5f  ;
				float start = delayMs + minDelayMS;
				delayMs = depth * (phase * (maxDelayMS - minDelayMS) );
				
				
				output[0][n] += (input[0][n] * invmix) + (m_delayUnitL.read_sample(delayMs ) * mix);
				output[1][n] += (input[1][n] * invmix) + (m_delayUnitR.read_sample(delayMs ) * mix);
				//	  std::cout << " " <<phase << " ";
				
				
				m_modPhasor.tick();
				m_delayUnitL.tick();
				m_delayUnitR.tick();
				*/
				for(ParameterList::const_iterator it = get_parameter_list().begin(); it != get_parameter_list().end();++it){
					(*it).second->tick(samplesRead);
				}
				m_samplePos++;
				samplesRead++;
			}
		}else{
			output[0] = input[0];
			output[1] = input[1];
		}
	}
	
	
	void Chorus::reset_effect(){
  m_delayUnitL.clear_buffer();
  m_delayUnitR.clear_buffer();
		
		m_modDelayRight.clear();
		m_modDelayLeft.clear();
	}
	
}


