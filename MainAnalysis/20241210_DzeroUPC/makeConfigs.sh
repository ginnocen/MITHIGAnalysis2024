#!/bin/bash

#PT_Y_BINS=(
#  #ptmin #ptmax #ymin #ymax
#  0   1   -1    0
#  0   1    0    1
#  1   2   -1    0
#  1   2    0    1
#  2   3   -1    0
#  2   3    0    1
#  3   4   -1    0
#  3   4    0    1
#  4   5   -1    0
#  4   5    0    1
#  0   2   -2.4 -2
#  0   2   -2   -1
#  0   2    1    2
#  0   2    2    2.4
#  2   5   -2.4 -2
#  2   5   -2   -1
#  2   5    1    2
#  2   5    2    2.4
#)

PT_Y_BINS=(
  2   5   -2   -1
  2   5   -1    0
  2   5    0    1
  2   5    1    2
)

MAKE_MICROTREE_CFGS=1
MAKE_MASSFIT_CFGS=1
MAKE_PLOT_CFGS=1
DO_REWEIGHTING=0
USE_GAMMAN_FOR_NGAMMA=1
MERGER_MIRROR_YBINS=1

SKIM_DATA="/data00/jdlang/UPCD0LowPtAnalysis/SkimsData/20250528_Skim_2023Data_Feb2025ReReco_NEW_HIForward01235679.root"
#SKIM_DATA="/data00/jdlang/UPCD0LowPtAnalysis/SkimsData/20250528_Skim_2023Data_Feb2025ReReco_OLD_HIForward01235679.root"
#SKIM_DATA="/data00/jdlang/UPCD0LowPtAnalysis/SkimsData/20250508_Skim_2023Data_Jan2024ReReco_HIForward0.root"
#SKIM_DATA="/data00/jdlang/UPCD0LowPtAnalysis/SkimsData/20250527_Skim_2023Data_Jan2024ReReco_HIForward0_Unfiltered.root"
#SKIM_DATA="/data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/SkimsData/20250312_ForestDfinderData23Skim_v4.root"
SKIM_MC_FORCED_D0_A="/data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/SkimsMC/20250306_v4_Pthat0_ForceD0DecayD0Filtered_MassWindow040_BeamA/mergedfile.root"
SKIM_MC_FORCED_D0_B="/data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/SkimsMC/20250306_v4_Pthat0_ForceD0DecayD0Filtered_MassWindow040_BeamB/mergedfile.root"
SKIM_MC_INCLUSIVE_A="/data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/SkimsMC/20250227_v4_OldPthat5_Inclusive_BeamA/mergedfile.root"
SKIM_MC_INCLUSIVE_B="/data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/SkimsMC/20250227_v4_OldPthat5_Inclusive_BeamA/mergedfile.root"
GPT_GY_WEIGHT_DIR="../../WeightHandler/20250305_DzeroUPC_GptGyWeight/Weights"
MULT_WEIGHT_DIR="../../WeightHandler/20250305_DzeroUPC_multiplicityWeight/Weights"

MICROTREE_CFG_DIR="configs/microtree"
MICROTREE_ROOTS=(
  "Data.root"
  "MC.root"
  "MC_inclusive.root"
)
MICROTREE_fullAnalysis="$MICROTREE_CFG_DIR/fullAnalysis.json"
MICROTREE_systDalpha="$MICROTREE_CFG_DIR/systDalpha.json"
MICROTREE_systDchi2cl="$MICROTREE_CFG_DIR/systDchi2cl.json"
MICROTREE_systDsvpv="$MICROTREE_CFG_DIR/systDsvpv.json"
MICROTREE_systDtrkPt="$MICROTREE_CFG_DIR/systDtrkPt.json"
MICROTREE_systRapGapLoose="$MICROTREE_CFG_DIR/systRapGapLoose.json"
MICROTREE_systRapGapTight="$MICROTREE_CFG_DIR/systRapGapTight.json"

