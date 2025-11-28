# PHASE 7 IMPLEMENTATION PLAN
**DSP Filter Chain - Telephone & Radio Voice Effects**

**Date**: November 28, 2025  
**Status**: Ready for Implementation  
**Following**: PLANNING_AND_CHECKLIST.md, VALIDATION_GUIDE.md, RESEARCH_GUIDE.md

---

## 1. GOAL

Implement three functional audio DSP filters (Telephone, Radio, Custom) that:
- Modify voice frequency response realistically
- Are selectable via Mode ComboBox parameter
- Switch in real-time without audio artifacts
- Maintain real-time safety (no allocations in audio thread)
- Pass validation against established DSP practice

**Success Criteria**:
- All three modes produce distinct, audible effects
- Mode switching is click-free and immediate
- Plugin remains responsive in DAW
- CPU usage stays <15% during sustained audio
- Tests pass in Reaper with speech & music

---

## 2. REQUIREMENTS & CONSTRAINTS

### 2.1 Technical Requirements
- **JUCE Version**: 8.1.0 (installed at C:\dev\JUCE)
- **Framework**: juce_dsp module with IIR filter factories
- **Plugin State**: Audio passthrough working, GUI functional
- **DAW Testing**: Reaper (loaded and tested)
- **Build System**: CMake 4.2.0, MSVC compiler

### 2.2 DSP Specifications

**Telephone Filter** (narrowband voice):
- Type: Bandpass IIR filter
- Center frequency: ~1850 Hz
- Bandwidth: ~3100 Hz (300 Hz - 3400 Hz)
- Q factor: ~0.6 (loose, natural sound)
- Design: Cascaded HP(300Hz) + LP(3400Hz)
- Goal: Simulate phone/intercom frequency response

**Radio Filter** (bright presence):
- Type: High-pass + Presence peak
- High-pass cutoff: ~80 Hz (remove rumble)
- Presence peak: ~2000 Hz center, +6dB gain
- Peak Q: ~1.5 (moderate width)
- Optional: Soft saturation (future)
- Goal: Simulate radio broadcast/voice-over quality

**Custom Mode** (blended):
- Mix all filters in parallel
- Use mix parameter (0.0 = dry, 1.0 = wet) to control blend
- Allow Mode to select which preset effect
- Future: User-adjustable parameters

### 2.3 Real-Time Safety Constraints
- ✅ No heap allocations in processBlock()
- ✅ No mutex locks or thread synchronization
- ✅ All filter state pre-allocated in prepareToPlay()
- ✅ Parameter changes safe via atomic reads
- ✅ Use ScopedNoDenormals for CPU efficiency

### 2.4 Architectural Constraints
- Plugin must remain buildable after every change
- All new DSP code goes in Source/dsp/
- No breaking changes to PluginProcessor or PluginEditor
- APVTS parameters already defined (mode, mix, enabled)
- Mode parameter: "Telephone", "Radio", "Custom"

### 2.5 Dependencies
- juce_dsp module must be linked in CMakeLists.txt
- TelephonyFilter and RadioFilter must be created before DSPChain
- DSPChain must be created before PluginProcessor integration
- Mode parameter must be read correctly in processBlock()

---

## 3. DETAILED STEP-BY-STEP PLAN

### Phase 3A: DSP Infrastructure Setup

**Step 1: Verify juce_dsp module in CMakeLists.txt**
- Open CMakeLists.txt
- Search for existing juce_dsp in target_link_libraries
- If missing, add: `juce_dsp`
- Verify all JUCE modules are properly linked

**Step 2: Create Source/dsp directory structure**
- Create folder: `Source/dsp/`
- This will contain: TelephonyFilter.h/cpp, RadioFilter.h/cpp, DSPChain.h

---

### Phase 3B: Implement TelephonyFilter

**Step 3: Create TelephonyFilter.h**
- Define class: `class TelephonyFilter`
- Members:
  - `juce::dsp::IIR::Filter<float> highPassFilter` (300 Hz)
  - `juce::dsp::IIR::Filter<float> lowPassFilter` (3400 Hz)
  - `juce::dsp::ProcessSpec spec`
  - Current sample rate tracking
