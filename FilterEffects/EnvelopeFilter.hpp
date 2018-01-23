//
//  EnvelopeFilter.hpp
//  AudioEffectsSuite
//
//  Created by admin on 12/01/2018.
//  Copyright © 2018 AudioEffectsSuiteTeam. All rights reserved.
//

#ifndef EnvelopeFilter_hpp
#define EnvelopeFilter_hpp

#include <cmath>
#include "../BaseClasses/DelayEffectBase.hpp"
#include "../BaseClasses/FilterEffectBase.hpp"
#include "../FilterEffects/SimpleLPF.hpp"

class EnvelopeFilter : public FilterEffectBase
{
public:
	/** Constructor */
	EnvelopeFilter(): envelopeFollower(.00006, 4)
	{
	// NOTE: Initialising chebyshev coeffcients allocates memory, perhaps alter so that memory is already pre allocated
	changeChebyICoefficients(.01, false, .1, 4);
	envelopeFollower.setChebyICoefficients(.00006, false, 0);
	};
	/** Desructor */
	~EnvelopeFilter(){};
	/** main process method: applies an envelope filter
		to the incoming signal sample
		
		@params sample		incoming signal sample value
		
		@returns processed sample value
		*/
		double process(double sample);
private:
	/** this follows the signal envelope and alters the internal
		low pass filter cutoff
	*/
	SimpleLPF envelopeFollower;
};

#endif /* EnvelopeFilter_hpp */
