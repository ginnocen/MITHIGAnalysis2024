default: ExecuteChargedHadronRAA

ExecuteChargedHadronRAA: ChargedHadronRAA.cpp
	g++ ChargedHadronRAA.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include \
	  -I./include \
		-o ExecuteChargedHadronRAA `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

TestRun: ExecuteChargedHadronRAA
	./ExecuteChargedHadronRAA --Input skimexample.root --Output chargedhadronRAA.root
