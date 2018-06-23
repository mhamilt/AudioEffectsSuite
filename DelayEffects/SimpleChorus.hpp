//
//  SimpleChorus.hpp
//
//
//  Created by admin on 12/01/2018.
//
//

#ifndef SimpleChorus_hpp
#define SimpleChorus_hpp

#include "../BaseClasses/DelayEffectBase.hpp"
#include "../FilterEffects/SimpleLPF.hpp"
#include "../BaseClasses/ModulationBaseClass.hpp"


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
     
     */
    SimpleChorus() : SimpleLPF(0.0001,4)
    {
    }
    SimpleChorus(int extSampleRate) : ModulationBaseClass(extSampleRate),
                                      DelayEffectBase(double(extSampleRate)*.031),
                                      SimpleLPF(0.0001,4)
    {
        swing = 0.005*sampleRate;
        base = 0.015*sampleRate;
        if (sampleRate != 0)
            setRandLfo();
    }
	//==============================================================================
    /**
     apply chorus effect to audio sample

     @param inputSample input audio sample
     @return processed audio sample
     */
    double process(double inputSample);
    
    /**
     set parameters and internal sample rate

     @param extSampleRate external sample rate
     */
    void setupChorus(double extSampleRate);
    
    /**
     set the 'swing' of the chorus: The amount intensity of vibrato in the delay
     signal

     @param swingAmount <#swingAmount description#>
     */
    void setSwing(double swingAmount);
    
    
    /**
     sets the 'base' of the chorus: the minimum delay in the signal.

     @param baseAmount <#baseAmount description#>
     */
    void setBase(double baseAmount);
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
    
    const double readSpeed = ((readNoise()+1)*.5)*.0005;
    
    void setRandLfo();
};

#endif /* SimpleChorus_hpp */
