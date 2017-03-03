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
	m_modDelayLeft(44100,44100,0.5),
	m_modDelayRight(44100,44100,0.5)
	{
		set_effect_name("Chorus");
		set_meta(get_tutId(),"link to html");
		
		add_parameter("Chorus Amount", 0.0f, 1.0f, 0.5f);
		m_amountParam = get_parameter("Chorus Amount");
		
		
		add_parameter("Depth", 0.0f, 1.0f, 0.5f);
		m_depthParam = get_parameter("Depth");
		
		add_parameter("Frequency", 0.00f, 5.0f, 1.0f);
		m_frequencyParam = get_parameter("Frequency");
		
		set_implementation();
		
		m_modDelayLeft.clear();
		m_modDelayRight.clear();
		
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
			
			
			float f = m_frequencyParam->get_value_zero_one();
			float d = m_depthParam->get_value_zero_one();
			float wet = m_amountParam->get_value_zero_one();
			
			float outL(0),outR(0);
			float dry(0.5);
			
			
			
			for(int n = 0; n < frameSize; ++n){
				f = m_frequencyParam->get_value_zero_one();
				d = m_depthParam->get_value_zero_one();
				wet = m_amountParam->get_value_zero_one();
				dry = 1.0f - wet;
				m_modDelayLeft.set_frequency(f);
				m_modDelayRight.set_frequency(f);
				m_modDelayLeft.setDepth(d);
				m_modDelayRight.setDepth(d);
				
				
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
	
	
	void Chorus::reset_effect(){
  		
		m_modDelayRight.clear();
		m_modDelayLeft.clear();
	}
	
}


