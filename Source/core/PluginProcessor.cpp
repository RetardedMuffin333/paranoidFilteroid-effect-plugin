#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../dsp/DSPChain.h"

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
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
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

    // Prepare DSP chain with audio specification
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();
    dspChain.prepare(spec);
}

void PluginProcessor::releaseResources() {
    // Reset DSP chain state
    dspChain.reset();
}

//==============================================================================
void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                   juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;

    // Read parameters
    bool enabled = apvts.getRawParameterValue("enabled")->load();
    int mode = static_cast<int>(apvts.getRawParameterValue("mode")->load());
    float mix = apvts.getRawParameterValue("mix")->load();

    // If disabled, clear output (bypass)
    if (!enabled) {
        buffer.clear();
        return;
    }

    // Process audio through DSP chain with selected mode and mix level
    dspChain.processBlock(buffer, mode, mix);
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
            auto newState = juce::ValueTree::fromXml(*xmlState);
            apvts.replaceState(newState);
        }
    }
}

//==============================================================================
// JUCE Plugin Entry Point
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}
