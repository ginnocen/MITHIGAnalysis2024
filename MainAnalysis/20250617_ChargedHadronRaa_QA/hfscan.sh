
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
USEONLINEHFE=0
source clean.sh

INPUT=$PATHSKIM/skim_20250606_HiForestMiniAOD_alphaO_06172025.root
OUTPUTANALYSIS=output_aO_NOHF.root
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
  --HFE_min_1 0 \
  --HFE_min_2 0 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root
OUTPUTANALYSIS=output_OO_NOHF.root
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
  --HFE_min_1 0 \
  --HFE_min_2 0 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root
OUTPUTANALYSIS=output_SD_NOHF.root
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
  --HFE_min_1 0 \
  --HFE_min_2 0 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250514_Starlight_DoubleDiffraction_OO_5362GeV_1505PR47944.root
OUTPUTANALYSIS=output_DD_NOHF.root
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
  --HFE_min_1 0 \
  --HFE_min_2 0 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_20250606_HiForestMiniAOD_alphaO_06172025.root
OUTPUTANALYSIS=output_aO_4OR.root
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
  --HFE_min_1 4 \
  --HFE_min_2 0 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root
OUTPUTANALYSIS=output_OO_4OR.root
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
  --HFE_min_1 4 \
  --HFE_min_2 0 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root
OUTPUTANALYSIS=output_SD_4OR.root
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
  --HFE_min_1 4 \
  --HFE_min_2 0 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250514_Starlight_DoubleDiffraction_OO_5362GeV_1505PR47944.root
OUTPUTANALYSIS=output_DD_4OR.root
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
  --HFE_min_1 4 \
  --HFE_min_2 0 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_20250606_HiForestMiniAOD_alphaO_06172025.root
OUTPUTANALYSIS=output_aO_4AND.root
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
  --HFE_min_1 4 \
  --HFE_min_2 4 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root
OUTPUTANALYSIS=output_OO_4AND.root
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
  --HFE_min_1 4 \
  --HFE_min_2 4 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root
OUTPUTANALYSIS=output_SD_4AND.root
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
  --HFE_min_1 4 \
  --HFE_min_2 4 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250514_Starlight_DoubleDiffraction_OO_5362GeV_1505PR47944.root
OUTPUTANALYSIS=output_DD_4AND.root
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
  --HFE_min_1 4 \
  --HFE_min_2 4 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_20250606_HiForestMiniAOD_alphaO_06172025.root
OUTPUTANALYSIS=output_aO_6OR.root
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
  --HFE_min_1 6 \
  --HFE_min_2 0 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root
OUTPUTANALYSIS=output_OO_6OR.root
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
  --HFE_min_1 6 \
  --HFE_min_2 0 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root
OUTPUTANALYSIS=output_SD_6OR.root
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
  --HFE_min_1 6 \
  --HFE_min_2 0 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250514_Starlight_DoubleDiffraction_OO_5362GeV_1505PR47944.root
OUTPUTANALYSIS=output_DD_6OR.root
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
  --HFE_min_1 6 \
  --HFE_min_2 0 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_20250606_HiForestMiniAOD_alphaO_06172025.root
OUTPUTANALYSIS=output_aO_6AND.root
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
  --HFE_min_1 6 \
  --HFE_min_2 6 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root
OUTPUTANALYSIS=output_OO_6AND.root
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
  --HFE_min_1 6 \
  --HFE_min_2 6 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root
OUTPUTANALYSIS=output_SD_6AND.root
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
  --HFE_min_1 6 \
  --HFE_min_2 6 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250514_Starlight_DoubleDiffraction_OO_5362GeV_1505PR47944.root
OUTPUTANALYSIS=output_DD_6AND.root
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
  --HFE_min_1 6 \
  --HFE_min_2 6 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_20250606_HiForestMiniAOD_alphaO_06172025.root
OUTPUTANALYSIS=output_aO_8OR.root
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
  --HFE_min_1 8 \
  --HFE_min_2 0 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root
OUTPUTANALYSIS=output_OO_8OR.root
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
  --HFE_min_1 8 \
  --HFE_min_2 0 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root
OUTPUTANALYSIS=output_SD_8OR.root
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
  --HFE_min_1 8 \
  --HFE_min_2 0 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250514_Starlight_DoubleDiffraction_OO_5362GeV_1505PR47944.root
OUTPUTANALYSIS=output_DD_8OR.root
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
  --HFE_min_1 8 \
  --HFE_min_2 0 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_20250606_HiForestMiniAOD_alphaO_06172025.root
OUTPUTANALYSIS=output_aO_8AND.root
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
  --HFE_min_1 8 \
  --HFE_min_2 8 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root
OUTPUTANALYSIS=output_OO_8AND.root
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
  --HFE_min_1 8 \
  --HFE_min_2 8 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root
OUTPUTANALYSIS=output_SD_8AND.root
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
  --HFE_min_1 8 \
  --HFE_min_2 8 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \

INPUT=$PATHSKIM/skim_HiForest_250514_Starlight_DoubleDiffraction_OO_5362GeV_1505PR47944.root
OUTPUTANALYSIS=output_DD_8AND.root
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
  --HFE_min_1 8 \
  --HFE_min_2 8 \
  --useOnlineHFE $USEONLINEHFE \
  --doQA $DOQA \



  mv output*.root output
