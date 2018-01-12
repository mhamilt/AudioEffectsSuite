//
//  EnvelopeFilter.cpp
//  AudioEffectsSuite
//
//  Created by admin on 12/01/2018.
//  Copyright © 2018 AudioEffectsSuiteTeam. All rights reserved.
//

#include "EnvelopeFilter.hpp"


double EnvelopeFilter::process(double sample)
{
	setChebyICoefficients(0.001+envelopeFollower.envelope(2*sample),false,.1); //Offset avoids zero cutoff value
	return applyFilter(sample);
}
