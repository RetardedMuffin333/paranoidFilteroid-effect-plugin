# paranoidFilteroid-effect-plugin
Radio/phone voice-like effect VST plugin

**Current Status**: ✅ **FUNCTIONAL** - Audio passthrough working, GUI operational  
**Latest Build**: November 28, 2025 | Debug: 24.7 MB VST3 | Tested in Reaper ✅

## Project Structure

```
paranoidFilteroid-effect-plugin/
├── Source/                    # Plugin source code (C++)
│   ├── core/                  # Audio engine (PluginProcessor, PluginEditor)
│   ├── dsp/                   # DSP modules (future: Telephone, Radio filters)
│   └── utils/                 # Constants and utilities (DSPDefines.h)
├── guides/                    # Documentation and guides
│   ├── QUICKSTART.md          # 3-step build guide ⭐ START HERE
│   ├── NEXT_STEPS.md          # Phase 7: DSP implementation roadmap 🎯
│   ├── build_journal.md       # Complete build history & timeline
│   ├── PREREQUISITES.md       # System requirements
│   ├── BUILD_PROGRESS.md      # Build progress tracker
│   ├── BUILD_COMPLETE.md      # Architecture overview
│   ├── ACTION_PLAN.md         # Development plan
│   ├── INSTALL_GUIDE.md       # Detailed installation
│   └── DOCUMENTATION_INDEX.md # Full documentation index
├── reports/                   # Planning and research
│   ├── RESEARCH_FINDINGS.md   # Technical research (DSP theory, JUCE)
│   ├── MINIMAL_SETUP_PLAN.md  # Initial architecture plan
│   └── IMPLEMENTATION_CHECKLIST.md # Development checklist & troubleshooting
├── claude-instructions/       # AI agent rulebooks
│   ├── CLAUDE_RULEBOOK.md     # Core principles & priorities
│   ├── RESEARCH_GUIDE.md      # Research methodology
│   ├── VALIDATION_GUIDE.md    # Code validation standards
│   ├── PLANNING_AND_CHECKLIST.md
│   └── BUILD_GUIDE_GENERATION.md
├── build/                     # CMake build output (generated)
├── CMakeLists.txt             # Build configuration (VST3, JUCE 8.1.0)
├── .gitignore                 # Git ignore rules
└── README.md                  # This file
```

## 📊 Current Status Summary

| Component | Status | Notes |
|-----------|--------|-------|
| **Code Base** | ✅ Complete | PluginProcessor, PluginEditor, parameter system |
| **Build System** | ✅ Complete | CMake 4.2.0, VS2026, VST3 only |
| **Compilation** | ✅ Working | 24.7 MB debug binary, zero errors |
| **DAW Testing** | ✅ Working | Loads in Reaper, audio passes through |
| **GUI** | ✅ Working | Mode, Mix, Enabled controls functional |
| **Audio Passthrough** | ✅ Working | Tested with stereo audio, no muting |
| **DSP Filters** | ⏳ Pending | Phase 7: Implement Telephone & Radio modes |

## Quick Start

**Just want to build?**
1. Open `guides/QUICKSTART.md` - 3 simple steps
2. Build takes ~2 minutes on typical machine
3. Test in your DAW (Reaper, Studio One, etc.)

**Ready to add DSP filters?**
1. See `guides/NEXT_STEPS.md` - Implementation roadmap
2. Create `Source/dsp/TelephonyFilter.h` class
3. Design filters using JUCE DSP module
4. Test with speech & music audio

**Need detailed info?**
- `guides/PREREQUISITES.md` - Tool installation guide
- `guides/INSTALL_GUIDE.md` - Step-by-step setup
- `reports/RESEARCH_FINDINGS.md` - DSP & JUCE theory
- `guides/build_journal.md` - Full build history with all fixes applied

