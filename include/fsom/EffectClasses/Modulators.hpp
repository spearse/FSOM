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


#ifndef __MODULATORS_HPP__
#define __MODULATORS_HPP__

#include "DSPEffect.hpp"
#include "../Utilities.hpp"
#include "../Table.hpp"
    
    
namespace fsom{
  
//Low Amplitude Modulator Effect
class LowAmpMod : public DSPEffect
{
  Phasor m_phasor;

public:
    LowAmpMod(dspCreationStruct data);
    ~LowAmpMod();
     void process(float** input, float** output, int frameSize, int channels) ;
     static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new LowAmpMod(data)) ; }
     virtual DSPEffectPtr clone() { return DSPEffectPtr( new LowAmpMod(*this) ); }
};  

//High Amplitude Modulator Effect
class HighAmpMod : public DSPEffect
{
  Phasor m_phasor;

public:
    HighAmpMod(dspCreationStruct data);
    ~HighAmpMod();
     void process(float** input, float** output, int frameSize, int channels) ;
     static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new HighAmpMod(data)) ; }
	virtual DSPEffectPtr clone() { return DSPEffectPtr( new HighAmpMod(*this) ); }
};

//Tremelo Effect
class Tremelo : public DSPEffect
{
  Phasor m_phasor;

public:
    Tremelo(dspCreationStruct data);
    ~Tremelo();
     void process(float** input, float** output, int frameSize, int channels) ;
     static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new Tremelo(data)) ; }
     	virtual DSPEffectPtr clone() { return DSPEffectPtr( new Tremelo(*this) ); }
};

//Ring Modulator Effect
class RingMod : public DSPEffect
{
   Phasor m_phasor;
   Table<double> m_table;
public:
    RingMod(dspCreationStruct data);
    ~RingMod();
     void process(float** input, float** output, int frameSize, int channels) ;
     static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new RingMod(data)) ; }
     virtual DSPEffectPtr clone() { return DSPEffectPtr( new RingMod(*this) ); }
};

//High Frequency Modulation
class HighFreqMod : public DSPEffect
{

public:
    HighFreqMod(dspCreationStruct data );
    virtual ~HighFreqMod();
    void process(float** input, float** output, int frameSize, int channels) ;
	static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new HighFreqMod(data)) ; }
  	virtual DSPEffectPtr clone() { return DSPEffectPtr( new HighFreqMod(*this) ); }
};

//Low Frequency Modulation
class LowFreqMod : public DSPEffect
{

public:
    LowFreqMod(dspCreationStruct data );
    virtual ~LowFreqMod();
    void process(float** input, float** output, int frameSize, int channels) ;
	static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new LowFreqMod(data)) ; }
	virtual DSPEffectPtr clone() { return DSPEffectPtr( new LowFreqMod(*this) ); }
};

}

#endif // TREMELO_HPP
