# How to generate skims for the OO charged hadron RAA analysis


## Load a ROOT environment
Go to [link](https://github.com/ginnocen/MITHIGAnalysis2024/blob/ecc108f43208f144118035f8ea821b6562b7b0ee/SampleGeneration/20250518_ForestReducer_RAAOxygenOxygen/clean.sh#L1C1-L7C1) and choose if you want to run it on a lxplus server (e.g. mithi03) or locally.
If you run on svmithi03 you need to create a CMSSW environment as requested in L2. Otherwise, if you are not running on svmithi03, you must have a working ROOT installation on your PC. 
## Set your input files
Go to [link](https://github.com/ginnocen/MITHIGAnalysis2024/blob/ecc108f43208f144118035f8ea821b6562b7b0ee/SampleGeneration/20250518_ForestReducer_RAAOxygenOxygen/clean.sh#L1C1-L7C1) and define the input file locations (Forest) 
and the output folder for the skims. **IMPORTANT**: make sure you dont overwrite the files in the standard mithi03 paths. 

## Run 
To produce OO samples you run 
```
bash RunExamples.sh
```

To produce pp samples you run
```
bash RunExamplesPP.sh
```


