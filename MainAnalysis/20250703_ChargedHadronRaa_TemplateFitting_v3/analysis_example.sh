
#!/bin/bash
PATHSKIM=/data00/OOsamples/Skims20250629
#source clean.sh
# rm *.root

VZMAX=15.0
CCFILTER=1
PVFILTER=1
MINTRACKPT=0
HFE_Min_1=0
HFE_Min_2=0
ADC_Min_1=14
ADC_Min_2=0
source clean.sh

INPUT=$PATHSKIM/skim_20250606_HiForestMiniAOD_alphaO_06172025.root
OUTPUTANALYSIS=template_aO.root
./ExecuteChargedHadronRAA \
  --Input $INPUT \
  --Output $OUTPUTANALYSIS \
  --VzMax $VZMAX \
  --CCFilter $CCFILTER \
  --PVFilter $PVFILTER \
  --MinTrackPt $MINTRACKPT \
  --HFE_min_1 $HFE_Min_1 \
  --HFE_min_2 $HFE_Min_2 \
  --ADC_min_1 $ADC_Min_1 \
  --ADC_min_2 $ADC_Min_2 \

INPUT=$PATHSKIM/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root
OUTPUTANALYSIS=template_OO.root
./ExecuteChargedHadronRAA \
  --Input $INPUT \
  --Output $OUTPUTANALYSIS \
  --VzMax $VZMAX \
  --CCFilter $CCFILTER \
  --PVFilter $PVFILTER \
  --MinTrackPt $MINTRACKPT \
  --HFE_min_1 $HFE_Min_1 \
  --HFE_min_2 $HFE_Min_2 \
  --ADC_min_1 $ADC_Min_1 \
  --ADC_min_2 $ADC_Min_2 \

INPUT=$PATHSKIM/skim_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root
OUTPUTANALYSIS=template_SD.root
./ExecuteChargedHadronRAA \
  --Input $INPUT \
  --Output $OUTPUTANALYSIS \
  --VzMax $VZMAX \
  --CCFilter $CCFILTER \
  --PVFilter $PVFILTER \
  --MinTrackPt $MINTRACKPT \
  --HFE_min_1 $HFE_Min_1 \
  --HFE_min_2 $HFE_Min_2 \
  --ADC_min_1 $ADC_Min_1 \
  --ADC_min_2 $ADC_Min_2 \

  INPUT=$PATHSKIM/skim_HiForest_250514_Starlight_DoubleDiffraction_OO_5362GeV_1505PR47944.root
OUTPUTANALYSIS=template_DD.root
./ExecuteChargedHadronRAA \
  --Input $INPUT \
  --Output $OUTPUTANALYSIS \
  --VzMax $VZMAX \
  --CCFilter $CCFILTER \
  --PVFilter $PVFILTER \
  --MinTrackPt $MINTRACKPT \
  --HFE_min_1 $HFE_Min_1 \
  --HFE_min_2 $HFE_Min_2 \
  --ADC_min_1 $ADC_Min_1 \
  --ADC_min_2 $ADC_Min_2 \
  
mv template_*.root templates
  


