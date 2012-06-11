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

#include "fsom/Engine.hpp"
#include "fsom/Session.hpp"
#include "fsom/RegionManager.hpp"
#include "fsom/SynthesisModuleManager.hpp"
/*
//FOR FIXING
#include <iostream>
*/
using namespace fsom;

Engine::Engine() :
	m_activeSession(0),m_outputBuffers(get_output_channels(),get_block_size())
{
	register_fx_factories();
	register_module_factories();
	register_region_factories();
	init();
}

Engine::~Engine(){
	close();

}

Engine* Engine::s_instance = 0;

Engine& Engine::get_instance(){
	if(!s_instance) s_instance = new Engine();
	return *s_instance;
}

void Engine::destroy(){
	if(s_instance) delete s_instance;
	s_instance = 0;
}

void Engine::attach_session(Session* s){
	assert(s);
	m_activeSession = s;
}

int Engine::process(const void *input, void *output, unsigned long frameCount){
	assert(frameCount == get_block_size());
	// session
	if(m_activeSession){
		m_activeSession->process(0, m_outputBuffers.get_buffers(), frameCount, get_output_channels());
	}

	float* out = static_cast<float*>(output);
	interleave ((const float**)m_outputBuffers.get_buffers(),out,get_output_channels(),frameCount);

	return 0;
}

Session& Engine::get_active_session(){
	assert(m_activeSession);
	return *m_activeSession;
}


void Engine::detach_active_session(){
    stop();
    clear_multichannel_buffers(m_outputBuffers.get_buffers(),get_output_channels(),get_block_size());
	delete m_activeSession;
    m_activeSession=NULL;
    start();
}


