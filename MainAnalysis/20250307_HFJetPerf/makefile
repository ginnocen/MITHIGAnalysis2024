default: ExecuteHFJetUPC

ExecuteHFJetUPC: HFJetUPC.cpp
	g++ HFJetUPC.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecuteHFJetUPC `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

ExecutePlotHFJetPerf: plotHFJetPerf.cpp
	g++ plotHFJetPerf.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecutePlotHFJetPerf `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

TestRun: ExecuteHFJetUPC
	./ExecuteHFJetUPC --Input /data/HFJetUPCanalysis/DataSkims23/20250106_ForestHFJetData23LowPtSkim_v1.root --Output outputHFJetUPC.root
