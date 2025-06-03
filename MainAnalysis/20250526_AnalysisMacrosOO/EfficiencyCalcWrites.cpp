#include <iostream>
#include <TFile.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLegend.h>
#include <utility>
#include <tuple>
#include "GraphingUtils.h"
#include "HistVariables.h"
#include "EfficiencyResults.h"
using namespace std;

//CCFilter == 1 && PVFilter == 1 && nVtx > 0 && abs(VZ) < 15 
//Can implement leadingPtSelection
pair<double, int> countingLeadingptcuts(const char* inFileName, float cut = 0, bool cutBool = false, int leadingPtSelection = 0);
pair<double, int> countingTrkptcuts(const char* inFileName, float cut = 0, bool cutBool = false, int trkptcut = 0);
pair<double, int> countingJingcuts(const char* inFileName, float cut = 0, bool cutBool = false, int coincidence = 0);
//Write Efficiency, Purity, and BKG Rejection to a file
void EfficiencyPurityDataWrite_1bkg(const char* inFileNameSignal, 
    const char* inFileNameBKG, 
    float xsec_EM, 
    float xsec_had, 
    int N, 
    float xMax,
    int coincidence,
    string outFileAdd = "");


void EfficiencyPurityDataWrite_withtrkpt(
    const char* inFileNameSignal, 
    const char* inFileNameBKGSD, 
    const char* inFileNameBKGDD,
    float xsec_SD, 
    float xsec_DD,
    float xsec_had, 
    int N, 
    float xMax,
    int trkptcut,
    string outFileAdd = "");

void run(){
    const char* HIJINGFile = "/data00/OOsamples/Skims/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root";
    const char* StarlightSD = "/data00/OOsamples/Skims/skim_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root";
    const char* StarlightDD = "/data00/OOsamples/Skims/skim_HiForest_250514_Starlight_SingleDiffraction_OO_5362GeV_1505PR47944.root";

    float xsec_SD = 0.3;
    float xsec_had = 1.30;
    float xsec_DD = 0.0003;
    int N = 31;
    float xMax = 30.0;
    int coincidence = 1;

    for (int i = 20; i <= 30; i += 5) {
        EfficiencyPurityDataWrite_withtrkpt(HIJINGFile, StarlightSD, StarlightDD, 
            xsec_SD,xsec_DD, xsec_had, 
            N, xMax, i);
    }

   /* EfficiencyPurityDataWrite(HIJINGFile, StarlightDD, xsec_DD, xsec_had, N, xMax, 1,"_DD");
    EfficiencyPurityDataWrite(HIJINGFile, StarlightDD, xsec_DD, xsec_had, N, xMax, 2,"_DD");
    EfficiencyPurityDataWrite(HIJINGFile, StarlightSD, xsec_SD, xsec_had, N, xMax, 2,"_SD");
*/
}


pair<double, int> countingLeadingptcuts(const char* inFileName, float cut = 0, bool cutBool = false, int leadingPtSelection = 0) {
    cout << Form("------- Counting for HFEMax cut %f and ptcut %d -------",cut,leadingPtSelection) << endl;

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
    float HFEMaxPlus, HFEMaxMinus;
    float VZ, leadingPtEta1p0_sel;
    const Long64_t nEvt = tree->GetEntries();
    int eventCounter = 0, subeventCounter = 0;
    int denominator = 0;

    tree->SetBranchAddress("ClusterCompatibilityFilter", &CCFilter);
    tree->SetBranchAddress("PVFilter", &PVFilter);
    tree->SetBranchAddress("nVtx", &nVtx);
    tree->SetBranchAddress("VZ", &VZ);
    tree->SetBranchAddress("HFEMaxPlus", &HFEMaxPlus);
    tree->SetBranchAddress("HFEMaxMinus", &HFEMaxMinus);
    tree->SetBranchAddress("leadingPtEta1p0_sel",&leadingPtEta1p0_sel);

    for (int i = 0; i < nEvt; i++) {
        tree->GetEntry(i);
        if (leadingPtEta1p0_sel > leadingPtSelection) {
            denominator++;
            if (CCFilter == 1 && PVFilter == 1 && nVtx > 0 && abs(VZ) < 15) {
                if (cutBool == false){
                    if (HFEMaxPlus > cut || HFEMaxMinus > cut){
                        eventCounter++;
                    }
                }
                else if (cutBool == true){
                    if (HFEMaxPlus > cut && HFEMaxMinus > cut){
                        eventCounter++;
                    }
                }
            }
        }
    }

    double ratio = static_cast<double>(eventCounter) / denominator;
    cout << "Ratio between Original and Filtered: " << ratio << endl;
    cout << "Total number of events: " << nEvt << endl;
    cout << "Total number of events after HFEcut "<< cut << ": " <<eventCounter << endl;
    cout << endl;
    cout << "------- Count Complete -------" << endl;
    return make_pair(ratio, eventCounter);
}