- Methods:
  - `prepare(const juce::dsp::ProcessSpec&)` - Initialize both filters
  - `process(juce::AudioBuffer<float>&)` - Apply filtering
  - `reset()` - Clear internal state

**Step 4: Implement TelephonyFilter.cpp**
- `prepare()`: 
  - Store sample rate from spec
  - Initialize high-pass: `makeButterworthHighPass(sampleRate, 300.0f)`
  - Initialize low-pass: `makeButterworthLowPass(sampleRate, 3400.0f)`
  - Call prepare() on both internal filters
- `process()`:
  - Loop through channels
  - Apply high-pass filter
  - Apply low-pass filter to result
  - Write processed audio back to buffer
- `reset()`:
  - Call reset() on both internal filters

**Step 5: Validate TelephonyFilter**
- ✅ Compiles without errors
- ✅ Filter frequencies match spec (300Hz HP, 3400Hz LP)
- ✅ Uses JUCE IIR factories (makeButterworthHighPass, makeButterworthLowPass)
- ✅ No allocations in process()
- ✅ Real-time safe

---

### Phase 3C: Implement RadioFilter

**Step 6: Create RadioFilter.h**
- Define class: `class RadioFilter`
- Members:
  - `juce::dsp::IIR::Filter<float> highPassFilter` (80 Hz high-pass)
  - `juce::dsp::IIR::Filter<float> presencePeakFilter` (2000 Hz peak)
  - `juce::dsp::ProcessSpec spec`
  - Current sample rate tracking
- Methods: Same signature as TelephonyFilter

**Step 7: Implement RadioFilter.cpp**
- `prepare()`:
  - Initialize high-pass: `makeButterworthHighPass(sampleRate, 80.0f)`
  - Initialize presence peak: `makePeakFilter(sampleRate, 2000.0f, 1.5f, 1.25f)` (center, Q, gain in dB)
  - Call prepare() on both filters
- `process()`:
  - Apply high-pass first (remove rumble)
  - Apply presence peak (add brightness)
  - Write back to buffer
- `reset()`:
  - Call reset() on both internal filters

**Step 8: Validate RadioFilter**
- ✅ Compiles without errors
- ✅ High-pass at 80 Hz (standard rumble cutoff)
- ✅ Presence peak at 2000 Hz with moderate Q
- ✅ No allocations in process()
- ✅ Real-time safe

---

### Phase 3D: Create DSPChain Orchestrator

**Step 9: Create DSPChain.h**
- Define class: `class DSPChain`
- Members:
  - `TelephonyFilter telephonyFilter`
  - `RadioFilter radioFilter`
  - `int currentMode` (0=Telephone, 1=Radio, 2=Custom)
  - `float currentMix`
- Methods:
  - `prepare(const juce::dsp::ProcessSpec&)` - Initialize both filters
  - `processBlock(juce::AudioBuffer<float>&, int mode, float mix)` - Route and blend
  - `reset()` - Clear all filters
  - `setMode(int newMode)` - Update current mode

**Step 10: Implement DSPChain.cpp**
- `prepare()`:
  - Call prepare() on both telephonyFilter and radioFilter
  - Store spec for later use
- `processBlock()`:
  - Check mode parameter
  - If mode == 0 (Telephone): Apply telephonyFilter
  - If mode == 1 (Radio): Apply radioFilter
  - If mode == 2 (Custom): Apply telephonyFilter then radioFilter (chain)
  - Apply mix scaling at end
- `reset()`:
  - Call reset() on both filters

**Step 11: Validate DSPChain**
- ✅ Correctly routes audio to selected filter
- ✅ Mode switching works in real-time
- ✅ Mix parameter applied correctly
- ✅ No allocations in processBlock()

---

### Phase 3E: Integrate into PluginProcessor

**Step 12: Add DSPChain member to PluginProcessor.h**
- Include `#include "dsp/DSPChain.h"`
- Add member: `DSPChain dspChain`

