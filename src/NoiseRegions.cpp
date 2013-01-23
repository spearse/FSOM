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



#include "../include/fsom/NoiseRegions.hpp"

namespace fsom {

WhiteNoise::WhiteNoise(regionCreationStruct data) :
        NoiseRegionBase(data),
        m_audioBuffer(2,4096)
{
	set_offset_amount(0);
	set_meta("RegionType","WhiteNoise");	

}

WhiteNoise::~WhiteNoise() {}

void WhiteNoise::process(float** input, float** output, int frameSize, int channels) {
    float val;
    
    float** t = m_audioBuffer.get_buffers();
    for (int n = 0; n < frameSize;++n) {
        val = 0.2 * ((rand()%100)/50)-1.0f;
        t[0][n] = val;
        t[1][n] = val;
    }
    //FIXME
    process_dsp_stack(t,output,frameSize,channels);
}

void WhiteNoise::on_region_start(fsom::SamplePosition seekTime) {
  reset_all_effects();
}


RedNoise::RedNoise(regionCreationStruct data) :
        NoiseRegionBase(data)
{
//        get_effect("LowPassFilter")->get_parameter("Frequency")-

}

RedNoise::~RedNoise() {}

void RedNoise::process(float** input, float** output, int frameSize, int channels) {
    float val;
    for (int n = 0; n < frameSize;++n) {

        val = ((rand()%101)/50)-1.0f;
        output[0][n] = val;
        output[1][n] = val;
    }

}


void RedNoise::on_region_start(fsom::SamplePosition seekTime) {}








}
