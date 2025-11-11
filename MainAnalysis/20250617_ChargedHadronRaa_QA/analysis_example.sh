
#!/bin/bash
PATHSKIM=/data00/OOsamples/Skims
#source clean.sh
# rm *.root
TRIGGER=0
ISDATA=0
DOQA=1
SCALEFACTOR=1.0

VZMAX=15.0
CCFILTER=1
PVFILTER=1
HFE_Min_1=6
HFE_Min_2=6
USEONLINEHFE=0
source clean.sh

INPUT=$PATHSKIM/skim_20250606_HiForestMiniAOD_alphaO_06172025.root
OUTPUTANALYSIS=output_aO.root
./ExecuteChargedHadronRAA \
  --Input $INPUT \
  --TriggerChoice $TRIGGER \
  --ScaleFactor $SCALEFACTOR \
  --Output $OUTPUTANALYSIS \
  --IsData $ISDATA \
  --VzMax $VZMAX \
  --CCFilter $CCFILTER \
  --PVFilter $PVFILTER \
  --IsHijing 0 \
  --HFE_min_1 $HFE_Min_1 \
  --HFE_min_2 $HFE_Min_2 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root
OUTPUTANALYSIS=output_OO.root
./ExecuteChargedHadronRAA \
  --Input $INPUT \
  --TriggerChoice $TRIGGER \
  --ScaleFactor $SCALEFACTOR \
  --Output $OUTPUTANALYSIS \
  --IsData $ISDATA \
  --VzMax $VZMAX \
  --CCFilter $CCFILTER \
  --PVFilter $PVFILTER \
  --IsHijing 1 \
  --HFE_min_1 $HFE_Min_1 \
  --HFE_min_2 $HFE_Min_2 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root
OUTPUTANALYSIS=output_SD.root
./ExecuteChargedHadronRAA \
  --Input $INPUT \
  --TriggerChoice $TRIGGER \
  --ScaleFactor $SCALEFACTOR \
  --Output $OUTPUTANALYSIS \
  --IsData $ISDATA \
  --VzMax $VZMAX \
  --CCFilter $CCFILTER \
  --PVFilter $PVFILTER \
  --IsHijing 0 \
  --HFE_min_1 $HFE_Min_1 \
  --HFE_min_2 $HFE_Min_2 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

  INPUT=$PATHSKIM/skim_HiForest_250514_Starlight_DoubleDiffraction_OO_5362GeV_1505PR47944.root
OUTPUTANALYSIS=output_DD.root
./ExecuteChargedHadronRAA \
  --Input $INPUT \
  --TriggerChoice $TRIGGER \
  --ScaleFactor $SCALEFACTOR \
  --Output $OUTPUTANALYSIS \
  --IsData $ISDATA \
  --VzMax $VZMAX \
  --CCFilter $CCFILTER \
  --PVFilter $PVFILTER \
  --IsHijing 0 \
  --HFE_min_1 $HFE_Min_1 \
  --HFE_min_2 $HFE_Min_2 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

  mv output*.root output
  


