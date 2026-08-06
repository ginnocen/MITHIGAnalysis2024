#pragma once

#include "Parameters.h"
#include <vector>

using namespace std;

std::string WriteEfficiencyRoot(
    FileNames filename,
    vector<Parameters> par_vector,
    int trkptcut,
    string outfolder,
    string outFileAdd);