# Code for fitting the A dependence of the energy loss

This code can perform fits to the A distribution of the charged hadron RAA across different colliding systems;

## input files
- **ptBinned_RAAVsA.root**: RAA and uncertainties plotted vs A
- **ptBinned_RAAVsA_Pow1-3.root**: RAA and uncertainties plotted vs A^1/3
- **ptBinned_RAAVsA_Pow2-3.root**: RAA and uncertainties plotted vs A^2/3

## macros
- **fitFormatAfixed.C** performs the fit at a fixed value of the exponential dependence A^(alpha). It is designed to work with alpha=1 and 2. 
- **fitFormatADepFloating.C**: perform an exponential fit, leaving the exponential alpha floating in the fit.

## backup folder
It contains obsolete macros, which were used in previous versions of the input data. They are kept only for reproducibility. 

