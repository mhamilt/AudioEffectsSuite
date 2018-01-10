//
//  AudioWavFileReader.cpp
//  FDTD_C_Plate
//
//  Created by admin on 15/12/2017.
//  Copyright © 2017 admin. All rights reserved.
//

#include "AudioWavFileReadWrite.hpp"

//==============================================================================
AudioWavFileReadWrite::~AudioWavFileReadWrite(){/*free(&wavWriteFileHeader)*/}

//==============================================================================
int AudioWavFileReadWrite::getSampleRate (){return wavReadFileSampRate;};
//==============================================================================
void AudioWavFileReadWrite::printWavHeader(const char *filename)
{
	FILE *f;
	waveFormatHeader hdr;
	
	f = fopen(filename, "rb");

	if (!f)
	{
		printf("NO FILE FOUND\n");
	}
	else
	{
		fread(&hdr, 1, sizeof(hdr), f);
		
		printf("--- .WAV HEADER --- \n\n");
		printf("Chunk ID		: %s\n", hdr.chunkID);
		printf("Chunk Size		: %u\n", hdr.chunkSize);
		printf("Format			: %s\n", hdr.format);
		printf("Sub-Chunk 1		: %s\n", hdr.subChunk1ID);
		printf("Sub-Chunk 1 size: %d\n", hdr.subChunk1Size);
		printf("Audio Format	: %d\n", hdr.audioFormat);
		printf("Num Channels	: %d\n", hdr.numChannels);
		printf("Sample Rate		: %u\n", hdr.sampleRate);
		printf("Byte Rate		: %u\n", hdr.byteRate);
		printf("Block Align		: %u\n", hdr.blockAlign);
		printf("Bits Per Samp	: %u\n", hdr.bitsPerSample);
		printf("Sub-Chunk 2		: %s\n", hdr.subChunk2ID);
		printf("Sub-Chunk 2 size: %u\n", hdr.subChunk2Size);
		printf("SampsPerChan	: %u\n", hdr.subChunk2Size*8/(hdr.numChannels*hdr.bitsPerSample));
		printf("\n");
		fclose(f);
	}
}

//==============================================================================

//	virtual const char* cmdLineSetFilename(const char *cmdLineArgV[]) = 0;

//==============================================================================

void AudioWavFileReadWrite::writeWavSS(double **audioData, const char outputFile[], int numberOfFrames, double sampleRate)
{
	normaliseStereoBuffer(audioData[0], audioData[1] ,numberOfFrames);
	FILE * file;
	file = fopen(outputFile, "w");
	stereo16bitWaveHeaderForLength(numberOfFrames,sampleRate);
	writeWaveHeaderToFile(file);
	int16_t sdata;
	const double amp = 32000.0;
	for(int i=0;i<numberOfFrames;i++)
	{
		sdata = (int16_t)(audioData[0][i]*amp);
		fwrite(&sdata, sizeof(int16_t), 1, file); //left channel
		sdata = (int16_t)(audioData[1][i]*amp);
		fwrite(&sdata, sizeof(int16_t), 1, file); //right channel
	}
	fclose(file);
	printf("%d samples written to %s\n", numberOfFrames*2,outputFile);
	
	//delete &wavWriteFileHeader; //TODO: memory management for header
}

//==============================================================================
bool AudioWavFileReadWrite::checkHeader(waveFormatHeader fileHeader)
{
	
	if ((strncmp(&fileHeader.chunkID[0], "RIFF", 4))	 ||
		(strncmp(&fileHeader.format[0],  "WAVE", 4))     ||
		(strncmp(&fileHeader.subChunk1ID[0], "fmt ", 4)) ||
		(strncmp(&fileHeader.subChunk2ID[0], "data", 4)))
	{
		return false;
	}
	else {return true;}
}
//==============================================================================

double* AudioWavFileReadWrite::readWav(const char *filename, int *sampsPerChan, int *sampleRate)
{
	FILE *f;
	f = fopen(filename, "rb");
	if (!f){return NULL;}
	fread(&wavReadFileHeader, 1, sizeof(wavReadFileHeader), f);
	
	if(!checkHeader(wavReadFileHeader))
	{
		fclose(f);
		printf("NOT A WAV FILE\n");
		return NULL;
	}
	
	if ((wavReadFileHeader.bitsPerSample != 16))
	{
		fclose(f);
		printf("FILE NOT 16-BIT\n");
		return NULL;
	}
	
	const int totalSamples = wavReadFileHeader.subChunk2Size * 8 /(wavReadFileHeader.bitsPerSample);
	*sampsPerChan	 = totalSamples/(wavReadFileHeader.numChannels);
	printf("Length: %d\tSamples: %d \n",totalSamples,*sampsPerChan);
	
	//int16_t as it is currently strictly 16 bit audio
	int16_t *buf = new int16_t[totalSamples];
	
	if (!buf)
	{
		fclose(f);
		return NULL;
	}
	
	fread(buf, wavReadFileHeader.subChunk2Size, 1, f);
	fclose(f);
	
	double *data = new double[*sampsPerChan];
	if (!data)
	{
		delete[] buf;
		return NULL;
	}
	
	// multiplying by this will scale a .wav to between -1 and 1
	const double wavbitscale = 2/pow(2,wavReadFileHeader.bitsPerSample);
	
	int sampleIndex = 0;
	for (int i = 0; i < (*sampsPerChan); i++)
	{
		for (int j = 0; j < wavReadFileHeader.numChannels; j++)
		{
			if (j==0)
			{
				data[i] = ((double)buf[sampleIndex]) * wavbitscale;
			}
			sampleIndex++;
		}
	}
	
	printf("%d samples read from %s\n",*sampsPerChan,filename);
	
	delete[] buf;
	*sampleRate = wavReadFileHeader.sampleRate;
	return data;
}

