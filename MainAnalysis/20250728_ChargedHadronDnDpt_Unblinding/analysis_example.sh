!/bin/bash

# ============================================================
# OO data
# ============================================================
source clean.sh

INPUT=samples/skim_HiForest_2025OO_LowPtCfg_2210.root
OUTPUTANALYSIS=output/20250728_skim_HiForest_2025OO_LowPtCfg_2210.root

./ExecuteChargedHadrondNdpt \
  --Input $INPUT \
  --Output $OUTPUTANALYSIS \
  --IsData true \
  --IsPP true \
  --ApplyEventSelection true \
  --UseEventWeight false \
  --UseTrackWeight false \
  --TrackWeightSelection 2 \
  --MinTrackPt 0.1 \
  --MinLeadingTrackPt -1 \
  --ScaleFactor 1

# 1: loose , 2: nominal , 3: tight , 4: y2017
