#!/bin/bash
outfile=RooUnfoldExample.py.svd.ref
python -u examples/RooUnfoldExample.py svd > $outfile
bash ref/cleanup.sh $outfile
diff $outfile ref/$outfile
