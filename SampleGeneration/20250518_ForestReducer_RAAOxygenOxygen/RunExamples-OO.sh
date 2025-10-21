#!/bin/bash
source clean.sh

INPUT=/eos/cms/store/group/phys_heavyions/jdlang/Run3_OxygenRAA/PromptForest/IonPhysics0/crab_OO_IonPhysics0_LowPtV2/250711_104114/0000/HiForest_2025OO_LowPtCfg_1.root
OUTPUT=tempOO_noTriggerNoEvt.root

CORRPATH=${ProjectBase}/CommonCode/root/

./Execute --Input $INPUT \
   --Output $OUTPUT \
   --IsData true \
   --CollisionSystem OO \
   --Fraction 1.0 \
   --ApplyTriggerRejection 0 \
   --ApplyEventRejection false \
   --ApplyTrackRejection true \
   --sampleType -1 \
   --DebugMode true \
   --includeL1EMU false \
   --CorrectionPath ${CORRPATH} \
