# Build Prerequisites & Installation Guide

**paranoidFilteroid-effect-plugin**  
**Updated**: November 28, 2025

---

## Overview

To build the paranoidFilteroid VST3 plugin, you need:

1. **CMake** (≥ 3.16)
2. **Visual Studio 2022** (with C++ tools)
3. **JUCE Framework** (v8.1.x)
4. **Git** (already installed ✅)

---

## Step 1: Install CMake

### Option A: Official Installer (Recommended)

1. Visit: https://cmake.org/download/
2. Download: **cmake-3.28.x-windows-x86_64.msi** (or latest)
3. Run installer
4. **Important**: Check "Add CMake to system PATH"
5. Finish installation
6. Verify:
   ```bash
   cmake --version
   ```
   Expected: `cmake version 3.28.x ...`

### Option B: Chocolatey (if installed)

```bash
choco install cmake
```

---

## Step 2: Install Visual Studio 2022

### Required Components

1. Visit: https://visualstudio.microsoft.com/downloads/
2. Download: **Visual Studio 2022 Community** (Free)
3. Run installer
4. **Essential Workloads** (check these):
   - ✅ Desktop development with C++
   - ✅ C++ core features
5. **Optional Components**:
   - ✅ MSVC v143 compiler toolset
   - ✅ Windows 11 SDK
6. Install (takes ~10–20 minutes)
7. Verify:
   ```bash
   msbuild /version
   ```
   Expected: `Microsoft (R) Build Engine version 17.x ...`

### Note

- If MSBuild is not in PATH, add: `C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin`
- Restart terminal/PowerShell after installation for PATH changes

---

## Step 3: Install JUCE Framework

### Option A: Git Clone (Recommended)

```bash
# Create dev directory
mkdir C:\dev

# Clone JUCE
git clone https://github.com/juce-framework/JUCE.git --branch 8.1.0 C:\dev\JUCE

# Verify
dir C:\dev\JUCE
```

Expected files:
```
C:\dev\JUCE\
├── CMakeLists.txt
├── modules/
├── examples/
├── extras/
└── docs/
```

### Option B: Download ZIP

1. Visit: https://github.com/juce-framework/JUCE/releases
2. Find: **v8.1.0** (or latest stable)
3. Download: **Source code (zip)**
4. Extract to: `C:\dev\JUCE`
5. Verify structure (same as above)

### Verify Installation

```bash
cd C:\dev\JUCE
dir
# Should show: CMakeLists.txt, modules/, etc.
```

---

## Step 4: Verify All Prerequisites

Run this command to verify everything is installed:

```bash
echo "Checking prerequisites..."
cmake --version
echo.
msbuild /version
echo.
dir "C:\dev\JUCE\CMakeLists.txt"
echo.
git --version
echo.
echo "All prerequisites verified!"
```

Expected output:
```
cmake version 3.28.x ...
Microsoft (R) Build Engine version 17.x ...
Volume in drive C has no label
  Directory of C:\dev\JUCE
    CMakeLists.txt
git version 2.51.x ...
All prerequisites verified!
```

---

## Step 5: Build the Plugin

### Navigate to Project

```bash
cd "c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin"
```

### Configure CMake

```bash
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
```

Expected output:
```
-- ...
-- Configuring done
-- Generating build files for Visual Studio 17 2022
```

### Compile Debug Build

```bash
cmake --build . --config Debug --parallel 4
```

Expected output (end of build):
```
Build succeeded.
0 Warning(s)
0 Error(s)
```

### Compile Release Build (Optional but Recommended)

```bash
cmake --build . --config Release --parallel 4
```

### Verify Plugin Created

```bash
dir "C:\Program Files\Common Files\VST3\paranoidFilteroid.vst3\Contents\x86_64-win\"
```

Expected files:
```
paranoidFilteroid.dll    (plugin binary)
paranoidFilteroid.lib    (import library)
```

---

## Troubleshooting

### CMake not found

**Problem**: `cmake: The term 'cmake' is not recognized...`

**Solution**:
1. Verify CMake installation: Check if `C:\Program Files\CMake\bin\cmake.exe` exists
2. Add to PATH:
   - Right-click "This PC" → Properties
   - Advanced system settings → Environment Variables
   - Add: `C:\Program Files\CMake\bin`
