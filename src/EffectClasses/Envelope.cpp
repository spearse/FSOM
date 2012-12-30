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



#include "../../include/fsom/EffectClasses/Envelope.hpp"
#include <fsom/Region.hpp>
#include <fsom/Engine.hpp>
#include <fsom/Session.hpp>

namespace fsom{
Envelope::Envelope(dspCreationStruct data) :
	DSPEffect(data), m_currentAmp(0.0)

{
	const SampleLength regionDuration(data.attatchedRegion->get_duration());
  	set_effect_name("Envelope"); //individual effect name set 
	
	  set_meta(get_tutId(),"link to html");
	  
	  add_parameter("Attack Time(ms)",10,regionDuration/44.1,10);
	  get_parameter("Attack Time(ms)")->set_meta("GuiHint","soCustomFader");
	
	  add_parameter("Decay Time(ms)",10,regionDuration/44.1,(regionDuration/44.1)*0.25);
	  get_parameter("Decay Time(ms)")->set_meta("GuiHint","soCustomFader");
	  
	  add_parameter("Release Time(ms)",10,regionDuration/44.1,(regionDuration/44.1)*0.8);
	  get_parameter("Release Time(ms)")->set_meta("GuiHint","soCustomFader");
	
	  add_parameter("Sustain Amplitude",0.1,1.0,1.0);
	  get_parameter("Sustain Amplitude")->set_meta("GuiHint","soCustomFader");
      
	  m_fadeUnit.add_breakpoint(TVPair(0, 0));
	  m_fadeUnit.add_breakpoint(TVPair(get_parameter("Attack Time(ms)")->get_value()*44.1, 1.0));
	  m_fadeUnit.add_breakpoint(TVPair(get_parameter("Decay Time(ms)")->get_value()*44.1, 
					   get_parameter("Sustain Amplitude")->get_value()));
	  m_fadeUnit.add_breakpoint(TVPair(get_parameter("Release Time(ms)")->get_value()*44.1,
					   get_parameter("Sustain Amplitude")->get_value()));
	  m_fadeUnit.add_breakpoint(TVPair(regionDuration, 0));
	  
	  set_implementation();
}

Envelope::~Envelope()
{

}

void  Envelope::process(float** input, float** output, int frameSize, int channels) {
	
  SamplePosition samplesRead;

  fsom::Session& sess = fsom::Engine::get_instance().get_active_session();
  
  if(sess.get_preview_state() == false){
      samplesRead = get_creation_struct().attatchedRegion->get_sample_position();
  }else{
    
      samplesRead = sess.get_previed_playhead_value(); 
  }
  

  if(!bypass_active()){
    
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

void Envelope::reset_effect(){
    m_fadeUnit.get_pair(1).t_ = get_parameter("Attack Time(ms)")->get_value()*44.1;
    m_fadeUnit.get_pair(2) = TVPair(get_parameter("Decay Time(ms)")->get_value()*44.1,
			     get_parameter("Sustain Amplitude")->get_value());
    m_fadeUnit.get_pair(3) = TVPair(get_parameter("Release Time(ms)")->get_value()*44.1,
			     get_parameter("Sustain Amplitude")->get_value());
    m_fadeUnit.sort();
}

float Envelope::get_amplitude_snapshot(){
 return m_currentAmp;
}

}