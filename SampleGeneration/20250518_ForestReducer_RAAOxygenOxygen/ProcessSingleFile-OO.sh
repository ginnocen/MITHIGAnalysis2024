#!/bin/bash

FILEPATH=${1}
COUNTER=${2}
OUTPUT=${3}
DOGENLEVEL=${4}
ISDATA=${5}
SAMPLETYPE=${6}
SAVETRIGGERBITS=${7}
DEBUGMODE=${8}
INCLUDEPPSANDFSC=${9}
INCLUDEPF=${10}

file="$FILEPATH"

EFFPATH=${ProjectBase}/CommonCode/root/

./Execute --Input "$file" \
   --Output ${OUTPUT}/output_${COUNTER}.root \
   --DoGenLevel $DOGENLEVEL \
   --Year 2025 \
   --IsData $ISDATA \
   --IsPP false \
   --Fraction 1.0 \
   --ApplyTriggerRejection true \
   --ApplyEventRejection false \
   --ApplyTrackRejection true \
   --PFTree particleFlowAnalyser/pftree \
   --sampleType $SAMPLETYPE \
   --DebugMode $DEBUGMODE \
   --includeFSCandPPSMode $INCLUDEPPSANDFSC \
   --includePFMode $INCLUDEPF \
   --saveTriggerBitsMode $SAVETRIGGERBITS \
   --TrackEfficiencyPath $EFFPATH \
   --HideProgressBar false \
   --MakeEventWeight true \
   --EvtSelCorrectionFiles "${EFFPATH}OORAA_MULT_EFFICIENCY_HIJING_HF13AND.root,${EFFPATH}OORAA_MULT_EFFICIENCY_HIJING_HF19AND.root,${EFFPATH}OORAA_MULT_EFFICIENCY_HIJING_HF10AND.root" \
   --MC_ReweightFile "${EFFPATH}OORAA_MC_Reweight_HIJING_HF13AND.root" \
   --Species_ReweightFile "${EFFPATH}ParticleSpeciesCorrectionFactorsOO.root" \

wait

sleep 0.1
wait
