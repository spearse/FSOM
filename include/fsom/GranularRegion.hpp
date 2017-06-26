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


#ifndef GRANULARREGION_HPP
#define GRANULARREGION_HPP

#include "Region.hpp"
#include "AudioFile.hpp"
#include "Table.hpp"
#include "GrainStream.hpp"


namespace fsom{
  
  class GranularRegion : public Region
  {	
//     AudioFile m_file;
//     Table<float> m_table;
//     MultiTableBuffer m_tables;
    MultiChannelBuffer m_diskStreamBuffers;
    ParameterList m_parameters;
    //for testing only
//     GrainStream m_grainStream;  
    int m_counter;
    bool m_fileLoaded;

//    typedef std::vector<GrainPtr> GrainList;
//    GrainList m_grains;
    MultiTablePtr m_table;
 
	  
	float m_sampleSize;
	float m_sampleRate;
	float m_NormalfilePlaybackSpeed;
	float m_spawnRate;
	float m_grainSize;
	float m_pitch;
	int m_samplesPerSpawn;
	int m_samplesToNextSpawn;
	Phasor m_phasor;
	float m_scrollSpeed;
	  float m_currentSampleStart;
	  
	typedef std::vector<Grain> GrainVector;
	GrainVector m_grainvector;
	const int m_numGrains = 4;

   

   
	  
	void kill_grains();
    void spawn();
    int m_nextSpawn;
    std::string m_filepath;

    void kill_all_grains();
  
  
  public:
      GranularRegion(regionCreationStruct data);
      virtual ~GranularRegion();
      void show_info();

      virtual void process(float** input, float** output, int frameSize, int channels);
      void grain_process(float** output, int channels,int frames);
      void reset();  

      virtual void save_to_xml_node(TiXmlElement* node,bool useRelatvie);
      void on_region_start(SamplePosition seekTime);
      SampleLength get_file_length();
      
      void load_soundfile(std::string filepath);
      std::string get_soundfile();

      bool get_load_state();
      void fill_table_sin();
//       AudioFile& get_audiofile();
      static RegionPtr create(regionCreationStruct data) { return RegionPtr(new GranularRegion(data)) ; }
      
  };
  typedef std::shared_ptr<GranularRegion> GranularSynthesisRegionPtr;

}
#endif // GRANULARREGION_HPP
