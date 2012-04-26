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



#include "fsom/GranularRegion.hpp"
#include "tinyxml/tinyxml.h"

using namespace fsom;

GranularRegion::GranularRegion(regionCreationStruct data):
Region(data), 
m_file(data.m_filepath),
m_diskStreamBuffers(2,4096),
m_table(0)
{
  
  
}

GranularRegion::~GranularRegion(){}


void GranularRegion::process(float** input, float** output, int frameSize, int channels){
	assert(channels == m_file.get_channels() && channels == 2);
	// make a request to the audiofile object to fill the disk stream buffers. 
	m_file.get_block(m_diskStreamBuffers.get_buffers(),frameSize);//TODO write granular functions
	// copy from the disk stream buffers through the DSP onto the output buffers.
	float** t=m_diskStreamBuffers.get_buffers();	
	// process the fx stack from the disk buffer to the output
	process_dsp_stack(t,output,frameSize,channels);
}

AudioFile& GranularRegion::get_audiofile(){
	return m_file;
}

std::string GranularRegion::get_file_path(){
	return m_file.get_file_path();	
}

SampleLength GranularRegion::get_file_length(){
	return m_file.get_file_length();
}

void GranularRegion::save_to_xml_node(TiXmlElement* node){
	TiXmlElement * element = new TiXmlElement( "GranularRegion" );	
	save_to_region_specifics_to_existing_xml_node(element);
	element->SetAttribute("path",get_file_path().c_str());
	node->LinkEndChild( element );	

}

void GranularRegion::on_region_start(SamplePosition seekTime){
	m_file.seek(seekTime); // this would seek to region file offset
}
