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


// pre-declaration
class TiXmlElement;

namespace fsom{

class MetaDataStoreKeyException{
	std::string m_keyString;
public:
	MetaDataStoreKeyException(const std::string& keyString) : m_keyString(keyString){}
};

class MetaDataStore
{
std::map<std::string,std::string> m_map;
public:
    MetaDataStore();
    virtual ~MetaDataStore();
	void register_meta(const std::string& key);
	
	void set_meta(const std::string& key,const std::string& value);
	void set_meta_as_int(const std::string& key,const int& value);
	void set_meta_as_float(const std::string& key,const float& value);
	
	const std::string& get_meta(const std::string& key) const;
	int get_meta_as_int(const std::string& key) const;
	float get_meta_as_float(const std::string& key) const;
	
	void save_meta_to_xml(TiXmlElement* node);
};


}
#endif // METADATASTORE_HPP


