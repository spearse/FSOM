#ifndef FREQUENCYMODULATION_HPP
#define FREQUENCYMODULATION_HPP
#include "DSPEffect.hpp"
#include "TransposeUnit.hpp"
#include "../Table.hpp"
#include "../Utilities.hpp"

namespace fsom{

class LowFrequencyModulation : public DSPEffect{
  TransposeUnit m_transposeUnit;
  Table<float> m_table;
  Phasor m_phasor;
public:
  LowFrequencyModulation(dspCreationStruct creationStruct);
virtual ~LowFrequencyModulation();
  virtual void reset_effect();
  static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new LowFrequencyModulation(data)) ; } 
  virtual DSPEffectPtr clone() { return DSPEffectPtr( new LowFrequencyModulation(*this) ); }
   virtual void process(float** input, float** output, int frameSize, int channels);
  
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class HighFrequencyModulation : public DSPEffect{
  TransposeUnit m_transposeUnit;
  Table<float> m_table;
  Phasor m_phasor;
public:
  HighFrequencyModulation(dspCreationStruct creationStruct);
virtual ~HighFrequencyModulation();
  virtual void reset_effect();
  static DSPEffectPtr create(dspCreationStruct data) { return DSPEffectPtr(new HighFrequencyModulation(data)) ; } 
  virtual DSPEffectPtr clone() { return DSPEffectPtr( new HighFrequencyModulation(*this) ); }
   virtual void process(float** input, float** output, int frameSize, int channels);
  
};


}
#endif // FREQUENCYMODULATION_HPP
