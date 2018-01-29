/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../../../BaseClasses/BaseClasses.h"
#include "../../../../DelayEffects/DelayEffects.h"
#include "../../../../FilterEffects/FilterEffects.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent,
private Timer,
                               public Slider::Listener
{
public:
    //==============================================================================
    MainContentComponent()
    {
//        setFullScreen(true);
//        setSize(getWidth(), getHeight());
//        ResizableWindow::setFullScreen();
        startTimerHz (30);
        setSize(400, 400);
        otherLookAndFeel.setColour(Slider::thumbColourId, Colour (0, 0, 0));
        
        addAndMakeVisible (frequency);
//        frequency.setSliderStyle(Slider::LinearBarVertical);
        frequency.setSliderStyle(Slider::LinearVertical);
        frequency.setRange (0, 1000.0);
        frequency.setTextBoxStyle (Slider::TextBoxBelow, false, 100, 40);
//        frequency.setTextValueSuffix("Hz");
        frequency.setLookAndFeel(&otherLookAndFeel);
        frequency.setSkewFactorFromMidPoint (500.0); // [4]
        frequency.addListener (this);
        // specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }
    
    //==============================================================================
    
    void sliderValueChanged (Slider* slider) override
    {
        if (slider == &frequency)
        {
            const float normSlide = (frequency.getValue()*0.001);
            const uint8 colouroffset = normSlide*255;
            otherLookAndFeel.setColour(Slider::thumbColourId, Colour(colouroffset,colouroffset,colouroffset));
            frequency.setLookAndFeel(&otherLookAndFeel);
            freq = frequency.getValue();
            rotSpeed = normSlide*0.05;
        }
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()
        
        
        waveTab.setupModulationBaseClass(sampleRate);
        waveTab.setSawtooth();
        
        delay.setupSimpleDelay(20000);
        delay.setFeedbackGain(.5);
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!
        // For more details, see the help for AudioProcessor::getNextAudioBlock()

        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)
        bufferToFill.clearActiveBufferRegion();
        float* const buffer = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);

        for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            buffer[sample] = delay.process(waveTab.readTable(freq) *.25);
            
        }
        
        for (int channel = 1; channel < bufferToFill.buffer->getNumChannels(); ++channel)
            bufferToFill.buffer->copyFrom(channel, 0, buffer, bufferToFill.numSamples);
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        rot += rotSpeed;
        if (rot>(2*double_Pi)) {
            rot=0;
        }

        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
        
        AffineTransform houseTransform;
        
        houseTransform = houseTransform.rotated(rot).translated (rectX,rectY);

        g.setColour (Colours::white);
        g.addTransform(houseTransform);
        
        g.fillRect(house);
        g.setFont (15.0f);
//
//        const int rectSize = jmin (getWidth(), getHeight()) / 2 - 200;
//        g.setOpacity (getAlpha());
//        g.drawRect (-100, -100, rectSize, rectSize, 5);
    }
    
    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
        frequency.setBounds ((getWidth()/2)-40, 100, 80, 400);
//        frequency.setBounds(<#int x#>, <#int y#>, <#int width#>, <#int height#>)
    }
    
    void timerCallback() override
    {
        repaint();
    }
    

private:
    //==============================================================================

    // Your private member variables go here...
    float rectX = 100;
    float rectY = 100;
    
    float rectRad = sqrt((rectX)*(rectX) + (rectY)*(rectY));
    float rectTheta = atan(rectY/rectX);
    float deltaX = 0;
    float deltaY = 0;
    
    Rectangle<float> house = Rectangle<float> (-50,-50,100,100);
    
    float rot = .25;
    float rotSpeed =0.;

    ModulationBaseClass waveTab;
    SimpleDelay delay;
    Slider frequency;
    LookAndFeel_V4 otherLookAndFeel;
    const double colourNorm = 255./log2(1000);
    double angleDelta = 2*double_Pi*200/44100.;
    double angle = 0;
    double freq = 0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
