# Setup

From lxplus8, run the following:

```bash
# Setup CMSSW
cmsrel CMSSW_13_2_13
cd CMSSW_13_2_13/src
cmsenv
git cms-init

# Merge CMSHI forest tools
git cms-merge-topic CmsHI:forest_CMSSW_13_2_X
git remote add cmshi git@github.com:CmsHI/cmssw.git

# Add jet analysis mods
git clone https://github.com/cfmcginn/production
cp production/HIRun2023/forestPPRef/ak* HeavyIonsAnalysis/JetAnalysis/python/
rm -rf production

# Add Dfinder with fixes and D-mass window of +/- 0.4
git clone -b Dfinder_13XX_miniAOD_TzuAnFix git@github.com:jdlang/Bfinder.git
source Bfinder/test/DnBfinder_to_Forest_132X_miniAOD.sh

scram b -j8

cmsenv
```

# Run Forest

```bash
# Init CMSSW
cd CMSSW_13_2_13/src
cmsenv

# Navigate to foresting dir
cd ~/path_to/MITHIGAnalysis2024/ForestSubmission/20250207_

# Uncomment the runs to forest.
# Process 5-10 runs simultaneously to prevent throttling.
vim crabSubmit.sh

# Update valid runs for status checks.
vim crabStatus.sh

# Init VOMS proxy
voms-proxy-init -rfc -voms cms

# Submit runs
bash crabSubmit.sh

# Check on status of runs
bash crabStatus.sh <opt:run_number> <opt:PD_number>
```
