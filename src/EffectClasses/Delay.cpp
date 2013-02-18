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



#include "../../include/fsom/EffectClasses/Delay.hpp"
#include <fsom/Region.hpp>
#include <fsom/Session.hpp>
#include <fsom/Engine.hpp>

namespace fsom{
Delay::Delay(dspCreationStruct data):
	DSPEffect(data),
	m_delayUnitL(44100*10),
	m_delayUnitR(44100*10)
{
	set_effect_name("Delay");
	set_meta(get_tutId(),"link to html");
	add_parameter("Delay Time",0.01,10.0,1.0);
// 	get_parameter("Delay Time")->set_meta("GuiHint","soCustomFader");
	add_parameter("Delay Volume",0.01,1,0.9);
// 	get_parameter("Delay Volume")->set_meta("GuiHint","soCustomFader");
	set_meta(get_tutId(),"learning/gaintree.xml");
	set_implementation();

	data.attatchedRegion->set_extension(44100*15);
}

Delay::~Delay()
{

}
void Delay::process(float** input, float** output, int frameSize, int channels){
  
	
	SamplePosition samplesRead;
	
	Session& sess = fsom::Engine::get_instance().get_active_session();
	
	if(sess.get_preview_state() == false){
	    samplesRead = get_creation_struct().attatchedRegion->get_sample_position();
	    
	}else{
	    samplesRead = sess.get_previed_playhead_value(); 
	}  
	  
  
	if(!bypass_active()){
  
	  float a,b;
	  for(int n = 0; n < frameSize; ++n){
		  m_delayUnitL.write_sample(input[0][n]);
		  m_delayUnitR.write_sample(input[1][n]);
		  a = input[0][n] + m_delayUnitL.read_sample(get_parameter("Delay Time")->get_value() * 44100);
		  b = input[1][n] + m_delayUnitR.read_sample(get_parameter("Delay Time")->get_value() * 44100);
		  output[0][n]= a;
		  output[1][n]= b;
		  m_delayUnitL.write_sample(b * get_parameter("Delay Volume")->get_value());
		  m_delayUnitR.write_sample(a * get_parameter("Delay Volume")->get_value());
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

void Delay::reset_effect(){
  m_delayUnitL.clear_buffer();
  m_delayUnitR.clear_buffer();
}

}