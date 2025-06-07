#!/bin/bash
source clean.sh

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
   --DebugMode true

./Execute --Input ${PATHSAMPLE}/crab_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root \
   --Output ${PATHSKIMSAMPLE}/skim_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root \
   --DoGenLevel false \
   --Year 2025 \
   --Fraction 1. \
   --IsData false \
   --PFTree particleFlowAnalyser/pftree \
   --sampleType 1 \
   --DebugMode true

./Execute --Input ${PATHSAMPLE}/crab_HiForest_250514_Starlight_DoubleDiffraction_OO_5362GeV_1505PR47944.root \
   --Output ${PATHSKIMSAMPLE}/skim_HiForest_250514_Starlight_DoubleDiffraction_OO_5362GeV_1505PR47944.root \
   --DoGenLevel false \
   --Year 2025 \
   --Fraction 1. \
   --IsData false \
   --PFTree particleFlowAnalyser/pftree \
   --sampleType 2 \
   --DebugMode true

./Execute --Input ${PATHSAMPLE}/HiForestMiniAOD_alphaO_10k.root\
   --Output ${PATHSKIMSAMPLE}/skim_20250606_HiForestMiniAOD_alphaO_10k.root \
   --DoGenLevel false \
   --Year 2025 \
   --Fraction 1. \
   --IsData false \
   --PFTree particleFlowAnalyser/pftree \
   --sampleType 4 \
   --DebugMode true
