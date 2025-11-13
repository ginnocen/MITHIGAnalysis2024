source clean.sh
rm *.root
#INPUT=/data00/g2ccbar/mc2018/skim_102725_all/mergedfile.root
INPUT=/data00/g2ccbar/data2018/skim_110525_0/mergedfile.root
OUTPUT=output_DoubleHQtagging_data.root
./ExecuteDoubleHQtagging \
     --Input $INPUT \
     --IsData 1 \
     --IsPP 1 \
     --MinJetPT 80 \
     --MaxJetPT 120 \
     --ChargeSelection -1 \
     --DCAString "abs(muDiDxy1) > 0.005 && abs(muDiDxy2) > 0.005" \
     --Output ${OUTPUT}

./MassFit --Input ${OUTPUT} --Output fit_result.root --Label "DoubleHQtagging" --MinJetPT 80 --MaxJetPT 120
root -l -b -q 'plotter.C("'${OUTPUT}'")'
wait
