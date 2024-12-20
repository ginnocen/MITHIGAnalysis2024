#source clean.sh
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
	mkdir -p $MicroTreeDir/pt${MinDzeroPT}-${MaxDzeroPT}_y${MinDzeroY}-${MaxDzeroY}_IsGammaN${IsGammaN}/
	Output=$MicroTreeDir/pt${MinDzeroPT}-${MaxDzeroPT}_y${MinDzeroY}-${MaxDzeroY}_IsGammaN${IsGammaN}/${MicroTreeBaseName}

	./ExecuteDzeroUPC --Input $Input \
										--MinDzeroPT $MinDzeroPT \
										--MaxDzeroPT $MaxDzeroPT \
										--MinDzeroY $MinDzeroY \
										--MaxDzeroY $MaxDzeroY \
										--IsGammaN $IsGammaN \
										--TriggerChoice $TriggerChoice \
										--IsData $IsData \
										--Output $Output

done