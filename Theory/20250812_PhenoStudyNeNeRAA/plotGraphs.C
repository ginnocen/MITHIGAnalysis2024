#include "TAxis.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TStyle.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Macro to read CSV data files, convert to TGraph, fit with saturating exponential,
// plot with set axis ranges, evaluate fits at selected pT, and plot RAA vs species
void plotGraphs() {
  // Disable statistics box
  gStyle->SetOptStat(0);

  // Input files and labels
  std::vector<std::string> filenames = {"inputs/pp_data.csv", "inputs/pPb_data.csv", "inputs/OO_data.csv", "inputs/ArAr_data.csv", "inputs/XeXe_data.csv"};
  std::vector<std::string> labels = {"pp", "pPb", "OO", "ArAr", "XeXe"};

  // Colors and markers for species
  std::vector<int> colors = {kRed, kBlue, kGreen + 2, kMagenta, kOrange};
  std::vector<int> markers = {20, 21, 22, 23, 34};

  // Container for TGraphs and TF1 fits
  std::vector<TGraph *> graphs;
  std::vector<TF1 *> fits;

  // Read, graph, and fit data
  for (size_t i = 0; i < filenames.size(); ++i) {
    std::ifstream file(filenames[i]);
    if (!file.is_open()) {
      std::cerr << "Cannot open " << filenames[i] << std::endl;
      continue;
    }
    std::vector<double> x, y;
    std::string line;
    while (std::getline(file, line)) {
      std::istringstream ss(line);
      double xi, yi;
      char comma;
      if (ss >> xi >> comma >> yi) {
        x.push_back(xi);
        y.push_back(yi);
      }
    }
    file.close();
    if (x.empty())
      continue;

    TGraph *g = new TGraph(x.size(), x.data(), y.data());
    g->SetName(labels[i].c_str());
    g->SetTitle(labels[i].c_str());
    g->SetLineColor(colors[i]);
    g->SetMarkerColor(colors[i]);
    g->SetMarkerStyle(markers[i]);
    graphs.push_back(g);

    // Saturating exponential fit: R_inf - (R_inf - R0)*exp(-x/p0)
    TF1 *f = new TF1(Form("fit_%s", labels[i].c_str()), "[0] - ([0]-[1])*exp(-x/[2])", 10, 210);
    f->SetParNames("R_inf", "R_0", "p0");
    f->SetParameters(1.0, y.front(), 30.0);
    f->SetLineColor(colors[i]);
    f->SetLineWidth(2);
    g->Fit(f, "RQ");
    fits.push_back(f);
  }

  // First canvas: Data + fits
  TCanvas *c1 = new TCanvas("c1", "Data with Exp Fit", 800, 600);
  if (!graphs.empty()) {
    graphs[0]->SetTitle("Data Comparison with Exp Fit;#it{p}_{T} [GeV];R_{AA}");
    graphs[0]->GetXaxis()->SetLimits(10, 210);
    graphs[0]->SetMinimum(0);
    graphs[0]->SetMaximum(1.2);
    graphs[0]->Draw("APL");
    fits[0]->Draw("same");
  }
  for (size_t i = 1; i < graphs.size(); ++i) {
    graphs[i]->Draw("PL same");
    fits[i]->Draw("same");
  }
  TLegend *leg1 = new TLegend(0.65, 0.35, 0.88, 0.58);
  for (size_t i = 0; i < graphs.size(); ++i)
    leg1->AddEntry(graphs[i], labels[i].c_str(), "lp");
  leg1->Draw();
  c1->Update();
  c1->SaveAs("Data_with_Exp_Fit.png");
  // pT values to evaluate
  std::vector<double> pts = {10, 15, 20, 25, 30, 40, 50};
  std::vector<int> ptCols = {kBlack, kRed, kBlue, kGreen + 2, kMagenta, kOrange, kCyan};

  // Second canvas: RAA vs species for each pT
  TCanvas *c2 = new TCanvas("c2", "RAA vs Species", 800, 600);
  int ns = labels.size();
  TH1F *frame = new TH1F("frame", "R_{AA} vs Species;Species;R_{AA}", ns, 0.5, ns + 0.5);
  for (int i = 0; i < ns; ++i)
    frame->GetXaxis()->SetBinLabel(i + 1, labels[i].c_str());
  frame->SetMinimum(0);
  frame->SetMaximum(1.2);
  frame->Draw();

  TLegend *leg2 = new TLegend(0.15, 0.25, 0.35, 0.45);
  for (size_t j = 0; j < pts.size(); ++j) {
    std::vector<double> xv(ns), yv(ns);
    for (int i = 0; i < ns; ++i) {
      xv[i] = i + 1;
      yv[i] = fits[i]->Eval(pts[j]);
    }
    TGraph *gpt = new TGraph(ns, xv.data(), yv.data());
    gpt->SetMarkerStyle(20 + j);
    gpt->SetMarkerColor(ptCols[j]);
    gpt->SetLineColor(ptCols[j]);
    gpt->SetLineWidth(2);
    gpt->Draw("PL same");
    leg2->AddEntry(gpt, Form("%.0f GeV", pts[j]), "lp");
  }
  leg2->Draw();
  c2->Update();
  c2->SaveAs("RAA_vs_Species.png");

  // Save everything
  TFile *out = TFile::Open("outputGraphs.root", "RECREATE");
  for (size_t i = 0; i < graphs.size(); ++i) {
    graphs[i]->SetName(Form("Raa_%s", labels[i].c_str()));
    graphs[i]->Write();
    fits[i]->Write();
  }
  // Optionally write the evaluation graphs or just the fits
  out->Close();
  std::cout << "Saved graphs, fits, and evaluations to outputGraphs.root\n";
}
