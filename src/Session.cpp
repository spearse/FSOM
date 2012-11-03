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



#include <boost/bind.hpp>
#include <sndfile.h>

#include "tinyxml/tinyxml.h"
#include "fsom/Session.hpp"
#include "fsom/Engine.hpp"
#include "fsom/AudioRegion.hpp"
#include <fsom/RegionManager.hpp>
#include "fsom/SynthesisModuleManager.hpp"
#include <algorithm>
#include <cassert>
#include <sstream>
#include "fsom/SynthesisRegion.hpp"

using namespace fsom;

/// temporary phasor objects to play around with
Phasor g_p1(44100,440);
Phasor g_p2(44100,330);

class XMLParseException{};

Session::Session() :
	m_audioMutex( Mutex::create() ),
	m_transportIsRolling(false),
	m_playHead(0),
	m_previewPlayHead(0),
	m_previewState(false),
	m_playbackDuration( (60*5)*44100), ///This currently sets the duration of entire session,
	m_loopPreviewState(true),
	m_leftLocator(0),
	m_rightLocator(44100),
	m_loopState(false)
{
	DSPManager::get_instance();
	assert(m_audioMutex && "Audio mutex not created");
}

Session::~Session(){
  Mutex::destroy(m_audioMutex); 
}

// Session::Session(std::string preset_path){

// }

ParameterPtr Session::create_parameter_from_node(TiXmlElement* element,Region* region){
	assert(element);
	std::string id = element->Attribute("Id");
	assert(id != "");
	float value(0),lowerBound(0),upperBound(1.0);
	element->QueryFloatAttribute("Value", &value);
// 	assert(value);
	element->QueryFloatAttribute("LowerBound", &lowerBound);
// 	assert(value);
	element->QueryFloatAttribute("UpperBound", &upperBound);
// 	assert(value);
	
	
	BreakPointUnitPtr tempBPUnit = BreakPointUnitPtr(new BreakPointUnit());    
	
	TiXmlElement * bpChild = element->FirstChildElement("Breakpoint");
	
	double bpVal;
	int bpPos;
	
	while(bpChild){  
	      
	      bpChild->QueryDoubleAttribute("Val", &bpVal);
	      bpChild->QueryIntAttribute("Pos", &bpPos);
	
	      tempBPUnit->add_breakpoint(TVPair(bpPos,bpVal));
	 	
	      bpChild = bpChild->NextSiblingElement("Breakpoint");
	}	
	
	tempBPUnit->sort();
	
	ParameterPtr t = ParameterPtr(new Parameter(region->get_duration(), id, lowerBound,upperBound,value, tempBPUnit));

	return t;

}

DSPEffectPtr Session::create_effect_from_node(TiXmlElement* element,Region* region){

  assert(element);
  std::string typeName = element->Attribute("name");
  DSPEffectPtr t =  DSPManager::get_instance().create(typeName,dspCreationStruct(region));
  assert(t);
  TiXmlElement * meta = element->FirstChildElement("MetaData");
  //TODO streamline this process of loading, specific function for loading dspeffect and region meta needed

  const char* tut = meta->Attribute("TutorialLink"); 
  
  t->register_meta("TutorialLink");
  t->set_meta("TutorialLink",std::string(tut));

  TiXmlElement * child = element->FirstChildElement("Parameter");
  while(child){
    ParameterPtr p = create_parameter_from_node(child, region);
    t->get_parameter(p->get_name())->set_value(p->get_value());
    t->get_parameter(p->get_name())->set_breakpoints(p->get_breakpoints());
    child = child->NextSiblingElement("Parameter");
  }
  std::cout  << "Loaded effect:" << typeName << std::endl;
  return t;
  
}

