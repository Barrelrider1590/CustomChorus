/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CustomChorusAudioProcessor::CustomChorusAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    m_delayBufferSize(0),
    m_delayBuffer(getTotalNumOutputChannels()),
    m_delayInSamples(0)
#endif
{
}

CustomChorusAudioProcessor::~CustomChorusAudioProcessor()
{
}

//==============================================================================
const juce::String CustomChorusAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CustomChorusAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CustomChorusAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CustomChorusAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CustomChorusAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CustomChorusAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CustomChorusAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CustomChorusAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String CustomChorusAudioProcessor::getProgramName (int index)
{
    return {};
}

void CustomChorusAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void CustomChorusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    m_delayInSamples = sampleRate / 2;
    m_delayBufferSize = sampleRate;
    m_delayBuffer.setSize(m_delayBufferSize);
    m_delayBuffer.setDelay(m_delayInSamples);
}

void CustomChorusAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CustomChorusAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
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

void CustomChorusAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        for (int i{ 0 }; i < buffer.getNumSamples(); ++i)
        {
            float bufferSample{ buffer.getSample(channel, i) };
            m_delayBuffer.write(channel, bufferSample);
            float delaySample{ m_delayBuffer.read(channel) };
            float delayedSample{ std::clamp(delaySample + bufferSample, -1.f, 1.f) };
            buffer.setSample(channel, i, delayedSample);
        }
    }
}

//==============================================================================
bool CustomChorusAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CustomChorusAudioProcessor::createEditor()
{
    return new CustomChorusAudioProcessorEditor (*this);
}

//==============================================================================
void CustomChorusAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void CustomChorusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CustomChorusAudioProcessor();
}