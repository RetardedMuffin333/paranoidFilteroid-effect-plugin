#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Mode parameter: Telephone / Radio / Custom
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "mode", "Mode",
        juce::StringArray("Telephone", "Radio", "Custom"),
        0  // default: Telephone
    ));

    // Mix parameter: 0–100%, represented as 0.0–1.0
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "mix", "Mix",
        0.0f, 1.0f,  // range
        1.0f  // default: 100% (wet)
    ));

    // Enabled parameter: bypass toggle
    layout.add(std::make_unique<juce::AudioParameterBool>(
        "enabled", "Enabled",
        true  // default: enabled
    ));

    return layout;
}

//==============================================================================
PluginProcessor::PluginProcessor()
    : AudioProcessor(BusesProperties()
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    , apvts(*this, nullptr, "STATE", createParameterLayout())
{
}

PluginProcessor::~PluginProcessor() {
}

//==============================================================================
void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    currentSampleRate = sampleRate;
    maxBlockSize = samplesPerBlock;

    juce::ScopedNoDenormals noDenormals;

    // Initialize any DSP state here (none yet for pass-through)
}

void PluginProcessor::releaseResources() {
    // Clean up any allocated resources
}

//==============================================================================
void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                   juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;

    // Read parameters
    bool enabled = apvts.getRawParameterValue("enabled")->load();
    float mix = apvts.getRawParameterValue("mix")->load();

    if (!enabled) {
        buffer.clear();
        return;
    }

    // Pass-through processing (no DSP yet)
    // For now, just pass audio unchanged
    // (DSP modules will be added here in Phase 3)

    int numSamples = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();

    for (int ch = 0; ch < numChannels; ++ch) {
        auto* channelData = buffer.getWritePointer(ch);

        for (int n = 0; n < numSamples; ++n) {
            // Output = input (100% dry for now)
            // When DSP is added, this becomes: output = mix*wet + (1-mix)*dry
        }
    }

    // For now, just ensure audio passes through (no processing)
}

//==============================================================================
juce::AudioProcessorEditor* PluginProcessor::createEditor() {
    return new PluginEditor(*this);
}

const juce::String PluginProcessor::getName() const {
    return "paranoidFilteroid";
}

//==============================================================================
void PluginProcessor::getStateInformation(juce::MemoryBlock& destData) {
    // Save APVTS state
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PluginProcessor::setStateInformation(const void* data, int sizeInBytes) {
    // Load APVTS state
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr) {
        if (xmlState->hasTagName(apvts.state.getType())) {
            apvts.replaceState(*xmlState);
        }
    }
}
