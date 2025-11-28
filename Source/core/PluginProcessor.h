#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "../dsp/DSPChain.h"

//==============================================================================
class PluginProcessor : public juce::AudioProcessor {
public:
    //==========================================================================
    PluginProcessor();
    ~PluginProcessor() override;

    //==========================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==========================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    //==========================================================================
    const juce::String getName() const override;
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    //==========================================================================
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int index) override {}
    const juce::String getProgramName(int index) override { return "Default"; }
    void changeProgramName(int index, const juce::String& newName) override {}

    //==========================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //==========================================================================
    // APVTS (Audio Processor Value Tree State)
    juce::AudioProcessorValueTreeState apvts;

private:
    //==========================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    double currentSampleRate = 44100.0;
    int maxBlockSize = 512;

    // DSP Chain for audio processing
    DSPChain dspChain;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
