!/bin/bash

# ============================================================
# OO data
# ============================================================
source clean.sh

INPUT=/data00/kdeverea/OOsamples/Skims/output_20250728_Skim_OO_IonPhysics0_LowPtV2_250711_104114/0000_merged.root
#INPUT=samples/skim_HiForest_2025OO_LowPtCfg_2210.root
OUTPUTANALYSIS=output/20250728_skim_HiForest_2025OO_LowPtCfg_2210.root

./ExecuteChargedHadrondNdpT \
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
