#ifndef AudioWavFileReadWrite_hpp
#define AudioWavFileReadWrite_hpp

#include <iostream>
#include <cmath>
#include <cstdint>
#include <fstream>

/*!
 @class AudioWavFileReadWrite
 @abstract
 A class with methods to read and write wav files.
 
 @discussion
	Currently this class only tackles 16-bit PCM audio. Sample values read from
	a wav file are normalised in a double type array between 1.0 and -1.0. Basic
	functionality to cover
 
 - Read wav file as a single stream
 - Read multi-channel files and store as an array of pointer (2D Array)
 - Read/Write Header files
 - Memory allocation of read file
 
	Currently memory allocation of written file is dealt with outside the class
	but will be expanded upon
 */
//==============================================================================
class AudioWavFileReadWrite
{
public: // Type definitions
	//==============================================================================
	/** Header format for a .wav file,
	 this will be filled in with the initial read of a .wav.
	 */
	struct waveFormatHeader {
        /**waveFormatHeader: The first 4 bytes of a wav file should be the characters "RIFF"
		 */
		char     chunkID[4];
		
		/**waveFormatHeader: This is the size of the entire file in bytes minus 8 bytes
		 */
		uint32_t chunkSize;
		
		/**waveFormatHeader" The should be characters "WAVE"
		 */
		char     format[4];
		
		/**waveFormatHeader" This should be the letters "fmt ", note the space character
		 */
		char     subChunk1ID[4];
		
		/**waveFormatHeader: For PCM == 16, since audioFormat == uint16_t
		 */
		uint32_t subChunk1Size;
		
		/**waveFormatHeader: For PCM this is 1, other values indicate compression
		 */
		uint16_t audioFormat;
		
		/**waveFormatHeader: Mono = 1, Stereo = 2, etc.
		 */
		uint16_t numChannels;
		
		/**waveFormatHeader: Sample Rate of file
		 */
		uint32_t sampleRate;
		
		/**waveFormatHeader: SampleRate * NumChannels * BitsPerSample/8
		 */
		uint32_t byteRate;
		
		/**waveFormatHeader: The number of bytes for one sample including all channels
		 */
		uint16_t blockAlign;
		
		/**waveFormatHeader: 8 bits = 8, 16 bits = 16
		 */
		uint16_t bitsPerSample;
		
		/**waveFormatHeader: Contains the letters "data"
		 */
		char     subChunk2ID[4];
		
		/**waveFormatHeader: == NumSamples * NumChannels * BitsPerSample/8
		 This is the number of bytes in the data.
		 */
		uint32_t subChunk2Size;
	};
 
public:
	/**
	 Constructor
	 */
	AudioWavFileReadWrite()
	{
		srand (static_cast <unsigned> (time(0)));
	};
	/**
	 Destructor
	 */
	~AudioWavFileReadWrite();
	//==============================================================================
	/** Retrieve the sampleRate of the read wav file
	 @returns   sample rate of file as an int or NULL if no file has been read.
	 */
	int getSampleRate ();
	
	//==============================================================================
	/** prints to terminal the wave file header details
		@param filename const character pointer to an array containing path and
	 filename of desired file.
	 */
	void printWavHeader(const char *filename);
	
	//==============================================================================
	/** sets a filename from a command line input
	 This will see if a valid file has been passed from command line and will
	 return either a the requested filename or a default filename
	 @param cmdLineArgV command line variables from main()
	 @returns a pointer to a const char array containing path and filename
	 */
	//	virtual const char* cmdLineSetFilename(const char *cmdLineArgV[]) = 0;
	
	//==============================================================================
	/** writes audio data to a mono wav file
		@param audio pointer to array of audio data
		@param outputFile character array of path and filename
		@param numberOfFrames number of frames to be written
		@param sampleRate sampling rate of file
		*/
	void writeWavMS(double* audio,const char outputFile[], int numberOfFrames, double sampleRate);
	
	/** writes audio data to a mono wav file
		@param audioData double pointer to a 2D array of audio data
	 audioData[channel][sample]
		@param outputFile character array of path and filename
		@param numberOfFrames number of frames to be written
		@param sampleRate sampling rate of file
		*/
	void writeWavSS(double **audioData, const char outputFile[], int numberOfFrames, double sampleRate);
	
	//==============================================================================
	
	/** Read in wav file as a mono file.
		Will read first channel only on multichannel files
		@param filename pointer to character array of path and filename
		@param sampsPerChan int pointer that will be set to number of samples per channel
		@param sampleRate int pointer sampling rate of file
		@returns a pointer to a array of type double or NULL on error
	 */
	double *readWav(const char *filename, int *sampsPerChan, int *sampleRate);
	