GeneratorPtr Session::create_generator_from_node(TiXmlElement* element, Region* region){
    std::cout << "Attempting to create generator"<<std::endl;
    GeneratorPtr gen = GeneratorPtr(new Generator(Generator::GEN_Sine,dspCreationStruct(region)));
    
    TiXmlElement* basicInfoElement = element->FirstChildElement("BasicInfo");
    if(basicInfoElement){
	std::cout << "Generator basic info found"<<std::endl;
	int genType,noiseState;
	std::string path = basicInfoElement->Attribute("Path"); 
	basicInfoElement->QueryIntAttribute("GenType",&genType);
	basicInfoElement->QueryIntAttribute("NoiseState",&noiseState);
	Generator::GeneratorType type = Generator::GeneratorType(genType);
	gen->set_generator_voice(type);
	gen->set_noise_state(noiseState);
	gen->set_file_path(path);
	std::cout << "noise state for gen set to "<<noiseState<<std::endl;
	std::cout << "gen type = "<<genType<<std::endl;
	
	
	
    }
    
    TiXmlElement * child = element->FirstChildElement("Parameter");
    while(child){
      std::cout << "Parameter in generator found"<<std::endl;
      ParameterPtr p = create_parameter_from_node(child, region);
      gen->get_parameter(p->get_name())->set_value(p->get_value());
      child = child->NextSiblingElement("Parameter");
    }
    
    return gen;
}

SynthesisModulePtr Session::create_module_from_node(TiXmlElement* element, Region* region){
  std::cout << "Attempting to create module from node"<<std::endl;
   TiXmlElement* basicInfoElement = element->FirstChildElement("BasicInfo");
   SynthesisModulePtr module;
  if(basicInfoElement){
   std::cout << "Module basic info found"<<std::endl;
   std::string name = basicInfoElement->Attribute("Type");
   module = SynthesisModuleManager::get_instance().create(name,dspCreationStruct()); 
    TiXmlElement * child = element->FirstChildElement("Parameter");
    while(child){
      std::cout << "Parameter in module found"<<std::endl;
      ParameterPtr p = create_parameter_from_node(child, region);
      module->get_parameter(p->get_name())->set_value(p->get_value());
      child = child->NextSiblingElement("Parameter");
    }
   
   
   return module;
  }
  
  ///WARNING POSSIBLE TO RETURN WITHOUT A VALUE 
  //   SynthesisModulePtr 
  
  
}


RegionPtr Session::create_region_from_node(TiXmlElement* element){
  assert(element);
  TiXmlElement * basicInfoElement = element->FirstChildElement("BasicInfo");
  assert(basicInfoElement);
  int start = 0;
  basicInfoElement->QueryIntAttribute("start", &start);
  int duration = 0;
  
  basicInfoElement->QueryIntAttribute("duration", &duration);
  int offset = 0;
  basicInfoElement->QueryIntAttribute("offset", &offset);
  int lanenum = 0;
  
  basicInfoElement->QueryIntAttribute("lanenum", &lanenum);
	int extension = 0;
  basicInfoElement->QueryIntAttribute("extension", &extension);
 
  int reverseState =0;
  basicInfoElement->QueryIntAttribute("reversestate",&reverseState);
  
  int muteState = 0;
  basicInfoElement->QueryIntAttribute("mutestate",&muteState);

  std::string path = basicInfoElement->Attribute("path");
  std::cout << "Loading, working directory = "<< m_workingDirectory<<std::endl;
  regionCreationStruct cs(start, duration,offset,lanenum,extension, path,m_workingDirectory,bool(reverseState));
  
  TiXmlElement * meta = element->FirstChildElement("MetaData");

  RegionPtr pRegion = RegionManager::get_instance().create(meta->Attribute("RegionType"),cs);
  pRegion->set_mute_state(muteState);
  assert(pRegion);
  
  pRegion->register_meta("RegionType");
  pRegion->register_meta("Tip");

  pRegion->set_meta("RegionType", meta->Attribute("RegionType"));
  pRegion->set_meta("Tip", meta->Attribute("Tip"));
  
  std::string image = meta->Attribute("image");
  std::string managerId = meta->Attribute("managerId");
  pRegion->register_meta("image");
  pRegion->register_meta("managerId");
  pRegion->set_meta("image",image);
  pRegion->set_meta("managerId",managerId);
  std::cout << "Metadata read, image = " <<image<<std::endl;
  
  TiXmlElement * effectChild = element->FirstChildElement("Effect");
  
  while(effectChild){
    
	DSPEffectPtr e = create_effect_from_node(effectChild,pRegion.get());
	pRegion->attach_effect(e);
	effectChild = effectChild->NextSiblingElement("Effect");
	
  }
  
  std::cout  << "Loaded region:" << path << std::endl;
  
  if(pRegion->get_meta("RegionType") != "Audio" && pRegion->get_meta("RegionType") != "WhiteNoise"){
      std::cout << "Synthesis region found"<<std::endl;
      SynthesisRegionPtr synthregion = boost::dynamic_pointer_cast<fsom::SynthesisRegion>(pRegion); 
      //Remove the automatic generators
      synthregion->remove_all_generators();
      //remove the automatic modules
      synthregion->remove_all_modules();
      
      TiXmlElement* generatorElement = element->FirstChildElement("Generator");
      while(generatorElement){
	GeneratorPtr gen = create_generator_from_node(generatorElement,pRegion.get());
	synthregion->add_generatorPtr(gen);
	generatorElement = generatorElement->NextSiblingElement("Generator");
      }
      std::cout << "No of generators spawned =  "<< synthregion->get_generator_stack().size()<<std::endl;
      TiXmlElement* moduleElement = element->FirstChildElement("Module");
      while(moduleElement){
	SynthesisModulePtr mod = create_module_from_node(moduleElement,pRegion.get());
	synthregion->add_modulePtr(mod);
	moduleElement = moduleElement->NextSiblingElement("Module");
      }
//       pRegion = synthregion;
  }
  
  
  
  
  return RegionPtr(pRegion);
}





