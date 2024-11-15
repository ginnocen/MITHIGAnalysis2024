ROOT=`root-config --cflags --glibs`

MKDIR_OUTPUT=mkdir -p $(PWD)/out

MkdirOutput:
	$(MKDIR_OUTPUT)

DumpNewSkim: DumpNewSkim.cpp
	g++ DumpNewSkim.cpp -o DumpNewSkim $(ROOT) \
		-I$(ProjectBase)/CommonCode/include $(ProjectBase)/CommonCode/library/Messenger.o

DumpOldSkim: DumpOldSkim.cpp
	g++ DumpOldSkim.cpp -o DumpOldSkim $(ROOT) \
		-I$(ProjectBase)/CommonCode/include $(ProjectBase)/CommonCode/library/Messenger.o

Validate: Validate.cpp
	g++ Validate.cpp -o Validate $(ROOT) \
		-I$(ProjectBase)/CommonCode/include $(ProjectBase)/CommonCode/library/Messenger.o

ValidateData: MkdirOutput DumpNewSkim DumpOldSkim Validate
	./DumpNewSkim --Input /data/NewSkims23_24/ValidationSamples/20241106_NoEventDrejection/SkimHiForestMiniAOD_UPCPbPb23_HiVertex_279.root \
				  --Output out/DumpNewSkim --IsData true --SkimDzeroPresenceAbovePTMin 2.0

	./DumpOldSkim --Input /home/data/public/yuchenc/SkimsCheck_20241104/OldSkim/Data/GAP/gmTestFileForNewSkim_20241104_GAP_GapPlusORMinusEMinMinus8p6_Plus9p2_RecoDPtMin2p0.root \
				  --Output out/DumpOldSkim --IsData true --SkimDzeroPresenceAbovePTMin 2.0
	
	./Validate --Input1 out/DumpNewSkim.root --Input2 out/DumpOldSkim.root \
						 --LegendName1 New --LegendName2 Old \
						 --PlotName out/ValidateData_New_vs_Old_Skim.pdf

	echo "Diff between old file (<) and new file (>)" > out/DumpSkims.diff
	echo "Quit 'less out/DumpSkims.diff' by entering 'q'" >> out/DumpSkims.diff
	diff out/DumpOldSkim.txt out/DumpNewSkim.txt >> out/DumpSkims.diff || echo 'Files differ'
	less out/DumpSkims.diff

ValidateDataWDsel: MkdirOutput DumpNewSkim DumpOldSkim Validate
	./DumpNewSkim --Input /data/NewSkims23_24/ValidationSamples/20241106_NoEventDrejection/SkimHiForestMiniAOD_UPCPbPb23_HiVertex_279.root \
				  --Output out/DumpNewSkim --IsData true --SkimDzeroPresenceAbovePTMin 2.0 --ApplyDSelection true

	./DumpOldSkim --Input /home/data/public/yuchenc/SkimsCheck_20241104/OldSkim/Data/GAP/gmTestFileForNewSkim_20241104_GAP_GapPlusORMinusEMinMinus8p6_Plus9p2_RecoDPtMin2p0.root \
				  --Output out/DumpOldSkim --IsData true --SkimDzeroPresenceAbovePTMin 2.0 --ApplyDSelection true

	./Validate --Input1 out/DumpNewSkim.root --Input2 out/DumpOldSkim.root \
						 --LegendName1 New --LegendName2 Old \
						 --PlotName out/ValidateDataWDSel_New_vs_Old_Skim.pdf

	echo "Diff between old file (<) and new file (>)" > out/DumpSkims.diff
	echo "Quit 'less out/DumpSkims.diff' by entering 'q'" >> out/DumpSkims.diff
	diff out/DumpOldSkim.txt out/DumpNewSkim.txt >> out/DumpSkims.diff || echo 'Files differ'
	less out/DumpSkims.diff

ValidateMC: MkdirOutput DumpNewSkim DumpOldSkim Validate
	./DumpNewSkim --Input /data/NewSkims23_24/ValidationSamples/20241106_NoEventDrejection/SkimForcedD0Decay100M_BeamA_HiForestMiniAOD_44.root \
				  --Output out/DumpNewSkim --IsData false --SkimDzeroPresenceAbovePTMin 2.0

	./DumpOldSkim --Input /home/data/public/hannah/mc_productions/OfficialMC_pTHat2/UnmergedForests/ForcedD0Decay100M_BeamA/HiForestMiniAOD_44.root \
				  --Output out/DumpOldSkim --IsData false --SkimDzeroPresenceAbovePTMin 2.0

	./Validate --Input1 out/DumpNewSkim.root --Input2 out/DumpOldSkim.root \
						 --LegendName1 New --LegendName2 Old \
						 --PlotName out/ValidateMC_New_vs_Old_Skim.pdf

	echo "Diff between old file (<) and new file (>)" > out/DumpSkims.diff
	echo "Quit 'less out/DumpSkims.diff' by entering 'q'" >> out/DumpSkims.diff
	diff out/DumpOldSkim.txt out/DumpNewSkim.txt >> out/DumpSkims.diff || echo 'Files differ'
	less out/DumpSkims.diff

ValidateMCWDsel: MkdirOutput DumpNewSkim DumpOldSkim Validate
	./DumpNewSkim --Input /data/NewSkims23_24/ValidationSamples/20241106_NoEventDrejection/SkimForcedD0Decay100M_BeamA_HiForestMiniAOD_44.root \
				  --Output out/DumpNewSkim --IsData false --SkimDzeroPresenceAbovePTMin 2.0 --ApplyDSelection true

	./DumpOldSkim --Input /home/data/public/hannah/mc_productions/OfficialMC_pTHat2/UnmergedForests/ForcedD0Decay100M_BeamA/HiForestMiniAOD_44.root \
				  --Output out/DumpOldSkim --IsData false --SkimDzeroPresenceAbovePTMin 2.0 --ApplyDSelection true

	./Validate --Input1 out/DumpNewSkim.root --Input2 out/DumpOldSkim.root \
						 --LegendName1 New --LegendName2 Old \
						 --PlotName out/ValidateMCWDSel_New_vs_Old_Skim.pdf

	echo "Diff between old file (<) and new file (>)" > out/DumpSkims.diff
	echo "Quit 'less out/DumpSkims.diff' by entering 'q'" >> out/DumpSkims.diff
	diff out/DumpOldSkim.txt out/DumpNewSkim.txt >> out/DumpSkims.diff || echo 'Files differ'
	less out/DumpSkims.diff
