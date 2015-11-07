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



#ifndef __AUDIOFILE_HPP__
#define __AUDIOFILE_HPP__

#include <string>
#include <vector>
#include <memory>
#include <memory>
#include "Utilities.hpp"

namespace fsom{
  
class AudioFileException{};

///pImpl struct type to contain basic libsndfile data
struct AudioFileImpl; 

///AudioFile Class Declaration
class AudioFile{
public:
	
	typedef std::vector<float> audioVec; ///audioVec is a vector of floats
	typedef std::vector<FMinMax> rangeList; ///rangeList is a vector of MinMax values contained in a channel
	typedef std::vector<rangeList> multiRangeList; ///multiRangeList is a vector of rangeLists - one for each channel
	typedef std::vector<multiRangeList> audioPeakImage; ///audioPeakImage is a vector of multiRangeLists
private:
	std::auto_ptr<AudioFileImpl> m_pImpl; ///the structure containing libsndfile data for AudioFile class 
	audioVec m_audioBuffer; ///audio buffer for the audio file
	std::string m_filePath; ///string path for the AudioFile
	int m_mode; ///the mode 
	audioPeakImage m_peakImage; ///Contains all peak image data of AudioFile
public:
	///enums to set the depth of peak image draw values
	enum DrawDepth{
		//D16 =0,
		//D32,
		//D64,
		//D128,
		D256=0,
		D512,
		D1024,
		D2048,
		D4096,
		DMAX /// keep last
	};
private:
    ///the draw depth of peak image
	DrawDepth m_drawDepth;
	///fill_range_list goes through the whole soundfile working at the smallest blocksize
	///and determines the minimum and maximum for the block for each of the channels
    ///storing results in the output array which is one rangeList per channel
	void fill_range_list(fsom::AudioFile::multiRangeList& output, int blockSize);
public:
	///AudioFile constructor requires a string address for file
	AudioFile(const std::string& filePath);
	///AudioFile Destructor
	~AudioFile();
	///function to print AudioFile properties
	void show_info() const;
	///function to print peak data 
	void show_peaks(DrawDepth dd,int channel ) const;
	///gets a block of audio data the size of blockSize
	void get_block(float** out,int blockSize);
	///returns the AudioFile's number of channels
	int get_channels() const;
	///returns the AudioFile's file length
	SampleLength get_file_length() const;
	///returns the AudioFile's set file path
	std::string get_file_path();
	///function to seek to position within the AudioFile
	void seek(size_t pos);
	///returns a const reference to a rangeList describing the peak image
	const rangeList& get_peak_image(fsom::AudioFile::DrawDepth dd, int channel) const;

};

typedef std::shared_ptr<AudioFile> AudioFilePtr;


} //fsom

#endif //__AUDIOFILE__
