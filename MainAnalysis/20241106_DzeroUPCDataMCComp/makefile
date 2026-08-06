default: DataMCComp

DataMCComp: DataMCComp.cpp
	g++ DataMCComp.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o DataMCComp `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o
