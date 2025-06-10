default: ExecuteUPCEEC

ExecuteUPCEEC: UPCEEC.cpp
	g++ UPCEEC.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecuteUPCEEC `root-config --cflags --glibs` \
		-I$(ProjectBase)/CommonCode/include \
		$(ProjectBase)/CommonCode/library/*.o

ExecutePlotUPCEEC: plotUPCEEC.cpp
	g++ plotUPCEEC.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecutePlotUPCEEC `root-config --cflags --glibs` \
		-I$(ProjectBase)/CommonCode/include \
		$(ProjectBase)/CommonCode/library/*.o