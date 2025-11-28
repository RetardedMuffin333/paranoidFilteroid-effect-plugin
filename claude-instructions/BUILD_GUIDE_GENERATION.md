# BUILD_GUIDE_GENERATION.md
**Rules for creating a progressive build journal and a final LEGO-style instructional guide**

This document instructs Claude on how to generate a **step-by-step build guide** for the entire project.  
The guide acts as a **teaching journal**, **developer diary**, and **instruction manual** all in one.

Claude must produce this guide automatically as development progresses, and eventually compile it into a final **PDF** or **markdown book** (upon user request).

---

# 1. Purpose of the Build Guide

Claude must create a **clear, visual, step-oriented guide** that explains:

- What was built  
- Why it was built  
- How it was built  
- How each piece connects to the whole plugin  

The guide must be written for:
- Beginners learning JUCE  
- Intermediate developers wanting a structured workflow  
- Anyone who wants a clear cookbook-like reference  

Think of it as a **LEGO instruction manual** for building a VST plugin.

---

# 2. Build Guide Format (Core Structure)

Claude must create the guide in this structure:

**Title Page**
Plugin name, author, version
**Introduction**
- What the plugin does
- Overview of the project
- Prerequisites (JUCE, IDE, CMake, etc.)
**Part 1 — Setup Phase**
Step-by-step instructions with explanations:
- Install tools
- Create JUCE plugin project
- Configure VST3
- Build + load in DAW
**Part 2 — Parameters & Architecture**
Step-by-step:
- Introduce APVTS
- Add first parameters
- Create parameter layout
- Test parameters in DAW
- Explain signal flow diagram
**Part 3 — DSP Modules (Each one with steps)**
- Mono summing
- Band-pass HP/LP filters
- EQ
- Compressor
- Saturation
- Noise
- Wet/dry
For each module:
- “What this block does”
- “Why it matters”
- “Implementation steps”
- “Code snippets”
- “Validation steps”
- “Troubleshooting tips”
**Part 4 — GUI Construction**
- Add sliders
- APVTS attachments
- Layout
- Grouping
- Styling
**Part 5 — Mode System**
- Telephone mode defaults
- Radio mode defaults
- Custom mode
- Best practices
**Part 6 — Testing & Validation**
- Audio tests
- Parameter sweeps
- Stress tests
- Debugging tips
**Final Chapter — Packaging & Distribution**
- Build final binary
- Test in DAW
- Optional: Sign plugin
- Prepare release assets
**Appendices**
- DSP reference
- Glossary
- Troubleshooting
- Additional reading


Claude must **gradually fill in** this guide as development continues.

---

# 3. Step-Based, LEGO-Style Instructions

Claude must write every section like LEGO instructions:

- Small, bite-sized steps  
- Clear executable actions  
- Visual-friendly breakdowns  
- Diagrams when useful (ASCII or markdown)


This must be the tone and structure of the **entire build guide**.

---

# 4. Ongoing Journal Logging

Claude must maintain a progressive log:

Each time we build or implement something, Claude must append a new entry in the guide:

Progress Log Entry 
Summary
- What was added.
Code Added
- code snippet
- Explanation
- Why this is important.
Validation
- How to test correctness.
Next Steps
- What we will do after this.


This creates a chronological build journal.

---

# 5. Final Export Rules

When the user says:

- “Generate the final build guide”  
- “Create the handbook”  
- “Export the PDF”  

Claude must:

1. Combine all journal entries  
2. Combine all structured instructions  
3. Format cleanly  
4. Produce:
   - A single `.md` file  
   - Or a **PDF** using python_user_visible (if requested)

The final guide must be:

- Clean  
- Well structured  
- Fully readable  
- Minimal fluff  
- Beginner-friendly  
- Accurate  

---

# 6. Content Restrictions

Claude must **NOT**:

- Invent APIs not in JUCE  
- Add unverified DSP techniques  
- Add irrelevant theory  
- Generate filler content  
- Write speculative or unstable instructions  

The guide must be 100% aligned with:

- DSP_SPEC.md  
- VALIDATION_GUIDE.md  
- PLANNING_AND_CHECKLIST.md  
- Actual code generated in the project  

---

# 7. Short Summary Rules (Compact)

Claude must:

- Write step-based, LEGO-style instructions  
- Maintain a running journal  
- Explain everything clearly  
- Add diagrams or tables when helpful  
- Validate each step before adding it to the guide  
- Avoid speculation  
- Combine everything into a final PDF/Markdown manual upon request  

