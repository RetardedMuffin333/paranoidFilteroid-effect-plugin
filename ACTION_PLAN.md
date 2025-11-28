# 🚀 INSTALLATION ACTION PLAN
**Next Steps to Build paranoidFilteroid Plugin**

---

## Current Status

| Component | Status | Action |
|-----------|--------|--------|
| Code Implementation | ✅ DONE | - |
| CMake | ❌ MISSING | Install (Step 1) |
| Visual Studio 2022 | ❌ MISSING | Install (Step 2) |
| JUCE Framework | ❌ MISSING | Clone (Step 3) |
| First Build | ⏳ PENDING | After tools installed |
| DAW Testing | ⏳ PENDING | After successful build |

---

## What You Need to Do

### 🎯 Step 1: Install CMake (10 min)

1. Go to: https://cmake.org/download/
2. Download: `cmake-3.28.3-windows-x86_64.msi` (latest 3.28.x)
3. Run installer
4. **CHECK**: "Add CMake to system PATH" ← Important!
5. Finish installation
6. **RESTART PowerShell** (close and open new one)
7. Verify: `cmake --version` → Should show version 3.28.x

**✓ Move to Step 2**

---

### 🎯 Step 2: Install Visual Studio 2022 (40 min)

1. Go to: https://visualstudio.microsoft.com/downloads/
2. Click: "Download" for "Community" edition
3. Run installer (vs_Community.exe)
4. **SELECT**: "Desktop development with C++"  ← Very Important!
5. Optional: Add "Windows 11 SDK"
6. Click: "Install"
7. Wait 15-30 minutes for download & installation
8. **RESTART COMPUTER** ← Critical step!
9. After restart, verify: `msbuild /version` → Should show version 17.x

**✓ Move to Step 3**

---

### 🎯 Step 3: Clone JUCE Framework (10 min)

1. Open PowerShell
2. Create dev directory:
   ```powershell
   mkdir C:\dev -Force
   ```
3. Clone JUCE:
   ```powershell
   git clone https://github.com/juce-framework/JUCE.git --branch 8.1.0 C:\dev\JUCE
   ```
4. Wait for clone to complete (~500 MB, 5-10 min)
5. Verify:
   ```powershell
   dir C:\dev\JUCE
   ```
   Should show: `CMakeLists.txt`, `modules/`, `examples/`, etc.

**✓ All tools installed!**

---

## Verification Checklist

After all 3 steps, verify everything:

```powershell
# Test CMake
cmake --version
# Expected: cmake version 3.28.x ...

# Test MSBuild
msbuild /version
# Expected: Microsoft (R) Build Engine version 17.x ...

# Test JUCE
dir C:\dev\JUCE
# Expected: CMakeLists.txt, modules, examples folders

# Test Git
git --version
# Expected: git version 2.x.x
```

✓ **If all pass, you're ready to build!**

---

## Building the Plugin

Once tools are installed:

```powershell
# Navigate to project
cd "c:\Users\HP\Documents\Aljaz\GitHub projects\paranoidFilteroid-effect-plugin"

# Enter build folder
cd build

# Configure CMake
cmake .. -G "Visual Studio 17 2022" -A x64

# Build Debug version
cmake --build . --config Debug --parallel 4
```

Expected output:
```
Build succeeded.
0 Warning(s)
0 Error(s)
```

Plugin will be at:
```
C:\Program Files\Common Files\VST3\paranoidFilteroid.vst3\Contents\x86_64-win\paranoidFilteroid.dll
```

---

## Testing in DAW

After build succeeds:

1. Open your DAW (Reaper, FL Studio, Ableton, etc.)
2. Rescan plugins (right-click plugin list)
3. Search for "paranoidFilteroid"
4. Load on audio track
5. Verify:
   - ✅ Editor window opens
   - ✅ 3 controls visible (Mode, Mix, Enabled)
   - ✅ Audio passes through
   - ✅ Controls respond to input

---

## If Something Fails

See **INSTALL_GUIDE.md** troubleshooting section:
- CMake not found error → How to fix PATH
- MSBuild not found error → Restart computer
- JUCE not found error → How to update path in CMakeLists.txt
- Build compiler errors → How to reinstall VS2022

---

## Estimated Time

- Step 1 (CMake): 10 minutes
- Step 2 (Visual Studio): 40 minutes
- Step 3 (JUCE): 10 minutes
- Build: 5-10 minutes
- **Total: ~70-80 minutes**

---

## Quick Reference

Full step-by-step guide: **INSTALL_GUIDE.md**  
Build progress log: **build_journal.md**  
3-step overview: **QUICKSTART.md**  
Architecture details: **BUILD_COMPLETE.md**  

---

## Next Action

→ **Read INSTALL_GUIDE.md and follow Step 1 (CMake installation)**

Once done, come back here and do Step 2!

