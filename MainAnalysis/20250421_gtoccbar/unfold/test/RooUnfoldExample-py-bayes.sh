#!/bin/bash
outfile=RooUnfoldExample.py.bayes.ref
python -u examples/RooUnfoldExample.py bayes > $outfile
bash ref/cleanup.sh $outfile
diff $outfile ref/$outfile
