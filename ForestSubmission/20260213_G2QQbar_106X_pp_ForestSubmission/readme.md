
Instructions to set up CMSSW + begin foresting. Foresting config for MC/Data are recipe5_MC.py/recipe5_DATA.py

The corresponding crab configs are labelled here crabConfig_MC.py / crabConfig_DATA.py


```bash
cmssw-el7
cmsrel CMSSW_10_6_46
cd CMSSW_10_6_46/src/
cmsenv
git cms-init
git clone -b bJetAggUL106X_fromMandrenguyen https://github.com/Cristian-Baldenegro/cmssw.git
cd cmssw/
mv * ../
rm -rf cmssw
scram b -j8
voms-proxy-init --voms cms
cmsRun 
```

Note. For the data processing, submissions have been divided into 3 subcategories in order to manage the number of job submissions and the data volume per job correctly. These divisions are represented by the collisionsA.json, collisionsB.json, and collisionsC.json. You can verify that these partition the official golden JSON for the 2017 ppref sample included in fullLumis.json.


