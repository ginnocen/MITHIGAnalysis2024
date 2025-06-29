#!/bin/bash
source clean.sh

INPUT=/eos/cms/store/group/phys_heavyions/jdlang/Run3_OO_2025Data_FastPrivateReco/TEMP/HiForestMiniAOD_PhysicsIonPhysics0_393767.root
OUTPUT=/data00/kdeverea/OOsamples/Skims/Run3_OO_2025Data_FastPrivateReco_393767.root

./Execute --Input $INPUT \
   --Output $OUTPUT \
   --DoGenLevel false \
   --Year 2024 \
   --IsData true \
   --IsPP false \
   --Fraction 1.0 \
   --ApplyTriggerRejection true \
   --ApplyEventRejection true \
   --ApplyTrackRejection true \
   --PFTree particleFlowAnalyser/pftree \
   --sampleType -1 \
   --DebugMode true \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/
