#!/bin/bash

# ============================================================
# OO data
# ============================================================
source clean.sh

#INPUT=/data00/bakovacs/OOsamples/Skims/20250705_OO_394153_FIRSTLOOK_only25.root
#OUTPUTANALYSIS=output/20250705_OO_394153_FIRSTLOOK_only25_eventSel_min04_HFAND14_HFAND12.root

INPUT=/data00/bakovacs/OOsamples/Skims/20250705_OO_394153_FIRSTLOOK.root
OUTPUTANALYSIS=output/20250705_OO_394153_FIRSTLOOK_eventSel_min04_HFAND14_HFAND12.root

./ExecuteChargedHadronRAA \
  --Input $INPUT \
  --Output $OUTPUTANALYSIS \
  --IsData true \
  --IsPP false \
  --UseTrackWeight false \
  --UseEventWeight false \
  --ApplyEventSelection true \
  --MinTrackPt 0.4 \
  --OnlineHFAND 14 \
  --OfflineHFAND 12 \
  --ScaleFactor 1
