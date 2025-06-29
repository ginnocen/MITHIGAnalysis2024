# MITHIGAnalysis2024

Analysis repository for 2024 analyses. Skimming code based on the framework developed by Yi Chen (Vanderbilt) https://github.com/FHead/PhysicsZHadronEEC.


## Code-Review Workflow

### Weekly Code-Review Meeting
- **When:** every **Thursday, 09:00 ET** (3pm CERN time). Extra slots can be arranged if needed.  
- **Scope:**
  - Structural changes to skimming code or individual skims  
  - Updates to the reconstruction / foresting / skimming workflow  
  - Major rewrites of analyzers or other highly sensitive modules  
---

### Pull-Request integration: strategies and requirements

#### 1 ▸ PRs that affect *skimmers*
- Add a detailed `README.md` in the PR summarizing **motivation** and **implementation**.  
- Assign one teammate as the primary reviewer.  
- Request approval and merge from one of the code-review responsibles.
- Present a five-minute summary at the next code-review meeting to present the modications.

#### 2 ▸ PRs that affect *main analysis folders*
- Add a detailed `README.md` in the PR summarizing **motivation** and **implementation**.  
- Assign one teammate as the primary reviewer.
- Request approval and merge from one of the code-review responsibles.

#### 3 ▸ PRs in *personal development folders*
- Provide the `README.md` summary.  
- Assign one teammate as the primary reviewer.
- **Merge after that single approval.**
---

### How to Request a Review
1. Push your branch and open a GitHub PR. Label the PR according to the level of review needed (`skimmer`, `core analysis`, or `standalone dev`)
2. Add the required summary document.  
3. Tag the appropriate reviewers (see categories above).  
---
