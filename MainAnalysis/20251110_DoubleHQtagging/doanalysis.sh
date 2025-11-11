source clean.sh
rm *.root
INPUT=/data00/g2ccbar/mc2018/skim_102725_0/mergedfile.root
./ExecuteDoubleHQtagging \
     --Input $INPUT \
     --IsData 0 \
     --IsPP 1 \
     --MinJetPT 80 \
     --MaxJetPT 100 \
     --ChargeSelection -1 \
     --NbHad 2 \
     --NcHad -1 \
     --DCAString "muDiDxy1 > 0.005 && muDiDxy2 > 0.005" \
     --Output output_DoubleHQtagging.root
wait