void Session::load_session(const char* fileLocation){
	std::vector<std::string> temp_effect_names;
	TiXmlDocument doc(fileLocation);
	std::cout << "Working dir in load session= "<<m_workingDirectory<<std::endl; 
	if(doc.LoadFile()){
		TiXmlHandle docHandle( &doc );
		TiXmlElement* sessionElement = docHandle.FirstChild( "Session" ).Element();
		if(sessionElement){
			TiXmlElement* regionElement = sessionElement->FirstChildElement("Region");
			while(regionElement){
				RegionPtr t = create_region_from_node(regionElement);
				add_region(t);
				regionElement = regionElement->NextSiblingElement("Region");
			}
		} else {
			throw XMLParseException();
		}
	}
}

void Session::save_session(const char* fileLocation){
	TiXmlDocument doc(fileLocation);
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );
	TiXmlElement * root = new TiXmlElement( "Session" );  
	doc.LinkEndChild( root );  
	//FIXME sort out upper and lower bound in parameter
	for (std::list<RegionPtr>::iterator it=m_regionPlaylist.begin();it != m_regionPlaylist.end();++it){
		(*it)->save_to_xml_node(root);
	}
	doc.SaveFile( fileLocation );  



/*
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );
	
	TiXmlElement * element = new TiXmlElement( "Hello" );
	doc.LinkEndChild( element );
	
	TiXmlText * text = new TiXmlText( "World" );
	element->LinkEndChild( text );
	
	doc.SaveFile( "madeByHand2.xml" );

*/
}

class SessionEventSortPredicate{
public:
	bool operator()(const SessionEvent& lhs, const SessionEvent& rhs){
		return lhs.time < rhs.time;
	}
};

