!/bin/bash

# ============================================================
# OO data
# ============================================================
source clean.sh

INPUT=/data00/OOsamples/Skims/tempOO.root
#/data00/kdeverea/OOsamples/Skims/output_20250728_Skim_OO_IonPhysics0_LowPtV2_250711_104114/0000_merged.root
#INPUT=samples/skim_HiForest_2025OO_LowPtCfg_2210.root
OUTPUTANALYSIS=output/20250728_skim_HiForest_2025OO_LowPtCfg_2210_trackTight.root

./ExecuteChargedHadrondNdpt \
  --Input $INPUT \
  --Output $OUTPUTANALYSIS \
  --IsData true \
  --ApplyEventSelection true \
  --UseSpeciesWeight false \
  --UseEventWeight false \
  --UseTrackWeight false \
  --TrackWeightSelection 3 \
  --MinTrackPt 0.4 \
  --MinLeadingTrackPt -1 \
  --TriggerChoice 1 \
  --CollisionSystem true \
  --EventSelectionOption 0 \
  --SpeciesCorrectionOption 0 \
  --ScaleFactor 1

# 1: loose , 2: nominal , 3: tight , 4: y2017
