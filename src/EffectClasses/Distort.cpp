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



#include "../../include/fsom/EffectClasses/Distort.hpp"
#include <fsom/Session.hpp>
#include <fsom/Engine.hpp>

namespace fsom{
Distort::Distort(dspCreationStruct data):
	DSPEffect(data),
	m_table(512)
{
  
	set_effect_name("Distort");
	set_meta(get_tutId(),"link to html");
	add_parameter("Distortion Level",0.1,1.0,0.1);
	get_parameter("Distortion Level")->set_meta("GuiHint","soCustomFader");
	m_table.fill_clipped(0.01);
	//m_table.print_table();
	set_meta(get_tutId(),"learning/gaintree.xml");
	set_implementation();
	
}

Distort::~Distort()
{

}
void Distort::process(float** input, float** output, int frameSize, int channels){
  
  SamplePosition samplesRead;
  
  Session& sess = fsom::Engine::get_instance().get_active_session();
  
  if(sess.get_preview_state() == false){
      samplesRead = get_creation_struct().attatchedRegion->get_sample_position();
      
  }else{
      samplesRead = sess.get_previed_playhead_value(); 
  }  
  
  if(!bypass_active()){
	
	double N = m_table.get_size();
	for (int n = 0; n < frameSize; ++n){
		float wetMix = get_parameter("Distortion Level")->get_value();
		float dryMix = 1.0f - wetMix;
		
		output[0][n] = (m_table.linear_lookup(0.5*(N*input[0][n])+(0.5*N))*9.0) * wetMix + (input[0][n]*dryMix);
		output[1][n] = (m_table.linear_lookup(0.5*(N*input[1][n])+(0.5*N))*9.0) * wetMix + (input[1][n]*dryMix);
	
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

}