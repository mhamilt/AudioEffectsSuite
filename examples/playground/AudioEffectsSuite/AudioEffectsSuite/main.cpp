//
//  main.cpp
//  AudioEffectsSuiteDevelopment
//
//  Created by admin on 18/12/2017.
//  Copyright © 2017 AudioEffectsSuiteTeam. All rights reserved.
//

#include <iostream>
#include <vector>
#include <immintrin.h>

#include "../../../../cpp-cli-audio-tools/src/CliAudioTools.h"

#include "../../../../BaseClasses/BaseClasses.h"
#include "../../../../DelayEffects/DelayEffects.h"
#include "../../../../FilterEffects/FilterEffects.h"

int main(int argc, const char * argv[])
{
    printf("Path relative to the working directory is: %s\n", argv[0]);
    //==========================================================================
    // hard coded files: these can be changed to be elements of argv for command-line options
    const char ifile[] = "../../../../../../AudioExamples/AfricanDrumming.wav";
    const char ofile[] = "../../../../../../OuputFiles/test.wav";
    //==========================================================================
    // read file(s)
    AudioWavFileReadWrite audioReadWriter;
    int numOfFrames = 44100*.25, sampleRate = 44100;
    double** stereoIn = audioReadWriter.readStereoWav(ifile, &numOfFrames, &sampleRate);
    if (!stereoIn)
    {
        stereoIn = new double*[2];
        stereoIn[0] = audioReadWriter.readWav(ifile, &numOfFrames, &sampleRate);
        stereoIn[1] = audioReadWriter.readWav(ifile, &numOfFrames, &sampleRate);
    }
    if (!stereoIn[0])
    {
        return EXIT_FAILURE;
    }
    double** stereoOut = new double*[2];
    stereoOut[0] = new double[numOfFrames];
    std::fill(stereoOut[0], stereoOut[0]+numOfFrames, 0);
    stereoOut[1] = new double[numOfFrames];
    std::fill(stereoOut[1], stereoOut[0]+numOfFrames, 0);
    //==========================================================================
    // Sample Rate Dependant Effects
    SimpleDelay delayLeft(8810, sampleRate), delayRight(8810, sampleRate);
    //==========================================================================
    // Change Parameters
    delayLeft.setupSimpleDelay(8000);
    delayRight.setupSimpleDelay(4000);
    //==========================================================================
    const double radPerSec = 2*3.1415926536/double(sampleRate);
    //==========================================================================
    for (int i = 0; i < numOfFrames; i++)
    {
        stereoOut[0][i] = delayLeft.process(stereoIn[0][i]);
        stereoOut[1][i] = delayRight.process(stereoIn[1][i]);
    }
    //==========================================================================
    audioReadWriter.writeWavSS(stereoOut, ofile, numOfFrames, sampleRate);
    //==========================================================================
    audioReadWriter.printWavHeader(ifile);
    audioReadWriter.printWavHeader(ofile);
    //==========================================================================
    for (int chan = 0; chan < 2; chan++)
    {
        delete [] stereoIn[chan];
        delete [] stereoOut[chan];
    }
    delete [] stereoIn;
    delete [] stereoOut;
    //==========================================================================
    AudioPlayerOsX ap;
    ap.playFile(ofile);
    //==========================================================================
    
    return EXIT_SUCCESS;
}

