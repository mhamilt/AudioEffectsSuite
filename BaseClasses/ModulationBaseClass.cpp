//
//  ModulationBaseClass.cpp
//  AudioEffectsSuite
//
//  Created by admin on 16/01/2018.
//  Copyright © 2018 AudioEffectsSuiteTeam. All rights reserved.
//

#include "ModulationBaseClass.hpp"
//==============================================================================
double ModulationBaseClass::readTable(double freq)
{
    const double out = getInterpolatedOut(tableIndex);
    //    const double out  = waveTable[int(tableIndex)%sampleRate];
    //    const double out  = waveTable[int(tableIndex)];
    tableIndex += freq;
    if (tableIndex-sampleRate > 0) {
        tableIndex -= sampleRate;
    }
    
    return out;
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

bool ModulationBaseClass::setInterpoationTable()
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

double ModulationBaseClass::getInterpolatedOut(double bufferIndex)
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
        for (int j = 1; j <35; j+=1)
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
