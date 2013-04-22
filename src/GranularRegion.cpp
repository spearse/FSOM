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
#include "fsom/Engine.hpp"
#include "fsom/Session.hpp"

using namespace fsom;

GranularRegion::GranularRegion(regionCreationStruct data):
Region(data), 
// m_file(data.m_filepath),
m_diskStreamBuffers(2,4096),
// m_table(512),
m_sinTable(512),
m_counter(0),
m_fileLoaded(false)
{
  add_parameter("GrainSize",441,44100,44100);
  add_parameter("GrainPitch",0,10,1);
  add_parameter("GrainPosition",0,1,0);
  add_parameter("GrainRate",0.1,20,1);

  //for testing
  m_sinTable.fill_triangle();
  m_phasor.set_frequency(120);
  m_diskStreamBuffers.clear();

}

GranularRegion::~GranularRegion(){}


void GranularRegion::process(float** input, float** output, int frameSize, int channels){
  
    SamplePosition samplesRead;
    
    Session& sess = fsom::Engine::get_instance().get_active_session();
    
    if(sess.get_preview_state() == false){
	samplesRead = get_sample_position();
	
    }else{
	samplesRead = sess.get_previed_playhead_value(); 
    }  
    
  
// 	assert(channels == m_file.get_channels() && channels == 2);
	// make a request to the audiofile object to fill the disk stream buffers. 
// 	m_file.get_block(m_diskStreamBuffers.get_buffers(),frameSize);//TODO write granular functions
	// copy from the disk stream buffers through the DSP onto the output buffers.
	m_diskStreamBuffers.clear();
	
	m_grainStream.set_basePitch( get_parameter("GrainPitch")->get_value()  );
	m_grainStream.set_basePosition(get_parameter("GrainPosition")->get_value()    );
	m_grainStream.set_grainRate(get_parameter("GrainRate")->get_value());
	m_grainStream.set_grainSize(get_parameter("GrainSize")->get_value());
	
	
	float** t=m_diskStreamBuffers.get_buffers();	
	float v=0;
	//for testing only
	
	m_grainStream.process(t,0,frameSize);
	samplesRead += frameSize;
	 for(ParameterList::const_iterator it = get_parameter_list().begin(); it != get_parameter_list().end();++it){
	    (*it).second->tick(samplesRead);
	 }
// 	

	m_counter += frameSize;
	
	
	
	
	
	// process the fx stack from the disk buffer to the output
	process_dsp_stack(t,output,frameSize,channels);
}
/*
AudioFile& GranularRegion::get_audiofile(){
	return m_file;
}
*/



SampleLength GranularRegion::get_file_length(){
// 	return m_file.get_file_length();
      return get_duration();
}

void GranularRegion::save_to_xml_node(TiXmlElement* node){
	TiXmlElement * element = new TiXmlElement( "Region" );	
	save_meta_to_xml(element);
	save_to_region_specifics_to_existing_xml_node(element);
	element->SetAttribute("path",get_file_path().c_str());
	node->LinkEndChild( element );	

}

void GranularRegion::on_region_start(SamplePosition seekTime){
// 	m_file.seek(seekTime); // this would seek to region file offset
//   m_grainStream.reset();
    m_counter = 0;
    m_grainStream.reset();
    m_diskStreamBuffers.clear();
    
}



void GranularRegion::load_soundfile(std::string filepath){
  try{
    m_grainStream.load_soundfile(filepath);
    m_fileLoaded = true;
  }catch(...){
      m_fileLoaded = false;
  }
    
}

bool GranularRegion::get_load_state(){
    return m_fileLoaded;
}











