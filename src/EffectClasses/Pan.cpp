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



#include "fsom/EffectClasses/Pan.hpp"
#include <cmath>
#include "fsom/Utilities.hpp"
#include <iostream>

namespace fsom{

Pan::Pan(dspCreationStruct data):

	DSPEffect(data)
{
	set_effect_name("Pan");
	add_parameter("Pan",0,1.0,0.5); 
	set_meta(get_tutId(),"learning/pantree.xml");
	get_parameter("Pan")->set_meta("GuiHint","soCustomFader");
	set_implementation();
	

	get_arrange_list().push_back(0);

}

Pan::~Pan()
{
}
void Pan::process(float** input, float** output, int frameSize, int channels) {
    if(!bypass_active()){
	float val = get_parameter("Pan")->get_value() ;
	float g1 = sin( val * PI/2.0f); 
	float g0 = cos( val * PI/2.0f);
	assert(channels == 2);

	for(int n = 0; n < frameSize; ++n){
		output[0][n] = input[0][n] * g0;
		output[1][n] = input[1][n] * g1;
	}
    }else{
	    output[0] = input[0];
	    output[1] = input[1];
    }	  
}


}
