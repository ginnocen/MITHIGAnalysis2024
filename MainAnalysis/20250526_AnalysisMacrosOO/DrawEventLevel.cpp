#include <TFile.h>
#include <TH1F.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TH2D.h>
#include "HistVariables.h"

TH1F* GenHistInt(const char* inFileName,const char* branchName,HistVar1D hvar,bool Cuts = false){
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
    int branch;
    int eventCounter = 0;
    int CCFilter, PVFilter, nVtx;

    tree->SetBranchAddress(branchName, &branch);
    tree->SetBranchAddress("ClusterCompatibilityFilter", &CCFilter);
    tree->SetBranchAddress("PVFilter", &PVFilter);
    tree->SetBranchAddress("nVtx", &nVtx);

    TH1F* h1 = new TH1F(hvar.xLabel, hvar.histTitle.c_str(), hvar.nbin, hvar.xmin, hvar.xmax);
    
    for (int i = 0; i < nEvt; i++) {
        tree->GetEntry(i);
        if (Cuts == true) {
            if (CCFilter == 0 || PVFilter == 0 || nVtx <= 0) {
                continue;
            }
        }
        h1->Fill(branch);
        eventCounter++;
    }

    cout << "Total nEvt: " << nEvt << endl;
    cout << "Total number of events: " << eventCounter << endl;
    cout << endl;

    if (!(hvar.ymin == 0 && hvar.ymax == 0)) {
        h1->GetYaxis()->SetRangeUser(hvar.ymin, hvar.ymax);
    }
    h1->SetXTitle(hvar.xLabel); // Set the x-axis label
    h1->SetYTitle(hvar.yLabel); // Set the y-axis label
    cout << "------- Draw Complete -------" << endl;
    cout << endl;

    return h1;
}

TH1F* GenHistFloat(const char* inFileName,const char* branchName,HistVar1D hvar,bool Cuts = false){
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
    Float_t branch;
    int eventCounter = 0;
    int CCFilter, PVFilter, nVtx;

    tree->SetBranchAddress(branchName, &branch);
    tree->SetBranchAddress("ClusterCompatibilityFilter", &CCFilter);
    tree->SetBranchAddress("PVFilter", &PVFilter);
    tree->SetBranchAddress("nVtx", &nVtx);


    TH1F* h1 = new TH1F(hvar.xLabel, hvar.histTitle.c_str(), hvar.nbin, hvar.xmin, hvar.xmax);
    for (int i = 0; i < nEvt; i++) {
        tree->GetEntry(i);
        if (Cuts == true) {
            if (CCFilter == 0 || PVFilter == 0 || nVtx <= 0) {
                continue;
                cout << "Event " << i << " skipped due to cuts." << endl;
            }
        }
        h1->Fill(branch);
        eventCounter++;
    }

    cout << "Total nEvt: " << nEvt << endl;
    cout << "Total number of events: " << eventCounter << endl;
    cout << endl;

    if (!(hvar.ymin == 0 && hvar.ymax == 0)) {
        h1->GetYaxis()->SetRangeUser(hvar.ymin, hvar.ymax);
    }

    h1->SetXTitle(hvar.xLabel); // Set the x-axis label
    h1->SetYTitle(hvar.yLabel); // Set the y-axis label
    cout << "------- Draw Complete -------" << endl;
    cout << endl;
    return h1;
}


void DrawHist(TH1F* hist, CanvasVar cvar) {
    TCanvas* canvas = new TCanvas("canvas", cvar.canvasName, 800, 800);
    hist->Draw("HIST");

    canvas->SetLeftMargin(0.16); // Increase right margin
    canvas->SaveAs(cvar.outFileName.c_str());
    
}