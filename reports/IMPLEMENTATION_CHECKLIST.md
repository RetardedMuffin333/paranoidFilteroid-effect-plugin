# Implementation Checklist & Build Guide
**paranoidFilteroid-effect-plugin - Minimal Setup Phase**

**Status**: Ready to Build  
**Last Updated**: November 28, 2025

---

## Phase 1: Environment & Prerequisites

### 1.1 Tool Verification
- [ ] **CMake** (≥ 3.16)
  - Command: `cmake --version` 
  - Expected: `cmake version 3.16.x` or higher
  - Action if missing: Download from https://cmake.org/download/

- [ ] **Visual Studio 2022 Community** (or Pro)
  - Command: `"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.4x\bin\Hostx64\x64\cl.exe" /version`
  - Expected: Microsoft C/C++ Optimizing Compiler
  - Action if missing: Download from https://visualstudio.microsoft.com/downloads/

- [ ] **JUCE Library** (v8.1.x recommended)
  - Location: Download to `C:\dev\JUCE` or similar
  - Command: `git clone https://github.com/juce-framework/JUCE.git --branch 8.1.0`
  - Expected: JUCE folder with `CMakeLists.txt`, `modules/`, `examples/`
  - Action if missing: Clone JUCE repository

- [ ] **Git** (for version control)
  - Command: `git --version`
  - Expected: `git version 2.x.x`
  - Action if missing: Download from https://git-scm.com/

### 1.2 Environment Setup
- [ ] Create base directory: `C:\dev\` (or your preferred location)
- [ ] Clone JUCE: `git clone https://github.com/juce-framework/JUCE.git --branch 8.1.0 C:\dev\JUCE`
- [ ] Verify JUCE structure:
  ```
  C:\dev\JUCE\
  ├── CMakeLists.txt
  ├── modules/
  ├── examples/
  ├── extras/
  └── docs/
  ```

---

## Phase 2: Repository Structure & CMake Setup

