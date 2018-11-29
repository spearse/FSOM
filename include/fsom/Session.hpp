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


#ifndef __SESSION_HPP__
#define __SESSION_HPP__


#include <list>
#include "Region.hpp"
#include "MetaDataStore.hpp"
#include "Generator.hpp"
#include "SynthesisModule.hpp"
#include "Table.hpp"

namespace fsom{
  
typedef std::shared_ptr<Region> RegionPtr;
typedef std::list<RegionPtr> RegionList;
typedef std::list<Region*> ActiveRegionList;

enum SessionEventType{
	SE_REGION_START = 0,
	SE_REGION_ACTIVE,
	SE_REGION_EXTENSION,
	SE_REGION_END,
	SE_SESSION_LOOP
	// other events such as midi, and envelop breakpoints will extend this
};
/// A session event is used by the session events cache to manage the dsp loop during playback
struct SessionEvent{
	SessionEventType type;
	RegionPtr region; // the region this event is related to
	SamplePosition time; // the time this event occurs
	float value; // FUTURE, may used by breakpoint event types.
	void* user; // FUTURE
};

typedef std::list<SessionEvent> SessionEventList;

class Session : public MetaDataStore{
public:
	enum FileType{
	    FT_WAV=0,
	    FT_AIFF,
	    FT_OGG,
	    FT_FLAC
	};
private:
  
	Mutex* m_audioMutex;
  ///Currently active session name
//	const char* m_sessionName;

	/// boolean for state of play
	bool m_transportIsRolling;
	
	bool m_loopPreviewState;
	
	bool m_loopPreviewExtendState;
	
	bool m_loopState;
	
	bool m_hardLimit;

	/// playhead location
	SamplePosition m_playHead;
	
	SamplePosition m_previewPlayHead;
	
	SampleLength m_playbackDuration;
	
	SamplePosition m_startPos;

	///Locators for looping
	SamplePosition m_leftLocator;
	
	SamplePosition m_rightLocator;
	///Storage for average amplitudes for multichannel data
	ChannelAmplitudes m_channelAmps;
	///Storage for peak amplitude data
	PeakData* m_peakData;
	
	/// vector of all regions in the composition. This is maintained in a order of start time and duration.
	RegionList m_regionPlaylist;

	/// a list of all the regions currently playing.
	/// uses raw pointers for speed and used in the dsp loop
	/// this may evolve into a fixed size array to avoid allocation and deallocation hence the special type
	ActiveRegionList m_activeRegions;

	/// a cache of all pending events for playback, this is populated during the seek operation.
	SessionEventList m_eventCache;
	/// the iterator pointing to the next event to be processed
	SessionEventList::iterator m_nextEvent;
	
	///boolean representing state of preview
	bool m_previewState;
	///pointer to region to preview
	RegionPtr m_previewRegion;
	
	///parameter for master volume control
	ParameterPtr m_masterVolume;
	
	/// internal function to activate a region
	void add_active_region(Region* p);

	/// internal function to deactivate a region
	void remove_active_region(Region* p);
	
	///internal function to clear all active regions
	void clear_all_active_regions();

	/// internal audio proccessing function.
	/// advance the processing within the block by n samples
	void internal_process(float** ins,float** outs,int frameCount,int channelCount, size_t offset,SamplePosition globalTime=0);
	//working directory, used in this case for loading
	std::string m_workingDirectory;
	
	RegionPtr create_region_from_node(TiXmlElement* element);
	ParameterPtr create_parameter_from_node(TiXmlElement* element,Region* region);
	DSPEffectPtr create_effect_from_node(TiXmlElement* element,Region* region);
	GeneratorPtr create_generator_from_node(TiXmlElement* element, Region* region);
	SynthesisModulePtr create_module_from_node(TiXmlElement* element, Region* region);
	void load_region_parameters(TiXmlElement* element,Region* region);
public:
	Session();
	~Session();
private:
	Session(const Session&);
	Session& operator = (const Session&);
	
