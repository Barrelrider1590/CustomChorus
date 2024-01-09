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
    m_apvts(*this, nullptr, "Parameters", CreateParameterLayout())
#endif
{
    for (int i{ 0 }; i < 4; i++)
    {
        m_chorusVoices.push_back(ChorusVoice(0.35f, 48000));
    }

    for (int i{ 0 }; i < 2; ++i)
    {
        m_stereoChorus.push_back(std::vector<ChorusVoice>());
    }

    for (auto& channel : m_stereoChorus)
    {
        for (int i{ 0 }; i < 4; ++i)
        {
            channel.push_back(ChorusVoice(0.35f, 48000));
        }   
    }
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

    PluginSettings settings{ GetPluginSettings(m_apvts) };

    for (auto& chorusVoice : m_chorusVoices)
    {
        chorusVoice.Prepare(spec, settings.delayInSeconds);
    }

    for (auto& channel : m_stereoChorus)
    {
        for (auto& chorusVoice : channel)
        {
            chorusVoice.Prepare(spec, settings.delayInSeconds);
        }
    }
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

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        m_chorusVoices[channel].SetFrequency(settings.rate);

        for (int i{ 0 }; i < 4; ++i)
        {
            m_stereoChorus[channel][i].SetFrequency(settings.rate);
        }

        for (int i{ 0 }; i < buffer.getNumSamples(); ++i)
        {
            //float bufferSample{ buffer.getSample(channel, i) };
            //float delayedSample{};
            //for (int i{ 0 }; i < 2; ++i)
            //{
            //    m_chorusVoices[i].Write(bufferSample);
            //    m_chorusVoices[i].Update(settings.delayInSeconds, settings.depth);
            //    delayedSample = m_chorusVoices[i].Read();
            //}
            //float wetDryMix{ std::clamp((delayedSample * settings.wet) + (bufferSample * settings.dry), -1.f, 1.f) };
            //buffer.setSample(channel, i, wetDryMix);
            
            float bufferSample{ buffer.getSample(channel, i) };
            float delayedSample{};
            for (int i{ 0 }; i < settings.voices; ++i)
            {
                m_stereoChorus[channel][i].Write(bufferSample);
                m_stereoChorus[channel][i].Update(settings.delayInSeconds, settings.depth);
                delayedSample = m_stereoChorus[channel][i].Read();
            }
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

    layout.add(std::make_unique<juce::AudioParameterInt>("voices", "Nr. Of Voices", 0, 4, 1));
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

    settings.voices = apvts.getRawParameterValue("voices")->load();
    settings.delayInSeconds = apvts.getRawParameterValue("delaySec")->load();
    settings.rate = apvts.getRawParameterValue("rate")->load();
    settings.depth = apvts.getRawParameterValue("depth")->load();
    settings.dry = apvts.getRawParameterValue("dry")->load();
    settings.wet = apvts.getRawParameterValue("wet")->load();

    return settings;
}