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
	void sum_sample(T sample){
		m_buffer[m_writePos] +=sample;
	}
	///return a single sample at a given delay time 	
	T read_sample(float delayTime){
		/*
		sample_index t = m_writePos-delayTime;
		while(t > m_maxDelay )t-=m_maxDelay;
		while(t < 0 )t+=m_maxDelay;
		return m_buffer[t];
		 */
		float pos, previousSampleValue, nextSampleValue, fraction;
		
		pos = m_writePos - delayTime;
		
		//Make shure the delay does not overshoot the buffer
		//Change to while if extreme delays are allowed
		//If statements are a little bit cheaper in terms of instructions
		if (pos < 0 ) {
			pos += m_maxDelay;
		} else if (pos >= m_maxDelay) {
			pos -= m_maxDelay;
		}
		
		// Interpolate the signal as one could try to access a value inbetween
		// what is stored in the buffer as sound waves are continious
		// while values handled are discrete.
		int index = (int) pos;
		fraction = pos - index;
		previousSampleValue = m_buffer[index];
		
		// Get next element in the circular buffer,
		// If previous element was the last element, get the first element
		if (index != m_maxDelay-1) {
			nextSampleValue = m_buffer[index + 1];
		} else {
			nextSampleValue = m_buffer[0];
		}
		
		// Get interpolated value
		float interpolatedValue = ((1.0f - fraction) * previousSampleValue) +
		(fraction * nextSampleValue);
		
		return interpolatedValue;
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
