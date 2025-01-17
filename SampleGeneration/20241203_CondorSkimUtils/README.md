# Outline of README

* First-time Setup
* Recurring Setup
    * 1. Initialize CMSSW environment
    * 2. Source analysis scripts
    * 3. Initialize VOMS proxy
* Skimming with Condor
    * 
* Useful Commands


# First-time Setup

### 0. SubMIT account
You will need an account for 
[MIT SubMIT](https://submit.mit.edu/submit-users-guide/index.html) and a 
[CERN/CMS certificate](https://uscms.org/uscms_at_work/computing/getstarted/get_grid_cert.shtml). 
If you need a SubMIT account you can [request one here](https://submit.mit.edu) 
through the SubMIT Portal. You can only use an ssh key to autheticate!
Passwords do not work.


## 1. Setup CMSSW
Connect to Submit:
```bash
ssh <user>@submit.mit.edu
```

Navigate to your working directory and install the CMSSW version 
_for your analysis_. For example:
```bash
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsrel CMSSW_13_2_4
cd CMSSW_13_2_4/src/
cmsenv
cd -
```
Note, for data processed in 2024 and 2025 you may need to use `CMSSW_14_1_7`.


## 2. Clone and source analysis repository
Clone the MITHIGAnalysis2024 repository to your SubMIT folder:
```bash
git clone --recursive git@github.com:<username>/MITHIGAnalysis2024.git
git remote add upstream git@github.com:ginnocen/MITHIGAnalysis2024.git
```

Source the analysis setup:
```
cd MITHIGAnalysis2024/
source SetupAnalysis.sh
```

Navigate to the forest reducer directory _for your analysis_ and clean the 
directory. For example: 
```bash
cd SampleGeneration/20241214_ForestReducer_DzeroUPC_2023OldReco/
source clean.sh
```


## 4. Add VOMS credentials
You will need to copy your VOMS certificate to SubMIT before you can access
files with xrootd. 
[Follow the instructions linked here for help.](https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookStartingGrid#ObtainingCert)

Once your certificate is setup, initialize a proxy session:
```bash
voms-proxy-init -rfc -voms cms -valid 120:00

# If your proxy file saves to /tmp/ you must move it!
cp /tmp/x509up_u'$(id -u)' ~/
```


# Recurring Setup

## 1. Initialize CMSSW environment
```bash
cd CMSSW_<version>/src/
cmsenv
cd -
```

## 2. Source analysis scripts
```bash
cd MITHIGAnalysis2024/
source SetupAnalysis.sh
cd SampleGeneration/<ForestReducer_dir>/
source clean.sh
```

## 3. Initialize VOMS proxy
```bash
voms-proxy-init -rfc -voms cms -valid 120:00

# If your proxy file saves to /tmp/ you must move it!
cp /tmp/x509up_u'$(id -u)' ~/
```


# Skimming with Condor

## Running

Make/modify `RunCondorSkim.sh` to your 'ForestReducer' directory within 
`SampleGeneration`. Descriptions of parameters can be found by running:
```bash
# If RunCondorSkim.sh is not present:
cp ../20241203_CondorSkimUtils/RunCondorSkim_Template.sh ./RunCondorSkim.sh

# For descriptions of input arguments:
bash $ProjectBase/SampleGeneration/20241203_CondorSkimUtils/InitCondorSkim.sh
```

When ready, run your configuration file:
```bash
bash RunCondorSkim.sh
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


## Important Utility Files

**The following files are used for all condor-based forest reducers**! Do not
change these without validation.


**InitCondorSkim.sh**

Loops over all forest files in the provided directory. Configure the settings
from the `RunCondorSkim.sh` scritps in the 'ForestReducer' directories.
You can also edit this to change the source and output locations from
`T2_US_MIT` to other xrood-accessible locations.


**MakeXrdFileList.sh**

Makes a master list of file paths from any xrootd enable server (such as 
`T2_US_MIT`) that will be filtered and processed in jobs.


**MakeCondorConfigs.sh**

Creates the Condor submission configuration and the bash script that is executed 
on the Condor servers. The job script starts after `cat > $SCRIPT <<EOF1` and 
ends at the line `EOF1`. The Condor config starts after `cat > $CONFIG <<EOF2` 
and ends at `EOF2`.


**MakeAnalysisTar.sh**

Copies essential files from the local `MITHIGAnalysis2024` repo to a compressed
file. This is passed to individual condor jobs so up-to-date scripts are used.


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

To make it easier to initiate proxies, add the following lines to `~/.bashrc`:
```bash
alias proxy='voms-proxy-init -rfc -voms cms -valid 72:00 ; cp /tmp/x509up_u'$(id -u)' ~/ ;'
export PROXYFILE=~/x509up_u$(id -u)
```
Then reset your terminal:
```bash
hash -r
source ~/.bashrc
```
From now on, you can initiate a new proxy with the command `proxy`!
