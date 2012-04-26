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



#include "../include/fsom/EffectClasses/Gain.hpp"

namespace fsom{

Gain::Gain(dspCreationStruct data) :
	DSPEffect(data)
{
	set_effect_name("Gain");
	add_parameter("Gain",0.0,1.0,1.0); 
	set_meta(get_tutId(),"learning/gaintree.xml");
	set_meta("Tip","Volume");
	get_parameter("Gain")->set_meta("GuiHint","soCustomFader");
	get_parameter("Gain")->set_meta("Tip","Volume Control");
	set_implementation();
}
Gain::~Gain(){

}




void Gain::process(float** input, float** output, int frameSize, int channels) {
  
	if(!bypass_active()){
  
	    float g0 = get_parameter("Gain")->get_value() ; 
	    assert(channels == 2);

	    for(int n = 0; n < frameSize; ++n){
		    output[0][n] = input[0][n] * g0;
		    output[1][n] = input[1][n] * g0;
	    }
	}else{
	    output[0] = input[0];
	    output[1] = input[1];
	}
}


}
