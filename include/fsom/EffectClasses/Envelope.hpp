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


#ifndef ENVELOPE_HPP
#define ENVELOPE_HPP

#include "DSPEffect.hpp"

namespace fsom{
	//DSPEffect Pointer
typedef boost::shared_ptr<DSPEffect> DSPEffectPtr;
//DSPEffect function pointers
typedef DSPEffectPtr(*DSPEffectCreateFunc)();

	
class Envelope : public DSPEffect
{

  float m_currentAmp;
  BreakPointUnit m_fadeUnit;
  
public:
    Envelope(dspCreationStruct data); //constructor
    virtual ~Envelope(); // destructor
        void process(float** input, float** output, int frameSize, int channels) ; //process function for ADSR envelope
	static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new Envelope(data)) ; }
	virtual DSPEffectPtr clone() { return DSPEffectPtr( new Envelope(*this) ); }
	virtual void reset_effect();
	float get_amplitude_snapshot();
};


}
#endif // ADSRENVELOPE_HPP
