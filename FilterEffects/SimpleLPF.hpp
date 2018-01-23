//
//  SimpleLPF.hpp
//  AudioEffectsSuite
//
//  Created by admin on 08/01/2018.
//  Copyright © 2018 AudioEffectsSuiteTeam. All rights reserved.
//

#ifndef SimpleLPF_hpp
#define SimpleLPF_hpp

#include <iostream>
#include <cmath>
#include "../BaseClasses/FilterEffectBase.hpp"

/** Class for applying a FIR lowpass filter
	
	Intialised with the order of filter and the normalised cutoff frequency
	
	object declared with order of low pass filter
	*/
class SimpleLPF : public FilterEffectBase
{
public:
/** Constructor: Intialised with the order of FIR filter
	
	minimum cutoff frequency is 1/sampleRate
	
	@param cutoff	normalised cutoff frequency [0, 1);
    @param order	filter order
	*/
	SimpleLPF(double cutoff, int order)
	{
		changeChebyICoefficients(cutoff, false, .1, order);
	};
public:
	/** destructor*/
	~SimpleLPF(){};
};
#endif /* SimpleLPF_hpp */
