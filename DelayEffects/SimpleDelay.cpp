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
		delaySample(inputSample*.707);
		return (getInterpolatedOut(double(currentDelayIndex)) + inputSample);
//		return inputSample;
}
