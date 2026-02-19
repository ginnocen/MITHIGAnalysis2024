Execute: ReduceForest.cpp $(ProjectBase)/CommonCode/library/DimuonMessenger.o
	g++ ReduceForest.cpp -o Execute \
		`root-config --cflags --glibs` \
		-I$(ProjectBase)/CommonCode/include \
		$(ProjectBase)/CommonCode/library/Messenger.o \
		$(ProjectBase)/CommonCode/library/DimuonMessenger.o
