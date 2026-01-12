#!/bin/bash
source clean.sh
rm -rf Output/
mkdir -p Output/

#################
### Data PbPb ###
#################

#./Execute --Input /data00/g2ccbar/mc2018/forest_lowerJetPt_may232025/HiForestMiniAOD_1000.root --IsData false --IsPP false --svtx true --Output ch5.root --MinJetPT 30 --Fraction 1.00
./Execute \
    --Input HiForestMiniAOD_123.root \
    --IsData false \
    --IsPP true \
    --svtx true \
    --Output tested2.root \
    --MinJetPT 30 \
    --Fraction 1.00 \
    --PFJetCollection ak3PFJetAnalyzer/t
