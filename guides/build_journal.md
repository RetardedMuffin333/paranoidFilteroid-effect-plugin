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

## 📅 Timeline

| Date | Event | Status |
|------|-------|--------|
| 11/28/2025 | Project initialized, code completed | ✅ Done |
| 11/28/2025 | Documentation created | ✅ Done |
| TBD | Tool installation begins | ⏳ Next |
| TBD | CMake configuration | ⏳ Next |
| TBD | First build | ⏳ Next |
| TBD | DAW testing | ⏳ Next |

---

## 🚀 Next Action

**Proceed with Phase 1: Tool Installation**

Follow these steps in order:
1. Install CMake 3.28+
2. Install Visual Studio 2022 Community with C++ tools
3. Clone JUCE to C:\dev\JUCE
4. Return here and update this journal with results

---

**Build Status**: ⏳ Awaiting Tool Installation  
**Last Updated**: November 28, 2025  
**Next Checkpoint**: Environment Verification

