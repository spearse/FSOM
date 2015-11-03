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


#ifndef FRAGMENT_HPP
#define FRAGMENT_HPP
#include "DSPEffect.hpp"

namespace fsom{
//DSPEffect Pointer
typedef std::shared_ptr<DSPEffect> DSPEffectPtr;
//DSPEffect function pointers
typedef DSPEffectPtr(*DSPEffectCreateFunc)();

	
class Fragment : public DSPEffect
{

public:
    Fragment(dspCreationStruct data );
    virtual ~Fragment();
    void process(float** input, float** output, int frameSize, int channels) ;
	static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new Fragment(data)) ; }
	virtual DSPEffectPtr clone() { return DSPEffectPtr( new Fragment(*this) ); }
};
}
#endif // FRAGMENT_HPP
