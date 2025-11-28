# 📖 Build Journal - paranoidFilteroid VST3 Plugin
**Project**: paranoidFilteroid-effect-plugin  
**Date Started**: November 28, 2025  
**Build Engineer**: Claude AI + Developer  
**Objective**: Build a minimal, testable JUCE VST3 audio plugin with phone/radio voice effects

---

## 🎯 Project Overview

This journal documents the complete build process of the paranoidFilteroid VST3 plugin, from tool installation through DAW testing. It serves as both a development log and a reference guide for future rebuilds.

**Build Date**: November 28, 2025  
**Platform**: Windows 10/11, x64  
**Target**: VST3 audio plugin  

**Final Goal**: Working VST3 plugin loadable in DAW with 3 controls (Mode, Mix, Enabled) and clean audio pass-through.

**Key Milestones**:
1. ✅ Code implementation complete (Source files created)
2. ✅ CMakeLists.txt configuration ready
3. ⏳ Tools installation (CMake, VS2022, JUCE)
4. ⏳ CMake configuration
5. ⏳ First build
6. ⏳ DAW testing
7. ⏳ Release build (optional)

---

## 📋 Build Checklist

- [ ] **Phase 1**: Tool Installation (CMake, Visual Studio 2022, JUCE)
- [ ] **Phase 2**: Environment Verification
- [ ] **Phase 3**: CMake Configuration
- [ ] **Phase 4**: First Build (Debug)
- [ ] **Phase 5**: Plugin Verification
- [ ] **Phase 6**: DAW Testing
- [ ] **Phase 7**: Release Build (Optional)

---

## 🔧 Phase 1: Tool Installation

### Current Status
**Initial System Check** (November 28, 2025 - 00:00)

| Tool | Status | Notes |
|------|--------|-------|
| Git | ✅ Installed | `C:\Program Files\Git\cmd\git.exe` |
| CMake | ❌ Missing | Need to install 3.28+ |
| Visual Studio 2022 | ❌ Missing | Need full installation with C++ tools |
| JUCE 8.1.0 | ❌ Missing | Need to clone to `C:\dev\JUCE` |

### 1.1 Installing CMake

**Objective**: Install CMake 3.28+ and add to system PATH

**Steps**:
1. Download CMake installer from https://cmake.org/download/
2. Run installer and follow wizard
3. **IMPORTANT**: Check "Add CMake to system PATH" during installation
4. Restart terminal to apply PATH changes
5. Verify installation: `cmake --version`

**Expected Outcome**:
```
cmake version 3.28.x ...
CMake suite maintained and supported by Kitware (kitware.com/cmake).
```

### 1.2 Installing Visual Studio 2022 Community

**Objective**: Install Visual Studio 2022 with C++ development tools

**Steps**:
1. Download VS 2022 Community from https://visualstudio.microsoft.com/downloads/
2. Run installer
3. **CRITICAL**: Select "Desktop development with C++" workload
4. Include optional components:
   - ✓ MSVC v143 compiler toolset
   - ✓ Windows 11 SDK
5. Complete installation (15-30 minutes)
6. Restart computer after installation completes
7. Verify: Open new PowerShell, run `msbuild /version`

**Expected Outcome**:
```
Microsoft (R) Build Engine version 17.x.xxxxx
```

### 1.3 Installing JUCE Framework

**Objective**: Clone JUCE 8.1.0 to `C:\dev\JUCE`

**Steps**:
1. Create dev directory:
   ```powershell
   mkdir C:\dev -Force
   ```
2. Clone JUCE:
   ```powershell
   git clone https://github.com/juce-framework/JUCE.git --branch 8.1.0 C:\dev\JUCE
   ```
3. Wait for clone to complete (~500 MB download)
4. Verify structure:
   ```powershell
   dir C:\dev\JUCE
   ```

**Expected Outcome**:
```
Directory: C:\dev\JUCE

Mode                 LastWriteTime         Length Name
----                 -------------         ------ ----
d-----         11/28/2025   3:00 PM                .git
d-----         11/28/2025   3:00 PM                modules
d-----         11/28/2025   3:00 PM                examples
d-----         11/28/2025   3:00 PM                extras
-a----         11/28/2025   3:00 PM           XXX  CMakeLists.txt
```

