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
#include "../BaseClasses/DelayEffectBase.hpp"

class SimpleFlanger;
//==============================================================================
/**
	Simple Flanger Effect Consistig of a single voice flanger
	The flanger has an effective range between 0 and 15 miliseconds
	in this case dleay buffer should be set to sampleRate*3/200
	
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
    SimpleFlanger();
    SimpleFlanger(double extSampleRate);
	
	/** Destructor. */
    ~SimpleFlanger();
	
	/** setEffectGain: sets the effect gain to a value between 1 and -1
	 
		@param gain		required delay gain. Values beyond 1 and -1
	 are capped to the maximum to avoid idiocy.
	 Negative velus invoke a phase inversion.
	 */
	void setEffectGain(double gain);
    
    /**
     <#Description#>

     @param depth <#depth description#>
     */
    void setDepth(const double depth);
    
    /**
     <#Description#>

     @param rate <#rate description#>
     */
    void setRate(const double rate);
	
	/** setEffectGain: sets the parameters for effect
		@param	gain	effect gain
		@param	depth	depth of modulation in samples
		@param	rate	rate of modulation in Hz
	 */
	void setEffectParams(double gain, double depth, double rate);
	//==============================================================================
	/**Apply the DSP effect*/
	double process(double inputSample) override;
	
    void setupSimpleFlanger(double extSampleRate);
private: //Methods
	/** capGain: caps gain to a range of 1 and -1;
	 *	@param gain address of gain value
	 */
	double capGain(double gain);
	
	/** setAngleDelta: sets the angleDelta for delay modulation*/
    void setAngleDelta();
	
	/** updateModulation: updates the modulationIndex by the correct
		increment*/
	void updateModulation();
	
private: //member vairables
	/** internal class declaration of pi
		it would likely make sense to have this moved
		to a higher class
		*/
	constexpr static const double internal_Pi = 3.141592653589793;
	
	/***/
	double modulationDepth = 1000, modulationRate = 0, effectGain = .01;
	
	//	M1 = M01*(1+sin(2*pi*f1.*n/FS))
	/***/
	double modulationIndex = 0;
	
	/** 1/sampleRate: The time in seconds between samples*/
	double timeStep = 1./44100.;
	
	/** 2 * pi * modulationRate / sampleRate */
	double modulationConstant, modulationAngle = 0;
	
	//const double cyclesPerSample = modulationRate * timeStep;
	/**increment value for modulation signal*/
	double angleDelta = 2*internal_Pi*timeStep;
};


#endif /* SimpleFlanger_hpp */
