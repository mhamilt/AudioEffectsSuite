//
//  DigitalEffectBase.cpp
//  AudioEffectsSuite
//
//  Created by admin on 12/12/2017.
//  Copyright © 2017 admin. All rights reserved.
//
#ifndef DigitalEffectBase_cpp
#define DigitalEffectBase_cpp

#include "DigitalEffectBase.hpp"

//==============================================================================
bool DigitalEffectBase::setInterpolationTable(const int interpolationOrder,const int alphaResolution)
{
	const int order = interpolationOrder;
	const int res = alphaResolution;
	interpolationTable = new double*[order];
	if(!interpolationTable){return false;}
	
	for(int i=0;i<order;i++)
	{
		interpolationTable[i] = new double[res+1];
		if(!interpolationTable[i]){return false;}
		std::fill(interpolationTable[i], interpolationTable[i]+res, 1);
	}
	
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
					interpolationTable[j][q] *= (alphas[q]-anchors[m]);

				}
			}
			interpolationTable[j][q] /= polynomial_normaliser[j];
		}
	}
//	delete[] polynomial_normaliser;
//	delete[] alphas;
//	delete[] anchors;
	return true;
}
//==============================================================================

double DigitalEffectBase::getInterpolatedOut(double bufferIndex)
{
	const int order = 4;
	const int orderHalf = order*.5;
	const int res = 100;
	double interpOut = 0;
	int intBufferIndex = floor(bufferIndex);
	int alphaIndex = int(floor((bufferIndex-intBufferIndex)*res));
	
	for(int i = 0; i<order;i++)
	{
		
		int interpIndex = (i+1 - orderHalf) + intBufferIndex;
		
		if(interpIndex < 0 || interpIndex > maxDelayBufferSize)
		{
			if(interpIndex < 0){interpIndex = maxDelayBufferSize+interpIndex;}
			else{interpIndex = interpIndex-maxDelayBufferSize;}
		}
		
		interpOut += (interpolationTable[i][alphaIndex]) * (delayBuffer[interpIndex]);
	}
	return interpOut;
}

//==============================================================================
//bool DigitalEffectBase::setMaxBufferSize()
//{
//	maxDelayBufferSize = 44100;
//	return true;
//}
//==============================================================================
bool DigitalEffectBase::setDelayBuffer(int bufferSizeSamples)
{
	delayBuffer = new double [maxDelayBufferSize];
	if(!delayBuffer){return false;}
	std::fill(delayBuffer, delayBuffer+maxDelayBufferSize, 0);
	return true;
}
//==============================================================================
void DigitalEffectBase::delaySample(double inputSample)
{
	delayBuffer[currentDelayIndex] = inputSample;
	currentDelayIndex++;
	if(currentDelayIndex>=delayTimeSamples){currentDelayIndex=0;}
}

//==============================================================================
void DigitalEffectBase::printInterpTable()
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
#endif /* DigitalEffectBase_cpp */
