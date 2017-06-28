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
m_filepath(""),
m_sampleRate(44100),
m_sampleSize(44100*5),
m_NormalfilePlaybackSpeed(0.1),
//spawn every 100MS
m_spawnRate(441),
m_grainSize(44100),
m_pitch(1),
m_phasor(m_sampleRate),
m_scrollSpeed(1)
{
	m_samplesPerSpawn= 4410;
	m_samplesToNextSpawn = 0;
	m_phasor.set_frequency(m_NormalfilePlaybackSpeed * m_scrollSpeed);
	for(int n = 0 ; n < 16;++n){
		m_grainvector.push_back(Grain());
	}
	
	
	
	
	
	set_meta("RegionType", "GranularSynthesis");
	add_parameter("GrainSize - ms",10.f,1000.f,100.f);
	add_parameter("GrainPitch",0.f,3.f,1.f);
	add_parameter("GrainPosition",0.f,1.f,0.f);
	add_parameter("GrainRate - grains per second",0.1f,64.f,10.f);
	add_parameter("GrainAmp",0.01f,1.5f,0.3f);
	add_parameter("PlaySpeed", -5.0f, 5.0f, 1.0f);
	
	m_parameterGrainSpawnRate = get_parameter("GrainRate - grains per second");
	m_parameterGrainAmp = get_parameter("GrainAmp");
	m_parameterGrainSize = get_parameter("GrainSize - ms");
	m_parameterGrainPosition = get_parameter("GrainPosition");
	m_parameterGrainPitch = get_parameter("GrainPitch");
	m_parameterPlaybackSpeed = get_parameter("PlaySpeed");
	
	
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
	element->SetAttribute("path",m_filepath.c_str());
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
		MultiTablePtr t = MultiTablePtr(new MultiTableBuffer( Engine::get_instance().get_active_session().load_file_to_table(filepath))) ;
		m_filepath = filepath;
		//     m_grainStream.load_soundfile(filepath);
		set_file_path(filepath);
		
		
		
		m_fileLoaded = true;
		m_sampleSize = t->at(0)->get_size();
		m_table = t;
		m_NormalfilePlaybackSpeed =1.0f/( m_sampleSize/m_sampleRate);
		m_phasor.set_frequency(m_NormalfilePlaybackSpeed  * m_scrollSpeed);
		
		
		
		
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
	//
	//
	//	m_internalClock = 0;
	//    m_nextSpawn = 0;
	//    m_counter = 0;
	//    kill_all_grains();
	//    m_diskStreamBuffers.clear();
}

void GranularRegion::kill_all_grains(){
	//    for(GrainList::size_type n =0; n < m_grains.size();++n){
	//	    m_grains.at(n).reset();
	//	    m_grains.erase(m_grains.begin()+n);
	//    }
	//      fsom::DebugStream << "Killed All grains"<<std::endl;
}




