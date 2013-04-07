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



#include "fsom/Region.hpp"

#include <iostream>
#include "tinyxml/tinyxml.h"

using namespace fsom;

Region::Region(regionCreationStruct creationStruct) :
	m_stackBufferA(2,4096),
	m_stackBufferB(2,4096),
	m_dataStruct(creationStruct),
	m_isInExtensionMode(false),
	m_internalPos(0),
	m_muted(false)
	{
	register_meta("RegionType");
	register_meta("Tip");
	
	set_duration(m_dataStruct.m_duration);
	set_lane_num(m_dataStruct.m_laneNum);
	set_start_pos(m_dataStruct.m_startPos);
	set_offset_amount(m_dataStruct.m_offset);
	set_working_directory(m_dataStruct.m_workingDirectory
	);
	set_reverse_state(m_dataStruct.m_reverseState);
	set_extension(m_dataStruct.m_extension);
}
/*
Region::~Region(){

clear_all_effects();

}
*/
void Region::set_start_pos(SamplePosition startPos){
	m_dataStruct.m_startPos=startPos;
}
void Region::set_lane_num(int laneNum){
	m_dataStruct.m_laneNum=laneNum;
}
void Region::set_duration(SampleLength duration){
	m_dataStruct.m_duration=duration;
	//TODO update effects
	
}

void Region::set_file_path(std::string filePath){
    
  m_dataStruct.m_filepath=filePath;
  
}

SamplePosition Region::get_start_pos() const{
	return m_dataStruct.m_startPos;
}
int Region::get_lane_num() const{
	return m_dataStruct.m_laneNum;
}
SampleLength Region::get_duration() const{
	return m_dataStruct.m_duration;
}


SampleLength Region::get_extension()const{
  return m_dataStruct.m_extension;
}

void Region::set_extension(SampleLength extension){
    m_dataStruct.m_extension = extension;
}



std::string Region::get_file_path() const{
    return m_dataStruct.m_filepath;
}
void Region::set_working_directory(std::string directory){
    m_dataStruct.m_workingDirectory = directory;
}

std::string Region::get_working_directory()const { 
    return m_dataStruct.m_workingDirectory;
}


void Region::print_region_info() const{
	std::cout << "Region: t=" << m_dataStruct.m_startPos << " d=" << m_dataStruct.m_duration << " l=" << m_dataStruct.m_laneNum << std::endl;
}

void Region::save_region_parameters(TiXmlElement* node){
      std::map<std::string,ParameterPtr>::iterator it;
		
	for(it = m_parameterList.begin();it!=m_parameterList.end();++it){
	    TiXmlElement * Parameter = new TiXmlElement( "Parameter" );
	    node->LinkEndChild(Parameter);
	    Parameter->SetAttribute("Id", (*it).first.c_str());
	    Parameter->SetDoubleAttribute("Value", (*it).second->get_value());
		
	    /*
	    //adding breakpoints into saved filePath
	    fsom::BreakPointUnitPtr tempUnit = (*it).second->get_breakpoints();
	    for(int i = 0; i < tempUnit->get_list_size(); ++i){
	      TiXmlElement * Breakpoint = new TiXmlElement("Breakpoint");
	      Parameter->LinkEndChild(Breakpoint);
	      Breakpoint->SetAttribute("Pos",tempUnit->get_pair(i).t_);
	      Breakpoint->SetDoubleAttribute("Val",tempUnit->get_pair(i).v_);
	    }
	*/
	}
	

  
};


