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
    CircularBuffer() :
        m_writeIndex(0),
        m_readIndex(0),
        m_size(0),
        m_frac(0),
        m_offset(0)
    {
        
    }

    void setSize(int size)
    {
        m_size = size;
        m_buffer.resize(m_size);
        std::fill(m_buffer.begin(), m_buffer.end(), 0);
    }

    void write(float sample)
    {
        m_buffer[m_writeIndex] = sample;
        m_writeIndex = (m_writeIndex + 1) % m_size;
        m_readIndex = (m_writeIndex - m_offset + m_size) % m_size;

    }

    float read()
    {
        //float bSample1{ m_buffer[m_readIndex] };
        //float bSample2{ m_buffer[(m_readIndex + 1) % m_size] };
        //float oSample1{ bSample1 * m_frac };
        //float oSample2{ bSample2 * (1 - m_frac) };
        //return oSample1 + oSample2;
        return m_buffer[m_readIndex];
    }

    void setDelay(float delayInSamples)
    {
        //float integerDelay{ 0 };
        //m_frac = std::modf(delay, &integerDelay);
        //m_offset = m_size - (static_cast<int>(integerDelay) + 1);
        m_offset = delayInSamples;
    }

private:
    std::vector<float> m_buffer;
    int m_writeIndex;
    int m_readIndex;
    int m_size;
    int m_frac;
    int m_offset;
};