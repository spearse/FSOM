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



#include "../../include/fsom/EffectClasses/Harmonizer.hpp"

namespace fsom{
  
Harmonizer::Harmonizer(dspCreationStruct data): DSPEffect(data){
  set_effect_name("Harmonizer");
  add_parameter("Pitch One Shift Amount",-12.0,12.0,0.0);
  add_parameter("Pitch Two Shift Amount",-12.0,12.0,0.0);
  add_parameter("Pitch Three Shift Amount",-12.0,12.0,0.0);
  add_parameter("Pitch Four Shift Amount",-12.0,12.0,0.0);
  set_implementation();
}


Harmonizer::~Harmonizer(){
}

void Harmonizer::process(float** input, float** output, int frameSize, int channels){
   if(!bypass_active()){
	m_unit1.set_frequency(get_parameter("Pitch One Shift Amount")->get_value());
	m_unit2.set_frequency(get_parameter("Pitch Two Shift Amount")->get_value());
	m_unit3.set_frequency(get_parameter("Pitch Three Shift Amount")->get_value());
	m_unit4.set_frequency(get_parameter("Pitch Four Shift Amount")->get_value());
	for (int n = 0; n < frameSize; ++n){
	      m_unit1.process(input[0][n],input[1][n],output[0][n],output[1][n],0.25);
	      m_unit2.process(input[0][n],input[1][n],output[0][n],output[1][n],0.25);
	      m_unit3.process(input[0][n],input[1][n],output[0][n],output[1][n],0.25);
	      m_unit4.process(input[0][n],input[1][n],output[0][n],output[1][n],0.25);
	}
    }else{
	    output[0] = input[0];
	    output[1] = input[1];
	}
  
  
  
}
  
void Harmonizer::reset_effect(){
    m_unit1.clear_delays();
    m_unit2.clear_delays();
    m_unit3.clear_delays();
    m_unit4.clear_delays();
}
  
  
}