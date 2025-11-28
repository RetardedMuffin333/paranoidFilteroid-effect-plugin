# Minimal JUCE Audio Plugin Setup Plan
**paranoidFilteroid-effect-plugin**

---

## 1. Goal

Establish a minimal, buildable JUCE VST3 audio plugin project that:
- Compiles cleanly on Windows (CMake-based)
- Loads in a DAW (Reaper, FL Studio, Ableton, etc.)
- Includes a pass-through audio processor with APVTS parameter system
- Provides a foundation for adding DSP modules and GUI components
- Follows the repo's architecture guidelines from CLAUDE_RULEBOOK.md

---

## 2. Requirements & Constraints

### 2.1 Technical Requirements
- **Framework**: JUCE (latest stable version recommended: 8.x or 7.x)
- **Build System**: CMake (cross-platform, maintainable)
- **Plugin Format**: VST3 (primary target; VST2/AU optional)
- **Target Platform**: Windows (x64 initially)
- **Audio I/O**: Stereo in, stereo out
- **Sample Rate**: Support 44.1 kHz, 48 kHz, and higher
- **Buffer Sizes**: Support variable buffer sizes (64–4096 samples)

### 2.2 Repository Architecture
Per CLAUDE_RULEBOOK.md (Section 9), project structure must be:
```
Source/
  ├── dsp/          ← DSP modules (filters, EQ, compressor, saturation, noise)
  ├── core/         ← Processor, editor, application glue
  └── utils/        ← Helper utilities
```

### 2.3 APVTS Parameter System
Must be minimal but complete:
- `mode` (enum: Telephone, Radio, Custom)
- `mix` (0–100%, wet/dry balance)
- `enabled` (bypass flag)

(Additional parameters like filters, EQ, compressor will be added in later phases)

### 2.4 Stability & Safety Rules
- **Real-time Safety**: No heap allocations in audio callback
- **Thread Safety**: All DSP state accessed safely
- **No Pops/Clicks**: Parameter changes smoothed appropriately
- **Denormal Safety**: Use JUCE denormal handling or check for denormals

### 2.5 Build & Validation
- **Must compile**: No errors or warnings (with /Wall or -Wall)
- **Must load**: Plugin discoverable and loadable in DAW
- **Must pass audio**: No crashes, DC offset, or audio dropout
- **Must not hang**: Parameter changes responsive

---

## 3. Detailed Step-by-Step Plan

### **PHASE 1: Project Initialization**

#### Step 1.1 – Environment Verification
- **What**: Verify required tools are installed and accessible
- **Tools Needed**:
  - CMake (≥ 3.16)
  - Visual Studio 2022 (Community or Pro) with MSVC 2022
  - JUCE source code (download or clone from GitHub)
  - Git (for version control)
- **Action**:
  - Run `cmake --version` in terminal (should report ≥ 3.16)
  - Run `msbuild /version` in Visual Studio terminal (MSVC installed)
  - Download/clone JUCE: https://github.com/juce-framework/JUCE (tag: latest stable, e.g., `8.1.0`)
- **Deliverable**: Confirmed tool availability
- **Validation**: All commands execute without "not found" errors

#### Step 1.2 – Create Directory Structure
- **What**: Initialize repo directories following CLAUDE_RULEBOOK.md architecture
- **Action**:
  ```
  c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin\
  ├── Source/
  │   ├── dsp/
  │   ├── core/
  │   └── utils/
  ├── cmake/
  ├── build/
  └── CMakeLists.txt  (root)
  ```
- **Deliverable**: Directory tree ready for source files
- **Validation**: All folders exist and are empty/writable

#### Step 1.3 – Create Root CMakeLists.txt
- **What**: Set up CMake project configuration
- **Key Configuration**:
  - Project name: `paranoidFilteroid`
  - JUCE modules: `juce_audio_basics`, `juce_audio_processors`, `juce_gui_extra`, `juce_dsp`
  - C++ standard: C++17 or C++20
  - Plugin format: VST3
  - Target: Mono and Stereo plugin instances
- **Action**:
  - Create `CMakeLists.txt` in root
  - Configure plugin metadata (name, manufacturer, version)
  - Add JUCE subdirectory
  - Define plugin binary target
- **Deliverable**: Root-level CMakeLists.txt
- **Validation**: CMake configuration passes without errors

#### Step 1.4 – Create Minimal Source Files Structure
- **What**: Stub out empty source file hierarchy
- **Files to Create**:
  - `Source/core/PluginProcessor.h` / `.cpp` (audio processor)
  - `Source/core/PluginEditor.h` / `.cpp` (GUI editor)
  - `Source/utils/DSPDefines.h` (shared constants, enums)
  - `Source/dsp/PassThrough.h` (minimal DSP placeholder)
