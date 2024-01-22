/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 22 Jan 2024 8:23:48am
    Author:  AWizardInATower

  ==============================================================================
*/

#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel(juce::Colour bgClr) :
    m_backgroundClr(bgClr)
{

}

CustomLookAndFeel::~CustomLookAndFeel(){}


void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g,
    int x, int y,
    int width, int height,
    float sliderPos,
    float rotaryStartAngle, float rotaryEndAngle,
    juce::Slider&){}

const juce::Colour& CustomLookAndFeel::GetBackgroundColour()
{
    return m_backgroundClr;
}