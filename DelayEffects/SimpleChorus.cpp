//
//  SimpleChorus.cpp
//
//
//  Created by admin on 12/01/2018.
//
//
#ifndef SimpleChorus_hpp
#include "SimpleChorus.hpp"

void SimpleChorus::setSwing(double swingAmount)
{
    swing = swingAmount*sampleRate;
}

void SimpleChorus::setBase(double baseAmount)
{
    base = baseAmount*sampleRate;
}

void SimpleChorus::setupChorus(double extSampleRate)
{
    setupModulationBaseClass(extSampleRate);
    setupDelayEffectBase(double(extSampleRate)*.1);
    //    SimpleLPF(0.0004,4)
    setChebyICoefficients(0.00005, false, 0);
    
    swing = readSpeed*extSampleRate*5;
    base = readSpeed*extSampleRate*20;
//    delete [] waveTable;
    setRandLfo();
}

//==============================================================================
double SimpleChorus::process(double inputSample)
{
    delaySample(inputSample);
    const double waveDelay = getModSignal();
    const double delayAmount = ((int(currentDelayWriteIndex - waveDelay) + delayTimeSamples) % delayTimeSamples)
                                + ((currentDelayWriteIndex - waveDelay) - trunc(currentDelayWriteIndex - waveDelay));
    
    const double out = .0*inputSample + 1.*getInterpolatedOut(delayAmount);
    
    return out;
}
//==============================================================================
void SimpleChorus::setRandLfo()
{
    std::fill(iirBuffer, iirBuffer+filterOrder, .5);
    for (int i = 0; i < sampleRate; i++)
    {
        waveTable[i] = (readNoise()+1)*.5;
//        waveTable[i] = applyFilter((readNoise()+1)*.5);
        if (waveTable[i] < modMin)
            modMin = waveTable[i];
        if (waveTable[i] > modMax)
        {
            modMax = waveTable[i];
        }
    }
    
    modNorm = 1/(modMax-modMin);
    
    // normalises the delay signal
    for (int i = 0; i < sampleRate; i++)
    {
        waveTable[i] -= modMin;
        waveTable[i] *= modNorm;
    }
    
//    setOffSine();
    
    // this code fades out at the end and fades in at the start
    // to avoid any discontinuities int the signal.
//    const int fadeSize = 10000;
//    const double fadeSpeed = 2*M_PI/fadeSize;
//    for (int i = 0; i < fadeSize; i++)
//    {
//        const int fadeIndex = ((sampleRate-fadeSize/2)+i)%sampleRate;
//        waveTable[fadeIndex] *= (1+cos(fadeSpeed*i))*.5;
//    }
}
//==============================================================================
double SimpleChorus::getModSignal()
{
    return (readTable(readSpeed)*swing) + base;
}

//==============================================================================
#endif /* SimpleChorus_hpp */
