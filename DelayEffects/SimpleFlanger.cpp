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
	delaySample(inputSample);
	const double out = (inputSample)+(getInterpolatedOut(modulationIndex));
	updateModulation();
	return out;
}
//==============================================================================
void SimpleFlanger::updateModulation()
{
	modulationAngle+= angleDelta;
	modulationIndex = currentDelayWriteIndex-(modulationDepth*(1+(sin(modulationAngle))));
	
	// possibly add clipping of mudulation via tan or similar
	// offset with
	// tanh(sin(x))+tanh(1);
	//	modulationIndex = currentDelayWriteIndex-
	//					 (modulationDepth *
	//					 (tanh((4*sin(modulationAngle) ) ) + 0.761594156)*0.65 );
	
	return;
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
	modulationDepth = depth*.5;
	modulationRate  = rate;
	setAngleDelta();
}