**Step 13: Update PluginProcessor::prepareToPlay()**
- Call `dspChain.prepare(spec)` with sample rate and block size
- Verify no exceptions thrown

**Step 14: Update PluginProcessor::processBlock()**
- Read mode parameter: `int mode = apvts.getRawParameterValue("mode")->load()`
- Read mix parameter: `float mix = apvts.getRawParameterValue("mix")->load()`
- Call `dspChain.processBlock(buffer, mode, mix)`
- Maintain enabled bypass logic

**Step 15: Update PluginProcessor::releaseResources()**
- Call `dspChain.reset()` to clear filter state

**Step 16: Validate Integration**
- ✅ Compiles without linking errors
- ✅ No circular includes
- ✅ processBlock() calls DSPChain correctly
- ✅ Mode parameter is read and used
- ✅ Mix parameter is read and applied

---

### Phase 3F: Build System Updates

**Step 17: Update CMakeLists.txt**
- Verify `juce_dsp` is in target_link_libraries
- Add new source files to JUCE plugin target:
  - `Source/dsp/TelephonyFilter.cpp`
  - `Source/dsp/RadioFilter.cpp`
  - `Source/dsp/DSPChain.cpp`
- Verify include paths include Source/dsp/

**Step 18: Initial Compile**
- Run: `final-build.bat`
- Verify: No compilation errors
- Check: Plugin size reasonable (~25-30 MB for debug)
- If errors: Debug and fix before proceeding

---

### Phase 3G: Testing & Validation

**Step 19: Load in Reaper**
- Open Reaper
- Rescan VST3 plugins
- Add paranoidFilteroid to audio track
- Verify Mode ComboBox shows all three modes

**Step 20: Test Telephone Mode**
- Select Mode = "Telephone"
- Play speech audio (voice/podcast)
- Listen for narrowband, muffled effect
- Adjust mix slider: 0% = dry, 100% = full telephone effect
- Verify smooth response

**Step 21: Test Radio Mode**
- Select Mode = "Radio"
- Play same speech audio
- Listen for bright, presence-heavy effect
- Compare vs. Telephone mode (should sound different)
- Verify smooth response

**Step 22: Test Custom Mode**
- Select Mode = "Custom"
- Listen to blended effect
- Verify audio quality and lack of artifacts

**Step 23: Test Mode Switching**
- Switch between modes while audio plays
- Listen for clicks, pops, or glitches
- Verify smooth transitions
- Switch rapidly to stress-test

**Step 24: CPU Profiling**
- Monitor DAW CPU meter during sustained audio
- Verify usage <15% on typical system
- If higher: Optimize filter design or check for unnecessary allocations

---

## 4. DELIVERABLES

### Code Files (New)
- [ ] `Source/dsp/TelephonyFilter.h` (class definition + documentation)
- [ ] `Source/dsp/TelephonyFilter.cpp` (implementation)
- [ ] `Source/dsp/RadioFilter.h` (class definition + documentation)
- [ ] `Source/dsp/RadioFilter.cpp` (implementation)
- [ ] `Source/dsp/DSPChain.h` (class definition + documentation)
- [ ] `Source/dsp/DSPChain.cpp` (implementation)

### Code Files (Modified)
- [ ] `Source/core/PluginProcessor.h` (add DSPChain member)
- [ ] `Source/core/PluginProcessor.cpp` (integrate DSPChain, update processBlock)
- [ ] `CMakeLists.txt` (add juce_dsp, add source files)

### Testing & Documentation
- [ ] Test results recorded (pass/fail for each mode)
- [ ] CPU profiling results documented
- [ ] Audio quality assessment (no artifacts)
- [ ] Mode switching validation (click-free)

### Build Artifacts
- [ ] Plugin compiles successfully
- [ ] Plugin loads in Reaper without errors
- [ ] All GUI controls functional
- [ ] All three modes accessible and working

---

## 5. RISKS & VALIDATION REQUIREMENTS

### 5.1 Technical Risks

