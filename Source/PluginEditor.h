/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin editor.
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

struct CustomSlider : juce::Slider
{
    CustomSlider() : juce::Slider(juce::Slider::SliderStyle::LinearHorizontal,
        juce::Slider::TextEntryBoxPosition::TextBoxAbove)
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

    CustomSlider InputSlider, OutputSlider;

    std::vector<juce::Component*> getComps();


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GeneralPluginAudioProcessorEditor)
};