/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CustomChorusAudioProcessorEditor::CustomChorusAudioProcessorEditor(CustomChorusAudioProcessor& p)
    : AudioProcessorEditor(&p), m_audioProcessor(p),
    m_lookAndFeel(juce::Colour(20, 20, 20)),
    m_nrOfVoicesKnob(),
    m_nrOfVoicesLbl(m_nrOfVoicesKnob, "Voices"),
    m_nrOfVoicesKnobAttach(*p.GetAPVTS(), "voices", m_nrOfVoicesKnob),
    m_delayKnob(),
    m_delayLbl(m_delayKnob, "Delay"),
    m_delayAttach(*p.GetAPVTS(), "delaySec", m_delayKnob),
    m_rateKnob(),
    m_rateLbl(m_rateKnob, "Rate"),
    m_rateAttach(*p.GetAPVTS(), "rate", m_rateKnob),
    m_depthKnob(),
    m_depthLbl(m_depthKnob, "Depth"),
    m_depthAttach(*p.GetAPVTS(), "depth", m_depthKnob),
    m_dryKnob(),
    m_dryLbl(m_dryKnob, "Dry"),
    m_dryAttach(*p.GetAPVTS(), "dry", m_dryKnob),
    m_wetKnob(),
    m_wetLbl(m_wetKnob, "Wet"),
    m_wetAttach(*p.GetAPVTS(), "wet", m_wetKnob)
{
    for (auto comp : GetComponents())
    {
        comp->setLookAndFeel(&m_lookAndFeel);
        addAndMakeVisible(comp);
    }
    for (ComponentLabel* l : GetLabels())
    {
        l->InitialiseLabel();
    }

    m_delayKnob.setTextValueSuffix(" s");
    m_rateKnob.setTextValueSuffix(" Hz");
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 600);
}

CustomChorusAudioProcessorEditor::~CustomChorusAudioProcessorEditor()
{
}

//==============================================================================
void CustomChorusAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (m_lookAndFeel.GetBackgroundColour());

    auto bounds{ getLocalBounds() };
    auto titleBnds{ bounds.removeFromTop(bounds.getHeight() * .1f)};
    titleBnds.reduce(10, 10);
    g.setColour(m_lookAndFeel.GetColourFromGradient(0.5));
    g.setFont(30.0f);
    g.drawFittedText("Custom Chorus", titleBnds, juce::Justification::centred, 1);
}

void CustomChorusAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    float marginBig{ 25 };
    float marginSmall{ 10 };
    juce::Rectangle<int> bounds{ getLocalBounds() };

    auto mixBnds{ bounds.removeFromTop(bounds.getHeight() * .33f) };
    auto voicesBnds{ bounds.removeFromTop(bounds.getHeight() * .5f) };
    auto chorusBnds{ bounds };

    mixBnds.reduce(marginSmall, marginSmall);
    auto dryBnds{ mixBnds.removeFromRight(mixBnds.getWidth() * .5f) };
    dryBnds.reduce(marginBig, marginBig);
    dryBnds.setY(dryBnds.getY() + marginBig * 2);
    auto wetBnds{ mixBnds };
    wetBnds.reduce(marginBig, marginBig);
    wetBnds.setY(wetBnds.getY() + marginBig * 2);
    m_dryKnob.setBounds(dryBnds);
    m_wetKnob.setBounds(wetBnds);

    voicesBnds.reduce(marginSmall, marginSmall);
    voicesBnds.setY(voicesBnds.getY() - marginBig);
    m_nrOfVoicesKnob.setBounds(voicesBnds);

    chorusBnds.reduce(marginSmall, marginSmall);
    auto delayBnds{ chorusBnds.removeFromLeft(chorusBnds.getWidth() * .33f) };
    auto rateBnds{ chorusBnds.removeFromLeft(chorusBnds.getWidth() * .5f) };
    auto depthBnds{ chorusBnds };
    delayBnds.reduce(marginBig, marginBig);
    delayBnds.setY(delayBnds.getY() - marginBig * 2.5);
    m_delayKnob.setBounds(delayBnds);
    rateBnds.reduce(marginBig, marginBig);
    m_rateKnob.setBounds(rateBnds);
    depthBnds.reduce(marginBig, marginBig);
    depthBnds.setY(depthBnds.getY() - marginBig * 2.5);
    m_depthKnob.setBounds(depthBnds);

}

std::vector<juce::Component*> CustomChorusAudioProcessorEditor::GetComponents()
{
    return
    {
        &m_nrOfVoicesKnob,
        &m_delayKnob,
        &m_rateKnob,
        &m_depthKnob,
        &m_dryKnob,
        &m_wetKnob
    };
}

std::vector<ComponentLabel*> CustomChorusAudioProcessorEditor::GetLabels()
{
    return
    {
        &m_nrOfVoicesLbl,
        &m_delayLbl,
        &m_rateLbl,
        &m_depthLbl,
        &m_dryLbl,
        &m_wetLbl
    };
}