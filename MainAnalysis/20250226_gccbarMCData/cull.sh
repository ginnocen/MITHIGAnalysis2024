#!/bin/bash
PARAM_FILE="$1"
echo $PARAM_FILE

source clean.sh

#INPUT="/data00/g2ccbar/mc2018/skim_02052025/mergedfile_02032025_30cores.root"
INPUT="/data00/g2ccbar/data2018/minBias_skims_02052025/mergedfileData_02042025_mbAll.root"
#INPUT="/data00/g2ccbar/mc2018/skimTest/mergedfile_02102025.root"
# Extract the filename and parameters
OUTFILE=$1    
echo $OUTFILE
    
# Build the command to run with the appropriate parameters
./ExecuteMuMuJet_culler --Input $INPUT --Output $OUTFILE 
wait
