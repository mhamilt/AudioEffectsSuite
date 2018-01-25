//
//  FilterEffectBase.hpp
//  AudioEffectsSuite
//
//  Created by admin on 05/01/2018.
//  Copyright © 2018 AudioEffectsSuiteTeam. All rights reserved.
//

#ifndef FilterEffectBase_hpp
#define FilterEffectBase_hpp

#include <iostream>
#include <cmath>

//==============================================================================
/**
	A Base class for filter based effects including methods for simple
	high, low and band pass filtering
 
 @see FilterEffectBase
 */
class FilterEffectBase
{
public:		// methods
	//==========================================================================
	/** Constructor. */
    FilterEffectBase();
	/** Destructor. */
    ~FilterEffectBase();
	//==========================================================================
	/** with the current filter coefficients this method filters a
	 sample then stores it the sample Buffer and increments the index
	 
	 @param sampVal is the sample to be processed
	 
	 @returns filtered audio sample
	 */
	double applyFilter(double sampVal);
	/** detect the envelop of an incoming signal
	 
	 @param sample		the incoming signal sample value
	 
	 @returns returns envelope dection signal sample value
	 */
	 double envelope(double sample);
	//==========================================================================
	/***/
	void printBuffers();
	/***/
	void printCoefs();
	//==========================================================================
	/** changes the current Chebyshev type 1 coefficients without altering the filter
		order. This allows for use in an audio process thread as it avoids dynamic allocation
		of memory. Filter sample and coefficient buffers are unaltered
		
		for initialising a chebyshev type 1 filter @see setChebyICoefficients
		
		@params cutFreq		normalised cutoff frequency (0 < x < .5)
		@params shelfType	bool filter shelf type, false = low pass, true = high pass
		@params ripple		percentage ripple (<.2929)
		
		@returns boolean false on error and true on success
	 */
	bool setChebyICoefficients(double cutFreq, bool shelfType, double ripple);
	//==========================================================================
protected:	// methods
	//==========================================================================
	/** set firCoefficients and iirCoefficients
		for required chebyshev type I filter
		sampleBuffer memory is also set
		
		@params cutFreq		normalised cutoff frequency (0 < x < .5)
		@params shelfType	bool filter shelf type, false = low pass, true = high pass
		@params ripple		percentage ripple (<.2929)
		@params poles		number of poles
		
		@returns boolean false on error and true on success
	 */
	bool changeChebyICoefficients(double cutFreq, bool shelfType, double ripple,int poles);
	/** a simple normalised fir low pass filter
		@params order	number of delay coefficients
		*/
	//==========================================================================
	bool setSimpleLpf(int order);
private:	// methods
	//==========================================================================
	/** increment the buffer index and wrap it to the filter order*/
	void incBufferIndex();
	
	//==========================================================================
	/** checks internal memory storage of filter coeffcients and deletes if
		required
		*/
	void clearMemory();
	
	/** will allocate memory to a buffer given the current filter order and set
		all values == 0.00
		*/
	void allocateBufferMemory();
	/** root mean square of signal over a specific sample window */
	double rms(double sample);
	
public:		// variables
	//==========================================================================
protected:  // variables
	//==========================================================================
	/** Numerator coefficients in delay filter
		firCoefficients[0] z^0 coeffcieint
		firCoefficients[1] z^-1 coefficient
	 */
	double* firCoefficients = 0;
	/** Denomiator coefficients in delay filter
		@see firCoefficients
	 */
	double* iirCoefficients = 0;
	/** hold temporary values for fir coeffcient buffer*/
	double* firTemp = 0;
	/** hold temporary values for iir coeffcient buffer*/
	double* iirTemp = 0;
	/** buffer to hold forward delay sample data
	 */	
	double* firBuffer = 0;
	/** buffer to hold backward delay sample data
	 */
	double* iirBuffer = 0;
	/**current buffer index*/
	int bufferIndex = 0;
	/** order of delay filter including the zero delay coefficients*/
	int filterOrder = 0;
	/***/
	int samplingRate = 0;
	/** window size in samples of rms window*/
	const int rmsWindowSize = 128;
	/** current write index of rmsBuffer */
	int rmsBufferIndex = 0;
	/** RMS window buffer */
	double* rmsBuffer = new double[rmsWindowSize];
private:	// variables
	//==========================================================================
	/***/
	constexpr static const double pi = 3.141592653589793;
};

#endif /* FilterEffectBase_hpp */