**Risk 1: Filter Instability at Extreme Parameters**
- Mitigation: Use proven JUCE IIR factories, validate coefficients
- Validation: Test with extreme sample rates (8kHz, 192kHz)
- Safety: Butterworth filters are known-stable

**Risk 2: Real-Time Safety Violations**
- Mitigation: Pre-allocate all filter state in prepareToPlay()
- Validation: Use static analysis and inspection
- Safety: No new/malloc in processBlock()

**Risk 3: Mode Switching Artifacts (Clicks)**
- Mitigation: Use continuous filter state (no reinit on mode change)
- Validation: Listen for artifacts during rapid switching
- Safety: Keep filter internal state between mode changes

**Risk 4: Parameter Synchronization Issues**
- Mitigation: Read mode/mix via atomic operations (already in place)
- Validation: Change parameters during audio playback
- Safety: APVTS handles atomicity

**Risk 5: DSP Math Errors in Filter Design**
- Mitigation: Use JUCE's verified IIR coefficient factories
- Validation: Reference JUCE documentation and DSP guides
- Safety: No custom coefficient math

### 5.2 Validation Checklist (Before Shipping Phase 7)

**Syntax & Compilation**
- [ ] All files compile without errors
- [ ] No unused variable warnings (or marked as intentional)
- [ ] No linking errors
- [ ] Plugin binary generated successfully

**DSP Validation**
- [ ] Filter frequencies match spec (300-3400Hz for Telephone, 80Hz HP for Radio)
- [ ] Filter types are correct (Butterworth/Peaking)
- [ ] No NaN or infinity outputs
- [ ] Filter coefficients are rational (no extreme values)

**Real-Time Safety**
- [ ] No allocations in processBlock()
- [ ] No mutex locks in audio thread
- [ ] All filter state pre-allocated
- [ ] ScopedNoDenormals used for efficiency

**Parameter Routing**
- [ ] Mode parameter correctly read
- [ ] Mix parameter correctly applied
- [ ] Enabled bypass still functional
- [ ] Parameter changes reflected in audio

**Audio Quality**
- [ ] No DC offset introduced
- [ ] No unexpected gain changes
- [ ] No pops or clicks during playback
- [ ] Mode switching clean and artifact-free
- [ ] All three modes audibly different

**DAW Integration**
- [ ] Plugin loads without crashes
- [ ] GUI responsive during audio playback
- [ ] Presets save/recall correctly
- [ ] Works with project file (redo/undo)

### 5.3 Things Requiring User Approval

- [ ] Filter frequency choices (300-3400Hz, 80Hz) - approved by user before implementation
- [ ] Filter Q values and gain - approved by user listening tests
- [ ] Mode naming (Telephone/Radio/Custom) - matches user intent
- [ ] CPU usage threshold (15%) - acceptable for target systems

---

## 6. ESTIMATED TIMELINE

- Infrastructure setup: ~15 minutes
- TelephonyFilter implementation: ~30 minutes
- RadioFilter implementation: ~30 minutes
- DSPChain implementation: ~30 minutes
- Integration into PluginProcessor: ~20 minutes
- Build system updates: ~10 minutes
- Compilation & fixing: ~15 minutes (if needed)
- DAW testing: ~30 minutes
- **Total: ~3 hours**

---

## 7. SUCCESS CONFIRMATION

Phase 7 is **COMPLETE** when:

✅ Plugin compiles to <30MB debug binary  
✅ Loads in Reaper without errors  
✅ Mode = Telephone produces narrowband voice effect  
✅ Mode = Radio produces bright, present effect  
✅ Mode = Custom produces blended effect  
✅ Mode switching is click-free and immediate  
✅ Mix parameter controls wet/dry blend (0-100%)  
✅ CPU stays <15% during sustained audio  
✅ All audio quality checks pass  
✅ build_journal.md updated with results  

---

**Ready to proceed with implementation!**  
Following: PLANNING_AND_CHECKLIST.md, VALIDATION_GUIDE.md, RESEARCH_GUIDE.md

