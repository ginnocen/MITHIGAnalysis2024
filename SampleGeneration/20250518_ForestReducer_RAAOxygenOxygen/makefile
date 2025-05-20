default: TestRun

TestRun: Execute
	./Execute --Input ${PATHSAMPLE}/crab_HiForest_250514_Hijing_ppreco_1500pre2_250507.root \
                  --Output skim_testfile.root \
                  --DoGenLevel false \
                  --Year 2025 \
                  --IsData false \
                  --Fraction 0.1 \
                  --PFTree particleFlowAnalyser/pftree

TestRunBase: Execute
	./Execute --Input ${PATHSAMPLE}/crab_HiForest_250514_Hijing_ppreco_1500pre2_250507.root \
                  --Output skim_testfile.root \
                  --DoGenLevel false \
                  --Year 2025 \
                  --IsData false \
                  --Fraction 0.1 \
                  --PFTree particleFlowAnalyser/pftree

Execute: ReduceForest.cpp
	g++ ReduceForest.cpp -o Execute \
		`root-config --cflags --glibs` \
		-I$(ProjectBase)/CommonCode/include $(ProjectBase)/CommonCode/library/Messenger.o
