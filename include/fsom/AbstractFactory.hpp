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





#ifndef ABSTRACTFACTORY_HPP
#define ABSTRACTFACTORY_HPP

#include <memory>
#include <map>
#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <typeinfo>

namespace fsom{

class AbstractFactoryException{};

template<class T, class Arg1> //abstract factory template class
class AbstractFactory
{
public:
	typedef std::shared_ptr<T> ptr;
	typedef ptr(*Factory)(Arg1);
private:
	typedef std::map<std::string,Factory> FactoryMap; //typedef for a map of Factory types and their assigned names
	FactoryMap m_factories;
	AbstractFactory(){} //AbstractFactory constructor
	/// Abstract factory is non copyable
	AbstractFactory(const AbstractFactory<T,Arg1>&); //AbstractFactory constructor taking a const reference to defined
	AbstractFactory& operator = (const AbstractFactory<T,Arg1>&);

	~AbstractFactory(){}
public:
	///register factory and add into the factory map
	bool register_factory(const std::string& name, Factory fact){
		FSOM_ASSERT( fact != 0 && "Factory function given was null");
		FSOM_ASSERT(m_factories.count(name) == 0);
		m_factories[name] = fact;

		return true;
	}
	///run through the map and return the pointer to the name given in function
	ptr create(const std::string& name, const Arg1& arg1) const {
		typename FactoryMap::const_iterator it;
		it = m_factories.find(name);

		FSOM_ASSERT( it == m_factories.end() && "Factory with given name not found .");
		if((*it).second == 0 || it == m_factories.end() )throw "Unable to find factory";
		
		return (*it).second(arg1);
	}
	///get the singleton instance of the AbstractFactory class
	static AbstractFactory<T,Arg1>& get_instance()
	{
		static AbstractFactory<T,Arg1> singleton;
		return singleton;
	};
	void remove_factory(std::string name){

	}
	std::vector<std::string> get_names() const {
		typename FactoryMap::const_iterator it;
		std::vector<std::string> t;
		for( it=m_factories.begin(); it != m_factories.end(); ++it ){
		    t.push_back(it->first);
		}
	    return t;
	}
	int get_factory_size() const {
		return m_factories.size();
	}
};


}
#endif // ABSTRACTFACTORY_HPP
