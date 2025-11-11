source clean.sh
rm *.root
INPUT=/data00/g2ccbar/mc2018/skim_102725_0/mergedfile.root
./ExecuteDoubleHQtagging \
     --Input $INPUT \
     --IsData 0 \
     --IsPP 1 \
     --MinJetPT 80 \
     --MaxJetPT 120 \
     --ChargeSelection -1 \
     --NbHad -1 \
     --NcHad -1 \
     --DCAString "abs(muDiDxy1) > 0.005 && abs(muDiDxy2) > 0.005" \
     --Output output_DoubleHQtagging.root

./MassFit --Input output_DoubleHQtagging.root --Output fit_result.root --Label "DoubleHQtagging" --MinJetPT 80 --MaxJetPT 120
root -l -b -q 'plotter.C("output_DoubleHQtagging.root","hInvMass",0)'
wait
