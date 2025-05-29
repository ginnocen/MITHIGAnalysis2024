default: TestRun

TestRun: Execute
	./Execute --Input ${PATHSAMPLE}/HiForestMiniAOD_Hijing_merged_DEBUG.root \
                  --Output skim_testfile.root \
                  --DoGenLevel false \
                  --Year 2025 \
                  --IsData false \
                  --Fraction 0.1 \
                  --PFTree particleFlowAnalyser/pftree \
                  --sampleType 0 \
                  --DebugMode true
TestRunBase: Execute
	./Execute --Input ${PATHSAMPLE}/HiForestMiniAOD_Hijing_merged_DEBUG.root \
                  --Output skim_testfile.root \
                  --DoGenLevel false \
                  --Year 2025 \
                  --IsData false \
                  --Fraction 0.1 \
                  --PFTree particleFlowAnalyser/pftree \
                  --sampleType 0 \
                  --DebugMode true
Execute: ReduceForest.cpp
	g++ ReduceForest.cpp -o Execute \
		`root-config --cflags --glibs` \
		-I$(ProjectBase)/CommonCode/include $(ProjectBase)/CommonCode/library/Messenger.o
