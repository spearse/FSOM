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
	
	for(int i = 0; i < get_parameter_list().size(); i++){
	    get_arrange_list().push_back(i);
	}
}

Distort::~Distort()
{

}
void Distort::process(float** input, float** output, int frameSize, int channels){
  if(!bypass_active()){
	float wetMix = get_parameter("Distortion Level")->get_value();
	float dryMix = 1.0f - wetMix;
	double N = m_table.get_size();
	for (int n = 0; n < frameSize; ++n){
		double x1 = input[0][n];
		double x2 = input[1][n];
		output[0][n] = (m_table.linear_lookup(0.5*(N*x1)+(0.5*N))*9.0) * wetMix + (input[0][n]*dryMix);
		output[1][n] = (m_table.linear_lookup(0.5*(N*x2)+(0.5*N))*9.0) * wetMix + (input[0][n]*dryMix);
	}
   }else{
    output[0] = input[0];
    output[1] = input[1];
  }		
}

}