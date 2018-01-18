//
//  ModulationBaseClass.hpp
//  AudioEffectsSuite
//
//  Created by admin on 16/01/2018.
//  Copyright © 2018 AudioEffectsSuiteTeam. All rights reserved.
//

#ifndef ModulationBaseClass_hpp
#define ModulationBaseClass_hpp

#include <iostream>
#include <cmath>
#include <cstdint>

/**
 <#Description#>
 */
class ModulationBaseClass {
public:
    //==============================================================================
    /** Constructor */
    ModulationBaseClass(double extSampRate)
    {
        sampleRate = extSampRate;
        timeStep = 1./extSampRate;
        allocateMemory();
        setInterpoationTable();
        srand (static_cast <unsigned> (time(0)));
    };
    /** Destructor */
    ~ModulationBaseClass(){};
    //==============================================================================
    /**
     sets a randomised white noise signal in the waveTable
     */
    void setRandLfo();
    /**
     sets wavetable to one period of a triangle wave
     */
    void setTriangle();
    /**
     sets wavetable to one period of a square wave
     */
    void setSquare();
    /**
     sets wavetable to one period of a sawtooth wave
     */
    void setSawtooth();
    /**
     sets wavetable to one period of a sine wave oscillating between -1 and 1
     */
    void setSine();
    /**
     sets wavetable to one period of a sine wave oscillating between 0 and 1
     */
    void setOffSine();
    /**
     
     */
    /**
     reads out white noise

     @return random number between -1 and 1
     */
    double readNoise();
    /**
     clip wave table values with a tanh function. Effect change with a variable
     amp to control intensity.

     @param amp <#amp description#>
     */
    void clipWave(double amp);
    //==============================================================================
    /**
     Read through values in waveTable as a given frequency
     
     @param freq read speed in Hz: essentially the number of samples
     jumped between reads
     @return value from table as double
     */
    double readTable(double freq);
    //==============================================================================
    void printInterpTable();
    //==============================================================================
    /**
     populates the internal interpolation table
     
     @return return tru on success, else false
     */
    bool setInterpoationTable();
private:
    //==============================================================================
    /**
     allocate memory to internal wave table based on sample rate
     
     @return returns true on success or false on failure
     */
    bool allocateMemory();
    //==============================================================================
    /**
     get the interpolated output of the waveTable from the
     given buffer index
     
     @param bufferIndex buffer index as double
     @return returns interpolated value from surrounding wavtable indices
     */
    double getInterpolatedOut(double bufferIndex);
    
    /**
     get a cubic spline interpolated out from the wave table
     
     Derived from code by Alec Wright at repository:
     https://github.com/Alec-Wright/Chorus
     
     @authors Matthew Hamilton, Alec Wright
     @param bufferIndex the required buffer index
     @param freq (speed) that the table is being read through
     @return returns interpolated value as double
     */
    double getSplineOut(double bufferIndex, int freq);
public:
    //==============================================================================
    /** current table read index */
    double tableIndex = 0;
    /** Internal Sample Rate */
    int sampleRate;
    /** time between samples: 1/sampRate */
    double timeStep;
    
private:
    //==============================================================================
    /** store modulation signal as*/
    double* waveTable;
    
    static const int order = 4;
    static const int res = 100;
    double interpTable[order][res] = {1};
};

#endif /* ModulationBaseClass_hpp */
