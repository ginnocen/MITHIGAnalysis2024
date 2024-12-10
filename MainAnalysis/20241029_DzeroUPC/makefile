default: ExecuteDzeroUPC MassFit

ExecuteDzeroUPC: DzeroUPC.cpp
	g++ DzeroUPC.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecuteDzeroUPC `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

MassFit: DzeroUPC.cpp
	g++ massfit.C -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o MassFit `root-config --cflags --glibs` -lRooFit -lRooFitCore \
		$(ProjectBase)/CommonCode/library/Messenger.o

TestRun: ExecuteDzeroUPC
	./ExecuteDzeroUPC --Input ../../Samples/SamplesSkims/MergedOutputJet830Oct.root --Output outputUPC.root
