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
        m_bufferMaxSize(maxDelayLengthSec* sampleRate),
        m_readPosition(m_bufferMaxSize),
        m_sampleRate(sampleRate),
        m_maxDelayLengthSec(maxDelayLengthSec),
        m_fraction(0),
        m_delayLength(0.0f)
    {
        m_channelBuffers.resize(m_nrOfChannels);

        for (auto& buffer : m_channelBuffers)
        {
            buffer.reserve(m_bufferMaxSize);
            buffer.resize(m_bufferMaxSize);
            std::fill(buffer.begin(), buffer.end(), 0);
        }
    }

    void prepare(double sampleRate, float delayInSamples)
    {
        m_delayLength = delayInSamples;
        if (m_sampleRate != sampleRate)
        {
            m_sampleRate = sampleRate;
            m_bufferMaxSize = m_maxDelayLengthSec * m_sampleRate;
            for (auto& buffer : m_channelBuffers)
            {
                buffer.reserve(m_bufferMaxSize);
                buffer.resize(m_bufferMaxSize);
                std::fill(buffer.begin(), buffer.end(), 0);
            }
        }
        SetDelayLength(m_delayLength);
    }

    void SetDelayLength(float delayInSamples)
    {
        m_fraction = std::modf(delayInSamples, &m_delayLength);

        for (auto& buffer : m_channelBuffers)
        {
            //buffer.resize(integerDelay);
            m_readPosition = m_delayLength - 2;
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
        float nextSample{ m_channelBuffers[channel][m_readPosition - 1] };
        float currentSample{ m_channelBuffers[channel][m_readPosition] };
        float lerpedSample{ interpolate(currentSample, nextSample, m_fraction) };
        return lerpedSample;
    }

private:

    float interpolate(float prevSample, float nextSample, float fraction)
    {
        return (fraction * nextSample) + ((1.0f - fraction) * prevSample);
    }

    //=====================================================================
    std::vector<std::vector<float>> m_channelBuffers;
    int m_nrOfChannels;
    int m_bufferMaxSize;
    int m_readPosition;
    float m_sampleRate;
    float m_maxDelayLengthSec;
    float m_fraction;
    float m_delayLength;
};