//==============================================================================

double** AudioWavFileReadWrite::readStereoWav(const char *filename, int *sampsPerChan, int *sampleRate)
{
	FILE *f;
	f = fopen(filename, "rb");
	if (!f)
	{
		return NULL;
	}
	
	fread(&wavReadFileHeader, sizeof(wavReadFileHeader), 1 , f);
	
	if(!checkHeader(wavReadFileHeader))
	{
		fclose(f);
		printf("NOT A WAV FILE\n");
		return NULL;
	}
	
	if ((wavReadFileHeader.bitsPerSample != 16))
	{
		fclose(f);
		return NULL;
	}
	
	if ((wavReadFileHeader.numChannels != 2))
	{
		fclose(f);
		return NULL;
	}
	
	const int totalSamples = wavReadFileHeader.subChunk2Size * 8 /(wavReadFileHeader.bitsPerSample);
	*sampsPerChan	 = totalSamples/(wavReadFileHeader.numChannels);
	
	short *buf = new short[totalSamples];
	if (!buf)
	{
		fclose(f);
		return NULL;
	}
	fread(buf, wavReadFileHeader.subChunk2Size, 1, f);
	fclose(f);
	
	double** data = new double*[wavReadFileHeader.numChannels];
	if (!data)
	{
		delete[] buf;
		return NULL;
	}
	
	for(int i = 0; i < wavReadFileHeader.numChannels; ++i)
	{
		data[i] = new double[*sampsPerChan];
	}
	
	const double wavbitscale = 2/pow(2,wavReadFileHeader.bitsPerSample);
	int sampleIndex = 0;
	for (int i = 0; i < (*sampsPerChan); i++)
	{
		for (int j = 0; j < wavReadFileHeader.numChannels; j++){
			data[j][i] = ((double)buf[sampleIndex]) * wavbitscale;
			sampleIndex++;
		}
	}
	
	printf("%d samples read from %s\n",totalSamples,filename);
	
	delete[] buf;
	*sampleRate = wavReadFileHeader.sampleRate;
	return data;
}
//==============================================================================

void AudioWavFileReadWrite::setBasicHeader()
{
	wavWriteFileHeader = *new waveFormatHeader;
	memcpy(wavWriteFileHeader.chunkID, &"RIFF", 4);
	memcpy(wavWriteFileHeader.format, &"WAVE", 4);
	memcpy(wavWriteFileHeader.subChunk1ID, &"fmt ", 4); //notice the space at the end!
	wavWriteFileHeader.subChunk1Size = 16;
}
//==============================================================================

void AudioWavFileReadWrite::stereo16bitWaveHeader(double sampleRate)
{
	setBasicHeader();
	wavWriteFileHeader.audioFormat = 1;
	wavWriteFileHeader.numChannels = 2;
	wavWriteFileHeader.sampleRate = (uint32_t)sampleRate;
	wavWriteFileHeader.bitsPerSample = 16;
	wavWriteFileHeader.byteRate = wavWriteFileHeader.numChannels * wavWriteFileHeader.sampleRate * wavWriteFileHeader.bitsPerSample/8;
	wavWriteFileHeader.blockAlign = wavWriteFileHeader.numChannels * wavWriteFileHeader.bitsPerSample/8;
	memcpy(wavWriteFileHeader.subChunk2ID, &"data", 4);
}
//==============================================================================
void AudioWavFileReadWrite::mono16bitWaveHeader(double sampleRate)
{
	setBasicHeader();
	wavWriteFileHeader.audioFormat = 1;
	wavWriteFileHeader.numChannels = 1;
	wavWriteFileHeader.sampleRate = (uint32_t)sampleRate;
	wavWriteFileHeader.bitsPerSample = 16;
	wavWriteFileHeader.byteRate = wavWriteFileHeader.numChannels * wavWriteFileHeader.sampleRate * wavWriteFileHeader.bitsPerSample/8;
	wavWriteFileHeader.blockAlign = wavWriteFileHeader.numChannels * wavWriteFileHeader.bitsPerSample/8;
	memcpy(wavWriteFileHeader.subChunk2ID, &"data", 4);
}
//==============================================================================
void AudioWavFileReadWrite::setLengthForWaveFormatHeader(size_t numberOfFrames)
{
	wavWriteFileHeader.subChunk2Size = (uint32_t)numberOfFrames * wavWriteFileHeader.numChannels * wavWriteFileHeader.bitsPerSample/8;
	wavWriteFileHeader.chunkSize = 36 + wavWriteFileHeader.subChunk2Size;
}
//==============================================================================
void AudioWavFileReadWrite::stereo16bitWaveHeaderForLength(size_t numberOfFrames,double sampleRate)
{
	stereo16bitWaveHeader(sampleRate);
	setLengthForWaveFormatHeader(numberOfFrames);
}
//==============================================================================
void AudioWavFileReadWrite::mono16bitWaveHeaderForLength(size_t numberOfFrames,double sampleRate)
{
	mono16bitWaveHeader(sampleRate);
	setLengthForWaveFormatHeader(numberOfFrames);
}
//==============================================================================