void GranularRegion::grain_process(float** output, int channels,int frames){
	
	
	SamplePosition samplesRead;
	
	Session& sess = fsom::Engine::get_instance().get_active_session();
	
	if(sess.get_preview_state() == false){
		samplesRead = get_sample_position();
		
	}else{
		samplesRead = sess.get_previed_playhead_value();
	}

	
	bool validSpawn = false;
	
	
	float amp = m_parameterGrainAmp->get_value();
	float pitch = m_parameterGrainPitch->get_value();
	
	
	for(int n = 0 ; n < frames; ++n){
		
		m_scrollSpeed = m_parameterPlaybackSpeed->get_value();
		
		
		m_phasor.set_frequency(m_NormalfilePlaybackSpeed  * m_scrollSpeed);
		
		amp = m_parameterGrainAmp->get_value();
		pitch = m_parameterGrainPitch->get_value();
		
		m_grainSize = m_parameterGrainSize->get_value() * 44.1f;
		
		m_samplesPerSpawn = m_parameterGrainSpawnRate->get_value()*0.00002267573696;
		
		for(int g = 0; g < m_grainvector.size();++g){
			
			
			
			m_grainvector[g].processSample(output[0][n], output[1][n],amp,pitch);
			
			
			if(m_samplesToNextSpawn <=0){
				
				m_currentSampleStart = m_phasor.get_phase()  * m_sampleSize;
				if(m_currentSampleStart <= 0)m_currentSampleStart +=m_sampleSize;
				if(m_currentSampleStart >= m_sampleSize) m_currentSampleStart-=m_sampleSize;
				//		std::cout  << "start " << sampleStart <<std::endl;
				
				for(int g = 0; g < m_grainvector.size();++g){
					if(m_grainvector[g].is_active() == false && validSpawn == false){
						m_grainvector[g].activate(m_table, m_grainSize,m_currentSampleStart);
						validSpawn = true;
					}
				}
				
				m_samplesToNextSpawn = m_samplesPerSpawn;
				
				
			}
		}
		--m_samplesToNextSpawn;
		
		for(ParameterList::const_iterator it = get_parameter_list().begin(); it != get_parameter_list().end();++it){
			(*it).second->tick(samplesRead);
		}
		
		m_phasor.tick();
		
	}
	
	
	//
	//
	//  int remainder = frames;
	//  int start = 0;
	//
	//
	//   SamplePosition samplesRead;
	//
	//    Session& sess = fsom::Engine::get_instance().get_active_session();
	//
	//    if(sess.get_preview_state() == false){
	//	samplesRead = get_sample_position();
	//
	//    }else{
	//	samplesRead = sess.get_previed_playhead_value();
	//    }
	//
	//
	//
	//
	//
	//  while(remainder > 0){
	////        fsom::DebugStream << m_nextSpawn<<std::endl;
	//	kill_grains();
	////       m_grainStream.set_basePitch( get_parameter("GrainPitch")->get_value()  );
	//// 	m_grainStream.set_basePosition(get_parameter("GrainPosition")->get_value()    );
	//// 	m_grainStream.set_grainRate(get_parameter("GrainRate")->get_value());
	//// 	m_grainStream.set_grainSize(get_parameter("GrainSize")->get_value());
	//	m_basePitch = get_parameter("GrainPitch")->get_value();
	//	m_basePosition = truncate_to_integer<int>( get_parameter("GrainPosition")->get_value() * m_table->at(0)->get_size() );
	//	m_grainRate = truncate_to_integer<int>( get_parameter("GrainRate - grains per second")->get_value());
	//	m_grainSize = truncate_to_integer<int>( get_parameter("GrainSize - ms")->get_value()*44.1f);
	//	m_grainAmp  = get_parameter("GrainAmp")->get_value();
	//
	//      if(m_nextSpawn <= 0){
	//	  spawn();
	//      }
	//
	//      if (remainder > m_nextSpawn) {
	//
	//
	//			// partial process
	//
	//			std::for_each(
	//				m_grains.begin(),m_grains.end(),
	//				std::bind(&Grain::process, std::placeholders::_1, output, start, m_nextSpawn)
	//			);
	//
	//			start += m_nextSpawn;
	//			remainder -= m_nextSpawn;
	//			m_nextSpawn = 0;
	//      } else {
	//			// remain process
	//
	//			std::for_each(
	//				m_grains.begin(),m_grains.end(),
	//				std::bind(&Grain::process, std::placeholders::_1, output, start, remainder)
	//			);
	//			m_nextSpawn -= remainder;
	//			remainder = 0;
	//      }
	//      samplesRead++;
	//
	//       for(ParameterList::const_iterator it = get_parameter_list().begin(); it != get_parameter_list().end();++it){
	//	    (*it).second->tick(samplesRead);
	//      }
	//
	//  }
	
}

void GranularRegion::spawn(){
	//
	//      float dur = 44100.0f/m_grainRate;
	//	  m_nextSpawn = truncate_to_integer<int>(dur);
	//
	//	if(m_grains.size() < 32){
	//      m_grains.push_back(GrainPtr(new Grain(m_window,m_table,m_grainSize,m_basePosition,m_basePitch,m_grainAmp)));
	////       fsom::DebugStream << "Spawned " << "next = " << m_nextSpawn<<std::endl;
	//	}else{
	//		for(int n = 0; n < m_grains.size();++n){
	//
	//			if(m_grains[n]->is_dead()){
	//				m_grains[n]->re_init(m_grainSize,m_basePosition,m_basePitch,m_grainAmp);
	//				return;
	//			}
	//		}
	//
	//
	//	}
}

void GranularRegion::kill_grains(){
	//  for(GrainList::size_type n =0; n < m_grains.size();++n){
	//	bool kill =false;
	//	if(m_grains[n]->is_dead())kill = true;
	/*
	 if(kill){
	 m_grains.at(n).reset();
	 m_grains.erase(m_grains.begin()+n);
	 //  	  fsom::DebugStream << "Killed a grain"<<std::endl;
	 }
	 */
	//    }
	
	
}

