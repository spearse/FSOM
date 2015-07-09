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


#ifndef __AUDIOREGION_HPP__
#define __AUDIOREGION_HPP__

#include "AudioFile.hpp"
#include "Region.hpp"

namespace fsom{
  

  
class AudioRegion : public Region{
AudioFilePtr m_file; ///audio file for audio region
MultiChannelBuffer m_diskStreamBuffers; ///Multichannel Buffer for process function to use
SamplePosition m_samplePosition;
  public:
	AudioRegion(regionCreationStruct data);
	~AudioRegion();
	AudioRegion& operator =(AudioFile& rhs);
	
	void show_info();
	
	
	///audio file data is passed into buffer which is in turn passed into the process functions of regions stack of DSPEffects
	virtual void process(float** input, float** output, int frameSize, int channels);
	
	///store the region data to an xml element
	virtual void save_to_xml_node(TiXmlElement* node, bool useRelative);
	
	///
	virtual void on_region_start(SamplePosition seekTime);
	
	///returns the length of source file
	SampleLength get_file_length();
	
	///returns the path of audio file
	std::string get_file_path();
	
	///returns a reference to the audioregions audiofile
	AudioFilePtr get_audiofile();
	
	///static create function that returns a pointer to its region
	static RegionPtr create(regionCreationStruct data) { return RegionPtr(new AudioRegion(data)) ; }
	
};
} //fsom

#endif //__AUDIOREGION__
