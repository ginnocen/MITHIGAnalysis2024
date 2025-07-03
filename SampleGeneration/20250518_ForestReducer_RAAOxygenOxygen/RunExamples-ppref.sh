#!/bin/bash
source clean.sh

DATE=$(date +%Y%m%d)

INPUT=/eos/cms/store/group/phys_heavyions/vpant/ppref2024output/PPRefZeroBiasPlusForward4/crab_ppref2024/250324_080237/0000/HiForestMiniAOD_1.root
OUTPUT=/data00/kdeverea/OOsamples/Skims/${DATE}_Skim_ppref2024_debug_noTrackEventSelection.root

./Execute --Input $INPUT \
   --Output $OUTPUT \
   --DoGenLevel false \
   --Year 2024 \
   --IsData true \
   --IsPP true \
   --Fraction 1.0 \
   --ApplyTriggerRejection true \
   --ApplyEventRejection false \
   --ApplyTrackRejection false \
   --PFTree particleFlowAnalyser/pftree \
   --sampleType -1 \
   --DebugMode true \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/
