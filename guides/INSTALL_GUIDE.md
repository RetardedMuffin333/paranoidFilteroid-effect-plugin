# Installation & Build Guide for paranoidFilteroid
**Step-by-Step Instructions for Windows**

---

## Current System Status

✓ Git: Installed  
✗ CMake: Missing  
✗ Visual Studio 2022: Missing  
✗ JUCE: Missing  

---

## Installation Steps

### Step 1: Install CMake (10 minutes)

1. **Download**
   - Visit: https://cmake.org/download/
   - Download: `cmake-3.28.3-windows-x86_64.msi` (or latest 3.28.x)

2. **Install**
   - Run the `.msi` installer
   - Follow setup wizard
   - **CRITICAL**: When prompted, check the box: **"Add CMake to system PATH"**
   - Click "Install"
   - Wait for completion

3. **Verify**
   - Close PowerShell completely
   - Open a NEW PowerShell window
   - Run: `cmake --version`
   - Expected output: `cmake version 3.28.x ...`

**✓ Step 1 Complete**: CMake installed and in PATH

---

### Step 2: Install Visual Studio 2022 Community (30-40 minutes)

1. **Download**
   - Visit: https://visualstudio.microsoft.com/downloads/
   - Click: "Community" under Visual Studio 2022
   - Download the installer (~2-3 MB)

2. **Run Installer**
   - Run the downloaded installer (vs_Community.exe)
   - Wait for the installer to load (may take 1-2 minutes)

3. **Select Workload**
   - Look for: **"Desktop development with C++"**
   - **CHECK THIS BOX** (it's critical!)
   - Optional: Also select "Windows 11 SDK" if available

4. **Install**
   - Click "Install"
   - Wait for installation to complete (15-30 minutes)
   - This involves downloading components (~4-8 GB)

5. **Post-Installation**
   - VS2022 will prompt to restart computer
   - **DO NOT SKIP THIS STEP** - Restart your computer!
   - This allows MSBuild and compilers to be added to PATH

6. **Verify**
   - After restart, open PowerShell
   - Run: `msbuild /version`
   - Expected output: `Microsoft (R) Build Engine version 17.x ...`

**✓ Step 2 Complete**: Visual Studio 2022 installed with C++ tools

---

### Step 3: Install JUCE Framework (10-15 minutes)

1. **Create Directory**
   - Open PowerShell
   - Run: `mkdir C:\dev -Force`

2. **Clone JUCE**
   - Run this command (all one line):
   ```powershell
   git clone https://github.com/juce-framework/JUCE.git --branch 8.1.0 C:\dev\JUCE
   ```
   - This downloads ~500 MB
   - Wait for clone to complete (5-10 minutes depending on internet)

3. **Verify**
   - Run: `dir C:\dev\JUCE`
   - You should see:
     ```
     CMakeLists.txt
     modules/
     examples/
     extras/
     docs/
     .git/
     ```

**✓ Step 3 Complete**: JUCE cloned to C:\dev\JUCE

---

## Verification

After all three steps, verify everything is working:

```powershell
# Close and reopen PowerShell first!

# Test 1: CMake
cmake --version
# Should output: cmake version 3.28.x ...

# Test 2: MSBuild
msbuild /version
# Should output: Microsoft (R) Build Engine version 17.x ...

# Test 3: JUCE
dir C:\dev\JUCE
# Should show CMakeLists.txt and folders

# Test 4: Git
git --version
# Should output: git version 2.x.x
```

**If all tests pass**, you're ready to build!

---

## Building the Plugin

Once all tools are installed, follow these steps:

```powershell
# Step 1: Navigate to project
cd "c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin"

# Step 2: Go to build folder
cd build

# Step 3: Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64

# Step 4: Build Debug version
cmake --build . --config Debug --parallel 4
```

**Expected output** at end of build:
```
Build succeeded.
0 Warning(s)
0 Error(s)
```

---

## If Build Fails

### CMake not found error

**Problem**: `cmake: The term 'cmake' is not recognized`

**Solution**:
1. CMake installed but not in PATH
2. Close PowerShell completely
3. Open a NEW PowerShell window
4. Try `cmake --version` again
5. If still fails, add manually:
   - Right-click "This PC" → Properties
   - Advanced system settings → Environment Variables
   - New PATH entry: `C:\Program Files\CMake\bin`
   - Restart PowerShell

### MSBuild not found error

**Problem**: `msbuild: The term 'msbuild' is not recognized`

**Solution**:
1. Visual Studio installed but MSBuild not in PATH yet
2. **Restart your computer** (this is critical!)
3. Open new PowerShell after restart
4. Try `msbuild /version` again

### JUCE not found error

**Problem**: `CMake error - JUCE not found`

**Solution**:
1. Verify JUCE is at: `C:\dev\JUCE\CMakeLists.txt`
2. If elsewhere, update `CMakeLists.txt` line 8:
   ```cmake
   add_subdirectory(C:/your/path/to/JUCE JUCE)
   ```
3. Reconfigure CMake: `cmake .. -G "Visual Studio 17 2022" -A x64`

### Compiler error in build

**Problem**: Build fails with C++ compiler errors

**Solution**:
1. Verify Visual Studio C++ tools installed
2. Run installer again, check "Desktop development with C++"
3. Delete build folder: `rmdir /s build`
4. Create new build folder: `mkdir build`
5. Reconfigure and rebuild

---

## After Successful Build

Plugin location:
```
C:\Program Files\Common Files\VST3\paranoidFilteroid.vst3\
└── Contents\x86_64-win\
    ├── paranoidFilteroid.dll
    └── paranoidFilteroid.lib
```

Next steps:
1. Open your DAW (Reaper, FL Studio, Ableton, etc.)
2. Rescan VST3 plugins
3. Search for "paranoidFilteroid"
4. Load on audio track
5. Test the 3 controls (Mode, Mix, Enabled)

---

## Troubleshooting Checklist

Before asking for help, verify:

- [ ] All 3 tools installed (CMake, VS2022, JUCE)
- [ ] PowerShell restarted after each installation
- [ ] Computer restarted after VS2022 installation
- [ ] `cmake --version` works
- [ ] `msbuild /version` works (or restart computer)
- [ ] `C:\dev\JUCE\CMakeLists.txt` exists
- [ ] CMakeLists.txt JUCE path matches your installation

---

## Quick Command Reference

```powershell
# Check all tools
cmake --version
msbuild /version
dir C:\dev\JUCE
git --version

# Build commands
cd "c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin"
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Debug --parallel 4

# Clean build (start fresh)
rmdir /s build
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Debug --parallel 4
```

---

## Estimated Total Time

- CMake installation: 10 minutes
- Visual Studio 2022 installation: 40 minutes (mostly download)
- JUCE clone: 10 minutes
- First build: 5-10 minutes
- **Total: ~70-80 minutes**

---

## Support & Reference

See these files for more info:
- `BUILD_PROGRESS.md` - Build status and next steps
- `QUICKSTART.md` - 3-step overview
- `BUILD_COMPLETE.md` - Full feature overview
- `reports/IMPLEMENTATION_CHECKLIST.md` - Detailed technical info

