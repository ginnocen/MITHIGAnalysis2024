default: ExecuteYield1 ExecuteYield2 ExecuteYield3

ExecuteYield1: Yield_5temp.cc
	g++ Yield_5temp.cc -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecuteYield `root-config --cflags --glibs` -lRooFit -lRooFitCore \
		$(ProjectBase)/CommonCode/library/Messenger.o \
		$(ProjectBase)/CommonCode/library/DimuonMessenger.o

ExecuteYield2: Yield_simul.cc
	g++ Yield_simul.cc -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecuteYield_simul `root-config --cflags --glibs` -lRooFit -lRooFitCore \
		$(ProjectBase)/CommonCode/library/Messenger.o \
		$(ProjectBase)/CommonCode/library/DimuonMessenger.o

ExecuteYield3: Yield_DRS.cc
	g++ Yield_DRS.cc -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecuteYield_DRS `root-config --cflags --glibs` -lRooFit -lRooFitCore \
		$(ProjectBase)/CommonCode/library/Messenger.o \
		$(ProjectBase)/CommonCode/library/DimuonMessenger.o