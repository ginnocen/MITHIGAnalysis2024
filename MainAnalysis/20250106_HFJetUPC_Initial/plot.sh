INPUT=output_MC_JetPtMin15_D0pTMin5.root

./ExecutePlotHFJet --Input $INPUT --tag "MCWithPtCuts"

./ExecutePlotDoFragFunction --Input "output_JetPtMin15_D0pTMin5.root","output_JetPtMin15_Max30_D0pTMin5.root","output_JetPtMin30_Max50_D0pTMin5.root","output_JetPtMin50_Max100_D0pTMin5.root" --tag "DataWithPtCuts_V2"

./ExecutePlotDoFragFunction --Input "output_MC_JetPtMin15_D0pTMin5.root","output_MC_JetPtMin15_MaxPt30_D0pTMin5.root","output_MC_JetPtMin30_MaxPt50_D0pTMin5.root","output_MC_JetPtMin50_MaxPt100_D0pTMin5.root" --tag "MCWithPtCuts_V2"