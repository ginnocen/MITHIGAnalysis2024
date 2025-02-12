INPUT=output_MC_JetPtvPtHat_pthat8s.root

#./ExecutePlotPtHat --Input "output_MC_JetPtvPtHat_pthat2.root","output_MC_JetPtvPtHat_pthat5.root","output_MC_JetPtvPtHat_pthat8.root","output_MC_JetPtvPtHat_pthat10.root","output_MC_JetPtvPtHat_pthat20.root","output_MC_JetPtvPtHat_pthat30.root" --tag "pThatOverlay" --Labels "#hat{p}_{T} > 2 GeV","#hat{p}_{T} > 5 GeV","#hat{p}_{T} > 8 GeV","#hat{p}_{T} > 10 GeV","#hat{p}_{T} >  20 GeV","#hat{p}_{T} > 30 GeV"


./ExecutePlotHFJet --Input $INPUT --tag "JustPtHat8"

#./ExecutePlotDoFragFunction --Input "output_JetPtMin15_D0pTMin5.root","output_JetPtMin15_Max30_D0pTMin5.root","output_JetPtMin30_Max50_D0pTMin5.root","output_JetPtMin50_Max100_D0pTMin5.root" --tag "DataWithPtCuts_V2"

#./ExecutePlotDoFragFunction --Input "output_MC_JetPtMin15_D0pTMin5.root","output_MC_JetPtMin15_MaxPt30_D0pTMin5.root","output_MC_JetPtMin30_MaxPt50_D0pTMin5.root","output_MC_JetPtMin50_MaxPt100_D0pTMin5.root" --tag "MCWithPtCuts_V2"