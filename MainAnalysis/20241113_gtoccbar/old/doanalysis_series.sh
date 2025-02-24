#!/bin/bash
PARAM_FILE="$1"
echo $PARAM_FILE

source clean.sh
#INPUT="/data00/g2ccbar/mc2018/SkimMC2018PbPb_Version20241220_InputForest_20241217_DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8/mergedfile.root"
#INPUT="/data00/g2ccbar/mc2018/SkimMC2018PbPb_Version20241220_InputForest_20241217_DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8/SkimMCFiltered_OnlyEventswithMuMuTaggedJets.root"        
#"/data/NewSkims_gtoccbar/SkimMC2018PbPb_Version20241201_InputForest_20241201_DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8_PARTIAL.root"
#INPUT="/data00/g2ccbar/mc2018/SkimMC2018PbPb_Version20241220_InputForest_20241217_DiJet_pThat-15_TuneCP5_HydjetDrumMB_5p02TeV_Pythia8_FIXEDDR/SkimMCFiltered_OnlyEventswithMuMuTaggedJets.root"

INPUT="/data00/g2ccbar/mc2018/skim01172025/mergedfile_01172025_v2.root"

tail -n +2 "$PARAM_FILE" | while read -r line; do
    set -- $line
    
    # Extract the filename and parameters
    OUTFILE=$1
    shift
    PARAMS="$@"
    
    echo $OUTFILE
    echo $PARAMS
    # Build the command to run with the appropriate parameters
    ./ExecuteMuMuJet_cuts --Input $INPUT --MinJetPT 40  --Output $OUTFILE $PARAMS
    wait
done
