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



#include "../../include/fsom/EffectClasses/Reverb.hpp"
#include <fsom/Region.hpp>
namespace fsom{
Reverb::Reverb(dspCreationStruct data):
	DSPEffect(data)
{
  set_effect_name("Reverb");
  add_parameter("Roomsize",0,1,0.5);
  add_parameter("Damping",0,1,0.5);
  add_parameter("Dry/Wet",0,1,0.8);
  add_parameter("Mode",0,1,0.1);
  set_implementation();

  data.attatchedRegion->set_extension(44100*10);
  
  for(int i = 0; i < get_parameter_list().size(); i++){
	      get_arrange_list().push_back(i);
  }
  
}

Reverb::~Reverb()
{

}

void Reverb::process(float** input, float** output, int frameSize, int channels) {
// 	    m_revUnit.
    m_revUnit.setdamp(get_parameter("Damping")->get_value());
    m_revUnit.setdry(1.0f -  get_parameter("Dry/Wet")->get_value());
    m_revUnit.setwet(get_parameter("Dry/Wet")->get_value());
    m_revUnit.setroomsize(get_parameter("Roomsize")->get_value());
    m_revUnit.setmode(get_parameter("Mode")->get_value());
    
    m_revUnit.processreplace(input[0],input[1],output[0],output[1],frameSize,1);
}

}