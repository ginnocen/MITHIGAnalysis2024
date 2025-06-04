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
#include "EfficiencyCounting.h"
#include "DataFilePaths.h"

using namespace std;

EfficiencyResults ReturnEfficiency(const char* inFilename);
EfficiencyResults ReturnEfficiencyOld(const char* inFilename);
void PlotLeadingPtCut(const char* inFileName, const char* inFileshort);
void drawROCsame(const char* fileNameShort,vector<double> x1, vector<double> y1, vector<double> x2, vector<double> y2, 
    EfficiencyResults results,
    HistVar1D hvar);
void drawROC(const char* fileNameShort,vector<double> x1, vector<double> y1, 
    EfficiencyResults results,
    HistVar1D hvar);

void drawROC6(
    const EfficiencyResults& res1, const std::string& label1,
    const EfficiencyResults& res2, const std::string& label2,
    const EfficiencyResults& res3, const std::string& label3,
    const EfficiencyResults& res4, const std::string& label4,
    const EfficiencyResults& res5, const std::string& label5,
    const EfficiencyResults& res6, const std::string& label6,
    const HistVar1D& hvar);

void run(){

  
    EfficiencyResults results_N50coin1_SD = ReturnEfficiencyOld(N50coin1SD);
    EfficiencyResults results_N30coin1_DD = ReturnEfficiencyOld(N30coin1DD);
    EfficiencyResults results_N30coin2_DD = ReturnEfficiencyOld(N30coin2DD);
    EfficiencyResults results_N30coin2_SD = ReturnEfficiencyOld(N30coin2SD);
    EfficiencyResults resultsN30trkpt0 = ReturnEfficiency(N30trkpt0);
    EfficiencyResults resultsN30trkpt5 = ReturnEfficiency(N30trkpt5);
    EfficiencyResults resultsN30trkpt7 = ReturnEfficiency(N30trkpt7);  
    EfficiencyResults resultsN30trkpt9 = ReturnEfficiency(N30trkpt9);
    EfficiencyResults resultsN30trkpt10 = ReturnEfficiency(N30trkpt10);
    EfficiencyResults resultsN30trkpt15 = ReturnEfficiency(N30trkpt15);

    EfficiencyResults results_DDOnly = ReturnEfficiency(DDOnly);
    EfficiencyResults resultsSD1p08 = ReturnEfficiencyOld(SD1p08);

    HistVar1D hvar1 {"Purity vs Efficiency Curve", "Efficiency", "Purity", 0, 0.94, 1, 0, 1.25, "HJSD"};

    hvar1.histTitle = "Purity vs Efficiency Curve for different track pt cuts";
    hvar1.outFileName = "zoomed";
    hvar1.xmin = 0.93;
    hvar1.xmax = 1.0;
    hvar1.ymin = 0.0;
    hvar1.ymax = 1.25; 
   // PlotLeadingPtCut(HIJINGFile,"HIJING");
   // PlotLeadingPtCut(StarlightSD,"SD");
   // PlotLeadingPtCut(StarlightDD,"DD");
   
    /*drawROC6(
    resultsN30trkpt0, "trkpt > 0GeV",
    resultsN30trkpt5, "trkpt > 5GeV",
    resultsN30trkpt7, "trkpt > 7GeV",
    resultsN30trkpt9, "trkpt > 9GeV",
    resultsN30trkpt10, "trkpt > 10GeV",
    resultsN30trkpt15, "trkpt > 15GeV",
    hvar1);*/


   /* hvar1.histTitle = "Purity vs Efficiency Curve (N30 trkpt0)";
    hvar1.outFileName = "N30trkpt0";
    drawROC(resultsN30trkpt0.EfficiencyHijingAND, resultsN30trkpt0.PurityAND, resultsN30trkpt0, hvar1);

    hvar1.histTitle = "Purity vs Efficiency Curve (N30 trkpt5)";
    hvar1.outFileName = "N30trkpt5";
    drawROC(resultsN30trkpt5.EfficiencyHijingAND, resultsN30trkpt5.PurityAND, resultsN30trkpt5, hvar1);

    hvar1.histTitle = "Purity vs Efficiency Curve (N30 trkpt10)";
    hvar1.outFileName = "N30trkpt10";
    drawROC(resultsN30trkpt10.EfficiencyHijingAND, resultsN30trkpt10.PurityAND, resultsN30trkpt10, hvar1);

    hvar1.histTitle = "Purity vs Efficiency Curve (N30 trkpt15)";
    hvar1.outFileName = "N30trkpt15";
    drawROC(resultsN30trkpt15.EfficiencyHijingAND, resultsN30trkpt15.PurityAND, resultsN30trkpt15, hvar1);
    //drawROC(resultsN30trkpt5.EfficiencyHijingAND, resultsN30trkpt5.PurityAND, resultsN30trkpt5, hvar1);
*/
    hvar1.histTitle = "Purity vs Efficiency Curve (Coinc1,SD)";
    hvar1.outFileName = "SD_new_1p08";
    hvar1.ymin = 0.8;
    hvar1.xmin = 0.6;
    hvar1.ymax = 1.05; 
    const char* SDshort = "SD";
    const char* DDshort = "DD";

    //results_N50coin1_SD.xsec_had = 1.08;
    //results_N30coin1_DD.xsec_had = 1.08;

   // drawROCsame(SDshort,resultsSD1p08.EfficiencyHijingAND, resultsSD1p08.PurityAND, resultsSD1p08.EfficiencyHijingOR, resultsSD1p08.PurityOR,
   //     resultsSD1p08, hvar1);
    hvar1.histTitle = "Purity vs Efficiency Curve (DD crosscheck)";
    hvar1.outFileName = "DD_crosscheck";
    drawROCsame(DDshort,results_DDOnly.EfficiencyHijingAND, results_DDOnly.PurityAND, results_N30coin1_DD.EfficiencyHijingAND, results_N30coin1_DD.PurityAND,
        results_DDOnly, hvar1);
  //  drawROC(SDshort,results_N50coin1_SD.EfficiencyHijingAND, results_N50coin1_SD.PurityAND, results_N50coin1_SD, hvar1);


  /*  hvar1.histTitle = "Purity vs Efficiency Curve (Coinc1,DD)";
    hvar1.outFileName = "DD_new_1p08";
    drawROCsame(DDshort,results_N30coin1_DD.EfficiencyHijingAND, results_N30coin1_DD.PurityAND, results_N30coin1_DD.EfficiencyHijingOR, results_N30coin1_DD.PurityOR,
        results_N30coin1_DD, hvar1);
    drawROC(DDshort,results_N30coin1_DD.EfficiencyHijingAND, results_N30coin1_DD.PurityAND,
        results_N30coin1_DD, hvar1);

   hvar1.histTitle = "Purity vs Efficiency Curve (Coinc2,DD)";
    hvar1.outFileName = "DD_new_1p08";
    drawROCsame(DDshort,results_N30coin2_DD.EfficiencyHijingAND, results_N30coin2_DD.PurityAND, results_N30coin2_DD.EfficiencyHijingOR, results_N30coin2_DD.PurityOR,
        results_N30coin2_DD, hvar1);
    drawROC(DDshort,results_N30coin2_DD.EfficiencyHijingAND, results_N30coin2_DD.PurityAND, results_N30coin2_DD, hvar1);

    hvar1.histTitle = "Purity vs Efficiency Curve (Coinc2,SD)";
    hvar1.outFileName = "SD_new_1p08";
    drawROCsame(SDshort,results_N30coin2_SD.EfficiencyHijingAND, results_N30coin2_SD.PurityAND, results_N30coin2_SD.EfficiencyHijingOR, results_N30coin2_SD.PurityOR,
        results_N30coin2_SD, hvar1);
    drawROC(SDshort,results_N30coin2_SD.EfficiencyHijingAND, results_N30coin2_SD.PurityAND, results_N30coin2_SD, hvar1);
*/
     }


