/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 22 Jan 2024 8:23:48am
    Author:  AWizardInATower

  ==============================================================================
*/

#include "CustomLookAndFeel.h"

struct Vector2f
{
    float x;
    float y;
};


CustomLookAndFeel::CustomLookAndFeel(juce::Colour bgClr) :
    m_backgroundClr(bgClr),
    m_strokeType(2.0f, 
                 juce::PathStrokeType::JointStyle::curved, 
                 juce::PathStrokeType::EndCapStyle::rounded),
    m_gradient()
{
    m_gradient.addColour(0.0f, juce::Colours::cyan);
    //m_gradient.addColour(0.5f, juce::Colours::lightpink);
    m_gradient.addColour(1.f, juce::Colours::magenta);
}

CustomLookAndFeel::~CustomLookAndFeel(){}


void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g,
    int x, int y,
    int width, int height,
    float sliderPos,
    float rotaryStartAngle, float rotaryEndAngle,
    juce::Slider& slider)
{
    float radius{ static_cast<float>(juce::jmin(width, height) * .6f) };
    float angle{ rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle) };
    Vector2f containerCentre{ x + width * .5f, y + height * .5f };
    Vector2f ellipseCentre{ containerCentre.x - radius * .5f, containerCentre.y - radius * .5f };
    juce::Colour knobBaseClr{ juce::Colour(100, 100, 100) };
    juce::Colour knobAccentClr{ m_gradient.getColourAtPosition((angle - rotaryStartAngle) / (rotaryEndAngle - rotaryStartAngle)) };

    // draw knob
    g.setColour(knobBaseClr);
    g.fillEllipse(ellipseCentre.x, ellipseCentre.y, radius, radius);

    float lineThickness{ 5.f };
    g.setColour(knobAccentClr);
    g.drawEllipse(ellipseCentre.x, ellipseCentre.y, radius, radius, lineThickness);

    //draw pointer
    juce::Path pointer{};

    float pointerLength{ radius * .25f };
    float pointerThickness{ 15.f };
    float cornerSize{ 2.f };

    //pointer.addRoundedRectangle(-pointerThickness * .5f, -radius * .5f, pointerThickness, pointerLength, cornerSize);
    pointer.addEllipse(-pointerThickness *.5f, -radius * .5f, pointerThickness, pointerThickness);
    pointer.applyTransform(juce::AffineTransform::rotation(angle).scaled(1.f).translated(containerCentre.x, containerCentre.y));

    g.setColour(knobBaseClr);
    g.fillPath(pointer);
    g.setColour(knobAccentClr.brighter(0.5f));
    g.strokePath(pointer, m_strokeType);
}

void CustomLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& l)
{
    Vector2f position{ 0,0 };
    int nrOfLines{ 1 };

    g.setColour(m_gradient.getColourAtPosition(0.5));
    g.drawFittedText(l.getText(), position.x, position.y, l.getWidth(), l.getHeight(), l.getJustificationType(), nrOfLines);
}

const juce::Colour& CustomLookAndFeel::GetBackgroundColour()
{
    return m_backgroundClr;
}

const juce::Colour& CustomLookAndFeel::GetColourFromGradient(double position)
{
    return m_gradient.getColourAtPosition(position);
}