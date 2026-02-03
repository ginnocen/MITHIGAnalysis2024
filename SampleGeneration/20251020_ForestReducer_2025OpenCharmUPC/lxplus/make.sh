#!/bin/bash
[[ $0 == "./make.sh" ]] && { echo 'usage: `. make.sh` or `source make.sh` instead of `./make.sh`' ; exit 1 ; }

make_libs=0
make_main=0
for arg in "$@"; do
    if [[ "$arg" == "--libs" ]]; then
        make_libs=1
    elif [[ "$arg" == "--main" ]]; then
        make_main=1
    fi
done

set -x

CURRENTFOLDER=$PWD
cd ../../../
[[ x$ProjectBase == x ]] && source SetupAnalysis.sh

[[ $make_libs -gt 0 ]] && {
    cd CommonCode/
    rm -rf binary library
    make || { cd $CURRENTFOLDER ; return 2 ; }
}

cd $CURRENTFOLDER/../

[[ $make_main -gt 0 ]] && {
    make || { cd $CURRENTFOLDER ; return 2 ; }
}

cd $CURRENTFOLDER

set +x
