source clean.sh
rm *.root
INPUT=/data/NewSkims_gtoccbar/20241113_DataPbPb2018gtoccbar_v2_TestV0.root
./ExecuteGtoMuMu --Input $INPUT --MinJetPT 80 --Output output_analysis_gtoccbar.root
wait
