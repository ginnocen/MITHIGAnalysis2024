This code is all self-contained, and can simply be run using "./execute.sh" (after making it executable with chmod)
The user will need to specify Data, HIJING, and Angantyr sample skims, as well as TCuts, and output file locations for the final efficiency corrections. Default examples are left in execute.sh. 
Running the script will induce the following chain:

1.) VZReweight: Is used to scale the VZ distribution in MC to data. Produces a 1D reweighting histogram in a rootfile for both Angantyr and HIJING samples. 
VZReweight takes the ratio of the Angantyr and MC samples to Data, and produces plots of the reweighting factors. We scale the MC such that their VZ distributions match the MC V Distribution AFTER event selection (without the VZ component)

2.) MultReweight: Is used to scale the multiplicity distributions of MC to data. As default, the multiplicity distributions for MC are generated WITH the VZ reweighting in step 1. Then additional factors are determined to match the MC to the data. The iterative correction can be disabled by setting "doreweight" to 0, more details are in execute.sh 

3.) TrkPtReweight: Is used to scale the track pT distributions of MC to data. As default the VZ and multiplicity reweighting factors are used when generating the track pT distributions for MC, and they can be disabled by setting "doreweight" to 0 as well. The weights for this quantity should be applied at track-level when making histograms

4.) histmaker.C: Uses an RDataframe to generate fully reweighted MC and Data distributions before and after event selection for Multiplicity, among other variables. These distributions as well as their ratios are saved to a user specified output file. (HISTOGRAMS_DESTINATION in execute.sh) These histograms should encompass the full breadth of options we could use to calculate efficiency, and are also useable inputs for plotting macros etc.

5.) CorrectedPtDist.C: Using a user specified efficiency histogram within HISTOGRAMS_DESTINATION, CorrectedPtDist calculates the track pT spectrum in DATA, with and without using the efficiency to weight each event's contributions. The ratio as well as both distributions are saved to HISTOGRAMS_DESTINATION, and plotted.

6.) FileSaver: Prepares a root file containing config-like information from execute.sh as well as the final efficiency histogram in a form compatible with the efficiency reweighting used in the skimmer. This root file is saved to EFFICIENCY_DESTINATION. Typically this is to a path within MITHIGAnalysis/CommonCode/root/, where it can then be fed into the skimmer.

contact me (Abraham) if you have any questions or issues 