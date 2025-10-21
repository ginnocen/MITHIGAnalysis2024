default: ExecuteChargedHadronRAA TemplateFitting

ExecuteChargedHadronRAA: ChargedHadronRAA.cpp
	g++ ChargedHadronRAA.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include \
	  -I./include \
		-o ExecuteChargedHadronRAA `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

TestRun: ExecuteChargedHadronRAA
	./ExecuteChargedHadronRAA --Input skimexample.root --Output chargedhadronRAA.root

TemplateFitting: Fitter.cpp
	g++ Fitter.cpp -O3 -I. -I./include \
	-I$(ProjectBase)/CommonCode/include \
	`root-config --cflags --libs` -lRooFit -lRooFitCore -o TemplateFitting

clean:
	rm -f ExecuteChargedHadronRAA TemplateFitting
