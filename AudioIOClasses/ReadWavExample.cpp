//
//  main.cpp
//  ReadStereo
//
//  Created by admin on 12/11/2017.
//  Copyright © 2017 admin. All rights reserved.
//

#include <iostream>
#include <cstring>

#include "AudioWavFileReadWrite.hpp"

int main(int argc, const char * argv[]) {
	
	const char *inFilename  /* = PATH/FILENAME.WAV*/;
	const char *outFilename /* = PATH/FILENAME.WAV*/;
	double *audioData;
	int length, SR;
	AudioWavFileReadWrite fileReadWriter;

	// Read a file then write it in another location
	audioData = fileReadWriter.readWav(inFilename, &length, &SR);
	fileReadWriter.writeWavMS(audioData, outFilename, length, SR);
	
    return 0;
}
