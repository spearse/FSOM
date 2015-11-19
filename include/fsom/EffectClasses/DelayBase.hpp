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


#ifndef __DELAYBASE_HPP__
#define __DELAYBASE_HPP__

#include <vector>
#include <iostream>
namespace fsom{

template<class T>

class DelayBase{
public:	
	typedef std::vector<T> BufferType;
	typedef int sample_index;

	BufferType m_buffer;
	sample_index m_maxDelay,m_writePos;	
public:
	DelayBase(sample_index maxDelay): 
		m_maxDelay(maxDelay),
		m_buffer(maxDelay,0),
		m_writePos(0)
	{		
		
	}
	///to write single samples into the buffer 
	void write_sample(T sample){
		m_buffer[m_writePos] = sample;
	}
	///return a single sample at a given delay time 	
	T read_sample(sample_index delayTime){
		sample_index t = m_writePos-delayTime;
		if(t < 0 )t+=m_maxDelay; 
		return m_buffer[t];
	}
	
	///increment the writing position
	void tick(){
		++m_writePos;
		if(m_writePos >= m_maxDelay){
			m_writePos = 0;
		}
	}

	virtual ~DelayBase(){
	}
	void clear_buffer(){
// 	  fsom::DebugStream << "Clearing delay buffer"<<std::endl;
		for (typename BufferType::size_type n = 0; n < m_buffer.size(); ++n){
	      m_buffer[n]=0;
	  }
// 	  fsom::DebugStream << m_buffer.size()<<std::endl;  
	}
}; 

}

#endif
