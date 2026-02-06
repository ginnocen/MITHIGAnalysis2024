#!/bin/bash

source clean.sh

INPUT="/data00/g2ccbar/mc2018/skim_011226_0/mergedfile.root"
#"/data00/g2ccbar/mc2018/skim_010526_soft_0/mergedfile.root"
#INPUT="/data00/g2ccbar/mc2018/skim_120925_1/mergedfile.root"

./ExecuteEfficiency \
    --Input $INPUT \
    --Output "testefficiencies.root" \
    --IsData false \
    --ptBins 100,120,160,200,250 \
    --muPt 4 \
    --chargeSelection 0 \
    --makeplots true \

echo "DONE WITH EFFICIENCIES"

./makefullmc_templates.sh
echo "DONE WITH DISTRIBUTIONS"

./ExecuteYield \
    --Input "data_distros_60.root" \
    --Templates "mcdistros_pthat.root" \
    --Output "testyields2.root" \
    --ptBins 100,120,160,200,250 \
    --variables muDR,mumuZ \
    --kde 1.3,1.1 \
    --fitRangeMin 0.0,0.0 \
    --fitRangeMax 0.6,1.0 \
    --chargeSelection 0 \
    --makeplots true \

echo "DONE WITH YIELD FITTING"
