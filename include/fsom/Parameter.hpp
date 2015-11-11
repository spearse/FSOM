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

#ifndef __PARAMETER_HPP__
#define __PARAMETER_HPP__

#include "MetaDataStore.hpp"
#include <memory>
#include "Utilities.hpp"

namespace fsom
{

class BreakPointUnit;

typedef std::shared_ptr<BreakPointUnit> BreakPointUnitPtr;

class Parameter : public MetaDataStore
{

	std::string m_IDName;
	float m_currentValue, m_lowerBound, m_upperBound, m_lastStaticValue; //last static value takes the current value when set to dynamic so that it can return to it later
	BreakPointUnitPtr m_bpUnit;
	SampleLength m_duration;
	bool m_inDynamicMode;

	Parameter& operator=(const Parameter& op);

  public:
	Parameter(fsom::SampleLength duration, std::string IDName, float lowerBound, float upperBound, float value, fsom::BreakPointUnitPtr bpUnit);
	Parameter(const Parameter& old);
	~Parameter();

	void set_value(float value);

	float get_value() const;
	const std::string& get_name() const;
	void tick(SampleLength& samplesRead);
	float get_lowerBound() const;
	float get_upperBound() const;
	float get_range() const;
	float get_last_breakpoint_position() const;

	void duplicate_last_breakpoint_at_time(float time);
	BreakPointUnitPtr get_breakpoints();
	void set_breakpoints(fsom::BreakPointUnitPtr bpUnit);
	void set_automated_mode(bool isDynamic);
	bool get_automated_mode() const;
};

typedef std::shared_ptr<Parameter> ParameterPtr;
typedef std::map<std::string, ParameterPtr> ParameterList;
}
#endif
