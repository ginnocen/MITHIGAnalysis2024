
source clean.sh
rm *.root
INPUT=/data00/g2ccbar/mc2018/skim_102725_0/mergedfile.root
OUTPUT=/flavoroutputs/output
./ExecuteDoubleHQtagging \
     --Input $INPUT \
     --IsData 0 \
     --IsPP 1 \
     --MinJetPT 80 \
     --MaxJetPT 100 \
     --ChargeSelection -1 \
     --NbHad 2 \
     --NcHad -1 \
     --DCAString "abs(muDiDxy1) > 0.005 && abs(muDiDxy2) > 0.005" \
     --Output ${OUTPUT}_bb.root

./ExecuteDoubleHQtagging \
     --Input $INPUT \
     --IsData 0 \
     --IsPP 1 \
     --MinJetPT 80 \
     --MaxJetPT 100 \
     --ChargeSelection -1 \
     --NbHad 1 \
     --NcHad -1 \
     --DCAString "abs(muDiDxy1) > 0.005 && abs(muDiDxy2) > 0.005" \
     --Output ${OUTPUT}_b.root

./ExecuteDoubleHQtagging \
     --Input $INPUT \
     --IsData 0 \
     --IsPP 1 \
     --MinJetPT 80 \
     --MaxJetPT 100 \
     --ChargeSelection -1 \
     --NbHad 0 \
     --NcHad 2 \
     --DCAString "abs(muDiDxy1) > 0.005 && abs(muDiDxy2) > 0.005" \
     --Output ${OUTPUT}_cc.root

./ExecuteDoubleHQtagging \
     --Input $INPUT \
     --IsData 0 \
     --IsPP 1 \
     --MinJetPT 80 \
     --MaxJetPT 100 \
     --ChargeSelection -1 \
     --NbHad 0 \
     --NcHad 1 \
     --DCAString "abs(muDiDxy1) > 0.005 && abs(muDiDxy2) > 0.005" \
     --Output ${OUTPUT}_c.root

./ExecuteDoubleHQtagging \
     --Input $INPUT \
     --IsData 0 \
     --IsPP 1 \
     --MinJetPT 80 \
     --MaxJetPT 100 \
     --ChargeSelection -1 \
     --NbHad 0 \
     --NcHad 0 \
     --DCAString "abs(muDiDxy1) > 0.005 && abs(muDiDxy2) > 0.005" \
     --Output ${OUTPUT}_uds.root

root -l -b -q 'plotter.C("flavoroutputs/output","hInvMass",1)'
