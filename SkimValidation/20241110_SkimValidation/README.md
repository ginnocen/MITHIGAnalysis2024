### Skim Format Validation
- Usage:
	- Comparing between different skimmed forests with the new skim format:
		```
		bash ValidateDataWDsel_NewSkim.sh <Input1> <Input2>
		```

	- Running with existing rules:
		```bash
		# validate the old and new skims (data) with event selections
		make ValidateData
		# validate the old and new skims (data) with event selections + D selections
		make ValidateDataWDsel
		# validate the old and new skims (MC) with event selections
		make ValidateMC
		# validate the old and new skims (MC) with event selections + D selections
		make ValidateMCWDsel
		# validate the two new skims (data) with event selections + D selections
		make ValidateDataWDsel_NewSkim20241115
		```
		Please update the rule in the makefile if the validation is an important one, for bookkepping.

	- Note that the above makefile rule will create an out/DumpSkim.diff file and display the content with `less out/DumpSkim.diff`.
	If the file is empty that means things are aligned btw the two skims, and one can quit 'less out/DumpSkims.diff' by entering 'q'.

- Macro:
	- `DumpNewSkim.cpp`, `DumpOldSkim.cpp`: print and store histograms for comparison

	- `Validate.cpp`: comparison btw two dump output root files

- `ValidateHist.h`:
	Defining histograms for validation