void Region::save_to_region_specifics_to_existing_xml_node(TiXmlElement* node){
		TiXmlElement * BasicInfo = new TiXmlElement( "BasicInfo" );
		node->LinkEndChild(BasicInfo );

		BasicInfo->SetAttribute("start",m_dataStruct.m_startPos);
		BasicInfo->SetAttribute("duration",m_dataStruct.m_duration);
		BasicInfo->SetAttribute("offset",m_dataStruct.m_offset);
		BasicInfo->SetAttribute("lanenum",m_dataStruct.m_laneNum);
		BasicInfo->SetAttribute("extension",m_dataStruct.m_extension);
		BasicInfo->SetAttribute("path", m_dataStruct.m_filepath.c_str());
		BasicInfo->SetAttribute("reversestate",m_dataStruct.m_reverseState);
		BasicInfo->SetAttribute("mutestate",m_muted);
		save_region_parameters(node);
		save_meta_to_xml(node);

		for(int n = 0; n < m_DSPStack.size(); ++n){
		    TiXmlElement * Effect = new TiXmlElement( "Effect" );
		    node->LinkEndChild(Effect );
		    Effect->SetAttribute("name", m_DSPStack[n]->get_effect_name().c_str());
			m_DSPStack[n]->save_meta_to_xml(Effect);
		    std::map<std::string,ParameterPtr>::iterator it;
		    for(it = m_DSPStack.at(n)->get_parameter_list().begin(); it != m_DSPStack[n]->get_parameter_list().end(); ++it){
		      TiXmlElement * Parameter = new TiXmlElement( "Parameter" );
		      Effect->LinkEndChild(Parameter);
		      Parameter->SetAttribute("Id", (*it).first.c_str());
		      Parameter->SetDoubleAttribute("Value", (*it).second->get_value());
		      
		      //adding breakpoints into saved filePath
			fsom::BreakPointUnitPtr tempUnit = (*it).second->get_breakpoints();
			
			for(int i = 0; i < tempUnit->get_list_size(); ++i){
			  TiXmlElement * Breakpoint = new TiXmlElement("Breakpoint");
			  Parameter->LinkEndChild(Breakpoint);
			  Breakpoint->SetAttribute("Pos",tempUnit->get_pair(i).t_);
			  Breakpoint->SetDoubleAttribute("Val",tempUnit->get_pair(i).v_);
			}
		      
		      //------------------------------------
		      
		    }

		}
		/*
		//run through the DSPStack map and pull each effect name from the list
		std::map<std::string,DSPEffectPtr>::iterator it;
		for(it= get_DSPStack().begin();it !=get_DSPStack().end();++it){
		    TiXmlElement * effect = new TiXmlElement( "effect" );
		    node->LinkEndChild( effect );
		    effect->SetAttribute("idName",(*it).first.c_str());

		    //Run through each effect's parameter and add as a double attribute
		    for(int n = 0; n < (*it).second->get_num_parameter(); ++n){
			std::stringsset_start_postream ss; //stringstream for converting int to string
			ss << n; //add n to stringstream
			//create an attribute called param with its number id affixed and place in the value
			//as a double
			effect->SetDoubleAttribute(("param" + ss.str()).c_str(),(*it).second->get_parameter(n).get_value());
		    }
		}*/
}

void Region::attach_effect(DSPEffectPtr p){

	m_DSPStack.push_back(p);

}

void Region::add_effect(std::string id){
	m_DSPStack.push_back(fsom::DSPManager::get_instance().create(id,dspCreationStruct(this)));
	std::cout << "***    Effect Stack  ***"<<std::endl;
	for(int n = 0; n < m_DSPStack.size();++n){
	 std::cout << "***" <<  m_DSPStack.at(n)->get_effect_name()<< "***"<<std::endl;
	}
	
}

void Region::remove_effect(std::string id){
    
	for (int n = 0; n <m_DSPStack.size();++n){
		if (id.compare(m_DSPStack.at(n)->get_effect_name()) == 0){
			std::cout << "Removing effect " << m_DSPStack.at(n)->get_effect_name() << std::endl;
			m_DSPStack.at(n).reset();
			m_DSPStack.erase(m_DSPStack.begin()+n);
		}
	}
}
//run through the DSPStack map and delete DSPEffectPtrs and clear the stack
void Region::clear_all_effects(){
   /* std::vector<DSPEffectPtr>::iterator it;
    for(it= m_DSPStack.begin();it !=m_DSPStack.end();++it){
        (*it).second.reset();
    }
    m_DSPStack.clear();*/
}