pair<double, int> countingTrkptcuts(const char* inFileName, 
    float cut = 0, 
    bool cutBool = false, 
    int trkptcut = 0) {
    cout << Form("------- Counting for HFEMax cut %f and ptcut %d -------",cut,trkptcut) << endl;

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
    float HFEMaxPlus, HFEMaxMinus;
    float VZ, leadingPtEta1p0_sel;
    vector<float>* trkPt = nullptr;
    const Long64_t nEvt = tree->GetEntries();
    int eventCounter = 0, subeventCounter = 0;
    int denominator = 0;
    bool trkPtBool = false;

    tree->SetBranchAddress("ClusterCompatibilityFilter", &CCFilter);
    tree->SetBranchAddress("PVFilter", &PVFilter);
    tree->SetBranchAddress("nVtx", &nVtx);
    tree->SetBranchAddress("VZ", &VZ);
    tree->SetBranchAddress("HFEMaxPlus", &HFEMaxPlus);
    tree->SetBranchAddress("HFEMaxMinus", &HFEMaxMinus);
    tree->SetBranchAddress("leadingPtEta1p0_sel",&leadingPtEta1p0_sel);
    tree->SetBranchAddress("trkPt",&trkPt); 

    for (int i = 0; i < nEvt; i++) {
        tree->GetEntry(i);
        trkPtBool = false;
        for (int j = 0; j < trkPt->size(); j++) {
            if (trkPt->at(j) > trkptcut) {
                trkPtBool = true;
            }
        }
        if (trkPtBool) {
            denominator++;
            if (CCFilter == 1 && PVFilter == 1 && nVtx > 0 && abs(VZ) < 15) {
                if (cutBool == false){
                    if (HFEMaxPlus > cut || HFEMaxMinus > cut){
                        eventCounter++;
                    }
                }
                else if (cutBool == true){
                    if (HFEMaxPlus > cut && HFEMaxMinus > cut){
                        eventCounter++;
                    }
                }
            }
        }
    }

    double ratio = static_cast<double>(eventCounter) / denominator;
    cout << "Ratio between Original and Filtered: " << ratio << endl;
    cout << "Total number of events: " << nEvt << endl;
    cout << "Total number of events after HFEcut "<< cut << ": " <<eventCounter << endl;
    cout << endl;
    cout << "------- Count Complete -------" << endl;
    return make_pair(ratio, eventCounter);
}