MASSFIT_CFG_DIR="configs/massfit"
MASSFIT_fullAnalysis="$MASSFIT_CFG_DIR/fullAnalysis.json"
MASSFIT_systDalpha="$MASSFIT_CFG_DIR/systDalpha.json"
MASSFIT_systDchi2cl="$MASSFIT_CFG_DIR/systDchi2cl.json"
MASSFIT_systDsvpv="$MASSFIT_CFG_DIR/systDsvpv.json"
MASSFIT_systDtrkPt="$MASSFIT_CFG_DIR/systDtrkPt.json"
MASSFIT_systRapGapLoose="$MASSFIT_CFG_DIR/systRapGapLoose.json"
MASSFIT_systRapGapTight="$MASSFIT_CFG_DIR/systRapGapTight.json"
MASSFIT_systFitPkBg="$MASSFIT_CFG_DIR/systFitPkBg.json"
MASSFIT_systFitSiglAlpha="$MASSFIT_CFG_DIR/systFitSiglAlpha.json"
MASSFIT_systFitSiglMean="$MASSFIT_CFG_DIR/systFitSiglMean.json"
MASSFIT_systFitMassWindow="$MASSFIT_CFG_DIR/systFitMassWindow.json"

PLOT_CFG_DIR="configs/plot"
# NO file extension for this one - it is added later!
PLOT_fullAnalysis="$PLOT_CFG_DIR/fullAnalysis"



### CONFIG BUILDER FUNCTIONS  #################################################



# Microtree Configs
make_microtree_config() {
  local configOutput=${1}
  local ptmin=${2}
  local ptmax=${3}
  local ymin=${4}
  local ymax=${5}
  local isGammaN=${6}
  local isLastEntry=${7}
  local doSystD=${8}
  local doSystRapGap=${9}
  local doReweighting=${10}
  echo "Making MicroTree Config: $(basename $configOutput)"
  # Make header
  if [[ ! -e "$configOutput" ]]; then
    microTreeDir=$(basename $configOutput)
    microTreeDir="${microTreeDir%.*}"
cat > $configOutput <<EOF
{
  "MicroTreeDir": "$microTreeDir",
  "MicroTrees": [
EOF
  fi
  # Add config for pt & y bin:
  for microtreeRoot in ${MICROTREE_ROOTS[@]}; do
    local isData="true"
    [[ "$microtreeRoot" != "Data.root" ]] && isData="false"
    local input=$SKIM_DATA
    if [[ "$microtreeRoot" == "MC.root" ]]; then
      (( $isGammaN == 1 )) && input=$SKIM_MC_FORCED_D0_A || input=$SKIM_MC_FORCED_D0_B
    elif [[ "$microtreeRoot" == "MC_inclusive.root" ]]; then
      (( $isGammaN == 1 )) && input=$SKIM_MC_INCLUSIVE_A || input=$SKIM_MC_INCLUSIVE_B
      (( $USE_GAMMAN_FOR_NGAMMA == 1 && $isGammaN == 0 )) && input=$SKIM_MC_INCLUSIVE_A
    fi
    local comma=""
    [[ $doReweighting -eq 1 && "$microtreeRoot" == "MC.root" ]] && comma=","
    # Write config settings to file:
cat >> $configOutput <<EOF
    {
      "MicroTreeBaseName": "$microtreeRoot",
      "Input": "$input",
      "MinDzeroPT": $ptmin,
      "MaxDzeroPT": $ptmax,
      "MinDzeroY": $ymin,
      "MaxDzeroY": $ymax,
      "IsGammaN": $isGammaN,
      "TriggerChoice": 1,
      "IsData": $isData,
      "DoSystD": $doSystD,
      "DoSystRapGap": $doSystRapGap$comma
EOF
    # Add reweighting (if needed)
    if [[ $doReweighting -eq 1 && "$microtreeRoot" == "MC.root" ]]; then
      doGptGyWeight="true"
      GptGyWeightFile="$GPT_GY_WEIGHT_DIR/testWeight.root"
      doMultWeight="true"
      multWeightFile="$MULT_WEIGHT_DIR/pt${ptmin}-${ptmax}_y${ymin}-${ymax}_IsGammaN${isGammaN}.root"
cat >> $configOutput <<EOF
      "DoGptGyReweighting": $doGptGyWeight,
      "GptGyWeightFileName": "$GptGyWeightFile",
      "DoMultReweighting": $doMultWeight,
      "MultWeightFileName": "$multWeightFile"
EOF
    fi
    # Close brackets:
    if [[ $isLastEntry -eq 1 && "$microtreeRoot" == "MC_inclusive.root" ]]; then
cat >> $configOutput <<EOF
    }
  ]
}
EOF
    else
      echo "    }," >> $configOutput
    fi
    wait
  done
  sleep 0.02
}

