
##### SPECIFY INPUT FILES HERE#####
DataFile="/data00/kdeverea/OOsamples/Skims/output_20250724_Skim_OO_IonPhysics0_LowPtV2_250711_104114/0000/output_74.root"
OOFile="/data00/kdeverea/OOsamples/Skims/output_20250725_Skim_OO_MinBias_HIJING_5362GeV/20250725_Skim_OO_MinBias_HIJING_5362GeV.root"
OOFile_Arg="/data00/kdeverea/OOsamples/Skims/output_20250724_Skim_MinBias_Pythia_Angantyr_OO_5362GeV/20250724_Skim_MinBias_Pythia_Angantyr_OO_5362GeV.root"
    
###### SPECIFY EVENT SELECTION CUT HERE ######
MCCUT="(VZ > -15 && VZ < 15) && \
    (PVFilter == 1) && \
    (ClusterCompatibilityFilter == 1) && \
    (HFEMaxPlus > 19.000000 && HFEMaxMinus > 19.000000) && \
    (HLT_MinimumBiasHF_OR_BptxAND_v1)"

DATACUT="(VZ > -15 && VZ < 15) && \
    (PVFilter == 1) && \
    (ClusterCompatibilityFilter == 1) && \
    (HFEMaxPlus > 19.000000 && HFEMaxMinus > 19.000000) && \
    (HLT_MinimumBiasHF_OR_BptxAND_v1)" 


HISTOGRAMS_DESTINATION="hists/output.root"
EFFICIENCY_DESTINATION="hists/OORAA_MULT_EFFICIENCY_HIJING_HF19AND.root"

#### REWEIGHTING #####
# last argument is 1 to compile histogram while considering previous weighting 0 to ignore
# for example, putting a 0 in TrkPtReweight and a 1 in MultReweight will compile the multiplicity 
# reweighting after applying the VZ reweighting, but will compile the track pT reweighting without 
# the multiplicity reweighting. Default to 1 for all macros for full reweighting.
cd VZReweight
root -l -b -q "VZReweight.C(\"${DATACUT}\",\"${MCCUT}\",\"${DataFile}\",\"${OOFile}\",\"${OOFile_Arg}\",1)" 
cd ..

cd MultReweight
root -l -b -q "MultReweight.C(\"${DATACUT}\",\"${MCCUT}\",\"${DataFile}\",\"${OOFile}\",\"${OOFile_Arg}\",1)" 
cd .. 

cd TrkPtReweight
root -l -b -q "TrkPtReweight.C(\"${DATACUT}\",\"${MCCUT}\",\"${DataFile}\",\"${OOFile}\",\"${OOFile_Arg}\",1)" 
cd ..

###### EFFICIENCY GENERATION ######
root -l -b -q "histmaker.C(\"${DATACUT}\",\"${MCCUT}\",\"${DataFile}\",\"${OOFile}\",\"${OOFile_Arg}\", 1, 1, 1, \"${HISTOGRAMS_DESTINATION}\")" 

###### CORRECTION GENERATION ######
root -l -b -q "CorrectedPtDist.C(\"${DATACUT}\",\"${DataFile}\",  \"${HISTOGRAMS_DESTINATION}\", \"hOO_Mult_Eff\",1)" 

###### FILE SAVER ######
root -l -b -q "FileSaver.C(\"${DATACUT}\",\"${MCCUT}\",\"${DataFile}\",\"${OOFile}\",\"${OOFile_Arg}\",\"${HISTOGRAMS_DESTINATION}\",\"hOO_Mult_Eff\",\"${EFFICIENCY_DESTINATION}\")" 

echo "Execution completed successfully :) Histograms saved to ${HISTOGRAMS_DESTINATION} and efficiency saved to ${EFFICIENCY_DESTINATION}"