
1. Generate your "subskim" files using Cull.sh.
   Simply place the folder location of your desired MC/Data skim in Cull.sh line 7
   (i.e. INPUT="/data00/g2ccbar/data2018/minBias_skims_02052025/mergedfileData_02042025_mbAll.root"). Then, run "./cull.sh odata.root" or "./cull.sh omc.root"
   depending on whether your skim was Data or MC. Once you have both odata.root and omc.root filled, proceed to step 2.

2. Simply run "root QA.C" or equivalent. This will produce a giant folder with PDFs of every plot made in its routine. A root file containing histograms is in development as an alternative output mode. Due to memory leaks (which I am fixing), some plots will randomly not draw due to axis spread considerations. Running QA.C again is a surefire way to bypass this issue. 
