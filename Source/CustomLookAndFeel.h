/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 22 Jan 2024 8:23:48am
    Author:  AWizardInATower

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomLookAndFeel : juce::LookAndFeel_V4
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
        juce::Slider&);

    //===============================================================
    const juce::Colour& GetBackgroundColour();

private:
    juce::Colour const m_backgroundClr;


};