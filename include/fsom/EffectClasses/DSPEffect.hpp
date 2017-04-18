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


#ifndef DSPEFFECT_HPP
#define DSPEFFECT_HPP

#include "../Parameter.hpp"
#include "../MetaDataStore.hpp"

#include <memory>
#include <map>
#include "../Utilities.hpp"

namespace fsom{

class Region;
  
struct dspCreationStruct{
	  int dummy;
	  Region* attatchedRegion;
	  dspCreationStruct(fsom::Region* region):attatchedRegion(region){}
	  dspCreationStruct(){}
};
 
 
class DSPEffect : public MetaDataStore
{
	std::string m_IdName;
	const std::string m_tutId;
	ParameterList m_parameterList;
	dspCreationStruct m_dataStruct;
	bool m_isImplemented;
	bool m_bypass;

protected:
	DSPEffect(const DSPEffect& op);
	DSPEffect& operator = (const DSPEffect& op) = delete;

public:	
	DSPEffect(dspCreationStruct creationStruct);

	virtual ~DSPEffect();
	virtual void process(float** input, float** output, int frameSize, int channels) = 0;
	ParameterPtr get_parameter(int id);
	ParameterPtr get_parameter(const std::string& IdName);
	int get_num_parameter() const;
	void add_parameter(const std::string& IdName, float lowerBound, float upperBound, float value); // Add a parameter object
	std::string& get_effect_name();
	void set_effect_name(const std::string& IdName);
	ParameterList& get_parameter_list();
	const std::string& get_tutId();
	bool is_implemented();
	void set_implementation();
	/// returns a copy of the creation arguments
	dspCreationStruct get_creation_struct(){ return m_dataStruct; }
	/// all effects must provide a deep copy using clone
	virtual std::shared_ptr<DSPEffect> clone() = 0;
	bool bypass_active();
	virtual void reset_effect(){};
	void set_bypass(bool state);
	
	bool has_parameter(std::string name);

};

typedef std::shared_ptr<DSPEffect> DSPEffectPtr;

}
#endif // DSPEFFECT_HPP
