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


#ifndef AUDIOFILEPOOL_HPP
#define AUDIOFILEPOOL_HPP

#include "AudioFile.hpp"
#include <map>
#include <string>
namespace fsom{ 

typedef std::map<std::string,AudioFilePtr> AudioFileMap;  
  
  
class AudioFilePool
{
  AudioFileMap m_map;
  AudioFilePool();
  AudioFilePool(const AudioFilePool&);
  AudioFilePool& operator = (const AudioFilePool&);
  static AudioFilePool* s_instance;

  public:
  static AudioFilePool& get_instance();
  void destroy();
  ///function to return audio file ptr, either hands you back an existing or creates a new one
  AudioFilePtr get_audio_file_ptr(std::string filePath);
  
};


}
#endif // AUDIOFILEPOOL_HPP
