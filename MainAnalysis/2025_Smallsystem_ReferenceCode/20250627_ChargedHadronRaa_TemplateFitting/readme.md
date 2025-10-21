
This repo works like the typical analysis repos do, with a couple caveats

1.) Before you do anything, run cmsenv within the /src directory of any CMSSW after 14
2.) To generate the root files containing histograms for the root fitting, run ./analysis_example.sh. Naturally, you can change the contents of these files by adding or removing histograms from the workload in the ChargedHadronRaa.cpp
3.) Move the output files (there should be 1 for each MC sample) to templatetrees/ 
4.) Run 'root histrun.C' with your desired branch quantity to generate a test histogram 
5.) to run the template fitting run ./fit.sh 

Best of luck

