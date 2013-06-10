//(c) Dave Moore & Stephen Pearse 2013
#ifndef _TIMESTRETCHER_HPP__
#define _TIMESTRETCHER_HPP__

#include <iostream>
#include <vector>
#include <string>
#include <sndfile.hh>
#include <stdexcept>
#include "kissfft.hh"
#include <cstdlib>

typedef double ScalarT;
typedef std::complex<ScalarT> ComplexT;
const ScalarT TPI = 2.0 * M_PI;

template<class T>
struct FixedArray{
  T* ptr;
  FixedArray(size_t size) : ptr(new T[size]) {
    for(int n = 0 ; n < size;++n)ptr[n]=0.0;
  }
  ~FixedArray(){ if(ptr) delete[] ptr; }
  T& operator [](int pos){
    return ptr[pos];
  }
};


inline ScalarT wrap(ScalarT input,ScalarT maximum){
    while(input >=maximum){
	input -=maximum;
    }
    while(input < 0 ){
	input +=maximum;
    }
    return input;
}

ScalarT rand_tpi(){
    return ((ScalarT)rand()/(ScalarT)RAND_MAX )* TPI;
}

//class dealing with the reading of blocks of spectral data
class FProc{
  FixedArray<ScalarT> m_output;
  int m_fftsize;
  int m_outPos;
  int m_offset;
  int m_index;
  kissfft<ScalarT> *m_fft;
  kissfft<ScalarT> *m_ifft;
  FixedArray<ComplexT> m_fftinput;
  FixedArray<ComplexT> m_fftResult;
  FixedArray<ComplexT> m_ifftOutput;
  FixedArray<ScalarT> m_magnitudes;
  FixedArray<ScalarT> m_phases;
  FixedArray<ScalarT> m_hann;
public:
  FProc(int fftsize,int offset,kissfft<ScalarT>* fftObject,kissfft<ScalarT>* ifftObject):
    m_fftsize(fftsize),
    m_outPos(offset),
    m_offset(offset),
    m_fft(fftObject),
    m_ifft(ifftObject),
    m_fftinput(fftsize),
    m_fftResult(fftsize),
    m_ifftOutput(fftsize),
    m_output(fftsize),
    m_magnitudes(fftsize),
    m_phases(fftsize),
    m_hann(fftsize)
  {

      for(int n =0; n < fftsize;++n)m_hann[n] = 0.5*(1.0-cos(n * TPI/(fftsize-1)));//*0.5; 
      std::cout << "Fproc spawned -> offset " << m_outPos<< " size "<< m_fftsize<<std::endl;
      
  }
  virtual ~FProc(){
  }
  //function needs to be passed a block to work on
  void process(ScalarT* readData,int size,int globalPos=0){
    /*
      //if each had internal speed you could read data here at different rates
      for(int n =0; n < size;++n ){
	m_fftinput[n] = readData[n] * m_hann[n]; 
      }
      m_fft->transform(m_fftinput.ptr,m_fftResult.ptr);
      ScalarT normalisationFactor = 1.0/size;
      for(int n =1; n < size/2;++n){
	  m_fftResult[n] *= normalisationFactor;
	  m_magnitudes[n] = std::abs(m_fftResult[n]);
	  m_phases[n] = std::arg(m_fftResult[n]);
	  m_fftResult[n] = std::polar<ScalarT>(m_magnitudes[n],   rand_tpi()  );
	  m_fftResult[m_fftsize-n]= m_fftResult[n];
      }
      m_fftResult[0] *= normalisationFactor;
      
      m_ifft->transform(m_fftResult.ptr,m_ifftOutput.ptr);
      //for testing, fake this with a big phasor so we can see the windowing....
      for(int n = 0;n < size;++n){
	m_output[n] = m_ifftOutput[n].real() * m_hann[n] ;//;
// 	m_output[n] = float(n)/float(size) ;//* m_hann[n];
      }
      */
  }
  void sumtobuffer(ScalarT* out, int size, float gain = 1.0f/4.0f)
  {
	// write size
	for(int n =0; n < size;++n){
	    int pos = (n + m_outPos) ;//- m_offset;
	    out[n] += m_output[pos];// * gain;
	}
	
	m_outPos = wrap( m_outPos + size,m_fftsize);
    }
  
};




