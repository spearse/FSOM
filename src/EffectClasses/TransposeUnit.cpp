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



#include "../../include/fsom/EffectClasses/TransposeUnit.hpp"

namespace fsom{
  
  
TransposeUnit::TransposeUnit():
	m_delaySize(2048),
	m_delayUnitL(2048),
	m_delayUnitR(2048),
	m_table(512),
	m_phasor(44100,3),
	m_proportion(truncate_to_integer<int>(44100.0f/2048.f))

{
	m_table.fill_hann();
}

TransposeUnit::~TransposeUnit(){
}
void TransposeUnit::process(float& inL, float& inR, float& outL, float& outR, float amp){
	m_delayUnitL.write_sample(inL);
	m_delayUnitR.write_sample(inR);
	float phasorPos = m_phasor.get_phase();
	float phasorPosOffset = wrap(phasorPos + 0.5f);

	DelayBase<float>::sample_index iA = truncate_to_integer<DelayBase<float>::sample_index>(phasorPos*m_delaySize);
	DelayBase<float>::sample_index iB = truncate_to_integer<DelayBase<float>::sample_index>(phasorPosOffset*m_delaySize);

	float d1 = m_delayUnitL.read_sample(iA);
	float d2 = m_delayUnitL.read_sample(iB);
	float dR1 = m_delayUnitR.read_sample(iA);
	float dR2 = m_delayUnitR.read_sample(iB);

	float o1 = m_table.linear_lookup(phasorPos * m_table.get_size());
	float o2 = m_table.linear_lookup(phasorPosOffset * m_table.get_size());

	outL += (d1*o1 + d2*o2)*amp;
	outR += (dR1*o1 + dR2*o2)*amp;

	m_phasor.tick();
	m_delayUnitL.tick();
	m_delayUnitR.tick();
 
}

void TransposeUnit::process_replace(float& inL, float& inR, float& outL, float& outR){
	m_delayUnitL.write_sample(inL);
	m_delayUnitR.write_sample(inR);
	float phasorPos = m_phasor.get_phase();
	float phasorPosOffset = wrap(phasorPos + 0.5f);

	DelayBase<float>::sample_index iA = truncate_to_integer<DelayBase<float>::sample_index>(phasorPos*m_delaySize);
	DelayBase<float>::sample_index iB = truncate_to_integer<DelayBase<float>::sample_index>(phasorPosOffset*m_delaySize);

	float d1 = m_delayUnitL.read_sample(iA);
	float d2 = m_delayUnitL.read_sample(iB);
	float dR1 = m_delayUnitR.read_sample(iA);
	float dR2 = m_delayUnitR.read_sample(iB);

	float o1 = m_table.linear_lookup(phasorPos * m_table.get_size());
	float o2 = m_table.linear_lookup(phasorPosOffset * m_table.get_size());


	outL = (d1*o1 + d2*o2);
	outR = (dR1*o1 + dR2*o2);

	m_phasor.tick();
	m_delayUnitL.tick();
	m_delayUnitR.tick();

}


void TransposeUnit::set_frequency(float value){
  m_phasor.set_frequency(  (((pow(2.0f, value/12.0f)-1)*-1.0f) *m_proportion  ));
}
  
 
void TransposeUnit::clear_delays(){
  m_delayUnitL.clear_buffer();
  m_delayUnitR.clear_buffer();
}
  
  
  
  
  
  
  
}