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



#include "../../include/fsom/EffectClasses/Fade.hpp"
#include <fsom/Region.hpp>
#include <iostream>
#include <fsom/Engine.hpp>
#include <fsom/Session.hpp>

namespace fsom{
Fade::Fade(dspCreationStruct data):
	DSPEffect(data), m_currentAmp(0.0)
{
  m_duration = data.attatchedRegion->get_duration();
  set_effect_name("Fade");

  set_meta(get_tutId(),"link to html");
  add_parameter("Fade In Time",0.1,m_duration/44100.0,1.0);
  
  add_parameter("Fade Out Time",0.1,m_duration/44100.0,2.0);
    
   m_fadeUnit.add_breakpoint(TVPair(0, 0));
   m_fadeUnit.add_breakpoint(TVPair(get_parameter("Fade In Time")->get_value()*44100.0, 1.0));
   m_fadeUnit.add_breakpoint(TVPair(get_parameter("Fade Out Time")->get_value()*44100.0, 1.0));
   m_fadeUnit.add_breakpoint(TVPair(m_duration, 0));
  

    get_arrange_list().push_back(1);
    get_arrange_list().push_back(0);
   
   set_implementation();
}

Fade::~Fade()
{
}

void Fade::process(float** input, float** output, int frameSize, int channels) { 
  
  SamplePosition samplesRead;
   
  fsom::Session& sess = fsom::Engine::get_instance().get_active_session();
  
  if(sess.get_preview_state() == false){
      samplesRead = get_creation_struct().attatchedRegion->get_sample_position();
      
  }else{
      samplesRead = sess.get_previed_playhead_value(); 
  }
 // std::cout << samplesRead <<std::endl;
  if(!bypass_active()){
       m_fadeUnit.get_pair(1).t_ = get_parameter("Fade In Time")->get_value()*44100.0;
       m_fadeUnit.get_pair(2).t_ = m_duration - (get_parameter("Fade Out Time")->get_value()*44100.0);
      for(int n = 0; n < frameSize; ++n){ 
	
	    m_currentAmp = m_fadeUnit.get_value(samplesRead);
	    
	    output[0][n] = input[0][n] * m_currentAmp;     
	    output[1][n] = input[1][n] * m_currentAmp;
	    
	    samplesRead++;
      }
      
  }else{
    output[0] = input[0];
    output[1] = input[1];
  }
  
}

void Fade::reset_effect(){
    m_fadeUnit.get_pair(1).t_ = get_parameter("Fade In Time")->get_value()*44100.0;
    m_fadeUnit.get_pair(2).t_ = get_parameter("Fade Out Time")->get_value()*44100.0;
    m_fadeUnit.sort();
}

float Fade::get_amplitude_snapshot(){
 return m_currentAmp;	
}

}