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


#ifndef __BIQUAD__
#define __BIQUAD__

#include <vector>
#include <complex>



namespace fsom{


struct Biquad{
	double A,w0, alpha,cw,sw, Fs , f0,dBgain,a0,a1,a2,b0,b1,b2;
	double x1,x2,y1,y2;

public:
	typedef std::complex<double> Complex;
	Biquad(double sampleRate);
	
	void set_LPF(double frequency,double Q);
	void set_HPF(double frequency,double Q);
	void set_BPF(double frequency,double Q);
	void set_BPF_const_gain(double frequency,double Q);
	void set_BAND_REJECT(double frequency, double Q);
	void set_ALL(double frequency,double Q);
	void set_PEAKING();
	void set_LOWSHELF(double frequency,double S,double amp);
	void set_HIGHSHELF(double frequency,double S,double amp);
	
	void process(const float* in,float* out,int frameSize);

	
	
	void solve_quadratic(Complex a, Complex b, Complex c, Complex* r);
	void print_coef();

	std::complex<double> H(std::complex<double> z);
	std::vector<float> get_amplitudes(int steps);


};
}

#endif
