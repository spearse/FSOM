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



#include "../../include/fsom/EffectClasses/AsymmetricDelay.hpp"
#include <fsom/Region.hpp>

namespace fsom{
  
  
AsymmetricDelay::AsymmetricDelay(dspCreationStruct data):
  DSPEffect(data),
  m_delayUnitL(44100*10),
  m_delayUnitR(44100*10)
  {
    set_effect_name("AsymmetricDelay");
    add_parameter("First Delay Time",0,10,1);
    add_parameter("First Delay Volume",0,1,0.5);
    add_parameter("Second Delay Time",0,10,3);
    add_parameter("Second Delay Volume",0,1,0.5);
    add_parameter("Third Delay Time",0,10,5);
    add_parameter("Third Delay Volume",0,1,0.5);
    data.attatchedRegion->set_extension(44100*15);
    set_implementation();
    	for(int i = 0; i < get_parameter_list().size(); i++){
	    get_arrange_list().push_back((get_parameter_list().size()-1)-i);
	}
}
 
 
AsymmetricDelay::~AsymmetricDelay(){
      
}
 
void AsymmetricDelay::process(float** input, float** output, int frameSize, int channels) {
 if(!bypass_active()){
  
	  float a,b;
	  for(int n = 0; n < frameSize; ++n){
		  m_delayUnitL.write_sample(input[0][n]);
		  m_delayUnitR.write_sample(input[1][n]);
		  
		  
		  
		  a = input[0][n] +	( m_delayUnitL.read_sample(get_parameter("First Delay Time")->get_value() * 44100)  * get_parameter("First Delay Volume")->get_value()   )
					  +( m_delayUnitL.read_sample(get_parameter("Second Delay Time")->get_value() * 44100)  * get_parameter("Second Delay Volume")->get_value()   )
				          + ( m_delayUnitL.read_sample(get_parameter("Third Delay Time")->get_value() * 44100)  * get_parameter("Third Delay Volume")->get_value()   );
		  b = input[1][n] + 	( m_delayUnitR.read_sample(get_parameter("First Delay Time")->get_value() * 44100)  * get_parameter("First Delay Volume")->get_value()   )
					  +( m_delayUnitR.read_sample(get_parameter("Second Delay Time")->get_value() * 44100)  * get_parameter("Second Delay Volume")->get_value()   )
				          + ( m_delayUnitR.read_sample(get_parameter("Third Delay Time")->get_value() * 44100)  * get_parameter("Third Delay Volume")->get_value()   );
		  output[0][n]= a;
		  output[1][n]= b;
		  m_delayUnitL.write_sample(b*0.9);
		  m_delayUnitR.write_sample(a*0.9);
		  m_delayUnitL.tick();
		  m_delayUnitR.tick();
	  }
	}else{
	    output[0] = input[0];
	    output[1] = input[1];
	}
}
  
void AsymmetricDelay::reset_effect(){
  std::cout << "Resetting AsymmetricDelay's delay units"<<std::endl;
    m_delayUnitL.clear_buffer();
    m_delayUnitR.clear_buffer();
}

  
  
  
  
  
  
  
  
  
  
  
}