//
//  FilteredDelay.hpp
//  
//
//  Created by admin on 10/01/2018.
//
//

#ifndef FilteredDelay_hpp
#define FilteredDelay_hpp

#include <iostream>
#include <cmath>
#include "../BaseClasses/DelayEffectBase.hpp"
#include "../BaseClasses/FilterEffectBase.hpp"

/** Delay effect that filters the repeat delay */
class FilteredDelay: public DelayEffectBase,
					 public FilterEffectBase
{
public:
	/** Constructor */
	FilteredDelay(int delayInSamples) : DelayEffectBase(44100)
	{
	delayTimeSamples = delayInSamples;
	changeChebyICoefficients(.05, true, .1, 4);
	};
	/** Destructor */
	~FilteredDelay(){};
public:
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
	/**apply the DSP effect*/
	double process(double inputSample) override;
	
private: //Methods
	/** capGain: caps gain to a range of 1 and -1;
	*	@param gain address of gain value
	*/
	void capGain(double& gain);

private: //member vairables
	/***/
	double delayGain=.707, feedbackGain=0.0;

};

#endif /* FilteredDelay_hpp */
