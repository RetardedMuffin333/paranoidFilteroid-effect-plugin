# RESEARCH_GUIDE.md  
**Guidelines for Researching DSP, Audio Plugins, and JUCE**

This document instructs Claude (AI copilot) how to **perform research**, **avoid hallucinations**, and **produce verifiable written research reports** for this project.

---

# 1. General Research Philosophy

Claude must:
- Prefer **verified**, **well-known**, and **well-documented** information.
- Prioritize **primary sources**:
  - Official JUCE documentation, examples, tutorials.
  - Official VST3 documentation.
  - Academic DSP resources (e.g., Julius O. Smith / CCRMA, standard DSP textbooks).
  - Established audio engineering practices recognized by industry professionals.
- Avoid **blogs**, **forums**, or **random snippets** unless:
  - They corroborate well-known practices AND
  - They match existing documented DSP knowledge.

Claude must **never** generate novel DSP theory unless explicitly requested and must **label it as speculative**.

---

# 2. Allowed Reference Categories

Claude may rely on:

## 2.1 Trusted Primary DSP Sources
- Julius O. Smith / Stanford CCRMA material  
- The Audio EQ Cookbook by Robert Bristow-Johnson  
- Textbooks like:
  - *Digital Signal Processing* (Oppenheim & Schafer)  
  - *DSP Guide* (Steven W. Smith)

## 2.2 Trusted Practical Audio Engineering Sources
- Reputable mixing textbooks (e.g., Bobby Owsinski)  
- Documented techniques used in:
  - Telephone / radio effects  
  - Microphone EQ shaping  
  - Broadcast processing

## 2.3 Trusted Framework Documentation
- JUCE official documentation  
- JUCE examples + tutorials  
- Steinberg VST3 documentation

## 2.4 Validation Through Cross-Checking
Claude must **validate every technical claim** by cross-referencing:
- A primary DSP source **OR**
- Multiple independent secondary sources with consistent information **OR**
- Known engineering practice + DSP stability principles.

If a claim cannot be verified → Claude must **flag it instead of generating assumptions**.

---

# 3. Research Workflow Claude Must Follow

## Step 1 — Identify the Topic  
Break the question into atomic, researchable pieces.

## Step 2 — Gather Known Verified Info  
Claude should:
- Recall verified DSP fundamentals.
- Pull from JUCE / VST documentation.
- Use recognized audio engineering practices.

## Step 3 — Detect Risk of Hallucination  
Claude must pause and check:
- “Is this an established method?”  
- “Do I know this from official documentation?”  
- “Am I inventing something?”

If uncertain:
- Claude must say: **“This part requires verification; here are options…”**
- Claude must present *existing known solutions only*.

## Step 4 — Produce a Mini-Report  
When the user asks:
- Summarize findings in a **clean, structured markdown research report**, with sections like:
  - Overview
  - Verified Facts
  - Industry Practices
  - Implementation Options (all verified)
  - Open Questions / Things needing user decision

## Step 5 — Provide Explicit Citations When Possible  
Claude must mention:
- “Source: JUCE documentation”
- “Source: CCRMA notes – filter design”
- “Source: Telephony standards (300–3400 Hz)”

Even if not giving exact URLs, Claude must label source type.

---

# 4. Strict “No-Hallucination” Rules

Claude must **NOT**:
- Invent APIs that don’t exist.
- Invent DSP functions not found in standard literature.
- Provide code that relies on unknown libraries.
- Describe “advanced DSP algorithms” without grounding in known publications.
- Recommend impossible or unstable filter configurations.

If user asks something impossible or unclear:
- Claude must respond with a clarification request OR present the safe, verifiable subset of the possible solution.

---

# 5. Required Output Format for Research Deliverables

At the end of the project research, Claude must generate research report:

Research Report:
1. Summary
- brief overview
2. Verified Information
- facts with sources
3. Relevant Industry Practices
- common techniques with justification
4. Implementation Considerations
- how this affects the plugin
5. Open Questions / Decisions for User