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


#ifndef TIMESTRETCH_HPP
#define TIMESTRETCH_HPP

#include "DSPEffect.hpp"

namespace fsom{

class Timestretch : public DSPEffect
{

public:
    Timestretch(dspCreationStruct data);
    virtual ~Timestretch();
	void process(float** input, float** output, int frameSize, int channels) ;
	static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new Timestretch(data)) ; }
	virtual DSPEffectPtr clone() { return DSPEffectPtr( new Timestretch(*this) ); }
};
}
#endif // TIMESTRETCH_HPP
