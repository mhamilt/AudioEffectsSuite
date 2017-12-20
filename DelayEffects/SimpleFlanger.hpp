//
//  SimpleFlanger.hpp
//  AudioEffectsSuite
//
//  Created by admin on 20/12/2017.
//  Copyright © 2017 AudioEffectsSuiteTeam. All rights reserved.
//

#ifndef SimpleFlanger_hpp
#define SimpleFlanger_hpp

#include <iostream>
#include <cmath>
#include "../BaseClasses/DelayEffectBase.cpp"

class SimpleFlanger;
//==============================================================================
/**
	Simple Flanger Effect Consistig of a single voice flanger
	
	Constructor requires internal delay in samples
	
	@see process
 */
class SimpleFlanger : public DelayEffectBase
{
public: // Methods
	//==============================================================================
	/** Constructor: DigitalEffect Base Must Be initialised
		
		@see DelayEffectBase constructor
	 */
	SimpleFlanger() : DelayEffectBase(44100){};
	
	/** Destructor. */
	~SimpleFlanger(){};
	
	/** setEffectGain: sets the effect gain to a value between 1 and -1
	 
		@param gain		required delay gain. Values beyond 1 and -1
	 are capped to the maximum to avoid idiocy.
	 Negative velus invoke a phase inversion.
	 */
	virtual void setEffectGain(double gain);
	
	//==============================================================================
	/**Apply the DSP effect*/
	double process(double inputSample) override;
	
private: //Methods
	/** capGain: caps gain to a range of 1 and -1;
	*	@param gain address of gain value
	*/
	virtual void capGain(double& gain);

private: //member vairables
	/***/
};


#endif /* SimpleFlanger_hpp */
