# 📘 paranoidFilteroid Plugin Specification

**Project**: paranoidFilteroid VST3 Audio Effect  
**Purpose**: Band-limited voice coloration effect (telephone/radio/lo-fi)  
**Version**: 1.0 roadmap  
**Last Updated**: November 29, 2025

---

## 1. Plugin Purpose & Context

paranoidFilteroid simulates voice coloration effects from:
- **Telephone**: Narrowband voice (300–3400 Hz bandpass)
- **Radio**: Broadcast voice (bright presence peak, rumble reduction)
- **Lo-Fi**: Vinyl/analog artifacts and radio hiss

**Design Philosophy**: Modular DSP chain with macro presets (Mode) + fine control via individual parameters.

---

## 2. Core Parameters (v1 Implementation)

### 🔹 Global Controls

| Parameter | Type | Range | Default | Description |
|-----------|------|-------|---------|-------------|
| **Enabled** | Boolean | — | `true` | Master bypass switch |
| **Mode** | Choice | Telephone / Radio / Custom | Telephone | Macro preset selector |
| **Mix** | Float | 0.0 → 1.0 | 1.0 | Wet/dry blend (0=dry, 1=wet) |
| **Output Gain** | Float | -24 → +24 dB | 0 dB | Final output trim stage |
| **Force Mono** | Boolean | — | `true` | Sum stereo to mono before processing |

### 🔹 Filter Parameters

| Parameter | Type | Range | Telephone | Radio | Description |
|-----------|------|-------|-----------|-------|-------------|
| **Low Cut** | Float | 100–1000 Hz | 350 Hz | 200 Hz | High-pass filter cutoff |
| **High Cut** | Float | 2000–8000 Hz | 3500 Hz | 5000 Hz | Low-pass filter cutoff |

### 🔹 Character Parameters

| Parameter | Type | Range | Telephone | Radio | Description |
|-----------|------|-------|-----------|-------|-------------|
| **Saturation** | Float | 0.0 → 1.0 | 0.20 | 0.30 | Soft-clipping drive amount |
| **Noise Level** | Float | -60 → -20 dB | -45 dB | -50 dB | Radio hiss/vinyl crackle volume |

---

## 3. DSP Signal Flow (Current v1 Implementation)

```
Input Audio
  ↓
[Mono Sum] (if forceMono enabled)
  ↓
[High-Pass Filter] (lowCutHz)
  ↓
[Low-Pass Filter] (highCutHz)
  ↓
[Saturation/WaveShaper] (saturationAmount)
  ↓
[Noise Generator] (noiseLevelDb)
  ↓
[Wet/Dry Mix] (mix parameter)
  ↓
[Output Gain] (outputGainDb)
  ↓
Output Audio
```

**Order Rationale**:
1. Mono sum first (if needed) to avoid stereo phasing issues
2. Filters before saturation (prevents unwanted harmonic distortion)
3. Saturation adds harmonic warmth and prevents clipping
4. Noise injection last (before mix) for authentic hiss texture
5. Mix allows easy A/B comparison and blending

---

## 4. Mode Macro Defaults (v1)

When user changes **Mode**, these parameters should automatically reset (unless user has customized them):

| Parameter | Telephone | Radio | Custom |
|-----------|-----------|-------|--------|
| **Low Cut** | 350 Hz | 200 Hz | No change |
| **High Cut** | 3500 Hz | 5000 Hz | No change |
| **Saturation** | 0.20 | 0.30 | No change |
| **Noise Level** | -45 dB | -50 dB | No change |
| **Force Mono** | `true` | `true` | No change |

This "soft preset" behavior improves user workflow and reduces parameter tweaking.

---

## 5. Implementation Roadmap

### ✅ Phase 1-8: Foundation (COMPLETE)
- [x] Audio passthrough confirmed in Reaper
- [x] GUI parameters (Mode, Mix, Enabled) functional
- [x] DSPChain infrastructure with Mode routing
- [x] Real IIR filters: TelephonyFilter & RadioFilter
- [x] Plugin compiles and loads successfully
- [x] Audio processes with real filter coefficients

### 🔄 Phase 9: Enhanced Parameter Architecture (NEXT)
- [ ] Add **Saturation Stage** (WaveShaper with soft-clipping)
- [ ] Add **Noise Generator** (procedural hiss)
- [ ] Implement **Parameter Smoothing** (LinearSmoothedValue for mix, gain)
- [ ] Add **Denormal Flushing** (audio thread optimization)
- [ ] Update UI to expose Saturation & Noise parameters

### ⏳ Phase 10: Advanced DSP (v1.5)
- [ ] **Presence Peak EQ**: 1500–3000 Hz, +3 to +9 dB
- [ ] **Compression**: Broadcast-style with threshold/ratio/makeup
- [ ] **Stereo vs. Mono Mode**: "Keep Original", "Mono", "Stereo Spread"
- [ ] **Lo-Fi / Bit-Crusher**: 4–16 bit sample rate reduction

