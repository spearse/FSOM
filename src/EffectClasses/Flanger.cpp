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



#include "../../include/fsom/EffectClasses/Flanger.hpp"
#include <fsom/Session.hpp>
#include <fsom/Engine.hpp>

namespace fsom{
Flanger::Flanger(dspCreationStruct data):
	DSPEffect(data),
	m_delayUnitL(2048),
	m_delayUnitR(2048),
	m_table(512),
	m_phasor(44100,2),
	m_lfoValue(0)
{
	set_effect_name("Flanger");
	m_table.fill_sine();
	add_parameter("Flanger Amount",0.0f, 1.0f, 0.5f);
	get_parameter("Flanger Amount")->set_meta("GuiHint","soCustomFader");
	
	add_parameter("Depth",0.0f, 1.0f, 0.5f);
	get_parameter("Depth")->set_meta("GuiHint","soCustomFader");
	
	add_parameter("Frequency", 0.01f, 8.0f, 0.1f);
	get_parameter("Frequency")->set_meta("GuiHint","soCustomFader");
	
	add_parameter("Feedback", 0.0f, 1.0f, 0.6f);
	get_parameter("Feedback")->set_meta("GuiHint","soCustomFader");
	
	set_implementation();
	
}

Flanger::~Flanger()
{

}
void Flanger::process(float** input, float** output, int frameSize, int channels) {
  
      SamplePosition samplesRead;
    
      Session& sess = fsom::Engine::get_instance().get_active_session();
      
      if(sess.get_preview_state() == false){
	  samplesRead = get_creation_struct().attatchedRegion->get_sample_position();
	  
      }else{
	  samplesRead = sess.get_previed_playhead_value(); 
      } 
  
    if(!bypass_active()){
	  static const float offset = 882.0f;
	  float S = static_cast<float>(m_table.get_size());
	  float depth;
	  float feedback;
	  float dry;
	  float wet;
	  float hdepth;
		
	  for (int n = 0; n < frameSize; ++n){
		  m_phasor.set_frequency(get_parameter("Frequency")->get_value());
		  depth = get_parameter("Depth")->get_value() * offset;
		  hdepth = depth*0.5;
		  feedback = get_parameter("Feedback")->get_value();
		  wet = get_parameter("Flanger Amount")->get_value();
		  dry = 1.0f - wet;
		  
		  m_lfoValue = m_table.linear_lookup((m_phasor.get_phase()*S)-1);
		  DelayBase<float>::sample_index dt = truncate_to_integer<DelayBase<float>::sample_index>(     ( m_lfoValue  +hdepth) *hdepth   );
		  output[0][n] = (m_delayUnitL.read_sample(dt) * wet) + (input[0][n]*dry);
		  output[1][n] = (m_delayUnitR.read_sample(dt) * wet) + (input[1][n]*dry);
		  
		  m_delayUnitL.write_sample((output[0][n]*feedback)+ input[0][n]);
		  
		  m_delayUnitR.write_sample((output[1][n]*feedback) + input[1][n]);
		  m_phasor.tick();
		  m_delayUnitL.tick();
		  m_delayUnitR.tick();
		  
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

void Flanger::reset_effect(){
  m_delayUnitL.clear_buffer();
  m_delayUnitR.clear_buffer();
}







}

