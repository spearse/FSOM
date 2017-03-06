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


#ifndef CHORUS_HPP
#define CHORUS_HPP

#include "DSPEffect.hpp"
#include "../Table.hpp"
#include "DelayBase.hpp"
#include "ModulatedDelay.hpp"


namespace fsom{
	//DSPEffect Pointer
	typedef std::shared_ptr<DSPEffect> DSPEffectPtr;
	//DSPEffect function pointers
	typedef DSPEffectPtr(*DSPEffectCreateFunc)();
	
	class Chorus : public DSPEffect
	{
		ModulatedDelay m_modDelayLeft,m_modDelayRight;
		ParameterPtr m_frequencyParam, m_depthParam, m_amountParam;
	public:
		Chorus(dspCreationStruct data);
		virtual ~Chorus();
		void process(float** input, float** output, int frameSize, int channels) ;
		static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new Chorus(data)) ; }
		virtual DSPEffectPtr clone() { return DSPEffectPtr( new Chorus(*this) ); }
		virtual void reset_effect();
		
		inline ModulatedDelay* get_mod_delay(){
			return &m_modDelayLeft;
		}
		
	};
}
#endif // CHORUS_HPP
