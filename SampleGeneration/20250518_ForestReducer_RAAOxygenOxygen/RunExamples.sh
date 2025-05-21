#!/bin/bash
source clean.sh

rm -rf Output/
mkdir -p Output/
./Execute --Input ${PATHSAMPLE}/HiForestMiniAOD_Hijing_merged.root \
   --Output skim_HiForest_Hijing.root \
   --DoGenLevel false \
   --Year 2025 \
   --IsData false \
   --PFTree particleFlowAnalyser/pftree

./Execute --Input ${PATHSAMPLE}/crab_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root \
   --Output skim_HiForest_SingleDiffraction.root \
   --DoGenLevel false \
   --Year 2025 \
   --IsData false \
   --PFTree particleFlowAnalyser/pftree