void Session::seek(SamplePosition seekTarget){
  
	//Engine::get_instance().clear_buffers();
	// set the current playback position and prepare to play
	// sort out which regions should be setup in the active regions list
	// make sure each region has its position set to the correct offset for this seek position.
	//		i.e. for each region in activeRegions region->seek(seekTarget)

	// we must prepare the event cache ready for playback
	// make a new cache then swap when done minimising the transition time.
	SessionEventList tempEvCache;
	
	// look for start events
	SessionEvent ev;
	ev.type = SE_REGION_START; //check to see if needs to be here
	for(RegionList::iterator it = m_regionPlaylist.begin(); 
			it != m_regionPlaylist.end();
			++it){
		if( (*it)->get_start_pos() >= seekTarget){ // only add future events
			// each region has both a start and an end so we do both
			// start
			ev.type = SE_REGION_START;
			ev.region = *it;
			ev.time = (*it)->get_start_pos();
			ev.value = 0.0f; // not used in this event type
			ev.user = 0; // not used in this event type
			tempEvCache.push_back(ev);
			
			//extension
			ev.type = SE_REGION_EXTENSION;
			ev.region = *it;
			ev.time = (*it)->get_start_pos()+(*it)->get_duration();
			ev.value = 0.0f;
			ev.user = 0;
			tempEvCache.push_back(ev);
			
			// end
			ev.type = SE_REGION_END;
			ev.region = *it;
			//for extension
			ev.time = (*it)->get_start_pos() + (*it)->get_duration() + (*it)->get_extension(); // calculate absolute end time
			ev.value = 0.0f; // not used in this event type
			ev.user = 0; // not used in this event type
			tempEvCache.push_back(ev);
			
		}
		else if((*it)->get_start_pos() < seekTarget && (*it)->get_start_pos() + (*it)->get_duration() > seekTarget){
			      
			 ev.type = SE_REGION_ACTIVE;
			 ev.region = *it;
			 //TODO look into....
			 ev.time = seekTarget;
			 ev.value = 0.0f; // not used in this event type
			 ev.user = 0; // not used in this event type
			 tempEvCache.push_back(ev);
			 
			//extension
			ev.type = SE_REGION_EXTENSION;
			ev.region = *it;
			ev.time = (*it)->get_start_pos()+(*it)->get_duration();
			ev.value = 0.0f;
			ev.user = 0;
			tempEvCache.push_back(ev);
			 
			 ev.type = SE_REGION_END;
			 ev.region = *it;
			 ev.time = (*it)->get_start_pos() + (*it)->get_duration() + (*it)->get_extension() ; // calculate absolute end time
			 ev.value = 0.0f; // not used in this event type
			 ev.user = 0; // not used in this event type
			tempEvCache.push_back(ev);
		}
	}
	
	if(m_loopState){
	  ev.type = SE_SESSION_LOOP;
	  ev.time = m_rightLocator;
	  tempEvCache.push_back(ev);
	}
	// now sort the event cache by time order ready for playback.
	tempEvCache.sort(SessionEventSortPredicate());

	// finally swap it in and get an iterator to the first one to be dealt with
	m_eventCache.swap(tempEvCache);
	m_nextEvent = m_eventCache.begin();
	m_playHead = seekTarget;
}

void Session::play(){
	// set transport state to play mode
	// playback will commence on next process buffer.
	m_transportIsRolling = true;
}

void Session::stop(){
	// set transport state to stoped
	// playback will stop on the next process block.
	clear_all_active_regions();//clear all active regions TODO needs to be more sophisticated to handle stop and continue
	m_transportIsRolling = false;
}


void Session::add_active_region(Region* p){
	p->on_region_start(m_playHead);
	m_activeRegions.push_back(p);
	std::cout << "activated region" << std::endl;
}

void Session::remove_active_region(Region* p){
	m_activeRegions.remove(p);
	std::cout << "deactivated region" << std::endl;
}


void Session::clear_all_active_regions(){
	m_activeRegions.clear(); 
	std::cout << "clearing all active regions" << std::endl;
}



void Session::internal_process(float** ins, float** outs, int frameCount, int channelCount, size_t offset, SamplePosition globalTime){
	float** offsetOutputs;
	if(offset > 0){
		std::cout << "handling offset process block due to event with offset=" << offset << std::endl;
		offsetOutputs = get_multichannel_offset_ptrs(outs,channelCount,offset);
	} else {
		offsetOutputs = outs;
	}
	for_each(m_activeRegions.begin(), m_activeRegions.end(), 
		bind(&Region::process_region, _1, ins, offsetOutputs, frameCount, channelCount,globalTime)
	);
	m_playHead += frameCount;
}

