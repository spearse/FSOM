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


#ifndef __PARAMETER_HPP__
#define __PARAMETER_HPP__

#include "../MetaDataStore.hpp"
#include <boost/shared_ptr.hpp>


namespace fsom{

  class BreakPointUnit;
  class Region;
  
  typedef boost::shared_ptr<Region> RegionPtr;
  
class Parameter : public MetaDataStore {

std::string m_IDName;
float m_currentValue,m_lowerBound,m_upperBound;
BreakPointUnit* m_bpUnit;

public:

Parameter(RegionPtr parentRegion ,std::string IDName,float lowerBound,float upperBound,float value);
~Parameter();
void set_value(float value);
//void set_value(float value, double timeCode);
float get_value();
std::string get_name();
void tick();
float get_lowerBound();
float get_upperBound();
float get_range();
BreakPointUnit* get_breakpoints();
};

}
#endif
