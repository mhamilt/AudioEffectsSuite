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
SimpleDelay::SimpleDelay(int maxDelayInSamples, int samplingRate) : DelayEffectBase(maxDelayInSamples), sampleRate(samplingRate)
{
    writeHeadIndex = 0;
    readHeadIndex = 1;
    currentDelaySamples = maxDelayInSamples;
    targetDelaySamples = maxDelayInSamples;
    setDelayTransitionTime(0.5);
}

SimpleDelay::~SimpleDelay()
{
}
//==============================================================================

void SimpleDelay::setupSimpleDelay(int delayInSamples)
{
    setupDelayEffectBase(delayInSamples);
}

//==============================================================================

double SimpleDelay::process(double inputSample)
{
    // write sample
    delayBuffer[writeHeadIndex] = inputSample;
    writeHeadIndex++;
    writeHeadIndex %= maxDelayBufferSize;
    
    // read sample
    double outSample = getSplineOut(readHeadIndex) + (inputSample * 1);
    if (delayTimeChanged)
    {
        count++;
        const double difference = (currentDelaySamples - targetDelaySamples);
        const double increment = delayIncrement * (difference / fabs(difference));
        currentDelaySamples -= increment;
        readHeadIndex += 1 + increment;
        readHeadIndex = std::fmod(readHeadIndex, maxDelayBufferSize);
        if (count > floor(delayTransitionTimeInSamples))
        {
            currentDelaySamples = targetDelaySamples;
            readHeadIndex = floor(readHeadIndex);
            delayTimeChanged = false;
        }
    }
    else
    {
        readHeadIndex++;
        readHeadIndex = std::fmod(readHeadIndex, maxDelayBufferSize);
    }
    return outSample;
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
double SimpleDelay::getSplineOut(double bufferIndex)
{
    const int n0 = floor(bufferIndex);
    const int n1 = (n0 + 1) % maxDelayBufferSize;
    const int n2 = (n0 + 2) % maxDelayBufferSize;
    const double alpha = bufferIndex - n0;
    
    const double a = delayBuffer[n1];
    const double c = ((3 * (delayBuffer[n2] - delayBuffer[n1])) -  (3 * (delayBuffer[n1] - delayBuffer[n0]))) * 0.25;
    const double b = (delayBuffer[n2] - delayBuffer[n1]) - (2 * c * 0.33333);
    const double d = (-c) * 0.33333;
    return a + (b * alpha) + (c * alpha * alpha) + (d * alpha * alpha * alpha);
}
//==============================================================================
void SimpleDelay::setDelayTransitionTime(double seconds)
{
    delayTransitionTime = seconds;
    delayTransitionTimeInSamples = seconds * sampleRate;
//    std::cout << delayTransitionTime << '\n' << delayTransitionTimeInSamples << '\n' << '\n';
}

void SimpleDelay::setDelayTime(double delayInSamples)
{
    delayTimeChanged = true;
    targetDelaySamples = delayInSamples;
    const double delayTimeDifference = currentDelaySamples - targetDelaySamples;
    delayIncrement = delayTimeDifference / delayTransitionTimeInSamples;
    count = 0;
}
#endif /* SimpleDelay_cpp */