### ⏳ Phase 11: Vinyl & Aesthetics (v2)
- [ ] Embedded vinyl crackle sample playback
- [ ] Random transient pops (lo-fi aesthetic)
- [ ] Filtered crackle (reduce harshness)
- [ ] Professional GUI with meter displays

---

## 6. Current Implementation Status

### ✅ Completed
- Real IIR filter implementation (Telephone & Radio modes)
- Mode-based parameter routing
- Wet/dry mixing (mix parameter)
- AudioBlock processing with proper JUCE API
- Plugin successfully loads in Reaper

### 🔄 In Development
- Parameter smoothing for mix/gain controls
- Saturation stage implementation
- Noise generator development

### ⏳ Planned
- Advanced presence EQ
- Compression module
- Vinyl crackle samples
- Professional UI refinements

---

## 7. Parameter Smoothing Strategy (Phase 9)

**Goal**: Eliminate zipper noise when parameters change in real-time.

**Implementation**:
- Use `juce::dsp::LinearSmoothedValue<float>` for:
  - `mix` parameter (50ms ramp)
  - `outputGainDb` parameter (50ms ramp)
  - Any **future** continuous parameters (saturation, noise level)

**Benefit**: Professional-sounding parameter changes, no audio artifacts.

---

## 8. Saturation Stage (Phase 9)

**Goal**: Add harmonic character and prevent clipping without digital artifacts.

**Design**:
- Use `juce::dsp::WaveShaper` with soft-clipping curve
- Parameter: `saturationAmount` (0.0 → 1.0)
- Process order: After filters, before noise
- Default: 0.0 (disabled) for backward compatibility

**Soft-Clipping Options**:
1. **Tanh soft-clip** (standard, smooth)
2. **Arctangent soft-clip** (slightly brighter)
3. **Polynomial soft-clip** (DSP efficient)

---

## 9. Noise Generator (Phase 9)

**Goal**: Add authentic vinyl hiss or radio static for lo-fi aesthetic.

**Design**:
- Procedural white noise generator
- Parameter: `noiseLevelDb` (-60 → -20 dB)
- Optional: Simple high-pass filter to reduce low-frequency rumble
- Process order: After saturation, before wet/dry mix

**Future Enhancement** (v2):
- `noiseMode` choice: "Off", "Hiss", "Vinyl"
- "Vinyl" mode uses embedded sample playback with random offsets

---

## 10. Professional Enhancement Summary

### High Priority (Phase 9)
✨ **Parameter Smoothing**: Prevents zipper noise (30 min, high impact)

✨ **Saturation Stage**: Adds character and warmth (60 min, medium impact)

✨ **Noise Generator**: Lo-fi aesthetic (60 min, medium impact)

### Medium Priority (Phase 10)
🎚️ **Presence Peak EQ**: Enhanced intelligibility (90 min)

🎚️ **Compression Module**: Broadcast authenticity (120 min)

🎚️ **Stereo Modes**: Modern flexibility (45 min)

### Lower Priority (Phase 11)
🎵 **Vinyl Crackle**: Authentic lo-fi feel (requires audio sample)

🎨 **Advanced UI**: Visual feedback and metering

---

## 11. Testing & Validation Criteria

### Audio Quality
- [ ] No clicks, pops, or zipper noise
- [ ] Mode switching is click-free and immediate
- [ ] Saturation doesn't clip unexpectedly
- [ ] Noise texture blends naturally

### Performance
- [ ] CPU usage <15% on sustained audio
- [ ] No real-time thread glitches
- [ ] Memory usage stable (no allocations in audio loop)

### DAW Compatibility
- [ ] Loads in Reaper without crashes
- [ ] Parameter automation works smoothly
- [ ] Preset save/load functionality (if added)
- [ ] Responsive to MIDI/automation

---

## 12. Deliverables Summary

**Phase 9 (Enhanced v1)**:
1. Saturation stage with soft-clipping
2. Procedural noise generator
3. Parameter smoothing (LinearSmoothedValue)
4. Updated GUI with new controls
5. Comprehensive testing in Reaper

**Phase 10 (Advanced v1.5)**:
1. Presence peak EQ module
2. Optional compression
3. Stereo processing modes
4. Bit-crusher (lo-fi)

**Phase 11 (v2 Aesthetic)**:
1. Embedded vinyl sample playback
2. Advanced GUI with metering
3. Preset manager
4. Professional documentation

---

## 13. Notes & References

- **JUCE DSP Documentation**: https://docs.juce.com/master/classjuce_1_1dsp_1_1IIR_1_1Filter.html
- **WaveShaper Reference**: https://docs.juce.com/master/classjuce_1_1dsp_1_1WaveShaper.html
- **Best Practices**: Refer to `PROFESSIONAL_RECOMMENDATIONS.md`
- **Current Status**: See `guides/build_journal.md` for Phase 8 completion details
