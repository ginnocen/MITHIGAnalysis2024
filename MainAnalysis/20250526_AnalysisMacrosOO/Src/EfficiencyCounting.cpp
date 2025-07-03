#include <TH2D.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLegend.h>
#include <utility>
#include <tuple>
#include <string>
#include <TLeaf.h>
#include "Messenger.h"
#include "EfficiencyCounting.h"
#include "Parameters.h"
#include "EventSelection.h"
using namespace std;

pair<double,int> calculateEfficiency(ChargedHadronRAATreeMessenger *ch,
        const Parameters &par,
        bool IsHijing,
        int trkPtCut){

  int evtPassedSel = 0; 
  int evtTotal = ch->GetEntries();
  int denominator = 0;
for (int i = 0; i < ch->GetEntries(); ++i) {
    ch->GetEntry(i);
    if (trkPtCut != -1 && std::none_of(ch->trkPt->begin(), ch->trkPt->end(),
                                [trkPtCut](float pt) { return pt > trkPtCut; })) continue; 

    denominator++;

    if (!eventSelection(ch, par)) continue;
    if (IsHijing && ch->Npart <= 1) continue; // Apply Npart cut if Hijing sample
    evtPassedSel++;
}

double ratio = static_cast<double>(evtPassedSel) / denominator;

cout << "Parameters used for counting:" << endl;
cout << "HFEmax_Online_min1: " << par.HFEmax_Online_min1 << " GeV" << endl;
cout << "HFEmax_Online_min2: " << par.HFEmax_Online_min2 << " GeV" << endl;
cout << "HFEmax_Offline_min1: " << par.HFEmax_Offline_min1 << " GeV" << endl;  
cout << "HFEmax_Offline_min2: " << par.HFEmax_Offline_min2 << " GeV" << endl;

cout << "Selection Efficiency: " << ratio*100 << endl;
cout << "Number of Event: " << evtTotal << endl;
cout << "Number of Event after ptCuts: " << trkPtCut << ": " << denominator << endl;
cout << "Number of event after cuts : " << evtPassedSel << endl;
cout << endl;
cout << "------- Count Complete -------" << endl;
return make_pair(ratio, evtPassedSel);
}

pair<double, int> countingTrkptAsymmVariable(const char* inFileName, 
    const char* varNamePlus,
    const char* varNameMinus,
    float cutplus, 
    float cutminus,
    bool cutBool, 
    bool HijingBool,
    int trkptcut){
    
    cout << Form("------- Counting for %s %f and %s %f and ptcut %d -------",
        varNamePlus, cutplus, varNameMinus, cutminus, trkptcut) << endl;

    TFile* inFile = new TFile(inFileName,"READ");
    if (!inFile || inFile->IsZombie()) {
        cout << "Error: Could not open the file!" << endl;
        return make_pair(0.0, 0);
    }

    TTree* tree = (TTree*)inFile->Get("Tree");
    if (!tree) {
        cout << "Error: Could not find the tree!" << endl;
        return make_pair(0.0, 0);
    }
    
    int CCFilter, PVFilter, nVtx;
    float HFEMaxPlus, HFEMaxMinus, VZ, Npart, branchPlus, branchMinus;
    vector<float>* trkPt = nullptr;
    const Long64_t nEvt = tree->GetEntries();
    int eventCounter = 0, subeventCounter = 0, denominator = 0;
    bool trkPtBool = false;

    tree->SetBranchAddress("ClusterCompatibilityFilter", &CCFilter);
    tree->SetBranchAddress("PVFilter", &PVFilter);
    tree->SetBranchAddress("nVtx", &nVtx);
    tree->SetBranchAddress("VZ", &VZ);
    tree->SetBranchAddress("HFEMaxPlus", &HFEMaxPlus);
    tree->SetBranchAddress("HFEMaxMinus", &HFEMaxMinus);
    tree->SetBranchAddress("trkPt",&trkPt); 
    tree->SetBranchAddress("Npart", &Npart);

    tree->SetBranchAddress(varNamePlus, &branchPlus);
    tree->SetBranchAddress(varNameMinus, &branchMinus);

    for (int i = 0; i < nEvt; i++) {
        tree->GetEntry(i);
        trkPtBool = false;
        if (trkptcut == -1){
            trkPtBool = true;
        }
        for (int j = 0; j < trkPt->size(); j++) {
            if (trkPt->at(j) > trkptcut) {
                trkPtBool = true;
            }
        }
        if (trkPtBool) {
            denominator++;
            if (CCFilter == 1 && PVFilter == 1 && nVtx > 0 && abs(VZ) < 15 && (!HijingBool || Npart > 1)){
                if (cutBool == false){
                    if ((branchPlus > cutplus || branchMinus > cutminus)|| (branchPlus > cutminus || branchMinus > cutplus)){
                        eventCounter++;
                    }
                }
                else if (cutBool == true){
                    if ((branchPlus > cutplus && branchMinus > cutminus)|| (branchPlus > cutminus && branchMinus > cutplus)){
                        eventCounter++;
                    }
                }
            }
        }
    }

    double ratio = static_cast<double>(eventCounter) / denominator;
    cout << varNamePlus << ": " << cutplus << " GeV" << endl;
    cout << varNameMinus << ": " << cutminus << " GeV" << endl;
    cout << "Event Selection Efficiency: " << ratio << endl;
    cout << "Total number of events: " << nEvt << endl;
    cout << "Denominator after trkptcuts: " << denominator << ": " << denominator << endl;
    cout << "Total number of events after HFEcut : " << eventCounter << endl;
    cout << endl;
    cout << "------- ------- -------" << endl;
    inFile->Close();
    delete inFile;

    return make_pair(ratio, eventCounter);
}

