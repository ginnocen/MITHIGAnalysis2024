default: ExecuteHFJetUPC

ExecuteHFJetUPC: HFJetUPC.cpp
	g++ HFJetUPC.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecuteHFJetUPC `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

ExecutePlotHFJet: plotHFjet.cpp
	g++ plotHFjet.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecutePlotHFJet `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

ExecutePlotPtHat: plotPtHat.cpp
	g++ plotPtHat.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecutePlotPtHat `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

ExecutePlotDoFragFunction: plotD0FragFunction.cpp
	g++ plotD0FragFunction.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecutePlotDoFragFunction `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

TestRun: ExecuteHFJetUPC
	./ExecuteHFJetUPC --Input /data/HFJetUPCanalysis/DataSkims23/20250106_ForestHFJetData23LowPtSkim_v1.root --Output outputHFJetUPC.root
