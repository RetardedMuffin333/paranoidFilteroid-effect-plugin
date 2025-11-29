# PHASE 9 IMPLEMENTATION PLAN
**Enhanced Parameter Architecture - Saturation, Noise & Smoothing**

**Date**: November 29, 2025  
**Status**: Ready for Implementation  
**Following**: FUNCTIONALITY_PARAMETERS.md, PROFESSIONAL_RECOMMENDATIONS.md

---

## 1. PHASE 9 GOAL

Enhance paranoidFilteroid from "functional filter plugin" to "professional audio effect" by adding:

1. **Parameter Smoothing** - Eliminate zipper noise on continuous parameters
2. **Saturation Stage** - Add harmonic character via soft-clipping WaveShaper
3. **Noise Generator** - Procedural hiss for lo-fi aesthetic
4. **Denormal Handling** - Audio thread optimization

**Success Criteria**:
- ✅ Parameter changes are smooth (no audible clicks/zippers)
- ✅ Saturation doesn't cause unexpected clipping
- ✅ Noise texture blends naturally with filtered audio
- ✅ CPU usage remains <15% on sustained audio
- ✅ Plugin loads and processes in Reaper without crashes
- ✅ All three modes (Telephone/Radio/Custom) are sonically distinct

---

## 2. PHASE 9 REQUIREMENTS & SPECIFICATIONS

### 2.1 Parameter Smoothing

**What**: Use `juce::dsp::LinearSmoothedValue<float>` to ramp parameter changes smoothly

**Why**: Prevents audible zipper noise when users automate or move faders in real-time

**Implementation**:
```cpp
// Header file member variable
juce::dsp::LinearSmoothedValue<float> smoothedMix;
juce::dsp::LinearSmoothedValue<float> smoothedOutputGain;

// In prepare():
smoothedMix.reset(spec.sampleRate, 0.05f);  // 50ms ramp time
smoothedOutputGain.reset(spec.sampleRate, 0.05f);

// In processBlock():
float currentMix = smoothedMix.getNextValue();
float currentGain = smoothedOutputGain.getNextValue();
```

**Parameters to Smooth**:
- `mix` (wet/dry blend) - **HIGH PRIORITY**
- `outputGainDb` - **HIGH PRIORITY**
- `saturationAmount` - **MEDIUM PRIORITY** (if added)
- `noiseLevelDb` - **MEDIUM PRIORITY** (if added)

### 2.2 Saturation Stage (WaveShaper)

**What**: Soft-clipping waveshaper that adds harmonic warmth

**Where in Chain**: After filters, before noise

**JUCE Component**: `juce::dsp::WaveShaper<float>`

**Design**:
```cpp
// Member variable
juce::dsp::WaveShaper<float> waveshaper;

// In prepare():
waveshaper.prepare(spec);

// In processBlock():
if (saturationAmount > 0.0f) {
    // Apply gain to increase saturation effect
    float drive = 1.0f + (saturationAmount * 9.0f);  // 1.0x to 10.0x drive
    buffer.applyGain(drive);
    
    // Apply soft-clipping via waveshaper
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    waveshaper.process(context);
    
    // Makeup gain compensation
    float makeupGain = 1.0f / juce::jlimit(1.0f, 10.0f, drive);
    buffer.applyGain(makeupGain);
}
```

**Soft-Clipping Function**:
- **Default**: Tanh soft-clip (smooth, natural)
- Formula: `y = tanh(x * drive) / tanh(drive)` normalizes output

**Parameters**:
- `saturationAmount`: 0.0 → 1.0 (0=off, 1=max saturation)
- Pre-calculated default for Telephone (0.20) and Radio (0.30)

### 2.3 Noise Generator

**What**: Procedural white noise generator blended into audio

**Where in Chain**: After saturation, before wet/dry mix

**Design**:
```cpp
// Member variable
juce::Random noiseGenerator;
std::unique_ptr<float[]> noiseBuffer;

// In prepare():
noiseBuffer.reset(new float[spec.maximumBlockSize]);

// In processBlock():
if (noiseLevelDb > -60.0f) {
    // Generate white noise
    for (int i = 0; i < numSamples; ++i) {
        noiseBuffer[i] = (noiseGenerator.nextFloat() - 0.5f) * 2.0f;  // -1.0 to 1.0
    }
    
    // Apply level (convert dB to linear)
    float noiseGain = juce::Decibels::decibelsToGain(noiseLevelDb);
    
    // Blend noise into each channel
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* channelData = buffer.getWritePointer(ch);
        for (int i = 0; i < numSamples; ++i) {
            channelData[i] += noiseBuffer[i] * noiseGain;
        }
    }
}
```

**Parameters**:
- `noiseLevelDb`: -60 → -20 dB (-60=off, -20=very loud)
- Pre-calculated defaults: Telephone (-45 dB), Radio (-50 dB)

