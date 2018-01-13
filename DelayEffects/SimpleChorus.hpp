//
//  SimpleChorus.hpp
//
//
//  Created by admin on 12/01/2018.
//
//

#ifndef SimpleChorus_hpp
#define SimpleChorus_hpp


/** Simple Chorus effect with a single delay voice and mono output
 
	Chorus is effective between 15 and 20 miliseconds delay of original
	audio. Requires the sample rate when initialising.*/
class SimpleChorus : public DelayEffectBase
{
	
private:
	/** swing range of delayed audio index in samples
	 typical maximum swing would be 15 milliseconds
		(i.e. 0.015*sampleRate)*/
	double swing = 0;
	/** minimum delay in samples. Typically 10 milliseconds */
	double base = 0;
	/** the minimum value of the lowpassed random modulation signal*/
	double modMin = .493;
	/** the maximum value of the lowpassed random modulation signal*/
	double modMax = .506;
	/** the normalising factor of the random delay signal*/
	double modNorm = 1/(modMax - modMin);
	
	/** modulation signal scaling equation: ((n - modMin)*ModNorm*swing) + base
	 
		modulates a random white noise signal by low pas filtering then scaling
		to a range between 15 to 25 miliseconds of delay.*/
	double modulateDelay();
public:
	/** Constructor: initialises the effect parameters
		Since the class inherits DelayEffectBase it must set a
		delay buffer size when initialising.
		
		@params sampleRate	The sample rate of incoming audio.
		*/
	SimpleChorus(int sampleRate) : DelayEffectBase(sampleRate)
	{
		swing = 0.015*sampleRate;
		base = 0.01*sampleRate;
	}
};

#endif /* SimpleChorus_hpp */
