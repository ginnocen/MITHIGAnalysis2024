default: ExecuteHFUPCEEC

ExecuteHFUPCEEC: HFUPCEEC.cpp
	g++ HFUPCEEC.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecuteHFUPCEEC `root-config --cflags --glibs` \
		-I$(ProjectBase)/CommonCode/include \
		$(ProjectBase)/CommonCode/library/*.o

ExecutePlotHFEEC: plotHFEEC.cpp
	g++ plotHFEEC.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecutePlotHFEEC `root-config --cflags --glibs` \
		-I$(ProjectBase)/CommonCode/include \
		$(ProjectBase)/CommonCode/library/*.o