---

## 📝 Installation Logs

### CMake Installation Log
**Status**: ⏳ Pending User Action  
**Guide**: See INSTALL_GUIDE.md (Step 1)  
**Expected Duration**: 10 minutes  
**Notes**: 
- Download from https://cmake.org/download/
- CRITICAL: Check "Add CMake to system PATH" during installation
- Must restart PowerShell after installation

### Visual Studio 2022 Installation Log
**Status**: ⏳ Pending User Action  
**Guide**: See INSTALL_GUIDE.md (Step 2)  
**Expected Duration**: 40 minutes (mostly download/installation)  
**Notes**: 
- Download from https://visualstudio.microsoft.com/downloads/
- SELECT: "Desktop development with C++" workload
- CRITICAL: Restart computer after installation

### JUCE Clone Log
**Status**: ⏳ Pending User Action  
**Guide**: See INSTALL_GUIDE.md (Step 3)  
**Expected Duration**: 10-15 minutes  
**Command**:
```powershell
git clone https://github.com/juce-framework/JUCE.git --branch 8.1.0 C:\dev\JUCE
```
**Notes**: ~500 MB download, depends on internet speed

---

## 🔍 Phase 2: Environment Verification (Next)

Once all tools are installed, we'll verify:
- [ ] CMake in PATH: `cmake --version`
- [ ] MSBuild in PATH: `msbuild /version`
- [ ] JUCE structure: Verify key folders exist
- [ ] All tools accessible from any directory

---

## 🛠️ Phase 3: CMake Configuration (Pending)

**Command**:
```bash
cd c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin\build
cmake .. -G "Visual Studio 17 2022" -A x64
```

**Expected Output**:
```
-- ...
-- Configuring done (X.Xs)
-- Generating build files for Visual Studio 17 2022
```

**Troubleshooting Points**:
- If JUCE not found: Update CMakeLists.txt JUCE path
- If compiler not found: Ensure VS2022 C++ tools installed

---

## 🏗️ Phase 4: First Build (Pending)

**Debug Build Command**:
```bash
cmake --build . --config Debug --parallel 4
```

**Expected Output**:
```
Build succeeded.
0 Warning(s)
0 Error(s)
```

**Plugin Location After Build**:
```
C:\Program Files\Common Files\VST3\paranoidFilteroid.vst3\Contents\x86_64-win\
├── paranoidFilteroid.dll
└── paranoidFilteroid.lib
```

---

## ✅ Phase 5: Plugin Verification (Pending)

Verification steps after successful build:
- [ ] Plugin binary exists at VST3 location
- [ ] Binary file size > 1 MB
- [ ] No missing DLL dependencies

---

## 🎵 Phase 6: DAW Testing (Pending)

**Test Platform**: Reaper (or your DAW)

**Test Procedure**:
1. Open DAW
2. Rescan plugins (right-click plugin list)
3. Search for "paranoidFilteroid"
4. Load on audio track
5. Verify editor window opens
6. Test controls: Mode, Mix, Enabled toggle
7. Verify audio pass-through
8. Test parameter persistence

**Expected Results**:
- ✅ Plugin loads without crash
- ✅ 3 controls visible and responsive
- ✅ Audio passes through unchanged when enabled
- ✅ Audio mutes when disabled

---

## 📊 Build Statistics (To Be Filled)

| Metric | Value | Notes |
|--------|-------|-------|
| Total Installation Time | --- | |
| CMake Download Size | 50 MB | approx |
| Visual Studio Download Size | 1-2 GB | approx |
| JUCE Clone Size | 500 MB | approx |
| First Build Time | --- | Debug config |
| Debug Binary Size | --- | paranoidFilteroid.dll |
| CPU Cores Used | 4 | parallel builds |

---

## 🎓 Lessons Learned & Notes

(To be filled as we proceed)

---

## 🔗 References

- **Official Docs**:
  - CMake: https://cmake.org/cmake/help/latest/
  - JUCE: https://docs.juce.com/
  - Steinberg VST3: https://docs.steinberg.net/display/VST

