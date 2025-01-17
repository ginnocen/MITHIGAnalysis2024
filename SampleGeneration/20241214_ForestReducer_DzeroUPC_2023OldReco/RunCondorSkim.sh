#!/bin/bash
DATE=$(date +%Y%m%d)

# Source of forest files (use xrootd path)
SOURCE_SERVER="root://xrootd.cmsaf.mit.edu/"
SOURCE_DIR="/store/user/jdlang/run3_2023Data_Jan2024ReReco"
# Output of skimmed files (use xrootd path)
OUTPUT_SERVER="root://xrootd.cmsaf.mit.edu/"
OUTPUT_DIR="/store/user/$USER/run3_2023Data_Jan2024ReReco_Skims_20250115"

# Job settings (memory and storage are in GB)
FILES_PER_JOB=200
JOB_MEMORY=5
JOB_STORAGE=20
CMSSW_VERSION="CMSSW_13_2_4"

# Directory on T2 where key files for compilers are saved
ANALYSIS_DIR="/store/user/$USER/MITHIGAnalysis2024_13_2_4"
# Subdirectory of MITHIGAnalysis2024 repo containing ReduceForest.cpp script
ANALYSIS_SUBDIR="SampleGeneration/$(basename $PWD)"

# Local directory for condor configs
CONFIG_DIR="condorSkimConfigs_${DATE}"
MASTER_FILE_LIST="${CONFIG_DIR}/forestFilesForSkim.txt"
# Include VOMS proxy in process
REFRESH_PROXY=1
# Copy key scripts from MITHIGAnalysis to T2_US_MIT for compiler
COPY_TO_T2=1

# For testing/debugging (set to 0 to run all):
MAX_JOBS=0

$ProjectBase/SampleGeneration/20241203_CondorSkimUtils/InitCondorSkim.sh $SOURCE_SERVER $SOURCE_DIR $OUTPUT_SERVER $OUTPUT_DIR $FILES_PER_JOB $JOB_MEMORY $JOB_STORAGE $CMSSW_VERSION $ANALYSIS_DIR $ANALYSIS_SUBDIR $CONFIG_DIR $MASTER_FILE_LIST $REFRESH_PROXY $COPY_TO_T2 $MAX_JOBS
