#Libfsom->Src Folder

cmake_minimum_required(VERSION 2.8)

add_subdirectory(tinyxml)
add_subdirectory(Freeverb)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_DIR}/src/CMakeModules/")

FIND_PACKAGE(Boost REQUIRED) # Debian/Ubuntu system this is package libboost-dev
IF(APPLE)
#	set(CMAKE_CXX_FLAGS "-g -Wall -arch i386")
#	set(CMAKE_CXX_FLAGS:STRING "-arch i386")
	set(CMAKE_CXX_FLAGS "-g -Wall -arch x86_64")
	set(CMAKE_CXX_FLAGS:STRING "-arch x86_64")
ENDIF(APPLE)

IF(WIN32)
  set(LIBSNDFILE_ROOT $ENV{LIBSNDFILE_ROOT})
  set(PORTAUDIO_ROOT $ENV{PORTAUDIO_ROOT})

  IF(LIBSNDFILE_ROOT)
    set(LIBSNDFILE_LIBRARY ${LIBSNDFILE_ROOT}/lib/libsndfile-1.lib)
    set(LIBSNDFILE_INCLUDE_DIR ${LIBSNDFILE_ROOT}/include)
  ENDIF(LIBSNDFILE_ROOT)

  IF(PORTAUDIO_ROOT)
    set(PORTAUDIO_LIBRARIES ${PORTAUDIO_ROOT}/lib/libportaudio.lib)
    set(PORTAUDIO_INCLUDE_DIRS ${PORTAUDIO_ROOT}/include)
  ENDIF(PORTAUDIO_ROOT)

ELSE(WIN32)
	FIND_PACKAGE(Portaudio REQUIRED)
	FIND_PACKAGE(LibSndFile REQUIRED)

ENDIF(WIN32)

include_directories(
	../include
	${Boost_INCLUDE_DIRS}
	${LIBSNDFILE_INCLUDE_DIR}
	${PORTAUDIO_INCLUDE_DIRS}
	
)

link_directories(
	${Boost_LIBRARY_DIRS}
)

SET(LIBS ${LIBS}
	${Boost_LIBRARIES}
	${LIBSNDFILE_LIBRARY} 
	${PORTAUDIO_LIBRARIES}

)
set(FSON_SOURCES
	AudioFile.cpp
	AudioRegion.cpp
	EffectClasses/DSPEffect.cpp
	DSPManager.cpp
	EffectClasses/Parameter.cpp
#	InterpControl.cpp
	Engine.cpp
	PaDriver.cpp
	Region.cpp
	RegionManager.cpp
	GranularRegion.cpp
	AdditiveSynthesisRegion.cpp
	Session.cpp
	NoiseRegionBase.cpp
	NoiseRegions.cpp
	MetaDataStore.cpp
	Utilities.cpp
	EffectClasses/Modulators.cpp
	EffectClasses/Gain.cpp
	EffectClasses/Biquad.cpp
	EffectClasses/Pan.cpp
	EffectClasses/Filters.cpp
	EffectClasses/Echo.cpp
	EffectClasses/Delay.cpp
	EffectClasses/Reverb.cpp
	EffectClasses/Fade.cpp
	EffectClasses/Transpose.cpp
	EffectClasses/Fragment.cpp
	EffectClasses/Distort.cpp
	EffectClasses/FeedbackDelay.cpp
	EffectClasses/Chorus.cpp
	EffectClasses/Timestretch.cpp
	EffectClasses/Flanger.cpp
	EffectClasses/Granulate.cpp
	EffectClasses/Envelope.cpp
	EffectClasses/Convolution.cpp
	EffectClasses/TransposeUnit.cpp
	EffectClasses/Harmonizer.cpp
	EffectClasses/AsymmetricDelay.cpp
	Generator.cpp
	SynthesisRegion.cpp
	SynthesisModule.cpp
	SubtractiveSynthesisRegion.cpp
	SynthesisModuleManager.cpp
	AudioFilePool.cpp
	../include/fsom/Table.hpp
)

add_library(fsom ${FSON_SOURCES})
target_link_libraries(fsom ${LIBS} tinyxml Freeverb)

