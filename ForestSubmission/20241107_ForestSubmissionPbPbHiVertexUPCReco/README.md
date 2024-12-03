# Setup

## 1. Setup CMSSW Environment

Get CMSSW_14_1_4_patch5 environment:
```bash
cmsrel CMSSW_14_1_4_patch5
cd CMSSW_14_1_4_patch5/src
cmsenv
```

Add additional necessary libraries:
```bash
# CMS HI modifications to CMSSW
git cms-merge-topic CmsHI:forest_CMSSW_14_1_X
# Remote repo alias for tracking updates
git remote add cmshi git@github.com:CmsHI/cmssw.git

# D & B finders
git clone -b Dfinder_14XX_miniAOD https://github.com/boundino/Bfinder.git --depth 1
source Bfinder/test/DnBfinder_to_Forest.sh

# Additional production tools
git clone https://github.com/cfmcginn/production
cp production/HIRun2023/forestPPRef/ak* HeavyIonsAnalysis/JetAnalysis/python/
cp production/HIRun2023/forestPPRef/forest_miniAOD_run3_ppRECO_DATA.py HeavyIonsAnalysis/Configuration/test/
rm -rf production
```

Compile:
```bash
scram b -j10
```

> [!NOTE]
> You only have to setup once. After this, navigate to the same
> `CMSSW_14_1_4_patch5/src` directory as above and run `cmsenv`

## 2. Setup MITHIGAnalysis2024 Local Repo
Clone from github (does not have to be in `CMSSW/src`):
```bash
git clone --recursive git@github.com:ginnocen/MITHIGAnalysis2024.git
```

Navigate to the productions directory:
```bash
cd MITHIGAnalysis2024/ForestSubmission/20241107_ForestSubmissionPbPbHiVertexUPCReco
```

Now you are ready to submit forest jobs on CRAB!

&nbsp;
&nbsp;



# Running on LXPlus with CRAB :crab:
Initialize CMSSW and navigate to 
`MITHIGAnalysis2024/ForestSubmission/20241107_ForestSubmissionPbPbHiVertexUPCReco`

Edit the parameters in `crabSubmit.sh` and add/uncomment an appropriate number
of runs to process.

> [!WARNING]
> **For a long list of runs with ~20 PDs each, only process ~5 runs at once!**
> Do not start new jobs until ongoing jobs are complete, 
> otherwise all jobs may get deprioritized.

Initialize VOMS (this will prompt you for your VOMS password):
```bash
voms-proxy-init -voms cms
```

Modify the file list 

Submit jobs to CRAB:
```bash
bash crabSubmit.sh
```

If this is successful, you should see something like:
```bash
Success: Your task has been delivered to the prod CRAB3 server.
Task name: 241201_185358:  crab_Run3UPC2024_388000_HIForward0
Project dir: path/to/workarea/crab_Run3UPC2024_388000_HIForward0
Please use ' crab status -d WorkArea/crab_Run3UPC2024_388000_HIForward0 ' to check how the submission process proceeds.
```

> [!NOTE]
> Use the `kill` command to stop a job that is running!
>
> Example: `crab kill path/to/WorkArea/20241107_ForestSubmissionPbPbHiVertexUPCReco`

> [!NOTE]
> If a job was killed or not submitted correctly, delete the job folder from 
> the work area, otherwise you cannot resubmit it!
>
> Example: `rm -r path/to/WorkArea/crab_Run3UPC2024_388000_HIForward0/`

> [!TIP]
> Due to the storage limits on lxplus, consider setting your CRAB work area
> to a location on `eos`.
>
> Example: `/eos/cms/store/group/phys_heavyions/username/crabWorkArea/`

&nbsp;
&nbsp;



# Running Locally
Initialize CMSSW and navigate to 
`MITHIGAnalysis2024/ForestSubmission/UPCReco_2024PbPbHiVertex`

Run with:
```bash
cmsRun forestCMSSWConfig_miniAOD_Run3UPC.py
```

> [!WARNING]
> This method DOES NOT set the run number or PD! It will process all
> valid runs in the config file source path!

&nbsp;
&nbsp;



# Overview of Files

### forestCMSSWConfig_miniAOD_Run3UPC.py
This defines the foresting procedure: trees to keep, D and B finders,
HLT paths to save.
```python
# THIS IS SET BY submit.sh
runYear=2024
isData=1
isMC = not isData
```
EDIT WITH CAUTION!

### forestCRABConfig_DataTemplate_Run3UPC2024.py
Template for CRAB submissions. Variables beginning with `$` are replaced
automatically by `crabSubmit.sh` (e.g. `$RUN` and `$JOBTAG`). EDIT WITH CAUTION!

### crabSubmit.sh
Iterates through runs and PDs to submit batches of jobs to CRAB. Edit the 
following parameters as needed:
  * `ISDATA`: data(`1`) or MC(`0`).
  * `RUNYEAR`: year data was **collected**: `2023` or `2024`.
  * `RUNLIST`: array of run numbers to process.
Within the RUNLIST/PD loop are additional parameters:
  * `PDNAME`: Name of PD from DAS, PD number is appended (e.g. `"HIForward$PD"`)
  * `JOBTAG`: label for CRAB logs and output directory.
  * `DATASET`: DAS dataset path, can incorporate `PDNAME`.
  * `DATABASE`: Location of the dataset: `'global'` or `'phys03'`.

### crabStatus.sh
Simplifies the process of checking on files, including resubmitting and killing
jobs. Edit parameters to match `crabSubmit.sh`. 

Running without args checks the first run in the list, but
run and PD can be provided to check specific runs.
```bash
# No args
bash crabStatus.sh

# Check a specific run
bash crabStatus.sh 388000

# Check a specific PD of a run
bash crabStatus.sh 388000 12
```



# Useful Links

[(GitHub) Foresting for 2024 HI Run](https://github.com/jusaviin/HiForestSetupPbPbRun2024)

[(GitHub) Dfinder/Bfinder from Jing](https://github.com/boundino/Bfinder)

[(GitHub) CmsHI forest_CMSSW_14_1_X](https://github.com/CmsHI/cmssw/tree/forest_CMSSW_14_1_X/HeavyIonsAnalysis/Configuration/test)