	void limit_block(float** block,int blocksize,int channels);
	
public:
// 	Session(std::string preset_path);
	void load_session(const char* fileLocation);
	void save_session(const char* fileLocation, bool useRelative);

	/// seek the transport to the specified sample position and prepare for playback.
	void seek(SamplePosition seekTarget);
	/// start playback from the current transport position
	void play();
	/// stop playback leaving the transport position in the current location
	void stop();
	/// dsp process function
	void process(float** ins,float** outs,int frameCount,int channelCount);

	/// make sure that the region playlist is sorted.
	/// sort is by ascending start time and ascending duration where start times are the same.
	void sort_regions();

	/// add a region object into the region list.
	/// adding the region causes the region playlist to be sorted.
	void add_region(RegionPtr region);

	/// remove a region object from the region list.
	/// removing the region causes the region playlist to be sorted.
	void remove_region(RegionPtr region);

	/// a function to print out the whole playlist to the console.
	void print_region_playlist();

	///Return size of event list
	int get_list_size(); 

	///Return reference to the list; 
	RegionList& get_region_list();

	///Function to bounce session down to (initially) stereo wav.  Boolean allows you to toggle whether you wish to bounce between locators.
	void bounce_session(std::string filepath,FileType type = FT_WAV,bool useLocators=false);
	
	void normalise_file(std::string filepath,float targetDb);
	
	void set_preview_region(fsom::RegionPtr region);
	
	void set_preview_state(bool state);
	
	void set_playback_duration(SampleLength duration);
	
	bool get_preview_state();
	
	bool get_preview_loop_state();
	
	void set_preview_loop_state(bool state);
	
	void set_preview_loop_extend_state(bool state);
	
	bool get_preview_loop_extend_state();
	
	SamplePosition get_playhead_value();
	
	SamplePosition& get_previed_playhead_value();
	
	void bounce_region(fsom::RegionPtr region, std::string filename,FileType type = FT_WAV);
	
	void bounce_region_pre(fsom::RegionPtr region, std::string filename,FileType type = FT_WAV);
	//for bouncing a selection of regions..
	void bounce_regions(fsom::RegionList& regionList,std::string filename,FileType type=FT_WAV);
	
	const SampleLength& get_playback_duration() const; ///returns the length of playback time in session
	
	RegionPtr copy_region(const fsom::RegionPtr& region, SamplePosition position , std::string factoryName,int laneId=NULL); ///Function to create a new region from an existing region and add it to the session;
	
	RegionPtr splice_region(fsom::RegionPtr region, fsom::SamplePosition offsetPosition, std::string factoryName ); ///Function to splice a region at a point, truncating an existing and handing back a new version;
	
	RegionPtr& get_region_from_playlist();
	
	bool transport_is_rolling();
	
	void set_working_directory(std::string directory);
	
	std::string get_working_directory()const;
	
	std::string timestretch_region(RegionPtr region, float speed,std::string folderpath,std::string name,int fftSize = 512,int numOverlaps = 4);
	
	///function to set left locator
	void set_left_locator(SamplePosition location);
	///function to get left locator
	SamplePosition get_left_locator()const;
	///function to set right locator
	void set_right_locator(SamplePosition location);
	///function to get right locator
	SamplePosition get_right_locator()const;
	///function to set the session loop state
	void set_loop_state(bool state);
	///function to return session loop state
	bool get_loop_state();
	
	void load_metadata(fsom::Session* session, TiXmlElement* element );
	void save_metadata();
	///return master level
	ParameterPtr get_master_level();
	///set master level, ideally between 0 - 1
	void set_master_level(float level);
	///Function to reset all effects on seek
	void reset_all_effects();
	
	float get_amp_envelope(int chan);
	
	MultiTableBuffer load_file_to_table(std::string path);

	PeakData* get_peak_data();
	
	void set_hard_limit_state(bool state);
};



}//fsom

#endif //__SESSION__
