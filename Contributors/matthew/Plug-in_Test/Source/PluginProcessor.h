/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../../../BaseClasses/BaseClasses.h"
#include "../../../../DelayEffects/DelayEffects.h"
#include "../../../../FilterEffects/FilterEffects.h"
//==============================================================================
/**
*/
class Plugin_testAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Plugin_testAudioProcessor();
    ~Plugin_testAudioProcessor();
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect () const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    
private:
    //==============================================================================
//    ModulationBaseClass ampTremolo;
//    SimpleChorus chorusEffect;
//    SimpleDelay delayEffect;
      SimpleFlanger flangerEffect;
public:
    double tremRate = 0; // rate of tremolo in Hz [changed by slider]
    double gain     = 0; // gain of signal
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugin_testAudioProcessor)
};
