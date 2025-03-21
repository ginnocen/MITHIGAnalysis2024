#!/bin/bash
# To use:
# bash processAllSamples.sh
# optional flag: -d <config_dir> , uses different config directory from default
# optional flag: -c , runs clean.sh before processing configs

CONFIG_DIR="pt2-5_fitSettings"
JSON_VERSION="_useGammaNForNgammaForFitFunc"
CONFIG_LIST=(
  "fullAnalysis$JSON_VERSION.json"
  "systDalpha$JSON_VERSION.json"
  "systDchi2cl$JSON_VERSION.json"
  "systDsvpv$JSON_VERSION.json"
  "systDtrkPt$JSON_VERSION.json"
  "systFitComb$JSON_VERSION.json"
  "systFitPkBg$JSON_VERSION.json"
  "systFitSigAlpha$JSON_VERSION.json"
  "systFitSigMean$JSON_VERSION.json"
  "systMassWindow$JSON_VERSION.json"
  "systRapGapLoose$JSON_VERSION.json"
  "systRapGapTight$JSON_VERSION.json"
)
DO_CLEAN=0

# Parse args
while [[ $# -gt 0 ]]; do
  case "$1" in
    -d)
      CONFIG_DIR="$2"
      shift 2
      ;;
    -c|--clean)
      DO_CLEAN=1
      shift
      ;;
  esac
done
if [[ "$DO_CLEAN" -eq "1" ]]; then
  source clean.sh
  wait
else
  make
  wait
fi

# Process configs
echo ""
echo "Config directory: $CONFIG_DIR"
echo ""
for CONFIG_JSON in ${CONFIG_LIST[@]}; do
  bash massfit.sh $CONFIG_DIR/$CONFIG_JSON
  wait
done
wait
bash plot.sh "pt2-5_plotSettings/fullAnalysis.json"
root -l -b -q plotCompareDataMCmass.cpp
root -l -b -q plotMassfitSignalStudy.cpp
