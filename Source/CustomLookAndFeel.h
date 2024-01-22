/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 22 Jan 2024 8:23:48am
    Author:  AWizardInATower

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel(juce::Colour bgClr);
    ~CustomLookAndFeel();

    //===============================================================
    void drawRotarySlider(juce::Graphics& g,
        int x, int y,
        int width, int height,
        float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle,
        juce::Slider& slider) override;
    void drawLabel(juce::Graphics& g, juce::Label& l) override;

    //===============================================================
    const juce::Colour& GetBackgroundColour();
    const juce::Colour& GetColourFromGradient(double position);

private:
    juce::Colour const m_backgroundClr;
    juce::PathStrokeType const m_strokeType;
    juce::ColourGradient m_gradient;

};