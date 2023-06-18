/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin editor.
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


// ChatGDP
class CustomSlider : public juce::Slider
{
public:
    CustomSlider()
    {
        // Customize the appearance and behavior of the slider here
        
        setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxAbove, true, 50, 30);
        setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::red);
        //setRange(-24, 24, 0.1); 
    }

    ~CustomSlider()
    {
    }

    // Add any additional member functions or overrides as needed
    void SetWaveZoomSlider()
    {
        setRange(256, 1024, 1);
    }
};

class CustomButton : public juce::ToggleButton
{
public:
    CustomButton()
    {
        setColour(juce::ToggleButton::ColourIds::tickColourId, juce::Colours::white);
        setButtonText("Loudnes compensation");
    }

    ~CustomButton()
    {
    }

    // Add any additional member functions or overrides as needed
};

class BackGroundText : public juce::Component
{
public:
    BackGroundText()
    {
        backgroundText = "xxxxxx";
    };
    ~BackGroundText() {};

    void paint(juce::Graphics& g) override 
    {
        // Fill the background with a color or image
        // You can customize the appearance as per your requirements
        // g.fillAll(juce::Colours::white);

        // Draw the text on the background
        g.setColour(juce::Colours::ghostwhite);
        g.setFont(25.0f);
        g.drawText(backgroundText, getLocalBounds(), juce::Justification::centred, true);
    };
    void SetText(auto Text)
    {
        backgroundText = Text;
    }

    

private:
    juce::String backgroundText;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BackGroundText)
};
//class VolumeMeterComponent : public juce::Component
//{
//public:
//    VolumeMeterComponent();
//    ~VolumeMeterComponent();
//    void paint(juce::Graphics& g) override;
//    void setVolume(float volume); // Method to set the volume level    
//private:
//    float currentVolume;

//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VolumeMeterComponent)
//};
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
    
    CustomButton LinkButton;
    BackGroundText DistortionText, OutputVolumeText;
                                      

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GeneralPluginAudioProcessorEditor)
};