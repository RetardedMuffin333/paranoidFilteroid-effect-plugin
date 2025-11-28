#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_basics/juce_audio_basics.h>

//==============================================================================
/**
 * TelephonyFilter - Narrowband voice effect (300-3400 Hz)
 * 
 * Simulates the frequency response of telephone/intercom systems.
 * For now, provides a simple pass-through with mix scaling.
 * DSP filter implementation will be added in a future phase.
 */
class TelephonyFilter
{
public:
    TelephonyFilter() = default;
    ~TelephonyFilter() = default;

    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        currentSampleRate = spec.sampleRate;
        // Filter coefficients will be implemented in Phase 8
    }

    void process(juce::AudioBuffer<float>& buffer)
    {
        // For Phase 7, just pass audio through
        // Full bandpass filtering (300-3400Hz) will be added next
    }

    void reset()
    {
        // Reset any internal filter state here
    }

    double getSampleRate() const { return currentSampleRate; }

private:
    double currentSampleRate = 44100.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TelephonyFilter)
};
