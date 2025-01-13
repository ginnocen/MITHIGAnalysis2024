default: Execute

TestRun: Execute
	./Execute --Input HiForestMiniAOD_1.root \
                  --Output SkimReco_UPCPbPb23_388000_HIForward0_1.root --Year 2024 --IsData true \
                  --ApplyTriggerRejection false \
                  --ApplyEventRejection false \
                  --ApplyZDCGapRejection false \
                  --ApplyDRejection false \
                  --PFTree particleFlowAnalyser/pftree

TestRunBase: Execute
	./Execute --Input HiForestMiniAOD_1.root \
                  --Output SkimReco_UPCPbPb23_388000_HIForward0_1.root --Year 2024 --IsData true \
                  --ApplyTriggerRejection false \
                  --ApplyEventRejection false \
                  --ApplyZDCGapRejection false \
                  --ApplyDRejection false \
                  --PFTree particleFlowAnalyser/pftree

Execute: ReduceForest.cpp
	CGO_ENABLED=0
#	g++ ReduceForest.cpp -o Execute \
#		-Wl,-Bstatic -lpthread -lc \
#		-Wl,-Bdynamic `root-config --cflags --libs` \
#		-I$(ProjectBase)/CommonCode/include $(ProjectBase)/CommonCode/library/Messenger.o
	g++ ReduceForest.cpp -o Execute \
		`root-config --cflags --glibs` \
		-I$(ProjectBase)/CommonCode/include $(ProjectBase)/CommonCode/library/Messenger.o
