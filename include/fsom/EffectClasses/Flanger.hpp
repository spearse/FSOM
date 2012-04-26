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


#ifndef FLANGER_HPP
#define FLANGER_HPP

#include "DSPEffect.hpp"
#include "DelayBase.hpp"
#include "../Utilities.hpp"
#include "../Table.hpp"

namespace fsom{

class Flanger :public DSPEffect
{
	DelayBase<double> m_delayUnitL,m_delayUnitR;
	Table<double> m_table;
	Phasor m_phasor;
public:
	Flanger(dspCreationStruct data );
	virtual ~Flanger();
	void process(float** input, float** output, int frameSize, int channels) ;
	static DSPEffectPtr create(dspCreationStruct data ) { return DSPEffectPtr(new Flanger(data)) ; }
	virtual DSPEffectPtr clone() { return DSPEffectPtr( new Flanger(*this) ); }
	virtual void reset_effect();
};

}

#endif // FLANGER_HPP
