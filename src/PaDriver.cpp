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

#include "fsom/Utilities.hpp"
#include "fsom/PaDriver.hpp"

#include <cassert>
#include <portaudio.h> // this is firewalled from other areas of the library

using namespace fsom;

namespace fsom{

///struct contains PaStream 
struct PaDriverImpl{
	PaStream *m_stream;
};
}

int pa_callback( const void *input,
                  void *output,
                  unsigned long frameCount,
                  const PaStreamCallbackTimeInfo* timeInfo,
                  PaStreamCallbackFlags statusFlags,
                  void *userData ){
	PaDriver* driver = reinterpret_cast<PaDriver*>(userData);
	FSOM_ASSERT(driver);
	driver->process(input,output,frameCount);
	// NO FURTHER ALTERATIONS
	return 0;
}

PaDriver::PaDriver() :
	m_pImpl(new PaDriverImpl), 
	m_sr(44100), 
	m_blockSize(512),
	m_outputChannels(2)
	{
}

PaDriver::~PaDriver(){
}

void PaDriver::init(){
	if( Pa_Initialize() != paNoError ){
		throw PortaudioException("Pa_Initialize() failed");
	}
	int err = Pa_OpenDefaultStream(
			&m_pImpl->m_stream,0,m_outputChannels,paFloat32,m_sr,m_blockSize,pa_callback,this
			);

	if(err != paNoError ){
		throw PortaudioException("Pa_OpenDefaultStream() failed");
	}
}

void PaDriver::start(){
	if( Pa_StartStream( m_pImpl->m_stream ) != paNoError ){
		throw PortaudioException("Pa_StartStream() failed");
	}
}
void PaDriver::stop(){
	if( Pa_StopStream( m_pImpl->m_stream ) != paNoError ){
		throw PortaudioException("Pa_StopStream() failed");
	}
}
void PaDriver::close(){
	if( Pa_CloseStream( m_pImpl->m_stream ) != paNoError ){
		throw PortaudioException("Pa_CloseStream() failed");
	}
	if( Pa_Terminate() != paNoError ){
		throw PortaudioException("Pa_Terminate() failed");
	}
}

int PaDriver::get_sample_rate() const{
	return m_sr;
}

int PaDriver::get_output_channels() const{
	return m_outputChannels;
}

int PaDriver::get_block_size() const{
	return m_blockSize;
}
