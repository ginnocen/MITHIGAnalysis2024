default: ExecuteChargedHadrondNdpt

ExecuteChargedHadrondNdpt: ChargedHadrondNdpt.cpp
	g++ ChargedHadrondNdpt.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include \
	  -I./include \
		-o ExecuteChargedHadrondNdpt `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

TestRun: ExecuteChargedHadrondNdpt
	./ExecuteChargedHadrondNdpt --Input skimexample.root --Output chargedhadronRAA.root
