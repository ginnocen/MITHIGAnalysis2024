
#!/bin/bash
PATHSKIM=/data00/OOsamples/Skims
HIJING_INPUT=$PATHSKIM/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root
DD_INPUT=$PATHSKIM/skim_HiForest_250514_Starlight_DoubleDiffraction_OO_5362GeV_1505PR47944.root
SD_INPUT=$PATHSKIM/skim_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root
ALPHAO_INPUT=$PATHSKIM/skim_Debug20250610_HiForestMiniAOD_alphaO_10k_NpartFix.root
#source clean.sh
# rm *.root
TRIGGER=0
ISDATA=0
SCALEFACTOR=1.0

VZMAX=15.0
NVTXMIN=1
CCFILTER=1
PVFILTER=1
HFE_min_1=4.0
HFE_min_2=4.0
source clean.sh


HIJING_OUTPUTANALYSIS=output_hijing.root
./ExecuteChargedHadronRAA \
  --Input $HIJING_INPUT \
  --TriggerChoice $TRIGGER \
  --ScaleFactor $SCALEFACTOR \
  --Output $HIJING_OUTPUTANALYSIS \
  --IsData $ISDATA \
  --VzMax $VZMAX \
  --NvtxMin $NVTXMIN \
  --CCFilter $CCFILTER \
  --PVFilter $PVFILTER \
  --IsHijing 1 \
  --HFE_min_1 $HFE_MIN_1 \
  --HFE_min_2 $HFE_MIN_2 \

DD_OUTPUTANALYSIS=output_dd.root
./ExecuteChargedHadronRAA \
  --Input $DD_INPUT \
  --TriggerChoice $TRIGGER \
  --ScaleFactor $SCALEFACTOR \
  --Output $DD_OUTPUTANALYSIS \
  --IsData $ISDATA \
  --VzMax $VZMAX \
  --NvtxMin $NVTXMIN \
  --CCFilter $CCFILTER \
  --PVFilter $PVFILTER \
  --IsHijing 0 \
  --HFE_min_1 $HFE_MIN_1 \
  --HFE_min_2 $HFE_MIN_2 \

SD_OUTPUTANALYSIS=output_sd.root
./ExecuteChargedHadronRAA \
  --Input $SD_INPUT \
  --TriggerChoice $TRIGGER \
  --ScaleFactor $SCALEFACTOR \
  --Output $SD_OUTPUTANALYSIS \
  --IsData $ISDATA \
  --VzMax $VZMAX \
  --NvtxMin $NVTXMIN \
  --CCFilter $CCFILTER \
  --PVFilter $PVFILTER \
  --IsHijing 0 \
  --HFE_min_1 $HFE_MIN_1 \
  --HFE_min_2 $HFE_MIN_2 \

ALPHAO_OUTPUTANALYSIS=output_alphaO.root
./ExecuteChargedHadronRAA \
  --Input $ALPHAO_INPUT \
  --TriggerChoice $TRIGGER \
  --ScaleFactor $SCALEFACTOR \
  --Output $ALPHAO_OUTPUTANALYSIS \
  --IsData $ISDATA \
  --VzMax $VZMAX \
  --NvtxMin $NVTXMIN \
  --CCFilter $CCFILTER \
  --PVFilter $PVFILTER \
  --IsHijing 0 \
  --HFE_min_1 $HFE_MIN_1 \
  --HFE_min_2 $HFE_MIN_2 \
  


