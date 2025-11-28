# Research Findings: JUCE VST3 Plugin Development
**paranoidFilteroid-effect-plugin**  
**Research Date**: November 28, 2025

---

## 1. JUCE Plugin Architecture Overview

### 1.1 Core Inheritance Hierarchy

All JUCE audio plugins inherit from **`juce::AudioProcessor`**, which defines the plugin's audio contract with the DAW:

```cpp
class PluginProcessor : public juce::AudioProcessor {
    // Core virtual methods must be overridden:
    
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void releaseResources();
    void processBlock(juce::AudioBuffer<float>& buffer, 
                     juce::MidiBuffer& midiMessages);
    
    // State management:
    void getStateInformation(juce::MemoryBlock& destData);
    void setStateInformation(const void* data, int sizeInBytes);
    
    // Metadata:
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    // ... more metadata methods
};
```

**Key Points**:
- `processBlock()` is called by the DAW in real-time; **must not allocate memory**
- `prepareToPlay()` is called once at startup; can allocate buffers here
- `releaseResources()` cleanup when session ends
- All APVTS parameters must be defined in `createParameterLayout()`

**Source**: JUCE API Documentation (juce::AudioProcessor)

---

## 2. AudioProcessorValueTreeState (APVTS)

### 2.1 Parameter Definition & Layout

APVTS is a **thread-safe parameter management system** connecting GUI, DSP, and state:

```cpp
juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    // Float parameter: mix (0–1.0)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "mix", "Mix", 0.0f, 1.0f, 1.0f
    ));
    
    // Choice parameter: mode (discrete enum)
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "mode", "Mode", 
        juce::StringArray("Telephone", "Radio", "Custom"),
        0  // default index
    ));
    
    // Bool parameter: enabled (bypass)
    layout.add(std::make_unique<juce::AudioParameterBool>(
        "enabled", "Enabled", true
    ));
    
    return layout;
}
```

**Critical Rules**:
- Parameter IDs must be **unique** and **stable** (used in state save/load)
- Default values must be explicitly specified
- Float parameters: range 0.0–1.0 is normalized; JUCE handles scaling
- Choice parameters: names array must match enum order

### 2.2 GUI Attachment Pattern

In the **editor**, attach APVTS to GUI controls **without manual synchronization**:

```cpp
class PluginEditor : public juce::AudioProcessorEditor {
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    
    // Member variables:
    juce::Slider mixSlider;
    std::unique_ptr<SliderAttachment> mixAttachment;
    
    juce::ComboBox modeCombo;
    std::unique_ptr<ComboBoxAttachment> modeAttachment;
    
    PluginEditor(PluginProcessor& p) : processor(p) {
        // Create attachment (auto-connects bidirectionally)
        mixAttachment = std::make_unique<SliderAttachment>(
            processor.apvts, "mix", mixSlider
        );
        
        modeAttachment = std::make_unique<ComboBoxAttachment>(
            processor.apvts, "mode", modeCombo
        );
    }
};
```

**Critical Rules**:
- Attachments are **bidirectional**: GUI updates APVTS, APVTS updates GUI
- Must be stored as member variables (destroyed with editor)
- No manual `SliderListener` needed
- Thread-safe parameter reads/writes guaranteed by JUCE

### 2.3 Reading Parameters in Audio Thread

In `processBlock()`, read parameters safely:

```cpp
void processBlock(juce::AudioBuffer<float>& buffer, 
                  juce::MidiBuffer& midiMessages) override {
    auto enabled = apvts.getRawParameterValue("enabled")->load();
    auto mix = apvts.getRawParameterValue("mix")->load();
    auto mode = static_cast<int>(apvts.getRawParameterValue("mode")->load());
    
    if (!enabled) {
        buffer.clear();
        return;
    }
    
    // Process audio using mix and mode...
}
```

**Critical Rules**:
- Use `getRawParameterValue()` (returns atomic pointer)
- Call `.load()` to read; no memory allocation
- This is **real-time safe**
- No listener callbacks in audio thread

**Source**: JUCE APVTS Tutorial & AudioProcessorValueTreeState API

---

