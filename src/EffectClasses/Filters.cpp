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



#include "../include/fsom/EffectClasses/Filters.hpp"

#include <fsom/Session.hpp>
#include <fsom/Engine.hpp>

namespace fsom{

  
std::vector<float> Filter::get_amplitudes(int steps){

  return get_biquad_left().get_amplitudes(steps);

}
  
LowPassFilter::LowPassFilter(dspCreationStruct data):
	Filter(data)
{
	set_effect_name("LowPassFilter");
	add_parameter("Frequency",5,10000.0f,10000.0f);
	add_parameter("Quality of Filter",0.2f,10.0f,1);
	
	set_meta(get_tutId(),"link to html");	
	get_parameter("Quality of Filter")->set_meta("GuiHint","soCustomFader");
	get_parameter("Frequency")->set_meta("GuiHint","soCustomFader");
	get_biquad_left().set_LPF(10000.0f,5.0f);
	get_biquad_right().set_LPF(10000.0f,5.0f);
	get_biquad_left().print_coef();
	set_implementation();
	
}

LowPassFilter::~LowPassFilter(){
	
}

void LowPassFilter::one_shot(){
	float f = get_parameter("Frequency")->get_value();
	float q = get_parameter("Quality of Filter")->get_value();
	get_biquad_left().set_LPF(f,q);
	get_biquad_right().set_LPF(f,q);

}


void LowPassFilter::process(float** input, float** output, int frameSize, int channels) {
      SamplePosition samplesRead;
    
      Session& sess = fsom::Engine::get_instance().get_active_session();
      
      if(sess.get_preview_state() == false){
	  samplesRead = get_creation_struct().attatchedRegion->get_sample_position();
	  
      }else{
	  samplesRead = sess.get_previed_playhead_value(); 
      }  
  
  
	if(!bypass_active()){
	    FSOM_ASSERT(channels == 2);
	    float f = get_parameter("Frequency")->get_value();
	    float q = get_parameter("Quality of Filter")->get_value();
	    get_biquad_left().set_LPF(f,q);
	    get_biquad_right().set_LPF(f,q);
	    get_biquad_left().process(input[0],output[0],frameSize);
	    get_biquad_right().process(input[1],output[1],frameSize);
	}else{
	    output[0] = input[0];
	    output[1] = input[1];
	}
	
      for(ParameterList::const_iterator it = get_parameter_list().begin(); it != get_parameter_list().end();++it){
	  (*it).second->tick(samplesRead);
      }
      
      samplesRead++;
}

HighPassFilter::HighPassFilter(dspCreationStruct data):
	Filter(data)
{
	set_effect_name("HighPassFilter");
	add_parameter("Frequency",5.0f,20000.0f,440.0f);
	add_parameter("Quality of Filter",0.2f,10,0.5);
	
	set_meta(get_tutId(),"link to html");
	get_parameter("Quality of Filter")->set_meta("GuiHint","soCustomFader");
	get_parameter("Frequency")->set_meta("GuiHint","soCustomFader");
	get_biquad_left().set_HPF(440,5.0f);
	get_biquad_right().set_HPF(440.0f,5.0f);
	set_implementation();
	
}

HighPassFilter::~HighPassFilter(){
	
}

void HighPassFilter::one_shot(){
	float f = get_parameter("Frequency")->get_value();
	float q = get_parameter("Quality of Filter")->get_value();
	get_biquad_left().set_HPF(f,q);
	get_biquad_right().set_HPF(f,q);
}

void HighPassFilter::process(float** input, float** output, int frameSize, int channels) {
  
        SamplePosition samplesRead;
    
	Session& sess = fsom::Engine::get_instance().get_active_session();
	
	if(sess.get_preview_state() == false){
	    samplesRead = get_creation_struct().attatchedRegion->get_sample_position();
	    
	}else{
	    samplesRead = sess.get_previed_playhead_value(); 
	}  
  
	if(!bypass_active()){
	FSOM_ASSERT(channels == 2);
	float f = get_parameter("Frequency")->get_value();
	float q = get_parameter("Quality of Filter")->get_value();
	get_biquad_left().set_HPF(f,q);
	get_biquad_right().set_HPF(f,q);
 	get_biquad_left().process(input[0],output[0],frameSize);
 	get_biquad_right().process(input[1],output[1],frameSize);
	}else{
	    output[0] = input[0];
	    output[1] = input[1];
	}
	
	for(ParameterList::const_iterator it = get_parameter_list().begin(); it != get_parameter_list().end();++it){
	  (*it).second->tick(samplesRead);
	}
	
	samplesRead++;

}
		
ResonatingFilter::ResonatingFilter(dspCreationStruct data):
	Filter(data)
{
	set_effect_name("ResonatingFilter");
	add_parameter("Frequency",5.0f,20000,440.0f);
	add_parameter("Q",0.1f,10.0f,5.0f);
	
	set_meta(get_tutId(),"link to html");
	get_parameter("Q")->set_meta("GuiHint","soCustomFader");
	get_parameter("Frequency")->set_meta("GuiHint","soCustomFader");
	get_biquad_left().set_BPF(440,1);
	get_biquad_right().set_BPF(440,1);
	set_implementation();
	
}

ResonatingFilter::~ResonatingFilter(){
	
}

void ResonatingFilter::one_shot(){
	float f = get_parameter("Frequency")->get_value();
	float q = get_parameter("Q")->get_value();
	get_biquad_left().set_BPF_const_gain(f,q);
	get_biquad_right().set_BPF_const_gain(f,q);
}

void ResonatingFilter::process(float** input, float** output, int frameSize, int channels) {
     if(!bypass_active()){ 
	FSOM_ASSERT(channels == 2);
	float f = get_parameter("Frequency")->get_value();
	float b = get_parameter("Q")->get_value();
	get_biquad_left().set_BPF_const_gain(f,b);
	get_biquad_right().set_BPF_const_gain(f,b);
 	get_biquad_left().process(input[0],output[0],frameSize);
 	get_biquad_right().process(input[1],output[1],frameSize);
     }else{
	    output[0] = input[0];
	    output[1] = input[1];
	}
	
}

BandRejectFilter::BandRejectFilter(dspCreationStruct data):
	Filter(data)
{
	set_effect_name("BandRejectFilter");
	add_parameter("Frequency",10.0f,20000,440.0f);
	add_parameter("Quality of Filter",0.1f,10.0f,5.0f);
	
	set_meta(get_tutId(),"link to html");
	get_parameter("Quality of Filter")->set_meta("GuiHint","soCustomFader");
	get_parameter("Frequency")->set_meta("GuiHint","soCustomFader");
	get_biquad_left().set_BAND_REJECT(440,1);
	get_biquad_right().set_BAND_REJECT(440,1);
	set_implementation();
	
} 	
	

BandRejectFilter::~BandRejectFilter(){
	
}


void BandRejectFilter::one_shot(){
	float f = get_parameter("Frequency")->get_value();
	float q = get_parameter("Quality of Filter")->get_value();
	get_biquad_left().set_BAND_REJECT(f,q);
	get_biquad_right().set_BAND_REJECT(f,q);
}
void BandRejectFilter::process(float** input, float** output, int frameSize, int channels) {
  
	SamplePosition samplesRead;
    
	Session& sess = fsom::Engine::get_instance().get_active_session();
	
	if(sess.get_preview_state() == false){
	    samplesRead = get_creation_struct().attatchedRegion->get_sample_position();
	    
	}else{
	    samplesRead = sess.get_previed_playhead_value(); 
	}
  
	if(!bypass_active()){
	    FSOM_ASSERT(channels == 2);
	    float f = get_parameter("Frequency")->get_value();
	    float b = get_parameter("Quality of Filter")->get_value();
	    get_biquad_left().set_BAND_REJECT(f,b);
	    get_biquad_right().set_BAND_REJECT(f,b);
	    get_biquad_left().process(input[0],output[0],frameSize);
	    get_biquad_right().process(input[1],output[1],frameSize);
	}else{
	    output[0] = input[0];
	    output[1] = input[1];
	}
	
	for(ParameterList::const_iterator it = get_parameter_list().begin(); it != get_parameter_list().end();++it){
	  (*it).second->tick(samplesRead);
	}
	
	samplesRead++;
	
}	
	
BandPassFilter::BandPassFilter(dspCreationStruct data):
	Filter(data)
{
	set_effect_name("BandPassFilter");
	add_parameter("Frequency",10.0f,20000,440.0f);
	add_parameter("Quality of Filter",0.1f,10.0f,5.0f);
	
	set_meta(get_tutId(),"link to html");
	get_parameter("Quality of Filter")->set_meta("GuiHint","soCustomFader");
	get_parameter("Frequency")->set_meta("GuiHint","soCustomFader");
	get_biquad_left().set_BPF(440,1);
	get_biquad_right().set_BPF(440,1);
	set_implementation();
	
} 	
	

BandPassFilter::~BandPassFilter(){
	
}

void BandPassFilter::one_shot(){
	float f = get_parameter("Frequency")->get_value();
	float q = get_parameter("Quality of Filter")->get_value();
	get_biquad_left().set_BPF(f,q);
	get_biquad_right().set_BPF(f,q);
}


void BandPassFilter::process(float** input, float** output, int frameSize, int channels) {
  
    SamplePosition samplesRead;

    Session& sess = fsom::Engine::get_instance().get_active_session();
    
    if(sess.get_preview_state() == false){
	samplesRead = get_creation_struct().attatchedRegion->get_sample_position();
	
    }else{
	samplesRead = sess.get_previed_playhead_value(); 
    }
  
    if(!bypass_active()){
	FSOM_ASSERT(channels == 2);
	float f = get_parameter("Frequency")->get_value();
	float q = get_parameter("Quality of Filter")->get_value();
	get_biquad_left().set_BPF_const_gain(f,q);
	get_biquad_right().set_BPF_const_gain(f,q);
 	get_biquad_left().process(input[0],output[0],frameSize);
 	get_biquad_right().process(input[1],output[1],frameSize);
    }else{
	    output[0] = input[0];
	    output[1] = input[1];
    }
    
    for(ParameterList::const_iterator it = get_parameter_list().begin(); it != get_parameter_list().end();++it){
	  (*it).second->tick(samplesRead);
    }
	
    samplesRead++;
	
}	
	
CombFilter::CombFilter(dspCreationStruct data):
	DSPEffect(data),
	m_delayUnitL(2048),
	m_delayUnitR(2048)
{
	set_effect_name("CombFilter");
	add_parameter("Dry Amount",0.0f,1.0f,0.9f);
	get_parameter("Dry Amount")->set_meta("GuiHint","soCustomFader");
	add_parameter("Delay Time",10,800,10);
	get_parameter("Delay Time")->set_meta("GuiHint","soCustomFader");
	add_parameter("Feedback",0.0f,0.99f,0.68f);
	get_parameter("Feedback")->set_meta("GuiHint","soCustomFader");
	add_parameter("Feedforward",0.0f,0.99f,0.68f);
	get_parameter("Feedforward")->set_meta("GuiHint","soCustomFader");
	set_implementation();
	
}

CombFilter::~CombFilter(){}

void CombFilter::one_shot(){}

void CombFilter::process(float** input, float** output, int frameSize, int channels) {
	
	FSOM_ASSERT(channels == 2);
	
	  if(!bypass_active()){
	      float feedforward = get_parameter("Feedforward")->get_value();
	      float feedback = get_parameter("Feedback")->get_value();
	      float dry = get_parameter("Dry Amount")->get_value();
	      float wet = 1.0f - dry;
		  DelayBase<float>::sample_index dt = truncate_to_integer<DelayBase<float>::sample_index>(get_parameter("Delay Time")->get_value());
	      for (int n = 0; n < frameSize; ++n){
		      output[0][n] = m_delayUnitL.read_sample(dt) * wet + input[0][n]*dry;	
		      output[1][n] = m_delayUnitR.read_sample(dt) * wet+ input[1][n]*dry;
		      m_delayUnitL.write_sample(output[0][n]*feedback + input[0][n]);
		      m_delayUnitR.write_sample(output[1][n]*feedback + input[1][n]);
		      output[0][n] = m_delayUnitL.read_sample(dt) * wet + input[0][n]*dry;	
		      output[1][n] = m_delayUnitR.read_sample(dt) * wet+ input[1][n]*dry;
		      m_delayUnitL.write_sample(output[0][n]*feedforward + input[0][n]);
		      m_delayUnitR.write_sample(output[1][n]*feedforward + input[1][n]);
		      m_delayUnitL.tick();
		      m_delayUnitR.tick();
	      }
	      
	}else{
	    output[0] = input[0];
	    output[1] = input[1];
	}
}


AllPassFilter::AllPassFilter(dspCreationStruct data):
	Filter(data)
{
	set_effect_name("AllPassFilter");
	add_parameter("Frequency",10.0f,20000,440.0f);
	add_parameter("Quality of Filter",0.1f,10.0f,5.0f);
	
	set_meta(get_tutId(),"link to html");
	get_parameter("Quality of Filter")->set_meta("GuiHint","soCustomFader");
	get_parameter("Frequency")->set_meta("GuiHint","soCustomFader");
	get_biquad_left().set_ALL(440,1);
	get_biquad_right().set_ALL(440,1);
	set_implementation();
	
} 	
	

AllPassFilter::~AllPassFilter(){
	
}

void AllPassFilter::one_shot(){
	float f = get_parameter("Frequency")->get_value();
	float q = get_parameter("Quality of Filter")->get_value();
	get_biquad_left().set_ALL(f,q);
	get_biquad_right().set_ALL(f,q);
}


void AllPassFilter::process(float** input, float** output, int frameSize, int channels) {
    if(!bypass_active()){
	FSOM_ASSERT(channels == 2);
	float f = get_parameter("Frequency")->get_value();
	float q = get_parameter("Quality of Filter")->get_value();
	get_biquad_left().set_ALL(f,q);
	get_biquad_right().set_ALL(f,q);
 	get_biquad_left().process(input[0],output[0],frameSize);
 	get_biquad_right().process(input[1],output[1],frameSize);
    }else{
	    output[0] = input[0];
	    output[1] = input[1];
	}	
}


MultiBandFilter::MultiBandFilter(dspCreationStruct data):DSPEffect(data), m_biquadAmount(6),m_combinedAmps(0){
    set_effect_name("MultiBandFilter");
    add_parameter("Low Shelf Frequency",5,10000.0f,50.0f);
    add_parameter("Low Shelf Quality",0.2f,10.0f,1);
    
    add_parameter("Band Pass Frequency",500,20000.0f,5000.0f);
    add_parameter("Band Pass Quality",0.2f,10.0f,1);
    
    add_parameter("High Shelf Frequency",50,20000.0f,440.0f);
    add_parameter("High Shelf Quality",0.2f,10.0f,1);
    
    set_meta(get_tutId(),"link to html");	
    get_parameter("Low Shelf Frequency")->set_meta("GuiHint","soCustomFader");
    get_parameter("Low Shelf Quality")->set_meta("GuiHint","soCustomFader");
    
    get_parameter("Band Pass Frequency")->set_meta("GuiHint","soCustomFader");
    get_parameter("Band Pass Quality")->set_meta("GuiHint","soCustomFader");
    
    get_parameter("High Shelf Frequency")->set_meta("GuiHint","soCustomFader");
    get_parameter("High Shelf Quality")->set_meta("GuiHint","soCustomFader");
    
    for(int n=0;n < m_biquadAmount; n++){
	m_biquadList.push_back(BiquadPtr(new Biquad(44100)));
    }
    
    m_biquadList.at(0)->set_HPF(10000.0f,5.0f);
    m_biquadList.at(1)->set_HPF(10000.0f,5.0f);
    
    m_biquadList.at(2)->set_BPF(5000.0f,5.0f);
    m_biquadList.at(3)->set_BPF(5000.0f,5.0f);
    
    m_biquadList.at(4)->set_LPF(440.0f,5.0f);
    m_biquadList.at(5)->set_LPF(440.0f,5.0f);
    
    one_shot(0);
    
    set_implementation();
}

void MultiBandFilter::process(float** input, float** output, int frameSize, int channels){
  if(!bypass_active()){
    FSOM_ASSERT(channels == 2);
    
    float lpf = get_parameter("Low Shelf Frequency")->get_value();
    float lpq = get_parameter("Low Shelf Quality")->get_value();
    
    float bpf = get_parameter("Band Pass Frequency")->get_value();
    float bpq = get_parameter("Band Pass Quality")->get_value();
    
    float hpf = get_parameter("High Shelf Frequency")->get_value();
    float hpq = get_parameter("High Shelf Quality")->get_value();
    
      m_biquadList.at(0)->set_LPF(lpf,lpq);
      m_biquadList.at(1)->set_LPF(lpf,lpq);
      
      m_biquadList.at(2)->set_BPF(bpf,bpq);
      m_biquadList.at(3)->set_BPF(bpf,bpq);
      
      m_biquadList.at(4)->set_HPF(hpf,hpq);
      m_biquadList.at(5)->set_HPF(hpf,hpq);
    
      for(int n = 0; n < m_biquadAmount; n+=2){
	m_biquadList.at(n)->process(input[0],output[0],frameSize);
	m_biquadList.at(n+1)->process(input[1],output[1],frameSize);
      }
      
  }else{
      output[0] = input[0];
      output[1] = input[1];
  }
  
}

void MultiBandFilter::one_shot(int steps){
    float lpf = get_parameter("Low Shelf Frequency")->get_value();
    float lpq = get_parameter("Low Shelf Quality")->get_value();
    
    float bpf = get_parameter("Band Pass Frequency")->get_value();
    float bpq = get_parameter("Band Pass Quality")->get_value();
    
    float hpf = get_parameter("High Shelf Frequency")->get_value();
    float hpq = get_parameter("High Shelf Quality")->get_value();
    
      m_biquadList.at(0)->set_LPF(lpf,lpq);
      m_biquadList.at(1)->set_LPF(lpf,lpq);
      
      m_biquadList.at(2)->set_BPF(bpf,bpq);
      m_biquadList.at(3)->set_BPF(bpf,bpq);
      
      m_biquadList.at(4)->set_HPF(hpf,hpq);
      m_biquadList.at(5)->set_HPF(hpf,hpq);
      
      float combinedValue = 0.0f;
      m_combinedAmps.clear();
      
      for(int i = 0; i < steps; ++i){
	  m_combinedAmps.push_back(1.0f);
      }
      for(int j = 0; j < steps; ++j){
	  combinedValue = 0.0f;
	  for(int i = 0; i < m_biquadAmount; ++i){
	    combinedValue +=  m_biquadList.at(i)->get_amplitudes(steps).at(j);
	  }
	  m_combinedAmps.at(j) = combinedValue/m_biquadAmount;
      }
}

  
const std::vector<float>& MultiBandFilter::get_combined_amplitudes(){
       
  return m_combinedAmps;
}

}