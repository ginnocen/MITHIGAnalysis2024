default: ExecuteDzeroUPC MassFit CorrectedYields PlotCrossSection PlotRapGapScan PlotHFDist

ExecuteDzeroUPC: DzeroUPC.cpp
	g++ DzeroUPC.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecuteDzeroUPC `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

MassFit: massfit.C
	g++ massfit.C -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o MassFit `root-config --cflags --glibs` -lRooFit -lRooFitCore \
		$(ProjectBase)/CommonCode/library/Messenger.o

CorrectedYields: correctedYields.C
	g++ correctedYields.C -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o CorrectedYields `root-config --cflags --glibs` -lRooFit -lRooFitCore \
		$(ProjectBase)/CommonCode/library/Messenger.o

PlotCrossSection: plotCrossSection.C
	g++ plotCrossSection.C -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o PlotCrossSection `root-config --cflags --glibs` -lRooFit -lRooFitCore \
		$(ProjectBase)/CommonCode/library/Messenger.o

PlotRapGapScan: rapGapScanPlotter/plotRapGapScan.C
	g++ rapGapScanPlotter/plotRapGapScan.C -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o PlotRapGapScan `root-config --cflags --glibs` -lRooFit -lRooFitCore \
		$(ProjectBase)/CommonCode/library/Messenger.o

PlotHFDist: rapGapScanPlotter/plotHFDist.C
	g++ rapGapScanPlotter/plotHFDist.C -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o PlotHFDist `root-config --cflags --glibs` -lRooFit -lRooFitCore \
		$(ProjectBase)/CommonCode/library/Messenger.o

TestRun: ExecuteDzeroUPC
	./ExecuteDzeroUPC --Input ../../Samples/SamplesSkims/MergedOutputJet830Oct.root --Output outputUPC.root
