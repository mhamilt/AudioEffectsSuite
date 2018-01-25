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
#include "../BaseClasses/DelayEffectBase.hpp"

class SimpleDelay;
//==============================================================================
/**
	Simple Delay effect consiting of a single tap delay with Effect Gain and
	feed back controls
	
	Constructor requires internal delay in samples
	
	@see process
 */
class SimpleDelay : public DelayEffectBase
{
public: // Methods
	//==============================================================================
	/** Constructor: DigitalEffect Base Must Be initialised
	 
		@param delayInSamples Set the amount of delay in samples
		
		@see DelayEffectBase constructor
	 */
    SimpleDelay() {};
	SimpleDelay(int delayInSamples, double extSampleRate) : DelayEffectBase(delayInSamples)
    {
        delayTimeSamples = delayInSamples;
    };
	
	/** Destructor. */
	~SimpleDelay(){};
	
	/** setDelayGain: sets the delay gain to a value between 1 and -1
	 
		@param gain		required delay gain. Values beyond 1 and -1
	 are capped to the maximum to avoid idiocy.
	 Negative velus invoke a phase inversion.
	 
		@see setFeedbackGain
	 */
	void setDelayGain(double gain);
	
	/** setDelayGain: sets the feedback gain to a value between 1 and -1
	 
		@param gain		required delay gain. Values beyond 1 and -1
	 are capped to the maximum to avoid idiocy.
	 Negative velus invoke a phase inversion.
	 
		@see setDelayGain
	 */
	void setFeedbackGain(double gain);
	
	//==============================================================================
	/**
	 Apply delay and return input sample along with delay buffer signal

	 @param inputSample <#inputSample description#>
	 @return <#return value description#>
	 */
	double process(double inputSample) override;
	
    /**
     <#Description#>

     @param delayInSamples <#delayInSamples description#>
     */
    void setupSimpleDelay(int delayInSamples);
private: //Methods
	/** capGain: caps gain to a range of 1 and -1;
	*	@param gain address of gain value
	*/
	void capGain(double& gain);

private: //member vairables
	/***/
    double delayGain = .707;
    /***/
    double feedbackGain = 0.;
};

#endif /* SimpleDelay_hpp */
