//
//  DelayEffectBase.cpp
//  AudioEffectsSuite
//
//  Created by admin on 12/12/2017.
//  Copyright © 2017 admin. All rights reserved.
//

#ifndef DelayEffectBase_cpp
#define DelayEffectBase_cpp

#include "DelayEffectBase.hpp"

DelayEffectBase::DelayEffectBase()
{
}

DelayEffectBase::DelayEffectBase(int bufferSizeSamples)
{
    error = setDelayBuffer(bufferSizeSamples);
    delayTimeSamples = bufferSizeSamples;
}

DelayEffectBase::~DelayEffectBase()
{
    delete[] delayBuffer;
}

void DelayEffectBase::setupDelayEffectBase(const int bufferSizeSamples)
{
//    delete [] delayBuffer;
    error = setDelayBuffer(bufferSizeSamples);
    delayTimeSamples = bufferSizeSamples;
}
//==============================================================================
double** DelayEffectBase::setInterpolationTable()
{
	const int order = interpOrder;
	const int res = interpResolution;
	double** interpTable = new double*[order];
	if(!interpTable){return NULL;}
	
	for(int i=0;i<order;i++)
	{
		interpTable[i] = new double[res+1];
		if(!interpTable[i]){return NULL;}
		std::fill(interpTable[i], interpTable[i]+res, 1);
	}
	
	double *polynomial_normaliser = new double [order];
	if(!polynomial_normaliser){return NULL;}
	std::fill(polynomial_normaliser, polynomial_normaliser+order, 1);
	double *alphas = new double [res];
	if(!alphas){return NULL;}
	
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
	return interpTable;
}
//==============================================================================

double DelayEffectBase::getInterpolatedOut(double bufferIndex)
{
	const int order = interpOrder;
	const int orderHalf = order*.5;
	const int res = interpResolution;
	double interpOut = 0;
	int intBufferIndex = floor(bufferIndex);
	int alphaIndex = int(floor((bufferIndex-intBufferIndex)*res));
	
	for(int i = 0; i<order;i++)
	{
		
		int interpIndex = (i+1 - orderHalf) + intBufferIndex;
		
		if(interpIndex < 0 || (interpIndex >= maxDelayBufferSize))
		{
			if(interpIndex < 0){interpIndex = maxDelayBufferSize+interpIndex;}
			else{interpIndex = interpIndex-maxDelayBufferSize;}
		}
		
		interpOut += (interpolationTable[i][alphaIndex]) * (delayBuffer[interpIndex]);
	}
	return interpOut;
}

//==============================================================================
bool DelayEffectBase::setDelayBuffer(int bufferSizeSamples)
{
	maxDelayBufferSize = bufferSizeSamples;
	delayBuffer = new double [maxDelayBufferSize];
	if(!delayBuffer){return false;}
	std::fill(delayBuffer, delayBuffer+maxDelayBufferSize, 0);
	return true;
}
//==============================================================================
void DelayEffectBase::storeSample(double inputSample)
{
	delayBuffer[currentDelayWriteIndex] = inputSample;
}
void DelayEffectBase::incDelayBuffWriteIndex()
{
	currentDelayWriteIndex++;
    currentDelayWriteIndex %= delayTimeSamples;
//	if(currentDelayWriteIndex>=delayTimeSamples){currentDelayWriteIndex=0;}
}

void DelayEffectBase::incDelayBuffReadIndex(double indexInc)
{
	currentDelayReadIndex += indexInc;
	if(currentDelayReadIndex>=double(delayTimeSamples)){currentDelayReadIndex=0;}
	if(currentDelayReadIndex<0){currentDelayReadIndex=0;}
}

void DelayEffectBase::setDelayBuffReadIndex(double index)
{
	currentDelayReadIndex = index;
	if(currentDelayReadIndex>=double(delayTimeSamples)){currentDelayReadIndex=0;}
	if(currentDelayReadIndex<0){currentDelayReadIndex=0;}
}

void DelayEffectBase::delaySample(double inputSample)
{
	storeSample(inputSample);
	incDelayBuffWriteIndex();
}

//==============================================================================
void DelayEffectBase::printInterpTable()
{
	for (int j = 0; j<interpResolution; j++)
	{
		for (int i = 0; i<interpOrder;i++)
		{
			printf("index %d: %.2f \t",i,interpolationTable[i][j]);
		}
		printf("\n");
	}
}

//==============================================================================
// set static variables
//==============================================================================
double** DelayEffectBase::interpolationTable = DelayEffectBase::setInterpolationTable();

#endif /* DelayEffectBase_cpp */
