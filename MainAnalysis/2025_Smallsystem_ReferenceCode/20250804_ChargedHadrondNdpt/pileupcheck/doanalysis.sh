#!/bin/bash

MODE="output_lowpileup_2"
./RunParallelReadParam.sh \
    --Output ${MODE} \
    --Input ../NeNe_InputFileList.txt \
    --CollisionSystem true \
    --ApplyEventSelection true \
    --UseEventWeight true \
    --UseSpeciesWeight true \
    --UseTrackWeight true \
    --TrackWeightSelection 2 \
    --TriggerChoice 1 \
    --EventSelectionOption 2 \
    --SpeciesCorrectionOption 2 \
    --ScaleFactor 1