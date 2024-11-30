### Build your area in CMSSW_13_2_4
```
ssh -Y username@lxplus.cern.ch

source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsrel CMSSW_13_2_13
cd CMSSW_13_2_13/src
cmsenv
git cms-init

#Add forest
git cms-merge-topic CmsHI:forest_CMSSW_13_2_X_v2
git remote add cmshi git@github.com:CmsHI/cmssw.git

git clone -b 13XX_miniAOD https://github.com/milanchestojanovic/Bfinder.git --depth 1
source Bfinder/test/DnBfinder_to_Forest_132X_miniAOD.sh
```

## Submit a local foresting job
```
cd CMSSW_13_2_13/src
cmsenv
cmsRun forest_miniAOD_run2_MC_HFcounting.py
```

## Submit crab job
```
voms-proxy-init --rfc --voms cms -valid 192:00
cd CMSSW_13_2_13/src
cmsenv
```
Move to the gtoccbarHI/productions and do:
```
crab submit -c SubmitPbPbMC.py
crab status -d foldername
```

In case you want to kill a job do:
```
crab kill -d foldername
```
