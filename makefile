default: ExecuteMuMuJet_cuts

ExecuteMuMuJet_cuts: MuMuJet_cuts.cpp
	g++ MuMuJet_cuts.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
	-o ExecuteMuMuJet_cuts `root-config --cflags --glibs` \
	$(ProjectBase)/CommonCode/library/Messenger.o \
