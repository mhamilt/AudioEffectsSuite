//
//  main.cpp
//  Effect Suite
//
//  Created by Joseph Hook on 19/12/2017.
//  Copyright © 2017 Joseph Hook. All rights reserved.
//

#include <CoreFoundation/CoreFoundation.h>
#include <iostream>
#include "../../../AudioIOClasses/AudioPlayer.cpp"

int main(int argc, const char * argv[]){
    //==========================================================================
    // This only works if music is in the working diretory of the execution file
    // This directory cannot be accessed absolutely but if is in a container
    // Drop a file in here or change the destination of your xcode build
    // in the File -> Workspace settings... Workspace relative location
    //==========================================================================
    printf("Path relative to the working directory is: %s\n", argv[0]);
    
    playAudio("/Users/joeyhook/Desktop/nes drums/effects/asskick.wav");
    
    return EXIT_SUCCESS;
}

