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


#ifndef __MODULATEDDELAY_HPP__
#define __MODULATEDDELAY_HPP__

#include <vector>
#include <iostream>

#include <fsom/EffectClasses/DelayBase.hpp>
#include <cmath>
namespace fsom{

	class ModulatedDelay{
		
		DelayBase<float> m_delayUnit;
		float m_sampleRate,m_sampleRateMS,m_depth,m_frequency,m_phaseIndex;
		
		// 'speed' 0.0f and 1.0f.
		//It represents a fraction of the frequency range the unit has to offer.
		float frequencyFromSpeed(float speed) {
			
			float maxFrequency = 5.0f;
			float minFrequency = 0.002f;
			
			return (speed * (maxFrequency - minFrequency)) + maxFrequency;
		}
		float getDelay(float delayMS, float minPhaseDelayMS, float maxPhaseDelayMS){
			float start = minPhaseDelayMS + delayMS;
			
			
			//	float phase = (1.0f + OscillatorUnit::getValue()) * 0.5f;
			//samplerate need to be in MS?
			float phase = 1.0f + sinf((6.28318530718 * m_phaseIndex  * m_frequency)/m_sampleRate)   *0.5f   ;
			
			delayMS = m_depth * (phase * (maxPhaseDelayMS - minPhaseDelayMS)) + start;
			
			++m_phaseIndex;
	//		std::cout << " "<< delayMS * m_sampleRateMS << " ";
			return m_delayUnit.read_sample(delayMS * m_sampleRateMS);
		}
	public:
		ModulatedDelay(int maxDelay,float sampleRate,float depth):
			m_delayUnit(maxDelay),
			m_sampleRate(sampleRate),
			m_sampleRateMS(sampleRate * 0.001f),
			m_phaseIndex(0)

		{
			setDepth(depth);
		}
		//range 0 - 1
		void setDepth (float depth) {
			
			float max = 0.1f;
			float min = 0.005f;
			m_depth = (depth * (max - min)) + min;
		}
		//value between 0 and 1
		void set_frequency(float speed){
			m_frequency = frequencyFromSpeed(speed);
			
		}
		
	
		void tick(){
			m_delayUnit.tick();
		}
		float modulatedDelay(float delayMS){
			float minPhaseDelayMS = 5.0f;
			float maxPhaseDelayMS = 30.0f;
			
			return getDelay (delayMS, minPhaseDelayMS, maxPhaseDelayMS);
		}
		void clear(){
			m_delayUnit.clear_buffer();
		}
		void sum(float sample){
			m_delayUnit.sum_sample(sample);
		}
		void write(float sample){
			m_delayUnit.write_sample(sample);
		}
	};





}


#endif