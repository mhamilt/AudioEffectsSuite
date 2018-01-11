//
//  whiteNoiseExample.cpp
//
//
//  Created by admin on 11/01/2018.
//
//
#include <iostream>
#include <cstring>
#include "AudioWavFileReadWrite.cpp"

int main(int argc, const char * argv[])
{
	//==========================================================================
	//	printf("Path relative to the working directory is: %s\n", argv[0]);
	//==========================================================================
	const char ofile[] = "/Users/admin/Downloads/noise.wav";
	//==========================================================================
	AudioWavFileReadWrite audioReadWriter;
	int numOfFrames = 44100;
	int sampleRate = 44100;
	
	double **stereoWhiteNoise;
	stereoWhiteNoise = audioReadWriter.whiteNoise(numOfFrames, sampleRate);
	//==========================================================================
	audioReadWriter.writeWavSS(stereoWhiteNoise, ofile, numOfFrames, sampleRate);
	//==========================================================================
	return EXIT_SUCCESS;
}
