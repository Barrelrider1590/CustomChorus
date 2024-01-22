/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/**
*/
class CustomChorusAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    CustomChorusAudioProcessorEditor (CustomChorusAudioProcessor&);
    ~CustomChorusAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    CustomLookAndFeel m_lookAndFeel;


    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CustomChorusAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomChorusAudioProcessorEditor)
};
