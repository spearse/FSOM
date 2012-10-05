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


#ifndef HARMONIZER_HPP
#define HARMONIZER_HPP


#include "DSPEffect.hpp"
#include "TransposeUnit.hpp"

namespace fsom{

class Harmoniser : public DSPEffect
{
	
	TransposeUnit m_unit1,m_unit2,m_unit3,m_unit4;
	
public:
	Harmoniser(dspCreationStruct data);
	virtual ~Harmoniser();
	void process(float** input, float** output, int frameSize, int channels) ;
	static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new Harmoniser(data)) ; } 
	virtual DSPEffectPtr clone() { return DSPEffectPtr( new Harmoniser(*this) ); }
	virtual void reset_effect();
};
}
#endif // HARMONIZER_HPP