- **Action**:
  - Create each file with basic header guards and includes
  - Add class skeletons (no implementation yet)
- **Deliverable**: All source files present with forward declarations
- **Validation**: Files compile (individually, via compilation test)

---

### **PHASE 2: Audio Processor Core**

#### Step 2.1 – Implement PluginProcessor Header
- **What**: Define processor class inheriting from `juce::AudioProcessor`
- **Key Members**:
  - `juce::AudioProcessorValueTreeState` (APVTS) for parameters
  - `createParameterLayout()` function
  - Core audio callback: `processBlock()`
  - Prepare/release audio functions
- **Action**:
  - Define class: `class PluginProcessor : public juce::AudioProcessor`
  - Add APVTS member
  - Declare parameter IDs as string constants
  - Declare virtual methods required by AudioProcessor
- **Deliverable**: `PluginProcessor.h` complete header
- **Validation**: Header compiles with AudioProcessor base class

#### Step 2.2 – Implement PluginProcessor Parameter Layout
- **What**: Define APVTS parameters (mode, mix, enabled)
- **Parameters**:
  - `mode`: Choice parameter (Telephone, Radio, Custom)
  - `mix`: Float parameter (0–1.0, default 1.0)
  - `enabled`: Bool parameter (default true)
- **Action**:
  - Implement `createParameterLayout()` function
  - Return layout with all three parameters properly defined
  - Use JUCE's parameter builders: `std::make_unique<juce::AudioParameterChoice>(…)`, etc.
- **Deliverable**: Populated parameter layout function
- **Validation**: APVTS attaches to layout without errors

#### Step 2.3 – Implement PluginProcessor Constructor & Destructor
- **What**: Initialize processor and APVTS
- **Action**:
  - Create APVTS in constructor with layout
  - Initialize any DSP state
  - Ensure proper cleanup in destructor
- **Deliverable**: Constructor/destructor implementation
- **Validation**: Object can be created and destroyed without crashes

#### Step 2.4 – Implement prepareToPlay & releaseResources
- **What**: Handle audio setup and teardown
- **Action**:
  - `prepareToPlay()`: Store sample rate, buffer size; initialize DSP blocks
  - `releaseResources()`: Clean up any temporary memory (if any)
- **Key Points**:
  - No persistent heap allocations during prepareToPlay (use stack or pre-allocated)
  - Store `sampleRate` for DSP calculations
- **Deliverable**: Both functions implemented
- **Validation**: Called without exceptions; sample rate available for DSP

#### Step 2.5 – Implement processBlock (Pass-Through)
- **What**: Minimal pass-through audio processing
- **Action**:
  - Read input buffer (left/right channels)
  - Copy to output buffer (or apply minimal processing)
  - Respect bypass (enabled parameter)
  - Handle MIDI (if applicable; initially just ignore)
- **Logic**:
  ```
  if (enabled) {
    // Pass audio through unchanged (DSP will be added later)
    for (auto ch = 0; ch < buffer.getNumChannels(); ++ch) {
      auto* data = buffer.getWritePointer(ch);
      // Process (initially just copy from input)
    }
  } else {
    buffer.clear();  // Bypass = silence
  }
  ```
- **Deliverable**: Pass-through processBlock
- **Validation**: Audio passes through; no crashes; enables/disables cleanly

#### Step 2.6 – Implement Plugin Metadata Methods
- **What**: Required by AudioProcessor interface
- **Methods**:
  - `getName()`: Return "paranoidFilteroid"
  - `acceptsMidi()`: false (initially)
  - `producesMidi()`: false
  - `isMidiEffect()`: false
  - `getTailLengthInSeconds()`: 0.0 (pass-through has no tail)
  - `getNumPrograms()`: 1 (default)
  - `getCurrentProgram()`: 0
  - `getProgramName()`: "Default"
  - `setCurrentProgram()`: No-op (for now)
  - `changeProgramName()`: No-op (for now)
  - `getStateInformation()` / `setStateInformation()`: Save/load APVTS state
- **Action**: Implement each method with minimal logic
- **Deliverable**: All metadata methods
- **Validation**: Methods return expected values; compile without errors

---

### **PHASE 3: Audio Editor (GUI)**

#### Step 3.1 – Implement PluginEditor Header
- **What**: Define editor class inheriting from `juce::AudioProcessorEditor`
- **Key Members**:
  - Reference to processor (for APVTS access)
  - GUI component holders (sliders, buttons, labels)
  - Resizer corner (optional)
- **Action**:
  - Define class: `class PluginEditor : public juce::AudioProcessorEditor`
  - Add member references and GUI components
- **Deliverable**: `PluginEditor.h` complete header
- **Validation**: Header compiles with AudioProcessorEditor base class

