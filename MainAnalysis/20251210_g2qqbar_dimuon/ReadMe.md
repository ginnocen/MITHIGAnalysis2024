## G2bbbar Analysis Code

The code is structured to follow 3 macros: 
- Efficiencies.cc
- MakeDistros.cc 
- Yields.cc

They are designed to be run in sequence, and the outputs of one are the inputs to the other. To run the analysis code normally, just execute 
```
./runAnalysis.sh
```
If there is no PR on which to attach comments, I suggest we put questions or things to fix in this google doc (look for a tab that says "code development")
https://docs.google.com/document/d/1eKEmvsQBWXX00XjW_cYrr_Yn29plBR8YZWor37caT10/edit?usp=sharing

### Efficiencies.cc

The purpose of Efficiencies.cc is to derive and save all of the corrections we will be using in the analysis to one place. So far, it is only configured to save the reconstruction efficiency, but I am planning to stage the JES/JER corrections in it, as well as the pTHat reweighting. As inputs it takes
- Input skim file name
- output file name 
- IsData (i.e. not MC) parameter
- pT Bins
- Single muon pT cutoff
- Charge selection (0 -> No selection, 1-> same sign, -1-> opposite sign)
- makeplots (writes auxiliary plots to .pdf files + a plotting directory in the output root file)

The file will produce an output file containing all of the necessary correction histograms. For now it is simply inclusive jet yield (and efficiency and purity) alongside dimuon jet yield (alongside reconstruction efficiency and purity)

### MakeDistros.cc

MakeDistros.cc will produce distributions for the dimuons in DCA (product significance), Mass, and DR between the muons. When operating on a MC sample, the macro will build individual distributions for each jet flavor class to be used as templates in the yield extraction. When operating on a Data sample, the macro will create the overall distributions to be used as an input for template fitting in the next step. Small TNtuples are also created to allow for a binless fitting. Right now it is not used, but MakeDistros macro inherits the efficiency corrections from the previous macro to allow the option to apply these corrections while constructing the template shapes. 

 As inputs the macro uses 
- Input file name
- Efficiency file name (output of Efficiencies macro)
- IsData 
- Charge Selection 
- pT bins
- Single muon pT cutoff
- makeplots 

MakeDistros will save all of the above template and inclusive histograms to a root file 

### Yields.cc

In Yields.cc we extract Light and Heavy flavor yields using a binless 1D template fit to either the DCA product significance or the invariant mass distributions. Note that despite having a bunch of different flavor classes, perform this fitting using only 2 parameters: the amount of LF and the amount of HF (assuming all HF flavor classes have fixed ratios wrt each other) The inputs to this macro are simply the templates and distributions extracted in the previous step. 

- Input file name (this file contains the distributions that will be fitted)
- Template file name 
- Output file name
- Pt bins
- flag to do DCA-based estimation
- flag to do Invariant Mass-based estimation
- makeplots

Yields will save the HF and LF yields, HF fraction, and Total Yield to a root file. 