# Massfit Configs
make_massfit_config() {
  local configOutput=${1}
  local configInput=${2}
  local massfitDir=${3}
  local ptmin=${4}
  local ptmax=${5}
  local ymin=${6}
  local ymax=${7}
  local isGammaN=${8}
  local isLastEntry=${9}
  local doSystPkBg=${10}
  local doSystSiglAlpha=${11}
  local doSystSiglMean=${12}
  local doSystMassWindow=${13}
  
  fileString="pt${ptmin}-${ptmax}_y${ymin}-${ymax}_IsGammaN${isGammaN}"
  yminMirror=$(( -1 * $ymax ))
  ymaxMirror=$(( -1 * $ymin ))
  mirrFileString="pt${ptmin}-${ptmax}_y${yminMirror}-${ymaxMirror}_IsGammaN${isGammaN}"
  gNforNgFileString="pt${ptmin}-${ptmax}_y${yminMirror}-${ymaxMirror}_IsGammaN1"
  gNforNgMirrFileString="pt${ptmin}-${ptmax}_y${ymin}-${ymax}_IsGammaN1"
  local dataInput="$configInput/$fileString/Data.root"
  local fitmcInputs="$configInput/$fileString/MC_inclusive.root"
  local sigswpInputs="$configInput/$fileString/MC.root"
  local effmcInputs="$configInput/$fileString/MC.root"
  if (( $MERGER_MIRROR_YBINS == 1 )); then
    fitmcInputs="$fitmcInputs,$configInput/$mirrFileString/MC_inclusive.root"
  fi
  if (( $USE_GAMMAN_FOR_NGAMMA == 1 && $isGammaN == 0)); then
    fitmcInputs="$configInput/$gNforNgFileString/MC_inclusive.root"
    if (( $MERGER_MIRROR_YBINS == 1 )); then
      fitmcInputs="$fitmcInputs,$configInput/$gNforNgMirrFileString/MC_inclusive.root"
    fi
  fi
  (( $doSystPkBg == 1 )) && doPkkk="false" || doPkkk="true"
  (( $doSystPkBg == 1 )) && doPkpp="false" || doPkpp="true"
  (( $doSystSiglAlpha == 1 )) && sigAlphaRange="0.0" || sigAlphaRange="0.25"
  (( $doSystSiglMean == 1 )) && sigMeanRange="0.0" || sigMeanRange="0.015"
  (( $doSystMassWindow == 1 )) && massWindow="1.66,2.26,48" || massWindow="1.66,2.16,40"
  
  echo "Making MassFit Config: $(basename $configOutput)"
  # Make header
  if [[ ! -e "$configOutput" ]]; then
cat > $configOutput <<EOF
{
  "FitDir": "$massfitDir",
  "MicroTrees": [
EOF
  fi
  # Write config settings to file:
cat >> $configOutput <<EOF
    {
      "dataInput": "$dataInput",
      "fitmcInputs": "$fitmcInputs",
      "effmcInput": "$effmcInputs",
      "doPkkk": $doPkkk,
      "doPkpp": $doPkpp,
      "sigAlphaRange": $sigAlphaRange,
      "sigMeanRange": $sigMeanRange,
      "systMassWin": "$massWindow"
EOF
  # Close brackets:
  if (( $isLastEntry == 1 )); then
cat >> $configOutput <<EOF
    }
  ]
}
EOF
  else
    echo "    }," >> $configOutput
  fi
  wait
  sleep 0.02
}

