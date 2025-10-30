#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <fstream>
#include <iostream>
#include <vector>


#include "nlohmann/json.hpp"
using json = nlohmann::json;

TGraphAsymmErrors*  getGraphFromJSON(std::string inputFile, std::string datasetName);


void makeGraphFromJSON(){
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  
  TGraphAsymmErrors* g = (TGraphAsymmErrors*) getGraphFromJSON("neneOutput.json", "NeNe_000_100_Pi0"); 
  TGraphAsymmErrors* g2 = (TGraphAsymmErrors*) getGraphFromJSON("oxygenOutput.json", "OO_000_100_Pi0");
  
  // plot just to check if it's correct
  TCanvas *c = new TCanvas("c","c",800,600);
  g->Draw("A3");   // "3" = draw filled band
  g->GetHistogram()->GetXaxis()->SetTitle("#it{p}_{T}");
  g->GetHistogram()->GetYaxis()->SetTitle("#it{R}_{AA}");
  g->GetHistogram()->GetYaxis()->SetRangeUser(0.0, 1.2);
  g2->SetFillColorAlpha(kBlue, 0.3);
  g2->SetLineColor(kBlue);
  g2->Draw("3 same");

  TLegend* l = new TLegend(0.7, 0.16, 0.88, 0.3);
  l->SetFillColor(0);
  l->SetBorderSize(0);
  l->AddEntry(g, "NeNe", "f");
  l->AddEntry(g2, "OO", "f"); 
  l->Draw(); 
  c->SaveAs("MinBiasTheory.pdf");
  
  // write to a root file
  TFile* outFile = new TFile("MinBias.root", "RECREATE");
  outFile->cd();
  g->SetName("NeNeMinBias");
  g->Write();
  g2->SetName("OOMinBias");
  g2->Write();
  outFile->Close();
  
}

// function to get the graph from json
TGraphAsymmErrors*  getGraphFromJSON(std::string inputFile, std::string datasetName) {
    std::ifstream in(inputFile);
    json j;
    in >> j;

    // assuming we only care about min bias for the moment. 
    auto data = j[datasetName];

    std::vector<double> x_vals, y_central, y_err;
    std::vector<double> x_err_low, x_err_high; 

    for (size_t i = 0; i < data["max"].size(); i++) {
        double x = data["max"][i][0].get<double>();
        double y_max = data["max"][i][1].get<double>();
        double y_min = data["min"][i][1].get<double>();

        double y_mid = 0.5 * (y_max + y_min);
        double err   = 0.5 * (y_max - y_min);

        x_vals.push_back(x);
        y_central.push_back(y_mid);
        y_err.push_back(err);

        x_err_low.push_back(0.0);
        x_err_high.push_back(0.0);
    }

    int n = x_vals.size();

    TGraphAsymmErrors *g = new TGraphAsymmErrors(n, x_vals.data(), y_central.data(), x_err_low.data(), x_err_high.data(), y_err.data(), y_err.data());
    g->SetFillColorAlpha(kRed, 0.35);
    g->SetLineColor(kRed);
    g->SetLineWidth(2);

    return g; 
}
