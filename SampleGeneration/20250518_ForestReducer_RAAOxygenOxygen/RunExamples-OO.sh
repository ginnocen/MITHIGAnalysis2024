#!/bin/bash
source clean.sh

INPUT=samples/HiForest_2025OO_LowPtCfg_2210.root
#INPUT=/eos/cms/store/group/phys_heavyions/jdlang/Run3_OxygenRAA/PromptForest/IonPhysics0/crab_OO_IonPhysics0_LowPtV2/250711_104114/0002/HiForest_2025OO_LowPtCfg_2210.root
OUTPUT=tempOO.root

CORRPATH=${ProjectBase}/CommonCode/root/

./Execute --Input $INPUT \
   --Output $OUTPUT \
   --IsData True \
   --CollisionSystem OO \
   --Fraction 0.1 \
   --ApplyTriggerRejection 0 \
   --ApplyEventRejection false \
   --ApplyTrackRejection false \
   --sampleType -1 \
   --DebugMode false \
   --includeL1EMU false \
   --CorrectionPath ${CORRPATH} \



