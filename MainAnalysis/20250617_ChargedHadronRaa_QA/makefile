default: ExecuteChargedHadronRAA

ExecuteChargedHadronRAA: ChargedHadronRAA.cpp QAMessenger.o
	g++ ChargedHadronRAA.cpp QAMessenger.o -O3 -I. -I$(ProjectBase)/CommonCode/include \
	  -I./include \
		-o ExecuteChargedHadronRAA `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

QAMessenger.o: source/QAMessenger.cpp ./include/QAMessenger.h
	g++ -c source/QAMessenger.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include `root-config --cflags --glibs`

TestRun: ExecuteChargedHadronRAA
	./ExecuteChargedHadronRAA --Input skimexample.root --Output chargedhadronRAA.root