## 3. CMake Build Configuration for JUCE

### 3.1 Root CMakeLists.txt Structure

```cmake
cmake_minimum_required(VERSION 3.16)
project(paranoidFilteroid VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE
add_subdirectory(path/to/JUCE JUCE)

# Create plugin target
juce_add_plugin(paranoidFilteroid
    PRODUCT_NAME "paranoidFilteroid"
    COMPANY_NAME "RetardedMuffin"
    VERSION 1.0.0
    DESCRIPTION "Radio/phone voice-like effect VST plugin"
    PLUGIN_MANUFACTURER_CODE Rmff
    PLUGIN_CODE Pfld
    FORMATS VST3
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST_COPY_DIR "C:/Program Files/Common Files/VST3"
)

# Add sources
target_sources(paranoidFilteroid PRIVATE
    Source/core/PluginProcessor.cpp
    Source/core/PluginEditor.cpp
)

# Link JUCE modules
target_link_libraries(paranoidFilteroid PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_processors
    juce::juce_gui_extra
    juce::juce_dsp
)
```

**Critical Points**:
- `PLUGIN_MANUFACTURER_CODE`: 4-character unique ID (check registry)
- `PLUGIN_CODE`: 4-character unique plugin ID
- `VST_COPY_DIR`: Plugin auto-copied after build
- `FORMATS VST3`: Only enable VST3 initially (VST2 legacy, AU macOS only)
- `CMAKE_CXX_STANDARD`: Must be ≥ 17 for modern JUCE

### 3.2 JUCE Module Selection

**Recommended minimal set**:
- `juce_audio_basics`: Audio buffer, channel types
- `juce_audio_processors`: AudioProcessor base class
- `juce_audio_utils`: Debugging, thread utilities
- `juce_gui_extra`: Component, painter, slider, combobox
- `juce_dsp`: Filters, processing primitives

### 3.3 Build Command Sequence (Windows)

```bash
# 1. Configure CMake (generates Visual Studio project)
cd build
cmake .. -G "Visual Studio 17 2022" -A x64

# 2. Compile (Debug first for development)
cmake --build . --config Debug

# 3. Compile (Release for distribution)
cmake --build . --config Release

# 4. Plugin auto-copied to VST3 folder (if VST_COPY_DIR set)
```

**Critical Points**:
- Always use `-A x64` for 64-bit builds
- Release build recommended for final testing (optimization matters for RT)
- Plugin DLL will be placed in `C:\Program Files\Common Files\VST3\`

**Source**: JUCE CMake API, JUCE Projucer documentation

---

## 4. Windows VST3 Plugin Discovery & Loading

### 4.1 Plugin Registry & Directory Structure

VST3 plugins on Windows are organized in a standard directory:

```
C:\Program Files\Common Files\VST3\
├── paranoidFilteroid.vst3\           (folder, like a bundle)
│   ├── Contents\
│   │   ├── Resources\
│   │   └── x86_64-win\               (platform-specific binaries)
│   │       ├── paranoidFilteroid.dll  (main plugin binary)
│   │       └── paranoidFilteroid.lib
│   └── ...
```

**DAW Discovery Process**:
1. DAW scans `C:\Program Files\Common Files\VST3\`
2. Reads `paranoidFilteroid.vst3\Contents\x86_64-win\paranoidFilteroid.dll`
3. Loads DLL, calls plugin factory function
4. Caches plugin info in registry/cache

**Critical Points**:
- VST3 uses **folder bundles** (not bare DLLs like VST2)
- JUCE CMake handles this structure automatically
- Plugin name comes from `juce_add_plugin(PRODUCT_NAME "...")`
- Manufacturer code must be unique (4 chars)

### 4.2 Plugin Factory Function

JUCE **automatically generates** the factory via macro:

```cpp
// In your PluginProcessor.cpp, JUCE requires this definition:
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}

