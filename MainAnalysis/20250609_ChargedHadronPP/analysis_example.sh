#!/bin/bash

# ============================================================
# OO data
# ============================================================
source clean.sh

INPUT=/data00/kdeverea/OOsamples/Skims/20250723_Skim_ppref2024_Official_noEvtSel.root
OUTPUTANALYSIS=output/20250723_Skim_ppref2024_Official_noEvtSel.root

./ExecuteChargedHadronRAA \
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
