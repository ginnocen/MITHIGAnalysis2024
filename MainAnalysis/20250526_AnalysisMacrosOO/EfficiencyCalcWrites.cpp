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
#include <string>
#include "GraphingUtils.h"
#include "HistVariables.h"

#include "EfficiencyResults.h"
#include "EfficiencyCounting.h"

using namespace std;

//Contains Run
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
    EfficiencyPurityDataWrite_withtrkpt(HIJINGFile, StarlightSD, StarlightDD, 
            xsec_SD,xsec_DD, xsec_had, 
            N, xMax, i);
 /*   EfficiencyPurityDataWrite_1bkg(
    HIJINGFile, 
    StarlightSD, 
    xsec_SD, 
    1.08, 
    N, 
    xMax,
    1,
    "SD_new_1p08");

    EfficiencyPurityDataWrite_withtrkpt(
    HIJINGFile, StarlightSD, StarlightDD,
    0, xsec_DD, xsec_had, 
    N, xMax, 0,
    "CrossCheck_DDOnly");

    EfficiencyPurityDataWrite_withtrkpt(
    HIJINGFile, StarlightSD, StarlightDD,
    xsec_SD, 0, xsec_had, 
    N, xMax, 0,
    "CrossCheck_SDOnly");

    EfficiencyPurityDataWrite_withtrkpt(
    HIJINGFile, StarlightSD, StarlightDD,
    xsec_SD, 0, xsec_had, 
    N, xMax, 0,
    "CrossCheck_BOTHSDDD");
*/
   /* for (int i = 5; i <= 9; i += 2) {
        EfficiencyPurityDataWrite_withtrkpt(HIJINGFile, StarlightSD, StarlightDD, 
            xsec_SD,xsec_DD, xsec_had, 
            N, xMax, i);
    }*/

   /* EfficiencyPurityDataWrite(HIJINGFile, StarlightDD, xsec_DD, xsec_had, N, xMax, 1,"_DD");
    EfficiencyPurityDataWrite(HIJINGFile, StarlightDD, xsec_DD, xsec_had, N, xMax, 2,"_DD");
    EfficiencyPurityDataWrite(HIJINGFile, StarlightSD, xsec_SD, xsec_had, N, xMax, 2,"_SD");
*/
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

    string outfolder = "DataFiles/";
    string filename = outfolder + Form("EfficiencyPurityData_N%d_coinc%d_", N-1, coincidence) + outFileAdd + ".txt";
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

    outfile << "HFEMaxCut Eff_Hijing(AND) Eff_Hijing(OR) Eff_SD(AND) Eff_SD(OR) Eff_DD(AND) Eff_DD(OR) BKGRej(AND) BKGRej(OR) Purity(AND) Purity(OR)\n";

    for (int i = 0; i < N; ++i) {
        x[i] = xMax * i / (N - 1); // Uniform spacing from 0 to xMax
        EfficiencyHijingAND[i] = countingTrkptcuts(inFileNameSignal, x[i], true, trkptcut).first;
        EfficiencyHijingOR[i] = countingTrkptcuts(inFileNameSignal, x[i], false, trkptcut).first;
        EfficiencySDAND[i] = countingTrkptcuts(inFileNameBKGSD, x[i], true, trkptcut).first;
        EfficiencySDOR[i] = countingTrkptcuts(inFileNameBKGSD, x[i], false, trkptcut).first;
        EfficiencyDDAND[i] = countingJingcuts(inFileNameBKGDD, x[i], true, trkptcut).first;
        EfficiencyDDOR[i] = countingJingcuts(inFileNameBKGDD, x[i], false, trkptcut).first;
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

