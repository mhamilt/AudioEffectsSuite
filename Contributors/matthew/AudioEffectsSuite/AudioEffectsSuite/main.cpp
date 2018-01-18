//
//  main.cpp
//  AudioEffectsSuiteDevelopment
//
//  Created by admin on 18/12/2017.
//  Copyright © 2017 AudioEffectsSuiteTeam. All rights reserved.
//

#include <iostream>
#include <vector>
#include "../../../../BaseClasses/DelayEffectBase.cpp"
#include "../../../../BaseClasses/FilterEffectBase.cpp"
#include "../../../../BaseClasses/ModulationBaseClass.cpp"
#include "../../../../DelayEffects/SimpleDelay.cpp"
#include "../../../../DelayEffects/SimpleFlanger.cpp"
#include "../../../../DelayEffects/FilteredDelay.cpp"
#include "../../../../AudioIOClasses/AudioWavFileReadWrite.cpp"
#include "../../../../AudioIOClasses/AudioPlayer.cpp"
#include "../../../../FilterEffects/SimpleLPF.cpp"
#include "../../../../FilterEffects/EnvelopeFilter.cpp"

int main(int argc, const char * argv[])
{
    
    //==============================================================================
    //	printf("Path relative to the working directory is: %s\n", argv[0]);
//    SimpleLPF filterLeft(4), filterRight(4), envelopeDetectorLeft(4),envelopeDetectorRight(4);
//    FilteredDelay filterDelayLeft(10000),filterDelayRight(10000);
    
    // Delay class initiliases with a delay value in samples.
//    SimpleDelay delayLeft(8000);
//    SimpleDelay delayRight(800);
//    SimpleFlanger flangerLeft;
//    SimpleFlanger flangerRight;
    
//    EnvelopeFilter envFiltL,envFiltR;
    //==============================================================================
    const char ifile[] = "/Users/admin/Music/Space Lion.wav";
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
    //    audioReadWriter.printWavHeader(ifile);
    //==============================================================================
    AudioWavFileReadWrite audioReadWriter;
    int numOfFrames = 44100, sampleRate = 44100;
    
    
    //	double** stereoIn = audioReadWriter.whiteNoise(numOfFrames, sampleRate);;
    double** stereoIn = audioReadWriter.readStereoWav(ifile, &numOfFrames, &sampleRate);
    //    double* audioData = audioReadWriter.readWav(ifile, &numOfFrames, &sampleRate);
    
    double** stereoOut = new double*[2];
    stereoOut[0] = new double[numOfFrames];
    stereoOut[1] = new double[numOfFrames];
    //==============================================================================
    // // Change Parameters
//    filterDelayLeft.setDelayGain(.85);
//    filterDelayLeft.setFeedbackGain(0.2);
//    filterDelayRight.setDelayGain(.95);
//    filterDelayRight.setFeedbackGain(0.5);
    
//    envelopeDetectorLeft.setChebyICoefficients(.00006, false, 0);
//    envelopeDetectorRight.setChebyICoefficients(.00006, false, 0);
    
//    const double flangeDepth = sampleRate*0.0015;
//    flangerLeft.setEffectParams(1., flangeDepth, .15);
//    flangerRight.setEffectParams(1., flangeDepth, .215);
    //==============================================================================
    
    for (int i = 0; i<numOfFrames;i++)
        //		for (int i = 0; i<20;i++)
    {
        stereoOut[0][i] = stereoIn[0][i];
        stereoOut[1][i] = stereoIn[1][i];
    }
    //==============================================================================
    audioReadWriter.writeWavSS(stereoOut, ofile, numOfFrames, sampleRate);
    //      audioReadWriter.writeWavSS(stereoIn, ofile, numOfFrames, sampleRate);
    //      audioReadWriter.writeWavMS(audioData, ofile, numOfFrames, sampleRate);
    //==============================================================================
    audioReadWriter.printWavHeader(ifile);
    audioReadWriter.printWavHeader(ofile);
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
