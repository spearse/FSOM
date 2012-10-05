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


#ifndef __BREAKPOINT_HPP__
#define __BREAKPOINT_HPP__

class Breakpoint{

float m_value;
double m_timeCode;

  public:

    Breakpoint(): m_value(0), m_timeCode(0){}
    Breakpoint(float value, double timeCode): m_value(value), m_timeCode(timeCode){}
    
    Breakpoint(const Breakpoint& op):m_timeCode(op.get_timeCode()),m_value(op.get_value()) {}
    
    void set_value(float value){ m_value = value ;}
    void set_timeCode(double timeCode){ m_timeCode = timeCode ;}
    void set_breakpoint(float value, double timeCode){m_value = value; m_timeCode = timeCode;}
    
    float& get_value(){ return m_value;}
    double& get_timeCode(){return m_timeCode;}
    
    bool operator < (const Breakpoint& rhs){ return (m_timeCode < rhs.m_timeCode);}
};
#endif
