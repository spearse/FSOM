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


#ifndef __PADRIVER_HPP__
#define __PADRIVER_HPP__

#include <memory>

namespace fsom{
  
///PortAudio Driver Exception Class
class PortaudioException : public std::exception{
	const char* m_msg; ///message to be displayed in exception 
public:
	PortaudioException(const char* msg) : m_msg(msg) {};
	const char* what() const throw() { return m_msg; }
};

struct PaDriverImpl; // library specific implementations

class PaDriver{
	
	int m_sr; ///driver sample rate
	int m_blockSize; ///driver block size
	int m_outputChannels; ///driver channels
	
	std::auto_ptr<PaDriverImpl> m_pImpl; /// pImpl idiom
	PaDriver(const PaDriver&); // disable copy
public:
	PaDriver(); ///PaDriver constructor
	~PaDriver();///PaDriver destructor
	void init();
	void start();
	void stop();
	void close();

	int get_sample_rate() const; ///returns sample rate
	int get_output_channels() const; ///returns channel amount
	int get_block_size() const; ///returns block size

	virtual int process(const void *input, void *output, unsigned long frameCount) = 0;
};

}

#endif
