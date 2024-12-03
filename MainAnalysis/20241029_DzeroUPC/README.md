### Preparing the micro tree for analysis
- Macro: `DzeroUPC.cpp`
	- Applies all the event selections and D selections
	- Creates a micro tree output in the specific (pt,y) bin
	- The format of the micro tree output contains Dmass and Dgen info

### Fit
- Macro: `massfit.C`
	- Perform the 1D mass fit
	- Takes two essential inputs: `dataInput` and `mcInput`
	- Determines the shape parameters for different components by doing the MC fits first. If one would want to specify the prefitted parameters, one can also replace one/some of the `sigswpInput`, `KKmcInput`, `pipimcInput`

### Controlling shell scripts
- Simple example:
	```bash
	bash analysis_example.sh
	```
- Perform the yield extraction study on all the (pt,y) bins using the cards (`sampleSettings/`, `fitSettings/`):
	- The output structure of a specific study will be arranged as such:
		```
		<MicroTreeDir>/
		 ├── sampleConfig.json: the config card in the `sampleSettings` folder, which controls the execution of `DzeroUPC.cpp` 
		 ├── <pt_xxx_y_xxx>/
		 │			└── <FitDir>/
		 │						└── fitConfig.json:  the config card in the `fitSettings` folder, which controls the execution of `massfit.C`
		 ├── ...
		 └── <pt_xxx_y_xxx>/

		 ### <MicroTreeDir> and <FitDir> can be user-specified tag names that benchmark the different productions / studies
		```
	- Usage:
		```bash
		make
		bash makeMicroTree.sh sampleSettings/xxx.json
		### This will create a <MicroTreeDir>/, and the input card argument would be copied under this folder as sampleConfig.json
		bash massfit.sh fitSettings/xxx.json
		### This will create a <FitDir>/ for each (pt,y)-binned micro tree sample, and the input card argument would be copied under this folder as fitConfig.json
		```
