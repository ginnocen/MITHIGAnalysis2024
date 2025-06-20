#!/bin/bash
# To use:
# bash processAllSamples.sh
# optional flag: -d <config_dir> , uses different config directory from default
# optional flag: -c , runs clean.sh before processing configs
# optional flag: -n , runs all steps for nominal plot only
# optional flag: -a , runs all steps for nominal and systematics

NOMINAL_ONLY=0
DO_CLEAN=0
DO_MICROTREE=1
DO_MASSFIT=1
DO_PLOTS=1


# MUST list nominal json first!
MICROTREE_CFG_DIR="configs/microtree"
MICROTREE_VERSION=""
MICROTREE_LIST=(
  "fullAnalysis$MICROTREE_VERSION.json"
  "systDalpha$MICROTREE_VERSION.json"
  "systDchi2cl$MICROTREE_VERSION.json"
  "systDsvpv$MICROTREE_VERSION.json"
  "systDtrkPt$MICROTREE_VERSION.json"
  "systRapGapLoose$MICROTREE_VERSION.json"
  "systRapGapTight$MICROTREE_VERSION.json"
)
MASSFIT_CFG_DIR="configs/massfit"
MASSFIT_VERSION=""
MASSFIT_LIST=(
  "fullAnalysis$MASSFIT_VERSION.json"
  "systDalpha$MASSFIT_VERSION.json"
  "systDchi2cl$MASSFIT_VERSION.json"
  "systDsvpv$MASSFIT_VERSION.json"
  "systDtrkPt$MASSFIT_VERSION.json"
  "systFitPkBg$MASSFIT_VERSION.json"
  "systFitSiglAlpha$MASSFIT_VERSION.json"
  "systFitSiglMean$MASSFIT_VERSION.json"
  "systFitMassWindow$MASSFIT_VERSION.json"
  "systRapGapLoose$MASSFIT_VERSION.json"
  "systRapGapTight$MASSFIT_VERSION.json"
)
PLOT_CFG_DIR="configs/plot"
PLOT_LIST=(
  "fullAnalysis_pt2-5_IsGammaN1.json"
  "fullAnalysis_pt2-5_IsGammaN0.json"
)

# Parse args
while [[ $# -gt 0 ]]; do
  case "$1" in
    # Process only [s]amples
    -s|--samples|--microtree)
      DO_MICROTREE=1
      DO_MASSFIT=0
      DO_PLOTS=0
      shift
      ;;
    # Process only [m]assfit
    -m|--massfit)
      DO_MICROTREE=0
      DO_MASSFIT=1
      DO_PLOTS=0
      shift
      ;;
    # Process only [p]lots
    -p|--plot)
      DO_MICROTREE=0
      DO_MASSFIT=0
      DO_PLOTS=1
      shift
      ;;
    # Process [a]ll configs and steps
    -a|--all)
      NOMINAL_ONLY=0
      DO_MICROTREE=1
      DO_MASSFIT=1
      DO_PLOTS=1
      shift
      ;;
    # Process only [n]ominal configs
    -n|--nominal)
      NOMINAL_ONLY=1
      shift
      ;;
    # Before processing, run [c]lean.sh
    -c|--clean)
      DO_CLEAN=1
      shift
      ;;
  esac
done
if (( $DO_CLEAN == 1 )); then
  source clean.sh
  wait
else
  make
  wait
fi

# Process sample configs
if (( $DO_MICROTREE == 1 )); then
  echo ""
  echo "Config directory: $MICROTREE_CFG_DIR"
  echo ""
  for MICROTREE_JSON in ${MICROTREE_LIST[@]}; do
      echo "Processing: $MICROTREE_CFG_DIR/$MICROTREE_JSON"
      bash makeMicroTree.sh $MICROTREE_CFG_DIR/$MICROTREE_JSON
      wait
      (( $NOMINAL_ONLY == 1 )) && break
  done
  wait
else
  echo "Skipping sample processing."
fi

# Process massfit configs
if (( $DO_MASSFIT == 1 )); then
  echo ""
  echo "MassFit config directory: $MASSFIT_CFG_DIR"
  echo ""
  for MASSFIT_JSON in ${MASSFIT_LIST[@]}; do
    echo "Processing: $MASSFIT_CFG_DIR/$MASSFIT_JSON"
    bash massfit.sh $MASSFIT_CFG_DIR/$MASSFIT_JSON
    wait
    (( $NOMINAL_ONLY == 1 )) && break
  done
  wait
else
  echo "Skipping massfit processing."
fi

# Process plot configs
if (( $DO_PLOTS == 1 )); then
  echo ""
  echo "Plot config directory: $PLOT_CFG_DIR"
  echo ""
  for PLOT_JSON in ${PLOT_LIST[@]}; do
    echo "Processing: $PLOT_CFG_DIR/$PLOT_JSON"
    bash plot.sh "$PLOT_CFG_DIR/$PLOT_JSON"
    wait
  done
  wait
else
  echo "Skipping plotting."
fi

echo "All done!"
