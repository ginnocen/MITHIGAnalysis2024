#!/bin/bash

# Usage and internal variables
usage="
Numbered arguments for RunCondorSkim.sh:
    1 ) SOURCE_SERVER: xrootd server address for source/input files.
    2 ) SOURCE_DIR: Parent directory on SOURCE_SERVER containing forest files.
        This is recursively searched, so files within subfolders are fine.
    3 ) OUTPUT_SERVER: xrootd server address for output files.
    4 ) OUTPUT_DIR: Output directory for skimmed files on OUTPUT_SERVER.
    5 ) FILES_PER_JOB: Number of forest files to process sequentially per job.
    6 ) JOB_MEMORY: Requested memory per job in GB.
    7 ) JOB_STORAGE: Requested storage space per job in GB.
    8 ) CMSSW_VERSION: Version name, same as for cmsrel command.
    9 ) ANALYSIS_DIR: Path on T2_US_MIT to copy of MITHIGAnalysis2024 repo,
        containing only key files for compilers.
    10) ANALYSIS_SUBDIR: Subdirectory within ANALYSIS_DIR containing
        the specific ForestReducer.cpp script to use.
    11) CONFIG_DIR: local directory where condor configs will be saved.
    12) MASTER_FILE_LIST: text file that will contain list of all 
        root files in SOURCE_DIR.
    13) REFRESH_PROXY: (0 or 1) Initiate a new VOMS proxy before processing.
    14) COPY_TO_T2: (0 or 1) Copy key files in MITHIGAnalysis2024 to T2_US_MIT. 
        This is necessary for jobs to compile ForestReducer.cpp.

Run using an external script to configure arguments, such as InitCondorSkim.sh:
    bash InitCondorSkim.sh
"
condor_skim_dir="SampleGeneration/20241203_CondorForestReducer"
n_args=14

# Set input variables
SOURCE_SERVER=${1}
SOURCE_DIR=${2}
OUTPUT_SERVER=${3}
OUTPUT_DIR=${4}
FILES_PER_JOB=${5}
JOB_MEMORY=${6}
JOB_STORAGE=${7}
CMSSW_VERSION=${8}
ANALYSIS_DIR=${9}
ANALYSIS_SUBDIR=${10}
CONFIG_DIR=${11}
MASTER_FILE_LIST=${12}
REFRESH_PROXY=${13}
COPY_TO_T2=${14}

if [ $# -ne $n_args  ]; then
  echo "Insufficient number of arguments given!"
  echo "$usage"
  exit 1
fi
if [[ $REFRESH_PROXY -eq 1 ]]; then
  voms-proxy-init -rfc -voms cms -valid 120:00
  cp /tmp/x509up_u'$(id -u)' ~/
  export PROXYFILE=~/x509up_u$(id -u)
fi
if [[ $COPY_TO_T2 -eq 1 ]]; then
  $ProjectBase/$condor_skim_dir/CopyToT2.sh $ANALYSIS_DIR $ANALYSIS_SUBDIR
  wait
fi
xrdfs $OUTPUT_SERVER mkdir -p $OUTPUT_DIR
mkdir -p $CONFIG_DIR
$ProjectBase/$condor_skim_dir/MakeXrdFileList.sh $SOURCE_SERVER $SOURCE_DIR $MASTER_FILE_LIST

# Function for job submission
submit_condor_jobs() {
  local JOB_NAME=${1}
  local JOB_LIST=${2}
  local JOB_COUNTER=${3}
  OUTPUT_PATH="${OUTPUT_DIR}/skim_output_${JOB_COUNTER}.root"
  $ProjectBase/$condor_skim_dir/MakeCondorSkim.sh $JOB_NAME $JOB_LIST $CONFIG_DIR $OUTPUT_SERVER $OUTPUT_PATH $PROXYFILE $JOB_MEMORY $JOB_STORAGE $CMSSW_VERSION $ANALYSIS_DIR $ANALYSIS_SUBDIR
  wait
  sleep 0.5
  return 0
}

# Split list into jobs
FILE_COUNTER=0
JOB_COUNTER=1
JOB_NAME="job${JOB_COUNTER}"
JOB_LIST="${CONFIG_DIR}/${JOB_NAME}_filelist.txt"
rm $JOB_LIST
while IFS= read -r LINE; do
  echo "$LINE" >> "$JOB_LIST"
  FILE_COUNTER=$((FILE_COUNTER + 1))
  if (( $FILE_COUNTER % $FILES_PER_JOB == 0 )); then
    submit_condor_jobs $JOB_NAME $JOB_LIST $JOB_COUNTER
    JOB_COUNTER=$((JOB_COUNTER + 1))
    JOB_NAME="job${JOB_COUNTER}"
    JOB_LIST="${CONFIG_DIR}/${JOB_NAME}_filelist.txt"
  fi
done < $MASTER_FILE_LIST
# Submit final job list
submit_condor_jobs $JOB_NAME $JOB_LIST $JOB_COUNTER