void AudioWavFileReadWrite::normaliseBuffer(double *audioData, int numberOfFrames)
{
	double temp;
	double maxy = 0.0; // Find max abs sample
	
	for(int n=0;n<numberOfFrames;n++)
	{
		if(fabs(audioData[n])>maxy) maxy = fabs(audioData[n]);
	}
	
	// Normalise
	if(maxy>0.00001)
	{
		for(int n=0;n<numberOfFrames;n++)
		{
			temp     = audioData[n];
			audioData[n] = temp/maxy;
		}
	}
	
	// Smooth last 500 samples
	if(numberOfFrames>501)
	{
		double inc  = 1.0/500.0;
		double ramp = 1.0;
		for(int n=numberOfFrames-501;n<numberOfFrames;n++)
		{
			audioData[n] *= ramp;
			if(ramp>0) ramp-=inc;
		}
	}
	
	printf("Normalised by : %.5f\n", maxy);
}

//==============================================================================

void AudioWavFileReadWrite::normaliseStereoBuffer(double *audioL, double *audioR, int numberOfFrames)
{
	double temp;
	double maxy = 0.0; // Find max abs sample
	
	for(int n=0;n<numberOfFrames;n++)
	{
		if(fabs(audioL[n])>maxy) maxy = fabs(audioL[n]);
		if(fabs(audioR[n])>maxy) maxy = fabs(audioR[n]);
		
	}
	
	// Normalise
	if(maxy>0.00001)
	{
		for(int n=0;n<numberOfFrames;n++)
		{
			temp      = audioL[n];
			audioL[n]  = temp/maxy;
			temp      = audioR[n];
			audioR[n] = temp/maxy;
		}
	}
	
	// Smooth last 500 samples
	if(numberOfFrames>501)
	{
		double inc  = 1.0/500.0;
		double ramp = 1.0;
		for(int n=numberOfFrames-501;n<numberOfFrames;n++)
		{
			audioL[n] *= ramp;
			audioR[n] *= ramp;
			if(ramp>0) ramp-=inc;
		}
	}
	
	printf("Normalised by : %.5f\n", maxy);
}
//==============================================================================

size_t AudioWavFileReadWrite::writeWaveHeaderToFile(FILE * file)
{
	return fwrite(&wavWriteFileHeader, sizeof(waveFormatHeader), 1, file);
}

void AudioWavFileReadWrite::writeWavMS(double* audio,const char outputFile[], int numberOfFrames, double sampleRate)
{
	
	normaliseBuffer(audio ,numberOfFrames);
	
	FILE * file;
	file = fopen(outputFile, "w");
	mono16bitWaveHeaderForLength(numberOfFrames,sampleRate);
	writeWaveHeaderToFile(file);
	int16_t sdata;
	const double amp = 32000.0;
	
	for(int i=0;i<numberOfFrames;i++)
	{
		sdata = (int16_t)(audio[i]*amp);  //set sdata to PCM 16-bit
		fwrite(&sdata, sizeof(int16_t), 1, file);
	}
	fclose(file);
	printf("%d samples written to %s\n", numberOfFrames,outputFile);
}

//==============================================================================
double** AudioWavFileReadWrite::whiteNoise(int sampsPerChan, int sampleRate)
{
	const double lo = -1.;
	const double hi =  1.;
	double** data = new double*[2];
	
	for(int i = 0; i < 2; ++i)
	{
		data[i] = new double[sampsPerChan];
	}

	for(int i = 0; i < sampsPerChan; ++i)
	{
		data[0][i] = lo + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(hi-lo)));
		data[1][i] = lo + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(hi-lo)));
	}
	return data;
}

//EOF