class TimeStretcher{
public:
  typedef std::vector<FProc*> FprocVector;
private:
  const int m_fftsize; 
  int m_numOverlaps;
  int m_blockSize;
  int m_samplerate;
  std::string m_filename,m_outputfilename;
  FprocVector m_childProcesses;
  SndfileHandle m_inputHandle,m_outputHandle;
  ScalarT* m_audioBuffer;
  ScalarT m_speed;
  kissfft<ScalarT> m_fft;
  kissfft<ScalarT> m_ifft;
public:
  TimeStretcher(ScalarT speed, int fftsize,int numoverlaps,std::string filename,std::string outfilename="output.wav"):
    m_fftsize(fftsize),
    m_numOverlaps(numoverlaps),
    m_filename(filename),
    m_outputfilename(outfilename),
    m_blockSize(fftsize/numoverlaps),
    m_speed(speed),
    m_fft(fftsize,false),
    m_ifft(fftsize,true)
  {
      std::cout << "Spawning timestretcher "<< m_fftsize << " overlaps:" << m_numOverlaps<<std::endl; 
      for(int n = 0; n < numoverlaps;++n){
	  m_childProcesses.push_back(new FProc(m_fftsize, wrap( m_fftsize-m_blockSize*n,m_fftsize ),&m_fft,&m_ifft    ));
      }
      m_inputHandle = SndfileHandle(m_filename,SFM_READ);
      if(m_inputHandle.error())throw std::runtime_error("Unable to open file for reading");
      m_samplerate = m_inputHandle.samplerate();
      m_outputHandle = SndfileHandle(m_outputfilename,SFM_WRITE,SF_FORMAT_PCM_24 | SF_FORMAT_WAV ,1,m_samplerate); 
      if(m_outputHandle.error())throw std::runtime_error("Unable to open file for writing");
      //create audio buffer of appropriate size
      int bufferSize = m_inputHandle.frames()+m_fftsize +1 ;
      m_audioBuffer = new ScalarT[bufferSize ];
      for(int n =0;n < bufferSize;++n)m_audioBuffer[n] = 0.0;
      //fill buffer with audio
      
      ScalarT* temp = new ScalarT[m_inputHandle.frames()* m_inputHandle.channels()];
      
      m_inputHandle.readf(temp,m_inputHandle.frames());
      
      std::cout << "TimeStretcher succesfully created & audio buffer filled(size)" << m_inputHandle.frames()<<std::endl;
      
      const ScalarT *p= temp;
      int channelCount = m_inputHandle.channels();
      int size = m_inputHandle.frames()* channelCount;
      for (int frame = 0;frame < m_inputHandle.frames();++frame  ){
		  m_audioBuffer[frame]= *p;
		  p+=channelCount;
      }


      
      delete[] temp;
      
  }
  void run(){
      int current = 0;
      int totalSize = m_inputHandle.frames() * (1.0/m_speed)+1;
//       std::cout << "Total output frames = "<<totalSize<<std::endl;
      ScalarT* readPosition = m_audioBuffer;
      FixedArray<ScalarT> output(m_blockSize);
      for(int block = 0;block < totalSize;block +=m_blockSize  ){
	  std::cout << ".";
	  m_childProcesses.at(current)->process(readPosition,m_fftsize,block);
	  int offset = m_blockSize* m_speed;
	  readPosition += offset ;
	  ++current;
	  current %= m_numOverlaps;
	  for(int n = 0; n< m_blockSize;++n)output[n]=0;
	  for(int n = 0; n < m_childProcesses.size();++n){
	      m_childProcesses.at(n)->sumtobuffer(output.ptr,m_blockSize,1.0/m_numOverlaps);
	  }
	  m_outputHandle.writef(output.ptr,m_blockSize);
	  
      }
      
    
  }
  
  
  
  
  virtual ~TimeStretcher(){
    for(int n = 0 ;n < m_childProcesses.size();++n){
	delete m_childProcesses.at(n);
    }
    if(m_audioBuffer)delete[] m_audioBuffer;
  }
};

#endif

/*

int main(int argc, char **argv) {
  TimeStretcher stretcher(0.2,2048,4,"speech.wav");
  stretcher.run();
  return 0;
}
*/
