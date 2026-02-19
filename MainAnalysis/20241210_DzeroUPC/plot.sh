PlotSettingCard=${1}
PlotDir=$(jq -r '.PlotDir' $PlotSettingCard)
PDFraction=$(jq -r '.PDFraction' $PlotSettingCard)
mkdir -p $PlotDir
cp $PlotSettingCard $PlotDir/plotConfig.json
PlotSettingCard=$PlotDir/plotConfig.json

echo "" > $PlotDir/plot.log

jq -c '.Plots[]' $PlotSettingCard | while read Plot; do
  MinDzeroPT=$(echo $Plot | jq -r '.MinDzeroPT')
  MaxDzeroPT=$(echo $Plot | jq -r '.MaxDzeroPT')
  IsGammaN=$(echo $Plot | jq -r '.IsGammaN')
  UseMaxFitUncert=$(echo $Plot | jq -r '.UseMaxFitUncert')
  InputPoints=$(echo $Plot | jq -r '.InputPoints')
  wSystLumi=$(echo $Plot | jq -r '.wSystLumi')
  wSystTrk=$(echo $Plot | jq -r '.wSystTrk')
  wSystBR=$(echo $Plot | jq -r '.wSystBR')
  wSystEvtSel=$(echo $Plot | jq -r '.wSystEvtSel')
  wSystPromptFrac=$(echo $Plot | jq -r '.wSystPromptFrac')
  wSystRapGapSel=$(echo $Plot | jq -r '.wSystRapGapSel')
  wSystDsvpv=$(echo $Plot | jq -r '.wSystDsvpv')
  wSystDtrkPt=$(echo $Plot | jq -r '.wSystDtrkPt')
  wSystDalpha=$(echo $Plot | jq -r '.wSystDalpha')
  wSystDchi2cl=$(echo $Plot | jq -r '.wSystDchi2cl')
  wSystFitPkBg=$(echo $Plot | jq -r '.wSystFitPkBg')
  wSystFitSiglAlpha=$(echo $Plot | jq -r '.wSystFitSiglAlpha')
  wSystFitSiglMean=$(echo $Plot | jq -r '.wSystFitSiglMean')
  wSystFitMassWindow=$(echo $Plot | jq -r '.wSystFitMassWindow')
  nominalSampleRST=$(echo $Plot | jq -r '.nominalSampleRST')
  nominalFitRST=$(echo $Plot | jq -r '.nominalFitRST')

  cmd="./PlotCrossSection --PlotDir $PlotDir --MinDzeroPT $MinDzeroPT --MaxDzeroPT $MaxDzeroPT --IsGammaN $IsGammaN --InputPoints $InputPoints"
  [ "$PDFraction" != "null" ] && cmd="$cmd --PDFraction $PDFraction"
  [ "$UseMaxFitUncert" != "null" ] && cmd="$cmd --UseMaxFitUncert $UseMaxFitUncert"
  [ "$wSystLumi" != "null" ] && cmd="$cmd --wSystLumi $wSystLumi"
  [ "$wSystTrk" != "null" ] && cmd="$cmd --wSystTrk $wSystTrk"
  [ "$wSystBR" != "null" ] && cmd="$cmd --wSystBR $wSystBR"
  [ "$wSystEvtSel" != "null" ] && cmd="$cmd --wSystEvtSel $wSystEvtSel"
  [ "$wSystPromptFrac" != "null" ] && cmd="$cmd --wSystPromptFrac $wSystPromptFrac"
  [ "$wSystRapGapSel" != "null" ] && cmd="$cmd --wSystRapGapSel $wSystRapGapSel"
  [ "$wSystDsvpv" != "null" ] && cmd="$cmd --wSystDsvpv $wSystDsvpv"
  [ "$wSystDtrkPt" != "null" ] && cmd="$cmd --wSystDtrkPt $wSystDtrkPt"
  [ "$wSystDalpha" != "null" ] && cmd="$cmd --wSystDalpha $wSystDalpha"
  [ "$wSystDchi2cl" != "null" ] && cmd="$cmd --wSystDchi2cl $wSystDchi2cl"
  [ "$wSystFitPkBg" != "null" ] && cmd="$cmd --wSystFitPkBg $wSystFitPkBg"
  [ "$wSystFitSiglAlpha" != "null" ] && cmd="$cmd --wSystFitSiglAlpha $wSystFitSiglAlpha"
  [ "$wSystFitSiglMean" != "null" ] && cmd="$cmd --wSystFitSiglMean $wSystFitSiglMean"
  [ "$wSystFitMassWindow" != "null" ] && cmd="$cmd --wSystFitMassWindow $wSystFitMassWindow"
  [ "$nominalSampleRST" != "null" ] && cmd="$cmd --nominalSampleRST $nominalSampleRST"
  [ "$nominalFitRST" != "null" ] && cmd="$cmd --nominalFitRST $nominalFitRST"

  echo "Executing >>>>>>"
  echo $cmd

  $cmd
  
#	$cmd >> $PlotDir/plot.log

done
