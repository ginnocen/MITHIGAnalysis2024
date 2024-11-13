default: ExecuteGtoMuMu

ExecuteGtoMuMu: GtoMuMu.cpp
	g++ GtoMuMu.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecuteGtoMuMu `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

TestRun: ExecuteGtoMuMu
	./ExecuteGtoMuMu --Input /data/NewSkims_gtoccbar/20241113_DataPbPb2018gtoccbar_v2_TestV0.root --Output outputGtoccbar.root --TriggerChoice 0 --IsData true