pair<double, int> countingTrkptVariable(
    const char*     inFileName, 
    const char*     varNamePlus,
    const char*     varNameMinus,
    float           cut, 
    bool            cutBool, 
    bool            HijingBool,
    int             trkptcut) {

    cout << Form("------- Counting for %s and %s > %f and ptcut %d -------",
        varNamePlus, varNameMinus, cut,trkptcut) << endl;
    cout << "File Name: " << inFileName << endl;

    TFile* inFile = new TFile(inFileName,"READ");
    if (!inFile || inFile->IsZombie()) {
        cout << "Error: Could not open the file!" << endl;
        return make_pair(0.0, 0);
    }

    TTree* tree = (TTree*)inFile->Get("Tree");
    if (!tree) {
        cout << "Error: Could not find the tree!" << endl;
        return make_pair(0.0, 0);
    }
    
    int CCFilter, PVFilter, nVtx;
    Float_t varPlusF, varMinusF;
    Int_t varPlusI, varMinusI;

    float VZ, leadingPtEta1p0_sel, Npart, Ncoll;
    vector<float>* trkPt = nullptr;
    const Long64_t nEvt = tree->GetEntries();
    int eventCounter = 0, subeventCounter = 0, denominator = 0;
    bool trkPtBool = false;

    tree->SetBranchAddress("ClusterCompatibilityFilter", &CCFilter);
    tree->SetBranchAddress("PVFilter", &PVFilter);
    tree->SetBranchAddress("nVtx", &nVtx);
    tree->SetBranchAddress("VZ", &VZ);
    tree->SetBranchAddress("leadingPtEta1p0_sel",&leadingPtEta1p0_sel);
    tree->SetBranchAddress("trkPt",&trkPt); 
    tree->SetBranchAddress("Npart", &Npart);
    tree->SetBranchAddress("Ncoll", &Ncoll);
    
    TLeaf *leafPlus  = tree->GetLeaf(varNamePlus);   // e.g. "HFEMaxPlus"
    TLeaf *leafMinus = tree->GetLeaf(varNameMinus);  // e.g. "HFEMaxMinus"
    int varPlus, varMinus;
    int failedPrintCount = 0;
    for (int i = 0; i < nEvt; i++) {
        tree->GetEntry(i);
        varPlus  = leafPlus ->GetValue();   // converted to Double_t automatically
        varMinus = leafMinus->GetValue();
        trkPtBool = false;
        if (trkptcut == -1){
            trkPtBool = true;
        }
        for (int j = 0; j < trkPt->size(); j++) {
            if (trkPt->at(j) > trkptcut) {
                trkPtBool = true;
            }
        }
        bool passed = false;
        if (trkPtBool) {
            denominator++;
            if (CCFilter == 1 && PVFilter == 1 && nVtx > 0 && abs(VZ) < 15  && (!HijingBool || Npart > 1)) {
                if (cutBool == false){
                    if (varPlus > cut || varMinus > cut){
                        eventCounter++;
                        passed = true;
                    }
                }
                else if (cutBool == true){
                    if (varPlus > cut && varMinus > cut){
                        eventCounter++;
                        passed = true;
                    }
                }
            }
        }
    }

    double ratio = static_cast<double>(eventCounter) / denominator;
    cout << "Selection Efficiency: " << ratio << endl;
    cout << "Number of Event: " << nEvt << endl;
    cout << "Number of Event after ptCuts: " << denominator << endl;
    cout << "Number of event after cuts "<< cut << ": " << eventCounter << endl;
    cout << endl;
    cout << "------- Count Complete -------" << endl;
    inFile->Close();
    delete inFile;
    return make_pair(ratio, eventCounter);
}