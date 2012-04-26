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



#include "fsom/AudioFilePool.hpp"
#include <iostream>
namespace fsom{

AudioFilePool* AudioFilePool::s_instance = 0;  
  
  
AudioFilePool& AudioFilePool::get_instance(){
	if (!s_instance)s_instance = new AudioFilePool();
	return *s_instance;
}


void AudioFilePool::destroy(){
  if(s_instance) delete s_instance;
	s_instance = 0;
}

AudioFilePool::AudioFilePool(){
    std::cout << "Audio file pool spawned "<<std::endl;
}

AudioFilePtr AudioFilePool::get_audio_file_ptr(std::string filePath){
     std::cout << "Requested "<<filePath << " audio file from audio pool"<<std::endl;  
  
    AudioFileMap::iterator it = m_map.find(filePath);
    
    if(it !=m_map.end())return (*it).second;
    std::cout << "Audio file requested not found in pool"<<std::endl;
    std::cout << "Audio file pool is now spawning a new audio file for "<< filePath<<std::endl;
    //TODO check that the audiofile is created successfully!!!!!!
    m_map[filePath] = AudioFilePtr(new AudioFile(filePath));
    
    return m_map[filePath];  
    
  
}

}