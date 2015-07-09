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



#include "fsom/AudioRegion.hpp"
#include "tinyxml/tinyxml.h"
#include <fsom/Engine.hpp>

#include <fsom/AudioFilePool.hpp>


using namespace fsom;

AudioRegion::AudioRegion(regionCreationStruct data):
	Region(data),
	m_file(AudioFilePool::get_instance().get_audio_file_ptr(data.m_workingDirectory +data.m_filepath  )),
	m_diskStreamBuffers(2,4096),
	m_samplePosition(0)
	{
	set_offset_amount(data.m_offset);
	set_meta("RegionType","Audio");	
	//add_parameter("AudioParam",0.0,1.0,1.0); 

	//get_parameter("AudioParam")->set_meta("GuiHint","soCustomFader");
	//get_parameter("AudioParam")->set_meta("Tip","Generic Audio Parameter");
	
	  if(data.m_duration == 0){
		 set_duration(m_file->get_file_length()); //when duration is set to 0 set to full file length
	  }
	  fsom::DebugStream << "Audio region spawned"<<std::endl;
	  fsom::DebugStream << "full path = " << data.m_filepath<<std::endl;
	  fsom::DebugStream << "working directory path = " << data.m_workingDirectory<<std::endl;
}

AudioRegion::~AudioRegion(){
}

void AudioRegion::show_info(){
	m_file->show_info();
}

void AudioRegion::process(float** input, float** output, int frameSize, int channels){
	//assert(channels == m_file.get_channels() && channels == 2);
	// make a request to the audiofile object to fill the disk stream buffers. 
	if(!get_reverse_state()){
	      m_file->seek(m_samplePosition);
	      m_file->get_block(m_diskStreamBuffers.get_buffers(),frameSize);
	      m_samplePosition+=frameSize;
	      // copy from the disk stream buffers through the DSP onto the output buffers.
	      float** t=m_diskStreamBuffers.get_buffers();	
	      process_dsp_stack(t,output,frameSize,channels);
	}else{
	    //If the sample position is at the back, offset by the sample size
	      if(m_samplePosition == get_duration())m_samplePosition = get_duration()-frameSize;
// 	      fsom::DebugStream <<"sp"  <<m_samplePosition<< std::endl;
// 	      fsom::DebugStream << "frmsz " << frameSize<<std::endl;
	      m_file->seek(m_samplePosition);
	      m_file->get_block(m_diskStreamBuffers.get_buffers(),frameSize);
	      m_samplePosition-=frameSize;
	      float** norm = m_diskStreamBuffers.get_buffers();
	      MultiChannelBuffer out(channels,frameSize);
	      int index = 0;
	      float** rev = out.get_buffers();
	      for(int chan = 0; chan<channels;++chan){
		for(int n = 0;n <frameSize ;++n){
		    rev[chan][n]=norm[chan][(frameSize-n)-1];
// 		    fsom::DebugStream << " "<< n <<" " << frameSize - n;
		}
// 		++index;
	    }
	      
	    process_dsp_stack(rev,output,frameSize,channels);
	}
	
	// process the fx stack from the disk buffer to the output
// 	process_dsp_stack(t,output,frameSize,channels);
}

AudioFilePtr AudioRegion::get_audiofile(){
	return m_file;
}

std::string AudioRegion::get_file_path(){
	return m_file->get_file_path();	
}

SampleLength AudioRegion::get_file_length(){
	return m_file->get_file_length();
}

void AudioRegion::save_to_xml_node(TiXmlElement* node,bool useRelative){
	TiXmlElement * element = new TiXmlElement( "Region" );	
	save_to_region_specifics_to_existing_xml_node(element,useRelative);
// 	element->SetAttribute("path",get_file_path().c_str());
	node->LinkEndChild( element );	

}

void AudioRegion::on_region_start(SamplePosition seekTime){
	fsom::DebugStream << "-------------------------------------------------------" << std::endl;
	if(!get_reverse_state()){
	    fsom::DebugStream << "------------------Normal audio region------------------------------"<<std::endl;
	    fsom::DebugStream << "seektime = " << seekTime << std::endl;
	    SamplePosition seekPositionOffset = (seekTime - get_start_pos());
	    fsom::DebugStream << "seekPositionOffset = " << seekPositionOffset << std::endl;
	    if(seekPositionOffset > 0){
	      fsom::DebugStream << "if" << std::endl;
	      m_samplePosition =seekPositionOffset + get_offset_amount(); 
    // 	  m_file->seek(); // add the audiofile offset to the seek offset in case of spliced region
	    }else{
	      fsom::DebugStream << "else" << std::endl;
	      m_samplePosition =get_offset_amount();
    // 	  m_file->seek(0 + get_region_offset());
	    }
	}else{
	  fsom::DebugStream << "--------------------Reversed audio region --------------------------"<<std::endl;
	  fsom::DebugStream << "seektime = " << seekTime <<std::endl;
	  SamplePosition seekPositionOffset = (seekTime - get_start_pos());
	  fsom::DebugStream << "seekPositionOffset = " << seekPositionOffset << std::endl;
	  if(seekPositionOffset > 0){
	    fsom::DebugStream << "if" << std::endl;
	    //TODO
	    m_samplePosition =seekPositionOffset + get_offset_amount(); 
  // 	  m_file->seek(); // add the audiofile offset to the seek offset in case of spliced region
	  }else{
	    fsom::DebugStream << "else" << std::endl;
	    ///If reversed an unheard set the sample position to be the duration of the region
	    m_samplePosition =get_duration();
  // 	  m_file->seek(0 + get_region_offset());
	  }
	
	  
	  
	  
	}
	reset_all_effects();
	fsom::DebugStream << "-------------------------------------------------------" << std::endl;
}


