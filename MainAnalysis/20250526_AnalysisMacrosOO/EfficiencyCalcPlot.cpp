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
// Everything Below lies on the assumption that the cuts listed are being applied:
// CCFilter == 1 && PVFilter == 1 && nVtx > 0 && abs(VZ) < 15

// Reads data from a text file (generated from EfficiencyPurityDataWrite_1bkg) and fills an EfficiencyResults struct.
// Arguments:
//   inFilename: path to the input text file.
// Returns:
//   EfficiencyResults struct containing vectors of efficiency and purity values and metadata.
EfficiencyResults ReturnEfficiency(const char* inFilename);
void PlotLeadingPtCut(const char* inFileName, const char* inFileshort);


EfficiencyResults ReturnEfficiency(const char* inFilename){

    ifstream infile(inFilename);
    string line, dummy;
    float xsec_EM, xsec_had, xMax;
    int N,coincidence = 0; // Default to 0, can be changed if needed

    EfficiencyResults results;

    vector<double> x, EfficiencyHijingAND, EfficiencyHijingOR;
    vector<double> EfficiencySDAND, EfficiencySDOR;
    vector<double> BKGRejectionAND, BKGRejectionOR;
    vector<double> PurityAND, PurityOR;
    string signalFile, backgroundFile;

    getline(infile, line);
    istringstream iss1(line);
    iss1 >> dummy >> signalFile;
    cout << "Signal File: " << signalFile << endl;

    getline(infile, line);
    istringstream iss2(line);
    iss2 >> dummy >> backgroundFile;

    cout << "Background File: " << backgroundFile << endl;

    getline(infile, line);
    istringstream iss(line);
    iss >> dummy >> xsec_EM >> dummy >> xsec_had >> dummy >> N >> dummy >> xMax >> dummy >> coincidence;

    results.xsec_EM = xsec_EM;
    results.xsec_had = xsec_had;
    results.N = N;
    results.xMax = xMax;
    results.coincidence = coincidence;
    cout << "xsec_EM: " << xsec_EM << ", xsec_had: " << xsec_had << ", N: " << N << ", xMax: " << xMax << "coincidence: " << coincidence << endl;
    getline(infile, line); 

    for (int i = 0; i < N && getline(infile, line); ++i) {
        istringstream iss(line);
        double tx, tEffHijingAND, tEffHijingOR, tEffSDAND, tEffSDOR, tBKGRejAND, tBKGRejOR, tPurityAND, tPurityOR;
        iss >> tx
            >> tEffHijingAND
            >> tEffHijingOR
            >> tEffSDAND
            >> tEffSDOR
            >> tBKGRejAND
            >> tBKGRejOR
            >> tPurityAND
            >> tPurityOR;
    
        results.x.push_back(tx);
        results.EfficiencyHijingAND.push_back(tEffHijingAND);
        results.EfficiencyHijingOR.push_back(tEffHijingOR);
        results.EfficiencySDAND.push_back(tEffSDAND);
        results.EfficiencySDOR.push_back(tEffSDOR);
        results.BKGRejectionAND.push_back(tBKGRejAND);
        results.BKGRejectionOR.push_back(tBKGRejOR);
        results.PurityAND.push_back(tPurityAND);
        results.PurityOR.push_back(tPurityOR);
    }
    infile.close();
    return results;
}
void PlotLeadingPtCut(const char* inFileName, const char* inFileshort) {

    int HFEmaxcut = 4;
    int xmax = 30;

    TH1F* h = new TH1F("h", Form("Efficiency of Leading Track p_{T} cuts (%s);Track p_{T} Cut;Efficiency", inFileshort), 30, 1, 31);
    for (int i = 0; i <= xmax; ++i) {
        double eff = countingtrkptcuts(inFileName, HFEmaxcut, true, i).first;
        h->SetBinContent(i, eff);
    }
    TCanvas* c = new TCanvas("cEff", "Efficiency vs Leading p_{T} Cut", 800, 600);
    h->Draw("hist");
    h->GetXaxis()->SetRangeUser(0, xmax);
    h->GetYaxis()->SetRangeUser(0.5, 1.1);
    gStyle->SetOptStat(0);

    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.035);
    latex.DrawLatex(0.6, 0.85, Form("HFE_max+ and - > %i", HFEmaxcut));
    latex.DrawLatex(0.23, 0.43, "Cuts on Num only:");
    latex.DrawLatex(0.23, 0.40, "PVFilter == 1");
    latex.DrawLatex(0.23, 0.36, "ClusterCompatibilityFilter == 1");
    latex.DrawLatex(0.23, 0.32, "abs(V_{z}) < 15");

    c->SaveAs(Form("Efficiency_vs_LeadingTrkPtCut_%s_HFEMax%i.png", inFileshort, HFEmaxcut));
}


