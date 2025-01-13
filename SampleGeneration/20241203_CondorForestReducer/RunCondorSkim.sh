#!/bin/bash

# Source of forest files (use xrootd path)
SOURCE_SERVER=${1}
SOURCE_DIR=${2}
# Output of skimmed files (use xrootd path)
OUTPUT_SERVER=${3}
OUTPUT_DIR=${4}

# Job settings (memory and storage are in GB)
FILES_PER_JOB=${5}
JOB_MEMORY=${6}
JOB_STORAGE=${7}
CMSSW_VERSION=${8}
# Subdirectory within MITHIGAnalysis2024 with skim scripts
ANALYSIS_SUBDIR=${9}

# Local directory for condor configs
CONFIG_DIR=${10}
MASTER_FILE_LIST=${11}
# Include VOMS proxy in process
REFRESH_PROXY=${12}
# Copy key scripts from MITHIGAnalysis to T2_US_MIT for compiler
COPY_TO_T2=${13}

if [[ $REFRESH_PROXY -eq 1 ]]; then
  voms-proxy-init -rfc -voms cms -valid 120:00
  cp /tmp/x509up_u'$(id -u)' ~/
  export PROXYFILE=~/x509up_u$(id -u)
fi
if [[ $COPY_TO_T2 -eq 1 ]]; then
  $ProjectBase/SampleGeneration/20241203_CondorForestReducer/CopyToT2.sh
  wait
fi
xrdfs $OUTPUT_SERVER mkdir -p $OUTPUT_DIR
mkdir -p $CONFIG_DIR
$ProjectBase/SampleGeneration/20241203_CondorForestReducer/MakeXrdFileList.sh $SOURCE_SERVER $SOURCE_DIR $MASTER_FILE_LIST

# Function for job submission
submit_condor_jobs() {
  local BASENAME=${1}
  local JOB_LIST=${2}
  local JOB_COUNTER=${3}
  OUTPUT_PATH="${OUTPUT_DIR}/skim_output_${JOB_COUNTER}.root"
  $ProjectBase/SampleGeneration/20241203_CondorForestReducer/MakeCondorSkim.sh $BASENAME $JOB_LIST $CONFIG_DIR $OUTPUT_SERVER $OUTPUT_PATH $PROXYFILE $JOB_MEMORY $JOB_STORAGE $CMSSW_VERSION $ANALYSIS_SUBDIR
  wait
  sleep 0.5
  return 0
}

# Split list into jobs
FILE_COUNTER=0
JOB_COUNTER=1
BASENAME="job${JOB_COUNTER}"
JOB_LIST="${CONFIG_DIR}/${BASENAME}_filelist.txt"
rm $JOB_LIST
while IFS= read -r LINE; do
  echo "$LINE" >> "$JOB_LIST"
  FILE_COUNTER=$((FILE_COUNTER + 1))
  if (( $FILE_COUNTER % $FILES_PER_JOB == 0 )); then
    submit_condor_jobs $BASENAME $JOB_LIST $JOB_COUNTER
    JOB_COUNTER=$((JOB_COUNTER + 1))
    BASENAME="job${JOB_COUNTER}"
    JOB_LIST="${CONFIG_DIR}/${BASENAME}_filelist.txt"
  fi
done < $MASTER_FILE_LIST
# Submit final job list
submit_condor_jobs $BASENAME $JOB_LIST $JOB_COUNTER