- **This Project**:
  - PREREQUISITES.md - Detailed installation guide
  - QUICKSTART.md - 3-step build summary
  - BUILD_COMPLETE.md - Architecture overview
  - reports/IMPLEMENTATION_CHECKLIST.md - Troubleshooting

---

## 🎯 Phase 7: DSP Infrastructure - COMPLETE ✅

### Mode Parameter Routing & DSP Chain (November 28, 2025)

**Status**: ✅ **PLUGIN COMPILES** - DSP infrastructure implemented, ready for filter coefficients

**What Was Built**:
1. **TelephonyFilter** (Source/dsp/TelephonyFilter.h) - Pass-through stub, ready for bandpass filtering (300-3400Hz)
2. **RadioFilter** (Source/dsp/RadioFilter.h) - Pass-through stub, ready for brightness enhancement (80Hz HP + 2kHz presence peak)
3. **DSPChain** (Source/dsp/DSPChain.h) - Orchestrates filter selection based on Mode parameter, handles wet/dry blending
4. **PluginProcessor Integration** - Now reads Mode ComboBox and routes audio through DSPChain

**Technical Achievement**:
- ✅ Mode parameter (Telephone/Radio/Custom) properly read in processBlock()
- ✅ Audio routed through selected filter via DSPChain
- ✅ Mix parameter controls wet/dry blending (0-100%)
- ✅ Enabled toggle still provides bypass functionality
- ✅ Real-time safe - no allocations in audio thread
- ✅ Compiles with zero errors (4 parameter warnings are expected JUCE boilerplate)
- ✅ Plugin: 23.6 MB debug binary

**Current Audio Path**:
```
Input → PluginProcessor → DSPChain → [Mode selection] →
  ├─ Telephone: Pass-through (ready for 300-3400Hz bandpass)
  ├─ Radio: Pass-through (ready for 80Hz HP + 2kHz peak)
  └─ Custom: Pass-through (ready for chained filters)
→ Mix blending (wet/dry) → Output
```

**Next Phase Tasks**:
- Implement actual IIR filter coefficients in TelephonyFilter and RadioFilter
- Use JUCE IIR::Coefficients API (need to research JUCE 8.1.0 specific factory methods)
- Test all three modes with speech and music audio in Reaper
- Validate audio quality and CPU usage

**Build Status**: ✅ Phase 7 infrastructure complete, audio passes through Mode switching infrastructure

---

## 📅 Timeline

| Date | Event | Status |
|------|-------|--------|
| 11/28/2025 | Project initialized, code completed | ✅ Done |
| 11/28/2025 | Documentation created | ✅ Done |
| 11/28/2025 | Tool installation (CMake 4.2.0, VS2026) | ✅ Done |
| 11/28/2025 | CMake configuration & multiple build fixes | ✅ Done |
| 11/28/2025 | First build (resolved API/VST2-VST3 issues) | ✅ Done |
| 11/28/2025 | DAW testing & audio passthrough fix | ✅ Done |

---

## 🎯 Phase 8: Real IIR Filter Coefficients - COMPLETE ✅

### Telephony & Radio Voice Effects Implementation (November 28-29, 2025)

**Status**: ✅ **PLUGIN COMPILES WITH REAL FILTERS** - All DSP processing implemented

**What Was Built**:

1. **TelephonyFilter** (Source/dsp/TelephonyFilter.h) - **NOW REAL**
   - High-pass filter: 300 Hz, Q=1.0 (removes low rumble)
   - Low-pass filter: 3400 Hz, Q=1.0 (removes sibilance)
   - Creates characteristic narrowband voice effect
   - JUCE API: `makeHighPass()`, `makeLowPass()`

2. **RadioFilter** (Source/dsp/RadioFilter.h) - **NOW REAL**
   - High-pass filter: 100 Hz, Q=0.7 (gentle rumble removal)
   - Presence peak filter: 3500 Hz, Q=2.0, +6dB gain (brightness enhancement)
   - Creates bright, clear "radio voice" effect
   - JUCE API: `makeHighPass()`, `makePeakFilter()`