// Counting function with cuts same as Jing's setup
// Can implement coincidence
pair<double, int> countingJingcuts(const char* inFileName,
    float cut = 0,
    bool cutBool = false,
    int coincidence = 0) {
    cout << "------- Counting (Cuts Same as Jing) -------" << endl;

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
    float HFEMaxPlus, HFEMaxMinus;
    float HFEMaxPlus2, HFEMaxMinus2, HFEMaxPlus3, HFEMaxMinus3;
    float VZ, leadingPtEta1p0_sel;

    tree->SetBranchAddress("ClusterCompatibilityFilter", &CCFilter);
    tree->SetBranchAddress("PVFilter", &PVFilter);
    tree->SetBranchAddress("nVtx", &nVtx);
    tree->SetBranchAddress("VZ", &VZ);
    tree->SetBranchAddress("HFEMaxPlus", &HFEMaxPlus);
    tree->SetBranchAddress("HFEMaxMinus", &HFEMaxMinus);

    tree->SetBranchAddress("HFEMaxPlus2", &HFEMaxPlus2);
    tree->SetBranchAddress("HFEMaxMinus2", &HFEMaxMinus2);
    tree->SetBranchAddress("HFEMaxPlus3", &HFEMaxPlus3);
    tree->SetBranchAddress("HFEMaxMinus3", &HFEMaxMinus3);
    tree->SetBranchAddress("leadingPtEta1p0_sel",&leadingPtEta1p0_sel);


    const Long64_t nEvt = tree->GetEntries();

    int eventCounter = 0, subeventCounter = 0;
    int eventCounterTotal = 0;
    int CC0Counter = 0, CC1Counter = 0, PV0Counter = 0, PV1Counter = 0, nVtxCounter = 0, VZCounter = 0;

    for (int i = 0; i < nEvt; i++) {
        tree->GetEntry(i);
        if (CCFilter == 1 && PVFilter == 1 && abs(VZ) < 15) {
            eventCounterTotal ++;
            if (cutBool == false) { // OR logic
                if (coincidence == 1) {
                    if (HFEMaxPlus > cut || HFEMaxMinus > cut) {
                        eventCounter++;
                    }
                } else if (coincidence == 2) {
                    if ((HFEMaxPlus > cut || HFEMaxMinus > cut) &&
                        (HFEMaxPlus2 > cut || HFEMaxMinus2 > cut)) {
                        eventCounter++;
                    }
                }
            } else if (cutBool == true) { // AND logic
                if (coincidence == 1) {
                    if (HFEMaxPlus > cut && HFEMaxMinus > cut) {
                        eventCounter++;
                    }
                } else if (coincidence == 2) {
                    if ((HFEMaxPlus > cut && HFEMaxMinus > cut) &&
                        (HFEMaxPlus2 > cut && HFEMaxMinus2 > cut)) {
                        eventCounter++;
                    }
                }
            }
        }
    }
    double ratio = static_cast<double>(eventCounter) / nEvt;
    cout << "Ratio between Original and Filtered: " << ratio << endl;
    cout << "Total number of events: " << nEvt << endl;
    cout << "Total number of events after HFEcut "<< cut << ": " <<eventCounter << endl;
    cout << endl;

    cout << "------- Count Complete -------" << endl;
    return make_pair(ratio, eventCounter);
}

//Writing Efficiency and Purity Data to File
void EfficiencyPurityDataWrite_1bkg(
    const char* inFileNameSignal, 
    const char* inFileNameBKG, 
    float xsec_EM, 
    float xsec_had, 
    int N, 
    float xMax,
    int coincidence,
    string outFileAdd = "") {

    string filename = Form("EfficiencyPurityData_N%d_coinc%d_", N-1, coincidence) + outFileAdd + ".txt";
    cout << "------- Writing Efficiency and Purity Data to File -------" << endl;
    ofstream outfile(filename.c_str());
    if (!outfile.is_open()) {
        cerr << "Error: Could not open file " << filename << " for writing." << endl;
        return;
    }

    double x[N], y[N], y2[N];
    double EfficiencyHijingAND[N], EfficiencyHijingOR[N],EfficiencySDAND[N], EfficiencySDOR[N],BKGRejectionAND[N], BKGRejectionOR[N],PurityAND[N], PurityOR[N];
    float leadingPtCut = 0.0;
    outfile << "SignalFile: " << inFileNameSignal << "\n";
    outfile << "BackgroundFile: " << inFileNameBKG << "\n";

    outfile << "xsec_EM: " << xsec_EM << " "
            << "xsec_had: " << xsec_had << " "
            << "N: " << N << " "
            << "xMax: " << xMax << " "
            << " Coincidence: " << coincidence << "\n";

    outfile << "HFEMaxCut Eff_Hijing(AND) Eff_Hijing(OR) Eff_SD(AND) Eff_SD(OR) BKGRej(AND) BKGRej(OR) Purity(AND) Purity(OR)\n";

    for (int i = 0; i < N; ++i) {
        x[i] = xMax * i / (N - 1); // Uniform spacing from 0 to xMax
        EfficiencyHijingAND[i] = countingJingcuts(inFileNameSignal, x[i], true, coincidence).first;
        EfficiencyHijingOR[i] = countingJingcuts(inFileNameSignal, x[i], false,coincidence).first;
        EfficiencySDAND[i] = countingJingcuts(inFileNameBKG, x[i], true,coincidence).first;
        EfficiencySDOR[i] = countingJingcuts(inFileNameBKG, x[i], false,coincidence).first;
        BKGRejectionAND[i] = 1.0 - EfficiencySDAND[i];
        BKGRejectionOR[i] = 1.0 - EfficiencySDOR[i];
        PurityAND[i] = 1 - (xsec_EM * EfficiencySDAND[i]) / (xsec_EM * EfficiencySDAND[i] + xsec_had * EfficiencyHijingAND[i]);
        PurityOR[i] = 1 - (xsec_EM * EfficiencySDOR[i]) / (xsec_EM * EfficiencySDOR[i] + xsec_had * EfficiencyHijingOR[i]);

        outfile << x[i] << " "
                << EfficiencyHijingAND[i] << " "
                << EfficiencyHijingOR[i] << " "
                << EfficiencySDAND[i] << " "
                << EfficiencySDOR[i] << " "
                << BKGRejectionAND[i] << " "
                << BKGRejectionOR[i] << " "
                << PurityAND[i] << " "
                << PurityOR[i] << "\n";
    }
    outfile.close();
}

