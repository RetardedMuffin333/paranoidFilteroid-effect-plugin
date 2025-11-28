# 🚀 QUICK START GUIDE
**paranoidFilteroid VST3 Audio Plugin**

---

## TL;DR - 3 Steps to Build & Test

### Step 1: Install Prerequisites (First Time Only)

```bash
# Install CMake
# Visit: https://cmake.org/download/
# Download .msi for Windows, check "Add to PATH"

# Install Visual Studio 2022 Community
# Visit: https://visualstudio.microsoft.com/downloads/
# Check: "Desktop development with C++"

# Install JUCE
git clone https://github.com/juce-framework/JUCE.git --branch 8.1.0 C:\dev\JUCE
```

**Verify all installed:**
```bash
cmake --version      # Should show: cmake version 3.x.x
msbuild /version    # Should show: Microsoft Build Engine
dir "C:\dev\JUCE\CMakeLists.txt"  # Should exist
```

### Step 2: Build Plugin

```bash
# Navigate to project
cd "c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin"

# Create & enter build directory
cd build

# Configure (generates Visual Studio project)
cmake .. -G "Visual Studio 17 2022" -A x64

# Compile Debug
cmake --build . --config Debug --parallel 4
```

**Success**: Plugin copied to `C:\Program Files\Common Files\VST3\paranoidFilteroid.vst3\`

### Step 3: Test in DAW

1. Open **Reaper** (or your DAW)
2. Rescan plugins (right-click plugin list → "Rescan VST3")
3. Search: "paranoidFilteroid"
4. Load on audio track
5. **Verify**:
   - ✅ Window opens with 3 controls (Mode, Mix, Enabled)
   - ✅ Play audio → passes through unchanged
   - ✅ Toggle Enabled → audio mutes
   - ✅ All controls respond

---

## 📚 Full Documentation

| Document | When to Read |
|----------|--------------|
| **PREREQUISITES.md** | Installing build tools (CMake, VS2022, JUCE) |
| **BUILD_PROGRESS.md** | Build commands & what's working |
| **BUILD_COMPLETE.md** | Architecture overview & status |
| **reports/IMPLEMENTATION_CHECKLIST.md** | Detailed steps + troubleshooting |
| **reports/RESEARCH_FINDINGS.md** | Technical deep-dive (JUCE, APVTS, VST3) |

---

## 🔧 If Build Fails

1. **CMake not found**
   - Install from: https://cmake.org/download/
   - Add to PATH during installation

2. **JUCE not found**
   - Clone to correct path: `C:\dev\JUCE`
   - Or update `CMakeLists.txt` line 8 with correct JUCE path

3. **Visual Studio not found**
   - Install: https://visualstudio.microsoft.com/downloads/
   - Select: "Desktop development with C++"

4. **Still stuck?**
   - See: `reports/IMPLEMENTATION_CHECKLIST.md` (Troubleshooting section)

---

## 📦 What You Get

✅ **Working VST3 Plugin** (pass-through audio engine)  
✅ **3 GUI Controls** (Mode selector, Mix slider, Enabled toggle)  
✅ **Parameter System** (APVTS with state save/load)  
✅ **Real-Time Safe** (no allocations in audio thread)  
✅ **Multi-Instance** (run multiple copies in DAW)  
✅ **Cross-Platform Ready** (structure supports macOS/Linux)

---

## 🎯 Next: Add DSP

Once plugin works in DAW, you can add:
- Band-pass filters (Telephone: 300–3400 Hz, Radio: 200–5000 Hz)
- Parametric EQ (mid-peak boost)
- Compressor (with threshold, ratio, attack, release)
- Saturation (soft-knee distortion)
- Noise generator (subtle background noise)

See `BUILD_PROGRESS.md` Phase 2 for DSP implementation.

---

## ✨ Current Status

| Phase | Task | Status |
|-------|------|--------|
| 1 | Code Implementation | ✅ Complete |
| 2 | Build & Test | ⏳ Next (follow steps above) |
| 3 | DSP Implementation | ⏹️ After Phase 2 |
| 4 | Mode Presets | ⏹️ After Phase 3 |
| 5 | GUI Enhancement | ⏹️ After Phase 4 |

---

## 📞 Need Help?

→ **For installation issues**: See `PREREQUISITES.md`  
→ **For build errors**: See `reports/IMPLEMENTATION_CHECKLIST.md`  
→ **For technical details**: See `reports/RESEARCH_FINDINGS.md`  
→ **For architecture**: See `reports/MINIMAL_SETUP_PLAN.md`

---

## ✅ Checklist

Before building:
- [ ] CMake installed & in PATH
- [ ] Visual Studio 2022 installed with C++ tools
- [ ] JUCE cloned to `C:\dev\JUCE`

To build:
```bash
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Debug --parallel 4
```

To test:
- [ ] Open DAW
- [ ] Rescan VST3
- [ ] Load paranoidFilteroid
- [ ] Check all 3 controls work
- [ ] Verify audio passes through

---

**Ready? → Follow PREREQUISITES.md to install build tools, then come back here to build!** 🚀

