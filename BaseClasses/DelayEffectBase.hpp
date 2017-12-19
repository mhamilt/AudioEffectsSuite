//
//  DelayEffectBase.hpp
//  AudioEffectsSuite
//
//  Created by admin on 12/12/2017.
//  Copyright © 2017 admin. All rights reserved.
//

#ifndef DelayEffectBase_hpp
#define DelayEffectBase_hpp

#include <stdio.h>

class DelayEffectBase;
//==============================================================================
/**
	A Base class for delay based digital effects. Provides the basic methods
	that are shared amongst Flanger, Delay, Chorus and Phaser
 
 @see DelayEffectBase
 */
class DelayEffectBase
{
public:
	/** Destructor. */
	virtual ~DelayEffectBase();
	
	//==============================================================================
	/** Returns the names of all the available output channels on this device.
	 To find out which of these are currently in use, call getActiveOutputChannels().
	 */
	virtual int getOutputNumber() = 0;
	
	/** Returns the names of all the available input channels on this device.
	 To find out which of these are currently in use, call getActiveInputChannels().
	 */
	virtual int getInputNumber() = 0;
	
	//==============================================================================
	/** Returns the delay buffer size
	 */
	virtual int getCurrentBufferSizeSamples() = 0;
	
	/** Returns the sample rate
	 */
	virtual double getCurrentSampleRate() = 0;
	
	/** Returns the current physical bit-depth.
	 */
	virtual int getCurrentBitDepth() = 0;
	
	//==============================================================================
	
	/** Tries to open the device ready to play.
	 
	 @param inputChannelNum         number of input channels to effect
	 @param outputChannelNum        number of output channels from effect
	 @param sampleRate				the sample rate to try to use
	 @param bufferSizeSamples		the size of delay buffer to be used
	 
	 @returns   an error description if there's a problem, or sets the internal delay buffer
	 
	 */
	
	virtual void setDelayBuffer(int inputChannelNum,int outputChannelNum,int sampleRate,int bufferSizeSamples) = 0;
	//==============================================================================
	
		//setMaxBufferSize, this should be informed by what kind off effect will be implemented
	
	//==============================================================================
	/** Sets the internal lagrange interpolation table. Ideally it should be shared
	 amongst all
	 
	 @param interpolationOrder	order of interpolation to be used
	 @param alphaResolution      the number of discrete alpha values that are represented
	 
		@returns    an error description if there's a problem, or sets the interpolation lookup table
	 
	 
	 */
	virtual void setInterpolationTable(int interpolationOrder, int alphaResolution) = 0;
	
private:	//Methods
	
protected: // member variable
	//==============================================================================
	/** Constructor. */
	DelayEffectBase();
	
	//==============================================================================
	/** Table of interpolation values as a 2D array indexed by
		interpolationTable[pointIndex][alphaIndex]
	 */
	double** interpolationTable = 0;
};



#endif /* DelayEffectBase_hpp */
