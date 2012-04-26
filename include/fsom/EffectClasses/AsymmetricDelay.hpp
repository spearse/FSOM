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


#ifndef ASSYMETRICDELAY_HPP
#define ASSYMETRICDELAY_HPP

#include "DSPEffect.hpp"
#include "DelayBase.hpp"

namespace fsom{

class AsymmetricDelay : public DSPEffect
{
  DelayBase<float> m_delayUnitL,m_delayUnitR;
  
  public:
    AsymmetricDelay(dspCreationStruct data);
    virtual ~AsymmetricDelay();
    void process(float** input, float** output, int frameSize, int channels) ;
    static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new AsymmetricDelay(data)) ; }
    virtual DSPEffectPtr clone() { return DSPEffectPtr( new AsymmetricDelay(*this) ); }
    virtual void reset_effect();  
  
};



}
#endif // ASSYMETRICDELAY_HPP
