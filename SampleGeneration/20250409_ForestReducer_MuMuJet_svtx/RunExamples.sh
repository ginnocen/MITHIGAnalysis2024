#!/bin/bash
source clean.sh
rm -rf Output/
mkdir -p Output/

#################
### Data PbPb ###
#################

#./Execute --Input /data00/g2ccbar/mc2018/forest_lowerJetPt_may232025/HiForestMiniAOD_1000.root --IsData false --IsPP false --svtx true --Output ch5.root --MinJetPT 30 --Fraction 1.00
./Execute --Input /data00/g2ccbar/mc2018/skimmer_validation/gccbartestforest.root --IsData false --IsPP false --svtx true --Output /data00/g2ccbar/mc2018/skimmer_validation/skim.root --MinJetPT 30 --Fraction 1.00
