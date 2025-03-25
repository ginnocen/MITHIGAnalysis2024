#!/bin/bash

SampleSettingCard=${1}
MicroTreeDir=$(jq -r '.MicroTreeDir' $SampleSettingCard)
mkdir -p $MicroTreeDir
cp $SampleSettingCard $MicroTreeDir/sampleConfig.json
SampleSettingCard=$MicroTreeDir/sampleConfig.json

jq -c '.MicroTrees[]' $SampleSettingCard | while read MicroTree; do
	MicroTreeBaseName=$(echo $MicroTree | jq -r '.MicroTreeBaseName')
	Input=$(echo $MicroTree | jq -r '.Input')
	MinDzeroPT=$(echo $MicroTree | jq -r '.MinDzeroPT')
	MaxDzeroPT=$(echo $MicroTree | jq -r '.MaxDzeroPT')
	MinDzeroY=$(echo $MicroTree | jq -r '.MinDzeroY')
	MaxDzeroY=$(echo $MicroTree | jq -r '.MaxDzeroY')
	IsGammaN=$(echo $MicroTree | jq -r '.IsGammaN')
	TriggerChoice=$(echo $MicroTree | jq -r '.TriggerChoice')
	IsData=$(echo $MicroTree | jq -r '.IsData')
	DoSystRapGap=$(echo $MicroTree | jq -r '.DoSystRapGap')
	DoSystD=$(echo $MicroTree | jq -r '.DoSystD')
	DoGptGyReweighting=$(echo $MicroTree | jq -r '.DoGptGyReweighting')
	GptGyWeightFileName=$(echo $MicroTree | jq -r '.GptGyWeightFileName')
	DoMultReweighting=$(echo $MicroTree | jq -r '.DoMultReweighting')
	MultWeightFileName=$(echo $MicroTree | jq -r '.MultWeightFileName')
	mkdir -p $MicroTreeDir/pt${MinDzeroPT}-${MaxDzeroPT}_y${MinDzeroY}-${MaxDzeroY}_IsGammaN${IsGammaN}/
	Output=$MicroTreeDir/pt${MinDzeroPT}-${MaxDzeroPT}_y${MinDzeroY}-${MaxDzeroY}_IsGammaN${IsGammaN}/${MicroTreeBaseName}

	cmd="./ExecuteDzeroUPC --Input $Input  --Output $Output \
			--MinDzeroPT $MinDzeroPT \
			--MaxDzeroPT $MaxDzeroPT \
			--MinDzeroY $MinDzeroY \
			--MaxDzeroY $MaxDzeroY \
			--IsGammaN $IsGammaN \
			--TriggerChoice $TriggerChoice \
			--IsData $IsData"
			[ $DoSystRapGap != null ] && cmd="$cmd --DoSystRapGap $DoSystRapGap"
			[ $DoSystD != null ] && cmd="$cmd --DoSystD $DoSystD"
			[ $DoGptGyReweighting != null ] && cmd="$cmd --DoGptGyReweighting $DoGptGyReweighting"
			[ $GptGyWeightFileName != null ] && cmd="$cmd --GptGyWeightFileName $GptGyWeightFileName"
			[ $DoMultReweighting != null ] && cmd="$cmd --DoMultReweighting $DoMultReweighting"
			[ $MultWeightFileName != null ] && cmd="$cmd --MultWeightFileName $MultWeightFileName"

	echo "Executing >>>>>>"
	echo $cmd

	MicroTreeLogName="${MicroTreeBaseName/.root/.log}"

	echo $cmd > $MicroTreeDir/pt${MinDzeroPT}-${MaxDzeroPT}_y${MinDzeroY}-${MaxDzeroY}_IsGammaN${IsGammaN}/${MicroTreeLogName}
	( $cmd >> $MicroTreeDir/pt${MinDzeroPT}-${MaxDzeroPT}_y${MinDzeroY}-${MaxDzeroY}_IsGammaN${IsGammaN}/${MicroTreeLogName} ) &
	sleep 0.1
done

sleep 1
while pgrep -x "ExecuteDzeroUPC" > /dev/null; do
  sleep 2
done
wait
exit 0