void drawROCsame(vector<double> x1, vector<double> y1, vector<double> x2, vector<double> y2, 
    EfficiencyResults results,
    HistVar1D hvar) {

    string outputDir = "/home/xirong/MITHIGAnalysis2024/MainAnalysis/20250526_AnalysisMacrosOO/Plots/FilterEfficiencyNewer/";

    PlotUtils::setgstyle();

    cout << "------- Drawing ROC Curves -------" << endl;
    TCanvas* c = new TCanvas("cBoth", "", 800, 800);

    TGraph *gr1 = new TGraph(results.N, x1.data(), y1.data());
    TGraph *gr2 = new TGraph(results.N, x2.data(), y2.data());

    c->cd();
    gr1->SetTitle(hvar.histTitle.c_str());
    gr1->GetXaxis()->SetTitle(hvar.xLabel);
    gr1->GetYaxis()->SetTitle(hvar.yLabel);
    gr1->Draw("AP");

    gr2->Draw("P SAME");

    gr1->GetYaxis()->SetRangeUser(hvar.ymin, hvar.ymax);
    gr1->GetXaxis()->SetRangeUser(hvar.xmin, hvar.xmax);

    c->SetGrid();

    PlotUtils::setgraphstyle(gr1, 0.0, 0.0);
    PlotUtils::setgraphstyle(gr2, 0.0, 0.0);
    gr1->SetMarkerColor(kBlue);
    gr2->SetMarkerColor(kRed);
    PlotUtils::GraphLegend({
        {gr1, "HFE_{max}^{+} and HFE_{max}^{-} > X"},
        {gr2, "HFE_{max}^{+} or HFE_{max}^{-} > X"}
    });

    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.035);
    latex.DrawLatex(0.7, 0.85, Form("HIJING (%.1f b)", results.xsec_had));
    latex.DrawLatex(0.7, 0.82, Form("Starlight %s (%.1f b)", hvar.outFileName.c_str(), results.xsec_EM));
    latex.DrawLatex(0.23, 0.40, "PVFilter == 1");
    latex.DrawLatex(0.23, 0.36, "ClusterCompatibilityFilter == 1");
    latex.DrawLatex(0.23, 0.32, "abs(V_{z}) < 15");
    c->SaveAs((outputDir + Form("PuritycurveBoth_Coinc%i_%i",results.coincidence,results.N) + "_" + hvar.outFileName + ".png").c_str());
}

void drawROC(vector<double> x1, vector<double> y1, 
    EfficiencyResults results,
    HistVar1D hvar) {

    string outputDir = "/home/xirong/MITHIGAnalysis2024/MainAnalysis/20250526_AnalysisMacrosOO/Plots/FilterEfficiencyNewer/";

    PlotUtils::setgstyle();

    cout << "------- Drawing ROC Curves -------" << endl;
    TCanvas* c = new TCanvas("cBoth", "", 800, 800);

    TGraph *gr1 = new TGraph(results.N, x1.data(), y1.data());

    c->cd();
    gr1->SetTitle(hvar.histTitle.c_str());
    gr1->GetXaxis()->SetTitle(hvar.xLabel);
    gr1->GetYaxis()->SetTitle(hvar.yLabel);
    gr1->Draw("AP");
    gr1->GetYaxis()->SetRangeUser(hvar.ymin, hvar.ymax);
    gr1->GetXaxis()->SetRangeUser(hvar.xmin, hvar.xmax);
    c->SetGrid();

    PlotUtils::setgraphstyle(gr1, 0.0, 0.0);

    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.035);
    latex.DrawLatex(0.7, 0.85, Form("HIJING (%.1f b)", results.xsec_had));
    latex.DrawLatex(0.7, 0.82, Form("Starlight %s (%.1f b)", hvar.outFileName.c_str(), results.xsec_EM));
    latex.DrawLatex(0.5, 0.5, Form("HFE_{max}^{+} and HFE_{max}^{-} > X"));

    latex.DrawLatex(0.23, 0.40, "PVFilter == 1");
    latex.DrawLatex(0.23, 0.36, "ClusterCompatibilityFilter == 1");
    latex.DrawLatex(0.23, 0.32, "abs(V_{z}) < 15");
    c->SaveAs((outputDir + Form("PuritycurveONE_Coinc%i_%i",results.coincidence,results.N) + "_" + hvar.outFileName + ".png").c_str());
}

