source clean.sh

# Choose dataset here
VZMAX=15.0
NVTXMIN=1
CCFILTER=1
PVFILTER=1
HFE_ONLINE_MIN1=16.0
HFE_ONLINE_MIN2=-2.0
HFE_OFFLINE_MIN1=14.0
HFE_OFFLINE_MIN2=-2.0
XSEC_SD=0.3
XSEC_DD=0.0003
XSEC_HAD=1.3
XSEC_ALPHAO=0.0
SIGNALFILE="/data00/OOsamples/Skims/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root"
STARLIGHTSD="/data00/OOsamples/Skims/skim_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root"
STARLIGHTDD="/data00/OOsamples/Skims/skim_HiForest_250514_Starlight_DoubleDiffraction_OO_5362GeV_1505PR47944.root"
ALPHAO="/data00/OOsamples/Skims/skim_20250606_HiForestMiniAOD_alphaO_06172025.root"
HFE_OFFLINE_MIN1_VECTOR="6.0,8.0,10.0"
HFE_OFFLINE_MIN2_VECTOR="-2.0,-2.0,-2.0"


./MainEfficiencyExecutable \
  --Input $INPUT \
  --Output $OUTPUTANALYSIS \
  --Vzmax $VZMAX \
  --NVtxMin $NVTXMIN \
  --CCFilter $CCFILTER \
  --PVFilter $PVFILTER \
  --HFEmax_Online_min1 $HFE_ONLINE_MIN1 \
  --HFEmax_Online_min2 $HFE_ONLINE_MIN2 \
  --HFEmax_Offline_min1 $HFE_OFFLINE_MIN1 \
  --HFEmax_Offline_min2 $HFE_OFFLINE_MIN2 \
  --xsec_SD $XSEC_SD \
  --xsec_DD $XSEC_DD \
  --xsec_had $XSEC_HAD \
  --xsec_alphaO $XSEC_ALPHAO\
  --SignalFile "$SIGNALFILE" \
  --StarlightSD "$STARLIGHTSD" \
  --StarlightDD "$STARLIGHTDD" \
  --AlphaO "$ALPHAO" \
  --HFEmax_Offline_min1_vector $HFE_OFFLINE_MIN1_VECTOR \
  --HFEmax_Offline_min2_vector $HFE_OFFLINE_MIN2_VECTOR \
