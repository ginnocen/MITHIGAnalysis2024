default: all

all: Setup bin/MakeWeights bin/ClosureTest bin/GenSpectra

Setup:
	mkdir -p bin
	mkdir -p img
	mkdir -p Weights


bin/MakeWeights: source/MakeWeights.cpp
	g++ source/MakeWeights.cpp -O3 -Iinclude -I$(ProjectBase)/WeightHandler/20250305_DzeroUPC_GptGyWeight/include -I$(ProjectBase)/CommonCode/include -o bin/MakeWeights `root-config \
	--cflags --glibs`

bin/ClosureTest: source/ClosureTest.cpp
	g++ source/ClosureTest.cpp -O3 -Iinclude -I$(ProjectBase)/WeightHandler/20250305_DzeroUPC_GptGyWeight/include -I$(ProjectBase)/CommonCode/include -o bin/ClosureTest `root-config --cflags --glibs`

bin/GenSpectra: source/GenSpectra.cpp
	g++ source/GenSpectra.cpp -O3 -Iinclude -I$(ProjectBase)/WeightHandler/20250305_DzeroUPC_GptGyWeight/include -I$(ProjectBase)/CommonCode/include -o bin/GenSpectra `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

test: bin/MakeWeights
	bin/MakeWeights \
	--UnweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN1/MC.root \
	--TargetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN1/Data.root \
	--TreeName ntReweighting \
	--WeightFileName Weights/pt2-5_y-2-2_IsGammaN1.root \
	--IsGammaN 1

	bin/MakeWeights \
	--UnweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN1/MC.root \
	--TargetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN1/Data.root \
	--TreeName ntReweighting \
	--WeightFileName Weights/pt2-5_y-2--1_IsGammaN1.root \
	--IsGammaN 1

	bin/MakeWeights \
	--UnweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-1-0_IsGammaN1/MC.root \
	--TargetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-1-0_IsGammaN1/Data.root \
	--TreeName ntReweighting \
	--WeightFileName Weights/pt2-5_y-1-0_IsGammaN1.root \
	--IsGammaN 1

	bin/MakeWeights \
	--UnweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y0-1_IsGammaN1/MC.root \
	--TargetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y0-1_IsGammaN1/Data.root \
	--TreeName ntReweighting \
	--WeightFileName Weights/pt2-5_y0-1_IsGammaN1.root \
	--IsGammaN 1

	bin/MakeWeights \
	--UnweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y1-2_IsGammaN1/MC.root \
	--TargetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y1-2_IsGammaN1/Data.root \
	--TreeName ntReweighting \
	--WeightFileName Weights/pt2-5_y1-2_IsGammaN1.root \
	--IsGammaN 1

	bin/MakeWeights \
	--UnweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN0/MC.root \
	--TargetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN0/Data.root \
	--TreeName ntReweighting \
	--WeightFileName Weights/pt2-5_y-2-2_IsGammaN0.root \
	--IsGammaN 0

	bin/MakeWeights \
	--UnweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN0/MC.root \
	--TargetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN0/Data.root \
	--TreeName ntReweighting \
	--WeightFileName Weights/pt2-5_y-2--1_IsGammaN0.root \
	--IsGammaN 0

	bin/MakeWeights \
	--UnweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-1-0_IsGammaN0/MC.root \
	--TargetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-1-0_IsGammaN0/Data.root \
	--TreeName ntReweighting \
	--WeightFileName Weights/pt2-5_y-1-0_IsGammaN0.root \
	--IsGammaN 0

	bin/MakeWeights \
	--UnweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y0-1_IsGammaN0/MC.root \
	--TargetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y0-1_IsGammaN0/Data.root \
	--TreeName ntReweighting \
	--WeightFileName Weights/pt2-5_y0-1_IsGammaN0.root \
	--IsGammaN 0

	bin/MakeWeights \
	--UnweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y1-2_IsGammaN0/MC.root \
	--TargetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y1-2_IsGammaN0/Data.root \
	--TreeName ntReweighting \
	--WeightFileName Weights/pt2-5_y1-2_IsGammaN0.root \
	--IsGammaN 0

	bin/ClosureTest \
	--UnweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN1/MC.root \
	--TargetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN1/Data.root \
	--WeightFileName Weights/pt2-5_y-2--1_IsGammaN1.root \
	--PlotDir pt2-5_y-2--1_IsGammaN1 --IsGammaN 1

	bin/ClosureTest \
	--UnweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-1-0_IsGammaN1/MC.root \
	--TargetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-1-0_IsGammaN1/Data.root \
	--WeightFileName Weights/pt2-5_y-1-0_IsGammaN1.root \
	--PlotDir pt2-5_y-1-0_IsGammaN1 --IsGammaN 1

	bin/ClosureTest \
	--UnweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y0-1_IsGammaN1/MC.root \
	--TargetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y0-1_IsGammaN1/Data.root \
	--WeightFileName Weights/pt2-5_y0-1_IsGammaN1.root \
	--PlotDir pt2-5_y0-1_IsGammaN1 --IsGammaN 1

	bin/ClosureTest \
	--UnweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y1-2_IsGammaN1/MC.root \
	--TargetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y1-2_IsGammaN1/Data.root \
	--WeightFileName Weights/pt2-5_y1-2_IsGammaN1.root \
	--PlotDir pt2-5_y1-2_IsGammaN1 --IsGammaN 1


	bin/ClosureTest \
	--UnweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN0/MC.root \
	--TargetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN0/Data.root \
	--WeightFileName Weights/pt2-5_y-2--1_IsGammaN0.root \
	--PlotDir pt2-5_y-2--1_IsGammaN0 --IsGammaN 0

	bin/ClosureTest \
	--UnweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-1-0_IsGammaN0/MC.root \
	--TargetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-1-0_IsGammaN0/Data.root \
	--WeightFileName Weights/pt2-5_y-1-0_IsGammaN0.root \
	--PlotDir pt2-5_y-1-0_IsGammaN0 --IsGammaN 0

	bin/ClosureTest \
	--UnweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y0-1_IsGammaN0/MC.root \
	--TargetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y0-1_IsGammaN0/Data.root \
	--WeightFileName Weights/pt2-5_y0-1_IsGammaN0.root \
	--PlotDir pt2-5_y0-1_IsGammaN0 --IsGammaN 0

	bin/ClosureTest \
	--UnweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y1-2_IsGammaN0/MC.root \
	--TargetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y1-2_IsGammaN0/Data.root \
	--WeightFileName Weights/pt2-5_y1-2_IsGammaN0.root \
	--PlotDir pt2-5_y1-2_IsGammaN0 --IsGammaN 0

	bin/GenSpectra \
  --UnweightedFileName /data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/SkimsMC/20250213_v4_Pthat0_ForcedD0DecayD0Filtered_BeamA/mergedfile.root \
  --WeightFileName Weights/pt2-5_y-2-2_IsGammaN1.root

	bin/GenSpectra \
  --UnweightedFileName /data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/SkimsMC/20250213_v4_Pthat0_ForcedD0DecayD0Filtered_BeamA/mergedfile.root \
  --WeightFileName Weights/pt2-5_y-2-2_IsGammaN1.root \
  --DoGptGyReweighting false

	bin/GenSpectra \
  --UnweightedFileName /data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/SkimsMC/20250213_v4_Pthat0_ForcedD0DecayD0Filtered_BeamA/mergedfile.root \
  --WeightFileName Weights/pt2-5_y-2-2_IsGammaN0.root \
  --IsgammaN 0

	bin/GenSpectra \
  --UnweightedFileName /data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/SkimsMC/20250213_v4_Pthat0_ForcedD0DecayD0Filtered_BeamA/mergedfile.root \
  --WeightFileName Weights/pt2-5_y-2-2_IsGammaN0.root \
  --IsgammaN 0 \
  --DoGptGyReweighting false
