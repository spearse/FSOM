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



#include "../include/fsom/SynthesisModule.hpp"
#include "tinyxml/tinyxml.h"

namespace fsom{
	
SynthesisModule::SynthesisModule(dspCreationStruct _struct) :
	DSPEffect(_struct)
{
	
}
SynthesisModule::~SynthesisModule(){
	
}




void SynthesisModule::save_parameters_to_node(TiXmlElement* element){
    ParameterList& paramList(get_parameter_list());
    ParameterList::iterator it;
    for(it=paramList.begin();it!=paramList.end();++it){
	TiXmlElement * Parameter = new TiXmlElement( "Parameter" );
	element->LinkEndChild(Parameter);
	Parameter->SetAttribute("Id", (*it).first.c_str());
	Parameter->SetDoubleAttribute("Value", (*it).second->get_value());
    }
  
}

void SynthesisModule::save_to_xml_node(TiXmlElement* element){
    TiXmlElement* moduleElement = new TiXmlElement("Module");
    TiXmlElement* basicElement = new TiXmlElement("BasicInfo");
    basicElement->SetAttribute("Type",get_effect_name().c_str());
    moduleElement->LinkEndChild(basicElement);
    
    save_parameters_to_node(moduleElement);
    save_meta_to_xml(moduleElement);
    
    element->LinkEndChild(moduleElement);
}



LFOModule::LFOModule(dspCreationStruct _struct):	
	SynthesisModule(_struct),
	m_sineTable(512),
	m_phasor(44100,1.0f)
{
	m_sineTable.fill_sine();
	add_parameter("Rate",0.0f,10.0f,1.0f);
	add_parameter("Amount",0.0f,1.0f,1.0f);
	set_effect_name("LFOModule");
	
}
    
LFOModule::~LFOModule(){
	
}

void LFOModule::process(float** input, float** output, int frameSize, int channels){
	m_phasor.set_frequency(get_parameter("Rate")->get_value()  );
	float val;
	float amnt;
	for(int frame=0;frame<frameSize;++frame){
		
		amnt =   get_parameter("Amount")->get_value();
		val = float(m_sineTable.linear_lookup(m_sineTable.get_size() * m_phasor.get_phase() )) ;  
		output[0][frame] +=   (input[0][frame] * val)*amnt; 
		output[1][frame] +=   (input[1][frame] * val)*amnt;
		m_phasor.tick();
		
	}
}

void LFOModule::save_specifics_to_xml_node(TiXmlElement* element){
  
}




FilterModule::FilterModule(dspCreationStruct _struct):	
	SynthesisModule(_struct)
{
      set_effect_name("FilterModule");

}
    
FilterModule::~FilterModule(){
	
}

void FilterModule::process(float** input, float** output, int frameSize, int channels){
	for(int frame=0;frame<frameSize;++frame){
		for(int chan = 0; chan<channels;++chan){
			output[chan][frame] += input[chan][frame]; 
		}
	}
}
void FilterModule::save_specifics_to_xml_node(TiXmlElement* element){
  
}




}

