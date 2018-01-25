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

    this->depthSlider.setSliderStyle (Slider::LinearVertical);
    this->depthSlider.setRange(0.0, 750, 5);
    this->depthSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    this->depthSlider.setPopupDisplayEnabled (false, false, this);
    this->depthSlider.setTextValueSuffix (" Depth");
    this->depthSlider.setValue(100);

    this->rateSlider.setSliderStyle (Slider::LinearVertical);
    this->rateSlider.setRange(0.0, 10, .02);
    this->rateSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    this->rateSlider.setPopupDisplayEnabled (false, false, this);
    this->rateSlider.setTextValueSuffix (" Rate (Hz)");
    this->rateSlider.setValue(1.0);

    addAndMakeVisible(&this->depthSlider);
    addAndMakeVisible(&this->rateSlider);

    this->depthSlider.addListener(this);
    this->rateSlider.addListener(this);
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
    this->depthSlider.setBounds(40, 30, 100, getHeight() - 60);
    this->rateSlider.setBounds(getWidth() - 140, 30, 100, getHeight() - 60);
}

void SimpleFlangerAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    double depth = this->depthSlider.getValue();
    double rate = this->rateSlider.getValue();

    this->processor.setParams(depth, rate);
}
