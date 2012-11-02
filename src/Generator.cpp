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



#include "../include/fsom/Generator.hpp"

#include "tinyxml/tinyxml.h"

namespace fsom{
	
Generator::Generator(fsom::Generator::GeneratorType type, fsom::dspCreationStruct data):
	DSPEffect(data),
	m_genType(type),
	m_shape(512),
	m_pitchPhasor(44100.0f,440.0f), 
	m_noiseState(false),
	m_filePath("")
{
	fill_generator_accordingly();
	add_parameter("Amp",0,1,0.5);
	add_parameter("Freq",30,5000,440.0f);
	m_pitchPhasor.set_frequency(get_parameter("Freq")->get_value());	
}



Generator::~Generator(){
	
}

void Generator::process(float** input, float** output, int frameSize, int channels){
	 
	if(m_noiseState ==false){
		float amp = get_parameter("Amp")->get_value();
		m_pitchPhasor.set_frequency(get_parameter("Freq")->get_value());	
		float val;
		for(int frame = 0; frame < frameSize;++frame){
			for(int chan =0; chan < channels;++chan){
				val = m_shape.linear_lookup(m_pitchPhasor.get_phase() * m_shape.get_size()) * amp;
				output[chan][frame] += val;
				m_pitchPhasor.tick();
			}
		}
	}else{
		for(int frame = 0; frame < frameSize;++frame){
			for(int chan =0; chan < channels;++chan){
				output[chan][frame] += (0.6 * ((rand()%100)/50)-1.0f)* get_parameter("Amp")->get_value();
			}
		}
	}	
}


void Generator:: set_generator_voice(GeneratorType type){
	m_genType = type;
	fill_generator_accordingly();
}

int Generator::get_generator_voice(){
	return m_genType;
}
	
void Generator::fill_generator_accordingly(){
	m_noiseState = false;
	switch(m_genType){
		case(GEN_Sine):
			m_shape.fill_sine();
			break;
		case(GEN_Cos):
			m_shape.fill_cos();
			break;
		case(GEN_Noise):
			m_shape.fill_noise();
			m_noiseState = true;
			break;
		case(GEN_Square):
			m_shape.fill_square();
			break;
		case(GEN_Saw):
			m_shape.fill_linear_offset();
			break;
		case(GEN_Tri):
			m_shape.fill_triangle();
	}
}
	
	
void Generator::set_noise_state(bool state){
	m_noiseState = state;
	fill_generator_accordingly();
}
	
void Generator::save_to_xml_node(TiXmlElement* node ){
  TiXmlElement* generatorElement = new TiXmlElement("Generator");
  TiXmlElement* infoElement = new TiXmlElement("BasicInfo");
  
  
  infoElement->SetAttribute("GenType",m_genType);
  infoElement->SetAttribute("NoiseState",m_noiseState);
  infoElement->SetAttribute("Path",m_filePath.c_str());
  
  generatorElement->LinkEndChild(infoElement);
  
  
  ParameterList& paramList(get_parameter_list());
  ParameterList::iterator it;
  for(it=paramList.begin();it!=paramList.end();++it){
	TiXmlElement * Parameter = new TiXmlElement( "Parameter" );
	generatorElement->LinkEndChild(Parameter);
	Parameter->SetAttribute("Id", (*it).first.c_str());
	Parameter->SetDoubleAttribute("Value", (*it).second->get_value());

  }
 
  save_meta_to_xml(generatorElement);
  
  node->LinkEndChild(generatorElement);
  
}
void Generator::set_file_path(std::string filepath){
    m_filePath = filepath;
}


Table<double> Generator::get_table()const{
  return m_shape;
  
}

  
}