TrackPTMIN=0.2
TrackPTMAX=1000000
TrackYMIN=-2.4
TrackYMAX=+2.4
PtSumMIN=1.0
PtSumMAX=10000
MtSumMIN=0.0
MtSumMAX=10000
USEPtSumHardScale=1
TRIGGER=0 # no trigger selection
ISGAMMAN=0
ISDATA=0
SCALEFACTOR=1.0
#INPUT=/data/HFJetUPCanalysis/MCSkims23/20250310_ForestHFJetMC23_D0Tagged_v9_ModTrackCuts_pthat5_BeamAabdBCombined.root
INPUT=/data/HFJetUPCanalysis/MCSkims23/20250602_ForestHFJetMC23_D0Tagged_v2_pthat0_BeamAandBCombined_SmallSample.root
OUTPUTANALYSIS=output_MC_pthat0_BeamAandBcombined_TrackPtCut020__PtSum0_10000_v5.root


./ExecuteHFUPCEEC --Input $INPUT --MinTrackPT $TrackPTMIN --MaxTrackPT $TrackPTMAX --MinTrackY $TrackYMIN --MaxTrackY $TrackYMAX --MinPtSum $PtSumMIN --MaxPtSum $PtSumMAX --MinMtSum $MtSumMIN --MaxMtSum $MtSumMAX --UsePtSumHardScale $USEPtSumHardScale  --IsGammaN $ISGAMMAN --TriggerChoice $TRIGGER --IsData $ISDATA --Output $OUTPUTANALYSIS --scaleFactor $SCALEFACTOR
wait
