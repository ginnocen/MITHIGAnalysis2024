#include <iostream>
#include <TFile.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLegend.h>
#include <vector>
#include <string>

using namespace std;

void run() {
    const char* kylesFileName = "/data00/kdeverea/skimValidation.root";
    const char* inFileName = "/data00/OOsamples/SkimDebug22May2025/skim_HiForest_Hijing_merged_DEBUG.root";
   
    HistVar1D histvar = {"Hist Title","xLabel","yLabel",
        50,0,90,
        0,0};
    
    CanvasVar canvasvar = {"CanvasName","Untitled.png",false};

    vector<string> varList = {"Run", 
        "Lumi",
        "hiBin",
        "nVtx",
        "ClusterCompatibilityFilter", 
        "PVFilter",
        "mMaxL1HFAdcPlus",
        "mMaxL1HFAdcMinus"
        }; 
    vector<string> varListFloat = {"VX", 
        "VY",
        "VZ",
        "VXError",
        "VYError", 
        "VZError",
        "HFEMaxPlus",
        "HFEMaxMinus",
        "hiHF_pf",
        "Npart",
        "Ncoll"}; 
    TFile* outFile = new TFile("EventLevelHistograms.root", "RECREATE");

    // Create a ROOT file to save histograms
/*
    string var = "VZ";
    TFile* outFile = new TFile("EventLevelHistograms.root", "RECREATE");
    histvar.histTitle = var + " Distribution fit to Gaussian";
    histvar.xLabel = "Primary vertex v_{Z} (cm)";
    histvar.yLabel = "Event Count" ;
    canvasvar.outFileName = "Plots/" + string(var) + "_Hist_withCutsandFitted.png";
    histvar.xmin = FindMinFloat(inFileName, var.c_str())-1;
    histvar.xmax = FindMaxFloat(inFileName, var.c_str())+1;
    histvar.nbin = (histvar.xmax-histvar.xmin)/0.1;
    
    TH1F* h1 = GenHistFloat(inFileName, var.c_str(), histvar,true);

    TCanvas* canvas = new TCanvas("canvas", canvasvar.canvasName, 800, 800);
    h1->Draw("HIST");
    h1->Fit("gaus");

    TF1* fitFunc = h1->GetFunction("gaus");
    fitFunc->SetLineColor(kRed);
    fitFunc->SetLineWidth(2);
    fitFunc->Draw("SAME");

    double amplitude = fitFunc->GetParameter(0); // height
    double mean      = fitFunc->GetParameter(1); // center
    double sigma     = fitFunc->GetParameter(2); // width


    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.02);
    latex.DrawLatex(0.63, 0.77, "PVFilter == 1");
    latex.DrawLatex(0.63, 0.74, "ClusterCompatibilityFilter == 1");
    latex.DrawLatex(0.63, 0.71, "nVtx > 0");

    latex.DrawLatex(0.65, 0.63, "Gaussian Fit Parameters:");
    latex.DrawLatex(0.65, 0.60, Form("Mean: %.2f", mean));
    latex.DrawLatex(0.65, 0.57, Form("#sigma: %.2f", sigma));

    
    if(canvasvar.isLog == true){
        canvas->SetLogy();
    }
    canvas->SetLeftMargin(0.16); // Increase right margin
    canvas->SaveAs(canvasvar.outFileName.c_str());

    cout << "------ Gaussian Fit Results ------" << endl;
    cout << "Amplitude: " << amplitude << endl;
    cout << "Mean: " << mean << endl;
    cout << "Sigma: " << sigma << endl;
    cout << "----------------------------------" << endl;
    */
    for (const auto& var : varList) {
        histvar.histTitle = var + " Distribution Testing";
        histvar.xLabel = var.c_str();
        histvar.yLabel = "Event Count";
        canvasvar.outFileName = "Plots/" + string(var) + "_Hist_withCuts.png";
        int maxVal = FindMaxInt(inFileName, var.c_str());
        int minVal = FindMinInt(inFileName, var.c_str());
        histvar.xmin = minVal - 0.5;
        histvar.xmax = maxVal + 0.5;
        histvar.nbin = maxVal - minVal + 1;
    
        TH1F* h1 = GenHistInt(inFileName, var.c_str(), histvar,true);
        outFile->cd(); // Make sure we are writing to outFile
        h1->Write(); // Save histogram to file
        DrawHist(h1,canvasvar);
    }

    for (const auto& var : varListFloat) {
        histvar.histTitle = var + " Distribution Testing";
        histvar.xLabel = var.c_str();
        histvar.yLabel = "Event Count";
        canvasvar.outFileName = "Plots/" + string(var) + "_Hist_withCuts.png";
        histvar.xmin = FindMinFloat(inFileName, var.c_str())-1;
        histvar.xmax = FindMaxFloat(inFileName, var.c_str())+1;
        histvar.nbin = (histvar.xmax-histvar.xmin)/0.1;
    
        TH1F* h1 = GenHistFloat(inFileName, var.c_str(), histvar,true);
      //  outFile->cd(); // Make sure we are writing to outFile
      //  h1->Write(); // Save histogram to file

        DrawHist(h1,canvasvar);
    }

    outFile->Close(); // Close the ROOT file
}