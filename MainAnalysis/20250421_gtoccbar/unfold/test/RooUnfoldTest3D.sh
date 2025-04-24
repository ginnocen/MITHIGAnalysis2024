#!/bin/bash
outfile=RooUnfoldTest3D.ref
RooUnfoldTest3D > $outfile
bash ref/cleanup.sh $outfile
diff $outfile ref/$outfile
