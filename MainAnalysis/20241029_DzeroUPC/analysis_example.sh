source clean.sh
rm *.root
PTMIN=5
PTMAX=8
YMIN=-1
YMAX=0
TRIGGER=2
ISGAMMAN=1
ISDATA=1
#INPUT=/data/NewSkims23_24/20241106_SkimOldReco23sample_DataAll.root
INPUT=/data/NewSkims23_24/20241107_SkimOldReco23sample_DataAll_RejectMode.root
OUTPUTANALYSIS=outputpt.root
OUTPUTFIT=outputfit.root
./ExecuteDzeroUPC --Input $INPUT --MinDzeroPT $PTMIN --MaxDzeroPT $PTMAX --MinDzeroY $YMIN --MaxDzeroY $YMAX --IsGammaN $ISGAMMAN --TriggerChoice $TRIGGER --IsData $ISDATA --Output $OUTPUTANALYSIS
wait
wait
./MassFit --Input $OUTPUTANALYSIS --Output $OUTPUTFIT
wait