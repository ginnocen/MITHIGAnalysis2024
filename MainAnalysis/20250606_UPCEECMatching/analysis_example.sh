TrackPTMIN=0.2
TrackPTMAX=1000000
TrackYMIN=-5.0
TrackYMAX=+5.0
HardScaleMIN=5.0
HardScaleMAX=100.0
USEPtSumHardScale=1
TRIGGER=1 # ZDC OR
ISGAMMAN=0
ISDATA=1
SCALEFACTOR=0.35
INPUT=/data/HFJetUPCanalysis/DataSkims23/20250602_ForestHFJetData23Skim_2023UPCJanRereco_v9_SmallSample.root
OUTPUTANALYSIS=output_Data_EEC_TrackPtCut020_HardScaleCut5_100_WithGammaNTest.root


./ExecuteUPCEEC --Input $INPUT --MinTrackPT $TrackPTMIN --MaxTrackPT $TrackPTMAX --MinTrackY $TrackYMIN --MaxTrackY $TrackYMAX --MinHardScale $HardScaleMIN --MaxHardScale $HardScaleMAX --UsePtSumHardScale $USEPtSumHardScale  --IsGammaN $ISGAMMAN --TriggerChoice $TRIGGER --IsData $ISDATA --Output $OUTPUTANALYSIS --scaleFactor $SCALEFACTOR

