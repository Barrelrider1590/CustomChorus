/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CircularBuffer.h"

struct PluginSettings
{

    int delayInSamples{ 0 };
    float delayInSeconds{ 0.0f }, dry{ 0.0f }, wet{ 0.0f };
};

//==============================================================================
/**
*/
class CustomChorusAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    CustomChorusAudioProcessor();
    ~CustomChorusAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    juce::AudioProcessorValueTreeState* GetAPVTS();
    juce::AudioProcessorValueTreeState::ParameterLayout CreateParameterLayout();
    PluginSettings GetPluginSettings(const juce::AudioProcessorValueTreeState& apvts);

private:
    CircularBuffer m_delayBuffer;
    int m_delayBufferSize;
    int m_delayInSamples;
    int m_sampleRate;

    juce::dsp::Oscillator<float> m_lfo;
    int m_lfoUpdateCounter;
    int m_lfoUpdateRate;

    juce::AudioProcessorValueTreeState m_apvts;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomChorusAudioProcessor)
};
