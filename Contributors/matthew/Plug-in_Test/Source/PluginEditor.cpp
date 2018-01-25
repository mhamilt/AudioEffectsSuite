/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Plugin_testAudioProcessorEditor::Plugin_testAudioProcessorEditor (Plugin_testAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    //==========================================================================
    //  Sliders
    //==========================================================================
    
    tremoloRate.setRange(0.0, 200, .001); // 1046.50 Hz == C6
    tremoloRate.setTextBoxStyle (Slider::TextBoxAbove, false, 90, 30);
    tremoloRate.setValue(processor.tremRate);
    tremoloRate.addListener (this);
    addAndMakeVisible (tremoloRate);
    
    gain.setRange(0.0, 1., .001);
    gain.setTextBoxStyle (Slider::TextBoxBelow, false, 90, 30);
    gain.setValue(processor.gain);
    gain.addListener (this);
    addAndMakeVisible (gain);
}


Plugin_testAudioProcessorEditor::~Plugin_testAudioProcessorEditor()
{
}

//==============================================================================
void Plugin_testAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Tremolo Rate (Hz)", 0, 0, getWidth(), 30, Justification::centred, 1);
}

void Plugin_testAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    //==========================================================================
    //  Sliders
    //==========================================================================
    tremoloRate.setBounds (10, 30, getWidth() - 30, 50);
    gain.setBounds(10, 100, getWidth() - 30, 50);
}

//==============================================================================

void Plugin_testAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
//    if (slider == &tremoloRate)
//        processor.flangerEffect.setDepth(tremoloRate.getValue());
//    if (slider == &gain)
//        processor.gain = gain.getValue();
}
