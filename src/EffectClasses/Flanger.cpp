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



#include "../../include/fsom/EffectClasses/Flanger.hpp"

namespace fsom{
Flanger::Flanger(dspCreationStruct data):
	DSPEffect(data),
	m_delayUnitL(2048),
	m_delayUnitR(2048),
	m_table(512),
	m_phasor(44100,2)
{
    set_effect_name("Flanger");
	m_table.fill_sine();
	add_parameter("DryMix",0.0,1.0,0.5);
	get_parameter("DryMix")->set_meta("GuiHint","soCustomFader");
	add_parameter("Depth",0.0,1.0,0.5);
	get_parameter("Depth")->set_meta("GuiHint","soCustomFader");
	add_parameter("Frequency",0.01,8.0,1.0);
	get_parameter("Frequency")->set_meta("GuiHint","soCustomFader");
	add_parameter("Feedback",0.0,1.0,0.2);
	get_parameter("Feedback")->set_meta("GuiHint","soCustomFader");
	set_implementation();
	
}

Flanger::~Flanger()
{

}
void Flanger::process(float** input, float** output, int frameSize, int channels) {
    if(!bypass_active()){
	  static const double offset = 882.0;
	  double S = m_table.get_size();
	  m_phasor.set_frequency(get_parameter("Frequency")->get_value());
	  double depth = get_parameter("Depth")->get_value() * offset-10;
	  double feedback = get_parameter("Feedback")->get_value();
	  double dry = get_parameter("DryMix")->get_value();
	  double wet = 1.0f - dry;
	  for (int n = 0; n < frameSize; ++n){
		  double dt = offset+ m_table.linear_lookup(m_phasor.get_phase()*S) * depth;
		  output[0][n] = m_delayUnitL.read_sample(dt) * wet + (input[0][n]*dry);	
		  output[1][n] = m_delayUnitR.read_sample(dt) * wet + (input[1][n]*dry);
		  m_delayUnitL.write_sample(output[0][n]*feedback + input[0][n]);
		  m_delayUnitR.write_sample(output[1][n]*feedback + input[1][n]);
		  m_phasor.tick();
		  m_delayUnitL.tick();
		  m_delayUnitR.tick();
	  }
    }else{
	    output[0] = input[0];
	    output[1] = input[1];
	}
}

void Flanger::reset_effect(){
  m_delayUnitL.clear_buffer();
  m_delayUnitR.clear_buffer();
}







}

