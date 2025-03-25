default: all

all: Setup bin/MakeWeights bin/ClosureTest bin/DataMC_ClosureTest

Setup:
	mkdir -p bin
	mkdir -p img
	mkdir -p Weights


bin/MakeWeights: source/MakeWeights.cpp
	g++ source/MakeWeights.cpp -O3 -Iinclude -I$(ProjectBase)/CommonCode/include -o bin/MakeWeights `root-config --cflags --glibs`

bin/ClosureTest: source/ClosureTest.cpp
	g++ source/ClosureTest.cpp -O3 -Iinclude -I$(ProjectBase)/CommonCode/include -o bin/ClosureTest `root-config --cflags --glibs` \
	$(ProjectBase)/CommonCode/library/Messenger.o

bin/DataMC_ClosureTest: source/DataMC_ClosureTest.cpp
	g++ source/DataMC_ClosureTest.cpp -O3 -Iinclude -I$(ProjectBase)/CommonCode/include -o bin/DataMC_ClosureTest `root-config --cflags --glibs`

test: bin/MakeWeights bin/ClosureTest
	bin/MakeWeights \
	--NumFileName /data00/cbaldene/forJanice/correctedcross_0nAn_PL.root \
	--NumHistName hCrossSection \
	--DenFileName /data00/cbaldene/forJanice/correctedcross_AnAn_PYTHIA8.root \
	--DenHistName hCrossSection

	bin/ClosureTest \
	--UnweightedFileName /data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/SkimsMC/20250213_v4_Pthat0_ForcedD0DecayD0Filtered_BeamA/mergedfile.root

	bin/ClosureTest \
	--UnweightedFileName /data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/SkimsMC/20250227_v4_Pthat0_ForcedD0DecayD0Filtered_BeamB/mergedfile.root \
	--IsGammaN false

	bin/DataMC_ClosureTest \
	--UnweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2-2_IsGammaN1/MC.root \
	--TargetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2-2_IsGammaN1/Data.root \
	--PlotDir pt2-5_y-2-2_IsGammaN1 --IsGammaN true

	bin/DataMC_ClosureTest \
	--UnweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2-2_IsGammaN0/MC.root \
	--TargetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2-2_IsGammaN0/Data.root \
	--PlotDir pt2-5_y-2-2_IsGammaN0 --IsGammaN false
