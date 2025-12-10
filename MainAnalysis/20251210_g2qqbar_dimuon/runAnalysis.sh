#!/bin/bash

source clean.sh

./ExecuteEfficiency \
    --Input "/data00/g2ccbar/mc2018/skim_120925_1/mergedfile.root" \
    --Output "testefficiencies.root" \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt 3.5 \

echo "DONE WITH EFFICIENCIES"
