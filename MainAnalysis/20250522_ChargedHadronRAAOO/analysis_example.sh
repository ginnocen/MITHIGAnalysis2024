
#!/bin/bash
## FIXME: need to use your own path to the skimmed data
PATHSKIM=/Users/ginnocen/Desktop/MITHIGAnalysis2024/SampleGeneration/20250518_ForestReducer_RAAOxygenOxygen/data00/OOsamples/Skims
#source clean.sh
# rm *.root
TRACKPTMIN=1
TRIGGER=0
ISDATA=0
SCALEFACTOR=1.0
source clean.sh

INPUT=$PATHSKIM/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root
OUTPUTANALYSIS=output.root
./ExecuteChargedHadronRAA \
  --Input $INPUT \
  --TRACKPTMIN $TRACKPTMIN \
  --TriggerChoice $TRIGGER \
  --ScaleFactor $SCALEFACTOR \
  --IsData $ISDATA \
  --Output $OUTPUTANALYSIS
