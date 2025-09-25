PlotSettingCard=${1}
PlotDir=$(jq -r '.PlotDir' $PlotSettingCard)
mkdir -p $PlotDir
cp $PlotSettingCard $PlotDir/plotConfig.json
PlotSettingCard=$PlotDir/plotConfig.json

echo "" > $PlotDir/plot.log

jq -c '.Plots[]' $PlotSettingCard | while read Plot; do
	MinDzeroPT=$(echo $Plot | jq -r '.MinDzeroPT')
	MaxDzeroPT=$(echo $Plot | jq -r '.MaxDzeroPT')
	MinDzeroY=$(echo $Plot | jq -r '.MinDzeroY')
	MaxDzeroY=$(echo $Plot | jq -r '.MaxDzeroY')
	IsGammaN=$(echo $Plot | jq -r '.IsGammaN')
	HFEMax=$(echo $Plot | jq -r '.HFEMax')

  cmd="./PlotRapGapScan --PlotDir $PlotDir --MinDzeroPT $MinDzeroPT --MaxDzeroPT $MaxDzeroPT --MinDzeroY $MinDzeroY --MaxDzeroY $MaxDzeroY --IsGammaN $IsGammaN --HFEMax $HFEMax"
			

  echo "Executing >>>>>>"
  echo $cmd

	$cmd >> $PlotDir/plot.log

done
