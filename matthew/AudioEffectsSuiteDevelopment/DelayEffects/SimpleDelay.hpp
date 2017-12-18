//
//  SimpleDelay.hpp
//  AudioEffectsSuiteDevelopment
//
//  Created by admin on 18/12/2017.
//  Copyright © 2017 AudioEffectsSuiteTeam. All rights reserved.
//

#ifndef SimpleDelay_hpp
#define SimpleDelay_hpp

#include <iostream>
#include <cmath>
#include "../../../BaseClasses/DigitalEffectBase.cpp"

class SimpleDelay;
//==============================================================================
/**
	Delay Effect Class for applying an delay effect onto an audio stream,
	sample by sample
	
	Constructor requires internal delay in samples
	
	@see process
 */
class SimpleDelay : public DigitalEffectBase
{
public: // Methods
	//==============================================================================
	/** Constructor: DigitalEffect Base Must Be initialised
	
		@param delayInSamples Set the amount of delay in samples
		
		@see DigitalEffectBase constructor
	*/
	SimpleDelay(int delayInSamples) : DigitalEffectBase(44100){delayTimeSamples=delayInSamples;};
	
	/** Destructor. */
	~SimpleDelay(){};
	
	//==============================================================================
	/**Delay sample by 1 seconds and*/
	double process(double inputSample) override;
};

#endif /* SimpleDelay_hpp */