**Future Enhancement**:
- Optional high-pass filter on noise (reduce rumble)
- `noiseMode` choice: "Off", "White", "Pink", "Vinyl" (v2)

### 2.4 Denormal Flushing

**What**: CPU optimization to prevent denormal numbers

**Why**: Denormals (very small numbers) can cause CPU spikes

**Implementation**:
```cpp
// Include this at top of processBlock()
juce::ScopedNoDenormals noDenormals;

// Alternative: Set flush-to-zero mode on Windows/x86
// _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
```

---

## 3. DETAILED STEP-BY-STEP IMPLEMENTATION

### Step 1: Update DSPChain.h with Parameter Smoothing

**File**: `Source/dsp/DSPChain.h`

**Changes**:
1. Add member variables for `LinearSmoothedValue`
2. Update `prepare()` to initialize smoothers
3. Update `processBlock()` to use smoothed values

**Code Addition**:
```cpp
private:
    // Parameter smoothers
    juce::dsp::LinearSmoothedValue<float> smoothedMix;
    juce::dsp::LinearSmoothedValue<float> smoothedOutputGain;
    
    // In prepare():
    smoothedMix.reset(spec.sampleRate, 0.05f);
    smoothedOutputGain.reset(spec.sampleRate, 0.05f);
```

### Step 2: Create NoiseGenerator.h

**File**: `Source/dsp/NoiseGenerator.h`

**Header-only class**:
```cpp
#pragma once
#include <juce_dsp/juce_dsp.h>

class NoiseGenerator {
public:
    void prepare(const juce::dsp::ProcessSpec& spec) {
        maximumBlockSize = spec.maximumBlockSize;
        numChannels = spec.numChannels;
        noiseBuffer.reset(new float[maximumBlockSize]);
    }
    
    void generateWhiteNoise(juce::AudioBuffer<float>& buffer, 
                           float levelDb) {
        if (levelDb <= -60.0f) return;  // Off
        
        int numSamples = buffer.getNumSamples();
        float gain = juce::Decibels::decibelsToGain(levelDb);
        
        // Generate white noise
        for (int i = 0; i < numSamples; ++i) {
            noiseBuffer[i] = (rng.nextFloat() - 0.5f) * 2.0f;
        }
        
        // Mix into all channels
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
            auto* data = buffer.getWritePointer(ch);
            for (int i = 0; i < numSamples; ++i) {
                data[i] += noiseBuffer[i] * gain;
            }
        }
    }
    
    void reset() {
        rng.setSeedRandomly();
    }
    
private:
    juce::Random rng;
    std::unique_ptr<float[]> noiseBuffer;
    int maximumBlockSize = 0;
    int numChannels = 0;
};
```

### Step 3: Update DSPChain.h with Saturation & Noise

**File**: `Source/dsp/DSPChain.h`

**Changes**:
1. Add `juce::dsp::WaveShaper<float>` member
2. Add `NoiseGenerator` member
3. Update `prepare()` to initialize waveshaper
4. Update `processBlock()` to apply saturation and noise

**Code Addition**:
```cpp
private:
    juce::dsp::WaveShaper<float> waveshaper;
    NoiseGenerator noiseGenerator;
    
    // In prepare():
    waveshaper.prepare(spec);
    noiseGenerator.prepare(spec);
    
    // In processBlock() - after filters, before mix:
    if (saturationAmount > 0.0f) {
        applySaturation(buffer, saturationAmount);
    }
    noiseGenerator.generateWhiteNoise(buffer, noiseLevelDb);
```

### Step 4: Update PluginProcessor.cpp Parameter Reading

**File**: `Source/core/PluginProcessor.cpp`

**Changes**:
1. Read `saturationAmount` parameter from APVTS
2. Read `noiseLevelDb` parameter from APVTS
3. Pass to DSPChain in processBlock()

**Code Pattern**:
```cpp
// In processBlock():
float saturation = *apvts.getRawParameterValue("saturation");
float noiseLevel = *apvts.getRawParameterValue("noiseLevel");

dspChain.processBlock(buffer, mode, mix, saturation, noiseLevel);
```

### Step 5: Update PluginEditor.cpp GUI

**File**: `Source/core/PluginEditor.cpp`

**Changes**:
1. Add Slider for Saturation (-/+)
2. Add Slider for Noise Level (dB)
3. Add Label text
4. Arrange in UI layout

**Target Layout**:
```
[ GLOBAL ]
Mode | Mix | Output Gain | Enable

[ CHARACTER ]
Saturation Slider | Noise Level Slider

[ FILTERS ]
Low Cut | High Cut
```

### Step 6: Update CMakeLists.txt

**File**: `CMakeLists.txt`

**Changes**:
1. Add `NoiseGenerator.h` to target_sources (if using explicit listing)
2. Ensure `juce_dsp` is linked (should already be)

