/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SimpleFlangerAudioProcessorEditor::SimpleFlangerAudioProcessorEditor (SimpleFlangerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    depthSlider.setSliderStyle (Slider::LinearVertical);
    depthSlider.setRange(0.0, 750, 5);
    depthSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    depthSlider.setPopupDisplayEnabled (false, false, this);
    depthSlider.setTextValueSuffix (" Depth");
    depthSlider.setValue(100);

    rateSlider.setSliderStyle (Slider::LinearVertical);
    rateSlider.setRange(0.0, 10, .02);
    rateSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    rateSlider.setPopupDisplayEnabled (false, false, this);
    rateSlider.setTextValueSuffix (" Rate (Hz)");
    rateSlider.setValue(1.0);

    addAndMakeVisible(&depthSlider);
    addAndMakeVisible(&rateSlider);

    depthSlider.addListener(this);
    rateSlider.addListener(this);
}

SimpleFlangerAudioProcessorEditor::~SimpleFlangerAudioProcessorEditor()
{
}

//==============================================================================
void SimpleFlangerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Simple Flanger", getLocalBounds(), Justification::centred, 1);
}

void SimpleFlangerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    depthSlider.setBounds(40, 30, 100, getHeight() - 60);
    rateSlider.setBounds(getWidth() - 140, 30, 100, getHeight() - 60);
}

void SimpleFlangerAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    double depth = depthSlider.getValue();
    double rate = rateSlider.getValue();

    processor.setParams(depth, rate);
}
