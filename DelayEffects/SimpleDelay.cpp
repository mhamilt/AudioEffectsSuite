//
//  SimpleDelay.cpp
//  AudioEffectsSuiteDevelopment
//
//  Created by admin on 18/12/2017.
//  Copyright © 2017 AudioEffectsSuiteTeam. All rights reserved.
//
#ifndef SimpleDelay_cpp
#define SimpleDelay_cpp

#include "SimpleDelay.hpp"

//==============================================================================

double SimpleDelay::process(double inputSample)
{
	delaySample((inputSample*delayGain)+ (feedbackGain*getInterpolatedOut(currentDelayWriteIndex)));
	const double out = getInterpolatedOut(currentDelayWriteIndex) + inputSample;
	return out;
}

//==============================================================================

void SimpleDelay::capGain(double& gain)
{
	if (gain > 1.)
	{
		gain = 1.;
	}
	else if (gain < -1.)
	{
		gain = -1.;
	}
	return;
}

void SimpleDelay::setDelayGain(double gain)
{
	capGain(gain);
	delayGain = gain;
}

void SimpleDelay::setFeedbackGain(double gain)
{
	capGain(gain);
	feedbackGain = gain;
}
//==============================================================================
#endif /* SimpleDelay_hpp */
