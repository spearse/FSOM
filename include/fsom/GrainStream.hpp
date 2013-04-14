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

#ifndef GRAINSTREAM_HPP
#define GRAINSTREAM_HPP



#include "Table.hpp"
#include <vector>

namespace fsom{

class Grain {
	bool m_isDead;
	MultiTablePtr m_table; 
	TablePtr m_window;
	int m_basePosition,m_grainSize,m_internalClock,m_dur;
	float m_basePitch;
	Phasor m_phasor;
public:
	/// Grain constructor
	Grain(TablePtr window,   MultiTablePtr table,int dur,int position, float pitch );
	/// Grain destructor
	virtual ~Grain();
	
	void process(float** outs, int start, int length) ;
	/// Return true is the grain is dead and process function no longer needs to be called
	bool is_dead()const;
	void kill();
};

typedef boost::shared_ptr<Grain> GrainPtr;


class GrainStream
{
  typedef std::vector<GrainPtr> GrainList;
  GrainList m_grains;
  MultiTablePtr m_table;
  TablePtr m_window;
  int m_density,m_basePosition,m_grainSize,m_internalClock,m_grainRate;
  float m_basePitch;
  void kill_grains();
  void spawn();
  int m_nextSpawn;
public:
GrainStream();
virtual ~GrainStream();
void load_soundfile(std::string filepath);
void process(float** output, int channels,int frames);
void reset();  
void set_basePitch(float in);
void set_density(float in);
void set_basePosition(float in);
void set_grainSize(float in);
void set_nextSpawn(int nextSpawn);
void set_grainRate(float rate);



};



}
#endif // GRAINSTREAM_HPP
