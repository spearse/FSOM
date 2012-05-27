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


#ifndef _SYNTHESISREGION_HPP_
#define _SYNTHESISREGION_HPP_

#include "Region.hpp"
#include <vector>
#include "Generator.hpp"
#include "Utilities.hpp"
#include "SynthesisModule.hpp"

namespace fsom{
	
typedef std::vector<GeneratorPtr> GeneratorStack;	
typedef std::vector<SynthesisModulePtr> SynthesisModuleStack;	

class SynthesisRegion :public Region{
	GeneratorStack m_generatorStack;
	SynthesisModuleStack m_moduleStack;
	MultiChannelBuffer m_stackBufferA,m_stackBufferB;
	BreakPointUnit m_internalFadeUnit;
		
	const float m_preferedFadeTime; //duration for prefered fading in and out if smaller make fades half duration
	
public:
	SynthesisRegion(regionCreationStruct creationStruct);
	virtual ~SynthesisRegion();
	virtual void process(float** input, float** output, int frameSize, int channels);
	
	/// abstract save function
	void save_to_xml_node(TiXmlElement* node) ;
	/// abstract function to initialise the region for playback.
	/// this is called automatically during the playback of a session prior to hearing the file for the first time.
	virtual void on_region_start(SamplePosition seekTime) ;
	GeneratorPtr get_generator(int index);
	GeneratorStack& get_generator_stack();
	void add_generator(GeneratorPtr generator);
	
	SynthesisModulePtr get_synth_module(int index);
	SynthesisModuleStack& get_synth_module_stack();
	
	void add_module(std::string id);
	//for loading
	void add_modulePtr(SynthesisModulePtr module);
	//for loading
	void add_generatorPtr(GeneratorPtr generator);

	
	void remove_all_generators();
	
	void remove_all_modules();
	
	void process_generator_stack(float** input, float** output, int frameSize, int channels);	
	void process_module_stack(float** input, float** output, int frameSize, int channels);	
	
	void update_fade_points();
	
	MultiChannelBuffer& get_bufferA();
	MultiChannelBuffer& get_bufferB();
};
	
typedef boost::shared_ptr<SynthesisRegion> SynthesisRegionPtr;
	
}



#endif