3. **Audio Processing Chain Fixed**
   - Discovered API issue: `ProcessContextReplacing` requires `AudioBlock<float>`, not `AudioBuffer<float>`
   - Solution: Convert buffer to AudioBlock: `juce::dsp::AudioBlock<float> audioBlock(buffer);`
   - Both filters now correctly chain audio through IIR processing

**Technical Achievement**:
- ✅ Real-time safe DSP coefficients using JUCE 8.1.0 IIR API
- ✅ Two distinct filter implementations producing different sonic effects
- ✅ Proper ProcessContext wrapping for JUCE DSP compatibility
- ✅ No allocations in audio thread (coefficients pre-computed in prepare())
- ✅ Compiles with zero errors (4 parameter warnings are JUCE boilerplate)
- ✅ Plugin: **23.8 MB** debug binary

**Actual Audio Path Now**:
```
Input → PluginProcessor → DSPChain → [Mode selection] →
  ├─ Telephone: 300Hz HP + 3400Hz LP (narrowband voice)
  ├─ Radio: 100Hz HP + 3500Hz presence peak (bright voice)
  └─ Custom: Both filters chained (maximum processing)
→ Mix blending (wet/dry) → Output
```

**Filter Specifications**:

**Telephone Filter (Narrowband Voice)**:
- Frequency Response: 300 Hz - 3400 Hz passband
- Use Case: Heavy voice compression (like old telephone lines)
- Character: Thin, compressed, recognizable but unnatural
- IIR Order: 4th order total (2x 2nd order stages)

**Radio Filter (Bright Voice)**:
- Frequency Response: 100 Hz rumble removal + 3500 Hz presence boost
- Use Case: Clear, present voice enhancement
- Character: Bright, punchy, easy-to-hear speech
- Presence Peak: +6 dB boost at 3500 Hz for clarity
- IIR Order: 4th order total (2x 2nd order stages)

**Build & Compilation Notes**:
- Configuration: 157.6s (JUCE helpers compilation)
- Compilation: Successful on first attempt after API fix
- No linker errors or PDB conflicts
- VST3 bundle created: `build/paranoidFilteroid_artefacts/Debug/VST3/paranoidFilteroid.vst3`

**Next Phase Tasks**:
- Load plugin in Reaper DAW
- Test each mode (Telephone/Radio/Custom):
  - Listen to speech with different modes
  - Verify distinct sonic differences
  - Test real-time mode switching (no clicks/pops)
  - Measure CPU usage (target: <15%)
- Record results in Phase 9 section

**Commits Made**:
- `f59b915` - Phase 8: Implement real IIR filter coefficients with correct JUCE API

**Build Status**: ✅ Phase 8 complete - Ready for DAW testing

---

## 📅 Timeline

| Date | Event | Status |
|------|-------|--------|
| 11/28/2025 | Project initialized, code completed | ✅ Done |
| 11/28/2025 | Documentation created | ✅ Done |
| 11/28/2025 | Tool installation (CMake 4.2.0, VS2026) | ✅ Done |
| 11/28/2025 | CMake configuration & multiple build fixes | ✅ Done |
| 11/28/2025 | First build (resolved API/VST2-VST3 issues) | ✅ Done |
| 11/28/2025 | DAW testing & audio passthrough fix | ✅ Done |
| 11/28/2025 | Phase 7: DSP infrastructure (Mode routing) | ✅ Done |
| 11/28-29/2025 | Phase 8: Real filter coefficients | ✅ Done |

---

## 🚀 Next Steps - Phase 9: DAW Testing & Validation

**Current Milestone**: All DSP implemented and compiling ✅

**Remaining Work**:
1. **Load plugin in Reaper** - Verify VST3 loads and initializes
2. **Test Telephone mode** - Verify narrowband effect on speech
3. **Test Radio mode** - Verify brightness enhancement
4. **Test Custom mode** - Verify chained filter effect
5. **Real-time switching** - No clicks/pops between modes
6. **CPU profiling** - Verify <15% CPU target on test audio
7. **Audio quality** - Listen for artifacts, noise floor, distortion
8. **Release build** - Performance optimization (optional)

---

**Build Status**: ✅ **PHASE 8 COMPLETE - FILTERS REAL & COMPILED**  
**Last Updated**: November 29, 2025  
**Next Checkpoint**: Phase 9 DAW testing & validation




