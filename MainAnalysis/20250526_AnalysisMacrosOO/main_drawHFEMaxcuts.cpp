#include <iostream>
#include <TFile.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLegend.h>

using namespace std;

void run(const char* inFileName) {

    string foldername = "Plots/TrackInfo_RatiosTestNew/8GeV/";

    int HFEcut = 8; // Now HFEcut is an int

    HistVar1D hvar = {"CanvasName","Hist Title",
        "Unknown.png","xLabel","yLabel",50,0,90,
        0,0,false};

    string varName = "trkPt";  
    string varLabel = varName.c_str();

    hvar.isLog = false;
    hvar.xLabel = varName.c_str();
    hvar.yLabel = "Event Count";
    hvar.xmin = 0;
    hvar.xmax = 15;
    hvar.nbin = (hvar.xmax-hvar.xmin)/0.1;  
    //End Change

    ofstream outfile(foldername + "_Information.txt");
    if (outfile.is_open()) {
        outfile << inFileName << endl;
        outfile.close();
    }

    //Creating Baseline Hist
    hvar.histTitle = varLabel + " Distribution (No Cuts on HFEMax)";
    TCanvas* canvas = new TCanvas(hvar.canvasName, hvar.canvasName, 800, 800);
    TH1F* h_Baseline = DrawHistFVector(inFileName, varName.c_str(), hvar, true, true, 0, 0);
    h_Baseline->Draw("HIST");

    //Hist 1
    hvar.histTitle = varLabel + " Distribution with HFEPlus > " + to_string(HFEcut) + " or HFEMinus > 0";
    TH1F* h_Plus = DrawHistFVector(inFileName, varName.c_str(), hvar, true, true, HFEcut, 0);
    h_Plus->Draw("HIST");

    //Hist 2
    hvar.histTitle = varLabel + " Distribution with HFEPlus > 0 or HFEMinus > " + to_string(HFEcut);
    TH1F* h_Minus = DrawHistFVector(inFileName, varName.c_str(), hvar, true, true, 0, HFEcut);
    h_Minus->Draw("HIST");

    //Hist 3
    hvar.histTitle = varLabel + " Distribution with HFE_{Max}- or HFE_{Max}+ > " + to_string(HFEcut);
    TH1F* h_both = DrawHistFVector(inFileName, varName.c_str(), hvar, true, true, HFEcut, HFEcut);
    h_both->Draw("HIST");

    //Ratio 3
    TH1F* ratio_both = (TH1F*)h_both->Clone("ratio");
    ratio_both->SetTitle(("Ratio of HFE_{Max}+ or E_{Max}- > " + to_string(HFEcut) + "GeV to Baseline").c_str());
    ratio_both->Divide(h_Baseline);  
    ratio_both->SetLineColor(kGreen+3);

    //Ratio 1
    TCanvas* canvas_ratio = new TCanvas("Ratio", "Ratio", 800, 800);

    TH1F* ratio_Plus = (TH1F*)h_Plus->Clone("ratio");
    ratio_Plus->SetTitle(("Ratio of HFE_{max}+ > " + to_string(HFEcut) + "GeV to Baseline").c_str());
    ratio_Plus->Divide(h_Baseline);  
    ratio_Plus->SetLineColor(kBlue);
    
    //Ratio 2
    TH1F* ratio_Minus = (TH1F*)h_Minus->Clone("ratio");
    ratio_Minus->SetTitle(("Ratio of HFE_{Max}- > " + to_string(HFEcut) + "GeV to Baseline").c_str());
    ratio_Minus->Divide(h_Baseline);  
    ratio_Minus->SetLineColor(kRed);

    // --- Save ratio_Plus ---
    TCanvas* c_ratio_plus = new TCanvas("c_ratio_plus", "Ratio Plus", 800, 800);
    gStyle->SetOptStat(0);
    ratio_Plus->Draw("HIST");
    c_ratio_plus->SaveAs((foldername + varLabel + "_RatioPlus" + to_string(HFEcut) + ".png").c_str());

    // --- Save ratio_Minus ---
    TCanvas* c_ratio_minus = new TCanvas("c_ratio_minus", "Ratio Minus", 800, 800);
    gStyle->SetOptStat(0);
    ratio_Minus->Draw("HIST");
    c_ratio_minus->SaveAs((foldername + varLabel + "_RatioMinus" + to_string(HFEcut) + ".png").c_str());

    // --- Save ratio_both ---
    TCanvas* c_ratio_both = new TCanvas("c_ratio_both", "Ratio Both", 800, 800);
    gStyle->SetOptStat(0);
    ratio_both->Draw("HIST");
    c_ratio_both->SaveAs((foldername + varLabel + "_RatioBoth" + to_string(HFEcut) + ".png").c_str());
    /*
    // --- Save h_Baseline ---
    TCanvas* c_baseline = new TCanvas("c_baseline", "Baseline", 800, 800);
    gStyle->SetOptStat(0);
    h_Baseline->Draw("HIST");
    c_baseline->SaveAs((foldername + varLabel + "_Baseline.png").c_str());

    // --- Save h_Plus ---
    TCanvas* c_plus = new TCanvas("c_plus", "HFEPlus > " + to_string(HFEcut) + " or HFEMinus > 0", 800, 800);
    gStyle->SetOptStat(0);
    h_Plus->Draw("HIST");
    c_plus->SaveAs((foldername + varLabel + "_Plus.png").c_str());

    // --- Save h_Minus ---
    TCanvas* c_minus = new TCanvas("c_minus", "HFEPlus > 0 or HFEMinus > " + to_string(HFEcut), 800, 800);
    gStyle->SetOptStat(0);
    h_Minus->Draw("HIST");
    c_minus->SaveAs((foldername + varLabel + "_Minus.png").c_str());

    // --- Save h_both ---
    TCanvas* c_both = new TCanvas("c_both", "HFEMaxMinus or Plus > " + to_string(HFEcut), 800, 800);
    gStyle->SetOptStat(0);
    h_both->Draw("HIST");
    c_both->SaveAs((foldername + varLabel + "_Both.png").c_str());
    */

    // --- Save all ratios together (already in your code) ---
    canvas_ratio->cd();
    ratio_Plus->SetTitle(("All HFE cuts Ratios vs " + varLabel).c_str());
    ratio_both->Draw("HIST");
    ratio_Plus->Draw("HIS SAME");
    ratio_Minus->Draw("HIST SAME");


    // Make legend and font larger

    TLegend* legend = new TLegend(0.15, 0.15, 0.52, 0.25); // left bottom corner
    legend->SetTextSize(0.016); // Increase font size
    legend->SetEntrySeparation(0.02); // Optional: reduce space between entries
    legend->AddEntry(ratio_Plus, ("HFE_{Max}+ > " + to_string(HFEcut) + "GeV or HFE_{Max}- > 0GeV").c_str(), "l");
    legend->AddEntry(ratio_Minus, ("HFE_{Max}+ > 0GeV or HFE_{Max}- > " + to_string(HFEcut) + "GeV").c_str(), "l");
    legend->AddEntry(ratio_both, ("HFE_{Max}+ > " + to_string(HFEcut) + "GeV or HFE_{Max}- > " + to_string(HFEcut) + "GeV").c_str(), "l");
    legend->Draw();
    gStyle->SetOptStat(0);
    canvas_ratio->SaveAs((foldername + varLabel + "_RatioTesting" + to_string(HFEcut) + ".png").c_str());

    /*
    hvar.isLog = true;
    hvar.histTitle = varLabel + " Distribution with HFEMaxPlus > " + to_string(HFEcut) + "GeV";
    hvar.outFileName = foldername + varLabel + "_HFEMaxPlus_log.png";
    DrawHistFVector(inFileName, varName.c_str(), hvar, true, true, HFEcut, 0);

    hvar.histTitle = varLabel + " Distribution with HFEMaxMinus > " + to_string(HFEcut) + "GeV";
    hvar.outFileName = foldername + varLabel + "_HFEMaxMinus_log.png";
    DrawHistFVector(inFileName, varName.c_str(), hvar, true, true, 0, HFEcut);

    hvar.histTitle = varLabel + " Distribution with HFEMaxMinus and HFEMaxPlus > " + to_string(HFEcut) + "GeV";
    hvar.outFileName = foldername + varLabel + "_BothCuts_log.png";
    DrawHistFVector(inFileName, varName.c_str(), hvar, true, true, HFEcut, HFEcut);
    */
}
