#!/bin/bash

DATE=$(date +%Y%m%d)
XROOTD_SERVER="root://xrootd.cmsaf.mit.edu/"
SOURCE_DIR="/store/user/jdlang/run3_2023Data_Jan2024ReReco"
OUTPUT_DIR="/store/user/jdlang/run3_2023Data_Jan2024ReReco_Skims_20250108"
CONFIG_DIR="condorSkimConfigs_${DATE}"
MASTER_FILE_LIST="${CONFIG_DIR}/forestFilesForSkim.txt"
FILES_PER_JOB=200

# Includes VOMS proxy in process
REFRESH_PROXY=0
# Copies key scripts from MITHIGAnalysis to T2_US_MIT for compiler
COPY_TO_T2=0

if [[ $REFRESH_PROXY -eq 1 ]]; then
  voms-proxy-init -rfc -voms cms -valid 120:00
  cp /tmp/x509up_u'$(id -u)' ~/
  export PROXYFILE=~/x509up_u$(id -u)
fi
if [[ $COPY_TO_T2 -eq 1 ]]; then
  ./CopyToT2.sh
  wait
fi
xrdfs $XROOTD_SERVER mkdir -p $OUTPUT_DIR
mkdir -p $CONFIG_DIR
./MakeXrdFileList.sh $XROOTD_SERVER $SOURCE_DIR $MASTER_FILE_LIST

# Function for job submission
submit_condor_jobs() {
  local BASENAME=$1
  local JOB_LIST=$2
  local JOB_COUNTER=$3
  OUTPUT_PATH="${OUTPUT_DIR}/skim_output_${JOB_COUNTER}.root"
  ./MakeCondorSkim.sh $BASENAME $JOB_LIST $CONFIG_DIR $XROOTD_SERVER $OUTPUT_PATH $PROXYFILE
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
    if [[ JOB_COUNTER -eq 3 ]]; then
      break
    fi
    submit_condor_jobs $BASENAME $JOB_LIST $JOB_COUNTER
    JOB_COUNTER=$((JOB_COUNTER + 1))
    BASENAME="job${JOB_COUNTER}"
    JOB_LIST="${CONFIG_DIR}/${BASENAME}_filelist.txt"
  fi
done < $MASTER_FILE_LIST
# Submit final job list
submit_condor_jobs $BASENAME $JOB_LIST $JOB_COUNTER
