//
//  DigitalEffectBase.hpp
//  AudioEffectsSuite
//
//  Created by admin on 12/12/2017.
//  Copyright © 2017 admin. All rights reserved.
//

#ifndef DigitalEffectBase_hpp
#define DigitalEffectBase_hpp

#include <iostream>
#include <cmath>


class DigitalEffectBase;
//==============================================================================
/**
	A Base class for delay based digital effects. Provides the basic methods
	that are shared amongst Flanger, Delay, Chorus and Phaser
 
 @see DigitalEffectBase
 */
class DigitalEffectBase
{
public: // Methods
	//==============================================================================
	/** Constructor. */
	DigitalEffectBase(int bufferSizeSamples)
	{
		setInterpolationTable(interpOrder,interpResolution);
		error = setDelayBuffer(bufferSizeSamples);
	};
	
public:
	/** Destructor. */
	~DigitalEffectBase(){delete[] delayBuffer;};
	
	//==============================================================================
	//	/** Returns the number of output channels
	//	 */
	//	virtual int getOutputNumber() = 0;
	//
	//	/** Returns the number of input channels
	//	 */
	//	virtual int getInputNumber() = 0;
	//
	//	//==============================================================================
	//	/** Returns the delay buffer size
	//	 */
	//	virtual int getDelayBufferSizeSamples() = 0;
	//
	//	/** Returns the current audio buffer size
	//	 */
	//	virtual int getAudioBufferSizeSamples() = 0;
	//
	//	/** Returns the current sample rate
	//	 */
	//	virtual double getCurrentSampleRate() = 0;
	//
	//	/** Returns the current bit-depth
	//	 */
	//	virtual int getCurrentBitDepth() = 0;
	//
	
	//==============================================================================
	/** Main process block for applying audio effect
		@param inputSample The input audio sample for the effect to be applied to
		
		@returns an audio sample as a double with effect applied
	 */
	virtual double process(double inputSample) = 0;
	
	
	
private:	//Methods
	//==============================================================================
	/** Sets the internal lagrange interpolation table. Ideally it should be shared
	 amongst all
	 
	 @param interpolationOrder	order of interpolation to be used
	 @param alphaResolution      the number of discrete alpha values that are represented
	 
	 @returns    false and an error description if there's a problem,
	 or sets the interpolation lookup table and returns true
	 */
	bool setInterpolationTable(const int interpolationOrder,const int alphaResolution);
	
	/***/
	void printInterpTable();
protected:
	//==============================================================================
	/**this should be informed by what kind off effect will be implemented*/
	//	virtual bool setMaxBufferSize() = 0;
	
	/** Allocates memory for delay buffer and initialises all elements to 0
	 
	 @param inputChannelNum         number of input channels to effect
	 @param outputChannelNum        number of output channels from effect
	 @param sampleRate				the sample rate to try to use
	 @param bufferSizeSamples		the size of delay buffer to be used
	 
	 @returns   false and error description if there's a problem,
	 or sets the internal delay buffer and returns true
	 */
	bool setDelayBuffer(int bufferSizeSamples);
	
	/** store input sample into the delay buffer and increment currentDelayIndex
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
	double** interpolationTable = 0;
	
	double testArray [4];
	
	/** buffer to stored audio buffer for delay effects*/
	double* delayBuffer = 0;
	
	/** Maximum number of samples that can be stored in delayBuffer*/
	int maxDelayBufferSize = 410000;
	
	/** the delay time of signal in samples*/
	int delayTimeSamples = 44100;
	
	/***/
	int currentDelayIndex = 0;
	
	/***/
	static const int interpOrder = 4;
	
	/***/
	static const int interpResolution = 100;
	
	bool error;
};



#endif /* DigitalEffectBase_hpp */