# Plot Configs
make_plot_config() {
  local configOutput=${1}
  local outputDir=${2}
  local inputPoints=${3}
  local ptmin=${4}
  local ptmax=${5}
  local isGammaN=${6}
  
  echo "Making Plot Config: $(basename $configOutput)"
  # Write config settings to file:
cat >> $configOutput <<EOF
{
  "PlotDir": "$outputDir",
  "Plots": [
    {
      "MinDzeroPT": $ptmin,
      "MaxDzeroPT": $ptmax,
      "IsGammaN": $isGammaN,
      "InputPoints": "$inputPoints",
      "UseMaxFitUncert": true
    }
  ]
}
EOF
  wait
  sleep 0.02
}



### MAKE CONFIG FILES #########################################################



if [[ "$MAKE_MICROTREE_CFGS" -eq "1" ]]; then
  rm -r $MICROTREE_CFG_DIR &>/dev/null
  mkdir -p $MICROTREE_CFG_DIR
fi
if [[ "$MAKE_MASSFIT_CFGS" -eq "1" ]]; then
  rm -r $MASSFIT_CFG_DIR &>/dev/null
  mkdir -p $MASSFIT_CFG_DIR
fi
if [[ "$MAKE_PLOT_CFGS" -eq "1" ]]; then
  rm -r $PLOT_CFG_DIR &>/dev/null
  mkdir -p $PLOT_CFG_DIR
fi

