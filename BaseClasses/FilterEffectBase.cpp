//
//  FilterEffectBase.cpp
//  AudioEffectsSuite
//
//  Created by admin on 05/01/2018.
//  Copyright © 2018 AudioEffectsSuiteTeam. All rights reserved.
//
#ifndef FilterEffectBase_hpp
#include "FilterEffectBase.hpp"


//==============================================================================
double FilterEffectBase::applyFilter(double sampVal)
{
	firBuffer[bufferIndex] = sampVal;
	// coeff index increases 0 to order
	// buffer index wraps round to avoid reallocating all sample values in memory
	double outSample = 0;
	int i = bufferIndex;
	for(int j = 0; j<filterOrder; j++)
	{
		outSample += firCoefficients[j]*firBuffer[i] + iirCoefficients[j]*iirBuffer[i];
		i++;
		i%=filterOrder;
	}
	iirBuffer[bufferIndex] = outSample;
	incBufferIndex();
	return outSample;
}

//==============================================================================

void FilterEffectBase::incBufferIndex()
{
	bufferIndex++;
	bufferIndex%=filterOrder;
}

//==============================================================================
bool FilterEffectBase::setChebyICoefficients(double cutFreq, bool shelfType, double ripple,int poles)
{
	clearMemory();
	filterOrder = poles+1;
	firCoefficients = new double[filterOrder];
	iirCoefficients = new double[filterOrder];
	double* firTemp = new double[filterOrder];
	double* iirTemp = new double[filterOrder];
	
	firCoefficients[2] = 1;
	iirCoefficients[2] = 1;
	
	double Es,Vx,Kx;
	if(ripple!=0)
	{
		Es = sqrt( pow(1/(1-ripple),2) - 1);
		Vx = (1/poles)*log(1/Es + sqrt(1/(pow(Es,2))+1));
		Kx = (1/poles)*log(1/Es + sqrt(1/(pow(Es,2))-1));
		Kx = cosh(Kx);
	}
	else
	{
		Vx = 1;
		Kx = 1;
	}
	
	const double T = 2*tan(.5);
	const double W = 2*pi*cutFreq;
	
	double K;
	
	if(shelfType==0) //// if low pass
	{
		K = sin(.5 - W/2)/sin(.5 + W/2);
	}
	else //// if high pass
	{
		
		K = -cos(.5 + W/2)/cos(W/2 - .5);
	}
	
	////// main algorithm
	for (int i = 0; i<filterOrder; i++)
	{
		////// Sub routine
		const double alpha = pi/(2*poles) + (i-1-1)*(pi/poles);
		
		double Rp, Ip;
		if(ripple!=0)
		{
			Rp = -cos(alpha)*sinh(Vx)/Kx;
			Ip = sin(alpha)*cosh(Vx)/Kx;
		}
		else
		{
			Rp = -cos(alpha);
			Ip = sin(alpha);
		}
		
		const double M = pow(Rp,2) + pow(Ip,2);
		const double D = 4 - 4*Rp*T + M*T;
		
		const double X0 = (pow(T,2))/D;
		const double X1 = (2*pow(T,2))/D;
		const double X2 = X0;
		
		const double Y1 = (8-(2*M*pow(T,2)))/D;
		const double Y2 = (-4 - 4*Rp*T - M*T)/D;
		
		const double D1 = 1/(1 + Y1*K - Y2*pow(K,2)); // renamed and inverted from original algorithm
		
		const double A0 =  (X0 - X1*K + X2*pow(K,2))*D1;
		double A1 =  (-2*X0*K + X1 + X1*pow(K,2) - 2*X2*K)*D1;
		const double A2 =  (X0*pow(K,2) - X1*K + X2)*D1;
		
		double B1 = (2*K + Y1 +Y1*pow(K,2) - 2*Y2*K)*D1;
		const double B2 = (-(pow(K,2)) - Y1*K + Y2)*D1;
		
		if(shelfType==1){A1 = -A1; B1 = -B1;}
		
		for (int j = 0; j<filterOrder; j++)
		{
			firTemp[i] = firCoefficients[i];
			iirTemp[i] = iirCoefficients[i];
		}
		
		for (int j = 0; j<filterOrder; j++)
		{
			firCoefficients[j] = A0*firTemp[j] + A1*firTemp[j-1] + A2*firTemp[j-2];
			iirCoefficients[j] =    iirTemp[j] - B1*iirTemp[j-1] - B2*iirTemp[j-2];
		}
		
	} //// end for i
	
	iirCoefficients[2] = 0;
	for (int j = 0; j<filterOrder; j++)
	{
		firCoefficients[j] = firCoefficients[j+2];
		iirCoefficients[j] = -iirCoefficients[j+2];
	}
	
	//////// Normalising
	double SA = 0; double SB = 0;
	if (shelfType==0)
	{
		for (int j = 0; j<filterOrder; j++)
		{
			SA += firCoefficients[j];
			SB += iirCoefficients[j];
		}
	}
	else
	{
		for (int j = 0; j<filterOrder; j++)
		{
			SA += firCoefficients[j]*pow(-1,j);
			SB += iirCoefficients[j]*pow(-1,j);
		}
	}
	
	const double gain = SA/(1-SB);
	
	for (int j = 0; j<filterOrder; j++)
	{
		firCoefficients[j] /= gain;
	}
	
	delete[] firTemp;
	delete[] iirTemp;
	return true;
}