void Session::process(float** ins,float** outs,int frameCount,int channelCount){
	assert(m_audioMutex);
	if ( ! m_audioMutex->try_lock() ) {
	  clear_multichannel_buffers(outs,channelCount,frameCount); 
	  return;
	}
	// write silence into the output buffers so that we can sum (mix) onto them with +=
	clear_multichannel_buffers(outs,channelCount,frameCount); 
	if(m_transportIsRolling && m_playHead < m_playbackDuration){
		float sr = Engine::get_instance().get_sample_rate();
		//std::cout << "Transport rolling: t="<< m_playHead << " " << float(m_playHead)/sr << std::endl;
		//simple idea for looping
		if(m_loopState &&  m_playHead >= m_rightLocator){
		    /*
		    m_playHead = m_leftLocator;
		    seek(m_leftLocator);
		    */
		}
		SamplePosition endOfBlockTime = m_playHead + frameCount;
		
		// enter the event processing loop, up until the end of the block or until
		while(m_playHead < endOfBlockTime){
			size_t offset = frameCount - (endOfBlockTime - m_playHead);
			
// 			std::cout << "m_nextEvent->time = " << m_nextEvent->time << std::endl;
		
			if(m_nextEvent != m_eventCache.end() && m_nextEvent->time < endOfBlockTime){
				// process up until the event
				SamplePosition t = m_nextEvent->time - m_playHead;
				if(t > 0){ // only process when there is time until the next event 
					internal_process(ins, outs, m_nextEvent->time - m_playHead, channelCount, offset,m_playHead);
				}
				// deal with the event
				switch(m_nextEvent->type){
					case SE_REGION_START:
						// an region must be added to the active regions
						add_active_region(m_nextEvent->region.get());
						m_nextEvent->region.get()->set_extension_mode(false);
						// jump to the next event in the cache
						++m_nextEvent;
						break;
					case SE_REGION_END:
						// a region should be removed from the active regions
						remove_active_region(m_nextEvent->region.get());
						// jump to the next event in the cache
						++m_nextEvent;						
						break;
					case SE_REGION_EXTENSION:
						//add region for extension mode sp
						m_nextEvent->region.get()->set_extension_mode(true);
						// jump to the next event in the cache
						++m_nextEvent;						
						break;
					case SE_REGION_ACTIVE:
						add_active_region(m_nextEvent->region.get());
						m_nextEvent->region.get()->set_extension_mode(false);
						// jump to the next event in the cache
						++m_nextEvent;						
						break;
					case SE_SESSION_LOOP:
						{
						  int remaining = endOfBlockTime - m_playHead;
						  m_activeRegions.clear();
						  seek(m_leftLocator);
						  endOfBlockTime = remaining + m_leftLocator;
						}
						break;
					default: 
						assert(false && "Unknown SessionEventType");
				}
			} else {
				// process remaining block
				internal_process(ins, outs, endOfBlockTime - m_playHead, channelCount, offset,m_playHead);
			}
		}
	}	
	
	if(m_previewState){  
		  if(m_previewPlayHead < m_previewRegion->get_duration() + m_previewRegion->get_extension()){
			  m_previewRegion->process(ins,outs,frameCount,channelCount);
			  m_previewPlayHead += frameCount;
		  }else {
		      if(m_loopPreviewState ==true){			
			  m_previewPlayHead = 0;
			  m_previewRegion->on_region_start(0);

		      }else{
			  m_previewState = false;
		      }
		  }
	}
	
	// now unlock the mutex
	m_audioMutex->unlock();
}


/// this is a utility predicate class for time and duration sort of regions.
/// see also Session::sort_regions()
class RegionSortPredicate{
public:
	bool operator()(const RegionPtr& lhs, const RegionPtr& rhs){
		if(lhs->get_start_pos() == rhs->get_start_pos()) return lhs->get_duration() < rhs->get_duration();
		return lhs->get_start_pos() < rhs->get_start_pos();
	}
};

void Session::sort_regions(){
	m_regionPlaylist.sort(RegionSortPredicate());
}

void Session::add_region(RegionPtr region){
  	assert(region && "Remove Region: RegionPtr Invalid");
 	ScopedMutexLock lock(m_audioMutex);
	m_regionPlaylist.push_back(region);
	sort_regions();
}

void Session::remove_region(RegionPtr region){
  	ScopedMutexLock lock(m_audioMutex);
	assert(region && "Remove Region: RegionPtr Invalid");
	
	assert(std::find(m_regionPlaylist.begin(), m_regionPlaylist.end(), region) != m_regionPlaylist.end()) ;
	m_regionPlaylist.remove(region);
	region.reset();
}



void Session::print_region_playlist(){
	using namespace boost;
	for_each(m_regionPlaylist.begin(), m_regionPlaylist.end(), bind(&Region::print_region_info,_1));
}


int Session::get_list_size(){
	return m_regionPlaylist.size();
}

RegionList& Session::get_region_list(){
	return m_regionPlaylist;
}

