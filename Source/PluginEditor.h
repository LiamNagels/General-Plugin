/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin editor.
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//struct CustomSlider : juce::Slider
//{
//    CustomSlider() : juce::Slider(juce::Slider::SliderStyle::LinearHorizontal, 
//                                  juce::Slider::TextEntryBoxPosition::TextBoxAbove)
//    {
//       
//    }
//};

// ChatGDP
class CustomSlider : public juce::Slider
{
public:
    CustomSlider()
    {
        // Customize the appearance and behavior of the slider here
        // For example, you can set the slider style, color, range, etc.
        setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, true, 50, 30);
        setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::red);
        setRange(-24, 24, 0.1); // Set the range and increment value
    }

    ~CustomSlider()
    {
    }

    // Add any additional member functions or overrides as needed
    void BecomeWaveZoomSlider()
    {
        setRange(256, 1024, 1);
    }
};


//==============================================================================
/**
*/
class GeneralPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    GeneralPluginAudioProcessorEditor(GeneralPluginAudioProcessor&);
    ~GeneralPluginAudioProcessorEditor() override;
    
    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GeneralPluginAudioProcessor& audioProcessor;
    
    CustomSlider DistortionSlider, OutputSlider, WaveZoom;
    using TREESTATE = juce::AudioProcessorValueTreeState;
    using Attachments = TREESTATE::SliderAttachment;
    Attachments DistortionAttachment, OutputAttachment;
    
    
    
                                      

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GeneralPluginAudioProcessorEditor)
};