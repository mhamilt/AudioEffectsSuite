//
//  main.cpp
//  AudioEffectsSuiteDevelopment
//
//  Created by admin on 18/12/2017.
//  Copyright © 2017 AudioEffectsSuiteTeam. All rights reserved.
//

#include <iostream>
#include <vector>
#include "../../../BaseClasses/DelayEffectBase.cpp"
#include "../../../BaseClasses/FilterEffectBase.cpp"
#include "../../../DelayEffects/SimpleDelay.cpp"
#include "../../../DelayEffects/SimpleFlanger.cpp"
#include "../../../AudioIOClasses/AudioWavFileReadWrite.cpp"
#include "../../../AudioIOClasses/AudioPlayer.cpp"
#include "../../../FilterEffects/SimpleLPF.cpp"

int main(int argc, const char * argv[])
{
	//==============================================================================
	//	printf("Path relative to the working directory is: %s\n", argv[0]);
	SimpleLPF filterLeft, filterRight;
	
	// Delay class initiliases with a delay value in samples.
	SimpleDelay delayLeft(8000);
	SimpleDelay delayRight(800);
	SimpleFlanger flangerLeft;
	SimpleFlanger flangerRight;
	const char ifile[] = "/Users/admin/Documents/Masters/PBMMI/Audio_Examples/GuitarStrum_12s_MN.wav";
	//	"/Users/admin/Documents/Masters/PBMMI/Audio_Examples/GuitarStrum_12s_MN.wav"
	//	"/Users/admin/Downloads/Space Lion.wav"
	//	"/Users/admin/Documents/Masters/PBMMI/Audio_Examples/trigen.wav"
	//	"/Users/admin/Documents/Masters/PBMMI/Audio_Examples/Dance_Stereo.wav"
	const char ofile[] = "/Users/admin/Downloads/MtoS_delayed.wav";
	//==============================================================================
	AudioWavFileReadWrite audioReadWriter;
	int numOfFrames, sampleRate;
	double **stereoIn;
	stereoIn = audioReadWriter.readStereoWav(ifile, &numOfFrames, &sampleRate);
	
	double** stereoOut = new double*[2];
	stereoOut[0] = new double[numOfFrames];
	stereoOut[1] = new double[numOfFrames];
	//==============================================================================
	// // Change Parameters
	delayLeft.setDelayGain(.75);
	delayLeft.setFeedbackGain(0.0);
	delayRight.setDelayGain(.75);
	delayRight.setFeedbackGain(0.0);
	
	const double flangeDepth = sampleRate*0.001;
	flangerLeft.setEffectParams(1, flangeDepth*2, .15);
	flangerRight.setEffectParams(1, flangeDepth, .15);
	//==============================================================================
	for (int i = 0; i<numOfFrames;i++)
	//	for (int i = 0; i<4;i++)
	{
		stereoOut[0][i] = filterLeft.applyFilter(stereoIn[0][i]);
		stereoOut[1][i] = filterRight.applyFilter(stereoIn[1][i]);
	}
	//==============================================================================
	audioReadWriter.writeWavSS(stereoOut, ofile, numOfFrames, sampleRate);
	//==============================================================================
	playAudio(ofile);
	//==============================================================================
	
	return EXIT_SUCCESS;
}
