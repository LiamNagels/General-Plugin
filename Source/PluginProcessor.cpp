/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin processor.
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GeneralPluginAudioProcessor::GeneralPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
    , treestate(*this, nullptr, "PARAMETER", createParameterLayout())
    , waveViewer(1)
#endif
{
    treestate.addParameterListener("input", this);
    treestate.addParameterListener("output", this);
    waveViewer.setRepaintRate(60);
    waveViewer.setBufferSize(256);
}

GeneralPluginAudioProcessor::~GeneralPluginAudioProcessor()
{
    treestate.removeParameterListener("input", this);
    treestate.removeParameterListener("output", this);
}

juce::AudioProcessorValueTreeState::ParameterLayout GeneralPluginAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

    auto pInput = std::make_unique<juce::AudioParameterFloat>("input", "Input", -24.f, 24.f, 0.f);
    auto pOutput = std::make_unique<juce::AudioParameterFloat>("output", "Output", -24.f, 24.f, 0.f);

    params.push_back(std::move(pInput));
    params.push_back(std::move(pOutput));

    return{ params.begin(), params.end() };
}

void GeneralPluginAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    UpdateParameters();
}

void GeneralPluginAudioProcessor::UpdateParameters()
{
    inputModule.setGainDecibels(treestate.getRawParameterValue("input")->load());
    outputModule.setGainDecibels(treestate.getRawParameterValue("output")->load());
}
//==============================================================================
const juce::String GeneralPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GeneralPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool GeneralPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool GeneralPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double GeneralPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GeneralPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int GeneralPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GeneralPluginAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String GeneralPluginAudioProcessor::getProgramName(int index)
{
    return {};
}

void GeneralPluginAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void GeneralPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // initialize spec for dsp module
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();

    inputModule.prepare(spec);
    inputModule.setRampDurationSeconds(0.02f);

    outputModule.prepare(spec);
    outputModule.setRampDurationSeconds(0.02f);


    distortionModule.prepare(spec);
    limiterModule.prepare(spec);
    limiterModule.setThreshold(0.99);
    limiterModule.setThreshold(1.f);
    UpdateParameters();


}

void GeneralPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GeneralPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void GeneralPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    juce::dsp::AudioBlock<float> block{ buffer };

    inputModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    distortionModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    limiterModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    outputModule.process(juce::dsp::ProcessContextReplacing<float>(block));

    waveViewer.pushBuffer(buffer);


}

//==============================================================================
bool GeneralPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GeneralPluginAudioProcessor::createEditor()
{
    return new GeneralPluginAudioProcessorEditor(*this);
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void GeneralPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // save params
    juce::MemoryOutputStream stream(destData, false);
    treestate.state.writeToStream(stream);
}

void GeneralPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // recall params
    auto tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));

    if (tree.isValid())
    {
        treestate.state = tree;
    }
}



//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GeneralPluginAudioProcessor();
}