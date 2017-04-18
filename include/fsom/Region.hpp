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


#ifndef __REGION_HPP__
#define __REGION_HPP__

#include "Utilities.hpp"
#include "MetaDataStore.hpp"
#include "DSPManager.hpp"
#include <string>
#include <iostream>
// pre-declaration
class TiXmlElement;

namespace fsom{

	typedef std::shared_ptr<Region> RegionPtr;
	typedef std::vector<DSPEffectPtr> DSPStack;
	
	typedef std::shared_ptr<Parameter> ParameterPtr;
	typedef std::map<std::string,ParameterPtr> ParameterList;
	
	
	
struct regionCreationStruct{

  SamplePosition m_startPos;
  int m_laneNum;
  SampleLength m_duration;
  SampleLength m_offset;
  SampleLength m_extension;
  std::string m_filepath;
  std::string m_workingDirectory;
  bool m_reverseState;
  regionCreationStruct(SamplePosition start, SampleLength duration, SampleLength offset, int laneNum,SampleLength extension=0,std::string filepath="",std::string workingDirectory="",bool reverseState=false) : 
	  m_startPos(start), m_duration(duration),m_offset(offset), m_laneNum(laneNum),m_filepath(filepath),m_workingDirectory(workingDirectory),m_extension(extension),m_reverseState(reverseState){
	fsom::DebugStream << "region creation struct complete"<<std::endl;
  }
};
	
	
class Region : public MetaDataStore, private NonCopyable{
	DSPStack m_DSPStack;
	MultiChannelBuffer m_stackBufferA;
	MultiChannelBuffer m_stackBufferB;
	regionCreationStruct m_dataStruct;
	
	ParameterList m_parameterList; //array of parameters for Region
	bool m_isInExtensionMode;
	SamplePosition m_internalPos;
	bool m_muted;
public:
	Region(regionCreationStruct creationStruct);
	//copy constructor
	//Region(const Region& _region);
	//virtual ~Region();
	/// abstract function that specific implementors must overide
	virtual void process(float** input, float** output, int frameSize, int channels) = 0;
	
	///The actual function that the session process function calls
	void process_region(float** input, float** output, int frameSize, int channels,SamplePosition globalPosition);
	
	/// abstract save function
	virtual void save_to_xml_node(TiXmlElement* node, bool useRelative)=0 ;
	/// abstract function to initialise the region for playback.
	/// this is called automatically during the playback of a session prior to hearing the file for the first time.
	virtual void on_region_start(SamplePosition seekTime) = 0;
	void save_to_region_specifics_to_existing_xml_node(TiXmlElement* node,bool useRelative);
	void save_region_parameters(TiXmlElement* node);
	void set_start_pos(SamplePosition startPos);
	void set_lane_num(int laneNum);//not being used
	void set_duration(SampleLength duration);
	void set_file_path(std::string);
	void set_working_directory(std::string directory);

	std::string get_working_directory()const;
	std::string get_file_path() const;
	SamplePosition get_start_pos() const;
	int get_lane_num() const;
	SampleLength get_duration() const;
	SampleLength get_offset_amount();
	void set_offset_amount(SampleLength offset);
	
	SampleLength get_extension() const;
	void set_extension(SampleLength extension);

	// overidable printing function to display region info to the console
	virtual void print_region_info() const;
	DSPStack& get_DSPStack();

	/// function that processes the dsp stack
	void process_dsp_stack(float** input, float** output, int frameSize, int channels);

	int attach_effect(DSPEffectPtr p);
	int add_effect(std::string id);
	void remove_effect(std::string id);
	void clear_all_effects();
	DSPEffectPtr get_effect(int id);
	void remove_effect(DSPEffectPtr effect);
	void remove_effect(int id);
	
	regionCreationStruct get_creation_struct();
	void add_parameter(std::string IdName, float lowerBound, float upperBound, float value);
	ParameterPtr get_parameter(std::string IdName);
	ParameterPtr get_parameter(int id);
	ParameterList& get_parameter_list();
	int get_num_parameter() const;
	
	void set_extension_mode(bool state);
	
	void set_reverse_state(bool state);
	
	bool get_reverse_state();
	
	const SamplePosition& get_sample_position();
	
	void reset_all_effects();
	
	//functions for muting
	void set_mute_state(bool state);
	bool get_mute_state();
	
	bool has_parameter(std::string name);

	
	
};
} // fsom
#endif //__REGION__
