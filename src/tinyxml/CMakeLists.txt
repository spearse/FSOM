#Libfsom->Src->TinyXML Folder

IF(APPLE)
#	set(CMAKE_CXX_FLAGS "-g -Wall -arch i386")
#	set(CMAKE_CXX_FLAGS:STRING "-arch i386")
	set(CMAKE_CXX_FLAGS "-g -Wall -arch x86_64")
	set(CMAKE_CXX_FLAGS:STRING "-arch x86_64")

ENDIF(APPLE)
IF(WIN32)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /vmg /Gd")
ENDIF(WIN32)
add_library(tinyxml tinystr.cpp  tinyxml.cpp  tinyxmlerror.cpp  tinyxmlparser.cpp)

