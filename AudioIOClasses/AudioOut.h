//
//  AudioOut.h
//
// Contains functions to write an array as an audio file.
// OpenAL has been used to play audio out from command line

#ifndef AudioOut_hpp
#define AudioOut_hpp

#include <iostream>
#include <math.h>

#ifdef __APPLE__
#include "TargetConditionals.h"
#ifdef TARGET_OS_MAC
#include <OpenAL/al.h>	// macOS Only
#include <OpenAL/alc.h>	// macOS Only
#endif
#elif defined _WIN32 || defined _WIN64
#include <AL/al.h>
#include <AL/alc.h>
#endif
// alut.h library deprecated in macOS: but you can use freealut.
//
// In Terminal
// >> brew install freealut
//
// Don't forget to add the AL folder in the header search path
//
// /usr/local/Cellar/freealut/1.1.0/include
//
#include <AL/alut.h>
#define BACKEND "alut"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~WRITING .WAV~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\
//==============================================================================
// WAV header format: http://soundfile.sapp.org/doc/WaveFormat/

typedef struct waveFormatHeader
{
    char     chunkid[4];
    uint32_t chunksize;
    char     format[4];
    char     subchunk1id[4];
    uint32_t subchunk1size;
    uint16_t audioformat;
    uint16_t numchannels;
    uint32_t samplerate;
    uint32_t byterate;
    uint16_t blockalign;
    uint16_t bitspersample;
    char     subchunk2id[4];
    uint32_t subchunk2size;
} waveFormatHeader_t;

typedef struct midiHeader
{
    char		mthd[4];	//4bytes
    uint32_t	length;		//4bytes Big endian
    uint16_t	format;		//2bytes
    uint16_t	ntrks;		//2bytes
    uint16_t	division;	//2bytes
} midiHeader_t;

//==============================================================================

/**
 Define a standard 16bit PCM .wav file header.

 @return <#return value description#>
 */
waveFormatHeader_t * basicHeader()
{
    waveFormatHeader_t * wh = (waveFormatHeader_t *)malloc(sizeof(waveFormatHeader_t));
    memcpy(wh->chunkid, &"RIFF", 4);
    memcpy(wh->format, &"WAVE", 4);
    memcpy(wh->subchunk1id, &"fmt ", 4); //notice the space at the end!
    wh -> subchunk1size = 16;
    return wh;
}

//==============================================================================
/**
 Fill in the gaps of basicHeader() to create a header for a Stereo .wav file

 @param SampRate <#SampRate description#>
 @return <#return value description#>
 */
waveFormatHeader_t * stereo16bitWaveHeader(double SampRate)
{
    waveFormatHeader_t * wh = basicHeader();
    wh->audioformat = 1;
    wh->numchannels = 2;
    wh -> samplerate = (uint32_t)SampRate;
    wh -> bitspersample = 16;
    wh -> byterate = wh->numchannels * wh -> samplerate * wh -> bitspersample/8;
    wh -> blockalign = wh->numchannels * wh -> bitspersample/8;
    memcpy(wh->subchunk2id, &"data", 4);
    return wh;
}

//==============================================================================

/**
 Fill in the gaps of basicHeader() to create a header for a Mono .wav file

 @param SampRate <#SampRate description#>
 @return <#return value description#>
 */
waveFormatHeader_t * mono16bitWaveHeader(double SampRate)
{
    waveFormatHeader_t * wh = basicHeader();
    wh->audioformat = 1;
    wh->numchannels = 1;
    wh -> samplerate = (uint32_t)SampRate;
    wh -> bitspersample = 16;
    wh -> byterate = wh->numchannels * wh -> samplerate * wh -> bitspersample/8;
    wh -> blockalign = wh->numchannels * wh -> bitspersample/8;
    memcpy(wh->subchunk2id, &"data", 4);
    return wh;
}

//==============================================================================

/**
 numberOfFrames in this case referes to the number of samples in the incoming
 array.

 @param wh <#wh description#>
 @param numberOfFrames <#numberOfFrames description#>
 */
void setLengthForWaveFormatHeader(waveFormatHeader_t * wh, size_t numberOfFrames)
{
    wh -> subchunk2size = (uint32_t)numberOfFrames * wh->numchannels * wh->bitspersample/8;
    wh -> chunksize = 36 + wh -> subchunk2size;
}

//==============================================================================

/**
 <#Description#>

 @param numberOfFrames <#numberOfFrames description#>
 @param SampRate <#SampRate description#>
 @return <#return value description#>
 */
waveFormatHeader_t * stereo16bitWaveHeaderForLength(size_t numberOfFrames,double SampRate)
{
    waveFormatHeader_t * wh = stereo16bitWaveHeader(SampRate);
    setLengthForWaveFormatHeader(wh, numberOfFrames);
    return wh;
}