void Session::bounce_session(std::string filepath, Session::FileType type, bool useLocators){

	if(!m_regionPlaylist.empty()){
	    
	  
	  
	    std::cout << "Bouncing Session" <<std::endl;
	    SNDFILE* outfile;
	    SF_INFO m_info;
	    m_info.channels = 2;
	    switch(type){
	      case(FT_WAV):
		  m_info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16 | SF_ENDIAN_LITTLE;
		  break;
	      case(FT_OGG):
		  m_info.format = SF_FORMAT_OGG | SF_FORMAT_PCM_16 | SF_ENDIAN_LITTLE;
		  break;
	      case(FT_AIFF):
		  m_info.format = SF_FORMAT_AIFF | SF_FORMAT_PCM_16 | SF_ENDIAN_LITTLE;
		  break;
	      case(FT_FLAC):
		  m_info.format = SF_FORMAT_FLAC | SF_FORMAT_PCM_16 | SF_ENDIAN_LITTLE;
		  break;
	    };
	    m_info.samplerate = 44100;
	    outfile = sf_open(filepath.c_str(),SFM_WRITE,&m_info);
	    assert(outfile);
	    size_t frames;
	    size_t start;
	    if(!useLocators){
		//size_t frames = m_info.samplerate * 20; //TODO make arbitrary numbers in bouncedown and session playback the same.//FIXME Stopping through regions causes serious glitching on playback or exporting
		frames = m_regionPlaylist.back()->get_start_pos() + m_regionPlaylist.back()->get_duration() + m_regionPlaylist.back()->get_extension(); //5 seconds removed to give accurate bouncedown for single region
		start = 0;
	    }else{
		frames = m_rightLocator;
		start = m_leftLocator;
		
	    }
	    std::cout << "Filesize = " << frames << " frames\t Start = " << start <<std::endl;
// 	    int blocksize = 512;
    // 	SamplePosition bouncePosition = 0; 
	    const int frameSize = 512;
	    MultiChannelBuffer deint(m_info.channels,frameSize);
	    
	    float* output = new float[frameSize*m_info.channels];
	    Engine::get_instance().stop();
	    bool prevLoopState = get_loop_state(); 
	    set_loop_state(false);
	  
	    seek(start);
	    play();
	    m_previewState = false;
	    
	    for (int n = start; n < frames; n += frameSize){
		    process(0,deint.get_buffers(),frameSize,m_info.channels);
		    interleave((const float**)deint.get_buffers(),output,m_info.channels,frameSize);
		    sf_writef_float(outfile,output,frameSize);
		    std::cout << " Export frame " << n << " : frames = "<< frames << std::endl;
	    }
	    //FIXME this looses data if the num frames isnt perfectly divisible by the framesize.
	    size_t remainingblock = frames % frameSize;
	    if(remainingblock > 0){
		std::cout << "Remainder = " << remainingblock<<"\n\n\n\n"<<std::endl;
		process(0,deint.get_buffers(),remainingblock,m_info.channels);
		interleave((const float**)deint.get_buffers(),output,m_info.channels,remainingblock);
		sf_writef_float(outfile,output,remainingblock);
	    }
	    delete [] output;
    // 	
	    sf_close(outfile);
	    set_loop_state(prevLoopState);
	    stop();
	    seek(0);
	    Engine::get_instance().start();	
	}
}
void Session::set_preview_region(fsom::RegionPtr region){
	m_previewRegion = region;
	m_previewRegion->reset_all_effects();
	m_previewRegion->on_region_start(0);

}
void Session::set_preview_state(bool state){
	if (m_previewRegion) m_previewRegion->on_region_start(0);
	m_previewState = state;
	m_previewPlayHead = 0;
// 	std::cout << "Preview state set to " << m_previewState <<std::endl;
}

bool Session::get_preview_state(){
	return m_previewState;
}

SamplePosition& Session::get_playhead_value(){
	return m_playHead;
}

const SampleLength& Session::get_playback_duration() const{
  return m_playbackDuration;
}


void Session::bounce_region(RegionPtr region, std::string filename, Session::FileType type){
  
	//use the bounce session to create file
	//e.g Session temp;
	//add the region 
	//temp->add_region(region)
	//bounce down
	//temp->bounce_session(filename)
	//change original region to the new soundfile.
	SamplePosition storedPosition = region->get_start_pos();
	Session temp;
	temp.set_playback_duration(region->get_duration() + region->get_extension());
	region->set_start_pos(0);
	temp.add_region(region);
	temp.bounce_session(filename,type);
	region->set_start_pos(storedPosition);
}