#### Step 3.2 – Implement PluginEditor Constructor
- **What**: Initialize editor and create GUI controls
- **Action**:
  - Set editor size (e.g., 400×300 pixels)
  - Create GUI components (sliders for mix, combo box for mode, etc.)
  - Attach APVTS parameters using `juce::AudioProcessorValueTreeState::SliderAttachment`
  - Add visual layout (labels, grouping if desired)
- **Deliverable**: Constructor with GUI setup
- **Validation**: Editor opens without crash; controls visible

#### Step 3.3 – Implement PluginEditor paint() & resized()
- **What**: Render GUI and layout components
- **Action**:
  - `paint()`: Draw background, labels, decorative elements
  - `resized()`: Position sliders, buttons, combo boxes based on window size
- **Deliverable**: Both methods implemented
- **Validation**: GUI displays correctly; resizing doesn't crash

#### Step 3.4 – Add APVTS Attachments for Controls
- **What**: Connect GUI sliders/boxes to APVTS parameters
- **Action**:
  - Create `SliderAttachment` for "mix" slider
  - Create `ComboBoxAttachment` for "mode" combo
  - Create `ButtonAttachment` for "enabled" toggle
- **Deliverable**: All control attachments in place
- **Validation**: Adjusting GUI controls updates APVTS; toggling bypass works

---

### **PHASE 4: Plugin Factory & Build**

#### Step 4.1 – Create Plugin Factory Functions
- **What**: Define entry point for DAW to load plugin
- **Action**:
  - Implement `juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()`
  - Return new instance of `PluginProcessor`
  - DAW will call this on load
- **Deliverable**: Factory function
- **Validation**: Compiles without unresolved symbols

#### Step 4.2 – Configure CMakeLists.txt for Plugin Target
- **What**: Add plugin compilation target to CMake
- **Action**:
  - Add `juce_add_plugin()` macro call
  - Configure plugin ID, company name, formats (VST3)
  - Link source files
  - Link JUCE modules
- **Deliverable**: Complete plugin build target
- **Validation**: CMake configuration succeeds

#### Step 4.3 – Build Project
- **What**: Compile plugin for first time
- **Action**:
  - Create build directory: `build/`
  - Run: `cd build && cmake .. -G "Visual Studio 17 2022"`
  - Run: `cmake --build . --config Release`
- **Deliverable**: Compiled `.vst3` binary
- **Validation**: Build completes with no errors; binary exists

