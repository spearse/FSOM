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


#ifndef GRANULATE_HPP
#define GRANULATE_HPP

#include "DSPEffect.hpp"


namespace fsom{

class Granulate : public DSPEffect
{

public:
    Granulate(dspCreationStruct data);
    virtual ~Granulate();
        void process(float** input, float** output, int frameSize, int channels) ;
	static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new Granulate(data)) ; }
	virtual DSPEffectPtr clone() { return DSPEffectPtr( new Granulate(*this) ); }
};

}
#endif // GRANULATE_HPP
