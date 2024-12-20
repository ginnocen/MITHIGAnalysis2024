#!/bin/bash
PARAM_FILE="$1"
echo $PARAM_FILE

source clean.sh
INPUT="/data/NewSkims_gtoccbar/20241113_DataPbPb2018gtoccbar_v2_TestV0.root"


tail -n +2 "$PARAM_FILE" | while read -r line; do
    set -- $line
    
    # Extract the filename and parameters
    OUTFILE=$1
    shift
    PARAMS="$@"
    
    echo $OUTFILE
    echo $PARAMS
    # Build the command to run with the appropriate parameters
    ./ExecuteMuMuJet_cuts --Input $INPUT --MinJetPT 80 --Output $OUTFILE $PARAMS
    wait
done
