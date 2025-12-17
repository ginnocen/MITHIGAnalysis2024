#!/bin/bash
[[ $0 == "./make.sh" ]] && { echo 'usage: `. clean.sh` or `source clean.sh` instead of `./clean.sh`' ; exit 1 ; }

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
    make || { cd $CURRENTFOLDER ; exit 2 ; }
}
cd $CURRENTFOLDER/../
make || { cd $CURRENTFOLDER ; exit 2 ; }
cd $CURRENTFOLDER

set +x