	/** Read in wav file as a mono file.
		Will read first channel only on multichannel files
		@param filename pointer to character array of path and filename
		@param sampsPerChan	int pointer that will be set to total number of samples
	 per channel
		@param sampleRate int pointer that is set to
	 sampling rate of read file
		@returns a double pointer to a 2D array of type double or NULL on error
		audioData[channel][sample]
	 */
	double **readStereoWav(const char *filename, int *sampsPerChan, int *sampleRate);
	
	/** creates a stereo buffer of white noise of specified length
	 
		@param sampsPerChan	int: number of samples for each channel
		@param sampleRate  int: sampling rate
		@returns a double pointer to a 2D array of type double or NULL on error
		audioData[channel][sample]
	 */
	double** whiteNoise(int sampsPerChan, int sampleRate);
	
private: // Methods
	//==============================================================================
	/** Sets a standard 16bit PCM .wav file header for write.
	 */
	void setBasicHeader();
	
	/** Fills in the gaps of basicHeader() to create a header for a
	 Stereo .wav file with PCM bit depth 16bit
	 @param sampleRate Sample Rate for writing file
	 */
	void stereo16bitWaveHeader(double sampleRate);
	
	/** Fill in the gaps of basicHeader() to create a header for a
	 Mono .wav file with PCM bit depth 16bit
	 @param sampleRate Sample Rate for writing file
	 
	 */
	void mono16bitWaveHeader(double sampleRate);
	
	/** For writing a wav file, sets the chunk size in the header
		@param numberOfFrames The number of sample frames as samples are interlaced
		*/
	void setLengthForWaveFormatHeader(size_t numberOfFrames);
	
	/** For writing a stereo wav file, sets the chunk size in the header
		@param numberOfFrames The number of sample frames as samples are interlaced
		@param sampleRate Sample Rate for writing file
		
		*/
	void stereo16bitWaveHeaderForLength(size_t numberOfFrames,double sampleRate);
	
	/** For writing a mono wav file, sets the chunk size in the header
		@param numberOfFrames The number of sample frames as samples are interlaced
		@param sampleRate Sample Rate for writing file
		
		*/
	void mono16bitWaveHeaderForLength(size_t numberOfFrames,double sampleRate);
	
	//==============================================================================
	
	/** Normalise an array of audio data to 1;
		@param audioData audio Data passed as a double
		@param numberOfFrames Number of samples per channel
		*/
	void normaliseBuffer(double *audioData, int numberOfFrames);
	
	/** Normalise two arrays of audio data representing a stereo signal;
		@param audioL audio data left channel
		@param audioR audio data right channel
		@param numberOfFrames number of samples per channel
		*/
	void normaliseStereoBuffer(double *audioL, double *audioR, int numberOfFrames);
	
	//==============================================================================
	/** writes the header to an open file
		@param file the file being written to
		@returns size_t of data written or zero on error
		*/
	size_t writeWaveHeaderToFile(FILE * file);
	
	//==============================================================================
	/** checks the file header to confirm if it is a wav file
		@params fileHeader the file header
		@returns true if it is a wav file or false if not
		*/
	bool checkHeader (waveFormatHeader fileHeader);
    /**
     Reads through open wav file and returns data scaled to between -1 and 1
     in array. Reads the first channel of the wav file only. Bit-depth agnostic,
     will parse data that is u8, 16, 24 or 32 bit.
     
     @param data pointer to array of audio data of type double to be filled
     @param f an open wav file where the header has been read
     @return returns a bool on sucess or false on failure.
     */
    bool parseWavMonoFile(double* data, FILE *f);
    /**
     Reads through an open wav file and save data to an array in a
     range of -1 to 1. Will parse u8, 16, 24 and 32 bit files.
     \see parseWavMonoFile
     
     @param data 2D array in format data[channel][sampleIndex]
     @param f open wav file to be read
     @return true on success, false on failure
     */
    bool parseWavFile(double** data, FILE *f);
	
private: // Variables
	/***/
	int wavReadFileSampRate;
    /***/
	int wavFileFrameNum;
    /***/
	waveFormatHeader wavReadFileHeader;
    /***/
	waveFormatHeader wavWriteFileHeader;
    /***/
	double *monoAudioData;
    /***/
	double *rightAudioData;
    /***/
	double *leftAudioData;
    /***/
	char* wavReadFilename;
};
#endif /* AudioWavFileReader_hpp */
