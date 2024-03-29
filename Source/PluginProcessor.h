/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ChorusVoice.h"

struct PluginSettings
{
    int voices{ 0 };
    float delayInSeconds{ 0.0f };
    float rate{ 0.0f }, depth{ 0.0f };
    float dry{ 0.0f }, wet{ 0.0f };
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
    std::vector<std::vector<ChorusVoice>> m_stereoChorus;

    juce::AudioProcessorValueTreeState m_apvts;

    static const int MAX_VOICES;
    static const float MAX_DELAY_SECONDS;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomChorusAudioProcessor)
};
