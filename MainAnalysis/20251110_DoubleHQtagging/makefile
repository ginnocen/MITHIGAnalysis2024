default: ExecuteDoubleHQtagging #MassFit

ExecuteDoubleHQtagging: DoubleHQtagging.cpp
	g++ DoubleHQtagging.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecuteDoubleHQtagging `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o \
                $(ProjectBase)/CommonCode/library/DimuonMessenger.o

#MassFit: DoubleHQtagging.cpp
#	g++ massfit.C -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
#		-o MassFit `root-config --cflags --glibs` -lRooFit -lRooFitCore \
#		$(ProjectBase)/CommonCode/library/Messenger.o

TestRun: ExecuteDoubleHQtagging
	./ExecuteDoubleHQtagging --Input /data00/g2ccbar/mc2018/skim_102725_0/mergedfile.root --Output outputDoubleHQtagging_testSample.root --TriggerChoice 0 --IsData false
