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



#include "../include/fsom/Parameter.hpp"
#include "../include/fsom/Utilities.hpp"
#include "../include/fsom/Region.hpp"
#include <iostream>

using namespace fsom;

Parameter::Parameter(SampleLength duration, std::string IDName, float lowerBound, float upperBound, float value, BreakPointUnitPtr bpUnit ) : 
	m_IDName(IDName),
	m_currentValue(value),
	m_lowerBound(lowerBound),
	m_upperBound(upperBound),
	m_duration(duration),
	m_inDynamicMode(false),
	m_lastStaticValue(value),
	m_bpUnit(bpUnit)
	{
	register_meta("GuiHint");
	register_meta("LowerBound");
	set_meta_as_float("LowerBound",lowerBound);
	register_meta("UpperBound");
	set_meta_as_float("UpperBound",upperBound);
	register_meta("Tip");
	
}

Parameter::Parameter(const Parameter& old):  
      m_IDName(old.m_IDName),
      m_currentValue(old.m_currentValue),
      m_lowerBound(old.m_lowerBound),
      m_upperBound(old.m_upperBound),
      m_duration(old.m_duration),
      m_inDynamicMode(old.m_inDynamicMode),
      m_lastStaticValue(old.m_lastStaticValue),
      m_bpUnit(BreakPointUnitPtr(new BreakPointUnit(  *old.m_bpUnit)))
{
  
	register_meta("GuiHint");
	register_meta("LowerBound");
	set_meta_as_float("LowerBound",old.get_meta_as_float("LowerBound"));
	register_meta("UpperBound");
	set_meta_as_float("UpperBound",old.get_meta_as_float("UpperBound"));
	register_meta("Tip");
  
}

Parameter::~Parameter(){}

void Parameter::set_value(float value){
	m_currentValue = value; 
}

float Parameter::get_value() const {

	return m_currentValue;
}

float Parameter::get_value_zero_one()const {
	return m_currentValue/get_range();
}


const std::string& Parameter::get_name() const{
  
    return m_IDName;
}

void Parameter::tick(SampleLength& samplesRead){
    if(m_inDynamicMode){
	  m_currentValue = m_bpUnit->get_value(static_cast<float>(samplesRead));
    }
}

float Parameter::get_lowerBound() const{
    return m_lowerBound;
}

float Parameter::get_upperBound() const{
    return m_upperBound;
}
float Parameter::get_range() const{
    return m_upperBound - m_lowerBound;
}

BreakPointUnitPtr Parameter::get_breakpoints(){
  return m_bpUnit;
}

void Parameter::set_breakpoints(BreakPointUnitPtr bpUnit){
  m_bpUnit = bpUnit;
}

void Parameter::set_automated_mode(bool isDynamic){
    if(isDynamic){
      m_lastStaticValue = m_currentValue;
    }else{
      m_currentValue = m_lastStaticValue;
    }
  
    m_inDynamicMode = isDynamic;
}

bool Parameter::get_automated_mode() const{
    return m_inDynamicMode;
}

float Parameter::get_last_breakpoint_position() const{
    return m_bpUnit->get_last_breakpoint(); 
}

void Parameter::duplicate_last_breakpoint_at_time(float time){
    m_bpUnit->duplicate_last_at_time(time);
}

void Parameter::set_upperBound(float upperBound ){
	m_upperBound = upperBound;
}

