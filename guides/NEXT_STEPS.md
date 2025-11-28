# 🎯 NEXT STEPS - DSP Implementation Phase

**Current Status**: ✅ Audio passthrough working, GUI functional  
**Date**: November 28, 2025  
**Next Phase**: Implement DSP filters (Telephone, Radio, Custom modes)

---

## 📍 Roadmap Overview

```
Phase 1-6: ✅ COMPLETE (Foundation & Audio Passthrough)
    ├─ Tool installation & setup
    ├─ Code implementation
    ├─ Build system configuration
    ├─ Compilation fixes (API, VST2/VST3)
    ├─ Audio bus configuration fix
    └─ DAW testing & verification

Phase 7: 🔄 IN PROGRESS (DSP Implementation)
    ├─ Implement Telephone filter (300-3400 Hz bandpass)
    ├─ Implement Radio filter (voice optimization)
    ├─ Implement Custom mode (user-adjustable)
    └─ Test audio quality in Reaper

Phase 8: ⏳ PENDING (Release & Optimization)
    ├─ Release build compilation
    ├─ Performance profiling
    ├─ GUI polish & UX improvements
    └─ Packaging & distribution
```

---

## 📋 Phase 7: DSP Implementation Checklist

### 7.1 Telephony Filter (300-3400 Hz Bandpass)

**What it does**: Simulates telephone voice quality by narrowing frequency response

**Implementation**:
```cpp
// In PluginProcessor.h - Add filter member
juce::dsp::IIR::Filter<float> telephoneFilter;

// In prepareToPlay() - Initialize
spec.maximumBlockSize = samplesPerBlock;
spec.sampleRate = sampleRate;
spec.numChannels = getMainBusNumOutputChannels();

// Design bandpass filter: 300 Hz low, 3400 Hz high
telephoneFilter.prepare(spec);
telephoneFilter.coefficients = 
    juce::dsp::IIR::Coefficients<float>::makeBandPass(
        sampleRate, 1850.0f, 0.707f);  // Center: 1850Hz, Q: 0.707
```

**Tasks**:
- [ ] Add `#include <juce_dsp/juce_dsp.h>` to CMakeLists.txt modules
- [ ] Create TelephonyFilter class in `Source/dsp/TelephonyFilter.h`
- [ ] Implement prepare(), process(), reset() methods
- [ ] Add to PluginProcessor
- [ ] Test in Reaper with speech audio

### 7.2 Radio Filter (Voice Optimization)

**What it does**: Compresses and brightens mid-range for radio broadcast sound

**Implementation Outline**:
```cpp
// Dual-band approach:
// 1. High-pass filter: ~80 Hz (remove rumble)
// 2. Mid-range boost: ~2000 Hz (presence peak)
// 3. Optional compression (look-ahead compressor)
```

**Tasks**:
- [ ] Create RadioFilter class in `Source/dsp/RadioFilter.h`
- [ ] Implement high-pass component
- [ ] Implement presence peak (EQ shelf or resonant peak)
- [ ] Optional: Add simple compressor
- [ ] Test quality vs. Telephone filter

### 7.3 Custom Mode (User-Adjustable)

**What it does**: Allows user to blend between Telephone and Radio characteristics

**Implementation**:
```cpp
// Current mix parameter can control blend:
// mix = 0.0  → Dry (no effect)
// mix = 0.5  → 50% Telephone + 50% Radio
// mix = 1.0  → 100% effect (Radio or selected mode)
```

**Tasks**:
- [ ] Add separate wet/dry processing
- [ ] Route through Telephone + Radio in parallel
- [ ] Use mix slider to blend between them
- [ ] Add mode selection via Mode ComboBox (currently unused)

---

## 🔧 Technical Details

### Audio DSP Module Structure

**Recommended File Organization**:
```
Source/
├─ core/
│  ├─ PluginProcessor.h
│  ├─ PluginProcessor.cpp
│  ├─ PluginEditor.h
│  └─ PluginEditor.cpp
└─ dsp/
   ├─ TelephonyFilter.h
   ├─ TelephonyFilter.cpp
   ├─ RadioFilter.h
   ├─ RadioFilter.cpp
   └─ DSPChain.h  (orchestrates all filters)
```

