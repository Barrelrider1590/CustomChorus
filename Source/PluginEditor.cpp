/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CustomChorusAudioProcessorEditor::CustomChorusAudioProcessorEditor (CustomChorusAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    m_lookAndFeel(juce::Colour(20, 20, 20))
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 400);
}

CustomChorusAudioProcessorEditor::~CustomChorusAudioProcessorEditor()
{
}

//==============================================================================
void CustomChorusAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (m_lookAndFeel.GetBackgroundColour());

    g.setColour (juce::Colours::goldenrod);
    g.setFont (15.0f);
    g.drawFittedText ("Custom Chorus", getLocalBounds(), juce::Justification::centred, 1);
}

void CustomChorusAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
