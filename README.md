# MITHIGAnalysis2024

Analysis repository for 2024 analyses. The skimming code is based on the framework developed by Yi Chen (Vanderbilt) and Yen-Jie https://github.com/FHead/PhysicsZHadronEEC.

## Setup 

```
git clone git@github.com:ginnocen/MITHIG_UPCHF2024.git
cd MITHIG_UPCHF2024
git checkout integratedD0
```

Download the example input folder (https://www.dropbox.com/scl/fo/ddtqjgbo2dx85h04d8atv/AFtOyFb4V75RjdrfCQwczRI?rlkey=xx9lshfscik26utchaqjc5apt&dl=0) 
and place it inside MITHIG_UPCHF2024. 

## Run the software 
cd MITHIG_UPCHF2024
source SetupAnalysis.sh

### Run the skimming example:
```
cd SampleGeneration/20241027_ForestReducer_DzeroUPC
bash clean.sh
bash RunExamples.sh

```

### Run the skimming example:
```
cd MainAnalysis/20241029_DzeroUPC
bash clean.sh
cd workflow
bash analysis_example.sh

```




