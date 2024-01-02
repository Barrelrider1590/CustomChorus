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
    ChorusVoice(int nrofChannels, float maxDelayLengthSec, int sampleRate) :
        m_delayLine(nrofChannels, maxDelayLengthSec, sampleRate),
        m_lfo([](float x) {return std::sinf(x); }, 128),
        m_smoothedSample(0.0f),
        m_smoothedDelay(0.0f),
        m_currentSample(0.0f),
        m_currentDelay(0.0f),
        m_delayAltered(false)
    {
        m_lfo.setFrequency(0.15f);
    }
    ~ChorusVoice() {}

    //========================================================================
    void Prepare(const juce::dsp::ProcessSpec& spec, float delayInSamples)
    {
        m_delayLine.prepare(spec.sampleRate, delayInSamples);
        m_lfo.prepare(spec);
        m_smoothedSample.reset(spec.sampleRate, 0.000002);
        m_smoothedSample.setCurrentAndTargetValue(0.0f);
    }

    void Update(int channel, float delayInSeconds, float sampleRate, float rate, float depth)
    {
        if (!m_smoothedSample.isSmoothing())
        {
            m_lfo.setFrequency(rate);
            float lfoOut{ m_lfo.processSample(1.0f) };
            float maxDepth{ 0.5f + (depth * 0.5f) };
            float minDepth{ 0.5f - (depth * 0.5f) };
            float lfoDepth{ juce::jmap(lfoOut, -1.0f, 1.0f, minDepth + 0.001f, maxDepth) };
            float delayInSamples = (delayInSeconds * lfoDepth) * sampleRate;
            m_smoothedSample.setCurrentAndTargetValue(m_delayLine.Read(channel));
            m_delayLine.SetDelayLength(delayInSamples);
            m_smoothedSample.setTargetValue(m_delayLine.Read(channel));
        }
    }

    void Write(int channel, float bufferSample)
    {
        m_delayLine.Write(channel, bufferSample);
        m_currentSample = m_delayLine.Read(channel); // breaks the process for some reason when removed
    }

    float Read(int channel)
    {
        if (m_smoothedSample.isSmoothing())
        {
            return m_smoothedSample.getNextValue();
        }
    }

private:
    CircularBuffer m_delayLine;
    juce::dsp::Oscillator<float> m_lfo;
    juce::SmoothedValue<float> m_smoothedSample;
    juce::SmoothedValue<float> m_smoothedDelay;

    float m_currentSample;

    float m_currentDelay;

    bool m_delayAltered;
};