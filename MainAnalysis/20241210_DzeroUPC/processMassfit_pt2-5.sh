#!/bin/bash

CONFIG_LIST=(
  "pt2-5_fitSettings/fullAnalysis_useGammaNForNgammaForFitFunc.json"
  "pt2-5_fitSettings/systDalpha_useGammaNForNgammaForFitFunc.json"
  "pt2-5_fitSettings/systDchi2cl_useGammaNForNgammaForFitFunc.json"
  "pt2-5_fitSettings/systDsvpv_useGammaNForNgammaForFitFunc.json"
  "pt2-5_fitSettings/systDtrkPt_useGammaNForNgammaForFitFunc.json"
  "pt2-5_fitSettings/systFitComb_useGammaNForNgammaForFitFunc.json"
  "pt2-5_fitSettings/systFitPkBg_useGammaNForNgammaForFitFunc.json"
  "pt2-5_fitSettings/systFitSigAlpha_useGammaNForNgammaForFitFunc.json"
  "pt2-5_fitSettings/systFitSigMean_useGammaNForNgammaForFitFunc.json"
  "pt2-5_fitSettings/systRapGapLoose_useGammaNForNgammaForFitFunc.json"
  "pt2-5_fitSettings/systRapGapTight_useGammaNForNgammaForFitFunc.json"
)

if [[ "$1" -eq "1" ]]; then
    source clean.sh
    wait
fi

for CONFIG in ${CONFIG_LIST[@]}; do
  bash massfit.sh $CONFIG
  wait
done
wait

bash plot.sh "pt2-5_plotSettings/fullAnalysis.json"
wait
