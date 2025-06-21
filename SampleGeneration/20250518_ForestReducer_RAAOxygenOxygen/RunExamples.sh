#!/bin/bash
source clean.sh

echo ${PATHSAMPLE}
echo ${PATHSKIMSAMPLE}

rm -rf Output/
mkdir -p Output/

./Execute --Input ${PATHSAMPLE}/crab_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root \
   --Output ${PATHSKIMSAMPLE}/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root \
   --DoGenLevel false \
   --Year 2025 \
   --IsData false \
   --Fraction 1. \
   --PFTree particleFlowAnalyser/pftree \
   --sampleType 0 \
   --DebugMode true \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/

./Execute --Input ${PATHSAMPLE}/crab_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root \
   --Output ${PATHSKIMSAMPLE}/skim_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root \
   --DoGenLevel false \
   --Year 2025 \
   --Fraction 1. \
   --IsData false \
   --PFTree particleFlowAnalyser/pftree \
   --sampleType 1 \
   --DebugMode true \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/

./Execute --Input ${PATHSAMPLE}/crab_HiForest_250514_Starlight_DoubleDiffraction_OO_5362GeV_1505PR47944.root \
   --Output ${PATHSKIMSAMPLE}/skim_HiForest_250514_Starlight_DoubleDiffraction_OO_5362GeV_1505PR47944.root \
   --DoGenLevel false \
   --Year 2025 \
   --Fraction 1. \
   --IsData false \
   --PFTree particleFlowAnalyser/pftree \
   --sampleType 2 \
   --DebugMode true \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/

./Execute --Input ${PATHSAMPLE}/HiForestMiniAOD_alphaO_06172025.root\
   --Output ${PATHSKIMSAMPLE}/skim_20250606_HiForestMiniAOD_alphaO_06172025.root \
   --DoGenLevel false \
   --Year 2025 \
   --Fraction 1. \
   --IsData false \
   --PFTree particleFlowAnalyser/pftree \
   --sampleType 4 \
   --DebugMode true \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/