```cmake
target_sources(paranoidFilteroid PRIVATE
    # ... existing sources ...
    Source/dsp/NoiseGenerator.h
)
```

---

## 4. TESTING & VALIDATION CHECKLIST

### Audio Quality Testing
- [ ] Load plugin in Reaper
- [ ] Test Telephone mode: Verify narrowband voice effect
- [ ] Test Radio mode: Verify bright presence effect
- [ ] Test Custom mode: Verify blend of both effects
- [ ] Test mode switching: No clicks/pops between modes
- [ ] Test mix parameter: Smooth fade from dry to wet (no zipper noise)
- [ ] Test saturation on each mode: Confirm character enhancement
- [ ] Test noise on each mode: Confirm hiss doesn't overpower
- [ ] Test with speech & music: Both should process naturally

### Performance Testing
- [ ] CPU usage during continuous playback (<15% target)
- [ ] No real-time thread glitches or underruns
- [ ] Parameter automation smooth (no stuttering)
- [ ] Noise generation doesn't spike CPU

### Edge Case Testing
- [ ] Max settings (all sliders at maximum)
- [ ] Min settings (all sliders at minimum)
- [ ] Rapid parameter changes (automation stress test)
- [ ] Very high sample rate (96kHz) processing
- [ ] Very low block sizes (64 samples)

### Quality Assurance
- [ ] No compiler warnings
- [ ] No memory leaks (check JUCE metrics)
- [ ] Plugin builds from clean state
- [ ] Git history is clean

---

## 5. ESTIMATED TIMELINE

| Task | Est. Time | Notes |
|------|-----------|-------|
| Parameter Smoothing | 30 min | LinearSmoothedValue integration |
| Saturation Stage | 60 min | WaveShaper + makeup gain |
| Noise Generator | 60 min | Random white noise + blending |
| GUI Updates | 45 min | Slider layout & labels |
| Testing & Validation | 90 min | Comprehensive DAW testing |
| **Total Phase 9** | **~4.5 hours** | Achievable in one session |

---

## 6. POTENTIAL ISSUES & MITIGATION

### Issue 1: Zipper Noise Still Audible
**Cause**: Smoother ramp time too long  
**Fix**: Reduce ramp time from 50ms to 20ms

### Issue 2: Saturation Causes Clipping
**Cause**: Drive amount too aggressive  
**Fix**: Reduce drive scaling or add better makeup gain compensation

### Issue 3: Noise Level Too Loud
**Cause**: dB to linear conversion incorrect  
**Fix**: Verify `juce::Decibels::decibelsToGain()` calculation

### Issue 4: CPU Spike on Noise Generation
**Cause**: Allocating noise buffer in processBlock()  
**Fix**: Pre-allocate in prepare(), reuse in processBlock()

### Issue 5: UI Sliders Not Updating Audio
**Cause**: Parameter not properly connected to APVTS  
**Fix**: Verify parameter ID matches between Editor and APVTS definition

---

## 7. PHASE 9 SUCCESS CRITERIA

✅ **Technical**:
- Plugin compiles without errors or warnings
- Parameter smoothing eliminates zipper noise
- Saturation adds character without clipping
- Noise texture blends naturally

✅ **Perceptual**:
- All three modes sound distinct and professional
- Mode switching is smooth and click-free
- Saturation adds "warmth" to the effect
- Noise doesn't overpower the processed audio

✅ **Performance**:
- CPU usage <15% on sustained audio
- No real-time glitches or underruns
- Plugin responsive to rapid parameter changes

✅ **Code Quality**:
- Clean git history with meaningful commits
- No memory leaks or resource warnings
- Well-commented code for future maintenance

---

## 8. NEXT PHASE PREVIEW (Phase 10)

After Phase 9 completes, Phase 10 will add:
1. **Presence Peak EQ** - Mid-range accentuation (1500-3000 Hz boost)
2. **Compression Module** - Broadcast-style dynamics control
3. **Stereo Processing Modes** - "Mono", "Dual-Mono", "Stereo Spread"
4. **Lo-Fi / Bit-Crusher** - Digital codec simulation

---

## 9. REFERENCES & DOCUMENTATION

- **JUCE DSP Module**: https://docs.juce.com/master/group__juce__dsp.html
- **LinearSmoothedValue**: https://docs.juce.com/master/classjuce_1_1dsp_1_1LinearSmoothedValue.html
- **WaveShaper**: https://docs.juce.com/master/classjuce_1_1dsp_1_1WaveShaper.html
- **Decibels Conversion**: https://docs.juce.com/master/classjuce_1_1Decibels.html
- **Current Status**: See `guides/build_journal.md` for Phase 8 completion

---

**Build Status**: ✅ **PHASE 8 COMPLETE - READY FOR PHASE 9**  
**Last Updated**: November 29, 2025  
**Next Checkpoint**: Implement saturation, noise, and parameter smoothing
