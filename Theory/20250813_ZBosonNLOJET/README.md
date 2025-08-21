# HOW TO: Run Z-boson predictions in pp, OO, PbPb collisions with NLOJET

## Source LHAPDF

We will use LHAPDF installation already available on lxplus 9
```

source /cvmfs/sft.cern.ch/lcg/views/LCG_106/x86_64-el9-gcc13-opt/setup.sh
export LHAPDF_DATA_PATH="$HOME/.local/share/LHAPDF:${LHAPDF_DATA_PATH:-}:/cvmfs/sft.cern.ch/lcg/external/lhapdfsets/current"

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
export PATH="$PATH:<YOUR_PATH_IN_LXPLUS>/nnlojet-v1.0.2/install/bin"
source "<YOUR_PATH_IN_LXPLUS>/nnlojet-v1.0.2/install/share/nnlojet-completion.sh"

```

Then source again the bashrc 

```
source ~/.bashrc 
```

## Now you are ready for running the production

```
cd nnlojet-v1.0.2/examples/Z
```

Copy the cms cards that are provided in the repository under "cardsCMS"
and then you are ready to run 

For each setup configure first the job (e.g. using cmsOO_proton.run)
and then submit the job. 

```
nnlojet-run init cmsOO_proton.run
nnlojet-run submit CMSOO_PROTON/ --job-max-runtime 5m
```

ATTENTION: if you run into a python conflix try the following 

```
# keep the LCG view for compilers/libs if you need them
source /cvmfs/sft.cern.ch/lcg/views/LCG_106/x86_64-el9-gcc13-opt/setup.sh

# drop the Python parts LCG injected
unset PYTHONHOME PYTHONPATH

# now use the dokan venv
source /afs/cern.ch/work/g/ginnocen/testZ/nnlojet-v1.0.2/install/dokan-venv/bin/activate

nnlojet-run init cmsOO_proton.run
```
Below you can find analogous lines for other useful setups:

```
nnlojet-run init cmsOO_nuclear.run
nnlojet-run submit CMSOO_NUCLEAR/ --job-max-runtime 5m

nnlojet-run init cmsOO_nuclearPb.run
nnlojet-run submit CMSOO_NUCLEAR/ --job-max-runtime 5m

```




