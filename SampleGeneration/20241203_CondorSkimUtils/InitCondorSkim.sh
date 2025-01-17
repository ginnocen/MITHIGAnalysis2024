#!/bin/bash
# InitCondorSkim.sh
# Author: Jordan Lang (jdlang)
#
# DO NOT RUN THIS DIRECTLY! This script is intended to be called by a
# RunCondorSkim.sh file in a separate folder. To skim files, navigate to the
# subdirectory of MITHIGAnalysis2024/SampleGeneration/ that has the version of
# ReduceForest.cpp you want to use. Then run:
#     bash RunCondorSkim.sh

# Usage and internal variables
usage="
Positional arguments (RunCondorSkim.sh -> InitCondorSkim.sh):
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
        the specific ReduceForest.cpp script to use.
    11) CONFIG_DIR: local directory where condor configs will be saved.
    12) MASTER_FILE_LIST: text file that will contain list of all 
        root files in SOURCE_DIR.
    13) REFRESH_PROXY: (0 or 1) Initiate a new VOMS proxy before processing.
    14) COPY_TO_T2: (0 or 1) Copy key files in MITHIGAnalysis2024 to T2_US_MIT. 
        This is necessary for jobs to compile ForestReducer.cpp.
    15) MAX_JOBS: (optional) Limit total number of submitted jobs. 
        Intended for testing/debugging.

Run using an external script to configure arguments, such as InitCondorSkim.sh:
    bash InitCondorSkim.sh
"
condor_skim_dir="SampleGeneration/20241203_CondorForestReducer"
n_args=15

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
MAX_JOBS=${15:-0}

if [ $# -ne $n_args || $# -ne $((n_args - 1)) ]; then
  echo "Insufficient number of arguments given!"
  echo "$usage"
  exit 1
else
  echo ">>> Starting RunCondorSkim.sh with the following arguments:"
  echo "SOURCE_SERVER:    $SOURCE_SERVER"
  echo "SOURCE_DIR:       $SOURCE_DIR"
  echo "OUTPUT_SERVER:    $OUTPUT_SERVER"
  echo "OUTPUT_DIR:       $OUTPUT_DIR"
  echo "FILES_PER_JOB:    $FILES_PER_JOB"
  echo "JOB_MEMORY:       $JOB_MEMORY"
  echo "JOB_STORAGE:      $JOB_STORAGE"
  echo "CMSSW_VERSION:    $CMSSW_VERSION"
  echo "ANALYSIS_DIR:     $ANALYSIS_DIR"
  echo "ANALYSIS_SUBDIR:  $ANALYSIS_SUBDIR"
  echo "CONFIG_DIR:       $CONFIG_DIR"
  echo "MASTER_FILE_LIST: $MASTER_FILE_LIST"
  echo "REFRESH_PROXY:    $REFRESH_PROXY"
  echo "COPY_TO_T2:       $COPY_TO_T2"
  if [ $MAX_JOBS -ne 0 ]; then
    echo "MAX_JOBS:         $MAX_JOBS"
  fi
fi

if [[ $REFRESH_PROXY -eq 1 ]]; then
  echo ">>> Refresh VOMS proxy"
  voms-proxy-init -rfc -voms cms -valid 120:00
  cp /tmp/x509up_u'$(id -u)' ~/
  export PROXYFILE=~/x509up_u$(id -u)
fi
#if [[ $COPY_TO_T2 -eq 1 ]]; then
#  echo ">>> Copying local MITHIGAnalysis2024 files to T2_US_MIT"
#  $ProjectBase/$condor_skim_dir/CopyToT2.sh $ANALYSIS_DIR $ANALYSIS_SUBDIR
#  wait
#fi

tar -cvf MITHIGAnalysis2024.tar -T ../condor_skim_dir/AnalysisCompressionList.txt

echo ">>> Setting up local workspace"
xrdfs $OUTPUT_SERVER mkdir -p $OUTPUT_DIR
mkdir -p $CONFIG_DIR
echo ">>> Making master file list $MASTER_FILE_LIST"
$ProjectBase/$condor_skim_dir/MakeXrdFileList.sh $SOURCE_SERVER $SOURCE_DIR $MASTER_FILE_LIST

# Function for job submission
submit_condor_jobs() {
  local JOB_NAME=${1}
  local JOB_LIST=${2}
  local JOB_COUNTER=${3}
  echo "Making configs for $JOB_NAME"
  OUTPUT_PATH="${OUTPUT_DIR}/skim_${JOB_COUNTER}.root"
  $ProjectBase/$condor_skim_dir/MakeCondorConfigs.sh $JOB_NAME $JOB_LIST $CONFIG_DIR $OUTPUT_SERVER $OUTPUT_PATH $PROXYFILE $JOB_MEMORY $JOB_STORAGE $CMSSW_VERSION $ANALYSIS_DIR $ANALYSIS_SUBDIR
  wait
  echo "Submitted $JOB_NAME"
  echo ""
  sleep 0.5
  return 0
}

# Split list into jobs
FILE_COUNTER=0
JOB_COUNTER=1
JOB_NAME="job${JOB_COUNTER}"
JOB_LIST="${CONFIG_DIR}/${JOB_NAME}_filelist.txt"
rm $JOB_LIST
echo ">>> Creating jobs"
echo ""
while IFS= read -r LINE; do
  echo "$LINE" >> "$JOB_LIST"
  ((FILE_COUNTER++))
  if (( $FILE_COUNTER % $FILES_PER_JOB == 0 )); then
    if (( $JOB_COUNTER == $MAX_JOBS )); then
      break
    fi
    submit_condor_jobs $JOB_NAME $JOB_LIST $JOB_COUNTER
    ((JOB_COUNTER++))
    JOB_NAME="job${JOB_COUNTER}"
    JOB_LIST="${CONFIG_DIR}/${JOB_NAME}_filelist.txt"
  fi
done < $MASTER_FILE_LIST
# Submit final job list
submit_condor_jobs $JOB_NAME $JOB_LIST $JOB_COUNTER

echo ">>> Done with RunCondorSkim.sh!"
