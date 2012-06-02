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



#include "../../include/fsom/EffectClasses/Transpose.hpp"
#include <cmath>
#include <fsom/Session.hpp>
#include <fsom/Engine.hpp>

namespace fsom{

Transpose::Transpose(dspCreationStruct data):
	DSPEffect(data)
{
	set_effect_name("Transpose");
	add_parameter("Pitch Shift Amount",-12.0,12.0,1.0);
// 	get_parameter("Pitch Shift Amount")->set_meta("GuiHint","soCustomFader");
	set_implementation();
}

Transpose::~Transpose()
{

}
void Transpose::process(float** input, float** output, int frameSize, int channels) {
  
    SamplePosition samplesRead;
      
    Session& sess = fsom::Engine::get_instance().get_active_session();
      
    if(sess.get_preview_state() == false){
	  samplesRead = get_creation_struct().attatchedRegion->get_sample_position();
    }else{
	  samplesRead = sess.get_previed_playhead_value(); 
    }
  
  
    if(!bypass_active()){
  
	static const float Factor = log(pow(2.0,1.0/12.0));
// 	m_phasor.set_frequency(  (((pow(2.0f, get_parameter("Pitch Shift Amount")->get_value()/12.0f)-1)*-1.0f) *m_proportion  )); //TODO Why does this need to be inverted?
	
	m_transposeUnit2.set_frequency(12);
	float valL,valR;
	for (int n = 0; n < frameSize; ++n){
	      m_transposeUnit.set_frequency(get_parameter("Pitch Shift Amount")->get_value());
	      m_transposeUnit.process_replace(input[0][n],input[1][n],output[0][n],output[1][n]);
	      
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

void Transpose::reset_effect(){
  m_transposeUnit.clear_delays();
  m_transposeUnit2.clear_delays();
}


}