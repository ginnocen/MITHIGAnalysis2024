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
#include "Include/GraphingUtils.h"
#include "Include/HistVariables.h"
#include "Include/DataFilePaths.h"
#include "Include/EfficiencyResults.h"
#include "Include/EfficiencyCounting.h"

using namespace std;

//Function Declarations

//ignore this function, this is the test function for using 1 bkg only.
//the "counting" functions used in counting number of events defined in EfficiencyCounting.h
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
    string outfolder = "DataFilesExample/",
    string outFileAdd = "");

//Code written in run. Use .L EfficiencyCalcWrites.cpp and then run() to run this code.
void run(){
    float xsec_SD = 0.3;
    float xsec_had = 1.30;
    float xsec_DD = 0.0003;
    int N = 4;
    float xMax = 3.0;
    float trkptcut = -1;
    string outFileName = "DataFilesExample/";

    //Example that writes Efficiency and Purity Data to File with all backgrounds
    //When Trkptcut = -1, no cut is applied
    EfficiencyPurityDataWrite_withtrkpt(HIJINGFile, StarlightSD, StarlightDD, 
            xsec_SD, xsec_DD, xsec_had, 
            N, xMax, -1,outFileName, "SampleWrite");
    //Example that writes Efficiency and Purity Data to File with only SD background
    EfficiencyPurityDataWrite_withtrkpt(HIJINGFile, StarlightSD, StarlightDD, 
            0, xsec_DD, xsec_had, 
            N, xMax, -1,outFileName, "SampleWriteSDbkg");

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

    string outfolder = "DataFilesNew/";
    string filename = outfolder + Form("EfficiencyPurityData_N%d_coinc%d_", N-1, coincidence) + outFileAdd + ".txt";
    cout << "------- Writing Efficiency and Purity Data to File -------" << endl;
    ofstream outfile(filename.c_str());
    if (!outfile.is_open()) {
        cerr << "Error: Could not open file " << filename << " for writing." << endl;
        return;
    }
    cout << "Output file: " << filename << endl;
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
    string outfolder = "DataFilesExample/",
    string outFileAdd = "") {
        
    string filename = outfolder + Form("EfficiencyPurityData_N%d_trkpt%d_", N-1, trkptcut) + outFileAdd + ".txt";
    cout << "------- Writing Efficiency and Purity Data to File -------" << endl;
    ofstream outfile(filename.c_str());
    if (!outfile.is_open()) {
        cerr << "Error: Could not open file " << filename << " for writing." << endl;
        return;
    }
    cout << "Output file: " << filename << endl;
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
        EfficiencyDDAND[i] = countingTrkptcuts(inFileNameBKGDD, x[i], true, trkptcut).first;
        EfficiencyDDOR[i] = countingTrkptcuts(inFileNameBKGDD, x[i], false, trkptcut).first;
        BKGRejectionAND[i] = 1.0 - EfficiencySDAND[i];
        BKGRejectionOR[i] = 1.0 - EfficiencySDOR[i];
        PurityAND[i] = 1 - (xsec_SD * EfficiencySDAND[i] + xsec_DD * EfficiencyDDAND[i]) / (xsec_SD * EfficiencySDAND[i] + xsec_DD * EfficiencyDDAND[i] + xsec_had * EfficiencyHijingAND[i]);
        PurityOR[i] = 1 - (xsec_SD * EfficiencySDOR[i] + xsec_DD * EfficiencyDDOR[i]) / (xsec_SD * EfficiencySDOR[i] + xsec_DD * EfficiencyDDOR[i] + xsec_had * EfficiencyHijingOR[i]);
        cout << "PurityAND[" << i << "] = " << PurityAND[i] << endl;
        cout << "PurityOR[" << i << "] = " << PurityOR[i] << endl;
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