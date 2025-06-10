TrackPTMIN=0.2
TrackPTMAX=1000000
TrackYMIN=-5.0
TrackYMAX=5.0
HardScaleMIN=5.0
HardScaleMAX=100.0
USEPtSumHardScale=1
TRIGGER=0 # no trigger selection
ISGAMMAN=0
ISDATA=0
SCALEFACTOR=0.1
INPUT=/data/HFJetUPCanalysis/MCSkims23/20250602_ForestHFJetMC23_D0Tagged_v2_pthat5_SmallSample.root
OUTPUTANALYSIS=output_MC_pthat5_BeamAandBcombined_TrackPtCut020_5_100_WithGammaNTest.root


./ExecuteUPCEEC --Input $INPUT --MinTrackPT $TrackPTMIN --MaxTrackPT $TrackPTMAX --MinTrackY $TrackYMIN --MaxTrackY $TrackYMAX --MinHardScale $HardScaleMIN --MaxHardScale $HardScaleMAX --UsePtSumHardScale $USEPtSumHardScale  --IsGammaN $ISGAMMAN --TriggerChoice $TRIGGER --IsData $ISDATA --Output $OUTPUTANALYSIS --scaleFactor $SCALEFACTOR
wait
