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


#ifndef __INTERPCONTROL_HPP__
#define __INTERPCONTROL_HPP__

#include <vector>
#include "Breakpoint.hpp"

namespace fsom{

class InterpControl{

std::vector<Breakpoint> m_points;
double m_currentTime;
float m_currentValue;

public:

InterpControl();
~InterpControl();
void set_value(float value, double timeCode);
void sort_points();
float get_value();
void tick();
bool predicate(const Breakpoint& a, const Breakpoint& b);
};


}

#endif
