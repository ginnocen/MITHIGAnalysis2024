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
#include "Include/GraphingUtils.h"
#include "Include/HistVariables.h"
#include "Include/EfficiencyResults.h"
#include "Include/EfficiencyCounting.h"
#include "Include/DataFilePaths.h"

using namespace std;

EfficiencyResults ReturnEfficiency(const char* inFilename);
EfficiencyResults ReturnEfficiency1bkg(const char* inFilename);
void PlotLeadingPtCut(const char* inFileName, const char* inFileshort);
void drawROCsame(vector<double> x1, vector<double> y1, vector<double> x2, vector<double> y2, 
    EfficiencyResults results,
    HistVar1D hvar);
void drawROC(const char* fileNameShort,vector<double> x1, vector<double> y1, 
    EfficiencyResults results,
    HistVar1D hvar);

// I plan on optimizing this function for any given number of input but so far it is what it is
void drawROC6(
    const EfficiencyResults& res1, const std::string& label1,
    const EfficiencyResults& res2, const std::string& label2,
    const EfficiencyResults& res3, const std::string& label3,
    const EfficiencyResults& res4, const std::string& label4,
    const EfficiencyResults& res5, const std::string& label5,
    const EfficiencyResults& res6, const std::string& label6,
    const HistVar1D& hvar);
void drawROC7AND(
    const EfficiencyResults& res1, const std::string& label1,
    const EfficiencyResults& res2, const std::string& label2,
    const EfficiencyResults& res3, const std::string& label3,
    const EfficiencyResults& res4, const std::string& label4,
    const EfficiencyResults& res5, const std::string& label5,
    const EfficiencyResults& res6, const std::string& label6,
    const EfficiencyResults& res7, const std::string& label7,
    const HistVar1D& hvar);
void drawROC7OR(
    const EfficiencyResults& res1, const std::string& label1,
    const EfficiencyResults& res2, const std::string& label2,
    const EfficiencyResults& res3, const std::string& label3,
    const EfficiencyResults& res4, const std::string& label4,
    const EfficiencyResults& res5, const std::string& label5,
    const EfficiencyResults& res6, const std::string& label6,
    const EfficiencyResults& res7, const std::string& label7,
    const HistVar1D& hvar);

// Run example that draws two ROC curves on same plot using drawsame
// Commonly used files are included in Include/DataFilePaths.h
// 1bkg means using function named 1bkg, doesn't mean it has 1bkg (but in this case it does too). All bkg means it has all bkg.
void runexample(){
    EfficiencyResults results_SD_1bkg = ReturnEfficiency1bkg(SD_1bkg);
    EfficiencyResults results_DD_1bkg = ReturnEfficiency1bkg(DD_1bkg);
    EfficiencyResults results_allbkg = ReturnEfficiency(allbkg);

    HistVar1D hvar1 {"Purity vs Efficiency Curve", "Efficiency", "Purity", 0, 0.94, 1, 0, 1.25, "HJSD"};

    hvar1.xmin = 0.6;
    hvar1.xmax = 1.0;
    hvar1.ymin = 0.8;
    hvar1.ymax = 1.05; 

    hvar1.histTitle = "ROC curve Hijing using SD+DD backgrounds";
    hvar1.outFileName = "";
    drawROCsame(results_allbkg.EfficiencyHijingAND, results_allbkg.PurityAND, 
        results_allbkg.EfficiencyHijingOR, results_allbkg.PurityOR,
        results_allbkg, hvar1);

}

