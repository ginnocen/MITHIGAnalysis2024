default: ExecuteHFUPCEEC

ExecuteHFUPCEEC: HFUPCEEC.cpp
	g++ HFUPCEEC.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecuteHFUPCEEC `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

ExecutePlotHFEEC: plotHFEEC.cpp
	g++ plotHFEEC.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecutePlotHFEEC `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

TestRun: ExecuteHFUPCEEC
	./ExecuteHFJetUPC --Input /data/HFJetUPCanalysis/DataSkims23/20250106_ForestHFJetData23LowPtSkim_v1.root --Output outputHFJetUPC.root
