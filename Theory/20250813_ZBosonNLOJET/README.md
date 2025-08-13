# HOW TO: Run Z-boson predictions in pp, OO, PbPb collisions with NLOJET

## Source LHAPDF

We will use LHAPDF installation already available on lxplus 9
```

source /cvmfs/sft.cern.ch/lcg/views/LCG_106/x86_64-el9-gcc13-opt/setup.sh
export LHAPDF_DATA_PATH=/cvmfs/sft.cern.ch/lcg/external/lhapdfsets/current:${HOME}/.local/share/LHAPDF
```
To check that the installation is successful, do:

```

which lhapdf
lhapdf --version
lhapdf-config --prefix

```

## (Optional-- skip unless you know what you are doing ) if you need to add PDFs that are now yet available 
```
grep PDF my_run_card.run
lhapdf list | grep <PDFNAME> || lhapdf install <PDFNAME>

```

## Copy and install nnlojet-v1.0.2 

```
mkdir StudyZBoson
cd StudyZBoson
cp /afs/cern.ch/work/g/ginnocen/public/nnlojet-v1.0.2.tar .
tar xf nnlojet-v1.0.2.tar
cd nnlojet-v1.0.2
mkdir build
cd build
cmake .. -DLHAPDF_ROOT_DIR=$(lhapdf-config --prefix) -DCMAKE_INSTALL_PREFIX=$PWD/../install
make -j20 
make install 
```

Add these lines to your bashrc 

```
export PATH="$PATH:/afs/cern.ch/work/g/ginnocen/testZ/nnlojet-v1.0.2/install/bin"
source "/afs/cern.ch/work/g/ginnocen/testZ/nnlojet-v1.0.2/install/share/nnlojet-completion.sh"

```

Then source again the bashrc 

```
source ~/.bashrc 
```

## Now you are ready for running the production



