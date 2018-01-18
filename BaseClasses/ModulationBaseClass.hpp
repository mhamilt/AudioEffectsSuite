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
     sets wavetable to one period of a sine wave
     */
    void setSine();
    
    /**
     Read through values in waveTable as a given frequency
     
     @param freq read speed in Hz: essentially the number of samples
     jumped between reads
     @return value from table as double
     */
    double readTable(double freq);
private:
    /**
     allocate memory to internal wave table based on sample rate
     
     @return returns true on success or false on failure
     */
    bool allocateMemory();
    
    /**
     get the interpolated output of the waveTable from the
     given buffer index

     @param bufferIndex buffer index as double
     @return returns interpolated value from surrounding wavtable indices
     */
    double getInterpolatedOut(double bufferIndex);
    
    bool setInterpoationTable();
private:
    /** Internal Sample Rate */
    int sampleRate;
    /** time between samples: 1/sampRate */
    double timeStep;
    /** current table read index */
    int tableIndex = 0;
    /** store modulation signal as*/
    double* waveTable;
    
    static const int order = 4;
    static const int res = 100;
    double interpTable[order][res] = {0};
};

#endif /* ModulationBaseClass_hpp */
