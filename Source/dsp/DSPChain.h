#pragma once

#include "TelephonyFilter.h"
#include "RadioFilter.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_core/juce_core.h>

//==============================================================================
/**
 * DSPChain - Main audio processing orchestrator
 * 
 * Routes input audio through the selected filter (Telephone or Radio)
 * and applies wet/dry blending via the mix parameter.
 * 
 * Modes:
 * - 0: Telephone (narrowband, muffled)
 * - 1: Radio (bright, presence-heavy)
 * - 2: Custom (both filters chained for extreme effect)
 * 
 * Real-Time Safe: ✅
 * - Mode switching done via parameter (no mode-specific allocations)
 * - All filter processing pre-allocated
 * - Mix blending done in real-time without allocations
 */
class DSPChain
{
public:
    DSPChain() = default;
    ~DSPChain() = default;

    //==============================================================================
    /** Initializes all DSP filters for the given audio specification.
     * 
     * Must be called once before any processBlock() calls, typically in
     * PluginProcessor::prepareToPlay().
     * 
     * @param spec Contains sample rate, block size, and channel count
     */
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        // Store spec for later reference
        currentSpec = spec;

        // Prepare both filters
        telephonyFilter.prepare(spec);
        radioFilter.prepare(spec);

        // Pre-allocate temporary buffer for wet/dry blending
        // This avoids allocations in real-time processBlock()
        tempBuffer.setSize(spec.numChannels, spec.maximumBlockSize);
    }

    //==============================================================================
    /** Processes an audio buffer through the selected filter.
     * 
     * Routes audio through Telephone, Radio, or Custom filter based on
     * the mode parameter, then blends with dry signal using mix parameter.
     * 
     * Real-Time Safe: No allocations (all state pre-allocated in prepare).
     * 
     * @param buffer The audio buffer to process in-place
     * @param mode   The selected mode (0=Telephone, 1=Radio, 2=Custom)
     * @param mix    Wet/dry blend (0.0=dry, 1.0=100% wet effect)
     */
    void processBlock(juce::AudioBuffer<float>& buffer, int mode, float mix)
    {
        const int numSamples = buffer.getNumSamples();
        const int numChannels = buffer.getNumChannels();

        // Ensure mix is in valid range [0, 1]
        mix = juce::jlimit(0.0f, 1.0f, mix);

        // If completely dry (mix=0), bypass processing
        if (mix < 0.001f)
        {
            return;  // Output = input, no processing needed
        }

        // Copy input to temp buffer for wet signal processing
        tempBuffer.makeCopyOf(buffer, true);

        // Route through selected filter
        switch (mode)
        {
            case 0:
                // Telephone mode: narrowband effect
                telephonyFilter.process(tempBuffer);
                break;

            case 1:
                // Radio mode: bright, presence effect
                radioFilter.process(tempBuffer);
                break;

            case 2:
                // Custom mode: both filters chained (extreme effect)
                telephonyFilter.process(tempBuffer);
                radioFilter.process(tempBuffer);
                break;

            default:
                // Safety: default to Telephone if invalid mode
                telephonyFilter.process(tempBuffer);
                break;
        }

        // Blend wet and dry signals
        // output = dry * (1 - mix) + wet * mix
        for (int ch = 0; ch < numChannels; ++ch)
        {
            auto* dryPtr = buffer.getWritePointer(ch);
            const auto* wetPtr = tempBuffer.getReadPointer(ch);

            for (int n = 0; n < numSamples; ++n)
            {
                dryPtr[n] = dryPtr[n] * (1.0f - mix) + wetPtr[n] * mix;
            }
        }
    }

    //==============================================================================
    /** Resets all internal filter state.
     * 
     * Clears delay lines to prevent clicks or pops when:
     * - Switching modes
     * - Re-enabling after bypass
     * - Plugin deactivation
     * 
     * Safe to call at any time, best called during mode changes.
     */
    void reset()
    {
        telephonyFilter.reset();
        radioFilter.reset();
    }

    //==============================================================================
    /** Returns the current sample rate for debugging. */
    double getSampleRate() const
    {
        return currentSpec.sampleRate;
    }

private:
    // Filter instances
    TelephonyFilter telephonyFilter;  ///< Narrowband voice effect
    RadioFilter radioFilter;          ///< Bright voice effect

    // Pre-allocated temporary buffer for wet signal (avoids real-time allocations)
    juce::AudioBuffer<float> tempBuffer;

    // Stored audio specification
    juce::dsp::ProcessSpec currentSpec;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DSPChain)
};
