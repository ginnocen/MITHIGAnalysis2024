

default: TestRun

TestRun: Execute
	./Execute --Input DYLLMC/HiForestMiniAOD_100.root --Output SkimReco.root --MinZPT 40 --Fraction 0.05 \
		--TrackEfficiencyPath $(ProjectBase)/CommonCode/root/ --DoGenLevel false \
		--DoTrackResidual false
	# ./Execute --Input DYLLMC/HiForestMiniAOD_100.root --Output SkimGen.root --MinZPT 40 --Fraction 0.05 \
	# 	--TrackEfficiencyPath $(ProjectBase)/CommonCode/root/ --DoGenLevel true \
	# 	--DoTrackResidual false

TestRunBase: Execute
	./Execute --Input HiForestMiniAOD_314.root --Output Skim.root --MinZPT 40 --Fraction 0.05 \
		--TrackEfficiencyPath $(ProjectBase)/CommonCode/root/ --DoGenLevel false \
		--DoTrackResidual true \
		--TrackResidualPath "$(ProjectBase)/CommonCode/root/20230522_TrackResidualCorrection_V9_0_20.root,$(ProjectBase)/CommonCode/root/20230522_TrackResidualCorrection_V9_20_60.root,$(ProjectBase)/CommonCode/root/20230522_TrackResidualCorrection_V9_60_100.root,$(ProjectBase)/CommonCode/root/20230522_TrackResidualCorrection_V9_100_200.root"

Execute: ReduceForest.cpp
	g++ ReduceForest.cpp -o Execute \
		`root-config --cflags --glibs` \
		-I$(ProjectBase)/CommonCode/include $(ProjectBase)/CommonCode/library/Messenger.o
