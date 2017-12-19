//
//  SimpleDelay.cpp
//  AudioEffectsSuiteDevelopment
//
//  Created by admin on 18/12/2017.
//  Copyright © 2017 AudioEffectsSuiteTeam. All rights reserved.
//

#include "SimpleDelay.hpp"

//==============================================================================

double SimpleDelay::process(double inputSample)
{
		delaySample(inputSample*.707+ .707*getInterpolatedOut(currentDelayIndex));
		return (getInterpolatedOut(double(currentDelayIndex)*.5) + inputSample);
		
}
