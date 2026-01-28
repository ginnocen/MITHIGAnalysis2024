#pragma once
#include <TMath.h>
#include <iostream>
#include <vector>

using namespace std;

struct EfficiencyResults {
    vector<float> HFEmax_Online_min1;
    vector<float> HFEmax_Online_min2;
    vector<float> HFEmax_Offline_min1;
    vector<float> HFEmax_Offline_min2;

    vector<float> EfficiencyHijing;
    vector<float> EfficiencySD;
    vector<float> EfficiencyDD;
    vector<float> EfficiencyAO;
    vector<float> Purity;

    float xsec_SD;
    float xsec_DD;
    float xsec_had;
    float xsec_alphaO;
};
