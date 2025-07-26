#!/bin/bash
source clean.sh

INPUT=HiForest_2025OO_LowPtCfg_2210.root
#INPUT=/eos/cms/store/group/phys_heavyions/jdlang/Run3_OxygenRAA/PromptForest/IonPhysics0/crab_OO_IonPhysics0_LowPtV2/250711_104114/0002/HiForest_2025OO_LowPtCfg_2210.root
OUTPUT=tempOO.root

./Execute --Input $INPUT \
   --Output $OUTPUT \
   --DoGenLevel false \
   --IsData true \
   --CollisionSystem OO \
   --Fraction 0.05 \
   --ApplyTriggerRejection 1 \
   --ApplyEventRejection true \
   --ApplyTrackRejection true \
   --PFTree particleFlowAnalyser/pftree \
   --sampleType -1 \
   --DebugMode true \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/
