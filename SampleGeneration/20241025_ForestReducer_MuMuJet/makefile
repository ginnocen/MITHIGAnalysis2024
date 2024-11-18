
default: TestRun

TestRun: Execute
	./Execute --Input /data/gtoccbar/20241023_DataPbPb2018gtoccbar_v2/241024_002902/0000/HiForestMiniAOD_479.root --IsData true --IsPP false --Output SkimReco.root --MinJetPT 80 --Fraction 1.00

TestRunBase: Execute
	./Execute --Input /data/gtoccbar/20241023_DataPbPb2018gtoccbar_v2/241024_002902/0000/HiForestMiniAOD_479.root --IsData true --IsPP false --Output SkimReco.root --MinJetPT 80 --Fraction 1.00

Execute: ReduceForest.cpp
	g++ ReduceForest.cpp -o Execute \
		`root-config --cflags --glibs` \
		-I$(ProjectBase)/CommonCode/include $(ProjectBase)/CommonCode/library/Messenger.o
