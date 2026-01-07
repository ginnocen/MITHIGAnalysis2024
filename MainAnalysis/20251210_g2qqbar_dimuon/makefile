default: ExecuteEfficiency MakeDistros ExecuteYield

ExecuteEfficiency: Efficiencies.cc
	g++ Efficiencies.cc -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecuteEfficiency `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o \
		$(ProjectBase)/CommonCode/library/DimuonMessenger.o

MakeDistros: MakeDistros.cc
	g++ MakeDistros.cc -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o MakeDistros `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o \
		$(ProjectBase)/CommonCode/library/DimuonMessenger.o

ExecuteYield: Yield.cc
	g++ Yield.cc -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecuteYield `root-config --cflags --glibs` -lRooFit -lRooFitCore \
		$(ProjectBase)/CommonCode/library/Messenger.o \
		$(ProjectBase)/CommonCode/library/DimuonMessenger.o