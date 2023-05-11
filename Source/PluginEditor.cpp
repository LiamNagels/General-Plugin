/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GeneralPluginAudioProcessorEditor::GeneralPluginAudioProcessorEditor(GeneralPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    for (auto* comps : getComps())
    {
        addAndMakeVisible(comps);
    }
    addAndMakeVisible(audioProcessor.waveViewer);
    setSize(600, 400);
}


GeneralPluginAudioProcessorEditor::~GeneralPluginAudioProcessorEditor()
{
}

//==============================================================================
void GeneralPluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    //g.drawFittedText("Hello ISP!", getLocalBounds(), juce::Justification::centred, 1);
}

void GeneralPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    //audioProcessor.waveViewer.setBounds(getLocalBounds().withSizeKeepingCentre(getWidth() * 0.5, getHeight() * 0.5));
    auto bounds = getLocalBounds();
    auto responseArea = bounds.removeFromTop(bounds.getHeight() / 2);
    auto OutputSliderArea = bounds.removeFromBottom(bounds.getHeight() / 2);
    auto InputSliderArea = OutputSliderArea.removeFromLeft(OutputSliderArea.getWidth()/2);
    
    audioProcessor.waveViewer.setBounds(responseArea);

    InputSlider.setBounds(InputSliderArea);
    OutputSlider.setBounds(OutputSliderArea);
}

std::vector<juce::Component*> GeneralPluginAudioProcessorEditor::getComps()
{
    return
    {
        &InputSlider,
        &OutputSlider
    };
}