#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <limits>

using namespace std;
int FindMaxInt(const char* inFileName, const char* branch) {
    TFile file(inFileName, "READ");
    TTree* tree = (TTree*)file.Get("Tree");
    if (!tree) {
        cerr << "Error: Tree not found in file." << endl;
        return 0;
    }

    int maxVal = numeric_limits<int>::min();
    int value;

    tree->SetBranchAddress(branch, &value);
    Long64_t nEntries = tree->GetEntries();

    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
        if (value > maxVal) maxVal = value;
    }
    cout << "Max value of branch " << branch << ": " << maxVal << endl;
    return maxVal;
}

int FindMaxFloat(const char* inFileName, const char* branch) {
    TFile file(inFileName, "READ");
    TTree* tree = (TTree*)file.Get("Tree");
    if (!tree) {
        cerr << "Error: Tree not found in file." << endl;
        return 0;
    }

    float maxVal = numeric_limits<float>::min();
    float value;

    tree->SetBranchAddress(branch, &value);
    Long64_t nEntries = tree->GetEntries();

    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
        if (value > maxVal) maxVal = value;
    }
    cout << "Max value of branch " << branch << ": " << maxVal << endl;
    return maxVal;
}

int FindMinInt(const char* inFileName, const char* branch) {
    TFile file(inFileName, "READ");
    TTree* tree = (TTree*)file.Get("Tree");
    if (!tree) {
        cerr << "Error: Tree not found in file." << endl;
        return 0;
    }

    int minVal = numeric_limits<int>::max();
    int value;

    tree->SetBranchAddress(branch, &value);
    Long64_t nEntries = tree->GetEntries();

    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
        if (value < minVal) minVal = value;
    }
    cout << "Min value of branch " << branch << ": " << minVal << endl;
    return minVal;
}

float FindMinFloat(const char* inFileName, const char* branch) {
    TFile file(inFileName, "READ");
    TTree* tree = (TTree*)file.Get("Tree");
    if (!tree) {
        cerr << "Error: Tree not found in file." << endl;
        return 0;
    }

    float minVal = numeric_limits<float>::max();
    float value;

    tree->SetBranchAddress(branch, &value);
    Long64_t nEntries = tree->GetEntries();

    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
        if (value < minVal) minVal = value;
    }
    cout << "Min value of branch " << branch << ": " << minVal << endl;
    return minVal;
}