RegionPtr Session::copy_region(const fsom::RegionPtr& region, SamplePosition position, std::string factoryName, int laneId){
	//copy region from existing
	fsom::RegionPtr newRegion = fsom::RegionManager::get_instance().create(factoryName,region->get_creation_struct());
    //
    
//	long end_point = region->get_start_pos()+region->get_duration();
	newRegion->set_start_pos(position);
	newRegion->set_offset_amount(region->get_offset_amount());
	newRegion->set_duration(region->get_duration());
	newRegion->set_extension(region->get_extension());
	newRegion->set_reverse_state(region->get_reverse_state());
	if(laneId != NULL){
	 newRegion->set_lane_num(laneId); 
	}
	//------------------------EFFECT STUFF ---------------------------------------------------------
	//TODO possibly unsafe way of "copying" DSPstack
	m_audioMutex->lock();
	int size = region->get_DSPStack().size();
	for( int n = 0; n < size; ++n){
		std::cout << "cloning fx " << region->get_DSPStack()[n]->get_effect_name() << std::endl;
		newRegion->attach_effect( region->get_DSPStack()[n]->clone() );
	}
	m_audioMutex->unlock();
	//----------------------------------------------------------------------------------------------
    //add the new region to the session
    //TODO SORT ME OUT!
	add_region(newRegion);
    //return the new region
    return newRegion;
}
	
RegionPtr Session::splice_region(fsom::RegionPtr region, SamplePosition splicePosition,std::string factoryName ){
	fsom::RegionPtr newRegion = copy_region(region,splicePosition,factoryName);
  
	if(factoryName =="Audio"){
	  
		//boost::shared_ptr<fsom::AudioRegion> origAudioRegion = boost::dynamic_pointer_cast<fsom::AudioRegion>(region);
		
		SamplePosition splicePortionA = splicePosition - region->get_start_pos();
		
		
		//TODO does this need to know about extension
		SamplePosition splicePortionB = region->get_duration() - splicePortionA;
		newRegion->set_duration(splicePortionB);

		region->set_duration(splicePortionA);
		
		newRegion->set_offset_amount(splicePortionA  + region->get_offset_amount());
		newRegion->set_extension(region->get_extension());
		
				
		return newRegion;//TODO sort this out needs better implementation
	}

    return newRegion;
}

bool Session::transport_is_rolling(){
	return m_transportIsRolling;
}

void Session::set_playback_duration(SampleLength duration){
  m_playbackDuration = duration;
}

void Session::set_working_directory(std::string directory){
  
    m_workingDirectory = directory;
    std::cout << "working dir set in session "<<directory<<std::endl;
}
	
std::string Session::get_working_directory()const{
  return m_workingDirectory;
}

bool Session::get_preview_loop_state(){
    return m_loopPreviewState;
}
	
void Session::set_preview_loop_state(bool state){
    m_loopPreviewState = state;
}
SamplePosition& Session::get_previed_playhead_value(){
    return m_previewPlayHead;
}

MultiTableBuffer Session::load_file_to_table(std::string path){
	SF_INFO sfInfo; //basic info of sound file
	SNDFILE* sndFile; //pointer to soundfile
	sndFile = sf_open(path.c_str(),SFM_READ,&sfInfo);    
	int frames = sfInfo.frames;
	int channels = sfInfo.channels;
	float* tempBuffer = new float[frames*channels];
	size_t ret =  sf_readf_float(sndFile,tempBuffer,frames);
// 	float size = float(frames)/float(channels);
	std::cout << "Importing " << path<<std::endl;
	std::cout << "framesize = "<< sfInfo.frames<<std::endl;
	std::cout << "size per channel = "<< frames<<std::endl;
	float** deInterleavedBuffer = new float*[channels] ;
	for (int n = 0; n < channels;++n){
	   deInterleavedBuffer[n] = new float[frames];
	}
	deinterleave(tempBuffer,deInterleavedBuffer,channels,frames);
	MultiTableBuffer buffer;
	for( int c =0;c<channels;++c){
	   TablePtr tempTable(new Table<double>(frames));
	    for(int f = 0;f<frames;++f){
		tempTable->get_table().at(f) = deInterleavedBuffer[c][f];

	    }

	    buffer.push_back(tempTable);
	}
	delete []tempBuffer;
	delete []deInterleavedBuffer;
	return buffer;
  
}

