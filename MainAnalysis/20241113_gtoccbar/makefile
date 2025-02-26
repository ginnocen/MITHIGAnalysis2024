default: ExecuteMuMuJet

ExecuteMuMuJet: MuMuJet.cpp
	g++ MuMuJet.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecuteMuMuJet `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

TestRun: ExecuteMuMuJet
	./ExecuteMuMuJet --Input /data/NewSkims_gtoccbar/20241113_DataPbPb2018gtoccbar_v2_TestV0.root --Output outputGtoccbar.root --TriggerChoice 0 --IsData true
