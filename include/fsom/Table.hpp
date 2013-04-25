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


#ifndef __TABLE_HPP__
#define __TABLE_HPP__
#include <vector>
#include <cmath>
#include <iostream>
#include "Utilities.hpp"
#include <stdlib.h>

namespace fsom{

template<class T>
class Table{
	std::vector<T> m_table;
public:
	Table(int size) : m_table(size,0){
		fill_sine();	
	}	
	void fill_sine(){
		fsom::DebugStream << "filling table with sine" <<std::endl;
		T f = 2.0*PI/(m_table.size()-1);
		for (int n = 0; n < m_table.size();++n){
			m_table.at(n) = sin( T(n) * f );
		}
		m_table.at(m_table.size()-1) = m_table.at(0);
	}
	void fill_cos(){
		fsom::DebugStream << "filling table with cosine" <<std::endl;
		T f = 2.0*PI/(m_table.size()-1);
		for (int n = 0; n < m_table.size();++n){
			m_table.at(n) = cos( T(n) * f );
		}
		m_table.at(m_table.size()-1) = m_table.at(0);
	}
	void fill_hann(){
		fsom::DebugStream << "filling table with hanning" <<std::endl;	
		T f = 2.0*PI/(m_table.size()-1);
		for (int n = 0; n < m_table.size();++n){
			m_table.at(n) =   0.5*(1.0-cos(T(n) * f ));
		}
	}
	void fill_linear(){
		fsom::DebugStream << "filling table linearly" << std::endl;
		T f = 1.0/(m_table.size()-1);
		for (int n = 0; n < m_table.size();++n){
			m_table.at(n) =   n*f;
		}
	}
	
	void fill_triangle(){
		fsom::DebugStream << "filling table with triangle" << std::endl;
		T f = (1.0/(m_table.size()-1));
		for (int n = 0; n < m_table.size();++n){
		      if(n <= m_table.size()/2){
			m_table.at(n) = (((m_table.size() - n) * f) - 1.0);
			
		      }else{
			m_table.at(n) = ((n*f) - 1.0);
		      }
		}
		m_table.at(m_table.size()-1) = m_table.at(0);
	}
	
	//for working with audio
	void fill_linear_offset(){
		fsom::DebugStream << "filling table withg linear data offset between -1 to 1" <<std::endl;
		T f = 2.0/(m_table.size()-1);
		for(int n = 0 ; n < m_table.size(); ++n){
			m_table.at(n) = (n * f) - 1.0; 	
		}
	}
	void fill_clipped(float threshold){
		fsom::DebugStream << "clipping table" <<std::endl;
		T f = 2.0/(m_table.size()-1);
		float t;
		for(int n = 0 ; n < m_table.size(); ++n){
			t = (n * f) - 1.0;
			if(t > threshold){ 
				m_table.at(n) = threshold;  		
			}else if (t < -threshold){
				m_table.at(n) = -threshold;
			}else{
				m_table.at(n)=t;
			}	
		}
	}
	void fill_noise(){
	    fsom::DebugStream << "filling table with noise"<<std::endl;
// 	    T f = 2.0*PI/(m_table.size()-1);
	      for(int n = 0; n < m_table.size();++n){
		      m_table.at(n) = (((rand()%1000)*0.001)*2.0f)-1.0f;
	      }
	}
	void fill_square(){
	    T f = 2.0/(m_table.size()-1);
	    for (int n = 0; n < m_table.size();++n){
	    if(n <= m_table.size()/2.0f){
		m_table.at(n) = 1.0f;
	    }else{
		m_table.at(n) = -1.0f;
	    }
	    }
	    m_table.at(m_table.size()-1) = m_table.at(0);
	}
	
	T at(int index){
		return m_table.at(index);
	}
	T linear_lookup(T index){
		T aIndex,bIndex,r,va,vb;
		r = modf(index,&aIndex);
		bIndex = aIndex+1;
		va = m_table[aIndex];
		vb = m_table[bIndex];
		return (1.0-r)* va + r*vb;
	}
	T cubic_lookup(T index){
	
	}	
	void print_table(){
		fsom::DebugStream << "Table values  = " ;
		for (int n = 0; n < m_table.size();++n){
			fsom::DebugStream << " " << m_table.at(n) << " "; 
		}
		fsom::DebugStream << " " << std::endl;
	}
	double get_size(){
		return m_table.size()-1;
	}
	std::vector<T>& get_table(){
	    return m_table;
	}

};



}
#endif
