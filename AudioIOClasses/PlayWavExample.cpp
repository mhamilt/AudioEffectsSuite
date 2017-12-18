//
//  Audio Player Test Compile
//  AudioEffectsSuite
//
//  Created by Matthew on 12/12/2017.
//
// This is hacked together from a couple of files I've found and is really
// just a way to quickly check audio in line. To compile it does require the
// Core Foundation and AudioToolbox frameworks.


#include <CoreFoundation/CoreFoundation.h>
#include <iostream>
#include "AudioPlayer.cpp"

int main(int argc, const char * argv[]){
	//==========================================================================
	// This only works if music is in the working diretory of the execution file
	// This directory cannot be accessed absolutely but if is in a container
	// Drop a file in here or change the destination of your xcode build
	// in the File -> Workspace settings... Workspace relative location
	//==========================================================================
	printf("Path relative to the working directory is: %s\n", argv[0]);
	
	playAudio(/*Filename*/);
	
	return EXIT_SUCCESS;
}

