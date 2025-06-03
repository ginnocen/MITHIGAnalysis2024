#include <iostream>
#include <TFile.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLegend.h>

using namespace std;

#include <utility>

pair<double, int> counting(const char* inFileName,
            bool CCFilterBool = false,
            bool PVFilterBool = false,
            bool nVtxBool = false,
            bool VZbool = false) {
    cout << "------- Counting  -------" << endl;

    TFile* inFile = new TFile(inFileName,"READ");
    if (!inFile || inFile->IsZombie()) {
        cout << "Error: Could not open the file!" << endl;
        return std::make_pair(0.0, 0);
    }

    TTree* tree = (TTree*)inFile->Get("Tree");
    if (!tree) {
        cout << "Error: Could not find the tree!" << endl;
        return std::make_pair(0.0, 0);
    }
    
    int CCFilter, PVFilter, nVtx;
    float VZ;

    tree->SetBranchAddress("ClusterCompatibilityFilter", &CCFilter);
    tree->SetBranchAddress("PVFilter", &PVFilter);
    tree->SetBranchAddress("nVtx", &nVtx);
    tree->SetBranchAddress("VZ", &VZ);

    const Long64_t nEvt = tree->GetEntries();

    int eventCounter = 0, subeventCounter = 0;
    int CC0Counter = 0, CC1Counter = 0, PV0Counter = 0, PV1Counter = 0, nVtxCounter = 0, VZCounter = 0;

    for (int i = 0; i < nEvt; i++) {
        tree->GetEntry(i);
        if (CCFilter == 1) CC1Counter++;
        if (PVFilter == 1) PV1Counter++;
        if (nVtx > 0) nVtxCounter ++;     
        if (abs(VZ) < 15) VZCounter++;

    if ((CCFilterBool && CCFilter == 0) ||
        (PVFilterBool && PVFilter == 0) ||
        (nVtxBool && nVtx <= 0) ||
        (VZbool && abs(VZ) > 15)) {
            continue;
        }
        eventCounter++;
    }
    cout << "CCFilter Passed: " << CC1Counter << endl;
    cout << "PVFilter Passed: " << PV1Counter << endl;
    cout << "nVtx Passed : " << nVtxCounter << endl;
    cout << "VZ Passed : " << VZCounter << endl;
    double ratio = static_cast<double>(eventCounter) / nEvt;
    cout << "Ratio between Original and Filtered: " << ratio << endl;

    cout << "Total number of events: " << nEvt << endl;
    cout << "Total number of events after filter: " << eventCounter << endl;
    cout << endl;

    cout << "------- Count Complete -------" << endl;
    return make_pair(ratio, eventCounter);
}


void run(const char* inFileName) {
    // Create a histogram with 4 bins
    TH1F* h = new TH1F("h", "Percent of Events with each Filter", 5, 0.5, 5.5);
    TH1F* h2 = new TH1F("h2", "Number of Events with each Filter", 5, 0.5, 5.5);

    // Call counting and fill each bin

    h->SetBinContent(1, counting(inFileName, false, false, false,false).first);
    h->SetBinContent(2, counting(inFileName, true, false,false,false).first);
    h->SetBinContent(3, counting(inFileName, true, true,false,false).first);
    h->SetBinContent(4, counting(inFileName, true, true,true,false).first);
    h->SetBinContent(5, counting(inFileName, true, true,true,true).first);

    h2->SetBinContent(1, counting(inFileName, false, false, false,false).second);
    h2->SetBinContent(2, counting(inFileName, true, false,false,false).second);
    h2->SetBinContent(3, counting(inFileName, true, true,false,false).second);
    h2->SetBinContent(4, counting(inFileName, true, true,true,false).second);
    h2->SetBinContent(5, counting(inFileName, true, true,true,true).second);


/*
    h->SetBinContent(1, counting(inFileName, false, false, false,false).second);
    h->SetBinContent(2, counting(inFileName, true, false,false,false).second);
    h->SetBinContent(3, counting(inFileName, false, true,false,false).second);
    h->SetBinContent(4, counting(inFileName, false, false,true,false).second);
    h->SetBinContent(5, counting(inFileName, false, false,false,true).second);
*/

    // Optionally set bin labels
    h->GetXaxis()->SetBinLabel(1, "No Selection");
    h->GetXaxis()->SetBinLabel(2, "CC Filter == 1");
    h->GetXaxis()->SetBinLabel(3, "CC and PV Filter == 1");
    h->GetXaxis()->SetBinLabel(4, "CC, PV filter and nVtx > 0");
    h->GetXaxis()->SetBinLabel(5, "CC, PV, nVtx > 0, abs(VZ) < 15");

    h2->GetXaxis()->SetBinLabel(1, "No Selection");
    h2->GetXaxis()->SetBinLabel(2, "CC Filter");
    h2->GetXaxis()->SetBinLabel(3, "PV Filter");
    h2->GetXaxis()->SetBinLabel(4, "nVtx > 0");
    h2->GetXaxis()->SetBinLabel(5, "abs(VZ) < 15");

    // Draw the histogram
    TCanvas* c1 = new TCanvas("c1", "Percent of Events with each Filter (SD)", 800, 600);
    c1->SetBottomMargin(0.18); // Increase bottom margin for axis labels
    c1->SetRightMargin(0.18); // Increase right margin
    h->Draw("HIST TEXT0");
    h->GetYaxis()->SetRangeUser(0, 1);
    gStyle->SetOptStat(0);
    c1->SaveAs("counting_results_DD_ratio.png");

    TCanvas* c2 = new TCanvas("c2", "Percent of Events with each Filter (SD)", 800, 600);
    c2->SetBottomMargin(0.18); // Increase bottom margin for axis labels
    c2->SetRightMargin(0.18); // Increase right margin
    h2->Draw("HIST TEXT0");
    gStyle->SetOptStat(0);
    c2->SaveAs("counting_results_DD_counts.png");


}

