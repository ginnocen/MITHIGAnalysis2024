#!/bin/bash
source clean.sh

INPUT=/eos/cms/store/group/phys_heavyions/vpant/ppref2024output/PPRefZeroBiasPlusForward4/crab_ppref2024/250324_080237/0000/HiForestMiniAOD_1.root
OUTPUT=temppp.root

CORRPATH=${ProjectBase}/CommonCode/root/

./Execute --Input $INPUT \
   --Output $OUTPUT \
   --IsData true \
   --CollisionSystem pp \
   --Fraction 1.0 \
   --ApplyTriggerRejection 0 \
   --ApplyEventRejection false \
   --ApplyTrackRejection false \
   --sampleType -1 \
   --DebugMode true \
   --includeL1EMU false \
   --CorrectionPath ${CORRPATH} \
