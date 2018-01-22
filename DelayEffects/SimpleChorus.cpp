//
//  SimpleChorus.cpp
//
//
//  Created by admin on 12/01/2018.
//
//

#include "SimpleChorus.hpp"

//==============================================================================
double SimpleChorus::process(double inputSample)
{
    delaySample(inputSample);
    const double waveDelay = getModSignal();
    const double delayAmount = waveDelay + currentDelayWriteIndex;
    
    //    printf("Delay %.2f\n",waveDelay);
    const double out = .5*inputSample + getInterpolatedOut(delayAmount);
    
    return out;
}
//==============================================================================
void SimpleChorus::setRandLfo()
{
    std::fill(iirBuffer, iirBuffer+filterOrder, .5);
    for (int i = 0; i < sampleRate; i++)
    {
        waveTable[i] = applyFilter((readNoise()+1)*.5);
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
    
    // this ocde fades out at the end and fades in at the start
    // to avoid any discontinuities int the signal.
    const int fadeSize = 10000;
    const double fadeSpeed = 2*M_PI/fadeSize;
    for (int i = 0; i < fadeSize; i++)
    {
        const int fadeIndex = ((sampleRate-fadeSize/2)+i)%sampleRate;
        waveTable[fadeIndex] *= (1+cos(fadeSpeed*i))*.5;
    }
    
    std::cout << modMin << '\t';
    std::cout << modMax << '\t';
    std::cout << modNorm << '\n';
    
    std::cout << base << '\t';
    std::cout << swing << '\n';
    
}
//==============================================================================
double SimpleChorus::getModSignal()
{
    return (readTable(.15)*swing) + base;
}



//==============================================================================
