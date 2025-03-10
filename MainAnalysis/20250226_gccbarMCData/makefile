default: ExecuteMuMuJet_culler

ExecuteMuMuJet_culler: MuMuJet_culler.cpp
	g++ MuMuJet_culler.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
	-o ExecuteMuMuJet_culler `root-config --cflags --glibs` \
	$(ProjectBase)/CommonCode/library/Messenger.o \


