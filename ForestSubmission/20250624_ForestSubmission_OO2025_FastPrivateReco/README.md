# Oxygen-Oxygen Foresting

## Contents
* 1) Setup
* 2) Processing Forests
* 3) Terms and References

## 1) Setup

### 1.1) Install CMSSW
```bash
cmsrel CMSSW_15_0_9
cd CMSSW_15_0_9/src
cmsenv
```

[!IMPORTANT]
> To use CRAB for foresting, you will need to be setup your environment 
> on **lxplus9**.
> ```bash
> ssh <your_cern_id>@lxplus9.cern.ch
> ```

### 1.2) Add CMS Heavy Ion foresting tools
```bash
git cms-merge-topic CmsHI:forest_CMSSW_15_0_X
scram build -j4
```

> [!TIP] 
> You can add CMSHI as a remote git reference in case of updates:
> ```bash
> git remote add cmshi git@github.com:CmsHI/cmssw.git
> ```

### 1.3) Download ZDC emap
```bash
wget https://github.com/hjbossi/ZDCOnlineMonitoring/blob/main/Conditions/emap/emap_2025_full.txt
```

### 1.4) Clone MITHIGAnalysis2024
Navigate back to the folder with CMSSW:
```bash
cd ../../
```

Clone with a different name to keep repositories separate:
```bash
git clone git@github.com:ginnocen/MITHIGAnalysis2024.git MITHIGAnalysisOO
cd MITHIGAnalysisOO/
```

Switch to the OO analysis branch:
```bash
git fetch
git switch mainOO
```

Navigate to the forest folder:
```bash
cd /ForestSubmission/20250624_ForestSubmission_OO2025_FastPrivateReco/
```

## 2) Processing Forests

### 2.1) Prepare your directory
```bash
source clean.sh
```

### 2.2) Initialize VOMS proxy
```bash
voms-proxy-init -rfc -voms cms
```
> [!TIP] 
> Add an alias for this to `~/.bash_profile` to make VOMS easier:
> ```bash
> alias proxy='voms-proxy-init -rfc -voms cms; cp/tmp/x509up_u'$(id -u)' ~/'
> ```
> This will let you initialize VOMS just by running the command: `proxy`


# 3) Terms and References

## CMSSW

## CRAB

### Helpful Commands

```bash
# Submit job:
crab submit -c <CRAB_config_file.py>

# Check job status:
crab status -d <path/to/crab_status_directory/>

# Kill a job (WARNING: this is irreversible!):
crab kill -d <path/to/crab_status_directory/>

# Resubmit failed jobs:
crab resubmit -d <path/to/crab_status_directory/>
# Resubmit with max memory and max runtime
crab resubmit --maxmemory 3000 --maxruntime 450 -d <path/to/crab_status_directory/>
```
> [!WARNING]
> Requesting more than the maximum allowed memory or runtime will result in
> your job being refused and **you will be unable to resubmit __any__ jobs after
> that!** 
> * `maxmemory` **cannot exceed 5000** (MB)
> * `maxruntime` **cannot exceed 900** (minutes)