void run(){

    const char* N50coin1SD = "/home/xirong/MITHIGAnalysis2024/MainAnalysis/20250526_AnalysisMacrosOO/DataFiles/EfficiencyPurityData_N50_coinc1_HJSD.txt";
    const char* N30coin1DD = "/home/xirong/MITHIGAnalysis2024/MainAnalysis/20250526_AnalysisMacrosOO/DataFiles/EfficiencyPurityData_N30_coinc1__DD.txt";
    const char* N30coin2DD = "/home/xirong/MITHIGAnalysis2024/MainAnalysis/20250526_AnalysisMacrosOO/DataFiles/EfficiencyPurityData_N30_coinc2__DD.txt";
    const char* N30coin2SD = "/home/xirong/MITHIGAnalysis2024/MainAnalysis/20250526_AnalysisMacrosOO/DataFiles/EfficiencyPurityData_N30_coinc2__SD.txt";

    EfficiencyResults results_N50coin1_SD = ReturnEfficiency(N50coin1SD);
    EfficiencyResults results_N30coin1_DD = ReturnEfficiency(N30coin1DD);
    EfficiencyResults results_N30coin2_DD = ReturnEfficiency(N30coin2DD);
    EfficiencyResults results_N30coin2_SD = ReturnEfficiency(N30coin2SD);

    HistVar1D hvar1 {"Purity vs Efficiency Curve", "Efficiency", "Purity", 0, 0.85, 1, 0, 1.2, "HJSD"};

    hvar1.histTitle = "Purity vs Efficiency Curve (Coinc1,SD)";
    hvar1.outFileName = "SD";
    drawROCsame(results_N50coin1_SD.EfficiencyHijingAND, results_N50coin1_SD.PurityAND, results_N50coin1_SD.EfficiencyHijingOR, results_N50coin1_SD.PurityOR,
        results_N50coin1_SD, hvar1);
    drawROC(results_N50coin1_SD.EfficiencyHijingAND, results_N50coin1_SD.PurityAND, results_N50coin1_SD, hvar1);

    hvar1.histTitle = "Purity vs Efficiency Curve (Coinc1,DD)";
    hvar1.outFileName = "DD";
    drawROCsame(results_N30coin1_DD.EfficiencyHijingAND, results_N30coin1_DD.PurityAND, results_N30coin1_DD.EfficiencyHijingOR, results_N30coin1_DD.PurityOR,
        results_N30coin1_DD, hvar1);
    drawROC(results_N30coin1_DD.EfficiencyHijingAND, results_N30coin1_DD.PurityAND,
        results_N30coin1_DD, hvar1);

    hvar1.histTitle = "Purity vs Efficiency Curve (Coinc2,DD)";
    hvar1.outFileName = "DD";
    drawROCsame(results_N30coin2_DD.EfficiencyHijingAND, results_N30coin2_DD.PurityAND, results_N30coin2_DD.EfficiencyHijingOR, results_N30coin2_DD.PurityOR,
        results_N30coin2_DD, hvar1);
    drawROC(results_N30coin2_DD.EfficiencyHijingAND, results_N30coin2_DD.PurityAND, results_N30coin2_DD, hvar1);

    hvar1.histTitle = "Purity vs Efficiency Curve (Coinc2,SD)";
    hvar1.outFileName = "SD";
   //drawROCsame(results_N30coin2_SD.EfficiencyHijingAND, results_N30coin2_SD.PurityAND, results_N30coin2_SD.EfficiencyHijingOR, results_N30coin2_SD.PurityOR,
      //  results_N30coin2_SD, hvar1);
    drawROC(results_N30coin2_SD.EfficiencyHijingAND, results_N30coin2_SD.PurityAND, results_N30coin2_SD, hvar1);


}