// JUCE's CMake + VST3 wrapper generates:
// - Entry point (.dll exports)
// - Factory registration
// - PluginFactory.cpp wrapper
```

**Critical Points**:
- You must **define `createPluginFilter()`** in one `.cpp` file
- JUCE CMake/Projucer generates the VST3 wrapper automatically
- No manual registration needed

**Source**: JUCE Plugin Wrapper documentation, VST3 SDK

---

## 5. Real-Time Safety & DSP Fundamentals

### 5.1 Denormal Numbers

**Problem**: Very small floating-point numbers (< 1e-37) are slow to process.

**Solution**:
```cpp
// Enable in prepareToPlay()
juce::ScopedNoDenormals noDenormals;
```

Or globally in plugin:
```cpp
juce::FloatVectorOperations::enableFlushToZeroMode(true);
```

### 5.2 Memory Allocation Rules

**FORBIDDEN in `processBlock()`**:
- `new`, `malloc()`, `std::vector::resize()`
- File I/O, locks, or any blocking operations
- String construction, network calls

**ALLOWED in `prepareToPlay()`**:
- Pre-allocate buffers
- Initialize DSP objects
- Set up filter coefficients

**Pattern**:
```cpp
class PluginProcessor : public juce::AudioProcessor {
    // Pre-allocated buffers (safe)
    juce::AudioBuffer<float> scratchBuffer;
    
    void prepareToPlay(double sampleRate, int samplesPerBlock) override {
        // Can allocate here
        scratchBuffer.setSize(2, samplesPerBlock);
    }
    
    void processBlock(juce::AudioBuffer<float>& buffer, 
                      juce::MidiBuffer& midiMessages) override {
        // Cannot allocate; use pre-allocated scratchBuffer
        int numSamples = buffer.getNumSamples();
        auto* leftIn = buffer.getReadPointer(0);
        auto* rightIn = buffer.getReadPointer(1);
        // ...process...
    }
};
```

### 5.3 Thread Safety

**JUCE guarantees**:
- APVTS parameter reads are atomic (thread-safe)
- Audio thread is real-time; GUI thread is not
- No locks allowed in audio thread

**Pattern**:
```cpp
// Safe in audio thread
auto value = apvts.getRawParameterValue("paramID")->load();

