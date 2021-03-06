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



#include "fsom/AudioFile.hpp"

#include <sndfile.h>
#include <iostream>

using namespace fsom;

namespace fsom{
struct AudioFileImpl{ //contains main data for AudioFile class to use
	SF_INFO m_sfInfo; //basic info of sound file
	SNDFILE* m_sndFile; //pointer to soundfile
};
}

AudioFile::AudioFile(const std::string& filePath) :
	m_pImpl(new AudioFileImpl), //pImpl idiom for AudioFile created
	m_filePath(filePath), //AudioFile file path set
	m_peakImage(DMAX) //peak image draw depth set to maximum
{
	fsom::DebugStream << "Attempting to spawn audiofile from"<< filePath<<std::endl;
	m_pImpl->m_sndFile = sf_open(m_filePath.c_str(),SFM_READ,&m_pImpl->m_sfInfo); //set soundfile to open and read mode and set filepath and structure to contain info
	if(!m_pImpl->m_sndFile) throw AudioFileException(); //if did not construct pImpl correctly throw exception
	//if(!m_pImpl->m_sndFile) throw "Could not open audio file";
	if(m_pImpl->m_sfInfo.frames == 0) throw  AudioFileException(); //if the audiofile is damaged or contains no data throw exception
	if(!m_pImpl->m_sfInfo.format & SF_FORMAT_WAV & SF_FORMAT_OGG & SF_FORMAT_AIFF & SF_FORMAT_FLAC)throw  AudioFileException();
 	if(m_pImpl->m_sfInfo.samplerate != 44100) throw  AudioFileException();
	//m_peakImage.at(D16) = multiRangeList(m_pImpl->m_sfInfo.channels);
	//m_peakImage.at(D32) = multiRangeList(m_pImpl->m_sfInfo.channels);
	//m_peakImage.at(D64) = multiRangeList(m_pImpl->m_sfInfo.channels);
	//m_peakImage.at(D128) = multiRangeList(m_pImpl->m_sfInfo.channels);
	m_peakImage.at(D256) = multiRangeList(m_pImpl->m_sfInfo.channels);
	m_peakImage.at(D512) = multiRangeList(m_pImpl->m_sfInfo.channels);
	m_peakImage.at(D1024) = multiRangeList(m_pImpl->m_sfInfo.channels);
	m_peakImage.at(D2048) = multiRangeList(m_pImpl->m_sfInfo.channels);
	m_peakImage.at(D4096) = multiRangeList(m_pImpl->m_sfInfo.channels);
    
	//fill_range_list(m_peakImage.at(D16),16);
	//fill_range_list(m_peakImage.at(D32),32);
	//fill_range_list(m_peakImage.at(D64),64);
	//fill_range_list(m_peakImage.at(D128),128);
	fill_range_list(m_peakImage.at(D256),256);
	fill_range_list(m_peakImage.at(D512),512);
	fill_range_list(m_peakImage.at(D1024),1024);
	fill_range_list(m_peakImage.at(D2048),2048);
	fill_range_list(m_peakImage.at(D4096),4096);
	//fsom::DebugStream << "RangeList  size= " << m_peakImage.at(D128).at(0).size() <<std::endl;
	//show_info(); //display audio data on construction
}

AudioFile::~AudioFile(){
	sf_close(m_pImpl->m_sndFile); //on destruction properly close SNDFILE and clean up memory 
}

//read a blocksized chunk from the file into an interleaved temporary buffer(sfreadfloat)
void AudioFile::get_block(float** out, int blockSize){
	
	float temp[4096]; //create a temporary array of floats
	assert(blockSize < 4096); //assert blocksize within maximum block size
	
	
	
	size_t ret = sf_readf_float(m_pImpl->m_sndFile,temp,blockSize); //fills temp with float data the size of blockSize taken from sndfile
		
	
	if(m_pImpl->m_sfInfo.channels == 1){ //if the sound file is mono
		for(int channel = 0; channel<2;++channel){ //go through same data twice to make stereo file
			for(int n =0; n < blockSize; ++n){ //go through the block size 
				float in = temp[n]; //get value within temp and set to in
				out[channel][n] =in ; //set sample within channel to value	
			}
		}	
	}else { //if channel size is larger than one
	
	// call deinterleave on the temp buffer into what we call out.
	deinterleave(temp,out,m_pImpl->m_sfInfo.channels,blockSize); //function to fill a float** buffer of a given blocksize with stored data
	}
	
}