#### Step 4.4 – Install Plugin to DAW Path
- **What**: Copy `.vst3` to system plugin directory
- **Action**:
  - Locate DAW plugin folder (e.g., `C:\Program Files\Common Files\VST3\`)
  - Copy compiled `.vst3` there
  - Rescan plugins in DAW
- **Deliverable**: Plugin appears in DAW plugin list
- **Validation**: Plugin discovered and loadable

---

### **PHASE 5: Validation & Testing**

#### Step 5.1 – Load Plugin in DAW
- **What**: Confirm plugin initializes without crash
- **Action**:
  - Open DAW (Reaper, FL Studio, Ableton, etc.)
  - Create new audio track
  - Add paranoidFilteroid plugin to track
  - Verify editor window opens
- **Deliverable**: Plugin loads and editor visible
- **Validation**: No crash; window renders; controls respond

#### Step 5.2 – Test Audio Pass-Through
- **What**: Confirm audio flows through plugin
- **Action**:
  - Route audio to track with plugin
  - Play reference tone or audio file
  - Toggle plugin bypass on/off
  - Verify audio is passed through
- **Deliverable**: Audio heard both with plugin active and bypass
- **Validation**: No artifacts, pops, clicks; bypass works

#### Step 5.3 – Test Parameter Changes
- **What**: Verify APVTS parameter updates work
- **Action**:
  - Adjust "mix" slider; confirm APVTS value updates
  - Toggle "mode" combo; confirm switch without crashes
  - Click "enabled" bypass; confirm audio mutes
- **Deliverable**: All parameters respond correctly
- **Validation**: No glitches, zipper noise, or pops

#### Step 5.4 – Test Multiple DAW Sessions
- **What**: Confirm plugin works across different DAW instances
- **Action** (optional for minimal phase):
  - Load plugin in 2+ DAW instances
  - Verify each operates independently
- **Deliverable**: Multi-instance stability confirmed
- **Validation**: No cross-talk; each instance independent

#### Step 5.5 – Performance Baseline
- **What**: Measure CPU usage
- **Action**:
  - Monitor CPU usage with plugin running
  - Confirm CPU < 1% (pass-through is minimal)
  - Check that no RT issues occur
- **Deliverable**: CPU baseline recorded
- **Validation**: CPU reasonable; no buffer underruns

---

## 4. Deliverables

### 4.1 Code Files
- `CMakeLists.txt` (root): Project build configuration
- `Source/core/PluginProcessor.h` / `.cpp`: Audio processor with APVTS
- `Source/core/PluginEditor.h` / `.cpp`: GUI editor with controls
- `Source/utils/DSPDefines.h`: Shared constants and enums
- `Source/dsp/PassThrough.h`: Minimal DSP placeholder (empty for now)

### 4.2 Build Artifacts
- `build/` directory with compiled plugin
- `.vst3` binary file (Windows x64)
- Plugin discoverable in DAW plugin list

### 4.3 Documentation
- This plan document (MINIMAL_SETUP_PLAN.md)
- Inline code comments explaining key sections
- Build guide entry (see BUILD_GUIDE_GENERATION.md)

### 4.4 Version Control
- Initial commit with all source files
- `.gitignore` for build artifacts
- Clean git history

---

## 5. Risks & Validation Checkpoints

### 5.1 Technical Risks

| Risk | Likelihood | Mitigation | Validation |
|------|------------|-----------|-----------|
| JUCE version compatibility | Low | Use latest stable JUCE (8.x) | Compiles without warnings |
| CMake configuration errors | Medium | Use simple, standard CMake patterns | Build succeeds on first try |
| VST3 plugin loading failure | Medium | Follow JUCE plugin macro conventions exactly | Plugin appears in DAW |
| Audio callback crashes | Low | Avoid heap allocations; use pre-allocated buffers | Play audio without crash |
| Parameter not updating DSP | Low | Verify APVTS attachment in GUI; read parameters in processBlock | Adjust slider; see value change |
| Denormal/DC offset issues | Low | Use JUCE denormal handling; apply high-pass on output | Check audio waveform for DC |

### 5.2 Validation Checkpoints

**Must Pass Before Proceeding:**
- [ ] **Checkpoint 1**: CMake configures without errors
- [ ] **Checkpoint 2**: Code compiles (Release + Debug builds)
- [ ] **Checkpoint 3**: Plugin loads in DAW without crash
- [ ] **Checkpoint 4**: Audio passes through (bypass on/off works)
- [ ] **Checkpoint 5**: Parameters update APVTS and reflect in GUI
- [ ] **Checkpoint 6**: No audio glitches or pops
- [ ] **Checkpoint 7**: CPU usage < 2% at idle
- [ ] **Checkpoint 8**: Multiple instances run independently

### 5.3 Open Questions / Decisions Needed from User

1. **JUCE Installation Path**: Where should JUCE be installed? (Globally or in repo subdirectory?)
2. **CMake Generator**: Which Visual Studio version? (VS 2022, VS 2019, etc.)
3. **Plugin Install Path**: Which DAW should be tested first? (Reaper, FL Studio, Ableton Live, etc.)
4. **Initial Sample Rate**: Should plugin default to 48 kHz or 44.1 kHz?
5. **GUI Size**: Preference for default editor window size? (400×300 suggested)

---

## 6. Architecture Overview (Signal Flow)

```
DAW Audio Input (Stereo)
    ↓
[PluginProcessor::processBlock]
    ↓
Check "enabled" parameter
    ├─ YES → Pass through (placeholder for DSP)
    └─ NO  → Output silence
    ↓
Check "mix" parameter (wet/dry)
    ├─ 0% → Output dry (input)
    ├─ 100% → Output wet (processed)
    └─ 50% → Blend
    ↓
DAW Audio Output (Stereo)
```

**APVTS Parameters:**
- `mode` (Telephone / Radio / Custom) → Controls DSP preset defaults
- `mix` (0–100%, linear) → Controls wet/dry blend
- `enabled` (true/false) → Bypass flag

**GUI Controls:**
- Mode ComboBox
- Mix Slider
- Enabled Toggle Button
- Labels and layout

---

## 7. Next Steps After Minimal Setup

Once minimal setup is complete and verified:

1. **Add DSP modules** (filters, EQ, compressor, saturation, noise)
2. **Implement mode presets** (Telephone 300–3400 Hz, Radio 200–5000 Hz, etc.)
3. **Enhance GUI** (grouping, styling, visual feedback)
4. **Add advanced parameters** (per DSP_SPEC.md)
5. **Multi-platform support** (macOS, Linux if desired)
6. **Generate final build guide** (combine all progress into LEGO-style manual)

---

## 8. Summary

This plan establishes a **minimal, buildable, tested JUCE VST3 plugin** that:
- ✅ Compiles cleanly
- ✅ Loads in a DAW
- ✅ Passes audio through unchanged (ready for DSP)
- ✅ Has working APVTS parameter system (mode, mix, enabled)
- ✅ Has a basic GUI (mode selector, mix slider, bypass toggle)
- ✅ Follows repo architecture (Source/dsp, Source/core, Source/utils)
- ✅ Is fully validatable and testable

Upon completion, the foundation is ready for adding all DSP processing modules.