// NOT safe in audio thread
apvts.state.setProperty("key", value, nullptr);  // Uses locks!
```

**Source**: JUCE Real-Time Audio Plugin Dev Best Practices, Audio Programming Standards

---

## 6. Plugin State Save/Load (APVTS State)

### 6.1 Automatic State Management

APVTS handles save/load automatically:

```cpp
void getStateInformation(juce::MemoryBlock& destData) override {
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void setStateInformation(const void* data, int sizeInBytes) override {
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr) {
        if (xmlState->hasTagName(apvts.state.getType())) {
            apvts.replaceState(*xmlState);
        }
    }
}
```

**Critical Points**:
- APVTS saves all parameter values as XML
- DAW calls these methods on session save/load
- User settings persist across sessions
- JUCE provides binary XML helpers

**Source**: JUCE State Management API

---

## 7. GUI Component Basics (JUCE)

### 7.1 Editor Component Hierarchy

```cpp
class PluginEditor : public juce::AudioProcessorEditor,
                     private juce::Timer {
    PluginProcessor& processor;
    
    // GUI components:
    juce::Slider mixSlider;
    juce::ComboBox modeCombo;
    juce::ToggleButton enabledToggle;
    juce::Label mixLabel;
    
    // Attachments:
    std::unique_ptr<SliderAttachment> mixAttachment;
    std::unique_ptr<ComboBoxAttachment> modeAttachment;
    std::unique_ptr<ButtonAttachment> enabledAttachment;
    
public:
    PluginEditor(PluginProcessor& p) : processor(p) {
        // Set editor size
        setSize(400, 300);
        
        // Add components
        addAndMakeVisible(mixSlider);
        addAndMakeVisible(modeCombo);
        addAndMakeVisible(enabledToggle);
        
        // Set properties
        mixSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        mixSlider.setRange(0.0, 1.0, 0.01);
        
        modeCombo.addItem("Telephone", 1);
        modeCombo.addItem("Radio", 2);
        modeCombo.addItem("Custom", 3);
        
        // Attach (auto-sync)
        mixAttachment = std::make_unique<SliderAttachment>(
            processor.apvts, "mix", mixSlider
        );
        // ... other attachments
        
        startTimer(30);  // ~30 Hz repaint
    }
    
    void resized() override {
        auto area = getLocalBounds();
        mixSlider.setBounds(area.removeFromTop(100));
        modeCombo.setBounds(area.removeFromTop(40));
        enabledToggle.setBounds(area.removeFromTop(40));
    }
    
    void paint(juce::Graphics& g) override {
        g.fillAll(getLookAndFeel().findColour(
            juce::ResizableWindow::backgroundColourId
        ));
    }
};
```

**Critical Points**:
- Always call `addAndMakeVisible()` for components
- `resized()` called on window size change
- `paint()` called on repaint; keep it fast
- Attachments handle all parameter sync

**Source**: JUCE Component/Editor API, JUCE GUI Tutorial

---

## 8. Audio Buffer Management

### 8.1 Stereo Buffer Processing

```cpp
void processBlock(juce::AudioBuffer<float>& buffer, 
                  juce::MidiBuffer& midiMessages) override {
    auto numSamples = buffer.getNumSamples();
    auto numChannels = buffer.getNumChannels();
    
    // Channel iteration pattern
    for (int ch = 0; ch < numChannels; ++ch) {
        auto* channelData = buffer.getWritePointer(ch);
        
        for (int n = 0; n < numSamples; ++n) {
            // Process each sample
            channelData[n] = processedValue;
        }
    }
}
```

**Critical Points**:
- `getWritePointer()` for in-place processing
- `getReadPointer()` to read without modifying
- Buffer is interleaved by channel internally; JUCE abstracts this
- Always respect `getNumSamples()` and `getNumChannels()`

---

## 9. JUCE Module Dependencies

### 9.1 Minimal Modules for Plugin

| Module | Purpose | Required |
|--------|---------|----------|
| `juce_audio_basics` | Audio buffer, channels | ✅ Yes |
| `juce_audio_processors` | AudioProcessor base | ✅ Yes |
| `juce_audio_utils` | Debugging, threading | ✅ Yes |
| `juce_gui_extra` | Components, sliders | ✅ Yes (if GUI) |
| `juce_dsp` | Filters, waveshapers | ✅ Yes (for DSP) |
| `juce_gui_basics` | Graphics, painter | ✅ Yes |
| `juce_core` | Memory, threading, utilities | ✅ (auto-linked) |

### 9.2 Including in Code

```cpp
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_dsp/juce_dsp.h>

// Or use JUCE macro (in CMakeLists.txt):
// juce_add_plugin(...) handles includes
```

---

## 10. VST3 Plugin Metadata

### 10.1 Required Plugin Information

```cpp
const juce::String PluginProcessor::getName() const override {
    return JucePlugin_Name;  // Set by CMake
}

bool PluginProcessor::acceptsMidi() const override {
    return false;  // Telephone effect: audio-only
}

bool PluginProcessor::producesMidi() const override {
    return false;
}

int PluginProcessor::getNumPrograms() const override {
    return 1;  // Single program (no presets yet)
}

int PluginProcessor::getCurrentProgram() const override {
    return 0;
}

void PluginProcessor::setCurrentProgram(int index) override {
    // No-op for minimal plugin
}

const juce::String PluginProcessor::getProgramName(int index) const override {
    return "Default";
}

void PluginProcessor::changeProgramName(int index, const juce::String& newName) override {
    // No-op for minimal plugin
}

