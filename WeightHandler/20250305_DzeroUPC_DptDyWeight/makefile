default: all

all: Setup bin/MakeWeights

Setup:
	mkdir -p bin
	mkdir -p Weights


bin/MakeWeights: source/MakeWeights.cpp
	g++ source/MakeWeights.cpp -O3 -Iinclude -I$(ProjectBase)/CommonCode/include -o bin/MakeWeights `root-config --cflags --glibs`

test: bin/MakeWeights
	bin/MakeWeights --numFileName xxx --numHistName xxx \
									--denFileName xxx --denHistName xxx \
									--weightFileName Weights/testWeight.root
