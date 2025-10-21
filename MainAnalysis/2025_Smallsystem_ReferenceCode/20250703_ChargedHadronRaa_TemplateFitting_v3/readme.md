Workflow

Inspect ChargedHadronRaa.cpp and verify that your desired trees or histograms are present in the file
Then, run ./analysis_example.sh to obtain the disred histograms for each sample. 
One can move these samples to /templates, where macros exist that can generate test examples for these templates
Either with a real data sample in hand, or a locally generated one, you will run "./fit.sh" after specifying the arguments 
which involve describing which files contain data versus templates, and executes the fitting routine. 

After all of this, a large amount of plots and analysis materials will be produced to evaluate the relative fraction as
well as the goodness of fit,
