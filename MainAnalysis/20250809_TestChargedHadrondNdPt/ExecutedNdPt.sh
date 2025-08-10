#!/bin/bash
rm *.so
rm *.d
rm *.pcm

THISFOLDER=$(pwd)
cd ../../
source SetupAnalysis.sh
cd ..
cd ${THISFOLDER}

INPUT_FILE1="/data00/kdeverea/OOsamples/Skims/output_20250805_Skim_NeNe_IonPhysics4_AllPtV1_0716LynnTest/merged_all.root"
APPLY_EVENT_WEIGHT=true
APPLY_TRACK_WEIGHT=true
APPLY_TRACK_SPECIES_WEIGHT=true
TRKPT_CUT=0.4
TRKETA_CUT=2.4
OUTPUT_FILE="dNdPT_allweights.root"
DEBUG=false

root -l -b -q "dNdPT.cpp+(\
\"${INPUT_FILE1}\", \
${APPLY_EVENT_WEIGHT}, \
${APPLY_TRACK_WEIGHT}, \
${APPLY_TRACK_SPECIES_WEIGHT}, \
${TRKPT_CUT}, \
${TRKETA_CUT}, \
\"${OUTPUT_FILE}\", \
${DEBUG})"