### 2.1 Create Directory Tree
- [ ] Create `c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin\Source\`
- [ ] Create `c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin\Source\core\`
- [ ] Create `c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin\Source\dsp\`
- [ ] Create `c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin\Source\utils\`
- [ ] Create `c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin\cmake\`
- [ ] Create `c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin\build\` (build output directory)

**Verify structure:**
```bash
cd c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin
dir /s
# Output should show: Source\core, Source\dsp, Source\utils, cmake, build
```

### 2.2 Create Root CMakeLists.txt
**File**: `c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin\CMakeLists.txt`

**Content** (copy exactly):
```cmake
cmake_minimum_required(VERSION 3.16)
project(paranoidFilteroid VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE as subdirectory
# UPDATE THIS PATH to match your JUCE installation
add_subdirectory(C:/dev/JUCE JUCE)

# Create plugin target
juce_add_plugin(paranoidFilteroid
    PRODUCT_NAME "paranoidFilteroid"
    COMPANY_NAME "RetardedMuffin"
    VERSION 1.0.0
    DESCRIPTION "Radio/phone voice-like effect VST plugin"
    
    # Plugin identifiers (4 chars each, must be unique)
    PLUGIN_MANUFACTURER_CODE Rmff
    PLUGIN_CODE Pfld
    
    # Audio format
    FORMATS VST3
    
    # Plugin type configuration
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    
    # Build options
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST_COPY_DIR "C:/Program Files/Common Files/VST3"
)

# Add source files
target_sources(paranoidFilteroid PRIVATE
    Source/core/PluginProcessor.cpp
    Source/core/PluginEditor.cpp
)

# Include directories
target_include_directories(paranoidFilteroid PRIVATE
    Source/
)

# Link JUCE modules
target_link_libraries(paranoidFilteroid PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_dsp
)

# Set compiler flags
if(MSVC)
    target_compile_options(paranoidFilteroid PRIVATE /W4)
else()
    target_compile_options(paranoidFilteroid PRIVATE -Wall -Wextra)
endif()
```

**Validation**:
- [ ] File created at correct path
- [ ] JUCE path matches your installation (edit if needed)
- [ ] No syntax errors (CMake will catch on configure)

---

## Phase 3: PluginProcessor Header

### 3.1 Create PluginProcessor.h
**File**: `Source/core/PluginProcessor.h`

**Content** (copy exactly):
```cpp
#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

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
    double getTailLengthInSeconds() const override { return 0.0; }

    //==========================================================================
    int getNumPrograms() const override { return 1; }
    int getCurrentProgram() const override { return 0; }
    void setCurrentProgram(int index) override {}
    const juce::String getProgramName(int index) const override { return "Default"; }
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

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
```

**Validation**:
- [ ] File created at `Source/core/PluginProcessor.h`
- [ ] No syntax errors in IDE (should show no red squiggles)
- [ ] Header guards present

### 3.2 Create PluginProcessor.cpp (Part 1: Constructor & Parameter Layout)
**File**: `Source/core/PluginProcessor.cpp` (FIRST PART)

**Content** (copy exactly):
```cpp
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
```

**Validation**:
- [ ] File created at `Source/core/PluginProcessor.cpp`
- [ ] No syntax errors (IDE check)
- [ ] Parameter layout includes: mode, mix, enabled
- [ ] APVTS initialized with layout

---

## Phase 4: PluginEditor Header & Implementation

### 4.1 Create PluginEditor.h
**File**: `Source/core/PluginEditor.h`

**Content** (copy exactly):
```cpp
#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "PluginProcessor.h"

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor {
public:
    explicit PluginEditor(PluginProcessor&);
    ~PluginEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    //==========================================================================
    // Attachment types
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    //==========================================================================
    PluginProcessor& processor;

    // GUI Components
    juce::ComboBox modeCombo;
    juce::Label modeLabel;

    juce::Slider mixSlider;
    juce::Label mixLabel;

    juce::ToggleButton enabledButton;

    // Attachments (must be members to keep alive)
    std::unique_ptr<ComboBoxAttachment> modeAttachment;
    std::unique_ptr<SliderAttachment> mixAttachment;
    std::unique_ptr<ButtonAttachment> enabledAttachment;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
```

**Validation**:
- [ ] File created at `Source/core/PluginEditor.h`
- [ ] No syntax errors
- [ ] Three attachment types defined

### 4.2 Create PluginEditor.cpp
**File**: `Source/core/PluginEditor.cpp`

**Content** (copy exactly):
```cpp
#include "PluginEditor.h"

//==============================================================================
PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p), processor(p) {
    // Set editor size
    setSize(400, 300);

    // Mode ComboBox
    modeCombo.addItem("Telephone", 1);
    modeCombo.addItem("Radio", 2);
    modeCombo.addItem("Custom", 3);
    modeCombo.setSelectedItemIndex(0);
    addAndMakeVisible(modeCombo);

    modeLabel.setText("Mode:", juce::dontSendNotification);
    modeLabel.attachToComponent(&modeCombo, true);
    addAndMakeVisible(modeLabel);

    // Mix Slider
    mixSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mixSlider.setRange(0.0, 1.0, 0.01);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    addAndMakeVisible(mixSlider);

    mixLabel.setText("Mix:", juce::dontSendNotification);
    mixLabel.attachToComponent(&mixSlider, true);
    addAndMakeVisible(mixLabel);

    // Enabled Toggle Button
    enabledButton.setButtonText("Enabled");
    enabledButton.setToggleState(true, juce::dontSendNotification);
    addAndMakeVisible(enabledButton);

    // Create APVTS attachments
    modeAttachment = std::make_unique<ComboBoxAttachment>(
        processor.apvts, "mode", modeCombo
    );

    mixAttachment = std::make_unique<SliderAttachment>(
        processor.apvts, "mix", mixSlider
    );

    enabledAttachment = std::make_unique<ButtonAttachment>(
        processor.apvts, "enabled", enabledButton
    );
}

PluginEditor::~PluginEditor() {
}

//==============================================================================
void PluginEditor::paint(juce::Graphics& g) {
    // Fill background
    g.fillAll(getLookAndFeel().findColour(
        juce::ResizableWindow::backgroundColourId
    ));
}

void PluginEditor::resized() {
    auto area = getLocalBounds().reduced(20);

    // Mode ComboBox (with label space)
    auto modeArea = area.removeFromTop(30);
    modeCombo.setBounds(modeArea.removeFromLeft(100));

    // Mix Slider (with label space)
    area.removeFromTop(10);
    auto mixArea = area.removeFromTop(30);
    mixSlider.setBounds(mixArea.removeFromLeft(200));

    // Enabled Toggle Button
    area.removeFromTop(10);
    auto enabledArea = area.removeFromTop(30);
    enabledButton.setBounds(enabledArea.removeFromLeft(100));
}
```

**Validation**:
- [ ] File created at `Source/core/PluginEditor.cpp`
- [ ] No syntax errors
- [ ] All three controls attached to APVTS
- [ ] resized() positions all components

---

## Phase 5: Placeholder DSP Files

### 5.1 Create DSPDefines.h
**File**: `Source/utils/DSPDefines.h`

**Content**:
```cpp
#pragma once

#include <cmath>

// DSP constants
namespace DSP {
    // Telephone mode specs (per DSP_SPEC.md)
    constexpr float TELEPHONE_LOW_CUT_HZ = 300.0f;
    constexpr float TELEPHONE_HIGH_CUT_HZ = 3400.0f;

    // Radio mode specs (per DSP_SPEC.md)
    constexpr float RADIO_LOW_CUT_HZ = 200.0f;
    constexpr float RADIO_HIGH_CUT_HZ = 5000.0f;

    // Mode enum
    enum class Mode {
        Telephone = 0,
        Radio = 1,
        Custom = 2
    };
}
```

**Validation**:
- [ ] File created at `Source/utils/DSPDefines.h`
- [ ] No syntax errors

### 5.2 Create PassThrough.h (Placeholder)
**File**: `Source/dsp/PassThrough.h`

**Content**:
```cpp
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
```

**Validation**:
- [ ] File created at `Source/dsp/PassThrough.h`
- [ ] No syntax errors

---

## Phase 6: CMake Configuration & Build

### 6.1 Configure CMake Project
**Command** (run in terminal):
```bash
cd c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin\build
cmake .. -G "Visual Studio 17 2022" -A x64
```

**Expected Output**:
```
-- The CXX compiler identification is MSVC 19.3x.xxxxx
-- Detecting CXX compiler ABI info
-- ...
-- Configuring done
-- Generating build files for Visual Studio 17 2022
```

**Validation**:
- [ ] No CMake errors (check for red text like "JUCE not found")
- [ ] Visual Studio project files generated in `build/` folder
- [ ] No "FATAL_ERROR" messages

### 6.2 Compile Debug Build
**Command**:
```bash
cd build
cmake --build . --config Debug --parallel 4
```

**Expected Output** (end of build):
```
Build succeeded.
0 Warning(s)
0 Error(s)
```

**Validation**:
- [ ] Build completes successfully
- [ ] No compiler errors
- [ ] `.vst3` bundle created in `C:\Program Files\Common Files\VST3\paranoidFilteroid.vst3\`

### 6.3 Verify Plugin Binary
**Command**:
```bash
dir "C:\Program Files\Common Files\VST3\paranoidFilteroid.vst3\Contents\x86_64-win\"
```

**Expected Output**:
```
paranoidFilteroid.dll
paranoidFilteroid.lib
```

**Validation**:
- [ ] `.dll` file exists and has non-zero size (> 1 MB)
- [ ] Both `.dll` and `.lib` present

---

## Phase 7: DAW Testing & Validation

### 7.1 Load Plugin in DAW

**Reaper (recommended for testing)**:
1. Open Reaper
2. Create new project
3. Insert audio track: `Track → Insert new track`
4. Open FX browser: Click "FX" button on track
5. Search for "paranoidFilteroid"
6. Double-click to add plugin

**Expected Result**:
- [ ] Plugin window opens
- [ ] Editor shows three controls: Mode combobox, Mix slider, Enabled toggle
- [ ] No crash

**If plugin not found**:
- [ ] Rescan plugins: `Options → Show MIDI/FX browser → Refresh`
- [ ] Check VST3 folder: `C:\Program Files\Common Files\VST3\paranoidFilteroid.vst3\`

### 7.2 Test Audio Pass-Through

1. **Create audio input**:
   - Record a test tone (e.g., 1 kHz sine wave in Audacity)
   - Export as WAV: `Export → WAV`
   - Save to Desktop

2. **Route audio to plugin**:
   - Drag WAV file onto Reaper track
   - Click track FX button, add paranoidFilteroid plugin

3. **Test with bypass OFF** (enabled = true):
   - Play audio → should hear tone unchanged
   - Amplitude should match input

4. **Test with bypass ON** (enabled = false):
   - Toggle enabled button → plugin should mute
   - Play audio → should hear silence

**Validation**:
- [ ] Audio passes through without distortion when enabled
- [ ] Audio mutes when disabled
- [ ] No pops or clicks when toggling

### 7.3 Test Parameter Controls

1. **Test Mode ComboBox**:
   - Select "Telephone"
   - Select "Radio"
   - Select "Custom"
   - Verify each selection sticks (no crash)

2. **Test Mix Slider**:
   - Drag slider left (0%)
   - Drag slider right (100%)
   - Check that value updates in real-time
   - Value should not "snap" or jump

3. **Test Enabled Toggle**:
   - Click toggle button multiple times
   - Audio should mute/unmute responsively

**Validation**:
- [ ] All parameters respond to input
- [ ] No crashes on parameter changes
- [ ] No zipper noise (parameter changes smooth)

### 7.4 Test State Save/Load

1. **Save plugin state**:
   - Adjust all controls (mode, mix, toggle)
   - Save Reaper project: `File → Save Project`

2. **Reload project**:
   - Close and reopen Reaper project
   - Check that all parameter values are restored

**Validation**:
- [ ] Mode combobox shows same selection after reload
- [ ] Mix slider at same position after reload
- [ ] Toggle button at same state after reload

### 7.5 Test Multiple Instances

1. **Create second audio track**
2. **Add paranoidFilteroid plugin to second track**
3. **Adjust parameters on both tracks differently**
4. **Play both tracks simultaneously**

**Validation**:
- [ ] Both plugins run independently (no cross-talk)
- [ ] Each has its own parameter state
- [ ] No crashes with multiple instances

---

## Phase 8: Performance & Stability

### 8.1 CPU Usage Baseline

1. **Measure idle CPU**:
   - Create Reaper project with paranoidFilteroid on track
   - Play silence (muted track) for 30 seconds
   - Note CPU meter reading

2. **Measure with audio**:
   - Play 1 kHz tone through plugin
   - Check CPU meter reading

**Expected**:
- [ ] Idle CPU < 0.5%
- [ ] With audio CPU < 1% (pass-through is minimal)

### 8.2 Long-Duration Stability

1. **Play 10-minute audio file** through plugin
2. **Watch for**:
   - Buffer underruns (DAW shows red X or warning)
   - Crashes or freezes
   - CPU spikes

**Expected**:
- [ ] No crashes or dropouts
- [ ] Consistent CPU usage (no spikes)
- [ ] Audio clean throughout

### 8.3 Rapid Parameter Changes

1. **Quickly toggle** enabled button (click 10 times rapidly)
2. **Rapidly adjust** mix slider (drag back and forth)
3. **Rapidly switch** mode (click combobox options repeatedly)

**Expected**:
- [ ] No crashes
- [ ] No pops or clicks in audio
- [ ] Parameters respond immediately

**Validation**:
- [ ] All stress tests pass without crashes
- [ ] No audio artifacts
- [ ] CPU remains stable

---

## Phase 9: Git Version Control

### 9.1 Initialize Git Repository
**Command**:
```bash
cd c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin
git init
git add -A
git commit -m "Initial commit: minimal JUCE VST3 plugin setup"
```

**Validation**:
- [ ] Git repository initialized (`.git` folder created)
- [ ] All source files committed
- [ ] No untracked files (optional: create `.gitignore`)

### 9.2 Create .gitignore
**File**: `c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin\.gitignore`

**Content**:
```
build/
.vs/
*.user
CMakeUserPresets.json
CMakePresets.json.user
*.dll
*.lib
```

**Validation**:
- [ ] Build artifacts ignored (no `build/` in git)
- [ ] IDE files ignored (no `.vs/` in git)

---

## Final Checklist Summary

### ✅ Environment (Complete before proceeding)
- [ ] CMake installed (≥ 3.16)
- [ ] Visual Studio 2022 installed
- [ ] JUCE downloaded to `C:\dev\JUCE`
- [ ] Git installed

### ✅ Source Code (All files created)
- [ ] `CMakeLists.txt` with correct JUCE path
- [ ] `Source/core/PluginProcessor.h` and `.cpp`
- [ ] `Source/core/PluginEditor.h` and `.cpp`
- [ ] `Source/utils/DSPDefines.h`
- [ ] `Source/dsp/PassThrough.h`
- [ ] Directory structure: `Source/{core,dsp,utils}`, `cmake/`, `build/`

### ✅ Build & Compilation
- [ ] CMake configures without errors
- [ ] Debug build succeeds (0 errors)
- [ ] `.vst3` plugin binary created
- [ ] Binary located at `C:\Program Files\Common Files\VST3\paranoidFilteroid.vst3\Contents\x86_64-win\paranoidFilteroid.dll`

### ✅ Plugin Loading
- [ ] Plugin appears in DAW plugin list
- [ ] Plugin loads without crash
- [ ] Editor window opens with three controls visible

### ✅ Audio & Parameters
- [ ] Audio passes through without distortion
- [ ] Bypass toggle mutes/unmutes audio
- [ ] Mode combobox changes without crash
- [ ] Mix slider responds to input
- [ ] All parameters update APVTS correctly

### ✅ State Management
- [ ] Settings persist across plugin reload
- [ ] APVTS state saves/loads correctly

### ✅ Stability
- [ ] CPU usage < 1% (pass-through baseline)
- [ ] 10+ minute audio playback without crash
- [ ] Rapid parameter changes don't cause pops
- [ ] Multiple instances run independently

### ✅ Version Control
- [ ] Git repository initialized
- [ ] All source files committed
- [ ] `.gitignore` configured

---

## Troubleshooting Guide

| Problem | Cause | Solution |
|---------|-------|----------|
| CMake: "JUCE not found" | JUCE_PATH in CMakeLists.txt wrong | Update path to your JUCE installation |
| Compiler: "juce_audio_processors.h not found" | JUCE modules not linked | Check `target_link_libraries()` in CMakeLists.txt |
| Build fails with "cannot open output file" | Plugin already loaded in DAW | Close DAW and rebuild |
| Plugin not found in DAW | Rescan not performed | Right-click on plugin list and rescan |
| Plugin crashes on load | Missing header includes | Check all `#include` statements in `.h` files |
| Audio not passing through | `processBlock()` not implemented | Verify processBlock copies input to output |
| Controls don't update APVTS | Attachments not created | Check constructor creates attachments as members |
| Parameters don't persist | State save/load broken | Verify `getStateInformation()` and `setStateInformation()` |

---

## Next Phase: DSP Implementation

Once all validations pass, proceed to:

1. Implement **Band-Pass Filters** (HP @ 300 Hz + LP @ 3400 Hz for Telephone mode)
2. Implement **Mid-Peak EQ** (parametric boost at 1 kHz)
3. Implement **Compressor** (threshold, ratio, attack, release)
4. Implement **Saturation Waveshaper** (soft-knee distortion)
5. Implement **Noise Generator** (low-level background noise)
6. Implement **Mode Presets** (Telephone, Radio, Custom switch defaults)

Each DSP module will be added following this same validation pattern.

---

## Sign-Off

**Status**: Ready to Build ✅  
**Last Verified**: November 28, 2025  
**Next Action**: Begin Phase 1 — Environment Verification

