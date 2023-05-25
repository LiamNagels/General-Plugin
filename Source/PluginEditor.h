/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"



struct CustomHorizontalSlider : juce::Slider
{
    CustomHorizontalSlider() : juce::Slider(juce::Slider::SliderStyle::LinearHorizontal,
                               juce::Slider::TextEntryBoxPosition::TextBoxLeft
                                )
    {
       
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

    CustomHorizontalSlider InputSlider, OutputSlider, WaveZoom;

    std::vector<juce::Component*> getComps();

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;
    Attachment InInputSlider, OutputSlider, WaveZoom;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GeneralPluginAudioProcessorEditor)
};