std::string Session::timestretch_region(RegionPtr region, double stretchAmount, std::string folderpath, std::string name){
  //got here
  //need to take region,
  //call internal process on the region passing the output into a table,
  //run through the table with phasor set up accordingly 
  //render out the new region using sndfile
  
  //initially use the table lookup functionality
  
  
  std::cout << "Entering Timestretch"<<std::endl;
  ///////////////////////////////////////////////////////
  ////////Create new session with the region/////////////
  SamplePosition storedPosition = region->get_start_pos();
  Session temp;
  temp.set_playback_duration(region->get_duration() + region->get_extension());
  region->set_start_pos(0);
  temp.add_region(region);
  std::string filepath = folderpath + name+"bounce.wav";
  temp.bounce_session(filepath);
  region->set_start_pos(storedPosition);
  std::cout << "Region Bounced"<<std::endl;
  ///////////////////////////////////////////////////////
  ///////
  MultiTableBuffer t_tables=load_file_to_table(filepath);
  std::cout << "File loaded into table"<<std::endl;
  std::stringstream ssPath;
  ssPath<<  folderpath<< name<<stretchAmount*100<< "%TStretch.wav";
  SNDFILE* outfile;
  SF_INFO m_info;
  m_info.channels = 2;
  m_info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16 | SF_ENDIAN_LITTLE;
  m_info.samplerate = 44100;
  outfile = sf_open(ssPath.str().c_str(),SFM_WRITE,&m_info);
  
  std::cout << "Table size =  "<< t_tables.at(0)->get_size()+1  ;
  
  
  //FIXME mixing logic here!!!!!!
  
  /*
  Phasor t_phasor(44100,stretchAmount/2.0f);
  float* tempBuf = new float[2];
  for(int n = 0; n < float(region->get_duration()/2)*stretchAmount ;++n){
	      tempBuf[0]= t_tables.at(0)->linear_lookup(t_phasor.get_phase()*(t_tables.at(0)->get_size()+1));
	      tempBuf[1]= t_tables.at(1)->linear_lookup(t_phasor.get_phase()*(t_tables.at(1)->get_size()+1));
	      sf_writef_float(outfile,tempBuf,2);//write to disk
	      t_phasor.tick();
  }
  delete []tempBuf;
  */
  
 
  const int HEADCOUNT = 16;
  const int WINDOWSIZE = 1024;
  const float FREQ = 44100.0f/float(WINDOWSIZE);
  Phasor tapeHead(44100,FREQ);
  Table<double> t_han(WINDOWSIZE);
  t_han.fill_hann();
  
  float pos = 0.0;
  
  float rate = 1.0f/stretchAmount;
  
  float headPos[HEADCOUNT];
  for(int n = 0; n < HEADCOUNT; ++n){
    headPos[n] = 0;
  }
  
  int size = region->get_duration()/rate;
  for(int samp = 0; samp < size;++samp){
  
	float s[2];
	s[0] = s[1] = 0.0f;
	
	
	
	for(int n = 0; n < HEADCOUNT; ++n){
	    float phase = phase_wrap(  tapeHead.get_phase() + float(n)/float(HEADCOUNT)    );      
	    float gain = t_han.linear_lookup(phase*float(WINDOWSIZE))      ;
	    
	    
	    if( phase < 0.001 ){
	      headPos[n] = pos;
	    }
	      
	    s[0] += ( t_tables.at(0)->linear_lookup( phase*float(WINDOWSIZE) + headPos[n]   ) * gain) * (1.0f/float(HEADCOUNT));
	    s[1] += ( t_tables.at(1)->linear_lookup( phase*float(WINDOWSIZE) + headPos[n]   ) * gain )* (1.0f/float(HEADCOUNT));
	}
	
	tapeHead.tick();
	
	sf_writef_float(outfile,s,1);//write to disk
	
	pos += rate;
  }
  
  
  
  
  
  return ssPath.str();
  //////////////////////////////////////////////////////
  ///////
  
  
}


void Session::set_left_locator(SamplePosition location){
  m_leftLocator = location;
  std::cout << "Left locator set to "<< m_leftLocator<<std::endl;
}
SamplePosition Session::get_left_locator()const{
    return m_leftLocator;
}
void Session::set_right_locator(SamplePosition location){
    m_rightLocator = location;
    std::cout << "Right locator set to "<< m_rightLocator<<std::endl;
}
SamplePosition Session::get_right_locator()const{
    return m_rightLocator;
}

void Session::set_loop_state(bool state){
    m_loopState = state;
}
bool Session::get_loop_state(){
    return m_loopState;
}



