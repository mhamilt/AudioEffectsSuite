//
//  SimpleFlanger.cpp
//  AudioEffectsSuite
//
//  Created by admin on 20/12/2017.
//  Copyright © 2017 AudioEffectsSuiteTeam. All rights reserved.
//

#include "SimpleFlanger.hpp"
//==============================================================================

double SimpleFlanger::process(double inputSample)
{
	return inputSample;
} 

//==============================================================================

void SimpleFlanger::capGain(double& gain)
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

//==============================================================================
void SimpleFlanger::setEffectGain(double gain)
{

}
