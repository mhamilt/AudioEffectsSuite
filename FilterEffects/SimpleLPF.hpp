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

/** Class for applying a simple 5th order FIR lowpass filter*/
class SimpleLPF : public FilterEffectBase
{
public:
	SimpleLPF()
	{
		setSimpleLpf();
		firBuffer = new double[filterOrder];
		iirBuffer = new double[filterOrder];
		std::fill(firBuffer, firBuffer+filterOrder, 0);
		std::fill(iirBuffer, iirBuffer+filterOrder, 0);
	};
public:
	~SimpleLPF(){};
};
#endif /* SimpleLPF_hpp */
