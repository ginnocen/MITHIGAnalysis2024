#!/bin/bash
outfile=RooUnfoldExample.cxx.ref
RooUnfoldExample > $outfile
bash ref/cleanup.sh $outfile
diff $outfile ref/$outfile