void EfficiencyPurityDataWrite_withtrkpt(
    const char* inFileNameSignal, 
    const char* inFileNameBKGSD, 
    const char* inFileNameBKGDD,
    float xsec_SD, 
    float xsec_DD,
    float xsec_had, 
    int N, 
    float xMax,
    int trkptcut,
    string outFileAdd = "") {

    string filename = Form("EfficiencyPurityData_N%d_trkpt%d_", N-1, trkptcut) + outFileAdd + ".txt";
    cout << "------- Writing Efficiency and Purity Data to File -------" << endl;
    ofstream outfile(filename.c_str());
    if (!outfile.is_open()) {
        cerr << "Error: Could not open file " << filename << " for writing." << endl;
        return;
    }

    double x[N], y[N], y2[N];
    double EfficiencyDDAND[N], EfficiencyDDOR[N];
    double EfficiencyHijingAND[N], EfficiencyHijingOR[N],EfficiencySDAND[N], EfficiencySDOR[N],BKGRejectionAND[N], BKGRejectionOR[N],PurityAND[N], PurityOR[N];
    float leadingPtCut = 0.0;
    outfile << "SignalFile: " << inFileNameSignal << "\n";
    outfile << "BackgroundFileSD: " << inFileNameBKGSD << "\n";
    outfile << "BackgroundFileDD: " << inFileNameBKGDD << "\n";

    outfile << "xsec_SD: " << xsec_SD << " "
            << "xsec_DD: " << xsec_DD << " "
            << "xsec_had: " << xsec_had << " "
            << "N: " << N << " "
            << "xMax: " << xMax << " "
            << "trkptcut: " << trkptcut << "\n";

    outfile << "HFEMaxCut Eff_Hijing(AND) Eff_Hijing(OR) Eff_SD(AND) Eff_SD(OR) BKGRej(AND) BKGRej(OR) Purity(AND) Purity(OR)\n";

    for (int i = 0; i < N; ++i) {
        x[i] = xMax * i / (N - 1); // Uniform spacing from 0 to xMax
        EfficiencyHijingAND[i] = countingTrkptcuts(inFileNameSignal, x[i], true, trkptcut).first;
        EfficiencyHijingOR[i] = countingTrkptcuts(inFileNameSignal, x[i], false, trkptcut).first;
        EfficiencySDAND[i] = countingTrkptcuts(inFileNameBKGSD, x[i], true, trkptcut).first;
        EfficiencySDOR[i] = countingTrkptcuts(inFileNameBKGSD, x[i], false, trkptcut).first;
        EfficiencyDDAND[i] = countingJingcuts(inFileNameBKGDD, x[i], true, 1).first;
        EfficiencyDDOR[i] = countingJingcuts(inFileNameBKGDD, x[i], false, 1).first;
        BKGRejectionAND[i] = 1.0 - EfficiencySDAND[i];
        BKGRejectionOR[i] = 1.0 - EfficiencySDOR[i];
        PurityAND[i] = 1 - (xsec_SD * EfficiencySDAND[i] + xsec_DD * EfficiencyDDAND[i]) / (xsec_SD * EfficiencySDAND[i] + xsec_DD * EfficiencyDDAND[i] + xsec_had * EfficiencyHijingAND[i]);
        PurityOR[i] = 1 - (xsec_SD * EfficiencySDOR[i] + xsec_DD * EfficiencyDDOR[i]) / (xsec_SD * EfficiencySDOR[i] + xsec_DD * EfficiencyDDOR[i] + xsec_had * EfficiencyHijingOR[i]);

        outfile << x[i] << " "
                << EfficiencyHijingAND[i] << " "
                << EfficiencyHijingOR[i] << " "
                << EfficiencySDAND[i] << " "
                << EfficiencySDOR[i] << " "
                << EfficiencyDDAND[i] << " "
                << EfficiencyDDOR[i] << " "
                << BKGRejectionAND[i] << " "
                << BKGRejectionOR[i] << " "
                << PurityAND[i] << " "
                << PurityOR[i] << "\n";
    }
    outfile.close();
}


