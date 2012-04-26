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


#ifndef NOISEREGIONBASE_HPP
#define NOISEREGIONBASE_HPP

#include "Region.hpp"
#include "Table.hpp"

namespace fsom{

class NoiseRegionBase : public fsom::Region
{
public:
    NoiseRegionBase(regionCreationStruct data);
    virtual ~NoiseRegionBase(); 
    virtual void on_region_start(SamplePosition seekTime) = 0;
    virtual void save_to_xml_node(TiXmlElement* node) ;
    virtual void process(float** input, float** output, int frameSize, int channels)=0;
};


}


#endif // NOISEREGIONBASE_HPP
