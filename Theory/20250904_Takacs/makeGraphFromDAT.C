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


TGraphAsymmErrors*  getGraphFromDAT(std::string inputFile, int mode);

void makeGraphFromDAT(){
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TGraphAsymmErrors* g = (TGraphAsymmErrors*) getGraphFromDAT("OO_baseline_RAA_hadron.dat", 0); 
  TGraphAsymmErrors* g2 = (TGraphAsymmErrors*) getGraphFromDAT("OO_baseline_RAA_hadron.dat", 1);
  TGraphAsymmErrors* g3 = (TGraphAsymmErrors*) getGraphFromDAT("OO_0-100_RAA_hadron.dat", 2);
  TGraphAsymmErrors* g4 = (TGraphAsymmErrors*) getGraphFromDAT("OO_0-100_RAA_hadron.dat", 3);

  
  // plot just to check if it's correct
  TCanvas *c = new TCanvas("c","c",800,600);
  c->SetLogx(); 
  g->Draw("A3");   // "3" = draw filled band
  g->GetHistogram()->GetXaxis()->SetTitle("#it{p}_{T}");
  g->GetHistogram()->GetYaxis()->SetTitle("#it{R}_{AA}");
  g->GetHistogram()->GetYaxis()->SetRangeUser(0.6, 1.2);
  g->SetFillColorAlpha(kRed, 0.3);
  g->SetLineColor(kRed);
  g2->SetFillColorAlpha(kBlue, 0.3);
  g2->SetLineColor(kBlue);
  g2->Draw("L3 same");
  g3->SetFillColorAlpha(kGreen, 0.3);
  g3->SetLineColor(kGreen);
  g3->Draw("L3 same");
  g4->SetFillColorAlpha(kGray, 0.3);
  g4->SetLineColor(kGray);
  g4->Draw("L3 same");

  TLine* l2 = new TLine(0, 1, 220, 1);
  l2->SetLineColor(kBlack);
  l2->SetLineStyle(2);
  l2->Draw("same");

  TLegend* l = new TLegend(0.4, 0.13, 0.6, 0.3);
  l->SetFillColor(0);
  l->SetBorderSize(0);
  l->SetTextSize(0.035); 
  l->AddEntry(g, "No Quenching Baseline, scale uncert.", "f");
  l->AddEntry(g2, "No Quenching Baseline, nPDF uncert.", "f");
  l->AddEntry(g3, "OO 0-100% Bayes", "f");
  l->AddEntry(g4, "OO 0-100% nPDF", "f");
  l->Draw(); 
  c->SaveAs("MinBiasTheory_Takacs.pdf");

  
  // write to a root file
  TFile* outFile = new TFile("MinBias_Takacs.root", "RECREATE");
  outFile->cd();
  g->SetName("OO_Baseline_Scale");
  g->Write();
  g2->SetName("OO_Baseline_nPDF");
  g2->Write();
  g3->SetName("OO_0_100_Bayes");
  g3->Write();
  g4->SetName("OO_0_100_nPDF");
  g4->Write();
  outFile->Close();
}

// take in a dat, return a TGraph - for now take 0-100%]
// mode = 0 (OO baseline scale), 1 (baseline npdf), 2 (quenching scale), 3 (quenching ndpf)
TGraphAsymmErrors*  getGraphFromDAT(std::string inputFile, int mode){

  // read in the input file
  std::ifstream infile(inputFile);

  std::vector<double> x, y, ex_low, ex_high, ey_low, ey_high;
  std::string line;

  while (std::getline(infile, line)) {
    if (line.empty() || line[0] == '#') continue; // skip comments & blanks

    double pT, scale_up, scale_low ,nPDF_up, nPDF_low; 
    std::istringstream ss(line);
    if (!(ss >> pT  >> scale_up >> scale_low >> nPDF_up >> nPDF_low))
      continue;

    double Raa = (scale_up + scale_low)/2;
    x.push_back(pT);
    if(mode == 0){
      y.push_back(Raa);
      ey_high.push_back(scale_up - Raa );
      ey_low.push_back(Raa -scale_low); 
    }
    else if(mode == 1){
      y.push_back(Raa);
      std::cout << "NPDFup Error: " << (nPDF_up - Raa) << "upper band value: " << nPDF_up << "nPDFdown error " << (Raa - nPDF_low) << std::endl;
      ey_high.push_back(nPDF_up - Raa);
      ey_low.push_back(Raa - nPDF_low);
    } 
    else if(mode == 2){
      y.push_back(Raa);
      ey_high.push_back(scale_up - Raa );
      ey_low.push_back(Raa -scale_low);
    }
    else if(mode == 3){
      y.push_back(Raa);
      ey_high.push_back(nPDF_up - Raa);
      ey_low.push_back(Raa -nPDF_low);
    }
    else{
      std::cout << "[ERROR]: mode "  << mode <<"  not recognized" << std::endl;
      y.push_back(-1);
      ey_high.push_back(0.0);
      ey_low.push_back(0.0);
    }

    ex_low.push_back(0.0);   // no x errors
    ex_high.push_back(0.0);

  }
  
  TGraphAsymmErrors *gr = new TGraphAsymmErrors( x.size(), x.data(), y.data(),ex_low.data(), ex_high.data(), ey_low.data(), ey_high.data());



  return gr; 
}
