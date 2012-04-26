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



#include "../include/fsom/EffectClasses/DSPEffect.hpp"


namespace fsom{
  
DSPEffect::DSPEffect(dspCreationStruct creationStruct) :

	m_tutId("TutorialLink"),
	m_dataStruct(creationStruct),
	m_isImplemented(false),
	m_bypass(false)
{
	register_meta(m_tutId);
	register_meta("Tip");
}

DSPEffect::DSPEffect(const DSPEffect& op) :
  	m_IdName(op.m_IdName),
	m_tutId(op.m_tutId),
	m_dataStruct(op.m_dataStruct),
	m_isImplemented(op.m_isImplemented),
	m_bypass(op.m_bypass)
{
	register_meta(m_tutId);
	register_meta("Tip");
	///Register Metadata not required since MetadataStore should copy automatically
	for(ParameterList::const_iterator it = op.m_parameterList.begin();
		it != op.m_parameterList.end();
		++it){
		std::pair<ParameterList::iterator,bool> ret = m_parameterList.insert( std::make_pair(it->first, ParameterPtr(new Parameter( *(it->second) ) ) ) );
		assert(ret.second == true && "could not insert into the map");
	}
}

DSPEffect::~DSPEffect()
{

}


ParameterPtr DSPEffect::get_parameter(std::string IDName) {
	std::map<std::string, ParameterPtr>::iterator it;
	it = m_parameterList.find(IDName);
	assert(it != m_parameterList.end() && "Could not find Param Name");
	return  (*it).second;
}

ParameterPtr DSPEffect::get_parameter(int ID) {
	std::map<std::string, ParameterPtr>::iterator it;
	it =  m_parameterList.begin();
	advance(it,ID);
	//assert(it != m_parameterList.end() && "Could not find Param Name");
	return  (*it).second;
}

ParameterList& DSPEffect::get_parameter_list(){
  
  return m_parameterList;
  
}

int DSPEffect::get_num_parameter() const {
	return m_parameterList.size();
}

void DSPEffect::add_parameter(std::string IdName, float lowerBound, float upperBound, float value){
	m_parameterList.insert(std::pair<std::string, ParameterPtr>(IdName,ParameterPtr(new Parameter(IdName,lowerBound,upperBound,value) )      ));
}

std::string& DSPEffect::get_effect_name(){
  
      return m_IdName; 
 
}

void DSPEffect::set_effect_name(std::string IdName){
  
     m_IdName = IdName; 
  
}

const std::string& DSPEffect::get_tutId(){
	return m_tutId;
} 


bool DSPEffect::is_implemented(){
  
     return m_isImplemented; 
}
void DSPEffect::set_implementation(){
     m_isImplemented = true; 
}
/*
fsom::Region& DSPEffect::get_parent_region(){
	return m_dataStruct.m_region;
}
*/

bool DSPEffect::bypass_active(){
  return m_bypass;
}
void DSPEffect::set_bypass(bool state){
      m_bypass = state;
}

	
}

