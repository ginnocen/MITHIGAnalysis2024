default: Execute_Lcpks Execute_Lcpkpi Execute_Dzero

TestRun: Execute_Lcpks
	./Execute_Lcpks --Input /data/NewSkims23_24/20241102_ForestOldReco23sample_Dataexample/HiForestMiniAOD_UPCPbPb23_HiVertex_279.root \
                  --Output SkimReco_UPCPbPb23_HiVertex_279.root --Year 2023 --IsData true \
                  --ApplyTriggerRejection 0 \
                  --ApplyEventRejection false \
                  --ApplyZDCGapRejection false \
                  --ApplyDRejection no \
                  --PFTree particleFlowAnalyser/pftree

Execute_Dzero: ReduceForest_Dzero.cpp
	g++ ReduceForest_Dzero.cpp -o Execute_Dzero \
		`root-config --cflags --glibs` \
		-I$(ProjectBase)/CommonCode/include $(ProjectBase)/CommonCode/library/Messenger.o

Execute_Lcpks: ReduceForest_LambdaCpks.cpp
	g++ ReduceForest_LambdaCpks.cpp -o Execute_Lcpks \
		`root-config --cflags --glibs` \
		-I$(ProjectBase)/CommonCode/include $(ProjectBase)/CommonCode/library/Messenger.o

Execute_Lcpkpi: ReduceForest_LambdaCpkpi.cpp
	g++ ReduceForest_LambdaCpkpi.cpp -o Execute_Lcpkpi \
		`root-config --cflags --glibs` \
		-I$(ProjectBase)/CommonCode/include $(ProjectBase)/CommonCode/library/Messenger.o
