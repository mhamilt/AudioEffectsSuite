//
//  FilteredDelay.cpp
//  
//
//  Created by admin on 10/01/2018.
//
//
#ifndef FilteredDelay_hpp
#include "FilteredDelay.hpp"
//==============================================================================

double FilteredDelay::process(double inputSample)
{
	delaySample(applyFilter((inputSample*delayGain)+ feedbackGain*getInterpolatedOut(currentDelayWriteIndex)));
	const double out = getInterpolatedOut(currentDelayWriteIndex) + inputSample;
	return out;
}

//==============================================================================

void FilteredDelay::capGain(double& gain)
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

void FilteredDelay::setDelayGain(double gain)
{
	capGain(gain);
	delayGain = gain;
}

void FilteredDelay::setFeedbackGain(double gain)
{
	capGain(gain);
	feedbackGain = gain;
}
//==============================================================================
#endif /* FilteredDelay_hpp */
