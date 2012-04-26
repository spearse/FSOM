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


#ifndef ADDITIVESYNTHESISREGION_HPP
#define ADDITIVESYNTHESISREGION_HPP

#include "SynthesisRegion.hpp"
#include "Table.hpp"
#include "Generator.hpp"


namespace fsom{
  
  class AdditiveSynthesisRegion : public SynthesisRegion
  {
	  public:
      AdditiveSynthesisRegion(regionCreationStruct data);
      virtual ~AdditiveSynthesisRegion();
      
//       virtual void process(float** input, float** output, int frameSize, int channels);
//       virtual void save_to_xml_node(TiXmlElement* node);
      virtual void on_region_start(SamplePosition seekTime);
      
      static RegionPtr create(regionCreationStruct data) { return RegionPtr(new AdditiveSynthesisRegion(data)) ; }
//       void process_generator_stack(float** input, float** output, int frameSize, int channels);
  };


  
}



#endif // ADDITIVESYNTHESISREGION_HPP
