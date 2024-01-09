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
        m_buffer.reserve(sampleRate * maxDelayLengthSec);
        m_buffer.resize(sampleRate * maxDelayLengthSec);
        std::fill(m_buffer.begin(), m_buffer.end(), 0.0f);
    }

    void SetDelay( float delayInSamples)
    {
        m_readPosition = std::floor(delayInSamples) - 1;
        if (m_readPosition < 0)
        {
            m_readPosition = 0;
        }
        m_fraction = delayInSamples - std::floor(delayInSamples);
        
        //if (m_fraction != 0) jassertfalse;
    }

    void Write(float sample)
    {
        m_buffer.pop_back();
        m_buffer.insert(m_buffer.begin(), sample);
    }

    float Read()
    {
        if (m_fraction == 0 || m_readPosition == 0)
        {
            return m_buffer[m_readPosition];
        }
        else
        {
            float currentSample{ m_buffer[m_readPosition] };
            float nextSample{ m_buffer[m_readPosition - 1] };
            float fractionRemainder{ 1 - m_fraction };
            float fractionalSample{ (currentSample * m_fraction) + (nextSample * fractionRemainder) };

            return std::clamp(fractionalSample, -1.f, 1.f);
        }
    }

private:
    std::vector<float> m_buffer;
    float m_readPosition;
    float m_fraction;

};