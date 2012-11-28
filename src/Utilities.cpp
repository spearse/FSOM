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

#ifdef _WIN32
	#define NOMINMAX
	#include <windows.h>
	
#else
	#include <unistd.h>
#endif


namespace fsom{

void generic_sleep(int t){
#ifdef _WIN32
	Sleep(t*1000);
#else
	sleep(t);
#endif
}



#if _WIN32
class WindowsMutex : public Mutex{
  CRITICAL_SECTION cs;
  public:
	WindowsMutex(){
		InitializeCriticalSection( &cs );
	};
	virtual ~WindowsMutex(){
		DeleteCriticalSection(&cs);
	};
	virtual void lock(){
	   EnterCriticalSection( &cs );
	}
	virtual bool try_lock(){
	  return true;
	}
	virtual void unlock(){
	  LeaveCriticalSection( &cs );
	}
};

#else
#include <pthread.h>
class PosixMutex : public Mutex{
  pthread_mutex_t m_mtx;
  public:
	PosixMutex(){
	  pthread_mutex_init (&m_mtx, NULL);
	}
	virtual ~PosixMutex(){
	  pthread_mutex_destroy(&m_mtx);
	}
	virtual void lock(){
	  pthread_mutex_lock(&m_mtx);
	}
	virtual bool try_lock(){
	  return pthread_mutex_trylock(&m_mtx) == 0;
	}
	virtual void unlock(){
	  pthread_mutex_unlock(&m_mtx);
	}
};
#endif

Mutex* Mutex::create(){
  #if _WIN32
	return new WindowsMutex();
  #else
	return new PosixMutex();
  #endif
};

void Mutex::destroy(Mutex* mutex){
    delete mutex;
};

}
