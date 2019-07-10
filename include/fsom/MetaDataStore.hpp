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


#ifndef METADATASTORE_HPP
#define METADATASTORE_HPP

#include <map>
#include <string>
#include <vector>

// pre-declaration
class TiXmlElement;

namespace fsom{
	
	class MetaDataStoreKeyException{
		std::string m_keyString;
	public:
		MetaDataStoreKeyException(const std::string& keyString) : m_keyString(keyString){}
		std::string get(){return  m_keyString;};
	};
	
	struct Tag{
		std::string m_tag;
		std::string m_dateAdded;
		Tag(std::string tag,std::string date):
		m_tag(tag),
		m_dateAdded(date)
		{
			
		}
		Tag(const Tag& other):
		m_tag(other.m_tag),
		m_dateAdded(other.m_dateAdded)
		{
			
		}
		bool operator == (const std::string& other){
			return m_tag == other;
		}
		bool contains(const std::string& other){
			return m_tag.find(other) != std::string::npos;
		}
	};
	typedef std::vector<Tag> TagVector;

	
	
	class MetaDataStore
	{
		std::map<std::string,std::string> m_map;
		TagVector m_tags;
		
	public:
		MetaDataStore();
		virtual ~MetaDataStore();
		MetaDataStore(const MetaDataStore& other);
		
		
		void register_meta(const std::string& key);
		
		void set_meta(const std::string& key,const std::string& value);
		void set_meta_as_int(const std::string& key,const int& value);
		void set_meta_as_float(const std::string& key,const float& value);
		
		const std::string& get_meta(const std::string& key) const;
		int get_meta_as_int(const std::string& key) const;
		float get_meta_as_float(const std::string& key) const;
		
		void save_meta_to_xml(TiXmlElement* node);
		std::map<std::string,std::string> get_map();
		void print_items();
		
		void copyMetaData(const MetaDataStore& other);
		void copyTags(const MetaDataStore& other);
		void addTag(std::string tag,std::string date);
		const TagVector* getTags();
		bool containsTag(std::string tagName);
		std::string getFormattedTags();
		void loadTagsFromXML(TiXmlElement* element);
		void removeTag(std::string tag );
		
	};
	
	
}
#endif // METADATASTORE_HPP