### Real-Time Safety Checklist

When implementing DSP, ensure:
- [ ] No malloc/new in processBlock()
- [ ] No I/O or blocking calls
- [ ] All audio buffers pre-allocated in prepareToPlay()
- [ ] Use juce::ScopedNoDenormals for CPU efficiency
- [ ] Test with rapid parameter changes
- [ ] Monitor CPU load in DAW

### APVTS Parameters Currently Available

```cpp
"mode"      → Choice: Telephone, Radio, Custom (currently unused)
"mix"       → Float: 0.0-1.0 (controls wet/dry blend)
"enabled"   → Bool: true/false (bypass control)
```

**Task**: Implement Mode parameter to actually switch filters!

---

## 📝 Implementation Order (Recommended)

### Step 1: Create DSP Infrastructure
1. Add `#include <juce_dsp/juce_dsp.h>` to CMakeLists.txt
2. Create `Source/dsp/DSPChain.h` to manage filters
3. Add prepare(), process(), reset() scaffolding

### Step 2: Implement Telephony Filter
1. Design bandpass using online calculator: https://www.analog.com/designtools/designcenter/
2. Create TelephonyFilter class
3. Test with white noise → should sound muffled
4. Verify CPU usage stays <5%

### Step 3: Implement Radio Filter
1. Design high-pass (80 Hz) + presence peak (2kHz)
2. Create RadioFilter class
3. Compare sound vs. Telephony filter
4. Iterate on center frequencies if needed

### Step 4: Integrate with Mode Parameter
1. Modify processBlock() to check Mode parameter
2. Route audio through appropriate filter
3. Test Mode switching in real-time
4. Verify no audio clicks/artifacts during switching

### Step 5: Test & Iterate
1. Load in Reaper with speech/music audio
2. A/B compare with/without plugin
3. Adjust filter parameters based on listening
4. Record test results in TESTING_RESULTS.md

---

## 🎓 Reference Materials

### JUCE DSP Documentation
- IIR Filters: https://docs.juce.com/master/classjuce_1_1dsp_1_1IIR_1_1Filter.html
- Filter Coefficients: https://docs.juce.com/master/classjuce_1_1dsp_1_1IIR_1_1Coefficients.html
- ProcessorChain: https://docs.juce.com/master/classjuce_1_1dsp_1_1ProcessorChain.html

### Filter Design Tools
- http://www.analog.com/designtools/designcenter/
- https://www.earlevel.com/main/2016/12/02/cascaded-shelving-filters/

### Audio DSP Learning
- "The Art of VA Filter Design" (Välimäki, Huovilainen)
- DSP basics: https://www.dsprelated.com/

---

## 🚀 Immediate Next Action

1. **Update CMakeLists.txt** to include juce_dsp module (if not already)
2. **Create Source/dsp/DSPChain.h** skeleton
3. **Implement TelephonyFilter** with working bandpass
4. **Test in Reaper** with speech audio
5. **Update build_journal.md** when each filter works

---

## 📊 Success Criteria

✅ **Phase 7 Complete When**:
- Telephone filter applies audible effect (muffled sound)
- Radio filter applies distinct sound (bright, present)
- Mode parameter switches between filters in real-time
- Mix slider blends wet/dry smoothly (0% = dry, 100% = wet)
- No audio glitches, clicks, or artifacts
- CPU usage <15% on typical DAW system
- All three modes sound natural and useful

---

## 🔗 Related Documentation

- **build_journal.md** - Build history & timeline
- **PREREQUISITES.md** - Tool installation reference
- **reports/RESEARCH_FINDINGS.md** - DSP theory & JUCE reference
- **reports/IMPLEMENTATION_CHECKLIST.md** - Detailed troubleshooting

---

**Phase Status**: 🔄 Ready to begin DSP implementation  
**Estimated Duration**: 2-4 hours for basic filters  
**Difficulty**: Intermediate (audio DSP + JUCE API)

Let me know when you're ready to start Phase 7! 🎵
