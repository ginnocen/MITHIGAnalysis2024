default: ExecuteDzeroUPC MassFit SidebandSubtraction GenMatchedTemplate CharmFractionExtraction

ExecuteDzeroUPC: DzeroUPC.cpp
	g++ DzeroUPC.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
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

GenMatchedTemplate: genMatchedTemplate.C
	g++ genMatchedTemplate.C -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o GenMatchedTemplate `root-config --cflags --glibs` -lRooFit -lRooFitCore \
		$(ProjectBase)/CommonCode/library/Messenger.o

CharmFractionExtraction: charmFractionExtraction.C
	g++ charmFractionExtraction.C -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o CharmFractionExtraction `root-config --cflags --glibs` -lRooFit -lRooFitCore \
		$(ProjectBase)/CommonCode/library/Messenger.o

TestRun: ExecuteDzeroUPC
	./ExecuteDzeroUPC --Input ../../Samples/SamplesSkims/MergedOutputJet830Oct.root --Output outputUPC.root
