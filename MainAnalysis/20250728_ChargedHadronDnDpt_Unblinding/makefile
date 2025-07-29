default: ExecuteChargedHadrondNdpT

ExecuteChargedHadrondNdpt: ChargedHadrondNdpT.cpp
	g++ ChargedHadrondNdpT.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include \
	  -I./include \
		-o ExecuteChargedHadrondNdpT `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

TestRun: ExecuteChargedHadrondNdpT
	./ExecuteChargedHadrondNdpT --Input skimexample.root --Output chargedhadronRAA.root
