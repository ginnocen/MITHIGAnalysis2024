default: ExecuteEfficiency

ExecuteEfficiency: Efficiencies.cc
	g++ Efficiencies.cc -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecuteEfficiency `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o \
		$(ProjectBase)/CommonCode/library/DimuonMessenger.o
