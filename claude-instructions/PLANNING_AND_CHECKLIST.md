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

### 3.1 Project Setup
- [ ] Create JUCE plugin project (CMake/Projucer)
- [ ] Configure plugin metadata (name, type, VST3)
- [ ] Add minimal pass-through processor
- [ ] Build + confirm plugin loads in DAW

---

### 3.2 Parameter System (APVTS)
- [ ] Implement `createParameterLayout()`
- [ ] Add `mode` (Telephone/Radio/Custom)
- [ ] Add filter params: `lowCutHz`, `highCutHz`
- [ ] Add EQ params: `midFreqHz`, `midGainDb`, `midQ`
- [ ] Add compressor params: threshold, ratio, attack, release, enabled
- [ ] Add `saturationAmount`
- [ ] Add `noiseLevelDb`
- [ ] Add `monoSumming`
- [ ] Add `mix`
- [ ] Check all ranges match DSP_SPEC.md

---

### 3.3 DSP Chain
- [ ] Create JUCE DSP chain object
- [ ] Implement mono summing
- [ ] Implement band-pass (HP + LP)
- [ ] Implement mid-peak EQ
- [ ] Implement compressor
- [ ] Implement saturation waveshaper
- [ ] Implement noise generator/mixer
- [ ] Implement wet/dry mix
- [ ] Add smoothing if needed
- [ ] Verify parameter → DSP updates

---

### 3.4 GUI
- [ ] Implement editor class
- [ ] Add controls for all parameters
- [ ] Connect using APVTS attachments
- [ ] Organize controls into logical groups
- [ ] Add optional resizing
- [ ] Verify all controls update DSP correctly

---

### 3.5 Mode Logic
- [ ] Implement `mode` switching
- [ ] Apply correct DSP defaults for each mode
- [ ] Avoid overwriting user-adjusted values
- [ ] Ensure UI reflects mode state

---

### 3.6 Validation (Per Feature)
- [ ] Code compiles + loads cleanly
- [ ] No crashes or RT issues
- [ ] No pops/clicks on parameter changes
- [ ] Each parameter affects audio as intended
- [ ] Matches DSP_SPEC.md behavior
- [ ] CPU usage acceptable

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

