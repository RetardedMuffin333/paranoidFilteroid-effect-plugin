#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

// Placeholder DSP module (to be populated later with filters, EQ, etc.)
namespace DSP {
    class PassThrough {
    public:
        PassThrough() = default;

        void prepare(double sampleRate, int blockSize) {
            // Will implement prepareToPlay logic here
        }

        void process(juce::AudioBuffer<float>& buffer) {
            // Will implement DSP chain here
            // For now, audio passes through unchanged
        }

    private:
        // DSP state will go here (filters, EQ, compressor, etc.)
    };
}
