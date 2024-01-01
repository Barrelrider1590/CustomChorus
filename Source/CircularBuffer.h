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
    CircularBuffer(int nrofChannels, float maxDelayLengthSec, int sampleRate) :
        m_nrOfChannels(nrofChannels),
        m_bufferMaxSize(maxDelayLengthSec * sampleRate),
        m_readPosition(m_bufferMaxSize),
        m_fraction(0)
    {
        m_channelBuffers.resize(m_nrOfChannels);

        for (auto& buffer : m_channelBuffers)
        {
            buffer.reserve(m_bufferMaxSize);
            buffer.resize(m_bufferMaxSize);
            std::fill(buffer.begin(), buffer.end(), 0);
        }
    }

    void SetDelayLength(float delayInSamples)
    {
        float integerDelay{ 0 };
        m_fraction = std::modf(delayInSamples, &integerDelay);

        for (auto& buffer : m_channelBuffers)
        {
            buffer.resize(integerDelay);
            m_readPosition = integerDelay-1;
            if (m_readPosition == 0)
            {
                m_readPosition = 1;
            }
        }

    }

    void Write(int channel, float sample)
    {
        m_channelBuffers[channel].pop_back();
        m_channelBuffers[channel].insert(m_channelBuffers[channel].begin(), sample);
    }

    float Read(int channel)
    {
        //float bSample1{ m_channelBuffers[channel][m_readPosition] };
        //float bSample2{ m_channelBuffers[channel][(m_readPosition - 1) % m_readPosition] };
        //float oSample1{ bSample1 * m_fraction };
        //float oSample2{ bSample2 * (1 - m_fraction) };
        //return oSample1 + oSample2;

        return m_channelBuffers[channel][m_readPosition];
    }

private:
    std::vector<std::vector<float>> m_channelBuffers;
    int m_nrOfChannels;
    int m_bufferMaxSize;
    int m_readPosition;
    float m_fraction;
};