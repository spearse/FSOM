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



#include "../include/fsom/AdditiveSynthesisRegion.hpp"
#include "../include/fsom/SynthesisModule.hpp"

using namespace fsom;

AdditiveSynthesisRegion::AdditiveSynthesisRegion(regionCreationStruct data): 
	SynthesisRegion(data)
{
	set_offset_amount(0);
	set_meta("RegionType","AdditiveSynthesis");
	//for testing, limited 
	fsom::DebugStream << "In constructor for additive synthesis, duration =  " << data.m_duration<< std::endl;
	 for(int n = 0; n < 4; ++n){
		GeneratorPtr t = GeneratorPtr(new Generator(Generator::GEN_Sine,dspCreationStruct(this)));
		t->get_parameter("Freq")->set_value(440);
		t->get_parameter("Amp")->set_value(0.2);
		add_generator(t);
	}
	//add_synth_module(SynthesisModulePtr(new LFOModule(dspCreationStruct(this))));

	add_module("VolModule");

}

AdditiveSynthesisRegion::~AdditiveSynthesisRegion(){}
/*
void AdditiveSynthesisRegion::process(float** input, float** output, int frameSize, int channels){
    get_bufferA().clear();
	get_bufferB().clear();
	float** t = get_bufferA().get_buffers();
	float** t2 = get_bufferB().get_buffers();

    //m_gen->process(input,output,frameSize,channels);
    //FIXME
//     process_dsp_stack(t,output,frameSize,channels);
}
*/

/*
void AdditiveSynthesisRegion::save_to_xml_node(TiXmlElement* node){


}
*/
void AdditiveSynthesisRegion::on_region_start(SamplePosition seekTime){
reset_all_effects();

}/*

void AdditiveSynthesisRegion::process_generator_stack(float** input, float** output, int frameSize, int channels){
	float** A = input;
	float** B = m_stackBufferA.get_buffers();
	std::vector<GeneratorPtr>::iterator it=m_generatorList.begin();
	(*it)->process( A,B,frameSize,channels );
	++it;
	A = m_stackBufferB.get_buffers();
 	while(it !=m_generatorList.end()){
		std::swap(A,B);
	    (*it)->process(A,B,frameSize,channels );
		++it;
	}
	// now sum B onto output;
	for(int n = 0 ; n < channels; ++n){
		for(int s = 0; s < frameSize; ++s){
			output[n][s] += B[n][s];
		}
	}
*/
	