//==============================================================================
bool FilterEffectBase::setSimpleLpf(int order)
{
	filterOrder = order;
	clearMemory();
	allocateBufferMemory();
	firCoefficients = new double[filterOrder];
	iirCoefficients = new double[filterOrder];
	std::fill(iirCoefficients, iirCoefficients+filterOrder, 0);
	//	firCoefficients[0] = 1*0.0625; firCoefficients[1] = 4*0.0625; firCoefficients[2] = 6*0.0625; firCoefficients[3] = 4*0.0625; firCoefficients[4] = 1*0.0625;
	//	iirCoefficients[0] = 0; iirCoefficients[1] = 0; iirCoefficients[2] = 0; iirCoefficients[3] = 0; iirCoefficients[4] = 0;
	
	int coef = 1;
	double gain = 0;
	for(int j = 0; j<filterOrder; j++)
	{
		if(j==0)
		{coef = 1;}
		else
		{coef = coef*(filterOrder-j)/j;}
		
		firCoefficients[j] = (double)coef;
		gain += firCoefficients[j];
	}
	
	for(int j = 0; j<=filterOrder; j++)
	{
		firCoefficients[j] /= gain;
	}
	
	return true;
}

//==============================================================================

void FilterEffectBase::clearMemory()
{
	
	if(firCoefficients)
	{
		delete[] firCoefficients;
	}
	
	if(iirCoefficients)
	{
		delete[] iirCoefficients;
	}
}

void FilterEffectBase::allocateBufferMemory()
{
	if(firBuffer)
	{
		delete[] firBuffer;
	}
	
	if(iirBuffer)
	{
		delete[] iirBuffer;
	}
	firBuffer = new double[filterOrder];
	iirBuffer = new double[filterOrder];
	std::fill(firBuffer, firBuffer+filterOrder, 0);
	std::fill(iirBuffer, iirBuffer+filterOrder, 0);
}
//==============================================================================

void FilterEffectBase::printBuffers()
{
	printf("FIRc\tIIRc\tFIRb\tIIRb\n");
 for (int i = 0; i<filterOrder;i++)
 {
	 printf("%.2f\t%.2f\t%.2f\t%.2f\n",firCoefficients[i],iirCoefficients[i],firBuffer[i],iirBuffer[i]);
 }
}

#endif /* FilterEffectBase_hpp */
