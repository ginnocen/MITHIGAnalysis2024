Hello! 

To run / alter the systematics, you first should run "doAnalsysAllNeNeSystematics.sh" to generate the dNdpT spectra for each systematic variation

Then, to run and re-run the systematics for different configurations, you can use CompareSystematics.C (already built into the doAll script of course)

The most important hyperparameters can be passed in as arguments: system type / bit to do each correction / root output file name / smoothing type. 
The macro will generate histograms for each of the systematic variations. It will plot 
1.) dNdpT ratio plots for each variation compared to the "unweighted" dNdpT spectrum: i.e. the dNdpT spectrum with Loose, Central, and Tight event selection weights compared to dNdpT with none such weights. They are saved as X_compare_no_weights.pdf for each systematic

2.) dNdpT ratio plots wrt "central" for each variation, saved as *Systematic_comparison.pdf 

3.) Plots with the smoothed and unsmoothed total systematic corrections labelled SystUncerts*.pdf 

4.) The smoothed and unsmoothed (symmetrized) individual systematic histograms are saved to the output root file, as well as the smoothed and unsmoothed total sysematic uncertainty are saved! I tend to copy these to data00 but they can go wherever you please. 



The specifics of the smoothign come from smoothsystematics.C, which includes the "systfit" class used to generate smoothing for the systematic histograms. Within smoothsystematics.C there are individual macros for the polynomial, Gaussian adaptive, and Gaussian flat reweighting schemes. If you want to add another, feel free to do so in this class. 

Two new developments have been encoded in this framework, which you may wish to edit. 
1.) An X value at which we disregard a given systematic is now in place. When declaring "Hist_Symmetrized_Errors" or initializing the "systfit" class, the minimum x value at which we can disregard the systematic (treat it as 0) is in the LAST argument. 

2.) The lower limit of pT at which the smoothing is applied is governed by the 2nd argument in "systfit." Sadly, these are hard coded right now, but can be changed no problem. 

Currently the standard hyperparameter choices are 
Polynomial fit -- deg 3
Gauss Adaptive fit -- 1.0 (std dev scaling)
Gauss Flat fit -- 10 (std)

Finally, you can use "test_smoothing.C" as an example to test new smoothing schemes. Please let me (Abraham) know if you have any questions! 