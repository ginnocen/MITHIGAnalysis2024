
#!/bin/bash
## FIXME: need to use your own path to the skimmed data
#PATHSKIM=/afs/cern.ch/work/g/ginnocen/public/OOsamples/Skims
PATHSKIM=/data00/kdeverea/OOsamples/Skims/

TRACKPTMIN=1
TRIGGER=0
ISDATA=1
SCALEFACTOR=1.0
source clean.sh

INPUT=$PATHSKIM/20250626_Skim_ppref2024.root

OUTPUTANALYSIS=output.root
./ExecuteChargedHadronRAA \
  --Input $INPUT \
  --IsData $ISDATA \
  --Output $OUTPUTANALYSIS \
  --ScaleFactor $SCALEFACTOR

# with track efficiency correction weight applied
OUTPUTANALYSIS=output_trackCor.root
./ExecuteChargedHadronRAA \
  --Input $INPUT \
  --IsData $ISDATA \
  --Output $OUTPUTANALYSIS \
  --ScaleFactor $SCALEFACTOR \
  --UseTrackWeight true
