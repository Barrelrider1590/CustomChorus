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
        m_readPosition(m_bufferMaxSize)
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
        for (auto& buffer : m_channelBuffers)
        {
            buffer.resize(delayInSamples);
            m_readPosition = delayInSamples - 1;
        }

    }

    void Write(int channel, float sample)
    {
        m_channelBuffers[channel].pop_back();
        m_channelBuffers[channel].insert(m_channelBuffers[channel].begin(), sample);
    }

    float Read(int channel)
    {
        return m_channelBuffers[channel][m_readPosition];
    }

private:
    std::vector<std::vector<float>> m_channelBuffers;
    int m_nrOfChannels;
    int m_bufferMaxSize;
    int m_readPosition;
};