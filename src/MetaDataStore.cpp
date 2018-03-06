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



#include "fsom/MetaDataStore.hpp"

#include <iostream>
#include <cassert>
#include <sstream>

#include "tinyxml/tinyxml.h"
#include "fsom/Utilities.hpp"

namespace fsom{
	
	class MetaException : public std::exception{
		
  virtual const char* what() const throw()
  {
	  return "MetaData exception";
  }
	} ;
	
	
	MetaDataStore::MetaDataStore()
	{
		
	}
	
	MetaDataStore::~MetaDataStore()
	{
		
	}
	
	void MetaDataStore::register_meta(const std::string& key){
		m_map[key];
	}
	void MetaDataStore::set_meta(const std::string& key,const std::string& value){
		if (m_map.find(key) == m_map.end()){
			fsom::DebugStream << "MetaData key " << key << " not registered" <<std::endl;
		} else{
			m_map[key] = value;
		}
	}
	
	void MetaDataStore::set_meta_as_int(const std::string& key, const int& value){
		
		std::stringstream intString;
		intString << value;
		
		set_meta(key,intString.str());
	}
	
	void MetaDataStore::set_meta_as_float(const std::string& key, const float& value){
		std::stringstream floatString;
		floatString << value;
		
		set_meta(key,floatString.str());
	}
	
	const std::string& MetaDataStore::get_meta(const std::string& key) const{
		FSOM_ASSERT(!key.empty());
		
		std::map<std::string,std::string >::const_iterator it;
		it = m_map.find(key);
		if(it != m_map.end()){
			return it->second;
		}
		throw MetaDataStoreKeyException(key);
	}
	
	int MetaDataStore::get_meta_as_int(const std::string& key) const{
		
		return atoi(get_meta(key).c_str());
	}
	
	float MetaDataStore::get_meta_as_float(const std::string& key) const{
		return strtof(get_meta(key).c_str(), nullptr);
	}
	
	void MetaDataStore::save_meta_to_xml(TiXmlElement* node){
		TiXmlElement * element = new TiXmlElement( "MetaData" );
		std::map<std::string,std::string>::iterator it;
		for (it = m_map.begin();it != m_map.end();++it){
			element->SetAttribute((*it).first.c_str(),(*it).second.c_str());
//			std::cout<<  "meta saving "<< (*it).first.c_str() << " attribute to......." << (*it).second.c_str() << std::endl;
		}
		
		node->LinkEndChild( element );
	}
	
	
	std::map<std::string,std::string> MetaDataStore::get_map(){
  return m_map;
	}
	
	void MetaDataStore::print_items(){
		std::map<std::string,std::string>::iterator it;
		for (it = m_map.begin();it != m_map.end();++it){
			std::cout<<  "meta "<< (*it).first.c_str() << " - " << (*it).second.c_str() << std::endl;
		}

	}
	
	
	
}
