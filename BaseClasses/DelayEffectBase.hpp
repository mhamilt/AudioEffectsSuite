//
//  DelayEffectBase.hpp
//  AudioEffectsSuite
//
//  Created by admin on 12/12/2017.
//  Copyright © 2017 admin. All rights reserved.
//

#ifndef DelayEffectBase_hpp
#define DelayEffectBase_hpp

#include <iostream>
#include <cmath>

//==============================================================================
/**
	A Base class for delay based digital effects. Provides the basic methods
	that are shared amongst Flanger, Delay, Chorus and Phaser
    
    @version 0.1
 
 @see DelayEffectBase
 */
class DelayEffectBase
{
public: // Methods
	//==============================================================================
	/** Constructor. */
    DelayEffectBase();
    DelayEffectBase(int bufferSizeSamples);

	/** Destructor. */
    ~DelayEffectBase();
	
	//==============================================================================
	/** Main process block for applying audio effect
		@param inputSample The input audio sample for the effect to be applied to
		
		@returns an audio sample as a double with effect applied
	 */
	virtual double process(double inputSample) = 0;

    /**
     <#Description#>

     @param bufferSizeSamples <#bufferSizeSamples description#>
     */
    void setupDelayEffectBase(const int bufferSizeSamples);
private:	//Methods
	//==============================================================================
	/** Sets the internal lagrange interpolation table. Ideally it should be shared
	 amongst all
	 
	 @returns    false and an error description if there's a problem,
	 or sets the interpolation lookup table and returns true
	 */
	static double** setInterpolationTable();
	
	/***/
	void printInterpTable();
protected:
	//==============================================================================
	/**this should be informed by what kind off effect will be implemented*/
	//	virtual bool setMaxBufferSize() = 0;
	
	/** Allocates memory for delay buffer and initialises all elements to 0
	 
	 @param bufferSizeSamples		the size of delay buffer to be used
	 
	 @returns   false and error description if there's a problem,
	 or sets the internal delay buffer and returns true
	 */
	bool setDelayBuffer(int bufferSizeSamples);
	
	/** store input sample into the delay buffer
	 
	 @param inputSample sample to be stored for delay (double)
	 */
	void storeSample(double inputSample);
	
	/** Increments the currentDelayWriteBufferIndex by 1
	 */
	void incDelayBuffWriteIndex();
	
	/** Increments the currentDelayBufferReadIndex by indexInc
		@param indexInc The amount to increment the delay buffer index
	 */
	void incDelayBuffReadIndex(double indexInc);
	
	/** sets the currentDelayBufferReadIndex by indexInc (Currently no wrapping)
		@param index the read index index required 
	 */
	void setDelayBuffReadIndex(double index);
	
	/** store input sample into the delay buffer and increment currentDelayWriteIndex
		for tracking when to loop back to start of buffer
	 
	 @param inputSample sample to be stored for delay (double)
	 */
	void delaySample(double inputSample);
	/** get the value of the requested buffer index by interpolating other points
		@param bufferIndex	The required buffer index
		
		@returns interpolated output
	 */
	double getInterpolatedOut(double bufferIndex);
	
protected:// member variables
	//==============================================================================
	/** Table of interpolation values as a 2D array indexed by
		interpolationTable[pointIndex][alphaIndex]
	 */
	static double** interpolationTable;

	/** buffer to stored audio buffer for delay effects*/
	double* delayBuffer = 0;
	
	/** Maximum number of samples that can be stored in delayBuffer*/
	int maxDelayBufferSize = 441000;
	
	/** the delay time of signal in samples*/
	int delayTimeSamples = 44100;
	
	/***/
	int currentDelayWriteIndex = 0;
	
	/***/
	double currentDelayReadIndex = 0;
	
	/***/
	static const int interpOrder = 4;
	
	/***/
	static const int interpResolution = 1000;
	
	/** internal class error boolean*/
	bool error;
};



#endif /* DelayEffectBase_hpp */
