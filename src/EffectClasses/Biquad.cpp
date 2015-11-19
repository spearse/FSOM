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



#include "fsom/EffectClasses/Biquad.hpp"

#include <cmath>
#include <iostream>
#include "fsom/Utilities.hpp"

using namespace fsom;

	Biquad::Biquad(float sampleRate):
		A(0),w0(0),alpha(0),cw(0),sw(0),Fs(sampleRate),f0(0),
			dBgain(0),a0(0),a1(0),a2(0),b0(0),b1(0),b2(0),
			x1(0),x2(0),y1(0),y2(0)
	{
	
	}
	
	void Biquad::set_LPF(float frequency,float Q){
		A = 0;
		f0 = frequency;
		w0 = TWOPI*(f0/Fs);
		cw = cos(w0);
		sw = sin(w0);
		alpha = sin(w0)/(2.f*Q);
		b0 =  (1.f - cw)/2.f;
		b1 =   1.f - cw;
		b2 =  (1.f - cw)/2.f;
		a0 =   1.f + alpha;
		a1 =  -2.f*cw;
		a2 =   1.f - alpha;
	}
	
	void Biquad::set_HPF(float frequency,float Q){
		A = 0;
		f0 = frequency;
		w0 = TWOPI*(f0/Fs);
		cw = cos(w0);
		sw = sin(w0);
		alpha = sin(w0)/(2.f*Q);
		b0 =  (1.f + cw)/2.f;
		b1 =  -( 1.f + cw);
		b2 =  (1.f + cw)/2.f;
		a0 =   1.f + alpha;
		a1 =  -2.f*cw;
		a2 =   1.f - alpha;
	}
	void Biquad::set_BPF(float frequency,float Q){
		A = 0;
		f0 = frequency;
		w0 = TWOPI*(f0/Fs);
		cw = cos(w0);
		sw = sin(w0);
		alpha = sin(w0)/(2.f*Q);
	//	b0 = Q*alpha;
		b0 = sw/float(2.f);
		b1 = 0.f;
		b2 = -Q*alpha;
		a0 =   1.f + alpha;
		a1 =  -2.f*cw;
		a2 =   1.f - alpha;
/*
			b0 =   sin(w0)/2  =   Q*alpha
            b1 =   0
            b2 =  -sin(w0)/2  =  -Q*alpha
            a0 =   1 + alpha
            a1 =  -2*cos(w0)
            a2 =   1 - alpha
*/
	}
	void Biquad::set_BPF_const_gain(float frequency,float Q){
		A = 0;
		f0 = frequency;
		w0 = TWOPI*(f0/Fs);
		cw = cos(w0);
		sw = sin(w0);
		alpha = sin(w0)/(2.f*Q);
		b0 =   alpha;
		b1 =   0;
		b2 =  -alpha;
		a0 =   1 + alpha;
		a1 =  -2*cw;
		a2 =   1 - alpha;
	}
	void Biquad::set_BAND_REJECT(float frequency, float Q){
		A = 0;
		f0 = frequency;
		w0 = TWOPI*(f0/Fs);
		cw = cos(w0);
		sw = sin(w0);
		alpha = sin(w0)/(2.f*Q);
		b0 =   1;
		b1 =  -2*cw;
		b2 =   1;
		a0 =   1 + alpha;
		a1 =  -2*cw;
		a2 =   1 - alpha;
	}
	void Biquad::set_ALL(float frequency,float Q){
		A = 0;
		f0 = frequency;
		w0 = TWOPI*(f0/Fs);
		cw = cos(w0);
		sw = sin(w0);
		alpha = sin(w0)/(2.f*Q);
		b0 =   1 - alpha;
		b1 =  -2*cw;
		b2 =   1 + alpha;
		a0 =   1 + alpha;
		a1 =  -2*cw;
		a2 =   1 - alpha;		
	}
	void Biquad::set_PEAKING(){
		//TODO
		b0 =   1 + alpha*A;
		b1 =  -2*cw;
		b2 =   1 - alpha*A;
		a0 =   1 + alpha/A;
		a1 =  -2*cw;
		a2 =   1 - alpha/A;
	}
	void Biquad::set_LOWSHELF(float frequency,float S,float amp){
		A = 0;//FIXME
		f0 = frequency;
		w0 = TWOPI*(f0/Fs);
		cw = cos(w0);
		sw = sin(w0);
		alpha= sw/2 * sqrt( (A + 1/A)*(1/S - 1) + 2 ); 
		b0 =    A*( (A+1) - (A-1)*cw + 2*sqrt(A)*alpha );
		b1 =  2*A*( (A-1) - (A+1)*cw                   );
		b2 =    A*( (A+1) - (A-1)*cw - 2*sqrt(A)*alpha );
		a0 =        (A+1) + (A-1)*cw + 2*sqrt(A)*alpha;
		a1 =   -2*( (A-1) + (A+1)*cw                   );
		a2 =        (A+1) + (A-1)*cw - 2*sqrt(A)*alpha;
	}
	void Biquad::set_HIGHSHELF(float frequency,float S,float amp){
		A = 0;//FIXME
		f0 = frequency;
		w0 = TWOPI*(f0/Fs);
		cw = cos(w0);
		sw = sin(w0);
		alpha= sw/2 * sqrt( (A + 1/A)*(1/S - 1) + 2 ); 
		b0 =    A*( (A+1) + (A-1)*cw + 2*sqrt(A)*alpha );
		b1 = -2*A*( (A-1) + (A+1)*cw                   );
		b2 =    A*( (A+1) + (A-1)*cw - 2*sqrt(A)*alpha );
		a0 =        (A+1) - (A-1)*cw + 2*sqrt(A)*alpha;
		a1 =    2*( (A-1) - (A+1)*cw                   );
		a2 =        (A+1) - (A-1)*cw - 2*sqrt(A)*alpha;

	}
	
	void Biquad::process(const float* in,float* out,int frameSize){
		while(--frameSize >= 0){ 
			float x0 = *in;
			float y0;
			y0 = (b0/a0)*x0 + (b1/a0)*x1 + (b2/a0)*x2 - (a1/a0)*y1 - (a2/a0)*y2; 
			x2 = x1;		
			x1 = x0;
			y2 = y1;
			y1 = y0; 
			*out = y0;
			++in;
			++out;
		}
	}

	typedef std::complex<float> Complex;
	
	void Biquad::solve_quadratic(Complex a, Complex b, Complex c, Complex* r){
		Complex t = sqrt(b*b - 4.f*a*c);
		r[0] = (-b + t) / (2.f*a);
		r[1] = (-b - t) / (2.f*a);
	}

	void Biquad::print_coef(){
		//fsom::DebugStream << "b0 = " << b0;
		//fsom::DebugStream << " b1 = " << b1;
		//fsom::DebugStream << " b2 = " << b2;
		//fsom::DebugStream << " a0 = " << a0;
		//fsom::DebugStream << " a1 = " << a1;
		//fsom::DebugStream << " a2 = " << a2 << std::endl;
		Complex r[2];
		solve_quadratic(b2,b1,b0,r);
		//fsom::DebugStream << "Zeros at  : " << r[0] << " " << r[1] << std::endl;
		//fsom::DebugStream << "Zeros args: " << arg(r[0]) << " " << arg(r[1]) << std::endl;
		solve_quadratic(a2,a1,a0,r);
		//fsom::DebugStream << "Poles at  : " << r[0] << " " << r[1] << std::endl;
		//fsom::DebugStream << "Poles args: " << arg(r[0])*(Fs/(TWOPI)) << " " << arg(r[1])*(Fs/(TWOPI)) << std::endl;
	}
	

	std::complex<float> Biquad::H(std::complex<float> z){

		return ( b0 + b1*pow(z,-1.f) + b2*pow(z,-2.f) ) /(a0 + a1*pow(z,-1.f) + a2*pow(z,-2.f) );
	}

	std::vector<float> Biquad::get_amplitudes(int steps){

		int N = 44100;

		std::complex<float> j(0,1);

		std::vector<float> amplitudes(steps);
		
		//goes through coefficients and pushes them into a vector to allow visualization
		for(int n = 0; n < steps ; ++n){
			float x = pow( (float(n) / steps), 2 ) * float(N) / 2.f;
			float w = TWOPI/N * x;
			std::complex<float> z = exp(j*w);
			float val = 20.f*log10(abs( H(z)));
			amplitudes[n] = val;
		}
		
		return amplitudes;
	}

