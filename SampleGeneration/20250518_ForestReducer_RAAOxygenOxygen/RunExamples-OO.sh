#!/bin/bash
source clean.sh

#INPUT=HiForest_2025OO_LowPtCfg_2210.root
INPUT=/eos/cms/store/group/phys_heavyions/jdlang/Run3_OxygenRAA/PromptForest/IonPhysics0/crab_OO_IonPhysics0_LowPtV2/250711_104114/0002/HiForest_2025OO_LowPtCfg_2210.root
OUTPUT=tempOO.root

EFFPATH=${ProjectBase}/CommonCode/root/

./Execute --Input $INPUT \
   --Output $OUTPUT \
   --DoGenLevel false \
   --IsData true \
   --CollisionSystem OO \
   --Fraction 1.0 \
   --ApplyTriggerRejection 0 \
   --ApplyEventRejection false \
   --ApplyTrackRejection false \
   --PFTree particleFlowAnalyser/pftree \
   --sampleType -1 \
   --DebugMode true \
   --includeL1EMU true \
   --TrackEfficiencyPath ${EFFPATH} \
   --MakeEventWeight true \
   --EvtSelCorrectionFiles "${EFFPATH}OORAA_MULT_EFFICIENCY_HIJING_HF13AND.root,${EFFPATH}OORAA_MULT_EFFICIENCY_HIJING_HF19AND.root,${EFFPATH}OORAA_MULT_EFFICIENCY_HIJING_HF10AND.root" \
