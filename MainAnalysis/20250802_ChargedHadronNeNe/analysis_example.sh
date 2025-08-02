!/bin/bash

# ============================================================
# OO data
# ============================================================
source clean.sh


INPUT=/data00/cbaldene/forNeNe/tempOO.root
OUTPUTANALYSIS=output/output.root

./ExecuteChargedHadrondNdpt \
  --Input $INPUT \
  --Output $OUTPUTANALYSIS \
  --IsData true \
  --ApplyEventSelection true \
  --UseSpeciesWeight true \
  --UseEventWeight true \
  --UseTrackWeight true \
  --TrackWeightSelection 3 \
  --MinTrackPt 0.4 \
  --MinLeadingTrackPt -1 \
  --TriggerChoice 1 \
  --CollisionSystem true \
  --EventSelectionOption 1 \
  --SpeciesCorrectionOption 0 \
  --ScaleFactor 1

#TrackWeightSelection: 1: loose , 2: nominal , 3: tight
#CollysionSystem: true: OO/NeNe, false: ppRef
#TriggerChoice: 0: ZB,  1: HFOR
#EventSelection Option: 1: loose, 2: nominal, 3: tight
#SpeciesCorrectionOption: 1: ppRef, 2: dNdeta=40 (OO-like), 3: dNdeta = 100 (central OO)
