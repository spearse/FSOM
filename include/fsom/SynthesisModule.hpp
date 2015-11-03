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


#ifndef _SYNTHESISMODULE_HPP_
#define _SYNTHESISMODULE_HPP_

#include "EffectClasses/DSPEffect.hpp"
#include <memory>
#include "Table.hpp"
#include "Utilities.hpp"

namespace fsom{
	
class SynthesisModule :public DSPEffect{
    
public:
    SynthesisModule(dspCreationStruct _struct);
    virtual ~SynthesisModule();
	virtual DSPEffectPtr clone() =0;
    virtual void process(float** input, float** output, int frameSize, int channels)=0;
    virtual void save_specifics_to_xml_node(TiXmlElement* element)=0;
     virtual void save_to_xml_node(TiXmlElement* element);
      private:
	
    virtual void save_parameters_to_node(TiXmlElement* element);
};

typedef std::shared_ptr<SynthesisModule> SynthesisModulePtr;




class LFOModule :public SynthesisModule{
	Table<double> m_sineTable;
	Phasor m_phasor;
	public:
	LFOModule(dspCreationStruct _struct);
	virtual ~LFOModule();
	static SynthesisModulePtr create(dspCreationStruct data) { return SynthesisModulePtr(new LFOModule(data)) ; }
	
	virtual DSPEffectPtr clone() { return DSPEffectPtr( new LFOModule(*this) ); }

	virtual void process(float** input, float** output, int frameSize, int channels);
	virtual void save_specifics_to_xml_node(TiXmlElement* element);	
};

class VolModule :public SynthesisModule{
	Table<double> m_sineTable;
	Phasor m_phasor;
	public:
	VolModule(dspCreationStruct _struct);
	virtual ~VolModule();
	static SynthesisModulePtr create(dspCreationStruct data) { return SynthesisModulePtr(new VolModule(data)) ; }
	
	virtual DSPEffectPtr clone() { return DSPEffectPtr( new VolModule(*this) ); }

	virtual void process(float** input, float** output, int frameSize, int channels);
	virtual void save_specifics_to_xml_node(TiXmlElement* element);	
};

class FilterModule :public SynthesisModule{
	public:
	FilterModule(dspCreationStruct _struct);
	virtual ~FilterModule();
	static SynthesisModulePtr create(dspCreationStruct data) { return SynthesisModulePtr(new FilterModule(data)) ; }
	
	virtual DSPEffectPtr clone() { return DSPEffectPtr( new FilterModule(*this) ); }

	virtual void process(float** input, float** output, int frameSize, int channels);
	
	virtual void save_specifics_to_xml_node(TiXmlElement* element);
};

}



#endif
