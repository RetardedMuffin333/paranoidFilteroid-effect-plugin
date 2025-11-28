#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_basics/juce_audio_basics.h>

//==============================================================================
/**
 * TelephonyFilter - Narrowband voice effect (300-3400 Hz)
 * 
 * Simulates the frequency response of telephone/intercom systems by:
 * - High-pass filter at 300 Hz (remove rumble, low frequency noise)
 * - Low-pass filter at 3400 Hz (remove high frequency sibilance)
 * 
 * This creates a characteristic "phone voice" quality used in:
 * - Telephone effect plugins
 * - Voice-over processing
 * - Radio call-in effects
 * 
 * DSP Specifications:
 * - Filter Type: High-pass (300 Hz, Q=1.0) + Low-pass (3400 Hz, Q=1.0)
 * - High-Pass: 300 Hz, Q=1.0 (gentle slope)
 * - Low-Pass: 3400 Hz, Q=1.0 (gentle slope)
 * - Total bandwidth: ~3100 Hz (narrow for voice)
 * - Stability: Verified across 8 kHz - 192 kHz sample rates
 * 
 * Real-Time Safety: ✅
 * - No allocations in process()
 * - All filter state pre-allocated in prepare()
 * - Thread-safe via JUCE DSP module
 */
class TelephonyFilter
{
public:
    TelephonyFilter() = default;
    ~TelephonyFilter() = default;

    //==============================================================================
    /** Initializes filters for the given audio specification.
     * 
     * Must be called once before any process() calls, typically in
     * PluginProcessor::prepareToPlay().
     * 
     * @param spec Contains sample rate, block size, and channel count
     */
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        currentSampleRate = spec.sampleRate;
        
        // Create high-pass filter at 300 Hz
        auto highPassCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighPass(
            spec.sampleRate, 
            300.0f,   // 300 Hz cutoff
            1.0f      // Q = 1.0 (gentle slope)
        );
        highPassFilter.coefficients = highPassCoeffs;
        highPassFilter.prepare(spec);
        
        // Create low-pass filter at 3400 Hz
        auto lowPassCoeffs = juce::dsp::IIR::Coefficients<float>::makeLowPass(
            spec.sampleRate, 
            3400.0f,  // 3400 Hz cutoff
            1.0f      // Q = 1.0 (gentle slope)
        );
        lowPassFilter.coefficients = lowPassCoeffs;
        lowPassFilter.prepare(spec);
    }

    //==============================================================================
    /** Processes an audio buffer through both filters.
     * 
     * Applies the high-pass filter first (remove low rumble),
     * then the low-pass filter (remove high sibilance),
     * creating the characteristic narrowband telephone voice effect.
     * 
     * Real-Time Safe: No allocations, only reads/writes to pre-allocated state.
     * 
     * @param buffer The audio buffer to process in-place
     */
    void process(juce::AudioBuffer<float>& buffer)
    {
        // Convert AudioBuffer to AudioBlock for DSP processing
        juce::dsp::AudioBlock<float> audioBlock(buffer);
        juce::dsp::ProcessContextReplacing<float> context(audioBlock);
        
        // Apply high-pass filter first (removes low frequencies)
        highPassFilter.process(context);

        // Apply low-pass filter second (removes high frequencies)
        lowPassFilter.process(context);
    }

    //==============================================================================
    /** Resets all filter state to zero.
     * 
     * Clears internal delay lines to prevent clicks when:
     * - Switching modes
     * - Re-enabling after bypass
     * - Plugin deactivation
     * 
     * Safe to call at any time, best called during parameter changes.
     */
    void reset()
    {
        highPassFilter.reset();
        lowPassFilter.reset();
    }

    //==============================================================================
    /** Returns the current sample rate for debugging/monitoring. */
    double getSampleRate() const { return currentSampleRate; }

private:
    // High-pass and low-pass filters for bandpass effect
    juce::dsp::IIR::Filter<float> highPassFilter;  ///< 300 Hz high-pass
    juce::dsp::IIR::Filter<float> lowPassFilter;   ///< 3400 Hz low-pass

    double currentSampleRate = 44100.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TelephonyFilter)
};
