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
#include "../BaseClasses/FilterEffectBase.cpp"

/** Class for applying a FIR lowpass filter
	
	Intialised with the order of FIR filter
	*/
class SimpleLPF : public FilterEffectBase
{
public:
/** Constructor: Intialised with the order of FIR filter
	@param order	filter order
	*/
	SimpleLPF(int order)
	{
//		setSimpleLpf(order);		
		setChebyICoefficients(.01, false, .1, order);
	};
public:
	/** destructor*/
	~SimpleLPF(){};
};
#endif /* SimpleLPF_hpp */
