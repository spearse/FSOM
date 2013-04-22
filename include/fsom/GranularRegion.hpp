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
    Phasor m_phasor;
//     Table<double> m_table;
//     MultiTableBuffer m_tables;
    MultiChannelBuffer m_diskStreamBuffers;
    ParameterList m_parameters;
    //for testing only
    Table<double> m_sinTable;
    GrainStream m_grainStream;  
    int m_counter;
    bool m_fileLoaded;
    public:
      GranularRegion(regionCreationStruct data);
      virtual ~GranularRegion();
      void show_info();

      virtual void process(float** input, float** output, int frameSize, int channels);
      virtual void save_to_xml_node(TiXmlElement* node);
      void on_region_start(SamplePosition seekTime);
      SampleLength get_file_length();
      
      void load_soundfile(std::string filepath);
      bool get_load_state();
      
//       AudioFile& get_audiofile();
      static RegionPtr create(regionCreationStruct data) { return RegionPtr(new GranularRegion(data)) ; }
    
  };
  typedef boost::shared_ptr<GranularRegion> GranularSynthesisRegionPtr;

}
#endif // GRANULARREGION_HPP
