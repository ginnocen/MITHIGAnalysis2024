#!/bin/bash

make
wait

CONFIG_LIST=(
  "pt2-5_sampleSettings/fullAnalysis_skimV4.json"
  "pt2-5_sampleSettings/systDalpha_skimV4.json"
  "pt2-5_sampleSettings/systDchi2cl_skimV4.json"
  "pt2-5_sampleSettings/systDsvpv_skimV4.json"
  "pt2-5_sampleSettings/systDtrkPt_skimV4.json"
  "pt2-5_sampleSettings/systRapGapLoose_skimV4.json"
  "pt2-5_sampleSettings/systRapGapTight_skimV4.json"
)

if [[ "$1" -eq "1" ]]; then
    source clean.sh
    wait
fi

for CONFIG in ${CONFIG_LIST[@]}; do
    bash makeMicroTree.sh $CONFIG
    wait
done
wait
