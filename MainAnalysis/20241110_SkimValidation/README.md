### Skim Format Validation
- Usage:
	```bash
	# validate the old and new skims (data) with event selections
	make ValidateData
	# validate the old and new skims (data) with event selections + D selections
	make ValidateDataWDsel
	# validate the old and new skims (MC) with event selections
	make ValidateMC
	# validate the old and new skims (MC) with event selections + D selections
	make ValidateMCWDsel
	```
	- Note that the above makefile rule will create an out/DumpSkim.diff file and display the content with `less out/DumpSkim.diff`.
	If the file is empty that means things are aligned btw the two skims, and one can quit 'less out/DumpSkims.diff' by entering 'q'.

- Macro:
	- `DumpNewSkim.cpp`, `DumpOldSkim.cpp`: print and store histograms for comparison

	- `Validate.cpp`: comparison btw two dump output root files

- `ValidateHist.h`:
	Defining histograms for validation

