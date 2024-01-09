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
    m_chorusVoice(2, 2, 48000),
    m_lfo([](float x) { return std::sinf(x); }, 128),
    m_apvts(*this, nullptr, "Parameters", CreateParameterLayout())
#endif
{
    for (int i{}; i < 2; i++)
    {
        m_delayLines.push_back(CircularBuffer(2, 48000));
    }

    m_lfo.setFrequency(0.05f);
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
    juce::dsp::ProcessSpec spec{ sampleRate, samplesPerBlock, getTotalNumOutputChannels() };
    
    m_chorusVoice.Prepare(spec, sampleRate * 0.5);
    
    for (auto& delayLine : m_delayLines)
    {
        delayLine.SetDelay(sampleRate * 2);

    }
    m_lfo.prepare(spec);
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
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    PluginSettings settings{ GetPluginSettings(m_apvts) };
    m_lfo.setFrequency(settings.rate);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        for (int i{ 0 }; i < buffer.getNumSamples(); ++i)
        {
#pragma ignore
            //float bufferSample{ buffer.getSample(channel, i) };
            //m_chorusVoice.Update(channel, 
            //                     settings.delayInSeconds, 
            //                     getSampleRate(), 
            //                     settings.rate, settings.depth);
            //m_chorusVoice.Write(channel, bufferSample);
            //float delayedSample{ m_chorusVoice.Read(channel) };
            //float wetDryMix{ std::clamp((delayedSample * settings.wet) + (bufferSample * settings.dry), -1.f, 1.f) };
            //buffer.setSample(channel, i, wetDryMix);
#pragma endregion

            float lfoOut{ m_lfo.processSample(0.0f) };
            float mod{ juce::jmap(lfoOut, -1.0f, 1.0f, 0.0001f, static_cast<float>(settings.delayInSeconds * getSampleRate()) * settings.depth ) };

            float bufferSample{ buffer.getSample(channel, i) };
            m_delayLines[channel].Write(bufferSample);
            m_delayLines[channel].SetDelay((settings.delayInSeconds * getSampleRate()) - mod);
            float delayedSample{ m_delayLines[channel].Read()};

            float wetDryMix{ std::clamp((delayedSample * settings.wet) + (bufferSample * settings.dry), -1.f, 1.f) };
            buffer.setSample(channel, i, wetDryMix);
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
    //return new CustomChorusAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor (*this);
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

juce::AudioProcessorValueTreeState* CustomChorusAudioProcessor::GetAPVTS()
{
    return &m_apvts;
}

juce::AudioProcessorValueTreeState::ParameterLayout CustomChorusAudioProcessor::CreateParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout{};

    //layout.add(std::make_unique<juce::AudioParameterInt>("delay", "Delay", 1, 48000, static_cast<int>(48000)));
    layout.add(std::make_unique<juce::AudioParameterFloat>("delaySec", "Delay", 0.15f, 0.35f, 0.20f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("rate", "Rate", 0.0f, 3.0f, 0.15));
    layout.add(std::make_unique<juce::AudioParameterFloat>("depth", "Depth", 0.0f, 1.0f, 0.25f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("dry", "Dry", 0.0f, 1.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("wet", "Wet", 0.0f, 1.0f, 1.0f));

    return layout;
}

PluginSettings CustomChorusAudioProcessor::GetPluginSettings(const juce::AudioProcessorValueTreeState& apvts)
{
    PluginSettings settings;

    settings.delayInSeconds = apvts.getRawParameterValue("delaySec")->load();
    settings.rate = apvts.getRawParameterValue("rate")->load();
    settings.depth = apvts.getRawParameterValue("depth")->load();
    settings.dry = apvts.getRawParameterValue("dry")->load();
    settings.wet = apvts.getRawParameterValue("wet")->load();

    return settings;
}