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



#include "fsom/InterpControl.hpp"
#include <cassert>
using namespace fsom;

InterpControl::InterpControl(){

}
InterpControl::~InterpControl(){

}

void InterpControl::set_value(float value, double timeCode){
  m_points.push_back(Breakpoint(value,timeCode)); 
 // sort_points();
}

void InterpControl::sort_points(){
 
 // sort(m_points.begin(), m_points.end(), predicate);
  //sort(m_points.begin(), m_points.end());
}

float InterpControl::get_value(){

	return m_currentValue;

}

void InterpControl::tick(){


}

bool InterpControl::predicate(const Breakpoint& a ,const Breakpoint& b){ 
    // if (a.get_timeCode() < b.get_timeCode()){
     //  return true;
     //}else{return false;}
	assert(false && "NOT YET IMPLEMENTED");
	return false;
}
