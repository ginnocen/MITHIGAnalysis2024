#include "EfficiencyWrite.h"
#include "EfficiencyCounting.h"
#include "EventSelection.h"
#include "EfficiencyResults.h"
#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <vector>

using namespace std;

EfficiencyResults ReturnEfficiency_root(const char* inFilename){
    TFile* file = TFile::Open(inFilename);
    if (!file || file->IsZombie()) {
        cerr << "Error: Cannot open file!" << endl;
        return EfficiencyResults(); // Return empty results
    }
    // Get the tree
    TTree* tree = (TTree*)file->Get("effTree");
    if (!tree) {
        cerr << "Error: Cannot find tree!" << endl;
        return EfficiencyResults(); // Return esmpty results
    }

    // Variables to read
    float_t HFEmax_Offline_min1,HFEmax_Offline_min2,HFEmax_Online_min1,HFEmax_Online_min2;
    float_t Eff_Hijing, Eff_SD;
    float_t Eff_DD, Eff_AlphaO, Purity;
    EfficiencyResults results;
    // Set branch addresses only if the branch exists
    #define SET_BRANCH_IF_EXISTS(branch, ptr) \
        if (tree->GetBranch(branch)) tree->SetBranchAddress(branch, ptr);

    SET_BRANCH_IF_EXISTS("HFEmax_Offline_min1", &HFEmax_Offline_min1);
    SET_BRANCH_IF_EXISTS("HFEmax_Offline_min2", &HFEmax_Offline_min2);
    SET_BRANCH_IF_EXISTS("HFEmax_Online_min1", &HFEmax_Online_min1);
    SET_BRANCH_IF_EXISTS("HFEmax_Online_min2", &HFEmax_Online_min2);
    SET_BRANCH_IF_EXISTS("Eff_Hijing", &Eff_Hijing);
    SET_BRANCH_IF_EXISTS("Eff_SD", &Eff_SD);
    SET_BRANCH_IF_EXISTS("Eff_DD", &Eff_DD);
    SET_BRANCH_IF_EXISTS("Eff_AlphaO", &Eff_AlphaO);
    SET_BRANCH_IF_EXISTS("Purity", &Purity);

    #undef SET_BRANCH_IF_EXISTS

    // Loop over entries
    Long64_t nentries = tree->GetEntries();
    cout << "Number of entries in the tree: " << nentries << endl;
    for (Long64_t i = 0; i < nentries; ++i) {
        tree->GetEntry(i);
        results.HFEmax_Offline_min1.push_back(HFEmax_Offline_min1);
        results.HFEmax_Offline_min2.push_back(HFEmax_Offline_min2);
        results.HFEmax_Online_min1.push_back(HFEmax_Online_min1);
        results.HFEmax_Online_min2.push_back(HFEmax_Online_min2);
        results.EfficiencyHijing.push_back(Eff_Hijing);
        results.EfficiencySD.push_back(Eff_SD);
        results.EfficiencyDD.push_back(Eff_DD); 
        results.EfficiencyAO.push_back(Eff_AlphaO);
        results.Purity.push_back(Purity);
    }

    file->Close();
    delete file;
    return results;

}

float CalculatePurity(vector<float> xsec, 
                    vector<float> Eff,
                    vector<float> backgroundfraction) {
    float denominator =  0;
    for (int i = 0; i < xsec.size(); ++i) {
        denominator += xsec[i] * Eff[i] * backgroundfraction[i];
    }
    float Purity = (xsec[0] * Eff[0]) / denominator;
    return Purity;
}
