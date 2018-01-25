//
//  ModulationBaseClass.cpp
//  AudioEffectsSuite
//
//  Created by admin on 16/01/2018.
//  Copyright © 2018 AudioEffectsSuiteTeam. All rights reserved.
//
#ifndef ModulationBaseClass_cpp
#define ModulationBaseClass_cpp

#include "ModulationBaseClass.hpp"

ModulationBaseClass::ModulationBaseClass()
{
    srand (static_cast <unsigned> (time(0)));
}

ModulationBaseClass::ModulationBaseClass(double extSampRate)
{
    sampleRate = extSampRate;
    timeStep = 1./extSampRate;
    allocateMemory();
    //                setInterpTable();
    srand (static_cast <unsigned> (time(0)));
}

ModulationBaseClass::~ModulationBaseClass()
{
}

void ModulationBaseClass::setupModulationBaseClass(double extSampRate)
{
    if (waveTable != nullptr)
    {
//        delete [] waveTable;
    }
    sampleRate = extSampRate;
    timeStep = 1./extSampRate;
    allocateMemory();
}

//==============================================================================
double ModulationBaseClass::readTable(double freq)
{
    if (freq > 0)
    {
        //    const double out = getInterpOut(tableIndex);
        const double out = getSplineOut(tableIndex, int(freq));
        tableIndex += freq;
        if (tableIndex-sampleRate > 0)
            tableIndex -= sampleRate;
        
        return out;
    }
    else
    {
        return 0.;
    }
}
//==============================================================================
bool ModulationBaseClass::allocateMemory()
{
    waveTable = new double[sampleRate];
    if(!waveTable){
        return false;
    }
    std::fill(waveTable, waveTable+sampleRate, 0);
    return true;
}

//==============================================================================

bool ModulationBaseClass::setInterpTable()
{
    double *polynomial_normaliser = new double [order];
    if(!polynomial_normaliser){return false;}
    std::fill(polynomial_normaliser, polynomial_normaliser+order, 1);
    double *alphas = new double [res];
    if(!alphas){return false;}
    
    for(int i = 0; i<res;i++)
    {
        alphas[i] = (i/float(res)) - 0.5;
    }
    
    double *anchors = new double [order];
    
    if ((order % 2)==0)
    {
        for(int i = 0; i<order;i++)
        {
            anchors[i] = -(double(order) - 1)*0.5 + double(i);
            std::fill(interpTable[i], interpTable[i]+res, 1);
        }
    }
    
    else
    {
        for(int i = 0; i<order;i++)
        {
            anchors[i] = (-(double(order))*0.5) + double(i);
        }
    }
    
    for (int q = 0; q<res;q++) // loop for every value of alpha
    {
        for (int j = 0; j<order;j++) // loop for sub polynomial
        {
            for (int m = 0; m < order; m++) //loop for each point in subpoly
            {
                if (m != j)
                {
                    if (q == 0)
                    {
                        polynomial_normaliser[j] = polynomial_normaliser[j]*(anchors[j]-anchors[m]);
                    }
                    interpTable[j][q] *= (alphas[q]-anchors[m]);
                }
            }
            interpTable[j][q] /= polynomial_normaliser[j];
        }
    }
    
    
    delete[] polynomial_normaliser;
    delete[] alphas;
    delete[] anchors;
    return true;
}

double ModulationBaseClass::getInterpOut(double bufferIndex)
{
    const int order = 4;
    const int orderHalf = order*.5;
    const int res = 100;
    double interpOut = 0;
    int intBufferIndex = floor(bufferIndex);
    int alphaIndex = int(floor((bufferIndex-intBufferIndex)*res));
    
    for(int i = 0; i<order;i++)
    {
        int interpIndex = (((i+1 - orderHalf) + intBufferIndex)+sampleRate)%sampleRate;
        interpOut += (interpTable[i][alphaIndex]) * (waveTable[interpIndex]);
    }
    return interpOut;
}

void ModulationBaseClass::printInterpTable()
{
    for (int j = 0; j < res; j++){
        for (int i = 0; i < order; i++) {
            std::cout << interpTable[i][j] << '\t';
        }
        std::cout << '\n';
    }
}
//==============================================================================
double ModulationBaseClass::getSplineOut(double bufferIndex, int freq)
{
    
    if (freq<1) {
        freq = 1;
    }
    const int n0 = floor(bufferIndex);
    const int n1 = (n0+freq)%sampleRate;
    const int n2 = (n0+(2*freq))%sampleRate;
    const double alpha = bufferIndex - n0;
    const double a = waveTable[n1];
    const double c = ((3*(waveTable[n2] - waveTable[n1])) -  (3*(waveTable[n1] - waveTable[n0]))) *.25;
    const double b = (waveTable[n2] - waveTable[n1]) - ((2*c))/3;
    const double d = (-c)/3;
    return a + (b*alpha) + (c*alpha*alpha) + (d*alpha*alpha*alpha);
}

//==============================================================================

void ModulationBaseClass::setSine()
{
    const double radPerSec = 2*3.1415926536*timeStep;
    for(int i = 0; i < sampleRate; i++)
        waveTable[i] = sin(i*radPerSec);
}
//==============================================================================
void ModulationBaseClass::setOffSine()
{
    const double radPerSec = 2*3.1415926536*timeStep;
    for(int i = 0; i < sampleRate; i++)
        waveTable[i] = (sin(i*radPerSec) + 1)*.5;
}

//==============================================================================

void ModulationBaseClass::setSawtooth()
{
    std::fill(waveTable, waveTable+sampleRate, 0);
    const double radPerSec = 2*3.1415926536*timeStep;
    for(int i = 0; i < sampleRate; i++)
    {
        for (int j = 1; j <11; j+=1)
            waveTable[i] += pow(-1,j)*sin(j*radPerSec*i)/double(j);
    }
}

//==============================================================================
void ModulationBaseClass::setSquare()
{
    std::fill(waveTable, waveTable+sampleRate, 0);
    const double radPerSec = 2*3.1415926536*timeStep;
    for(int i = 0; i < sampleRate; i++)
    {
        for (int j = 0; j <35; j+=1)
            waveTable[i] += (sin((2*j + 1)*i*radPerSec))/(2*j +1);
    }
}

//==============================================================================
void ModulationBaseClass::setTriangle()
{
    std::fill(waveTable, waveTable+sampleRate, 0);
    const double radPerSec = 2*3.1415926536*timeStep;
    for(int i = 0; i < sampleRate; i++)
    {
        for (int j = 0; j <35; j+=1)
            waveTable[i] += pow(-1.,j)*(sin((2.*double(j) + 1)*i*radPerSec))/(2.*double(j) +1);
    }
}

//==============================================================================

void ModulationBaseClass::setDC()
{
    for (int i = 0; i < sampleRate; i++)
        waveTable[i] = 1.0;
}

//==============================================================================

void ModulationBaseClass::setRamp()
{
    for (int i = 0; i < sampleRate; i++)
        waveTable[i] = i / double(sampleRate);
}

//==============================================================================

void ModulationBaseClass::clipWave(double amp)
{
    if (amp < .01)
    {
        amp = .01;
    }
    
    for(int i = 0; i < sampleRate; i++)
        waveTable[i] = tanh(amp*waveTable[i])/tanh(amp);
}

double ModulationBaseClass::readNoise()
{
    const double lo = -1.;
    const double hi =  1.;
    return lo + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(hi-lo)));
}

#endif
