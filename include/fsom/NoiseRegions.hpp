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


#ifndef NOISEREGIONS_HPP
#define NOISEREGIONS_HPP

#include "NoiseRegionBase.hpp"

namespace fsom{

class WhiteNoise : public NoiseRegionBase{
	MultiChannelBuffer m_audioBuffer;
    public:
    WhiteNoise(regionCreationStruct data);
    virtual ~WhiteNoise(); 
    virtual void process(float** input, float** output, int frameSize, int channels);
//     virtual void save_to_xml_node(TiXmlElement* node);
    virtual void on_region_start(SamplePosition seekTime);
    static RegionPtr create(regionCreationStruct data) { return RegionPtr(new WhiteNoise(data)) ; }
};

class RedNoise : public NoiseRegionBase{
    public:
    RedNoise(regionCreationStruct data);
    virtual ~RedNoise(); 
    virtual void process(float** input, float** output, int frameSize, int channels);
//     virtual void save_to_xml_node(TiXmlElement* node);
    virtual void on_region_start(SamplePosition seekTime);
    static RegionPtr create(regionCreationStruct data) { return RegionPtr(new RedNoise(data)) ; }
};









  
}

#endif // NOISEREGIONS_HPP
