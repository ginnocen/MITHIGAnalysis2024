#pragma once
#include <TMath.h>
#include <iostream>
#include <vector>

using namespace std;

struct EfficiencyResults {
    vector<double> x;
    vector<double> EfficiencyHijingAND;
    vector<double> EfficiencyHijingOR;

    vector<double> EfficiencySDAND;
    vector<double> EfficiencySDOR;

    vector<double> BKGRejectionAND;
    vector<double> BKGRejectionOR;

    vector<double> PurityAND;
    vector<double> PurityOR;
    float xsec_SD;
    float xsec_DD;
    float xsec_had;
    int N;
    float xMax;
    int coincidence;
};
