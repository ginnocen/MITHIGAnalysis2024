#!/bin/bash

source clean.sh

INPUT_MC="/data00/g2ccbar/mc2018/skim_011226_0/mergedfile.root"
INPUT_DATA="/data00/g2ccbar/data2018/supermerged.root"

# Check if both distribution files exist
if [[ -f "mcdistros.root" && -f "datadistros.root" ]]; then
    echo "Both mcdistros.root and datadistros.root exist, skipping to ExecuteYield"
else
    ./MakeDistros \
        --Input $INPUT_MC \
        --Input_Efficiency "testefficiencies.root" \
        --Output "mcdistros.root" \
        --IsData false \
        --chargeSelection 0 \
        --ptBins 60,80,100,120,160,200,250,300 \
        --muPt 3.5 \
        --makeplots true

    echo "DONE WITH DISTRIBUTIONS"

    ./MakeDistros \
        --Input $INPUT_DATA \
        --Input_Efficiency "testefficiencies.root" \
        --Output "datadistros.root" \
        --IsData true \
        --chargeSelection 0 \
        --ptBins 60,80,100,120,160,200,250,300 \
        --muPt 3.5 \
        --makeplots true
fi

./ExecuteYield \
    --Input "datadistros.root" \
    --Templates "mcdistros.root" \
    --Output "datayields.root" \
    --ptBins 60,80,100,120,160,200,250,300 \
    --doLF_DCA true \
    --doLF_invMass true \
    --makeplots true

echo "DONE WITH YIELD FITTING"
