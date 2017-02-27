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
	m_delayUnitL(2048),
	m_delayUnitR(2048),
	m_modTable(512),
	m_modPhasor(44100,1)
{
    set_effect_name("Chorus");
    set_meta(get_tutId(),"link to html");
    
    m_modTable.fill_sine();
    add_parameter("Chorus Amount", 0.0f, 1.0f, 0.5f);
    get_parameter("Chorus Amount")->set_meta("GuiHint","soCustomFader");
    
    add_parameter("Depth", 0.0f, 1.0f, 0.5f);
    get_parameter("Depth")->set_meta("GuiHint","soCustomFader");
    
    add_parameter("Frequency", 0.01f, 5.0f, 1.0f);
    get_parameter("Frequency")->set_meta("GuiHint","soCustomFader");

    set_implementation();
    


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

	  frequency = get_parameter("Frequency")->get_value() * 0.8f ;
	  
      m_modPhasor.set_frequency( frequency );
      for(int n = 0; n < frameSize; ++n){
	
	  depth = get_parameter("Depth")->get_value() * 44.1;
	  mix = get_parameter("Chorus Amount")->get_value();
	  invmix = 1.0f - mix;
	  frequency = get_parameter("Frequency")->get_value() * 0.8f ;
	  m_modPhasor.set_frequency( frequency );
	
	  float fDt = (((m_modTable.linear_lookup( m_modPhasor.get_phase() * m_modTable.get_size()))*0.5)+0.5)    * depth + depth ;
	  DelayBase<float>::sample_index dt = truncate_to_integer<DelayBase<float>::sample_index>(fDt);
	  
	  output[0][n] = input[0][n] * invmix + m_delayUnitL.read_sample(dt) * mix;     
	  output[1][n] = input[1][n] * invmix + m_delayUnitR.read_sample(dt) * mix;
	  
	  m_delayUnitL.write_sample(input[0][n]);
	  m_delayUnitR.write_sample(input[1][n]);
	  
	  
	  m_modPhasor.tick();
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

void Chorus::reset_effect(){
  m_delayUnitL.clear_buffer();
  m_delayUnitR.clear_buffer();
}



}