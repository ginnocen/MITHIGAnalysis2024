default: Execute_Dzero

Execute_Dzero: ReduceForest_Dzero.cpp
	g++ ReduceForest_Dzero.cpp -o Execute_Dzero \
	`root-config --cflags --glibs` \
	-I$(ProjectBase)/CommonCode/include $(ProjectBase)/CommonCode/library/Messenger.o
