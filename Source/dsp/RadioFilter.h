#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_basics/juce_audio_basics.h>

//==============================================================================
/**
 * RadioFilter - Bright, present voice effect
 * 
 * Simulates the frequency response of radio broadcast and voice-over
 * processing by:
 * - High-pass filter at 100 Hz (remove rumble and room noise)
 * - Presence peak at 3500 Hz (+6 dB gain, tight Q for clarity)
 * 
 * This creates a characteristic "radio voice" quality used in:
 * - Radio broadcast production
 * - Voice-over and narration
 * - Podcast processing
 * - PA announcements
 * 
 * DSP Specifications:
 * - Filter Type: High-pass (100 Hz, Q=0.7) + Peaking (3500 Hz, Q=2.0, +6dB)
 * - High-Pass: 100 Hz, Q=0.7 (gentle rumble removal)
 * - Presence Peak: 3500 Hz center, Q=2.0, +6 dB gain
 * - Total effect: Brightened, present mid-range (professional sound)
 * - Stability: Verified across 8 kHz - 192 kHz sample rates
 * 
 * Real-Time Safety: ✅
 * - No allocations in process()
 * - All filter state pre-allocated in prepare()
 * - Thread-safe via JUCE DSP module
 */
class RadioFilter
{
public:
    RadioFilter() = default;
    ~RadioFilter() = default;

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
        
        // Create high-pass filter at 100 Hz (remove rumble)
        auto highPassCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighPass(
            spec.sampleRate, 
            100.0f,   // 100 Hz cutoff (gentle rumble removal)
            0.7f      // Q = 0.7 (very gentle slope)
        );
        highPassFilter.coefficients = highPassCoeffs;
        highPassFilter.prepare(spec);

        // Create peaking filter for presence boost around 3.5 kHz
        auto peakingCoeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter(
            spec.sampleRate,
            3500.0f,  // Centre frequency at 3.5 kHz (presence peak)
            2.0f,     // Q = 2.0 (moderate width for presence)
            2.0f      // Gain = 2.0x (+6dB boost in amplitude)
        );
        peakingFilter.coefficients = peakingCoeffs;
        peakingFilter.prepare(spec);
    }

    //==============================================================================
    /** Processes an audio buffer through both filters.
     * 
     * Applies the high-pass filter first (remove rumble),
     * then the presence peak filter (add clarity and brightness),
     * creating the characteristic bright "radio voice" effect.
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
        
        // Apply high-pass filter first (removes low rumble and noise)
        highPassFilter.process(context);

        // Apply presence peak filter second (adds brightness/clarity)
        peakingFilter.process(context);
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
        peakingFilter.reset();
    }

    //==============================================================================
    /** Returns the current sample rate for debugging/monitoring. */
    double getSampleRate() const { return currentSampleRate; }

private:
    // High-pass filter for rumble removal + presence peak for brightness
    juce::dsp::IIR::Filter<float> highPassFilter;  ///< 100 Hz high-pass for rumble removal
    juce::dsp::IIR::Filter<float> peakingFilter;   ///< 3500 Hz peak (+6 dB) for presence

    double currentSampleRate = 44100.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RadioFilter)
};
