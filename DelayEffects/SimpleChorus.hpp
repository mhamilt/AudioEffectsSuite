//
//  SimpleChorus.hpp
//
//
//  Created by admin on 12/01/2018.
//
//

#ifndef SimpleChorus_hpp
#define SimpleChorus_hpp

#include "../BaseClasses/DelayEffectBase.cpp"
//#include "../BaseClasses/FilterEffectBase.cpp"
#include "../FilterEffects/SimpleLPF.cpp"
#include "../BaseClasses/ModulationBaseClass.cpp"


/** Simple Chorus effect with a single delay voice and mono output
 
	Chorus is effective between 15 and 20 miliseconds delay of original
	audio. Requires the sample rate when initialising.*/
class SimpleChorus : public DelayEffectBase,
                     public ModulationBaseClass,
                     public SimpleLPF
{
public:
    /** Constructor: initialises the effect parameters
     Since the class inherits DelayEffectBase it must set a
     delay buffer size when initialising.
     
     @params sampRate	The sample rate of incoming audio.
     */
    SimpleChorus(int sampRate) : ModulationBaseClass(sampRate),
                                 DelayEffectBase(double(sampRate)*.031),
                                 SimpleLPF(0.0004,4)
    {
        swing = 0.005*sampRate;
        base = 0.015*sampRate;
        setRandLfo();
    }
	//==============================================================================
    /**
     apply chorus effect to audio sample

     @param inputSample input audio sample
     @return processed audio sample
     */
    double process(double inputSample);
private:
    //==============================================================================
	/** swing range of delayed audio index in samples
	 typical maximum swing would be 15 milliseconds
		(i.e. 0.015*sampleRate)*/
	double swing;
	/** minimum delay in samples. Typically 10 milliseconds */
	double base;
	/** the minimum value of the lowpassed random modulation signal*/
	double modMin = .5;
	/** the maximum value of the lowpassed random modulation signal*/
	double modMax = .5;
	/** the normalising factor of the random delay signal*/
	double modNorm = 1/(modMax - modMin);
	
	/** modulation signal scaling equation: ((n - modMin)*modNorm*swing) + base
	 
		modulates a random white noise signal by lowpass filtering then scaling
		to a range between 15 to 25 miliseconds of delay.*/
	double getModSignal();
    
    void setRandLfo();
};

#endif /* SimpleChorus_hpp */
