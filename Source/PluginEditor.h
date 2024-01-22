/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"

struct RotaryKnob : public juce::Slider
{
    RotaryKnob(const juce::String& label = "Label") :
        juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow)
    {
    }
    ~RotaryKnob() {}
};
class ComponentLabel : public juce::Label
{
public:
    ComponentLabel(juce::Component& comp, const juce::String& labelText = "Name") :
        m_comp(comp),
        juce::Label::Label(labelText + "Label", labelText) { }
    ~ComponentLabel() {}

    //==============================================================================
    void InitialiseLabel()
    {
        auto lookAndFeel{ &m_comp.getLookAndFeel() };
        setLookAndFeel(lookAndFeel);
        setJustificationType(juce::Justification::centredTop);
        attachToComponent(&m_comp, false);
    }

private:
    juce::Component& m_comp;
};
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
    std::vector<juce::Component*> GetComponents();
    std::vector<ComponentLabel*> CustomChorusAudioProcessorEditor::GetLabels();

    //==============================================================================
    CustomLookAndFeel m_lookAndFeel;

    RotaryKnob m_nrOfVoicesKnob;
    ComponentLabel m_nrOfVoicesLbl;
    RotaryKnob m_delayKnob;
    ComponentLabel m_delayLbl;
    RotaryKnob m_rateKnob;
    ComponentLabel m_rateLbl;
    RotaryKnob m_depthKnob;
    ComponentLabel m_depthLbl;
    RotaryKnob m_dryKnob;
    ComponentLabel m_dryLbl;
    RotaryKnob m_wetKnob;
    ComponentLabel m_wetLbl;

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;
    Attachment m_nrOfVoicesKnobAttach;
    Attachment m_delayAttach;
    Attachment m_rateAttach;
    Attachment m_depthAttach;
    Attachment m_dryAttach;
    Attachment m_wetAttach;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CustomChorusAudioProcessor& m_audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomChorusAudioProcessorEditor)
};
