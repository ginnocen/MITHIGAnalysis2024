#!/bin/bash
python3 MLoptimization.py --input_file_mc /data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/SkimsMC/20241216_v1_filelist20241216_Pthat2_ForceD0Decay100M_BeamA_v1/mergedfile.root --tree_name Tree --output_csv output_pt_p1p2_y_m1p1.cvs --output_root output_pt_p1p2_y_m1p1.root --output_model XGBoostMConly_pt_p1p2_y_m1p1.json --ptmin 1 --ptmax 2 --ymin -1 --ymax 1
