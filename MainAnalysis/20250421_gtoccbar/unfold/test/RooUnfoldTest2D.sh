#!/bin/bash
outfile=RooUnfoldTest2D.ref
RooUnfoldTest2D > $outfile
bash ref/cleanup.sh $outfile
diff $outfile ref/$outfile
