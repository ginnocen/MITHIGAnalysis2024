/* makeGraphFromDAT.C: take in DAT file, produce root file with output
 * Hannah Bossi, <hannah.bossi@cern.ch>, 08/20/2025
 */
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TStyle.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>


TGraphAsymmErrors*  getGraphFromDAT(std::string inputFile);

void makeGraphFromDAT(){
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TGraphAsymmErrors* g = (TGraphAsymmErrors*) getGraphFromDAT("CUJETprediction_NeNe5360_0-100.dat"); 
  TGraphAsymmErrors* g2 = (TGraphAsymmErrors*) getGraphFromDAT("CUJETprediction_OO5360_0-100.dat");
  
  // plot just to check if it's correct
  TCanvas *c = new TCanvas("c","c",800,600);
  g->Draw("A3");   // "3" = draw filled band
  g->GetHistogram()->GetXaxis()->SetTitle("#it{p}_{T}");
  g->GetHistogram()->GetYaxis()->SetTitle("#it{R}_{AA}");
  g->GetHistogram()->GetYaxis()->SetRangeUser(0.0, 1.2);
  g->SetFillColorAlpha(kRed, 0.3);
  g->SetLineColor(kRed);
  g2->SetFillColorAlpha(kBlue, 0.3);
  g2->SetLineColor(kBlue);
  g2->Draw("3 same");

  TLegend* l = new TLegend(0.7, 0.16, 0.88, 0.3);
  l->SetFillColor(0);
  l->SetBorderSize(0);
  l->AddEntry(g, "NeNe", "f");
  l->AddEntry(g2, "OO", "f");
  l->Draw(); 
  c->SaveAs("MinBiasTheory_CUJET.pdf");

  // write to a root file
  TFile* outFile = new TFile("MinBias_CUJET.root", "RECREATE");
  outFile->cd();
  g->SetName("NeNeMinBias");
  g->Write();
  g2->SetName("OOMinBias");
  g2->Write();
  outFile->Close();
}

// take in a dat, return a TGraph - for now take 0-100%
TGraphAsymmErrors*  getGraphFromDAT(std::string inputFile){

  // read in the input file
  std::ifstream infile(inputFile);

  std::vector<double> x, y, ex_low, ex_high, ey_low, ey_high;
  std::string line;

  while (std::getline(infile, line)) {
    if (line.empty() || line[0] == '#') continue; // skip comments & blanks

    double pT, RAA_central, RAA_upper, RAA_lower;
    std::istringstream ss(line);
    if (!(ss >> pT >> RAA_central >> RAA_upper >> RAA_lower))
      continue;

    x.push_back(pT);
    y.push_back(RAA_central);

    ex_low.push_back(0.0);   // no x errors
    ex_high.push_back(0.0);

    ey_high.push_back(RAA_upper - RAA_central); // upward error
    ey_low.push_back(RAA_central - RAA_lower);  // downward error
  }
  
  TGraphAsymmErrors *gr = new TGraphAsymmErrors( x.size(), x.data(), y.data(),ex_low.data(), ex_high.data(), ey_low.data(), ey_high.data());



  return gr; 
}
