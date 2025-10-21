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

  TGraphAsymmErrors* g = (TGraphAsymmErrors*) getGraphFromDAT("RAA_Ne+Ne.dat"); 
  TGraphAsymmErrors* g2 = (TGraphAsymmErrors*) getGraphFromDAT("RAA_O+O.dat");
  TGraphAsymmErrors* g3 = (TGraphAsymmErrors*) getGraphFromDAT("RAA_Ne+Ne_mQGP.dat");
  TGraphAsymmErrors* g4 = (TGraphAsymmErrors*) getGraphFromDAT("RAA_O+O_mQGP.dat");
  
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
  g3->SetFillColorAlpha(kGreen, 0.3);
  g3->SetLineColor(kGreen);
  g3->Draw("3 same");
  g4->SetFillColorAlpha(kGray+2, 0.3);
  g4->SetLineColor(kGray+2);
  g4->Draw("3 same");

  TLegend* l = new TLegend(0.7, 0.16, 0.88, 0.3);
  l->SetFillColor(0);
  l->SetBorderSize(0);
  l->AddEntry(g, "NeNe", "f");
  l->AddEntry(g3, "NeNe mQGP", "f");
  l->AddEntry(g2, "OO", "f");
  l->AddEntry(g4, "OO mQGP", "f");
  l->Draw(); 
  c->SaveAs("MinBiasTheory_Zakharov.pdf");

  // write to a root file
  TFile* outFile = new TFile("MinBias_Zakharov.root", "RECREATE");
  outFile->cd();
  g->SetName("NeNeMinBias");
  g->Write();
  g2->SetName("OOMinBias");
  g2->Write();
  g3->SetName("NeNeMinBias_mQGP");
  g3->Write();
  g4->SetName("OOMinBias_mQGP");
  g4->Write();
  outFile->Close();
}

// take in a dat, return a TGraph - for now take 0-100%
TGraphAsymmErrors*  getGraphFromDAT(std::string inputFile){

  // read in the input file
  std::ifstream infile(inputFile);

  // only take 0-100% for now 
  std::vector<double> pt, raa;
  std::vector<double> exl, exh, eyl, eyh; 

  std::string line;
  // Skip header line(s)
  std::getline(infile, line);

  while (std::getline(infile, line)) {
    if (line.empty()) continue;

    std::istringstream iss(line);

    // even though we aren't using all of these, we need them to read in data
    double pT, raa_0_10, erraa_0_10;
    double raa_10_20, erraa_10_20;
    double raa_0_30, erraa_0_30;
    double raa_0_100, erraa_0_100;
    double raa_pdf;
    int A, m;

    iss >> pT >> raa_0_10 >> erraa_0_10 >> raa_10_20 >> erraa_10_20 >> raa_0_30 >> erraa_0_30 >> raa_0_100 >> erraa_0_100 >> raa_pdf >> A >> m;

    if (iss.fail()) continue;

    pt.push_back(pT);
    raa.push_back(raa_0_100);

    // symmetric errors -> fill both sides equally
    exl.push_back(0.0);   // no x errors
    exh.push_back(0.0);
    eyl.push_back(erraa_0_100);
    eyh.push_back(erraa_0_100);
  }

  int n = pt.size();
  TGraphAsymmErrors *gr = new TGraphAsymmErrors(n,pt.data(), raa.data(), exl.data(), exh.data(), eyl.data(), eyh.data());
  return gr; 
}