void run(){
    EfficiencyResults results_allbkg = ReturnEfficiency(allbkg);
    EfficiencyResults results_trkpt0 = ReturnEfficiency(trkpt0);
    EfficiencyResults results_trkpt2 = ReturnEfficiency(trkpt2);
    EfficiencyResults results_trkpt4 = ReturnEfficiency(trkpt4);
    EfficiencyResults results_trkpt6 = ReturnEfficiency(trkpt6);
    EfficiencyResults results_trkpt8 = ReturnEfficiency(trkpt8);
    EfficiencyResults results_trkpt10 = ReturnEfficiency(trkpt10);

    HistVar1D hvar1 {"Purity vs Efficiency Curve", "Efficiency", "Purity", 0, 0.94, 1, 0, 1.25, "HJSD"};

    hvar1.xmin = 0.6;
    hvar1.xmax = 1.0;
    hvar1.ymin = 0.8;
    hvar1.ymax = 1.05; 

    hvar1.histTitle = "Purity vs Efficiency Curve for different track pt cuts";
    hvar1.outFileName = "trkptcuts7";
    drawROC7OR(
        results_allbkg, "no trkpt cut",
        results_trkpt0, "trkpt > 0GeV",
        results_trkpt2, "trkpt > 2GeV",
        results_trkpt4, "trkpt > 4GeV",
        results_trkpt6, "trkpt > 6GeV",
        results_trkpt8, "trkpt > 8GeV",
        results_trkpt10, "trkpt > 10GeV",
        hvar1);
    hvar1.histTitle = "Purity vs Efficiency Curve for different track pt cuts";
    hvar1.outFileName = "trkptcuts7Zoom";
    hvar1.xmin = 0.95;  
    drawROC7OR(
        results_allbkg, "no trkpt cut",
        results_trkpt0, "trkpt > 0GeV",
        results_trkpt2, "trkpt > 2GeV",
        results_trkpt4, "trkpt > 4GeV",
        results_trkpt6, "trkpt > 6GeV",
        results_trkpt8, "trkpt > 8GeV",
        results_trkpt10, "trkpt > 10GeV",
        hvar1);
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
    cout << "xsec_SD: " << xsec_SD << ", xsec_DD: " << xsec_DD << ", xsec_had: " << xsec_had << ", N: " << N << ", xMax: " << xMax << ", trkptcut: " << coincidence << endl;
    getline(infile, line);

    for (int i = 0; i < N && getline(infile, line); ++i) {
        istringstream iss(line);
        double tx, tEffHijingAND, tEffHijingOR, tEffSDAND, tEffSDOR, tEffDDAND, tEffDDOR, tBKGRejAND, tBKGRejOR, tPurityAND, tPurityOR;
        iss >> tx
            >> tEffHijingAND
            >> tEffHijingOR
            >> tEffSDAND
            >> tEffSDOR
            >> tEffDDAND
            >> tEffDDOR
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

EfficiencyResults ReturnEfficiency1bkg(const char* inFilename){

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


void drawROCsame(vector<double> x1, vector<double> y1, vector<double> x2, vector<double> y2, 
    EfficiencyResults results,
    HistVar1D hvar) {

    string outputDir = "/home/xirong/MITHIGAnalysis2024/MainAnalysis/20250526_AnalysisMacrosOO/Plots/FilterEfficiencyNewest/";

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
    latex.DrawLatex(0.5, 0.85, Form("HIJING (%.1f b)", results.xsec_had));
    latex.DrawLatex(0.5, 0.825, Form("Single Diffractive (%.1f b)", results.xsec_SD));
    latex.DrawLatex(0.5, 0.80, Form("Double Diffractive (%.5f b)", results.xsec_DD));
    latex.DrawLatex(0.23, 0.40, "PVFilter == 1");
    latex.DrawLatex(0.23, 0.36, "ClusterCompatibilityFilter == 1");
    latex.DrawLatex(0.23, 0.32, "abs(V_{z}) < 15");
    c->SaveAs((outputDir + Form("PuritycurveBoth%i",results.N) + "_" + hvar.outFileName + ".png").c_str());
}

void drawROC(const char* fileNameShort, vector<double> x1, vector<double> y1, 
    EfficiencyResults results,
    HistVar1D hvar) {

    string outputDir = "/home/xirong/MITHIGAnalysis2024/MainAnalysis/20250526_AnalysisMacrosOO/Plots/FilterEfficiencyNewest/";

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
    latex.DrawLatex(0.7, 0.82, Form("Starlight SD (%.1f b)", results.xsec_SD));
    latex.DrawLatex(0.7, 0.79, Form("Starlight DD (%.5f b)", results.xsec_DD));
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
    string outputDir = "/home/xirong/MITHIGAnalysis2024/MainAnalysis/20250526_AnalysisMacrosOO/Plots/FilterEfficiencyNewest/";

    PlotUtils::setgstyle();

    TCanvas* c = new TCanvas("cROC6", "", 800, 800);

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

    // Use a visible rainbow palette (ROYGBV, but tuned for ROOT colors)
    gr1->SetMarkerColor(kRed);       gr1->SetLineColor(kRed);       gr1->SetMarkerStyle(20);    // Red
    gr2->SetMarkerColor(kOrange+7);  gr2->SetLineColor(kOrange+7);  gr2->SetMarkerStyle(20);    // Orange
    gr3->SetMarkerColor(kYellow+1);  gr3->SetLineColor(kYellow+1);  gr3->SetMarkerStyle(20);    // Yellow (not too bright)
    gr4->SetMarkerColor(kGreen+2);   gr4->SetLineColor(kGreen+2);   gr4->SetMarkerStyle(20);    // Green
    gr5->SetMarkerColor(kAzure+2);   gr5->SetLineColor(kAzure+2);   gr5->SetMarkerStyle(20);    // Blue
    gr6->SetMarkerColor(kViolet+1);  gr6->SetLineColor(kViolet+1);  gr6->SetMarkerStyle(20);    // Violet

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
        {gr6, label6}},
        0.6,0.15,0.9,0.35
    );

    // Add text
    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.035);
    latex.DrawLatex(0.6, 0.85, Form("HIJING (%.1f b)", res1.xsec_had));
    latex.DrawLatex(0.6, 0.82, Form("Starlight SD (%.1f b)", res1.xsec_SD));
    latex.DrawLatex(0.6, 0.79, Form("Starlight DD (%.5f b)", res1.xsec_DD));
    latex.DrawLatex(0.23, 0.40, "PVFilter == 1");
    latex.DrawLatex(0.23, 0.36, "ClusterCompatibilityFilter == 1");
    latex.DrawLatex(0.23, 0.32, "abs(V_{z}) < 15");

    // Save
    c->SaveAs((outputDir + "Puritycurve_AllTrackptcuts" + hvar.outFileName + ".png").c_str());
}

