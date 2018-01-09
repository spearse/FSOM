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



#include "../../include/fsom/EffectClasses/Reverb.hpp"
#include <fsom/Region.hpp>

#include <fsom/Session.hpp>
#include <fsom/Engine.hpp>

namespace fsom{
Reverb::Reverb(dspCreationStruct data):
	DSPEffect(data)
{
  set_effect_name("Reverb");
  
  add_parameter("Room Size",0,1,0.5f);
  get_parameter("Room Size")->set_meta("GuiHint","soCustomFader");
  
  add_parameter("Damping",0,1,0.5f);
  get_parameter("Damping")->set_meta("GuiHint","soCustomFader");
  
  add_parameter("Reverb Amount",0,1,0.8f);
  get_parameter("Reverb Amount")->set_meta("GuiHint","soCustomFader");
  
//  add_parameter("Freeze Amount",0,1,0.1);
  set_implementation();

  data.attatchedRegion->set_extension(44100*10);
  
}

Reverb::~Reverb()
{

}

void Reverb::process(float** input, float** output, int frameSize, int channels) {
// 	    m_revUnit.

    SamplePosition samplesRead;
    
    Session& sess = fsom::Engine::get_instance().get_active_session();
    
    if(sess.get_preview_state() == false){
	samplesRead = get_creation_struct().attatchedRegion->get_sample_position();
	
    }else{
	samplesRead = sess.get_previed_playhead_value(); 
    }  

  if(!bypass_active()){
    m_revUnit.setdamp(get_parameter("Damping")->get_value());
    m_revUnit.setdry(1.0f -  get_parameter("Reverb Amount")->get_value());
    m_revUnit.setwet(get_parameter("Reverb Amount")->get_value());
    m_revUnit.setroomsize(get_parameter("Room Size")->get_value());

  //  m_revUnit.setmode(get_parameter("Freeze Amount")->get_value());
    
    m_revUnit.processreplace(input[0],input[1],output[0],output[1],frameSize,1);
    
      for(ParameterList::const_iterator it = get_parameter_list().begin(); it != get_parameter_list().end();++it){
	      (*it).second->tick(samplesRead);
      }
	  
      samplesRead++;
    
  }else{
    output[0] = input[0];
    output[1] = input[1];
  }
}



//---------------SIMPLE REVERB---------------------------------------------//



SimpleReverb::SimpleReverb(dspCreationStruct data):
	DSPEffect(data)
{
  set_effect_name("SimpleReverb");
  add_parameter("Room Size",0,1,0.5);

  set_meta(get_tutId(),"link to html");
  get_parameter("Room Size")->set_meta("GuiHint","soCustomFader");
  
  set_implementation();

  data.attatchedRegion->set_extension(44100*10);
  
}

SimpleReverb::~SimpleReverb()
{

}

void SimpleReverb::process(float** input, float** output, int frameSize, int channels) {
// 	    m_revUnit.

    SamplePosition samplesRead;
    
    Session& sess = fsom::Engine::get_instance().get_active_session();
    
    if(sess.get_preview_state() == false){
	samplesRead = get_creation_struct().attatchedRegion->get_sample_position();
	
    }else{
	samplesRead = sess.get_previed_playhead_value(); 
    }  

  if(!bypass_active()){
    m_revUnit.setdamp(1.0);
    m_revUnit.setdry(0.0);
    m_revUnit.setwet(1.0);
    m_revUnit.setroomsize(get_parameter("Room Size")->get_value());

  //  m_revUnit.setmode(get_parameter("Freeze Amount")->get_value());
    
    m_revUnit.processreplace(input[0],input[1],output[0],output[1],frameSize,1);
    
      for(ParameterList::const_iterator it = get_parameter_list().begin(); it != get_parameter_list().end();++it){
	      (*it).second->tick(samplesRead);
      }
	  
      samplesRead++;
    
  }else{
    output[0] = input[0];
    output[1] = input[1];
  }
    
}



}