//print sound file data to console
void AudioFile::show_info() const{
	fsom::DebugStream << "----------------------------" << std::endl;
	fsom::DebugStream << "Sound Name = " << m_filePath << std::endl;
	fsom::DebugStream << "Samplerate = " << m_pImpl->m_sfInfo.samplerate << std::endl;
	fsom::DebugStream << "Channels = " << m_pImpl->m_sfInfo.channels << std::endl;
	fsom::DebugStream << "File Size (in samples) = " << m_pImpl->m_sfInfo.frames<<std::endl;
	fsom::DebugStream << "Format = " << m_pImpl->m_sfInfo.format << std::endl;
	fsom::DebugStream << "Sections = " << m_pImpl->m_sfInfo.sections << std::endl;
	fsom::DebugStream << "Seekable = " << m_pImpl->m_sfInfo.seekable << std::endl;
	fsom::DebugStream << "----------------------------" << std::endl;
	fsom::DebugStream << "Size of buffer loaded " << m_audioBuffer.size()<<std::endl;

}

//print peak data to console
void AudioFile::show_peaks(DrawDepth dd,int channel ) const{
	const rangeList& rl = get_peak_image(dd,channel); //get a rangelist determined by the drawdepth
	for (int n = 0; n< rl.size();++n){
		fsom::DebugStream << rl[n].fmin << " " << rl[n].fmax << " "; //couts the min and max value of each part of rangelist
	}
}


int AudioFile::get_channels() const {return m_pImpl->m_sfInfo.channels;} //returns the sound files number of channels 

SampleLength AudioFile::get_file_length() const {return m_pImpl->m_sfInfo.frames;} //returns the file length of the sound file

std::string AudioFile::get_file_path(){return m_filePath;} //returns the file path of sound file

void AudioFile::seek(size_t pos){
	sf_seek(m_pImpl->m_sndFile, pos, SEEK_SET); //seeks to sample position within the sound file
}


//job is to go through the whole soundfile working at the smallest blocksize
//determine minimum and maximum for the block for each of the channels#
//store results in the output array which is one rangList per channel
void AudioFile::fill_range_list(multiRangeList& output, int blockSize){

	//reads the file in blocks of the blocksize, each block will be multichannel
	//for each block go through each sample frame.
	//for each sample frame determine if we have reached the minimum & maximum of this block of this channel
	int channels = m_pImpl->m_sfInfo.channels; //get channel number and store in channels
	int size = m_pImpl->m_sfInfo.frames / blockSize + 1; //set size to the amount of blocks to be filled
	std::vector<float> temp(blockSize*channels); 
	assert(output.size() == channels );
	for (int c = 0; c < channels;++c){
		//allocating a new block without copying
		output[c] = rangeList(size);
	}
	//seek to the beginning of the file and seek through the entire file..
	seek(0);
	int blockCount = 0; //start the block count at 0
	size_t ret = sf_readf_float(m_pImpl->m_sndFile,&temp[0],blockSize); 
	while(ret > 0 ){
		//temp now contains multichannel sampleframes up to the blocksize
		//n represents the sampleframe
		for (int n=0;n < ret ;++n ){
			for(int c=0;c < channels;++c){
				float a = temp[n*channels+c];
				FMinMax& t = output[c][blockCount];
				if(a < t.fmax){
					t.fmax = a;
				}
				if(a > t.fmin){
					t.fmin = a;
				}
			}
		}

		//finally read the next block
		++blockCount;
		ret = sf_readf_float(m_pImpl->m_sndFile,&temp[0],blockSize);
	}

}

typedef std::vector<FMinMax> rangeList;

const rangeList& AudioFile::get_peak_image(DrawDepth dd, int channel )const {
		return m_peakImage.at(dd).at(channel);
}
