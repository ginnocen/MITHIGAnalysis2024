#!/bin/bash
source clean.sh

rm -rf Output/
mkdir -p Output/
./Execute --Input ${PATHSAMPLE}/crab_HiForest_250514_Hijing_ppreco_1500pre2_250507.root \
   --Output skim_HiForest_Hijing.root \
   --DoGenLevel true \
   --Year 2025 \
   --IsData false \
   --PFTree particleFlowAnalyser/pftree

./Execute --Input ${PATHSAMPLE}/crab_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root \
   --Output skim_HiForest_SingleDiffraction.root \
   --DoGenLevel true \
   --Year 2025 \
   --IsData false \
   --PFTree particleFlowAnalyser/pftree
