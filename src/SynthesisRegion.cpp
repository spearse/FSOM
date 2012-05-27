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


#include "../include/fsom/Session.hpp"
#include "../include/fsom/Engine.hpp"
#include "../include/fsom/SynthesisRegion.hpp"
#include "../include/fsom/SynthesisModuleManager.hpp"

#include "tinyxml/tinyxml.h"

namespace fsom{  
  
SynthesisRegion::SynthesisRegion(regionCreationStruct creationStruct) :
	Region(creationStruct),
	m_stackBufferA(2,4096),
	m_stackBufferB(2,4096),	
	m_preferedFadeTime(44.0) //duration for prefered fading in and out if smaller make fades half duration
{
	
	if(creationStruct.m_duration < (m_preferedFadeTime*2)){
	      float fadeTime = creationStruct.m_duration/2.0;
	  
	      m_internalFadeUnit.add_breakpoint(TVPair(0, 0));
	      m_internalFadeUnit.add_breakpoint(TVPair(fadeTime, 1.0));
	      m_internalFadeUnit.add_breakpoint(TVPair(creationStruct.m_duration-fadeTime, 1.0));
	      m_internalFadeUnit.add_breakpoint(TVPair(creationStruct.m_duration, 0));
	}else{
	      m_internalFadeUnit.add_breakpoint(TVPair(0, 0));
	      m_internalFadeUnit.add_breakpoint(TVPair(m_preferedFadeTime, 1.0));
	      m_internalFadeUnit.add_breakpoint(TVPair(creationStruct.m_duration-m_preferedFadeTime, 1.0));
	      m_internalFadeUnit.add_breakpoint(TVPair(creationStruct.m_duration, 0));
	}
}
	
SynthesisRegion::~SynthesisRegion(){
	
}
	
GeneratorPtr SynthesisRegion::get_generator(int index){
	return m_generatorStack.at(index);
	
}

GeneratorStack& SynthesisRegion::get_generator_stack(){
	return m_generatorStack;
}
	
void SynthesisRegion::add_generator(GeneratorPtr generator){
	m_generatorStack.push_back(generator);
}

void SynthesisRegion::process(float** input, float** output, int frameSize, int channels){
  
	SamplePosition samplesRead;
      
	fsom::Session& sess = fsom::Engine::get_instance().get_active_session();
  
	if(sess.get_preview_state() == false){
	    samplesRead = get_sample_position();
	    
	}else{
	    samplesRead = sess.get_previed_playhead_value(); 
	}
    
	float** t = m_stackBufferA.get_buffers();
	float** t2 = m_stackBufferA.get_buffers();
	float** t3 = m_stackBufferB.get_buffers();
	process_generator_stack(0,t,frameSize,channels);
	
	for(int n = 0; n < frameSize; ++n){ 
	    
	    t2[0][n] = t[0][n] * m_internalFadeUnit.get_value(samplesRead);   
	    t2[1][n] = t[1][n] * m_internalFadeUnit.get_value(samplesRead);
	    
	    samplesRead++;
	}
	
	process_module_stack(t2,t3,frameSize,channels);
	

	process_dsp_stack(t3,output,frameSize,channels);
	
	
}
	
	
void SynthesisRegion::process_generator_stack(float** input, float** output, int frameSize, int channels){
	 m_stackBufferA.clear();
	 m_stackBufferB.clear();
   	/*
	 float** A = input;
	float** B = m_stackBufferA.get_buffers();
	std::vector<GeneratorPtr>::iterator it=m_generatorStack.begin();
	(*it)->process( A,B,frameSize,channels );
	++it;
	A = m_stackBufferB.get_buffers();
 	while(it !=m_generatorStack.end()){
		std::swap(A,B);
	    (*it)->process(A,B,frameSize,channels );
		++it;
	}
	// now sum B onto output;
	for(int n = 0 ; n < channels; ++n){
		for(int s = 0; s < frameSize; ++s){
			output[n][s] += B[n][s];
		}
	}*/
	for(int n  = 0 ; n< m_generatorStack.size();++n){
		m_generatorStack.at(n)->process(input,output,frameSize,channels);
	}


}	
	
SynthesisModulePtr SynthesisRegion::get_synth_module(int index){
	return m_moduleStack.at(index);
}
SynthesisModuleStack& SynthesisRegion::get_synth_module_stack(){
	return m_moduleStack;
}

void SynthesisRegion::add_module(std::string id){
	m_moduleStack.push_back( SynthesisModuleManager::get_instance().create(id,dspCreationStruct())  );
  
}

void SynthesisRegion::process_module_stack(float** input, float** output, int frameSize, int channels){
	for(int n = 0; n < m_moduleStack.size();++n){
		m_moduleStack.at(n)->process(input,output,frameSize,channels);
	}
}

MultiChannelBuffer& SynthesisRegion::get_bufferA(){
	return m_stackBufferA;
}
MultiChannelBuffer& SynthesisRegion::get_bufferB(){
	return m_stackBufferB;
}	


void SynthesisRegion::save_to_xml_node(TiXmlElement* node) {
	TiXmlElement * element = new TiXmlElement( "Region" );	
	save_to_region_specifics_to_existing_xml_node(element);
	GeneratorStack::iterator it;
	for(it=m_generatorStack.begin(); it!=m_generatorStack.end();++it){
	    (*it)->save_to_xml_node(element);
	}
	SynthesisModuleStack::iterator modit;
	for(modit=m_moduleStack.begin(); modit!=m_moduleStack.end();++modit){
	    (*modit)->save_to_xml_node(element);
	}
	
	
	node->LinkEndChild( element );	
	
}
/// abstract function to initialise the region for playback.
/// this is called automatically during the playback of a session prior to hearing the file for the first time.
void SynthesisRegion::on_region_start(SamplePosition seekTime) {
	
}
void SynthesisRegion::remove_all_generators(){
    for (int n = 0; n <m_generatorStack.size();++n){
		std::cout << "Removing generator " << m_generatorStack.at(n)->get_effect_name() << std::endl;
		m_generatorStack.at(n).reset();
		m_generatorStack.erase(m_generatorStack.begin()+n);
      }
}

	
void SynthesisRegion::remove_all_modules(){
    for (int n = 0; n <m_moduleStack.size();++n){
		std::cout << "Removing effect " << m_moduleStack.at(n)->get_effect_name() << std::endl;
		m_moduleStack.at(n).reset();
		m_moduleStack.erase(m_moduleStack.begin()+n);
      }
}
	
void SynthesisRegion::add_modulePtr(SynthesisModulePtr module){
    m_moduleStack.push_back(module);
  
}
//for loading
void SynthesisRegion::add_generatorPtr(GeneratorPtr generator){
  m_generatorStack.push_back(generator);
}

void SynthesisRegion::update_fade_points(){
 
  	if(get_duration() < (m_preferedFadeTime*2)){
		float fadeTime = get_duration()/2.0;
	      
	        m_internalFadeUnit.get_pair(0).t_ = 0.0;
		m_internalFadeUnit.get_pair(1).t_ = fadeTime;
		m_internalFadeUnit.get_pair(2).t_ = get_duration()-fadeTime;
		m_internalFadeUnit.get_pair(3).t_ = get_duration();
		m_internalFadeUnit.sort();
	}else{
	      
	      m_internalFadeUnit.get_pair(0).t_ = 0.0;
	      m_internalFadeUnit.get_pair(1).t_ = m_preferedFadeTime;
	      m_internalFadeUnit.get_pair(2).t_ = get_duration()-m_preferedFadeTime;
	      m_internalFadeUnit.get_pair(3).t_ = get_duration();
	      m_internalFadeUnit.sort();
	}
  
  
  
}
 
	
	
	
	
}