//==============================================================================


/**
 <#Description#>

 @param numberOfFrames <#numberOfFrames description#>
 @param SampRate <#SampRate description#>
 @return <#return value description#>
 */
waveFormatHeader_t * mono16bitWaveHeaderForLength(size_t numberOfFrames,double SampRate)
{
    waveFormatHeader_t * wh = mono16bitWaveHeader(SampRate);
    setLengthForWaveFormatHeader(wh, numberOfFrames);
    return wh;
}

//==============================================================================

/**
 <#Description#>

 @param audio <#audio description#>
 @param NF <#NF description#>
 */
void normaliseBuffer(double *audio, int NF)
{
    
    int n;
    double temp;
    
    // Find max abs sample
    double maxy = 0.0;
    
    for(n=0;n<NF;n++)
        
    {
        if(fabs(audio[n])>maxy) maxy = fabs(audio[n]);
        
    }
    
    // Normalise
    if(maxy>0.00001)
    {
        for(n=0;n<NF;n++)
            
        {
            temp     = audio[n];
            audio[n] = temp/maxy;
        }
    }
    
    // Smooth last 500 samples
    if(NF>501)
    {
        double inc  = 1.0/500.0;
        double ramp = 1.0;
        for(n=NF-501;n<NF;n++)
            
        {
            audio[n] *= ramp;
            if(ramp>0) ramp-=inc;
        }
    }
    
    printf("Normalised by : %.5f\n", maxy);
    
}

//==============================================================================

/**
 <#Description#>

 @param wh <#wh description#>
 @param file <#file description#>
 @return <#return value description#>
 */
size_t writeWaveHeaderToFile(waveFormatHeader_t * wh, FILE * file)
{
    return fwrite(wh, sizeof(waveFormatHeader_t), 1, file);
}


//==============================================================================


/**
 <#Description#>

 @param audio <#audio description#>
 @param outputFile <#outputFile description#>
 @param NumberOfSamples <#NumberOfSamples description#>
 @param SampRate <#SampRate description#>
 */
