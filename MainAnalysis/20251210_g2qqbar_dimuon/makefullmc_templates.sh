#!/bin/bash

source clean.sh

INPUT_MC0="/data00/g2ccbar/mc2018/skim_011226_0/mergedfile.root"
INPUT_MC1="/data00/g2ccbar/mc2018/skim_011226_1/mergedfile.root"
INPUT_MC2="/data00/g2ccbar/mc2018/skim_011226_2/mergedfile.root"
INPUT_MC3="/data00/g2ccbar/mc2018/skim_011226_3/mergedfile.root"
INPUT_MC4="/data00/g2ccbar/mc2018/skim_011226_4/mergedfile.root"
INPUT_MC5="/data00/g2ccbar/mc2018/skim_011226_5/mergedfile.root"
INPUT_MC6="/data00/g2ccbar/mc2018/skim_011226_6/mergedfile.root"

INPUT_DATA_HighEG="/data00/g2ccbar/data2018/highEGfull.root"
INPUT_DATA_LowEG="/data00/g2ccbar/data2018/lowEGfull.root"

chargesel=0
muPt=3.5

doMC=true
doData=true
outname_MC="mcdistros_pthat.root"
outname_DATA_1="highEG.root"
outname_DATA_2="lowEG.root"

if [ "$doMC" = true ]; then
    # Run MakeDistros in parallel for all MC files
    ./MakeDistros \
    --Input $INPUT_MC0 \
    --Output "mcdistros_0.root" \
    --IsData false \
    --chargeSelection $chargesel \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt $muPt \
    --weightMC true \
    --makeplots true &

./MakeDistros \
    --Input $INPUT_MC1 \
    --Output "mcdistros_1.root" \
    --IsData false \
    --chargeSelection $chargesel \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt $muPt \
    --weightMC true \
    --makeplots true &

./MakeDistros \
    --Input $INPUT_MC2 \
    --Output "mcdistros_2.root" \
    --IsData false \
    --chargeSelection $chargesel \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt $muPt \
    --weightMC true \
    --makeplots true &

./MakeDistros \
    --Input $INPUT_MC3 \
    --Output "mcdistros_3.root" \
    --IsData false \
    --chargeSelection $chargesel \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt $muPt \
    --weightMC true \
    --makeplots true &

./MakeDistros \
    --Input $INPUT_MC4 \
    --Output "mcdistros_4.root" \
    --IsData false \
    --chargeSelection $chargesel \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt $muPt \
    --weightMC true \
    --makeplots true &

./MakeDistros \
    --Input $INPUT_MC5 \
    --Output "mcdistros_5.root" \
    --IsData false \
    --chargeSelection $chargesel \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt $muPt \
    --weightMC true \
    --makeplots true &

./MakeDistros \
    --Input $INPUT_MC6 \
    --Output "mcdistros_6.root" \
    --IsData false \
    --chargeSelection $chargesel \
    --ptBins 60,80,100,120,160,200,250,300 \
    --muPt $muPt \
    --weightMC  true\
    --makeplots true &
fi

if [ "$doData" = true ]; then
    echo "Creating data distributions..."
    ./MakeDistros \
        --Input $INPUT_DATA_HighEG \
        --Output $outname_DATA_1 \
        --IsData true \
        --DataTrigger 80 \
        --chargeSelection $chargesel \
        --ptBins 60,80,100,120,160,200,250,300 \
        --muPt $muPt \
        --weightMC false \
        --makeplots true &

    ./MakeDistros \
        --Input $INPUT_DATA_LowEG \
        --Output $outname_DATA_2 \
        --IsData true \
        --DataTrigger 60 \
        --chargeSelection $chargesel \
        --ptBins 60,80,100,120,160,200,250,300 \
        --muPt $muPt \
        --weightMC false \
        --makeplots true &
fi

# Wait for all parallel jobs to complete
wait

if [ "$doMC" = true ]; then
    echo "All MC distributions created. Merging with hadd..."
    hadd -f $outname_MC mcdistros_0.root mcdistros_1.root mcdistros_2.root mcdistros_3.root mcdistros_4.root mcdistros_5.root mcdistros_6.root
fi

if [ "$doMC" = true ]; then
    rm mcdistros_0.root
    rm mcdistros_1.root
    rm mcdistros_2.root
    rm mcdistros_3.root
    rm mcdistros_4.root
    rm mcdistros_5.root
    rm mcdistros_6.root
fi

echo "DONE"
