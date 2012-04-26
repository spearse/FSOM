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


#ifndef _GENERATOR_HPP_
#define _GENERATOR_HPP_
#include "Table.hpp"
#include "EffectClasses/DSPEffect.hpp"
#include "Utilities.hpp"
#include <boost/shared_ptr.hpp>

#include <string>

class TiXmlElement;  

namespace fsom{

  
class Generator :public DSPEffect{
public:
	enum GeneratorType{
		GEN_Sine =0,
		GEN_Cos,
		GEN_Square,
		GEN_Noise
	};
private:
	GeneratorType m_genType;
	Table<double> m_shape;
	Phasor m_pitchPhasor;
	bool m_noiseState;
	std::string m_filePath;
public: 
	Generator(GeneratorType type);
	virtual ~Generator();
	virtual void process(float** input, float** output, int frameSize, int channels);
	void set_generator_voice(GeneratorType type);
	int get_generator_voice();
	virtual DSPEffectPtr clone() { return DSPEffectPtr( new Generator(*this) ); }
	void set_noise_state(bool state);
	void save_to_xml_node(TiXmlElement* node );
	void set_file_path(std::string filepath);
	Table<double> get_table()const;
private:
	void fill_generator_accordingly();
	
};

typedef boost::shared_ptr<Generator> GeneratorPtr;


}

#endif
