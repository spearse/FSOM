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


#ifndef TRANSPOSEUNIT_HPP
#define TRANSPOSEUNIT_HPP

#include "../Table.hpp"
#include "DelayBase.hpp"
#include "../Utilities.hpp"

namespace fsom{

class TransposeUnit 
{
  const int m_delaySize;
  DelayBase<float> m_delayUnitL,m_delayUnitR;
  Table<float> m_table;
  Phasor m_phasor;
  float m_proportion;
  
  public:
    TransposeUnit();
    virtual ~TransposeUnit();
    void process(float& inL, float& inR, float& outL, float& outR,float amp,bool replace=false);
    void process_replace(float& inL, float& inR, float& outL, float& outR);
    void set_frequency(float value);
    void clear_delays();
};
}
#endif // TRANSPOSEUNIT_HPP
