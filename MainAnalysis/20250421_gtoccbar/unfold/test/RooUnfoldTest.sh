#!/bin/bash
outfile=RooUnfoldTest.ref
RooUnfoldTest > $outfile
bash ref/cleanup.sh $outfile
diff $outfile ref/$outfile