EfficiencyResults ReturnEfficiency(const char* inFilename){

    ifstream infile(inFilename);
    string line, dummy;
    float xsec_SD, xsec_DD, xsec_had, xMax;
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

    cout << "Background File SD: " << backgroundFile << endl;

    getline(infile, line);
    istringstream iss3(line);
    iss3 >> dummy >> backgroundFile;

    cout << "Background File DD: " << backgroundFile << endl;

    getline(infile, line);
    istringstream iss(line);
    iss >> dummy >> xsec_SD >> dummy >> xsec_DD >> dummy >> xsec_had >> dummy >> N >> dummy >> xMax >> dummy >> coincidence;
    results.xsec_SD = xsec_SD;
    results.xsec_DD = xsec_DD;
    results.xsec_had = xsec_had;
    results.N = N;
    results.xMax = xMax;
    results.coincidence = coincidence;
    cout << "xsec_SD: " << xsec_SD << ", xsec_DD: " << xsec_DD << ", xsec_had: " << xsec_had << ", N: " << N << ", xMax: " << xMax << ", coincidence: " << coincidence << endl;
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

EfficiencyResults ReturnEfficiencyOld(const char* inFilename){

    ifstream infile(inFilename);
    string line, dummy;
    float xsec_SD, xsec_had, xMax;
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

    cout << "Background File SD: " << backgroundFile << endl;

    getline(infile, line);
    istringstream iss(line);
    iss >> dummy >> xsec_SD >> dummy >> xsec_had >> dummy >> N >> dummy >> xMax >> dummy >> coincidence;
    results.xsec_SD = xsec_SD;
    results.xsec_had = xsec_had;
    results.N = N;
    results.xMax = xMax;
    results.coincidence = coincidence;
    cout << "xsec_EM: " << xsec_SD << ", xsec_had: " << xsec_had << ", N: " << N << ", xMax: " << xMax << ", coincidence: " << coincidence << endl;
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
        double eff = countingTrkptcuts(inFileName, HFEmaxcut, true, i).first; // Assuming countingJingcuts returns a pair with efficiency as the first element
        h->SetBinContent(i, eff);
    }
    
    TCanvas* c = new TCanvas("cEff", "Efficiency vs Leading p_{T} Cut", 800, 600);
    h->Draw("hist");
    h->GetXaxis()->SetRangeUser(0, xmax);
    h->GetYaxis()->SetRangeUser(0.0, 1.1);
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


void drawROCsame(const char* fileNameShort, vector<double> x1, vector<double> y1, vector<double> x2, vector<double> y2, 
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
    latex.DrawLatex(0.7, 0.825, Form("Starlight %s (%.1f b)", fileNameShort, results.xsec_SD));
  //  latex.DrawLatex(0.7, 0.80, Form("Starlight DD (%.1f b)", results.xsec_DD));
    latex.DrawLatex(0.23, 0.40, "PVFilter == 1");
    latex.DrawLatex(0.23, 0.36, "ClusterCompatibilityFilter == 1");
    latex.DrawLatex(0.23, 0.32, "abs(V_{z}) < 15");
    c->SaveAs((outputDir + Form("PuritycurveBoth_Coinc%i_%i",results.coincidence,results.N) + "_" + hvar.outFileName + ".png").c_str());
}

void drawROC(const char* fileNameShort, vector<double> x1, vector<double> y1, 
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
    latex.DrawLatex(0.7, 0.82, Form("Starlight %s (%.1f b)", fileNameShort, results.xsec_SD));
    //latex.DrawLatex(0.7, 0.79, Form("Starlight DD (%.5f b)", results.xsec_DD));
    latex.DrawLatex(0.5, 0.5, Form("HFE_{max}^{+} and HFE_{max}^{-} > X"));

    latex.DrawLatex(0.23, 0.40, "PVFilter == 1");
    latex.DrawLatex(0.23, 0.36, "ClusterCompatibilityFilter == 1");
    latex.DrawLatex(0.23, 0.32, "abs(V_{z}) < 15");
    c->SaveAs((outputDir + Form("PuritycurveONE_Coinc%i_%i",results.coincidence,results.N) + "_" + hvar.outFileName + ".png").c_str());
}
void drawROC6(
    const EfficiencyResults& res1, const std::string& label1,
    const EfficiencyResults& res2, const std::string& label2,
    const EfficiencyResults& res3, const std::string& label3,
    const EfficiencyResults& res4, const std::string& label4,
    const EfficiencyResults& res5, const std::string& label5,
    const EfficiencyResults& res6, const std::string& label6,
    const HistVar1D& hvar)
{
    std::string outputDir = "/home/xirong/MITHIGAnalysis2024/MainAnalysis/20250526_AnalysisMacrosOO/Plots/FilterEfficiencyNewer/";

    PlotUtils::setgstyle();

    TCanvas* c = new TCanvas("cROC4", "", 800, 800);

    // Create TGraphs for each result
    TGraph* gr1 = new TGraph(res1.N, res1.EfficiencyHijingAND.data(), res1.PurityAND.data());
    TGraph* gr2 = new TGraph(res2.N, res2.EfficiencyHijingAND.data(), res2.PurityAND.data());
    TGraph* gr3 = new TGraph(res3.N, res3.EfficiencyHijingAND.data(), res3.PurityAND.data());
    TGraph* gr4 = new TGraph(res4.N, res4.EfficiencyHijingAND.data(), res4.PurityAND.data());
    TGraph* gr5 = new TGraph(res5.N, res5.EfficiencyHijingAND.data(), res5.PurityAND.data());
    TGraph* gr6 = new TGraph(res6.N, res6.EfficiencyHijingAND.data(), res6.PurityAND.data());

    // Set styles
    PlotUtils::setgraphstyle(gr1, 0.0, 0.0);
    PlotUtils::setgraphstyle(gr2, 0.0, 0.0);
    PlotUtils::setgraphstyle(gr3, 0.0, 0.0);
    PlotUtils::setgraphstyle(gr4, 0.0, 0.0);
    PlotUtils::setgraphstyle(gr5, 0.0, 0.0);
    PlotUtils::setgraphstyle(gr6, 0.0, 0.0);

    gr1->SetMarkerColor(kBlue);   gr1->SetLineColor(kBlue);  gr1->SetMarkerStyle(20);
    gr2->SetMarkerColor(kRed);    gr2->SetLineColor(kRed);    gr2->SetMarkerStyle(20);
    gr3->SetMarkerColor(kGreen+2);gr3->SetLineColor(kGreen+2);gr3->SetMarkerStyle(20);
    gr4->SetMarkerColor(kOrange);gr4->SetLineColor(kOrange);gr4->SetMarkerStyle(20);
    gr5->SetMarkerColor(kMagenta);gr5->SetLineColor(kMagenta);gr5->SetMarkerStyle(20);
    gr6->SetMarkerColor(kCyan);gr6->SetLineColor(kCyan);gr6->SetMarkerStyle(20);
    gr1->SetMarkerSize(1.1);
    gr2->SetMarkerSize(1.1);
    gr3->SetMarkerSize(1.0);
    gr4->SetMarkerSize(0.9);
    gr5->SetMarkerSize(0.9);
    gr6->SetMarkerSize(0.9);

    gr1->SetTitle(hvar.histTitle.c_str());
    gr1->GetXaxis()->SetTitle(hvar.xLabel);
    gr1->GetYaxis()->SetTitle(hvar.yLabel);
    gr1->GetYaxis()->SetRangeUser(hvar.ymin, hvar.ymax);
    gr1->GetXaxis()->SetRangeUser(hvar.xmin, hvar.xmax);

    c->cd();
    gr1->Draw("AP");
    gr2->Draw("P SAME");
    gr3->Draw("P SAME");
    gr4->Draw("P SAME");
    gr5->Draw("P SAME");
    gr6->Draw("P SAME");

    c->SetGrid();

    // Legend
    PlotUtils::GraphLegend({
        {gr1, label1},
        {gr2, label2},
        {gr3, label3},
        {gr4, label4},
        {gr5, label5},
        {gr6, label6}
    });

    // Add text
    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.035);
    latex.DrawLatex(0.7, 0.85, Form("HIJING (%.1f b)", res1.xsec_had));
    latex.DrawLatex(0.7, 0.82, Form("Starlight SD (%.1f b)", res1.xsec_SD));
    latex.DrawLatex(0.7, 0.79, Form("Starlight DD (%.1f b)", res1.xsec_DD));
    latex.DrawLatex(0.23, 0.40, "PVFilter == 1");
    latex.DrawLatex(0.23, 0.36, "ClusterCompatibilityFilter == 1");
    latex.DrawLatex(0.23, 0.32, "abs(V_{z}) < 15");

    // Save
    c->SaveAs((outputDir + "Puritycurve_AllTrackptcuts.png").c_str());
}