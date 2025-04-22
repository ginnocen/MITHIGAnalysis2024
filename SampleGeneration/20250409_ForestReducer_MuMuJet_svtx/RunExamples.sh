#!/bin/bash

rm -rf Output/
mkdir -p Output/

#################
### Data PbPb ###
#################

#./Execute --Input /data00/g2ccbar/mc2018/forest_12172024/HiForestMiniAOD_1-236.root --IsData true --IsPP false --Output SkimReco.root --MinJetPT 80 --Fraction 1.
#./Execute --Input /home/aholterm/MITHIGAnalysis2024/MainAnalysis/20250401_gccbar2vtx/aod/HiForestMiniAOD.root --IsData false --IsPP false --svtx true --Output ch2.root --MinJetPT 60 --Fraction 1.00
./Execute --Input /data00/g2ccbar/mc2018/forest_04182025/HiForestMiniAOD_1059.root --IsData false --IsPP false --svtx false --Output ch3.root --MinJetPT 60 --Fraction 1.00
