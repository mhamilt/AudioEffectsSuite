//
//  main.cpp
//  AudioEffectsSuiteDevelopment
//
//  Created by admin on 18/12/2017.
//  Copyright © 2017 AudioEffectsSuiteTeam. All rights reserved.
//

#include <iostream>
#include "../../../BaseClasses/DigitalEffectBase.cpp"
#include "../DelayEffects/SimpleDelay.cpp"
#include "../../../AudioIOClasses/AudioWavFileReadWrite.cpp"

int main(int argc, const char * argv[]) {
	SimpleDelay delay(4000);
	AudioWavFileReadWrite audioReadWriter;
	
	const char ifile[] = "/Users/admin/Downloads/MtoS.wav";
	const char ofile[] = "/Users/admin/Downloads/MtoS_delayed.wav";

	int totalSamples, sampleRate;
	double *in;
	in = audioReadWriter.readWav(ifile, &totalSamples, &sampleRate);
	
	double *out = new double[totalSamples];
	
	for (int i = 0; i<totalSamples;i++)
	{
		out[i] = delay.process(in[i]);
	}
	
	audioReadWriter.writeWavMS(out, ofile, totalSamples, sampleRate);
	
	return EXIT_SUCCESS;
}
