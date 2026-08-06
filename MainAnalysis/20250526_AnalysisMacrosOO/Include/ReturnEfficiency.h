#pragma once
EfficiencyResults ReturnEfficiency_root(const char* inFilename);
float CalculatePurity(vector<Float_t> xsec, 
                    vector<Float_t> Eff,
                    vector<Float_t> backgroundfraction);
