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



#include "../include/fsom/SubtractiveSynthesisRegion.hpp"

namespace fsom{
	
SubtractiveSynthesisRegion::SubtractiveSynthesisRegion(regionCreationStruct _struct):
	SynthesisRegion(_struct)
{
	set_offset_amount(0);
	set_meta("RegionType","SubtractiveSynthesis");	
	add_module("FilterModule");
	add_generator(GeneratorPtr(new Generator(Generator::GEN_Noise)));
}

SubtractiveSynthesisRegion::~SubtractiveSynthesisRegion()
{

}


}

