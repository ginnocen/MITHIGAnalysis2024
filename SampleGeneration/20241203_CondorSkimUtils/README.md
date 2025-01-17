# First-time Setup

You will need an account for 
[MIT SubMIT](https://submit.mit.edu/submit-users-guide/index.html) and a 
[CERN/CMS certificate](https://uscms.org/uscms_at_work/computing/getstarted/get_grid_cert.shtml). 
If you need a SubMIT account you can [request one here](https://submit.mit.edu) 
through the SubMIT Portal. You can only use an ssh key to autheticate!
Passwords do not work.

Connect to Submit:
```bash
ssh <user>@submit.mit.edu
```

Navigate to your working directory and install CMSSW.

For data processed in 2023 and earlier:
```bash
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsrel CMSSW_13_2_4
cd CMSSW_13_2_4/src/
cmsenv
cd -
```

For data processed in 2024 and 2025:
```bash
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsrel CMSSW_14_1_7
cd CMSSW_14_1_7/src/
cmsenv
cd -
```

Clone the MITHIGAnalysis2024 repository:
```bash
git clone --recursive git@github.com:ginnocen/MITHIGAnalysis2024.git
cd MITHIGAnalysis2024/
source SetupAnalysis.sh
```

Navigate to the forest reducer directory for your analysis, for example: 
```bash
cd SampleGeneration/20241214_ForestReducer_DzeroUPC_2023OldReco/
source clean.sh
```

You should be ready. If you make changes to `ReduceForest.cpp`, `makefile`, or 
change anything under the `include/` folder or `MITHIGAnalysis2024/CommonCode/`,
then you will need to update the slimmed repo files on `T2_US_MIT`. Add the 
paths to `CopyToT2.sh` (**do not delete paths from this file!**), located here:
```
MITHIGAnalysis2024/SampleGeneration/20241203_CondorForestReducer/CopyToT2.sh
```

You can then run this script on its own:
```bash
bash CopyToT2.sh
```
Or set the following variable in your `InitCondorSkim.sh` script to `1` :
```bash
# Copy key scripts from MITHIGAnalysis to T2_US_MIT for compiler
COPY_TO_T2=1
```

# Recurring Setup

## 1. Initialize CMSSW environment
```bash
cd CMSSW_X_Y_Z/src/
cmsenv
cd -
```

## 2. Source analysis scripts
```bash
cd MITHIGAnalysis2024/
source SetupAnalysis.sh
cd SampleGeneration/20YYMMDD_ForestReducer/
source clean.sh
```

## 3. Initialize VOMS credentials
```bash
voms-proxy-init -rfc -voms cms -valid 120:00

# If your proxy file saves to /tmp/ you must move it!
cp /tmp/x509up_u'$(id -u)' ~/
```

# Skimming with Condor

## Running

**Easy Process**

Copy `InitCondorSkim.sh` to your directory within `SampleGeneration`. Adjust
the parameters as needed. Descriptions of parameters can be found by running:
```bash
bash $ProjectBase/SampleGeneration/20241203_CondorForestReducer/RunCondorSkim.sh
```

Two parameters that **do not** need to be used every time are below:
```bash
# Include VOMS proxy in process
REFRESH_PROXY=0
# Copy key scripts from MITHIGAnalysis to T2_US_MIT for compiler
COPY_TO_T2=1
```
The first prompts renewing your VOMS certificate, while the second copies 
changes to the local repo to a folder on T2_US_MIT. If you
are starting multiple sets of jobs in a short time or without changing other 
files, you can safely set these to `0`.

**When ready, run your configuration file:**
```bash
bash InitCondorSkim.sh
```

Check the status of condor jobs with:
```bash
condor_q
```

Kill or remove bad jobs with:
```bash
condor_rm <job_id>
```

Once jobs are complete (whether successful or failed), they will no longer
appear on the `condor_q` list.

Log files are saved to `condorConfigs_<YYYYMMDD>/`:
```bash
# log of job and server status
jobX_log_<job_id>.txt
# log of errors printed by the job scripts (some outputs will print here too)
jobX_err_<job_id>.txt
# log of print statements from job scripts
jobX_out_<job_id>.txt
```


## Important Central Files

**The following files are used for all condor-based forest reducers**! Do not
change these without validation.


**RunCondorSkim.sh**

Loops over all forest files in the provided directory. Configure the settings
from `InitCondorSkim.sh`,You can also edit this to change the source and output 
locations on `T2_US_MIT`.


**MakeXrdFileList.sh**

Makes a master list of file paths from any xrootd enable server (such as 
`T2_US_MIT`) that will be filtered and processed in jobs.


**MakeCondorSkim.sh**

Creates the Condor submission file and the bash script that is executed on
the Condor servers. The job script starts after `cat > $SCRIPT <<EOF1` and ends
at the line `EOF1`. The Condor config starts after `cat > $CONFIG <<EOF2` and
ends at `EOF2`.


**CopyToT2.sh**

Copies essential files from the local `MITHIGAnalysis2024` repo to `T2_US_MIT`
so they can be copied to every server at the start of a new job. This should
only be needed if `ReduceForest.cpp`, `makefile`, `include/` or 
`MITHIGAnalysis2024/CommonCode/` are changed.


# Useful Commands

## Condor

```bash
condor_submit <condor_config_file>
condor_q
condor_rm <job_id>
```


## xrootd

**CERN eos:** `root://eoscms.cern.ch/` `/store/group/phys_heavyions/<user>/`
**MIT T2:** `root://xrootd.cmsaf.mit.edu/` `/store/user/<user>/`

```bash
# Recursive list
xrdfs <server> ls -R -l <path>
# Make directory
xrdfs <server> mkdir -p <path/new_dir>

# Copy
xrdcp <server//path/new_dir> <local/path>
# Recursive copy for directory
xrdcp -r <server//path/dir> <local/path>
# Forced copy (overwrite)
xrdcp -f <server//path/file> <local/path>

# Delete file
xrdfs <server> rm <path/file.ext>
# Delete directory
xrdfs <server> rmdir <path/dir>
```


## VOMS

```bash
voms-proxy-init --rfc --voms cms -valid 72:00
voms-proxy-info
```

To make it easier to initiate proxies, add this to `~/.bashrc`:
```bash
alias proxy='voms-proxy-init -rfc -voms cms -valid 72:00 ; cp /tmp/x509up_u'$(id -u)' ~/ ;'
export PROXYFILE=~/x509up_u$(id -u)
```
Reset your terminal:
```bash
hash -r
source ~/.bashrc
```
From now on, you can initiate a new proxy with the command `proxy`!