3. Restart terminal and retry

### MSBuild not found

**Problem**: `msbuild: The term 'msbuild' is not recognized...`

**Solution**:
1. Find MSBuild.exe:
   ```bash
   dir "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\msbuild.exe"
   ```
2. If found, add to PATH (see CMake troubleshooting)
3. If not found, reinstall Visual Studio with "Desktop development with C++"

### JUCE path not found

**Problem**: CMake error: `JUCE not found at C:/dev/JUCE`

**Solution**:
1. Verify JUCE location:
   ```bash
   dir "C:\dev\JUCE\CMakeLists.txt"
   ```
2. If JUCE is elsewhere, update `CMakeLists.txt`:
   - Open: `c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin\CMakeLists.txt`
   - Find: `add_subdirectory(C:/dev/JUCE JUCE)`
   - Replace with your JUCE path
   - Save and reconfigure CMake

### Compiler error: "juce_audio_processors.h not found"

**Problem**: Build fails with include file not found

**Solution**:
1. Ensure JUCE is at `C:\dev\JUCE`
2. Verify CMakeLists.txt has correct JUCE path
3. Delete `build/` folder and start fresh:
   ```bash
   rmdir /s build
   mkdir build
   cd build
   cmake .. -G "Visual Studio 17 2022" -A x64
   cmake --build . --config Debug
   ```

### Build fails with "cannot open output file"

**Problem**: Build error during linking phase

**Solution**:
1. Close DAW if plugin is loaded (file lock)
2. Retry build:
   ```bash
   cmake --build . --config Debug
   ```

---

## JUCE Path Configuration

The plugin build system expects JUCE at: **`C:/dev/JUCE`**

If you installed JUCE elsewhere, you have two options:

### Option 1: Move JUCE (Easiest)

```bash
# If JUCE is at C:\JUCE:
mkdir C:\dev
move C:\JUCE C:\dev\JUCE
```

### Option 2: Update CMakeLists.txt

Edit: `c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin\CMakeLists.txt`

Find this line:
```cmake
add_subdirectory(C:/dev/JUCE JUCE)
```

Replace with your JUCE path:
```cmake
add_subdirectory(C:/your/path/to/JUCE JUCE)
```

Examples:
```cmake
add_subdirectory(C:/JUCE JUCE)
add_subdirectory(D:/libraries/JUCE JUCE)
add_subdirectory(C:/Users/HP/Documents/JUCE JUCE)
```

---

## Manual Build Commands

If you prefer command-line builds (without IDE):

```bash
# Navigate to project
cd "c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin\build"

# Clean previous builds (optional)
cmake --build . --config Debug --target clean

# Build Debug
cmake --build . --config Debug --parallel 4

# Build Release
cmake --build . --config Release --parallel 4

# Install (copies plugin to VST3 folder)
cmake --install . --config Debug
```

---

## Test Plugin in DAW

### Using Reaper (Recommended)

1. Open Reaper
2. Create new project
3. Insert audio track: `Track → Insert new track`
4. Open track effects: Click "FX" button
5. Rescan plugins: Right-click plugin list → Rescan VST3
6. Search: "paranoidFilteroid"
7. Double-click to add plugin
8. Editor window should open

### Using Other DAWs

- **FL Studio**: `View → Mixer → Plugin database → Rescan`
- **Ableton Live**: `Live Preferences → File/Folder → Rescan VST`
- **Studio One**: `Preferences → Locations → VST3 → Rescan`

---

## Next Steps

After successful build and DAW test:

1. Read: `reports/BUILD_PROGRESS.md` - Current status
2. Read: `reports/IMPLEMENTATION_CHECKLIST.md` - Detailed next steps
3. Implement DSP modules (filters, EQ, compressor, etc.)
4. Enhance GUI and add visual feedback

---

## Support

If build still fails after following all steps:

1. Check: `reports/IMPLEMENTATION_CHECKLIST.md` - Troubleshooting section
2. Check: `reports/RESEARCH_FINDINGS.md` - Technical reference
3. Verify all prerequisites are installed (versions match)

