source clean.sh
rm *.root

# Create output directory
OUTPUTDIR="results_data_nodca"
mkdir -p ${OUTPUTDIR}

#INPUT=/data00/g2ccbar/mc2018/skim_102725_all/mergedfile.root
INPUT=/data00/g2ccbar/data2018/skim_110525_0/mergedfile.root
OUTPUT=${OUTPUTDIR}/output_DoubleHQtagging.root
./ExecuteDoubleHQtagging \
     --Input $INPUT \
     --IsData 1 \
     --IsPP 1 \
     --MinJetPT 80 \
     --MaxJetPT 120 \
     --ChargeSelection -1 \
     --DCAString "abs(muDiDxy1 / muDiDxy1Err) > -1 && abs(muDiDxy2 / muDiDxy2Err) > -1" \
     --Output ${OUTPUT}

./MassFit --Input ${OUTPUT} --Output ${OUTPUTDIR}/fit_result.root --Label ${OUTPUTDIR}/DoubleHQtagging --MinJetPT 80 --MaxJetPT 120
root -l -b -q "plotter.C(\"${OUTPUT}\",\"${OUTPUTDIR}\")"
wait
