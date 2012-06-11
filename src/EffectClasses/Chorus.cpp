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
    m_modTable.fill_sine();
    add_parameter("Mix",0.0,1.0,0.5);
//     get_parameter("Mix")->set_meta("GuiHint","soCustomFader");
    add_parameter("Depth",0.0,1.0,0.5);
//     get_parameter("Depth")->set_meta("GuiHint","soCustomFader");
    add_parameter("Frequency",0.01,5.0,1.0);
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
      float depth;
      float mix;
      float invmix;
      float frequency;
      m_modPhasor.set_frequency( frequency );
      for(int n = 0; n < frameSize; ++n){
	
	  depth = get_parameter("Depth")->get_value() * 100.0f;
	  mix = get_parameter("Mix")->get_value();
	  invmix = 1.0f - mix;
	  frequency = get_parameter("Frequency")->get_value() *0.8 ;
	
	  float dt = m_modTable.linear_lookup( m_modPhasor.get_phase()* m_modTable.get_size()) * depth + depth + 1.0f;
	  m_delayUnitL.write_sample(input[0][n]);
	  m_delayUnitR.write_sample(input[1][n]);
	  output[0][n] = input[0][n] * invmix + m_delayUnitL.read_sample(dt) * mix;     
	  output[1][n] = input[1][n] * invmix + m_delayUnitR.read_sample(dt) * mix;
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