for (( isGammaN=1 ; isGammaN >= 0 ; isGammaN-- )); do
  for (( i=0 ; i < ${#PT_Y_BINS[@]} ; i+=4 )); do
    ptmin=${PT_Y_BINS[ $i + 0 ]}
    ptmax=${PT_Y_BINS[ $i + 1 ]}
    ymin=${PT_Y_BINS[ $i + 2 ]}
    ymax=${PT_Y_BINS[ $i + 3 ]}
    isLastEntry=0
    (( $i == $((${#PT_Y_BINS[@]} - 4)) && $isGammaN == 0 )) && isLastEntry=1
    echo "isGammaN: $isGammaN, $ptmin < Dpt < $ptmax, $ymin < Dy < $ymax"
    if [[ "$MAKE_MICROTREE_CFGS" -eq "1" ]]; then
      make_microtree_config $MICROTREE_fullAnalysis $ptmin $ptmax $ymin $ymax\
        $isGammaN $isLastEntry 0 0 $DO_REWEIGHTING
      make_microtree_config $MICROTREE_systDalpha $ptmin $ptmax $ymin $ymax\
        $isGammaN $isLastEntry 3 0 $DO_REWEIGHTING
      make_microtree_config $MICROTREE_systDchi2cl $ptmin $ptmax $ymin $ymax\
        $isGammaN $isLastEntry 4 0 $DO_REWEIGHTING
      make_microtree_config $MICROTREE_systDsvpv $ptmin $ptmax $ymin $ymax\
        $isGammaN $isLastEntry 1 0 $DO_REWEIGHTING
      make_microtree_config $MICROTREE_systDtrkPt $ptmin $ptmax $ymin $ymax\
        $isGammaN $isLastEntry 2 0 $DO_REWEIGHTING
      make_microtree_config $MICROTREE_systRapGapLoose $ptmin $ptmax\
        $ymin $ymax $isGammaN $isLastEntry 0 -1 $DO_REWEIGHTING
      make_microtree_config $MICROTREE_systRapGapTight $ptmin $ptmax\
        $ymin $ymax $isGammaN $isLastEntry 0 1 $DO_REWEIGHTING
    fi
    if [[ "$MAKE_MASSFIT_CFGS" -eq "1" ]]; then
      make_massfit_config $MASSFIT_fullAnalysis "fullAnalysis" "MassFit"\
        $ptmin $ptmax $ymin $ymax $isGammaN $isLastEntry 0 0 0 0
      make_massfit_config $MASSFIT_systDalpha "systDalpha" "MassFit"\
        $ptmin $ptmax $ymin $ymax $isGammaN $isLastEntry 0 0 0 0
      make_massfit_config $MASSFIT_systDchi2cl "systDchi2cl" "MassFit"\
        $ptmin $ptmax $ymin $ymax $isGammaN $isLastEntry 0 0 0 0
      make_massfit_config $MASSFIT_systDsvpv "systDsvpv" "MassFit"\
        $ptmin $ptmax $ymin $ymax $isGammaN $isLastEntry 0 0 0 0
      make_massfit_config $MASSFIT_systDtrkPt "systDtrkPt" "MassFit"\
        $ptmin $ptmax $ymin $ymax $isGammaN $isLastEntry 0 0 0 0
      make_massfit_config $MASSFIT_systRapGapLoose "systRapGapLoose" "MassFit"\
        $ptmin $ptmax $ymin $ymax $isGammaN $isLastEntry 0 0 0 0
      make_massfit_config $MASSFIT_systRapGapTight "systRapGapTight" "MassFit"\
        $ptmin $ptmax $ymin $ymax $isGammaN $isLastEntry 0 0 0 0
      make_massfit_config $MASSFIT_systFitPkBg "fullAnalysis"\
        "MassFit_systFitPkBg" $ptmin $ptmax $ymin $ymax $isGammaN\
        $isLastEntry 1 0 0 0
      make_massfit_config $MASSFIT_systFitSiglAlpha "fullAnalysis"\
        "MassFit_systFitSiglAlpha" $ptmin $ptmax $ymin $ymax $isGammaN\
        $isLastEntry 0 1 0 0
      make_massfit_config $MASSFIT_systFitSiglMean "fullAnalysis"\
        "MassFit_systFitSiglMean" $ptmin $ptmax $ymin $ymax $isGammaN\
        $isLastEntry 0 0 1 0
      make_massfit_config $MASSFIT_systFitMassWindow "fullAnalysis"\
        "MassFit_systFitMassWindow" $ptmin $ptmax $ymin $ymax $isGammaN\
        $isLastEntry 0 0 0 1
    fi
  done
  if [[ "$MAKE_PLOT_CFGS" -eq "1" ]]; then
    prev_ptmin=0
    prev_ptmax=0
    configOutput=""
    outputDir=""
    inputPoints=""
    for (( i=0 ; i < ${#PT_Y_BINS[@]} ; i+=4 )); do
      ptmin=${PT_Y_BINS[ $i + 0 ]}
      ptmax=${PT_Y_BINS[ $i + 1 ]}
      ymin=${PT_Y_BINS[ $i + 2 ]}
      ymax=${PT_Y_BINS[ $i + 3 ]}
      inputPoint="fullAnalysis/pt${ptmin}-${ptmax}_y${ymin}-${ymax}_IsGammaN${isGammaN}/MassFit/correctedYields.md"
      if (( $ptmin != $prev_ptmin || $ptmax != $prev_ptmax )); then
        if (( $prev_ptmin != $prev_ptmax )); then
          make_plot_config $configOutput $outputDir $inputPoints \
            $prev_ptmin $prev_ptmax $isGammaN
        fi
        prev_ptmin=$ptmin
        prev_ptmax=$ptmax
        outputBase="pt${ptmin}-${ptmax}_IsGammaN${isGammaN}"
        configOutput="${PLOT_fullAnalysis}_${outputBase}.json"
        outputDir="plot/fullAnalysis_$outputBase"
        inputPoints="$inputPoint"
      else
        inputPoints="$inputPoints,$inputPoint"
      fi
    done
    make_plot_config $configOutput $outputDir $inputPoints \
      $prev_ptmin $prev_ptmax $isGammaN
  fi
done
