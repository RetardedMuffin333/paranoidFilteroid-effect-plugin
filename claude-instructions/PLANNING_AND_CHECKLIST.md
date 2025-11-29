# PLANNING_AND_CHECKLIST.md  
**Planning Rules & Development Checklists for Claude**

This document defines how Claude must **plan**, **organize**, and **structure** all work done in this repository.  
Claude must rely on these rules whenever producing plans, tasks, roadmaps, or breakdowns.

---

# 1. Core Planning Principles

Claude must:

1. **Break tasks into explicit step-by-step plans.**
2. **Ensure every step is implementable** using JUCE and the DSP rules in this repo.
3. **Check for dependencies** (e.g., parameters must exist before GUI sliders).
4. **Always connect planning with verification** (from the VALIDATION_GUIDE.md file—coming later).
5. **Refuse ambiguous or impossible tasks** until clarified.
6. **Keep the plugin buildable at all times.**

Claude must never produce vague or abstract plans.  
All plans must be concrete, measurable, and directly tied to the repo’s architecture.

---

# 2. Required Planning Format

Every time Claude is asked to “make a plan,” “outline tasks,” or “what should we do next?” it must respond in this exact template:

**Plan:**
1. **Goal**
A clear and concrete description of what must be achieved.
2. **Requirements / Constraints**
All relevant rules, limitations, and dependencies (DSP_SPEC.md, JUCE rules, file structure, etc.).
3. **Detailed Step-by-Step Plan**
-Step 1
-Step 2
-Step 3
  |-Substep A
  |-Substep B
...
Each step must be implementable, ordered, and dependency-aware.
4. **Deliverables**
A list of all expected outputs (code files, functions, refactors, tests, etc.).
5. **Risks / Things to Validate**
A list of technical risks, missing information, and all items that must be validated after implementation.

Claude must **always** follow this format.

## 3. Master Development Checklist

This is the primary checklist Claude must use to track and validate development progress.

---

### 3.1 Phase 1-6: Foundation (COMPLETED ✅)
- [x] Create JUCE plugin project (CMake/Projucer)
- [x] Configure plugin metadata (name, type, VST3)
- [x] Add minimal pass-through processor
- [x] Build + confirm plugin loads in DAW
- [x] Implement `createParameterLayout()`
- [x] Add `mode` (Telephone/Radio/Custom)
- [x] Add basic parameters: `mix`, `enabled`
- [x] Audio passthrough confirmed in Reaper

---

### 3.2 Phase 7: DSP Infrastructure (COMPLETED ✅)
- [x] Create TelephonyFilter.h with real IIR coefficients
- [x] Create RadioFilter.h with real IIR coefficients
- [x] Create DSPChain.h orchestrator
- [x] Implement mode-based filter routing
- [x] Implement wet/dry mixing
- [x] Real-time safe (no allocations in audio thread)
- [x] Plugin loads and processes audio successfully

---

### 3.3 Phase 8: Real Filter Coefficients (COMPLETED ✅)
- [x] Implement TelephonyFilter: 300Hz HP + 3400Hz LP
- [x] Implement RadioFilter: 100Hz HP + 3500Hz presence peak
- [x] Fix AudioBlock API conversion issue
- [x] Plugin compiles and audio processes with real filters
- [x] All three modes produce distinct effects

---

### 3.4 Phase 9: Enhanced Parameters (IN PROGRESS 🔄)
- [ ] Implement LinearSmoothedValue for `mix` parameter
- [ ] Implement LinearSmoothedValue for `outputGainDb`
- [ ] Create NoiseGenerator.h (procedural white noise)
- [ ] Add WaveShaper saturation stage to DSPChain
- [ ] Add `saturationAmount` parameter to APVTS
- [ ] Add `noiseLevelDb` parameter to APVTS
- [ ] Update GUI with Saturation and Noise sliders
- [ ] Add ScopedNoDenormals for CPU optimization
- [ ] Test in Reaper: verify no zipper noise, smooth parameter changes
- [ ] Test audio quality: saturation sounds natural, noise doesn't overpower

---

### 3.5 Phase 10: Advanced DSP (PLANNED ⏳)
- [ ] Implement Presence Peak EQ (1500-3000 Hz boost)
- [ ] Implement Compression module (threshold/ratio/makeup)
- [ ] Add stereo processing modes (Mono/Dual-Mono/Stereo Spread)
- [ ] Add Lo-Fi / Bit-Crusher (sample rate reduction)
- [ ] Update APVTS with new parameters
- [ ] Update GUI layout for advanced controls
- [ ] Test all combinations in Reaper

---

### 3.6 Phase 11: Aesthetics & Polish (PLANNED ⏳)
- [ ] Add embedded vinyl crackle samples
- [ ] Implement sample playback with random timing
- [ ] Create advanced GUI with visual feedback
- [ ] Add meter displays
- [ ] Implement preset management system
- [ ] Professional documentation and help text

---

### 3.7 Audio Quality Validation (Continuous)
- [ ] Code compiles + loads cleanly (after each phase)
- [ ] No crashes or real-time issues
- [ ] No pops/clicks on parameter changes
- [ ] Each parameter affects audio as intended
- [ ] CPU usage <15% on sustained audio
- [ ] All three modes sound distinct
- [ ] Mode switching is smooth and click-free

---

## 4. Task Breakdown Rules

Claude must:
1. Identify dependencies before writing tasks.
2. Break work into atomic, executable steps.
3. Specify file-level impact for each step.
4. Always cross-check with DSP_SPEC.md.
5. Include a validation subsection with every plan.

---

## 5. Iterative Planning Rules

After each milestone Claude must:
- Update this checklist and mark progress.
- Suggest the next most logical tasks.
- Re-validate audio stability.
- Re-check past work if DSP spec changes.

Changes in DSP spec require:
- Re-evaluating affected code
- Identifying potential breaking changes
- Notifying the user

---

## 6. Rule Summary (Compact)

Claude must:
- Use the required planning template  
- Follow the master checklist  
- Produce small, ordered, dependency-aware tasks  
- Link planning ↔ implementation ↔ validation  
- Avoid vague, speculative, or unstructured plans  