double PluginProcessor::getTailLengthInSeconds() const override {
    return 0.0;  // No reverb tail; audio stops immediately
}
```

**Critical Points**:
- Metadata affects DAW display and behavior
- Audio-only effect: `acceptsMidi() = false`
- No tail for pass-through plugin
- Programs/presets optional for minimal version

---

## 11. Validation & Testing Checklist

### 11.1 Build Validation
- [ ] CMake configures without errors (no missing modules)
- [ ] Compiler errors: 0, Warnings: 0 (or non-critical)
- [ ] `.vst3` binary created (check `C:\Program Files\Common Files\VST3\`)
- [ ] Plugin appears in DAW plugin list (rescan if needed)

### 11.2 Plugin Loading
- [ ] Double-click plugin in DAW → editor window opens
- [ ] No crash on load
- [ ] Sliders and comboboxes visible and clickable
- [ ] Mode combobox shows: Telephone, Radio, Custom

### 11.3 Audio Processing
- [ ] Play audio through plugin (bypass OFF)
- [ ] Audio passes through unchanged (amplitude preserved)
- [ ] Toggle bypass ON → audio mutes
- [ ] Toggle bypass OFF → audio returns
- [ ] No pops, clicks, or artifacts

### 11.4 Parameter Control
- [ ] Move "mix" slider → value updates (0–100%)
- [ ] Change "mode" combobox → updates without crash
- [ ] Click "enabled" toggle → bypass works
- [ ] Parameter values persist on plugin reload

### 11.5 Real-Time Safety
- [ ] Play long audio file (5+ min) → no crashes or dropouts
- [ ] Switch modes rapidly → no pops or glitches
- [ ] CPU usage < 2% (baseline for pass-through)
- [ ] No buffer underruns (DAW shows no X marks)

---

## 12. Open Questions & Decisions

1. **JUCE Version**: Use latest stable (8.1.x) or 7.0.x LTS?
   - **Answer**: Recommend 8.1.x (better CMake support, VST3 compliance)

2. **CMake Generator**: Visual Studio 2022 vs 2019?
   - **Answer**: VS 2022 (latest, better C++17 support)

3. **Debug vs Release Build**: Which to test first?
   - **Answer**: Debug first (easier debugging); Release for final testing

4. **Plugin Manufacturer Code**: What should `PLUGIN_MANUFACTURER_CODE` be?
   - **Answer**: "Rmff" (suggested: RetardedMuffin); must be 4 chars, unique

5. **GUI Size**: Recommendation for initial window size?
   - **Answer**: 400×300 pixels (standard; can be resizable later)

6. **State Save Format**: XML or binary?
   - **Answer**: JUCE APVTS uses XML internally; DAW may compress

---

## 13. Summary of Verified Information

✅ **Verified Facts**:
- JUCE AudioProcessor is the industry standard (used by FL Studio, Reaper plugins, etc.)
- APVTS thread-safe parameter system is recommended by JUCE team
- CMake is now the official JUCE build system (Projucer is legacy)
- VST3 is the modern standard; VST2 deprecated
- Windows VST3 plugins in `.vst3` bundle format
- Real-time safety rules (no allocations in audio thread) are enforced by audio standards
- JUCE denormal handling is built-in and recommended

✅ **Implementation Options** (all verified):
1. **Parameter System**: APVTS (JUCE built-in, recommended)
2. **Build System**: CMake (official, modern)
3. **GUI Framework**: JUCE Components (built-in, no external dependencies)
4. **Plugin Wrapper**: VST3 (via JUCE CMake macros)

---

## 14. Sources & References

| Topic | Source | Confidence |
|-------|--------|-----------|
| AudioProcessor API | JUCE Official Docs (C++ reference) | ✅ High |
| APVTS | JUCE Tutorial & API Reference | ✅ High |
| CMake Setup | JUCE CMake documentation | ✅ High |
| VST3 Spec | Steinberg VST3 SDK docs | ✅ High |
| Real-Time Safety | JUCE Best Practices guide | ✅ High |
| Audio Buffer Processing | DSP textbooks (Oppenheim & Schafer) | ✅ High |
| Windows Plugin Discovery | VST3 spec, Steinberg docs | ✅ High |

---

## 15. Next Steps

Based on this research, the implementation is now ready to proceed:

1. ✅ Verified JUCE architecture is sound and industry-standard
2. ✅ APVTS provides thread-safe parameter management
3. ✅ CMake + VST3 is straightforward and automated
4. ✅ Real-time safety rules are clear and enforceable
5. ✅ Plugin discovery and loading is predictable

**Ready to begin**:
- [ ] Create directory structure
- [ ] Write `CMakeLists.txt`
- [ ] Implement `PluginProcessor` (header + source)
- [ ] Implement `PluginEditor` (header + source)
- [ ] Build and test in DAW

