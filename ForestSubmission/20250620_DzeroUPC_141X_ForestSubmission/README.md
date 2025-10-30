# Setup

From lxplus, run the following:

```bash
# Setup CMSSW
cmsrel CMSSW_14_1_9
cd CMSSW_14_1_9/src
cmsenv
git cms-init

# Merge CMSHI forest tools
git cms-merge-topic CmsHI:forest_CMSSW_14_1_X
git remote add cmshi git@github.com:CmsHI/cmssw.git

# Add Dfinder
git clone -b Dfinder_14XX_miniAOD git@github.com:boundino/Bfinder.git
source Bfinder/test/DnBfinder_to_Forest.sh

scram b -j8

cmsenv
```

# Run Forest

```bash
# Init CMSSW
cd CMSSW_14_1_9/src
cmsenv

# Navigate to foresting dir
cd ~/path_to/MITHIGAnalysis2024/ForestSubmission/2025XXXX
```

Modify the `forest_CMSSWConfig` and `forest_CRABConfig` files as needed.
Data should be submitted by PD if possible (NOT by run), and should use
the official golden json for run filtering.

Test that your CMSSW configs work first by replacing the test file with
a file from your target MiniAOD path:

```bash
# Init VOMS proxy
voms-proxy-init -rfc -voms cms

# 
cmsRun
```

When you're ready to submit, do VOMS authentication (if you have not already)
and submit each CRAB config file.

```bash
# Init VOMS proxy
voms-proxy-init -rfc -voms cms

# Submit jobs to CRAB
crab submit -c forest_CRABConfig_XXXX.py 

# Check the status of your jobs
crab status -d CrabWorkArea/crab_XXXX/
```