// Draw ROC with 7 curves
void drawROC7AND(
    const EfficiencyResults& res1, const std::string& label1,
    const EfficiencyResults& res2, const std::string& label2,
    const EfficiencyResults& res3, const std::string& label3,
    const EfficiencyResults& res4, const std::string& label4,
    const EfficiencyResults& res5, const std::string& label5,
    const EfficiencyResults& res6, const std::string& label6,
    const EfficiencyResults& res7, const std::string& label7,
    const HistVar1D& hvar)
{
    string outputDir = "/home/xirong/MITHIGAnalysis2024/MainAnalysis/20250526_AnalysisMacrosOO/Plots/FilterEfficiencyNewest/";

    PlotUtils::setgstyle();

    TCanvas* c = new TCanvas("cROC7", "", 800, 800);

    // Create TGraphs for each result
    TGraph* gr1 = new TGraph(res1.N, res1.EfficiencyHijingAND.data(), res1.PurityAND.data());
    TGraph* gr2 = new TGraph(res2.N, res2.EfficiencyHijingAND.data(), res2.PurityAND.data());
    TGraph* gr3 = new TGraph(res3.N, res3.EfficiencyHijingAND.data(), res3.PurityAND.data());
    TGraph* gr4 = new TGraph(res4.N, res4.EfficiencyHijingAND.data(), res4.PurityAND.data());
    TGraph* gr5 = new TGraph(res5.N, res5.EfficiencyHijingAND.data(), res5.PurityAND.data());
    TGraph* gr6 = new TGraph(res6.N, res6.EfficiencyHijingAND.data(), res6.PurityAND.data());
    TGraph* gr7 = new TGraph(res7.N, res7.EfficiencyHijingAND.data(), res7.PurityAND.data());

    // Set styles
    PlotUtils::setgraphstyle(gr1, 0.0, 0.0);
    PlotUtils::setgraphstyle(gr2, 0.0, 0.0);
    PlotUtils::setgraphstyle(gr3, 0.0, 0.0);
    PlotUtils::setgraphstyle(gr4, 0.0, 0.0);
    PlotUtils::setgraphstyle(gr5, 0.0, 0.0);
    PlotUtils::setgraphstyle(gr6, 0.0, 0.0);
    PlotUtils::setgraphstyle(gr7, 0.0, 0.0);

    // Use a visible rainbow palette (ROYGBIV, tuned for ROOT colors)
    gr1->SetMarkerColor(kRed);         gr1->SetLineColor(kRed);         gr1->SetMarkerStyle(20);    // Red
    gr2->SetMarkerColor(kOrange+7);    gr2->SetLineColor(kOrange+7);    gr2->SetMarkerStyle(20);    // Orange
    gr3->SetMarkerColor(kYellow+2);    gr3->SetLineColor(kYellow+2);    gr3->SetMarkerStyle(20);    // Yellow
    gr4->SetMarkerColor(kGreen+2);     gr4->SetLineColor(kGreen+2);     gr4->SetMarkerStyle(20);    // Green
    gr5->SetMarkerColor(kCyan+2);      gr5->SetLineColor(kCyan+2);      gr5->SetMarkerStyle(20);    // Cyan
    gr6->SetMarkerColor(kBlue);        gr6->SetLineColor(kBlue);        gr6->SetMarkerStyle(20);    // Blue
    gr7->SetMarkerColor(kViolet+1);    gr7->SetLineColor(kViolet+1);    gr7->SetMarkerStyle(20);    // Violet

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
    gr7->Draw("P SAME");

    c->SetGrid();

    // Legend
    PlotUtils::GraphLegend({
        {gr1, label1},
        {gr2, label2},
        {gr3, label3},
        {gr4, label4},
        {gr5, label5},
        {gr6, label6},
        {gr7, label7}},
        0.6,0.15,0.9,0.35
    );

    // Add text
    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.035);
    latex.DrawLatex(0.6, 0.85, Form("HIJING (%.1f b)", res1.xsec_had));
    latex.DrawLatex(0.6, 0.82, Form("Starlight SD (%.1f b)", res1.xsec_SD));
    latex.DrawLatex(0.6, 0.79, Form("Starlight DD (%.5f b)", res1.xsec_DD));
    latex.DrawLatex(0.23, 0.45, "PVFilter == 1");
    latex.DrawLatex(0.23, 0.42, "ClusterCompatibilityFilter == 1");
    latex.DrawLatex(0.23, 0.39, "abs(V_{z}) < 15");

    // Save
    c->SaveAs((outputDir + "Puritycurve_AllTrackptcuts" + hvar.outFileName + "_7curves.png").c_str());
}

