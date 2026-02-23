default: Execute_Dzero

Execute_Dzero: ReduceForest_Dzero.cpp
	g++ ReduceForest_Dzero.cpp -o Execute_Dzero \
	`root-config --cflags --glibs` \
	 -lTMVA -lTMVAGui -lXMLIO -lstdc++fs -lEG \
	-I$(ProjectBase)/CommonCode/include $(ProjectBase)/CommonCode/library/Messenger.o
