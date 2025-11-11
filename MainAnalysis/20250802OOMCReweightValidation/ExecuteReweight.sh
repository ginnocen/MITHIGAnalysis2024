#!/bin/bash
rm *.so
rm *.d
rm *.pcm

THISFOLDER=$(pwd)
cd ../../
source SetupAnalysis.sh
cd ..
cd ${THISFOLDER}

# Set variables for input files, cuts, and weights
# Inputfiles 1 and 2 are the data and skim respectively
# reweight file is where the reweighting histograms are saved
# VZ, Mult, and Pt weights are the output files saves the MC after reweighting so it easier to plot in the future
# But in this bash script i have the plotting built in with the second root file
INPUT_FILE1="/data00/kdeverea/OOsamples/Skims/output_20250802_Skim_NeNe_IonPhysics4_AllPtV1_0716LynnTest/merged_all.root"
INPUT_FILE2="/data00/kdeverea/OOsamples/Skims/output_20250805_Skim_MinBias_Hijing_NeNe_5362GeV/merged_all.root"
CUT1='(VZ > -15 && VZ < 15) && (PVFilter == 1) && (ClusterCompatibilityFilter == 1) && (HFEMaxPlus > 13.000000 && HFEMaxMinus > 13.000000) && (HLT_MinimumBiasHF_OR_BptxAND_v1)'
CUT2='(VZ > -15 && VZ < 15) && (PVFilter == 1) && (ClusterCompatibilityFilter == 1) && (HFEMaxPlus > 13.000000 && HFEMaxMinus > 13.000000)'

VZ_WEIGHT_OUTPUT="ReweightHist/VZWeightMonteCarlo_080425_Hijing_withTrkPass_NeNe_New.root"
MULT_WEIGHT_OUTPUT="ReweightHist/MultWeightMonteCarlo_080425_Hijing_withTrkPass_NeNe_New.root"
PT_WEIGHT_OUTPUT="ReweightHist/PtWeightMonteCarlo_080425_Hijing_withTrkPass_NeNe_New.root"
ALL_WEIGHT_OUTPUT="ReweightHist/allWeighthistMonteCarlo_080425_Hijing_withTrkPass_NeNe_New.root"
DATA_WEIGHT_OUTPUT="ReweightHist/DATA_080425_Hijing_withTrkPass_NeNe_New.root"

PLOTOUTPUTFOLDER="Plots/080525/"

# Set debug mode to 0 to run all branches
DEBUG_MODE=0
mkdir -p ReweightHist

root -l -b -q "PlotWithWeightNeneNew.cpp+(\
\"${INPUT_FILE1}\", \
\"${INPUT_FILE2}\", \
\"${CUT1}\", \
\"${CUT2}\", \
\"${VZ_WEIGHT_OUTPUT}\", \
\"${MULT_WEIGHT_OUTPUT}\", \
\"${PT_WEIGHT_OUTPUT}\", \
\"${ALL_WEIGHT_OUTPUT}\", \
\"${DATA_WEIGHT_OUTPUT}\", \
${DEBUG_MODE})"
root -l -b -q "DrawWeight.cpp+(\
\"${VZ_WEIGHT_OUTPUT}\", \
\"${MULT_WEIGHT_OUTPUT}\", \
\"${PT_WEIGHT_OUTPUT}\", \
\"${ALL_WEIGHT_OUTPUT}\", \
\"${DATA_WEIGHT_OUTPUT}\", \
\"${PLOTOUTPUTFOLDER}\")"
