#!/bin/bash
[[ $0 == *.sh ]] && { echo 'usage: `. make.sh` or `source make.sh` instead of `./make.sh`' ; exit 1 ; }

make_libs=0
for arg in "$@"; do
    if [[ "$arg" == "--libs" || "$arg" == "-l" ]]; then
        make_libs=1
        break
    fi
done

set -x
CURRENTFOLDER=$PWD
[[ $make_libs -gt 0 ]] && {
    cd ../../../
    [[ x$ProjectBase == x ]] && source SetupAnalysis.sh
    cd CommonCode/
    rm -rf binary library
    make || { cd $CURRENTFOLDER ; return 2 ; }
}
cd $CURRENTFOLDER/../
make Execute || { cd $CURRENTFOLDER ; return 2 ; }
cd $CURRENTFOLDER

set +x
