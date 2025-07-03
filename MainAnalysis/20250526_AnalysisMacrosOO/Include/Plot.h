
#pragma once
#include <iostream>
#include <vector>
#include "EfficiencyResults.h"
#include "Parameters.h"

using namespace std;
void drawROCmultiShapes(
    const vector<EfficiencyResults>& resultsVec,
    const vector<vector<double>>& effVecs,
    const vector<vector<double>>& purityVecs,
    const vector<string>& labels,
    const HistVar1D& hvar); 
