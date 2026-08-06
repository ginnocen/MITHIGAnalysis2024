default: ExecuteMain #ExecuteCentralityStudy #ExecuteMain Execute2DPlot 

SRC = $(wildcard Src/*.cpp)

ExecuteMain: MainEfficiency.cpp
	g++ MainEfficiency.cpp $(SRC) -O3 -I. -I$(ProjectBase)/CommonCode/include \
	  -I./Include -I./Src \
		-o MainEfficiencyExecutable `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

#PlotMain: PlotMain.cpp
 #   g++ PlotMain.cpp $(SRC) -O3 -I. -I$(ProjectBase)/CommonCode/include \
 #     -I./Include -I./Src \
 #       -o PlotExecutable `root-config --cflags --glibs` \
 #       $(ProjectBase)/CommonCode/library/Messenger.o

Execute2DPlot: Execute2DPlot.cpp
	g++ Execute2DPlot.cpp $(SRC) -O3 -I. -I$(ProjectBase)/CommonCode/include \
	  -I./Include -I./Src \
		-o Execute2DPlotExecutable `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

ExecuteCentralityStudy: CentralityStudy.cpp
	g++ CentralityStudy.cpp $(SRC) -O3 -I. -I$(ProjectBase)/CommonCode/include \
	  -I./Include -I./Src \
		-o ExecuteCentralityStudy `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o
