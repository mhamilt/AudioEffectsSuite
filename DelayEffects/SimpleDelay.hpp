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
    SimpleDelay(int maxDelayInSamples, int samplingRate);
	
	/** Destructor. */
	~SimpleDelay();
	
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
    /**
     <#Description#>

     @param delayInSample <#delayInSample description#>
     */
    void setDelayTime(double delayInSamples);
    /**
     <#Description#>

     @param seconds <#seconds description#>
     */
    void setDelayTransitionTime(double seconds);
private: //Methods
	/** capGain: caps gain to a range of 1 and -1;
	*	@param gain address of gain value
	*/
	void capGain(double& gain);
    //==============================================================================
    /**
     get a cubic spline interpolated out from the wave table
     
     Derived from code by Alec Wright at repository:
     https://github.com/Alec-Wright/Chorus
     
     @authors Matthew Hamilton, Alec Wright
     @param bufferIndex the required buffer index
     @return returns interpolated value as double
     */
    double getSplineOut(double bufferIndex);
    //==============================================================================
private: //member vairables
	/***/
    double delayGain = .707;
    /***/
    double feedbackGain = 0.;
    /***/
    double readHeadIndex;
    /***/
    unsigned int writeHeadIndex;
    /***/
    double currentDelaySamples;
    /***/
    double targetDelaySamples;
    /** increment when transition from current to target delay per sample set by delayTransitionTime*/
    double delayIncrement;
    /** inverse of delay increment: for working out whole number cyles to reach target delay*/
    double invDelayIncrement;
    /** time in seconds to transition from one delay to another*/
    double delayTransitionTime;
    double delayTransitionTimeInSamples;
    /***/
    const int sampleRate;
    int count = 0;
    bool delayTimeChanged = false;
};

#endif /* SimpleDelay_hpp */
