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


#include "../include/fsom/DummyRegion.hpp"

#include "tinyxml/tinyxml.h"
namespace fsom{
	
	DummyRegion::DummyRegion(regionCreationStruct creationStruct):
		Region(creationStruct)
	{
		
	}
	
	DummyRegion::~DummyRegion(){
		
	}
	void DummyRegion::process(float **input, float **output, int frameSize, int channels){
		return;
	}
	void DummyRegion::save_to_xml_node(TiXmlElement* node, bool useRelative){
		TiXmlElement * element = new TiXmlElement( "Region" );
		save_to_region_specifics_to_existing_xml_node(element,useRelative);
		node->LinkEndChild( element );
		return;
	}
	void DummyRegion::on_region_start(SamplePosition seekTime){
		return;
	}

}