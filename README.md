# paranoidFilteroid-effect-plugin
Radio/phone voice-like effect VST plugin

## Project Structure

```
paranoidFilteroid-effect-plugin/
├── Source/                    # Plugin source code (C++)
│   ├── core/                  # Audio engine (PluginProcessor, PluginEditor)
│   ├── dsp/                   # DSP modules (PassThrough, future filters)
│   └── utils/                 # Constants and utilities (DSPDefines.h)
├── guides/                    # Documentation and guides
│   ├── ACTION_PLAN.md         # Quick start - 3 steps to build
│   ├── INSTALL_GUIDE.md       # Detailed installation instructions
│   ├── QUICKSTART.md          # Quick reference
│   ├── DOCUMENTATION_INDEX.md # Master guide index
│   ├── BUILD_PROGRESS.md      # Build progress tracker
│   ├── BUILD_COMPLETE.md      # Build completion details
│   ├── build_journal.md       # Personal build log
│   └── PREREQUISITES.md       # System requirements
├── reports/                   # Planning and research documents
│   ├── RESEARCH_FINDINGS.md   # Technical research
│   ├── MINIMAL_SETUP_PLAN.md  # Architecture plan
│   └── IMPLEMENTATION_CHECKLIST.md # Development checklist
├── claude-instructions/       # AI agent rulebooks and guides
│   ├── CLAUDE_RULEBOOK.md     # Project conventions
│   ├── RESEARCH_GUIDE.md      # Research methodology
│   ├── VALIDATION_GUIDE.md    # Code validation
│   ├── PLANNING_AND_CHECKLIST.md
│   └── BUILD_GUIDE_GENERATION.md
├── build/                     # CMake build output (generated)
├── CMakeLists.txt             # Build configuration
├── .gitignore                 # Git ignore rules
└── README.md                  # This file
```

## Quick Start

1. **Start here**: Open `guides/ACTION_PLAN.md` for a 3-step installation and build process
2. **Need details?** Check `guides/INSTALL_GUIDE.md` for step-by-step instructions
3. **Questions?** See `guides/DOCUMENTATION_INDEX.md` for complete navigation

