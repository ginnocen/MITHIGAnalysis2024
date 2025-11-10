source clean.sh
rm *.root
INPUT=/data00/g2ccbar/mc2018/skim_102725_0/mergedfile.root
./ExecuteDoubleHQtagging --Input $INPUT --MinJetPT 80 --Output output_DoubleHQtagging.root
wait
