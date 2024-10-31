default: TestRun

TestRun: Execute
	./Execute --Input ../../Samples/SamplesForest23HIVertex/HiForestMiniAOD_UPCPbPb23_HiVertex_279.root \
                  --Output SkimReco_UPCPbPb23_HiVertex_279.root --Year 2023 --MinDzeroPT 2.0 --IsData true \
                  --PFTree particleFlowAnalyser/pftree

TestRunBase: Execute
	./Execute --Input ../../Samples/SamplesForest23HIVertex/HiForestMiniAOD_UPCPbPb23_HiVertex_279.root \
                  --Output SkimReco_UPCPbPb23_HiVertex_279.root --Year 2023 --MinDzeroPT 2.0 --IsData true \
                  --PFTree particleFlowAnalyser/pftree

Execute: ReduceForest.cpp
	g++ ReduceForest.cpp -o Execute \
		`root-config --cflags --glibs` \
		-I$(ProjectBase)/CommonCode/include $(ProjectBase)/CommonCode/library/Messenger.o
