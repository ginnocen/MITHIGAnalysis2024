echo "START"

# Enable ROOT multithreading 
export ROOT_ENABLE_MT=1
# Use only 10 cores (adjust this number as needed)
export OMP_NUM_THREADS=20

##### SPECIFY INPUT FILES HERE#####
MAIN_DIR=$(pwd)
DataSource="${MAIN_DIR}/datafiles.txt"
#OOFile="/data00/kdeverea/OOsamples/Skims/output_20250728_Skim_OO_MinBias_OO_5p36TeV_hijing/20250728_Skim_OO_MinBias_OO_5p36TeV_hijing.root"
#OOFile_Arg="/data00/kdeverea/OOsamples/Skims/output_20250724_Skim_MinBias_Pythia_Angantyr_OO_5362GeV/20250724_Skim_MinBias_Pythia_Angantyr_OO_5362GeV.root"
OOFile="/data00/kdeverea/OOsamples/Skims/output_20250805_Skim_MinBias_Hijing_NeNe_5362GeV/merged_all.root"
OOFile_Arg="/data00/kdeverea/OOsamples/Skims/output_20250724_Skim_MinBias_Pythia_Angantyr_OO_5362GeV/20250724_Skim_MinBias_Pythia_Angantyr_OO_5362GeV.root"

###### SPECIFY EVENT SELECTION CUT HERE ######
MCCUT="(VZ > -15 && VZ < 15) && \
    (PVFilter == 1) && \
    (ClusterCompatibilityFilter == 1) && \
    (HFEMaxPlus > 19.000000 && HFEMaxMinus > 19.000000)"


DATACUT="(VZ > -15 && VZ < 15) && \
    (PVFilter == 1) && \
    (ClusterCompatibilityFilter == 1) && \
    (HFEMaxPlus > 19.000000 && HFEMaxMinus > 19.000000) && \
    (HLT_MinimumBiasHF_OR_BptxAND_v1)" 


HISTOGRAMS_DESTINATION="hists/nene_output_19.root"
EFFICIENCY_DESTINATION="hists/NENERAA_MULT_EFFICIENCY_HIJING_HF19AND.root"

#### REWEIGHTING #####
# last argument is 1 to compile histogram while considering previous weighting 0 to ignore
# for example, putting a 0 in TrkPtReweight and a 1 in MultReweight will compile the multiplicity 
# reweighting after applying the VZ reweighting, but will compile the track pT reweighting without 
# the multiplicity reweighting. Default to 1 for all macros for full reweighting.

cd VZReweight
root -l -b -q "VZReweight.C(\"${DATACUT}\",\"${MCCUT}\",\"${DataSource}\",\"${OOFile}\",\"${OOFile_Arg}\",1)"
cd ..

cd MultReweight
root -l -b -q "MultReweight.C(\"${DATACUT}\",\"${MCCUT}\",\"${DataSource}\",\"${OOFile}\",\"${OOFile_Arg}\",1)"
cd .. 

cd TrkPtReweight
root -l -b -q "TrkPtReweight.C(\"${DATACUT}\",\"${MCCUT}\",\"${DataSource}\",\"${OOFile}\",\"${OOFile_Arg}\",1)"
cd ..

###### EFFICIENCY GENERATION ######
root -l -b -q "histmaker.C(\"${DATACUT}\",\"${MCCUT}\",\"${DataSource}\",\"${OOFile}\",\"${OOFile_Arg}\", 1, 1, 1, \"${HISTOGRAMS_DESTINATION}\")" 

###### CORRECTION GENERATION ######
root -l -b -q "CorrectedPtDist.C(\"${DATACUT}\",\"${DataSource}\",  \"${HISTOGRAMS_DESTINATION}\", \"hOO_Mult_Eff\",1)" 

###### FILE SAVER ######
root -l -b -q "FileSaver.C(\"${DATACUT}\",\"${MCCUT}\",\"${DataSource}\",\"${OOFile}\",\"${OOFile_Arg}\",\"${HISTOGRAMS_DESTINATION}\",\"hOO_Mult_Eff\",\"${EFFICIENCY_DESTINATION}\")" 

echo "=============================================="
echo "WEIGHTS ARE READY"
echo "Histograms saved to ${HISTOGRAMS_DESTINATION}"
echo "Efficiency saved to ${EFFICIENCY_DESTINATION}"
echo "=============================================="
