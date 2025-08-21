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
#include <cmath>
#include "/home/dcynolte/macros/MITHIG_CMSStyle.h"

// Macro to read CSV data files, convert to TGraph, fit with saturating exponential,
// plot with set axis ranges, evaluate fits at selected pT, and plot RAA vs species

std::vector<double> logspace(double start, double end, int num) {
    std::vector<double> result;
    result.reserve(num);
    double log_start = std::log10(start);
    double log_end   = std::log10(end);
    double step = (log_end - log_start) / (num - 1);
    for (int i = 0; i < num; ++i) {
        result.push_back(std::pow(10, log_start + step * i));
    }
    return result;
}
void plotGraphs() {
  // Disable statistics box
  gStyle->SetOptStat(0);
  //gStyle->SetOptTitle(0);

  // Input files and labels
  std::vector<std::string> filenames = {"pp_data.csv", "pPb_data.csv", "OO_data.csv", "ArAr_data.csv", "XeXe_data.csv", "PbPb_data.csv"};
  std::vector<std::string> labels = {"pp", "pPb", "OO", "ArAr", "XeXe", "PbPb"};

  // Colors and markers for species
  std::vector<int> colors = {cmsRed, cmsBlue, cmsViolet, cmsYellow, cmsTeal, cmsPurple};
  std::vector<int> markers = {20, 21, 22, 23, 34, 29, 33};

  // Container for TGraphs and TF1 fits
  std::vector<TGraph *> graphs;
  std::vector<TF1 *> fits;

  // Read, graph, and fit data
  for (size_t i = 0; i < filenames.size(); ++i) {
    std::ifstream file("inputs/" + filenames[i]);
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
    TF1 *f = new TF1(Form("fit_%s", labels[i].c_str()), "[0]-([0]-[1])*exp(-pow(x/[2],[3]))", 10, 200);
    f->SetParNames("R_inf", "R_0", "p0", "p1");
    f->SetParameters(1, y.front(), 30.0, 1);
    f->SetLineColor(colors[i]);
    f->SetLineWidth(2);
    g->Fit(f, "RQ", "", 10, 200);
    std::cout << "Fit parameters: " << f->GetParameter(0) << ", " << f->GetParameter(1) << ", " << f->GetParameter(2) << ", " << f->GetParameter(3) << std::endl;
    fits.push_back(f);
  }

  // First canvas: Data + fits
  TCanvas *c1 = new TCanvas("c1", "Data with Exp Fit", 800, 600);
  c1->SetLogx();
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
  TLegend *leg1 = new TLegend(0.65, 0.15, 0.88, 0.4);
  for (size_t i = 0; i < graphs.size(); ++i)
    leg1->AddEntry(graphs[i], labels[i].c_str(), "lp");
  leg1->SetBorderSize(0);
  leg1->Draw();
  c1->Update();
  c1->SaveAs("Data_with_Exp_Fit.pdf");
  // pT values to evaluate
  std::vector<double> pts = logspace(10, 200, 10);
  std::vector<int> ptCols = {cmsBlack, cmsPaleBlue, cmsViolet, cmsTeal, cmsPurple, cmsYellow, cmsBlue, cmsRed, cmsGray, cmsVioletD1};

  // Second canvas: RAA vs species for each pT
  TCanvas *c2 = new TCanvas("c2", "RAA vs Species", 800, 600);
  int ns = labels.size();
  TH1F *frame = new TH1F("frame", "R_{AA} vs Species;Species;R_{AA}", ns, 0.5, ns + 0.5);
  for (int i = 0; i < ns; ++i)
    frame->GetXaxis()->SetBinLabel(i + 1, labels[i].c_str());
  frame->SetMinimum(0);
  frame->SetMaximum(1.2);
  frame->Draw();

  TLegend *leg2 = new TLegend(0.15, 0.15, 0.35, 0.45);
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
  c2->SaveAs("RAA_vs_Species.pdf");
  leg2->Clear();

  TCanvas *c3 = new TCanvas("c3", "RAA as function of A", 800, 600);
  std::vector<double> A = {1, 16, 40, 129, 208};
  std::vector<double> ratioRAAONe(pts.size());
  std::vector<double> RAANe(pts.size());
  int np = A.size();
  for (int j = 0; j < pts.size(); ++j)
    {
      std::vector<double> yA(np);
      for (int i = 0; i < np; ++i){
        if (i == 0)
          yA[i] = fits[i]->Eval(pts[j]);
        else
          yA[i] = fits[i+1]->Eval(pts[j]);
      }
      TGraph *gA = new TGraph(A.size(), A.data(), yA.data());
        gA->SetMarkerStyle(20 + j);
        gA->SetMarkerColor(ptCols[j]);
        gA->SetLineColor(ptCols[j]);
        gA->SetLineWidth(2);
        gA->GetXaxis()->SetRangeUser(-10, 208 + 10);
        gA->SetTitle("R_{AA} as function of A for fixed p_{T}");
        gA->GetXaxis()->SetTitle("A");
        gA->GetYaxis()->SetTitle("R_{AA}");

        if (j == 0)
        {
          gA->Draw("AP");
          gA->GetYaxis()->SetRangeUser(0, 1.01);
        }
        else
          gA->Draw("P same");
        leg2->AddEntry(gA, Form("%.0f GeV", pts[j]), "p");
        leg2->SetTextSize(0.03);
        leg2->SetBorderSize(0);
        TF1 *func = new TF1(Form("func_A_%d", j), "[0] + [1]*x + [2]*x*x + [3]*x*x*x", 1, 208);
        gA->Fit(Form("func_A_%d", j), "0", "", 1, 208);
        func->SetLineColor(ptCols[j]);
        func->SetLineStyle(2);
        func->Draw("same");
        RAANe[j] = func->Eval(20);
        TMarker *pNe = new TMarker(20, RAANe[j], 20);
        pNe->SetMarkerColor(ptCols[j]);
        pNe->SetMarkerStyle(20 + j);
        pNe->Draw();
        ratioRAAONe[j] = RAANe[j]/yA[0];
      }
  TLine *legline = new TLine(15, 0.4625, 25, 0.4625);
  legline->SetLineColor(cmsGray);
  legline->SetLineStyle(2);
  legline->SetLineWidth(2);
  legline->Draw();
  TLatex *legtex = new TLatex(27.5, 0.45, "cubic fit");
  legtex->SetTextSize(0.03);
  legtex->SetTextFont(42);
  legtex->Draw("same");
  leg2->Draw();
  TLatex *tex = new TLatex();
  tex->SetTextSize(0.05);
  tex->SetTextFont(42);
  tex->DrawLatex(-1, 0.925, "p");
  tex->DrawLatex(10, 0.725, "O");
  tex->DrawLatex(15, 0.675, "Ne");
  tex->DrawLatex(35, 0.55, "Ar");
  tex->DrawLatex(125, 0.25, "Xe");
  tex->DrawLatex(205, 0.15, "Pb");
  c3->Update();
  c3->SaveAs("RAA_as_function_of_A.pdf");

  TCanvas *c4 = new TCanvas("c4", "OO vs Ne RAA", 800, 600);
  c4->SetLeftMargin(0.15);
  c4->SetLogx();
  TGraph *gratio = new TGraph(pts.size(), pts.data(), ratioRAAONe.data());
  gratio->SetTitle("OO vs Ne RAA;#it{p}_{T} [GeV];R_{NeNe}/R_{OO}");
  gratio->SetMarkerStyle(kPlus);
  //gratio->SetMarkerStyle(20);
  gratio->Draw("AP");
  c4->SaveAs("OO_vs_Ne_RAA.pdf");

  std::vector<string> labelswNe = {"pp", "pPb", "OO", "NeNe", "ArAr", "XeXe", "PbPb"};

  std::vector<TGraph*> graphswNe(graphs.size()+1);
  std::vector<TF1 *> fitswNe(graphs.size()+1);
  for (int i = 0; i< graphs.size()+1; i++) {
    if (i<3)
    {
      graphswNe[i] = graphs[i];
      fitswNe[i] = fits[i];
    }
    else if (i == 3)
    {
      graphswNe[i] = new TGraph(pts.size(), pts.data(), RAANe.data());
      TF1 *f = new TF1(Form("fit_%s", labelswNe[i].c_str()), "[0]-([0]-[1])*exp(-pow(x/[2],[3]))", 10, 210);
      f->SetParNames("R_inf", "R_0", "p0", "p1");
      f->SetParameters(1.0, RAANe.front(), 30.0, 1);
      f->SetLineColor(ptCols[i]);
      f->SetLineWidth(2);
      graphswNe[i]->Fit(f, "RQ", "", 0, 180);
      fitswNe[i] = f;
    }
    else
    {
      graphswNe[i] = graphs[i-1];
      fitswNe[i] = fits[i-1];
    }
  }

  TCanvas *c5 = new TCanvas("c5", "Data with Exp Fit with Ne", 800, 600);
  c5->SetLogx();
  if (!graphswNe.empty()) {
    graphswNe[0]->SetTitle("Data Comparison with Exp Fit;#it{p}_{T} [GeV];R_{AA}");
    graphswNe[0]->GetXaxis()->SetLimits(0, 200);
    graphswNe[0]->SetMinimum(0.2);
    graphswNe[0]->SetMaximum(1.1);
    graphswNe[0]->SetMarkerColor(ptCols[0]);
    graphswNe[0]->Draw("AP");
    graphswNe[0]->SetMarkerStyle(markers[0]);
    graphswNe[0]->SetLineColor(ptCols[0]);
    graphswNe[0]->SetMarkerColor(ptCols[0]);
    fits[0]->SetLineColor(ptCols[0]);
    fits[0]->Draw("same");
  }
  for (size_t i = 1; i < graphswNe.size(); i++) {
    //if (i == 3)
    //{
      //graphswNe[i]->SetMarkerStyle(20 + i);
      graphswNe[i]->SetMarkerColor(ptCols[i]);
      graphswNe[i]->SetLineColor(ptCols[i]);
      graphswNe[i]->SetMarkerStyle(markers[i]);
      graphswNe[i]->Draw("P same");
      fitswNe[i]->SetLineColor(ptCols[i]);
      fitswNe[i]->Draw("same");
    //}
  }
  TLegend *leg3 = new TLegend(0.65, 0.15, 0.88, 0.4);
  for (size_t i = 0; i < graphswNe.size(); ++i)
    {
      //if (i == 2 || i == 3)
      //{
        leg3->AddEntry(graphswNe[i], labelswNe[i].c_str(), "lp");
      //}
    }
  leg3->SetBorderSize(0);
  leg3->SetTextSize(0.04);
  leg3->Draw();
  c5->Update();
  c5->SaveAs("Data_with_Exp_Fit_with_Ne_OO.pdf");

  gStyle->SetOptTitle(0);
  TCanvas *c6 = new TCanvas("c6", "OO vs NeNe Raa", 800, 600);
  c6->SetLogx();
  TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  pad1->SetBottomMargin(0); // No x-axis labels here
  pad1->SetLogx();
  pad1->Draw();
  pad1->cd();

  graphswNe[2]->GetYaxis()->SetTitleSize(0.05);
  graphswNe[2]->GetYaxis()->SetTitle("R_{AA}");
  graphswNe[2]->Draw("AP");
  graphswNe[3]->Draw("same P");
  fitswNe[2]->Draw("same");
  fitswNe[3]->Draw("same");
  leg3->Clear();
  leg3->AddEntry(graphswNe[2], labelswNe[2].c_str(), "lp");
  leg3->AddEntry(graphswNe[3], labelswNe[3].c_str(), "lp");
  leg3->Draw();
  c6->cd();

  TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
  pad2->SetTopMargin(0);
  pad2->SetLogx();
  pad2->SetBottomMargin(0.3);
  pad2->Draw();
  pad2->cd();
  
  gratio->Draw("AP");
  gratio->GetXaxis()->SetTitleSize(0.05);
  gratio->GetYaxis()->SetTitleSize(0.05);

  c6->SaveAs("OO_NeNe.pdf");


  // Save everything
  TFile *out = TFile::Open("outputs/outputGraphswNe.root", "RECREATE");
  for (size_t i = 0; i < graphs.size(); ++i) {
    graphswNe[i]->SetName(Form("Raa_%s", labelswNe[i].c_str()));
    graphswNe[i]->Write();
    fitswNe[i]->Write();
  }
  gratio->SetName("Raa_NeNe_over_Oxygen-Oxygen");
  gratio->Write();
  // Optionally write the evaluation graphs or just the fits
  out->Close();
  std::cout << "Saved graphs, fits, and evaluations to outputGraphs.root\n";
}
