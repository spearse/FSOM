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
#include <functional>

using namespace fsom;

GranularRegion::GranularRegion(regionCreationStruct data):
Region(data), 
// m_file(data.m_filepath),
m_diskStreamBuffers(2,4096),
// m_table(512),
m_counter(0),
m_fileLoaded(false),
m_basePosition(0),
m_basePitch(1.0f),
m_density(2),
m_grainSize(44100),
m_nextSpawn(44100),
m_grainRate(1),
m_window(TablePtr(new Table<float>(512))),
m_filepath(""),
m_internalClock(0),
m_grainAmp(1.0f)
{
  add_parameter("GrainSize - ms",175.f,1000.f,500.f);
  add_parameter("GrainPitch",0.f,3.f,1.f);
  add_parameter("GrainPosition",0.f,1.f,0.f);
  add_parameter("GrainRate - grains per second",0.1f,16.f,4.f);
  add_parameter("GrainAmp",0.01f,1.5f,1.f);

  
  m_window->fill_hann();
  MultiTablePtr mt;
  TablePtr t1 = TablePtr(new Table<float>(44100*5));
  TablePtr t2 = TablePtr(new Table<float>(44100*5));
  m_table = MultiTablePtr(new MultiTableBuffer());
  m_table->push_back(t1);
  m_table->push_back(t2);
  
  fill_table_sin();
  
  //for testing
//   m_sinTable.fill_triangle();
//   m_phasor.set_frequency(120);
  m_diskStreamBuffers.clear();

}

GranularRegion::~GranularRegion(){}


void GranularRegion::process(float** input, float** output, int frameSize, int channels){
  
   
  
// 	FSOM_ASSERT(channels == m_file.get_channels() && channels == 2);
	// make a request to the audiofile object to fill the disk stream buffers. 
// 	m_file.get_block(m_diskStreamBuffers.get_buffers(),frameSize);//TODO write granular functions
	// copy from the disk stream buffers through the DSP onto the output buffers.
	m_diskStreamBuffers.clear();
	
// 	m_grainStream.set_basePitch( get_parameter("GrainPitch")->get_value()  );
// 	m_grainStream.set_basePosition(get_parameter("GrainPosition")->get_value()    );
// 	m_grainStream.set_grainRate(get_parameter("GrainRate")->get_value());
// 	m_grainStream.set_grainSize(get_parameter("GrainSize")->get_value());
	
	
	float** t=m_diskStreamBuffers.get_buffers();	
	float v=0;
	//for testing only
	
	grain_process(t,0,frameSize);
	
	
	
// 	samplesRead += frameSize;
	
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

void GranularRegion::save_to_xml_node(TiXmlElement* node, bool useRelatvie){
	TiXmlElement * element = new TiXmlElement( "Region" );	
	save_meta_to_xml(element);
	save_to_region_specifics_to_existing_xml_node(element,useRelatvie);
	element->SetAttribute("path",get_file_path().c_str());
	node->LinkEndChild( element );	

}

void GranularRegion::on_region_start(SamplePosition seekTime){
// 	m_file.seek(seekTime); // this would seek to region file offset
//   m_grainStream.reset();
    m_counter = 0;
    reset();
    m_diskStreamBuffers.clear();
    
}



void GranularRegion::load_soundfile(std::string filepath){
  try{
    m_table = MultiTablePtr(new MultiTableBuffer( Engine::get_instance().get_active_session().load_file_to_table(filepath))) ;
    m_filepath = filepath;
//     m_grainStream.load_soundfile(filepath);
    m_fileLoaded = true;
  }catch(...){
      m_fileLoaded = false;
  }
    
}


void GranularRegion::fill_table_sin(){
    m_table->at(0)->fill_sine(4400);
    m_table->at(1)->fill_sine(4400);
}

bool GranularRegion::get_load_state(){
    return m_fileLoaded;
}



std::string GranularRegion::get_soundfile(){
    return m_filepath;
}

void GranularRegion::reset(){
    m_internalClock = 0;
    m_nextSpawn = 0;
    m_counter = 0;
    kill_all_grains();
    m_diskStreamBuffers.clear();
}

void GranularRegion::kill_all_grains(){
    for(GrainList::size_type n =0; n < m_grains.size();++n){
	    m_grains.at(n).reset();
	    m_grains.erase(m_grains.begin()+n);
    }
//      fsom::DebugStream << "Killed All grains"<<std::endl; 
}
  



void GranularRegion::grain_process(float** output, int channels,int frames){
    int remainder = frames;
  int start = 0;

  
   SamplePosition samplesRead;
    
    Session& sess = fsom::Engine::get_instance().get_active_session();
    
    if(sess.get_preview_state() == false){
	samplesRead = get_sample_position();
	
    }else{
	samplesRead = sess.get_previed_playhead_value(); 
    }  
    
  
  
  
  
  while(remainder > 0){
//        fsom::DebugStream << m_nextSpawn<<std::endl;
	kill_grains();
//       m_grainStream.set_basePitch( get_parameter("GrainPitch")->get_value()  );
// 	m_grainStream.set_basePosition(get_parameter("GrainPosition")->get_value()    );
// 	m_grainStream.set_grainRate(get_parameter("GrainRate")->get_value());
// 	m_grainStream.set_grainSize(get_parameter("GrainSize")->get_value());
	m_basePitch = get_parameter("GrainPitch")->get_value();
	m_basePosition = truncate_to_integer<int>( get_parameter("GrainPosition")->get_value() * m_table->at(0)->get_size() );
	m_grainRate = truncate_to_integer<int>( get_parameter("GrainRate - grains per second")->get_value());
	m_grainSize = truncate_to_integer<int>( get_parameter("GrainSize - ms")->get_value()*44.1f);
	m_grainAmp  = get_parameter("GrainAmp")->get_value();
	
      if(m_nextSpawn <= 0){
	  spawn();  
      }
      
      if (remainder > m_nextSpawn) {
	
	  
			// partial process
			
			std::for_each(
				m_grains.begin(),m_grains.end(),
				std::bind(&Grain::process, std::placeholders::_1, output, start, m_nextSpawn)
			);
			
			start += m_nextSpawn;
			remainder -= m_nextSpawn;
			m_nextSpawn = 0;
      } else {
			// remain process
			
			std::for_each(
				m_grains.begin(),m_grains.end(),
				std::bind(&Grain::process, std::placeholders::_1, output, start, remainder)
			);
			m_nextSpawn -= remainder;
			remainder = 0;
      }
      samplesRead++;
 
       for(ParameterList::const_iterator it = get_parameter_list().begin(); it != get_parameter_list().end();++it){
	    (*it).second->tick(samplesRead);
      }
           
  }
  
}

void GranularRegion::spawn(){
      float dur = 44100.0f/m_grainRate;
	  m_nextSpawn = truncate_to_integer<int>(dur);
      
      m_grains.push_back(GrainPtr(new Grain(m_window,m_table,m_grainSize,m_basePosition,m_basePitch,m_grainAmp)));
//       fsom::DebugStream << "Spawned " << "next = " << m_nextSpawn<<std::endl; 
    
}

void GranularRegion::kill_grains(){
  for(GrainList::size_type n =0; n < m_grains.size();++n){
	bool kill =false; 
	if(m_grains[n]->is_dead())kill = true;
	if(kill){ 
	  m_grains.at(n).reset();
	  m_grains.erase(m_grains.begin()+n);
//  	  fsom::DebugStream << "Killed a grain"<<std::endl;
	}
      
    }
 
}