DSPStack& Region::get_DSPStack(){
	return m_DSPStack;
}

void Region::process_region(float** input, float** output, int frameSize, int channels, SamplePosition globalPosition){
  
  
    if(!m_isInExtensionMode){
	m_internalPos = globalPosition - m_dataStruct.m_startPos;
	//std::cout << "pos "<< globalPosition;
	if(!m_muted){
	  process(input,output,frameSize,channels);
	}
    }else{
	m_stackBufferB.clear();
	if(!m_muted){
	  process_dsp_stack(m_stackBufferB.get_buffers(),output,frameSize,channels);
	}
    }
  
}

void Region::process_dsp_stack(float** input, float** output, int frameSize, int channels){
	float** A = input;
	float** B = m_stackBufferA.get_buffers();
	std::vector<DSPEffectPtr>::iterator it=m_DSPStack.begin();
	(*it)->process( A,B,frameSize,channels );
	++it;
	A = m_stackBufferB.get_buffers();
 	while(it !=m_DSPStack.end()){
		std::swap(A,B);
		(*it)->process(A,B,frameSize,channels );
		++it;
	}
	// now sum B onto output;
	for(int n = 0 ; n < channels; ++n){
		for(int s = 0; s < frameSize; ++s){
			output[n][s] += B[n][s];
		}
	}
}

void Region::add_parameter(std::string IdName, float lowerBound, float upperBound, float value){
	
	BreakPointUnitPtr tempBPUnit = BreakPointUnitPtr(new BreakPointUnit());
	tempBPUnit->add_breakpoint(TVPair(0,value));
	tempBPUnit->add_breakpoint(TVPair(get_duration(),value));
  
	m_parameterList.insert(std::pair<std::string, ParameterPtr>(IdName,ParameterPtr(new Parameter(get_duration(),IdName,lowerBound,upperBound,value, tempBPUnit))));
}

ParameterPtr Region::get_parameter(std::string IdName){
	std::map<std::string, ParameterPtr>::iterator it;
	it = m_parameterList.find(IdName);
	assert(it != m_parameterList.end() && "Could not find Param Name");
	return  (*it).second;
}


ParameterPtr Region::get_parameter(int ID) {
	std::map<std::string, ParameterPtr>::iterator it;
	it =  m_parameterList.begin();
	advance(it,ID);
	//assert(it != m_parameterList.end() && "Could not find Param Name");
	return  (*it).second;
}


ParameterList& Region::get_parameter_list(){
  return m_parameterList;
}

int Region::get_num_parameter() const {
	return m_parameterList.size();
}

DSPEffectPtr Region::get_effect(int id){
	DSPEffectPtr fx = m_DSPStack.at(id);
	assert(fx.get() && "Contained ptr effect is invalid.");
	return  fx;
}

void Region::set_offset_amount(SampleLength offset){
	m_dataStruct.m_offset = offset;
}

SampleLength Region::get_offset_amount(){
	return m_dataStruct.m_offset;
}


regionCreationStruct Region::get_creation_struct(){
    return m_dataStruct;
}




void Region::set_extension_mode(bool state){
    m_isInExtensionMode = state;
}



void Region::set_reverse_state(bool state){
  m_dataStruct.m_reverseState = state;
}
bool Region::get_reverse_state(){
    return m_dataStruct.m_reverseState;
}
const SamplePosition& Region::get_sample_position(){
    return m_internalPos;
  
}

void Region::reset_all_effects(){
  DSPStack::iterator it;
  std::cout << "RESETTING EFFECTS"<<std::endl;
  for(it = m_DSPStack.begin(); it != m_DSPStack.end(); ++it){
   (*it)->reset_effect(); 
  }
  
}

void Region::set_mute_state(bool state){
    m_muted = state;
}

bool Region::get_mute_state(){
    return m_muted;
}