static void writeWavMS(double *audio,const char outputFile[], int NumberOfSamples, double SampRate)
{
    
    normaliseBuffer(audio ,NumberOfSamples);
    
    FILE * file;
    file = fopen(outputFile, "w");
    waveFormatHeader_t * wh = mono16bitWaveHeaderForLength(NumberOfSamples,SampRate);
    writeWaveHeaderToFile(wh, file);
    free(wh);
    int16_t sdata;
    double amp = 32000.0;
    
    int i;
    
    for(i=0;i<NumberOfSamples;i++)
    {
        
        sdata = (int16_t)(audio[i]*amp);  //set sdata to PCM 16-bit
        fwrite(&sdata, sizeof(int16_t), 1, file);
        
    }
    fclose(file);
    printf("%d samples written to %s\n", NumberOfSamples,outputFile);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~READING .WAV~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\
//==============================================================================

// Will read first channel only on multichannel files
/**
 <#Description#>

 @param filename <#filename description#>
 @param sampsperchan <#sampsperchan description#>
 @param sampleRate <#sampleRate description#>
 @return <#return value description#>
 */
double *readWav(const char *filename, int *sampsperchan, int *sampleRate)

{
    FILE *f;
    waveFormatHeader hdr;
    double *data;
    short *buf;
    int i, length;
    
    f = fopen(filename, "rb");
    if (!f)
    {
        return NULL;
    }
    fread(&hdr, 1, sizeof(hdr), f);
    /* quick sanity check */
    
    if ((strncmp(&hdr.chunkid[0], "RIFF", 4)) || (strncmp(&hdr.format[0], "WAVE", 4)) ||
        (strncmp(&hdr.subchunk1id[0], "fmt ", 4)) || (strncmp(&hdr.subchunk2id[0], "data", 4)))
    {
        fclose(f);
        return NULL;
    }
    
    /* check correct format */
    
    if ((hdr.bitspersample != 16))
    {
        fclose(f);
        printf("FILE NOT 16-BIT\n");
        return NULL;
    }
    
    *sampleRate = hdr.samplerate;
    length = hdr.subchunk2size / 2;
    *sampsperchan = hdr.subchunk2size*8/(hdr.numchannels*hdr.bitspersample);
    
    buf = (short *)malloc((length)*sizeof(short));
    if (!buf)
    {
        fclose(f);
        return NULL;
    }
    fread(buf, (length)*sizeof(short), 1, f);
    fclose(f);
    
    data = (double *)malloc((length)*sizeof(double));
    if (!data)
    {
        free(buf);
        return NULL;
    }
    
    // Dividing by this will scale a .wav to between -1 and 1
    double wavbitscale = pow(2,hdr.bitspersample)/2;
    
    int i2=0;
    for (i = 0; i < (*sampsperchan); i++)
    {
        for (int j = 0; j < hdr.numchannels; j++)
        {
            if (j==0)
            {
                data[i] = ((double)buf[i2]) / wavbitscale;
            }
            i2++;
        }
    }
    
    printf("%d samples read from %s\n",length,filename);
    
    free(buf);
    return data;
}

//==============================================================================

/**
 <#Description#>

 @param filename <#filename description#>
 @param length <#length description#>
 @param sampleRate <#sampleRate description#>
 @return <#return value description#>
 */
double **readStereoWav(const char *filename, int *length, int *sampleRate)


{
    FILE *f;
    waveFormatHeader hdr;
    //	double **data;
    short *buf;
    int i, sampsperchan;
    
    
    f = fopen(filename, "rb");
    if (!f)
    {
        return NULL;
    }
    fread(&hdr, 1, sizeof(hdr), f);
    /* quick sanity check */
    
    if ((strncmp(&hdr.chunkid[0], "RIFF", 4)) || (strncmp(&hdr.format[0], "WAVE", 4)) ||
        (strncmp(&hdr.subchunk1id[0], "fmt ", 4)) || (strncmp(&hdr.subchunk2id[0], "data", 4)))
    {
        fclose(f);
        return NULL;
    }
    
    
    /* check correct format */
    
    if ((hdr.bitspersample != 16))
    {
        fclose(f);
        return NULL;
    }
    
    *sampleRate = hdr.samplerate;
    *length = hdr.subchunk2size*.5;
    sampsperchan = hdr.subchunk2size*8/(hdr.numchannels*hdr.bitspersample);
    
    buf = (short *)malloc((*length)*sizeof(short));
    if (!buf)
    {
        fclose(f);
        return NULL;
    }
    
    fread(buf, (*length)*sizeof(short), 1, f);
    fclose(f);
    
    
    
    double** data = new double*[hdr.numchannels];
    
    if (!data)
    {
        free(buf);
        return NULL;
    }
    
    for(int i = 0; i < hdr.numchannels; ++i)
        
    {
        data[i] = (double *)malloc(sampsperchan * sizeof(double));
        
    }
    
    int i2=0;
    for (i = 0; i < (sampsperchan); i++)
    {
        for (int j = 0; j < hdr.numchannels; j++)
        {
            data[j][i] = ((double)buf[i2]) / 32768.0;
            i2++;
        }
    }
    
    
    printf("%d samples read from %s\n",*length,filename);
    
    free(buf);
    return data;
}


//==============================================================================

/**
 <#Description#>

 @param filename <#filename description#>
 */
void printWavHeader(const char *filename)

{
    FILE *f;
    waveFormatHeader hdr;
    
    f = fopen(filename, "rb");
    if (!f)
    {
        printf("NO FILE FOUND\n");
    }
    fread(&hdr, 1, sizeof(hdr), f);
    /* quick sanity check */
    printf("--- .WAV HEADER --- \n\n");
    printf("Chunk ID		: %s\n", hdr.chunkid);
    printf("Chunk Size		: %u\n", hdr.chunksize);
    printf("Format			: %s\n", hdr.format);
    printf("Sub-Chunk 1		: %s\n", hdr.subchunk1id);
    printf("Sub-Chunk 1 size: %d\n", hdr.subchunk1size);
    printf("Audio Format	: %d\n", hdr.audioformat);
    printf("Num Channels	: %d\n", hdr.numchannels);
    printf("Sample Rate		: %u\n", hdr.samplerate);
    printf("Byte Rate		: %u\n", hdr.byterate);
    printf("Block Align		: %u\n", hdr.blockalign);
    printf("Bits Per Samp	: %u\n", hdr.bitspersample);
    printf("Sub-Chunk 2		: %s\n", hdr.subchunk2id);
    printf("Sub-Chunk 2 size: %u\n", hdr.subchunk2size);
    printf("SampsPerChan	: %u\n", hdr.subchunk2size*8/(hdr.numchannels*hdr.bitspersample));
    // Subchunk2Size    == NumSamples * NumChannels * BitsPerSample/8
    // BitsPerSample    == 8 bits = 8, 16 bits = 16, etc.
    // Subchunk2Size*8/(NumChannels*BitsPerSample) = NumSamples
    fclose(f);
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~PLAYING .WAV~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==============================================================================

/**
 <#Description#>

 @param devices <#devices description#>
 */
static void list_audio_devices(const ALCchar *devices)

{
    const ALCchar *device = devices, *next = devices + 1;
    size_t len = 0;
    
    fprintf(stdout, "Devices list:\n");
    fprintf(stdout, "----------\n");
    while (device && *device != '\0' && next && *next != '\0')
    {
        fprintf(stdout, "%s\n", device);
        len = strlen(device);
        device += (len + 1);
        next += (len + 2);
    }
    fprintf(stdout, "----------\n");
}

#define TEST_ERROR(_msg)		\
error = alGetError();		\
if (error != AL_NO_ERROR) {	\
fprintf(stderr, _msg "\n");	\
}

//==============================================================================

/**
 <#Description#>

 @param channels <#channels description#>
 @param samples <#samples description#>
 @return <#return value description#>
 */
static inline ALenum to_al_format(short channels, short samples)
{
    bool stereo = (channels > 1);
    
    switch (samples)
    {
        case 16:
            if (stereo)
                return AL_FORMAT_STEREO16;
            else
                return AL_FORMAT_MONO16;
        case 8:
            if (stereo)
                return AL_FORMAT_STEREO8;
            else
                return AL_FORMAT_MONO8;
        default:
            return -1;
    }
}

//==============================================================================

#ifdef AL_ALUT_H
// This function reads in a .wav file and plays it out using OpenAL
/**
 <#Description#>

 @param inputfname <#inputfname description#>
 */
void playWavMS(char *inputfname)
{
    
    ALboolean enumeration;
    const ALCchar *defaultDeviceName = NULL;
    ALCdevice *device;
    ALvoid *data;
    ALCcontext *context;
    ALsizei size, freq;
    ALenum format;
    ALuint buffer, source;
    ALCenum error;
    ALint source_state;
    
    fprintf(stdout, "Using " BACKEND " as audio backend\n\n");
    
    enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
    if (enumeration == AL_FALSE)
        fprintf(stderr, "enumeration extension not available\n");
    
    list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
    
    if (!defaultDeviceName)
        defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
    
    device = alcOpenDevice(defaultDeviceName);
    if (!device)
    {
        
        TEST_ERROR("unable to open default device\n");
    }
    
    fprintf(stdout, "Device: %s\n", alcGetString(device, ALC_DEVICE_SPECIFIER));
    
    alGetError();
    
    context = alcCreateContext(device, NULL);
    if (!alcMakeContextCurrent(context))
    {
        fprintf(stderr, "failed to make default context\n");
        
    }
    
    TEST_ERROR("make default context");
    
    alGenSources((ALuint)1, &source);
    TEST_ERROR("source generation");
    
    alGenBuffers(1, &buffer);
    TEST_ERROR("buffer generation");
    
    alutLoadWAVFile(inputfname, &format, &data, &size, &freq);
    TEST_ERROR("loading wav file");
    
    alBufferData(buffer, format, data, size, freq);
    TEST_ERROR("buffer copy");
    
    alSourcei(source, AL_BUFFER, buffer);
    TEST_ERROR("buffer binding");
    
    alSourcePlay(source);
    TEST_ERROR("source playing");
    
    alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    TEST_ERROR("source state get");
    while (source_state == AL_PLAYING)
    {
        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
        TEST_ERROR("source state get");
    }
    
    /* exit context */
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    device = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
    
}
#else
/**
 <#Description#>

 @param inputfname <#inputfname description#>
 */
void playWavMS(char *inputfname)
{
    printf("ALUT not defined\n");
}
#endif

//==============================================================================

/**
 <#Description#>

 @param input <#input description#>
 */
void weirdPrint(const char input[])
{
    printf("%s\n", input);
}

//==============================================================================

/**
 <#Description#>

 @param filename <#filename description#>
 */
void printMidiHeader(const char *filename)

{
    FILE *f;
    midiHeader hdr;
    
    f = fopen(filename, "rb");
    if (!f)
    {
        printf("NO FILE FOUND\n");
    }
    fread(&hdr, 1, sizeof(hdr), f);
    /* quick sanity check */
    printf("--- .MIDI HEADER --- \n\n");
    printf("Header MThd		: %s\n", hdr.mthd);
    printf("Header Length	: %#x\n", hdr.length);
    printf("Format			: %#x\n", hdr.format);
    printf("Track Chunks	: %#x\n", hdr.ntrks);
    printf("Division		: %#x\n", hdr.division);
    
    fclose(f);
}

/*
 <Header Chunk>
 "MThd" 4 bytes
 the literal string MThd, or in hexadecimal notation: 0x4d546864. These four
 characters at the start of the MIDI file indicate that this is a MIDI file.
 
 <header_length> 4 bytes
 length of the header chunk (always 6 bytes long--the size of the next three
 fields which are considered the header chunk).
 
 <format> 2 bytes
 0 = single track file format
 1 = multiple track file format
 2 = multiple song file format (i.e., a series of type 0 files)
 
 <n> 2 bytes
 number of track chunks that follow the header chunk
 
 <division> 2 bytes
 unit of time for delta timing. If the value is positive, then it represents the
 units per beat. For example, +96 would mean 96 ticks per beat. If the value is
 negative, delta times are in SMPTE compatible units.
 */

#endif /* AudioOut_hpp */
