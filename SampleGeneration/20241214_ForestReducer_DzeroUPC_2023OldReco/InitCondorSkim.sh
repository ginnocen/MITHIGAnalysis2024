#!/bin/bash

# Source of forest files (use xrootd path)
SOURCE_SERVER="root://xrootd.cmsaf.mit.edu/"
SOURCE_DIR="/store/user/jdlang/run3_2023Data_Jan2024ReReco"
# Output of skimmed files (use xrootd path)
OUTPUT_SERVER="root://xrootd.cmsaf.mit.edu/"
OUTPUT_DIR="/store/user/$USER/run3_2023Data_Jan2024ReReco_Skims_20250113"

# Job settings (memory and storage are in GB)
FILES_PER_JOB=200
JOB_MEMORY=5
JOB_STORAGE=20
CMSSW_VERSION="CMSSW_13_2_4"
# Subdirectory within MITHIGAnalysis2024 with skim scripts
ANALYSIS_SUBDIR='SampleGeneration/20241214_ForestReducer_DzeroUPC_2023OldReco/'

# Local directory for condor configs
DATE=$(date +%Y%m%d)
CONFIG_DIR="condorSkimConfigs_${DATE}_2"
MASTER_FILE_LIST="${CONFIG_DIR}/forestFilesForSkim.txt"
# Include VOMS proxy in process
REFRESH_PROXY=1
# Copy key scripts from MITHIGAnalysis to T2_US_MIT for compiler
COPY_TO_T2=1

$ProjectBase/SampleGeneration/20241203_CondorForestReducer/RunCondorSkim.sh $SOURCE_SERVER $SOURCE_DIR $OUTPUT_SERVER $OUTPUT_DIR $FILES_PER_JOB $JOB_MEMORY $JOB_STORAGE $CMSSW_VERSION $ANALYSIS_SUBDIR $CONFIG_DIR $MASTER_FILE_LIST $REFRESH_PROXY $COPY_TO_T2
