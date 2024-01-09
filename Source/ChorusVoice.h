/*
  ==============================================================================

    ChorusVoice.h
    Created: 1 Jan 2024 8:35:16pm
    Author:  AWizardInATower

  ==============================================================================
*/

#pragma once
#include "CircularBuffer.h"
#include <JuceHeader.h>

class ChorusVoice
{
public:
    ChorusVoice(float maxDelaySeconds, float sampleRate) :
        m_delayLine(maxDelaySeconds, sampleRate),
        m_LFO([](float x) { return std::sinf(x); }, 125),
        m_sampleRate(sampleRate)
    {
        juce::Random rand{ juce::Random() };
        m_offset = juce::jmap(rand.nextFloat(), 0.8f, 1.0f);
        m_LFO.setFrequency(0.15f * m_offset);
    }
    void Prepare(const juce::dsp::ProcessSpec& spec, float delayInSeconds)
    {
        m_LFO.prepare(spec);
        m_sampleRate = spec.sampleRate;
        m_delayLine.SetDelay(delayInSeconds * m_sampleRate);
    }
    void SetFrequency(float newFrequency)
    {
        m_LFO.setFrequency(newFrequency * m_offset);
    }
    void Update(float delayInSeconds, float depth)
    {
        float lfoOut{ m_LFO.processSample(0.0f)};
        float lfoMin{ 0.5f - (0.5f * depth) };
        float lfoMax{ 0.5f + (0.5f * depth) };
        float mod{ juce::jmap(lfoOut, -1.0f, 1.0f, lfoMin, lfoMax) };

        m_delayLine.SetDelay(mod * (delayInSeconds * m_sampleRate));
    }
    void Write(float sample)
    {
        m_delayLine.Write(sample);
    }
    float Read()
    {
        return m_delayLine.Read();
    }

private:
    CircularBuffer m_delayLine;
    juce::dsp::Oscillator<float> m_LFO;

    float m_sampleRate;
    float m_offset;
};