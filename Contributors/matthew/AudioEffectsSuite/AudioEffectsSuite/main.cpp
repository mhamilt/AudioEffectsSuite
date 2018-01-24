//
//  main.cpp
//  AudioEffectsSuiteDevelopment
//
//  Created by admin on 18/12/2017.
//  Copyright © 2017 AudioEffectsSuiteTeam. All rights reserved.
//

#include <iostream>
#include <vector>
#include "../../../../AudioIOClasses/AudioWavFileReadWrite.cpp"
#include "../../../../AudioIOClasses/AudioPlayer.cpp"

#include "../../../../BaseClasses/BaseClasses.h"
#include "../../../../DelayEffects/DelayEffects.h"
#include "../../../../FilterEffects/FilterEffects.h"

#include "/Users/admin/Documents/GitHub/moog-filter-plugin/recent/NonLinearJoeyMoogSC.cpp"

int main(int argc, const char * argv[])
{
    //	printf("Path relative to the working directory is: %s\n", argv[0]);
    //==============================================================================
    // // Audio Effects
    
    //==============================================================================
    const char ifile[] = "/Users/admin/Documents/Masters/PBMMI/Audio_Examples/GuitarStrum_12s_MN.wav";
    //	"/Users/admin/Documents/Masters/PBMMI/Audio_Examples/GuitarStrum_12s_MN.wav"
    //	"/Users/admin/Music/Space Lion.wav"
    //	"/Users/admin/Documents/Masters/PBMMI/Audio_Examples/TestGuitarPhraseMono.wav"
    //	"/Users/admin/Documents/Masters/PBMMI/Audio_Examples/trigen.wav"
    //	"/Users/admin/Documents/Masters/PBMMI/Audio_Examples/Dance_Stereo.wav"
    
    //    "/Users/admin/Documents/Masters/PBMMI/Audio_Examples/220hz16bit.wav"
    //    "/Users/admin/Documents/Masters/PBMMI/Audio_Examples/220hz24bit.wav"
    //    "/Users/admin/Documents/Masters/PBMMI/Audio_Examples/220hz32bit.wav"
    //    "/Users/admin/Documents/Masters/PBMMI/Audio_Examples/220hzu8bit.wav"
    //    "/Users/admin/Documents/Masters/PBMMI/Audio_Examples/stereo24bit.wav"
    
    const char ofile[] = "/Users/admin/Downloads/MtoS_delayed.wav";
    //==============================================================================
    AudioWavFileReadWrite audioReadWriter;
    int numOfFrames = 44100*.25, sampleRate = 44100;
    
//    double** stereoIn = audioReadWriter.whiteNoise(numOfFrames, sampleRate);;
        double** stereoIn = audioReadWriter.readStereoWav(ifile, &numOfFrames, &sampleRate);
    //    double* audioData = audioReadWriter.readWav(ifile, &numOfFrames, &sampleRate);
    
    double** stereoOut = new double*[2];
    stereoOut[0] = new double[numOfFrames];
    stereoOut[1] = new double[numOfFrames];
    //==============================================================================
    // // Sample Rate Dependant Effects
    SimpleFlanger flangeLeft, flangeRight;
    //==============================================================================
    // // Change Parameters
    flangeLeft.setupSimpleFlanger(sampleRate);
    flangeRight.setupSimpleFlanger(sampleRate);

    //==============================================================================
    //    const double radPerSec = 2*3.1415926536/double(sampleRate);
    //==============================================================================
//    for (int i = 0; i<20;i++)
    for (int i = 0; i<numOfFrames;i++)
    {
        stereoOut[0][i] = flangeLeft.process(stereoIn[0][i]) ;
        stereoOut[1][i] = flangeRight.process(stereoIn[1][i]) ;
    }
    
    //==============================================================================
    audioReadWriter.writeWavSS(stereoOut, ofile, numOfFrames, sampleRate);
    //      audioReadWriter.writeWavSS(stereoIn, ofile, numOfFrames, sampleRate);
    //      audioReadWriter.writeWavMS(audioData, ofile, numOfFrames, sampleRate);
    //==============================================================================
    //    audioReadWriter.printWavHeader(ifile);
    //    audioReadWriter.printWavHeader(ofile);
    //==============================================================================
    for (int chan = 0; chan < 2; chan++)
    {
        delete [] stereoIn[chan];
        delete [] stereoOut[chan];
    }
    delete [] stereoIn;
    delete [] stereoOut;
    //==============================================================================
    playAudio(ofile);
    
    //==============================================================================
    
    return EXIT_SUCCESS;
}