void drawROC7OR(
    const EfficiencyResults& res1, const std::string& label1,
    const EfficiencyResults& res2, const std::string& label2,
    const EfficiencyResults& res3, const std::string& label3,
    const EfficiencyResults& res4, const std::string& label4,
    const EfficiencyResults& res5, const std::string& label5,
    const EfficiencyResults& res6, const std::string& label6,
    const EfficiencyResults& res7, const std::string& label7,
    const HistVar1D& hvar)
{
    string outputDir = "/home/xirong/MITHIGAnalysis2024/MainAnalysis/20250526_AnalysisMacrosOO/Plots/FilterEfficiencyNewest/";

    PlotUtils::setgstyle();

    TCanvas* c = new TCanvas("cROC7", "", 800, 800);

    // Create TGraphs for each result
    TGraph* gr1 = new TGraph(res1.N, res1.EfficiencyHijingOR.data(), res1.PurityOR.data());
    TGraph* gr2 = new TGraph(res2.N, res2.EfficiencyHijingOR.data(), res2.PurityOR.data());
    TGraph* gr3 = new TGraph(res3.N, res3.EfficiencyHijingOR.data(), res3.PurityOR.data());
    TGraph* gr4 = new TGraph(res4.N, res4.EfficiencyHijingOR.data(), res4.PurityOR.data());
    TGraph* gr5 = new TGraph(res5.N, res5.EfficiencyHijingOR.data(), res5.PurityOR.data());
    TGraph* gr6 = new TGraph(res6.N, res6.EfficiencyHijingOR.data(), res6.PurityOR.data());
    TGraph* gr7 = new TGraph(res7.N, res7.EfficiencyHijingOR.data(), res7.PurityOR.data());
    // Set styles
    PlotUtils::setgraphstyle(gr1, 0.0, 0.0);
    PlotUtils::setgraphstyle(gr2, 0.0, 0.0);
    PlotUtils::setgraphstyle(gr3, 0.0, 0.0);
    PlotUtils::setgraphstyle(gr4, 0.0, 0.0);
    PlotUtils::setgraphstyle(gr5, 0.0, 0.0);
    PlotUtils::setgraphstyle(gr6, 0.0, 0.0);
    PlotUtils::setgraphstyle(gr7, 0.0, 0.0);

    // Use a visible rainbow palette (ROYGBIV, tuned for ROOT colors)
    gr1->SetMarkerColor(kRed);         gr1->SetLineColor(kRed);         gr1->SetMarkerStyle(20);    // Red
    gr2->SetMarkerColor(kOrange+7);    gr2->SetLineColor(kOrange+7);    gr2->SetMarkerStyle(20);    // Orange
    gr3->SetMarkerColor(kYellow+2);    gr3->SetLineColor(kYellow+2);    gr3->SetMarkerStyle(20);    // Yellow
    gr4->SetMarkerColor(kGreen+2);     gr4->SetLineColor(kGreen+2);     gr4->SetMarkerStyle(20);    // Green
    gr5->SetMarkerColor(kCyan+2);      gr5->SetLineColor(kCyan+2);      gr5->SetMarkerStyle(20);    // Cyan
    gr6->SetMarkerColor(kBlue);        gr6->SetLineColor(kBlue);        gr6->SetMarkerStyle(20);    // Blue
    gr7->SetMarkerColor(kViolet+1);    gr7->SetLineColor(kViolet+1);    gr7->SetMarkerStyle(20);    // Violet

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
    gr7->Draw("P SAME");

    c->SetGrid();

    // Legend
    PlotUtils::GraphLegend({
        {gr1, label1},
        {gr2, label2},
        {gr3, label3},
        {gr4, label4},
        {gr5, label5},
        {gr6, label6},
        {gr7, label7}},
        0.6,0.15,0.9,0.35
    );

    // Add text
    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.035);
    latex.DrawLatex(0.6, 0.85, Form("HIJING (%.1f b)", res1.xsec_had));
    latex.DrawLatex(0.6, 0.82, Form("Starlight SD (%.1f b)", res1.xsec_SD));
    latex.DrawLatex(0.6, 0.79, Form("Starlight DD (%.5f b)", res1.xsec_DD));
    latex.DrawLatex(0.23, 0.45, "PVFilter == 1");
    latex.DrawLatex(0.23, 0.42, "ClusterCompatibilityFilter == 1");
    latex.DrawLatex(0.23, 0.39, "abs(V_{z}) < 15");

    // Save
    c->SaveAs((outputDir + "Puritycurve_AllTrackptcutsOR" + hvar.outFileName + "_7curves.png").c_str());
}