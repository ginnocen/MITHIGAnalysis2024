default: ExecuteDzeroUPC MassFit SidebandSubtraction DataMCComp PlotDyVsDalpha McDEfficiencyVsMult

McDEfficiencyVsMult: mcDEfficiencyVsMult.cpp
	g++ mcDEfficiencyVsMult.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o McDEfficiencyVsMult `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

PlotDyVsDalpha: plotDyVsDalpha.cpp
	g++ plotDyVsDalpha.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o PlotDyVsDalpha `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

ExecuteDzeroUPC: DzeroUPC.cpp
	g++ DzeroUPC.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include \
	  -I$(ProjectBase)/WeightHandler/20250305_DzeroUPC_GptGyWeight/include \
	  -I$(ProjectBase)/WeightHandler/20250312_DzeroUPC_multiplicityWeight/include \
	  -I./include \
		-o ExecuteDzeroUPC `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

MassFit: massfit.C
	g++ massfit.C -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o MassFit `root-config --cflags --glibs` -lRooFit -lRooFitCore \
		$(ProjectBase)/CommonCode/library/Messenger.o

SidebandSubtraction: sidebandSubtraction.C
	g++ sidebandSubtraction.C -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o SidebandSubtraction `root-config --cflags --glibs` -lRooFit -lRooFitCore \
		$(ProjectBase)/CommonCode/library/Messenger.o

DataMCComp: dataMCComp.C
	g++ dataMCComp.C -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o DataMCComp `root-config --cflags --glibs` -lRooFit -lRooFitCore \
		$(ProjectBase)/CommonCode/library/Messenger.o

TestRun: ExecuteDzeroUPC
	./ExecuteDzeroUPC --Input ../../Samples/SamplesSkims/MergedOutputJet830Oct.root --Output outputUPC.root
