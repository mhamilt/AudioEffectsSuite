//
//  main.cpp
//  AudioEffectsSuiteDevelopment
//
//  Created by admin on 18/12/2017.
//  Copyright © 2017 AudioEffectsSuiteTeam. All rights reserved.
//

#include <iostream>
#include "../../../BaseClasses/DelayEffectBase.cpp"
#include "../../../DelayEffects/SimpleDelay.cpp"
#include "../../../AudioIOClasses/AudioWavFileReadWrite.cpp"
#include "../../../AudioIOClasses/AudioPlayer.cpp"

int main(int argc, const char * argv[])
{
//==============================================================================

	//	printf("Path relative to the working directory is: %s\n", argv[0]);
	
	// Delay class initiliases with a delay value in samples.
	SimpleDelay delay(4000);

	//Set absolute path of files
	const char ifile[] = "/Users/admin/Downloads/MtoS.wav";
	const char ofile[] = "/Users/admin/Downloads/MtoS_delayed.wav";
	
//==============================================================================
	AudioWavFileReadWrite audioReadWriter;
	int totalSamples, sampleRate;
	double *in;
	in = audioReadWriter.readWav(ifile, &totalSamples, &sampleRate);
	
	double *out = new double[totalSamples];
//==============================================================================


	for (int i = 0; i<totalSamples;i++)
	{
		out[i] = delay.process(in[i]);
	}



//==============================================================================
	audioReadWriter.writeWavMS(out, ofile, totalSamples, sampleRate);
//==============================================================================
	playAudio(ofile);
//==============================================================================
	
	return EXIT_SUCCESS;
}
