//
//  SimpleFlanger.cpp
//  AudioEffectsSuite
//
//  Created by admin on 20/12/2017.
//  Copyright © 2017 AudioEffectsSuiteTeam. All rights reserved.
//
#ifndef SimpleFlanger_hpp
#include "SimpleFlanger.hpp"

SimpleFlanger::SimpleFlanger()
{
}

SimpleFlanger::SimpleFlanger(double extSampleRate) : DelayEffectBase(static_cast<int>(extSampleRate*0.02))
{
}

/** Destructor. */
SimpleFlanger::~SimpleFlanger()
{
}

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
	modulationIndex = (currentDelayWriteIndex-(modulationDepth*(1+(sin(modulationAngle))))) - 12;
    modulationIndex = ( (int(modulationIndex) + delayTimeSamples) % delayTimeSamples)
                      + (modulationIndex - floor(modulationIndex) );
}
//==============================================================================

double SimpleFlanger::capGain(double gain)
{
	if (gain > 1.)
	{
		gain = 1.;
	}
	else if (gain < -1.)
	{
		gain = -1.;
	}
	return gain;
}

//==============================================================================
void SimpleFlanger::setEffectGain(const double gain)
{
    effectGain = capGain(gain);
}


void SimpleFlanger::setDepth(const double depth)
{
    if (depth > double(delayTimeSamples))
        modulationDepth = double(delayTimeSamples)-1;
    else
        modulationDepth = depth;
}

void SimpleFlanger::setRate(const double rate)
{
    modulationRate = rate;
    setAngleDelta();
}
//==============================================================================
void SimpleFlanger::setAngleDelta()
{
	const double cyclesPerSample = modulationRate * timeStep;
	angleDelta = cyclesPerSample * 2.0 * internal_Pi;
}

void SimpleFlanger::setEffectParams(const double gain, const double depth, const double rate)
{
    setEffectGain(gain);
    setDepth(depth);
    setRate(rate);
}

#endif /* SimpleFlanger_hpp */
