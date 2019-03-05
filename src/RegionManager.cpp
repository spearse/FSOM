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



#include "fsom/RegionManager.hpp"

#include "fsom/AudioRegion.hpp"
#include "fsom/AdditiveSynthesisRegion.hpp"
#include "fsom/NoiseRegions.hpp"
#include "fsom/GranularRegion.hpp"
#include "fsom/SubtractiveSynthesisRegion.hpp"
#include "fsom/GranularRegion.hpp"
#include "fsom/DummyRegion.hpp"

using namespace fsom;

void fsom::register_region_factories(){
RegionManager::get_instance().register_factory("Audio",AudioRegion::create);
RegionManager::get_instance().register_factory("WhiteNoise",WhiteNoise::create);
RegionManager::get_instance().register_factory("Granular",GranularRegion::create);
RegionManager::get_instance().register_factory("AdditiveSynthesis",AdditiveSynthesisRegion::create);
RegionManager::get_instance().register_factory("SubtractiveSynthesis",SubtractiveSynthesisRegion::create);
RegionManager::get_instance().register_factory("GranularSynthesis",GranularRegion::create);
RegionManager::get_instance().register_factory("Dummy",DummyRegion::create);
	

}
