# VALIDATION_GUIDE.md  
**Rules for Verifying Code, DSP, Functionality, and Stability**

This document defines how Claude must validate all work in this repository.  
Validation must occur **after every code generation**, **every refactor**, **every DSP change**, and **every structural update**.

Claude must act as a strict QA and DSP checker.

---

# 1. General Validation Principles

Claude must confirm:

1. **Correctness**  
   - Code compiles and uses valid JUCE APIs  
   - DSP blocks follow DSP_SPEC.md exactly  
   - No invented functions, classes, or DSP algorithms

2. **Stability**  
   - No allocations in audio rendering  
   - No thread-unsafe operations  
   - No potential crashes (nullptrs, uninitialized members, etc.)

3. **Audio Integrity**  
   - No pops, clicks, DC offsets, or denormal issues  
   - Filters remain stable at all parameter values  
   - Saturation does not exceed safe output ranges  
   - Noise is controlled and optional

4. **User Experience**  
   - Sliders and controls reflect correct parameters  
   - Mode switching behaves predictably  
   - Plugin stays responsive in DAW

Claude must perform validation **before finalizing any generated code**.

---

# 2. Validation Workflow (Claude must ALWAYS follow this)

Whenever Claude produces code, it must run through this checklist:

**Validation Checklist**
1. Syntax & API Check
- Are all JUCE classes and methods real and used correctly?
2.DSP Check
- Does this block match DSP_SPEC.md?
- Are filter frequencies, gains, Q, and slopes correct?
3. Real-Time Safety Check
- Any heap allocations in audio threads?
- Any locks, file I/O, or dynamic memory ops?
4. Parameter Routing Check
- Do parameters exist in APVTS layout?
- Are they read/written correctly?
- Are ranges, units, and conversions correct?
5.Audio Quality Check
- Does processing avoid clipping unless intentional?
- Are transitions smooth (no zipper noise risk)?
6. Structural Check
- Does this code fit the repo architecture?
- Does it break any previous code?
7. Fail Conditions
- Flag any uncertainty
- Mark anything requiring user approval


Claude must **refuse to produce final code** if validation fails.

---

# 3. DSP Block Validation Rules

For each DSP component Claude implements, it must validate:

### 3.1 Filters  
- Cutoff values match DSP spec (300–3400 Hz for Telephone, etc.)  
- Using valid JUCE IIR filter factories  
- Coefficients updated safely inside `prepareToPlay` or parameter listener  
- No extreme Q values causing instability

### 3.2 Parametric EQ  
- Uses JUCE `makePeakFilter` or equivalent  
- Gain in dB converted to linear correctly  
- Q never below safety threshold (0.3)  

### 3.3 Compressor  
- Only uses JUCE dsp::Compressor  
- Threshold, ratio, attack, release connected to parameters  
- Disabled block bypasses internal processing cleanly  

### 3.4 Saturation  
- Uses known safe waveshaping (tanh or polynomial)  
- Output normalized  
- No clipping beyond -1.0 to 1.0 without intent  

### 3.5 Noise  
- Noise amplitude calculated correctly from dB  
- Noise mixed *after* main DSP but before wet/dry  
- Optional: noise passes through band-limiting  

### 3.6 Wet/Dry Mix  
- Must be a linear interpolation:  
  `out = dry * (1 - mix) + wet * mix`  
- Must avoid phase issues (no multi-pathing)  

Claude must confirm all of these rules every time.

---

# 4. Validation of Mode Logic

Claude must ensure:

- Telephone mode uses exactly:
  - 300–3400 Hz band-pass  
  - +6 dB at 1 kHz  
  - Light saturation  
  - Optional subtle compression  

- Radio mode uses:
  - Wider 200–5000 Hz band-pass  
  - Presence peak  
  - More saturation  

- Custom mode:
  - Never overwrites user-chosen parameters  
  - Enforces safe parameter limits  

Claude must check that mode switching does **not cause pops**, meaning:

- Filter coefficient changes must be smoothed  
- No instantaneous jumps in gain  

---

# 5. Parameter Validation Rules

For every parameter:

- Range must match DSP_SPEC.md  
- Default must match mode defaults  
- Parameter ID must be unique  
- Units must be appropriate (Hz, dB, ratio, % mix)  
- APVTS attachment must bind to GUI control  

Claude must validate that **every parameter used in the DSP exists in APVTS**.

If not, Claude must halt and warn the user.

---

# 6. Code Style & Architecture Validation

Claude must check that:

- New DSP code is placed in `Source/dsp/`  
- Processor owns the DSP chain  
- Editor handles only GUI  
- APVTS serves as the communication layer  
- No business logic is in the editor  
- No DSP code is in GUI  

If architecture is violated, Claude must rewrite or reorganize code.

---

# 7. Validation Before Finishing Any Task

Before closing a plan, producing code, or generating a patch, Claude must:

- Revisit the relevant checklist items  
- Confirm the new work aligns with existing architecture  
- Warn the user about any missing prerequisites  
- Suggest next validation steps  

Claude must never assume correctness.

---

# 8. Summary (Compact)

Claude must validate:

- **DSP correctness**  
- **Real-time safety**  
- **Parameter integrity**  
- **GUI consistency**  
- **Mode behavior**  
- **Audio stability**  
- **Repo architecture alignment**

Claude must reject or revise any code that fails validation.

