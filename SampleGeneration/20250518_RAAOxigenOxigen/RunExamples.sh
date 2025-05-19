#!/bin/bash

rm -rf Output/
mkdir -p Output/

./Execute --Input /data00/OOsamples/crab_HiForest_250514_Hijing_ppreco_1500pre2_250507.root \
   --Output skim_HiForest_250514_Hijing_ppreco_1500pre2_250507.root \
   --DoGenLevel false \
   --Year 2025 \
   --IsData false \
   --PFTree particleFlowAnalyser/pftree

