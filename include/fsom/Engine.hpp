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


#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "PaDriver.hpp"
#include "Utilities.hpp"

namespace fsom{

class Session;

class Engine : public PaDriver{

private:
	Engine();
	Engine(const Engine&);
	Engine& operator = (const Engine&);
	virtual ~Engine();
	MultiChannelBuffer m_outputBuffers; 
	static Engine* s_instance; ///< pointer to instance.
	Session* m_activeSession;
public:
	static Engine& get_instance();
	static void destroy();
	void attach_session(Session* s);
	virtual int process(const void *input, void *output, unsigned long frameCount);
	/// returns the currently active session object.
	Session& get_active_session();
    void detach_active_session();
};

}

#endif
