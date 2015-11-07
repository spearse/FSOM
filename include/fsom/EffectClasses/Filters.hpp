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


#ifndef FILTERS_HPP
#define FILTERS_HPP
#include "DSPEffect.hpp"
#include "DelayBase.hpp"
#include "Biquad.hpp"
#include "../Table.hpp"

namespace fsom{
	
class Filter : public DSPEffect{
	Biquad m_filterLeft,m_filterRight;
public:
      Filter(dspCreationStruct data):DSPEffect(data), m_filterLeft(44100), m_filterRight(44100){;}
      virtual ~Filter(){;}
      void process(float** input, float** output, int frameSize, int channels)=0 ;
	  virtual void one_shot()=0;
/*      static DSPEffectPtr create() =0;*/
      std::vector<float> get_amplitudes(int steps);
      Biquad& get_biquad_left(){return m_filterLeft;}
      Biquad& get_biquad_right(){return m_filterRight;}

};

class MultiBandFilter : public DSPEffect{
	typedef std::shared_ptr<Biquad> BiquadPtr;
	typedef std::vector<BiquadPtr> BiquadPtrList;
	BiquadPtrList m_biquadList;
	const int m_biquadAmount;
	std::vector<float> m_combinedAmps;
public:
      MultiBandFilter(dspCreationStruct data);
      ~MultiBandFilter(){;}
      
      void process(float** input, float** output, int frameSize, int channels);
      void one_shot(int steps);

      const std::vector<float>& get_combined_amplitudes();
      static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new MultiBandFilter(data)) ; }
      virtual DSPEffectPtr clone() { return DSPEffectPtr( new MultiBandFilter(*this) ); }
};

class LowPassFilter : public Filter{	
public:
	LowPassFilter(dspCreationStruct data);
	~LowPassFilter();
	void process(float** input, float** output, int frameSize, int channels) ;
	static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new LowPassFilter(data)) ; }
	void one_shot();
	virtual DSPEffectPtr clone() { return DSPEffectPtr( new LowPassFilter(*this) ); }
};
class HighPassFilter : public Filter{
public:
	HighPassFilter(dspCreationStruct data);
	~HighPassFilter();
	void process(float** input, float** output, int frameSize, int channels) ;
	static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new HighPassFilter(data)) ; }
	void one_shot();
	virtual DSPEffectPtr clone() { return DSPEffectPtr( new HighPassFilter(*this) ); }
};
class ResonatingFilter : public Filter{
public:
	ResonatingFilter(dspCreationStruct data);
	~ResonatingFilter();
	void process(float** input, float** output, int frameSize, int channels) ;
	static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new ResonatingFilter(data)) ; }
	void one_shot();
	virtual DSPEffectPtr clone() { return DSPEffectPtr( new ResonatingFilter(*this) ); }
};

class BandRejectFilter : public Filter{
public:
	BandRejectFilter(dspCreationStruct data);
	~BandRejectFilter();
	void process(float** input, float** output, int frameSize, int channels) ;
	static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new BandRejectFilter(data)) ; }
	std::vector<float>& get_amplitudes(int steps);
	void one_shot();
	virtual DSPEffectPtr clone() { return DSPEffectPtr( new BandRejectFilter(*this) ); }
};

class BandPassFilter : public Filter{
public:
	BandPassFilter(dspCreationStruct data);
	~BandPassFilter();
	void process(float** input, float** output, int frameSize, int channels) ;
	static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new BandPassFilter(data)) ; }
	std::vector<float>& get_amplitudes(int steps);
	void one_shot();
	virtual DSPEffectPtr clone() { return DSPEffectPtr( new BandPassFilter(*this) ); }
};

class AllPassFilter : public Filter{
public:
	AllPassFilter(dspCreationStruct data);
	~AllPassFilter();
	void process(float** input, float** output, int frameSize, int channels) ;
	static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new AllPassFilter(data)) ; }
	std::vector<float>& get_amplitudes(int steps);
	void one_shot();
	virtual DSPEffectPtr clone() { return DSPEffectPtr( new AllPassFilter(*this) ); }
};


class CombFilter :public DSPEffect
{
	DelayBase<float> m_delayUnitL,m_delayUnitR;	
	//Table<float> m_table;
	Phasor m_phasor;
public:
	CombFilter(dspCreationStruct data);
	virtual ~CombFilter();
	void process(float** input, float** output, int frameSize, int channels) ;
	static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new CombFilter(data)) ; }
	std::vector<float>& get_amplitudes(int steps);
	void one_shot();
	virtual DSPEffectPtr clone() { return DSPEffectPtr( new CombFilter(*this) ); }
	
};			
	
	
	
	
}
#endif 
