#Libfsom->Src Folder

cmake_minimum_required(VERSION 2.8)

add_subdirectory(tinyxml)
add_subdirectory(Freeverb)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_DIR}/src/CMakeModules/")

#FIND_PACKAGE(Boost REQUIRED) # Debian/Ubuntu system this is package libboost-dev

IF(APPLE)
#	set(CMAKE_CXX_FLAGS "-g -Wall -arch i386")
#	set(CMAKE_CXX_FLAGS:STRING "-arch i386")
	set(CMAKE_CXX_FLAGS "-g -Wall -arch x86_64")
	set(CMAKE_CXX_FLAGS:STRING "-arch x86_64")
ENDIF(APPLE)

#find_package(Sndfile)


IF(WIN32)
  set(SNDFILE_ROOT $ENV{SNDFILE_ROOT})
  set(PORTAUDIO_ROOT $ENV{PORTAUDIO_ROOT})

  IF(SNDFILE_ROOT)
    set(SNDFILE_LIBRARY ${SNDFILE_ROOT}/lib/libsndfile-1.lib)
    set(SNDFILE_INCLUDE_DIR ${SNDFILE_ROOT}/include)
  ENDIF(SNDFILE_ROOT)

  IF(PORTAUDIO_ROOT)
    set(PORTAUDIO_LIBRARIES ${PORTAUDIO_ROOT}/lib/libportaudio.lib)
    set(PORTAUDIO_INCLUDE_DIRS ${PORTAUDIO_ROOT}/include)
  ENDIF(PORTAUDIO_ROOT)

ELSE(WIN32)
	FIND_PACKAGE(Portaudio REQUIRED)
	FIND_PACKAGE(SndFile REQUIRED)

ENDIF(WIN32)

include_directories(
	../include
	../FSOM/include/
	../FSOM/src/
	${Boost_INCLUDE_DIRS}
	${SNDFILE_INCLUDE_DIR}
	${PORTAUDIO_INCLUDE_DIRS}
	${SNDFILE_INCLUDE_DIR} 
)

link_directories(
	${Boost_LIBRARY_DIRS}
)

SET(LIBS ${LIBS}
	${Boost_LIBRARIES}
	${SNDFILE_LIBRARY} 
	${PORTAUDIO_LIBRARIES}
	${SNDFILE_LIBRARIES}

)

set(FSOM_SOURCES
	AudioFile.cpp
	AudioRegion.cpp
	DSPManager.cpp
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
	Generator.cpp
	SynthesisRegion.cpp
	SynthesisModule.cpp
	SubtractiveSynthesisRegion.cpp
	SynthesisModuleManager.cpp
	AudioFilePool.cpp
	GrainStream.cpp
	GranularRegion.cpp
	EffectClasses/FrequencyModulation.cpp
)

set(FSOM_HEADERS
../include/fsom/AbstractFactory.hpp
../include/fsom/AdditiveSynthesisRegion.hpp
../include/fsom/AudioFile.hpp
../include/fsom/AudioFilePool.hpp
../include/fsom/AudioRegion.hpp
../include/fsom/Breakpoint.hpp
../include/fsom/DSPManager.hpp
../include/fsom/Engine.hpp
../include/fsom/Generator.hpp
../include/fsom/GrainStream.hpp
../include/fsom/GranularRegion.hpp
../include/fsom/InterpControl.hpp
../include/fsom/MetaDataStore.hpp
../include/fsom/NoiseRegionBase.hpp
../include/fsom/NoiseRegions.hpp
../include/fsom/PaDriver.hpp
../include/fsom/Parameter.hpp
../include/fsom/Region.hpp
../include/fsom/RegionManager.hpp
../include/fsom/Session.hpp
../include/fsom/SubtractiveSynthesisRegion.hpp
../include/fsom/SynthesisModule.hpp
../include/fsom/SynthesisModuleManager.hpp
../include/fsom/SynthesisRegion.hpp
../include/fsom/Table.hpp
../include/fsom/TimeStretcher.hpp
../include/fsom/Utilities.hpp
)

source_group(sources FILES ${FSOM_SOURCES} ${FSOM_HEADERS})

set(FSOM_EFFECT_SOURCES
	EffectClasses/DSPEffect.cpp
	EffectClasses/Parameter.cpp
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
)

set(FSOM_EFFECT_HEADERS
	../include/fsom/EffectClasses/AsymmetricDelay.hpp
	../include/fsom/EffectClasses/Biquad.hpp
	../include/fsom/EffectClasses/Chorus.hpp
	../include/fsom/EffectClasses/Convolution.hpp
	../include/fsom/EffectClasses/DSPEffect.hpp
	../include/fsom/EffectClasses/Delay.hpp
	../include/fsom/EffectClasses/DelayBase.hpp
	../include/fsom/EffectClasses/Distort.hpp
	../include/fsom/EffectClasses/Echo.hpp
	../include/fsom/EffectClasses/Envelope.hpp
	../include/fsom/EffectClasses/Fade.hpp
	../include/fsom/EffectClasses/FeedbackDelay.hpp
	../include/fsom/EffectClasses/Filters.hpp
	../include/fsom/EffectClasses/Flanger.hpp
	../include/fsom/EffectClasses/Fragment.hpp
	../include/fsom/EffectClasses/FrequencyModulation.hpp
	../include/fsom/EffectClasses/Gain.hpp
	../include/fsom/EffectClasses/Granulate.hpp
	../include/fsom/EffectClasses/Harmonizer.hpp
	../include/fsom/EffectClasses/Modulators.hpp
	../include/fsom/EffectClasses/Pan.hpp
	../include/fsom/EffectClasses/Reverb.hpp
	../include/fsom/EffectClasses/Timestretch.hpp
	../include/fsom/EffectClasses/Transpose.hpp
	../include/fsom/EffectClasses/TransposeUnit.hpp
)
source_group(effects FILES ${FSOM_EFFECT_SOURCES} ${FSOM_EFFECT_HEADERS})

add_library(fsom ${FSOM_SOURCES} ${FSOM_HEADERS} ${FSOM_EFFECT_SOURCES} ${FSOM_EFFECT_HEADERS})
target_link_libraries(fsom ${LIBS} tinyxml Freeverb)
