# HF Jet Pefrormance code

This version of the jet performance code is designed to plot the jet energy scale (JES) and jet energy resolution (JER).This uses preliminary MC truth corrections derived by Nick Barnett. This should be updated in the future, but serves as a baseline for the performance. 


## Running the code
After compiling the code using the makefile, to run this code, first produce a root file of histograms by doing.

`bash analysis_example_MC.sh`

then do

`bash plot.sh`

this should run out of the box on svmithi03.