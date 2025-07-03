#!/bin/bash
source clean.sh

INPUT=/eos/cms/store/group/phys_heavyions/jdlang/Run3_OO_2025Data_QuickForest/pO_PhysicsIonPhysics0_393952/crab_pO_PhysicsIonPhysics0_393952/250701_063441/0001/HiForestMiniAOD_1.root
OUTPUT=/data00/kdeverea/OOsamples/Skims/temp.root

./Execute --Input $INPUT \
   --Output $OUTPUT \
   --DoGenLevel false \
   --Year 2025 \
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
