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
#include "../../../DelayEffects/SimpleDelay.cpp"
#include "../../../DelayEffects/SimpleFlanger.cpp"
#include "../../../AudioIOClasses/AudioWavFileReadWrite.cpp"
#include "../../../AudioIOClasses/AudioPlayer.cpp"

int main(int argc, const char * argv[])
{
	//==============================================================================
	//	printf("Path relative to the working directory is: %s\n", argv[0]);
	
	// Delay class initiliases with a delay value in samples.
	SimpleDelay delay(4000);
	SimpleFlanger flanger;
	SimpleFlanger flangerLeft;
	SimpleFlanger flangerRight;
	//Set absolute path of files
	const char ifile[] = "/Users/admin/Documents/Masters/PBMMI/Audio_Examples/GuitarStrum_12s_MN.wav";
	//	const char ofile[] = "/Users/admin/Downloads/MtoS_delayed.wav";
	/// Users/admin/Documents/Masters/PBMMI/Audio_Examples/trigen.wav
	const char ofile[] = "/Users/admin/Downloads/MtoS_delayed.wav";
	//	"/Users/admin/Documents/Masters/PBMMI/Audio_Examples/Dance_Stereo.wav"
	//==============================================================================
	AudioWavFileReadWrite audioReadWriter;
	int numOfFrames, sampleRate;
	double *in, **stereoIn;
	in = audioReadWriter.readWav(ifile, &numOfFrames, &sampleRate);
	stereoIn = audioReadWriter.readStereoWav(ifile, &numOfFrames, &sampleRate);
	
	double *out = new double[numOfFrames];
	double** stereoOut = new double*[2];
	stereoOut[0] = new double[numOfFrames];
	stereoOut[1] = new double[numOfFrames];
	//==============================================================================
	// Change Parameters
		const double flangeDepth = sampleRate*0.001;
	//	flanger.setEffectParams(1, flangeDepth, 6.15);
	flangerLeft.setEffectParams(1, flangeDepth*2, .15);
	flangerRight.setEffectParams(1, flangeDepth, .15);
	//==============================================================================
	for (int i = 0; i<numOfFrames;i++)
	{
		//	out[i] = flanger.process(in[i]);
		stereoOut[0][i] = .6*flangerLeft.process(stereoIn[0][i]);
		stereoOut[1][i] = .6*flangerRight.process(stereoIn[1][i]);
	}
	//==============================================================================
	//	audioReadWriter.writeWavMS(out, ofile, totalSamples, sampleRate);
	audioReadWriter.writeWavSS(stereoOut, ofile, numOfFrames, sampleRate);
	//==============================================================================
	playAudio(ofile);
	//==============================================================================
	
	return EXIT_SUCCESS;
}
