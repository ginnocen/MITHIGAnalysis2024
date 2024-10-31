default: ExecuteCorrelationAnalysis

ExecuteCorrelationAnalysis: CorrelationAnalysis.cpp
	g++ CorrelationAnalysis.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include -I./include \
		-o ExecuteCorrelationAnalysis `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

TestRun: ExecuteCorrelationAnalysis
	./ExecuteCorrelationAnalysis --Input XXX1.root,XXX2.root --Output output.root