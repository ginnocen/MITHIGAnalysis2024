#include <TFile.h>
#include <TH1F.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TH2D.h>
#include "HistVariables.h"

TH1F* DrawHistFVector(const char* inFileName,
		    const char* branchName,
            HistVar1D hvar,
            bool CCFilterBool = false,
            bool PVFilterBool = false,
            int HFEMaxPlusCut = 0,
            int HFEMaxMinusCut = 0){
    cout << "------- Drawing Histogram of: " << branchName << " -------" << endl;

    // Get the tree from the input file
    TFile* inFile = new TFile(inFileName,"READ");
    if (!inFile || inFile->IsZombie()) {
        cout << "Error: Could not open the file!" << endl;
        return nullptr;
    }

    TTree* tree = (TTree*)inFile->Get("Tree");
    if (!tree) {
        cout << "Error: Could not find the tree!" << endl;
        return nullptr;
    }


    // Set up the branches
    const Long64_t nEvt = tree->GetEntries();
    int CCFilter, PVFilter, eventCounter, subeventCounter;
    float HFEMaxPlus, HFEMaxMinus;
    vector<float>* branch = nullptr;
   // vector<float>* trkPt = nullptr;

    tree->SetBranchAddress(branchName, &branch);
    tree->SetBranchAddress("ClusterCompatibilityFilter", &CCFilter);
    tree->SetBranchAddress("PVFilter", &PVFilter);
    tree->SetBranchAddress("HFEMaxPlus", &HFEMaxPlus);
    tree->SetBranchAddress("HFEMaxMinus", &HFEMaxMinus);
  //  tree->SetBranchAddress("trkPt", &trkPt);

    tree->GetEntry(0);
    if (!branch) {
        cout << "Failed to set branch or branch is empty!" << endl;
        inFile->Close();
        delete inFile;
        return nullptr;
    }

    TH1F* h1 = new TH1F(hvar.xLabel, hvar.histTitle.c_str(), hvar.nbin, hvar.xmin, hvar.xmax);

    eventCounter = 0;
    subeventCounter = 0;
    int CC0Counter = 0;
    int CC1Counter = 0;
    int PV0Counter = 0;
    int PV1Counter = 0;

    cout << "HFEMaxPlusCut: " << HFEMaxPlusCut << endl;
    cout << "HFEMaxMinusCut: " << HFEMaxMinusCut << endl;
    for (int i = 0; i < nEvt; i++) {
        tree->GetEntry(i);
        if (CCFilter == 1) CC1Counter++;
        if (PVFilter == 1) PV1Counter++;
        if (CCFilter == 0) CC0Counter++;
        if (PVFilter == 0) PV0Counter++;

        if (!CCFilterBool) CCFilter = 1;
        if (!PVFilterBool) PVFilter = 1;
        if (!CCFilter || !PVFilter) continue;
        if (HFEMaxMinus < HFEMaxMinusCut && HFEMaxPlus < HFEMaxPlusCut) continue;
        eventCounter++;

        if (branch && branch->size() != 0 ) {
            for (size_t j = 0; j < branch->size(); j++) {
                //if (trkPt->at(j) > 10) continue; // Skip if trkPt is greater than 10
                h1->Fill(branch->at(j));
                subeventCounter++;
            }
        }
    }
    cout << "CCFilter 0: " << CC0Counter << endl;
    cout << "CCFilter 1: " << CC1Counter << endl;

    cout << "PVFilter 0: " << PV0Counter << endl;
    cout << "PVFilter 1: " << PV1Counter << endl;

    cout << "Total nEvt" << nEvt << endl;
    cout << "Total number of events: " << eventCounter << endl;
    cout << "Total number of subevents: " << subeventCounter << endl;
    if (!(hvar.ymin == 0 && hvar.ymax == 0)) {
        h1->GetYaxis()->SetRangeUser(hvar.ymin, hvar.ymax);
    }
    //cout << "XLabel: " << hvar.xLabel << endl;
    h1->SetXTitle(hvar.xLabel); // Set the x-axis label
    h1->SetYTitle(hvar.yLabel); // Set the y-axis label
    // h1.Draw("HIST");
    // canvas->SaveAs(hvar.outFileName.c_str());
    cout << "------- Draw Complete -------" << endl;
    cout << endl;
    return h1;
}