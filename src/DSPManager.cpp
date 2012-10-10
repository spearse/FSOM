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



#include "fsom/DSPManager.hpp"
#include "fsom/EffectClasses/Gain.hpp"
#include "fsom/EffectClasses/Pan.hpp"
#include "fsom/EffectClasses/Modulators.hpp"
#include "fsom/EffectClasses/Filters.hpp"
#include "fsom/EffectClasses/Delay.hpp"
#include "fsom/EffectClasses/Echo.hpp"
#include "fsom/EffectClasses/Reverb.hpp"
#include "fsom/EffectClasses/Fade.hpp"
#include "fsom/EffectClasses/Transpose.hpp"
#include "fsom/EffectClasses/Distort.hpp"
#include "fsom/EffectClasses/FeedbackDelay.hpp"
#include "fsom/EffectClasses/Timestretch.hpp"
#include "fsom/EffectClasses/Flanger.hpp"
#include "fsom/EffectClasses/Granulate.hpp"
#include "fsom/EffectClasses/Envelope.hpp"
#include "fsom/EffectClasses/Convolution.hpp"
#include "fsom/EffectClasses/Fragment.hpp"
#include "fsom/EffectClasses/Chorus.hpp"
#include "fsom/EffectClasses/Harmonizer.hpp"
#include "fsom/EffectClasses/AsymmetricDelay.hpp"

using namespace fsom;

void fsom::register_fx_factories(){
DSPManager::get_instance().register_factory("Gain",Gain::create);
DSPManager::get_instance().register_factory("Pan",Pan::create);
DSPManager::get_instance().register_factory("Tremelo",Tremelo::create);
DSPManager::get_instance().register_factory("RingMod",RingMod::create);
DSPManager::get_instance().register_factory("Delay",Delay::create);
DSPManager::get_instance().register_factory("SimpleReverb",SimpleReverb::create);
DSPManager::get_instance().register_factory("Reverb",Reverb::create);
DSPManager::get_instance().register_factory("Fade",Fade::create);

DSPManager::get_instance().register_factory("Fragment",Fragment::create);
DSPManager::get_instance().register_factory("Echo",Echo::create);

DSPManager::get_instance().register_factory("CombFilter",CombFilter::create);
DSPManager::get_instance().register_factory("LowPassFilter",LowPassFilter::create);
DSPManager::get_instance().register_factory("HighPassFilter",HighPassFilter::create);
DSPManager::get_instance().register_factory("ResonatingFilter",ResonatingFilter::create);
DSPManager::get_instance().register_factory("BandRejectFilter",BandRejectFilter::create);
DSPManager::get_instance().register_factory("BandPassFilter",BandPassFilter::create);
DSPManager::get_instance().register_factory("MultiBandFilter",MultiBandFilter::create);

DSPManager::get_instance().register_factory("Transpose",Transpose::create);
DSPManager::get_instance().register_factory("Distort",Distort::create);
DSPManager::get_instance().register_factory("Feedbackdelay",FeedbackDelay::create);
// DSPManager::get_instance().register_factory("Timestretch",Timestretch::create);
DSPManager::get_instance().register_factory("Flanger",Flanger::create);
DSPManager::get_instance().register_factory("Granulate",Granulate::create);
DSPManager::get_instance().register_factory("Chorus",Chorus::create);
DSPManager::get_instance().register_factory("Envelope",Envelope::create);
DSPManager::get_instance().register_factory("Convolution",Convolution::create);

DSPManager::get_instance().register_factory("LowAmpMod",LowAmpMod::create);
DSPManager::get_instance().register_factory("HighAmpMod",HighAmpMod::create);
DSPManager::get_instance().register_factory("LowFreqModulation",LowFreqMod::create);
DSPManager::get_instance().register_factory("HighFreqModulation",HighFreqMod::create);
DSPManager::get_instance().register_factory("Harmoniser",Harmoniser::create);

DSPManager::get_instance().register_factory("AsymmetricDelay",AsymmetricDelay::create);



}
