//
//  SimpleFlanger.cpp
//  AudioEffectsSuite
//
//  Created by admin on 20/12/2017.
//  Copyright © 2017 AudioEffectsSuiteTeam. All rights reserved.
//
#ifndef SimpleFlanger_hpp
#include "SimpleFlanger.hpp"
void SimpleFlanger::setupSimpleFlanger(double extSampleRate)
{
    setupDelayEffectBase(extSampleRate*.02);
    timeStep = 1./extSampleRate;
    setEffectParams(.707, extSampleRate*.02, .1);
}
//==============================================================================

double SimpleFlanger::process(double inputSample)
{
	delaySample(inputSample);
	const double out = ((1-fabs(effectGain*.2))*(inputSample) + (effectGain * getInterpolatedOut(modulationIndex)));
	updateModulation();
	return out;
}
//==============================================================================
void SimpleFlanger::updateModulation() //TODO: swap for usage of ModulationBaseClassInheritance
{
	modulationAngle += angleDelta;
	modulationIndex = (currentDelayWriteIndex-(modulationDepth*(1+(sin(modulationAngle))))) - 1;
    modulationIndex = ( (int(modulationIndex) + delayTimeSamples) % delayTimeSamples)
                      + (modulationIndex - floor(modulationIndex) );
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

void SimpleFlanger::setAngleDelta()
{
	const double cyclesPerSample = modulationRate * timeStep;
	angleDelta = cyclesPerSample * 2.0 * internal_Pi;
}

void SimpleFlanger::setEffectParams(double gain, double depth, double rate)
{
    effectGain = gain;
    modulationDepth = depth*.5;
    
    if (modulationDepth > double(delayTimeSamples))
        modulationDepth = double(delayTimeSamples);
    
	modulationRate  = rate;
	setAngleDelta();
}

#endif /* SimpleFlanger_hpp */
