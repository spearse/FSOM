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
	MetaDataStore::MetaDataStore(const MetaDataStore& other)
	{
		copyMetaData(other);
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
		for(auto tag: m_tags){
			TiXmlElement * tagElement = new TiXmlElement( "Tag" );
			tagElement->SetAttribute("id",tag.m_tag.c_str()  );
			tagElement->SetAttribute("dateadded",tag.m_dateAdded.c_str()  );

			element->LinkEndChild(tagElement);
		}
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
	void MetaDataStore::copyMetaData(const MetaDataStore& other){
		m_map = other.m_map;
		for(auto tag : other.m_tags){
			if(!containsTag(tag.m_tag)){
				m_tags.push_back(tag);
			}
		}
	}
	void MetaDataStore::copyTags(const MetaDataStore& other){
		for(auto tag : other.m_tags){
			if(!containsTag(tag.m_tag)){
				m_tags.push_back(tag);
			}
		}
	}
	void MetaDataStore::addTag(std::string tag,std::string date){
		if(!containsTag(tag)){
				m_tags.push_back(Tag(tag,date));
		}
	}
	const TagVector* MetaDataStore::getTags(){
		return &m_tags;
	}
	bool MetaDataStore::containsTag(std::string tagName){
		
		for(auto tag : m_tags){
			if(tag.contains(tagName))return true;
		}
		
		return false;
	}
	void MetaDataStore::loadTagsFromXML(TiXmlElement* element){
		TiXmlElement* tag = element->FirstChildElement("Tag");
		while(tag){
			std::string tagText = tag->Attribute("id");
			std::string dateAdded = tag->Attribute("dateadded");
			
			addTag(tagText, dateAdded);
			
			tag = tag->NextSiblingElement("Tag");
		}
	}
	std::string MetaDataStore::getFormattedTags(){
		std::string out;
		for(auto tag : m_tags){
			out += tag.m_tag + "  ";
		}
		return out;
	}
	void MetaDataStore::removeTag(std::string tag){
		int n = 0;
		TagVector::iterator it = std::find(m_tags.begin(), m_tags.end(), tag);
		if(it != m_tags.end()){
			m_tags.erase(it);
		}
		/*
		f8or(auto itag : m_tags){
			if(itag == tag){
				m_tags.e
			}
				++n;
		}
		 */
	}
}
