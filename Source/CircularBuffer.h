/*
  ==============================================================================

    CircularBuffer.h
    Created: 26 Nov 2023 12:57:53pm
    Author:  Phoenix

  ==============================================================================
*/

#pragma once

#include <vector>
#include <cmath>
#include <algorithm>

class CircularBuffer
{
public:
    CircularBuffer(float maxDelayLengthSec, int sampleRate) :
        m_buffer(),
        m_readPosition(sampleRate * maxDelayLengthSec),
        m_fraction(0)
    {
        m_buffer.reserve(sampleRate * maxDelayLengthSec + 1);
        m_buffer.resize(sampleRate * maxDelayLengthSec + 1);
    }

    void SetDelay( float delayInSamples)
    {
        m_readPosition = std::floor(delayInSamples);
        m_fraction = delayInSamples - std::floor(delayInSamples);
    }

    void Write(float sample)
    {
        m_buffer.pop_back();
        m_buffer.insert(m_buffer.begin(), sample);
    }

    float Read()
    {
        float currentSample{ m_buffer[m_readPosition] };
        float nextSample{ m_buffer[m_readPosition + 1] };
        float fractionRemainder{ 1 - m_fraction };
        float fractionalSample{ (currentSample * fractionRemainder) + (nextSample * m_fraction) };

        return std::clamp(fractionalSample, -1.f, 1.f);
    }

private:
    std::vector<float> m_buffer;
    int m_readPosition;
    float m_fraction;

};