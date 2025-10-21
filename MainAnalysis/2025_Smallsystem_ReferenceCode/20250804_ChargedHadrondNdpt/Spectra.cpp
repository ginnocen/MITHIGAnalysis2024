#include <TCanvas.h>
#include <TColor.h>
#include <TFile.h>
#include <TH1.h>
#include <TH1F.h>
#include <TLegend.h>
#include <TPad.h>
#include <TStyle.h>
#include <TSystem.h>
#include <iostream>

#include "MITHIG_CMSStyle.h"

int main(int argc, char *argv[]) {

  SetTDRStyle();

  if (argc != 8) {
    std::cout << "Wrong number of arguments!!!!!!!!!!!" << std::endl;
  }
  TString label = argv[1];
  TString filenameNucleusNucleus = argv[2];
  TString histonameNucleusNucleus = argv[3];
  TString filenameNucleusNucleusSyst = argv[4];
  TString histonameNucleusNucleusSyst = argv[5];
  float lumiscale = atof(argv[6]);
  TString plotfolder = argv[7];

  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetStatBorderSize(0);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);

  // load data
  TFile *fpp = TFile::Open("ResultsUIC/pp_OO_raa_20250729_Unblinding_Final_v3.root", "read");
  TH1D *pp = (TH1D *)fpp->Get("pp_Nominal_data_points");
  TH1D *ppSyst = (TH1D *)fpp->Get("pp_Total_uncertainty");

  TFile *fNucleusNucleus = TFile::Open(filenameNucleusNucleus, "read");
  TH1D *NucleusNucleus = (TH1D *)fNucleusNucleus->Get(histonameNucleusNucleus);
  TFile *fNucleusNucleusSyst = TFile::Open(filenameNucleusNucleusSyst, "read");
  TH1D *NucleusNucleusSyst = (TH1D *)fNucleusNucleusSyst->Get(histonameNucleusNucleusSyst);

  // applying lumi scaling for NucleusNucleus
  NucleusNucleus->Scale(1. / lumiscale);
  NucleusNucleusSyst->Scale(1. / lumiscale);
  // set up canvas and pads
  TCanvas *canv2 = new TCanvas("canv2", "canv2", 700, 800);
  canv2->SetBorderSize(0);
  TPad *pad1 = new TPad("pad1", "pad1", 0.0, 0.25, 1.0, 1.0, 0);
  TPad *pad2 = new TPad("pad2", "pad2", 0.0, 0.0, 1.0, 0.25, 0);
  canv2->SetLineWidth(0);
  pad1->SetBottomMargin(0);
  pad1->SetLeftMargin(0.15);
  pad1->SetTopMargin(0.07);
  pad1->SetBorderSize(0);
  pad1->Draw();
  pad2->SetTopMargin(0);
  pad2->SetLeftMargin(0.15);
  pad2->SetBottomMargin(0.3);
  pad2->SetBorderSize(0);
  pad2->Draw();
  pad1->cd();
  pad1->SetLogx();
  pad1->SetLogy();

  // dummy histogram to define the frame
  TH1D *ppSpecD = new TH1D("specDummy1", "", 3, 2, 120);
  ppSpecD->GetYaxis()->SetTitle("#frac{1}{4#pi p_{T}} #frac{d#sigma}{dp_{T}} (mb/GeV^{2})");
  ppSpecD->GetYaxis()->SetTitleOffset(1.4);
  ppSpecD->GetYaxis()->SetTitleSize(0.045);
  ppSpecD->GetYaxis()->SetLabelSize(0.04);
  ppSpecD->GetYaxis()->CenterTitle();
  ppSpecD->GetYaxis()->SetLabelOffset(0.002);
  ppSpecD->GetYaxis()->SetRangeUser(1.1e-14, 1e5);
  ppSpecD->GetXaxis()->SetRangeUser(2, 120);
  ppSpecD->Draw();

  // drawing data
  pp->SetMarkerStyle(8);
  pp->SetMarkerColor(kBlack);
  pp->SetLineWidth(2);
  pp->SetLineColor(kBlack);
  pp->GetXaxis()->SetRangeUser(3, 100);
  pp->Draw("p same");

  NucleusNucleus->SetMarkerStyle(21);
  NucleusNucleus->SetLineWidth(2);
  NucleusNucleus->SetMarkerColor(TColor::GetColor("#5790fc"));
  NucleusNucleus->SetLineColor(TColor::GetColor("#5790fc"));
  NucleusNucleus->GetXaxis()->SetRangeUser(3, 100);
  NucleusNucleus->Draw("p same");

  // legends
  TLegend *specLeg = new TLegend(0.6, 0.75, 1, 0.9);
  specLeg->SetTextFont(42);
  specLeg->SetTextSize(0.05);
  specLeg->SetFillStyle(0);
  specLeg->AddEntry((TObject *)0, "|#eta| < 1", "");
  specLeg->AddEntry(pp, "pp", "p");
  specLeg->AddEntry(NucleusNucleus, label, "p");
  specLeg->SetFillStyle(0);
  specLeg->Draw("same");

  TLegend *systLeg = new TLegend(0.2, 0.03, 0.6, 0.18);
  systLeg->SetTextFont(42);
  systLeg->SetTextSize(0.05);
  systLeg->SetFillStyle(0);
  systLeg->AddEntry((TObject *)0, "", "");
  systLeg->AddEntry(ppSyst, "pp", "f");
  systLeg->AddEntry(NucleusNucleusSyst, label, "f");
  systLeg->SetFillStyle(0);
  systLeg->Draw("same");

  TLegend *systLeg2 = new TLegend(0.15, 0.03, 0.65, 0.23);
  systLeg2->SetTextFont(42);
  systLeg2->SetTextSize(0.05);
  systLeg2->SetFillStyle(0);
  systLeg2->SetTextAlign(22);
  systLeg2->AddEntry((TObject *)0, "Normalization", "");
  systLeg2->AddEntry((TObject *)0, "uncertainty", "");
  systLeg2->AddEntry((TObject *)0, "5%", "");
  systLeg2->AddEntry((TObject *)0, "5%", "");
  systLeg2->SetFillStyle(0);
  systLeg2->Draw("same");

  // lower panel
  pad2->cd();
  pad2->SetLogx();
  TH1D *ppSpecD2 = new TH1D("specDummy2", "", 3, 2, 120);
  ppSpecD2->GetYaxis()->SetRangeUser(0.0, 9.999);
  ppSpecD2->GetYaxis()->SetNdivisions(4, 4, 0, kTRUE);
  ppSpecD2->GetYaxis()->SetTitleOffset(0.4);
  ppSpecD2->GetYaxis()->SetTitleFont(42);
  ppSpecD2->GetYaxis()->SetTitleSize(0.095 * 1.2);
  ppSpecD2->GetYaxis()->SetLabelSize(0.095 * 1.2);
  ppSpecD2->GetXaxis()->SetTitleFont(42);
  ppSpecD2->GetYaxis()->SetTitle(Form("Syst. uncert. (%s)", "%"));
  ppSpecD2->GetXaxis()->SetRangeUser(2, 120);
  ppSpecD2->GetXaxis()->SetTitle("p_{T} (GeV)");
  ppSpecD2->GetXaxis()->SetTitleSize(0.1 * 1.2);
  ppSpecD2->GetXaxis()->SetLabelSize(0.1 * 1.2);
  ppSpecD2->GetXaxis()->SetTitleOffset(1.2);
  ppSpecD2->GetXaxis()->CenterTitle();
  ppSpecD2->GetXaxis()->SetTickLength(0.06);
  ppSpecD2->Draw();

  // drawing systematics
  ppSyst->SetFillColor(kBlack);
  ppSyst->SetFillStyle(3003);
  ppSyst->GetXaxis()->SetRangeUser(3, 100);
  ppSyst->SetLineColor(kBlack);
  ppSyst->SetLineWidth(3);
  ppSyst->Draw("same HIST");

  // drawing systematics
  NucleusNucleusSyst->SetFillColor(TColor::GetColor("#5790fc"));
  NucleusNucleusSyst->SetFillStyle(3004);
  NucleusNucleusSyst->GetXaxis()->SetRangeUser(3, 100);
  NucleusNucleusSyst->SetLineColor(TColor::GetColor("#5790fc"));
  NucleusNucleusSyst->SetLineWidth(3);
  NucleusNucleusSyst->Draw("same HIST");

  CMS_lumi(pad1, 0, 11);

  canv2->SaveAs(Form("%s/Figure_001.pdf", plotfolder.Data()));
  canv2->SaveAs(Form("%s/Figure_001.png", plotfolder.Data()));
  canv2->SaveAs(Form("%s/Figure_001.C", plotfolder.Data()));

    // Create a new ROOT file
    TFile* outputFile = new TFile("Results/NucleusNucleus_raa_20250805_Unblinding.root", "RECREATE");


// First create the ratio histograms
TH1D *Unnormalized_RAA_NucleusNucleus = (TH1D *)NucleusNucleus->Clone("Unnormalized_RAA_NucleusNucleus");
Unnormalized_RAA_NucleusNucleus->Reset();

TH1D *Raa_Total_uncertainty_NucleusNucleus = (TH1D *)NucleusNucleus->Clone("Raa_Total_uncertainty_NucleusNucleus");
Raa_Total_uncertainty_NucleusNucleus->Reset();

int nBins = NucleusNucleus->GetNbinsX();

for (int i = 1; i <= nBins; ++i) {
    // Central values
    double A     = NucleusNucleus->GetBinContent(i);
    double B     = pp->GetBinContent(i);

    // Statistical uncertainties (absolute)
    double dA_stat = NucleusNucleus->GetBinError(i);
    double dB_stat = pp->GetBinError(i);

    // Systematic uncertainties
    double dA_syst = NucleusNucleusSyst->GetBinError(i); // absolute
    double relB_syst_percent = ppSyst->GetBinError(i);   // in percent
    double dB_syst = (relB_syst_percent / 100.0) * B;     // convert to absolute

    if (A > 0 && B > 0) {
        double R = A / B;

        // Statistical error propagation
        double dR_stat = R * std::sqrt( (dA_stat / A)*(dA_stat / A) + (dB_stat / B)*(dB_stat / B) );

        // Systematic error propagation
        double dR_syst = R * std::sqrt( (dA_syst / A)*(dA_syst / A) + (dB_syst / 100.)*(dB_syst / 100.) );
       
        Unnormalized_RAA_NucleusNucleus->SetBinContent(i, R);
        Unnormalized_RAA_NucleusNucleus->SetBinError(i, dR_stat);

        Raa_Total_uncertainty_NucleusNucleus->SetBinContent(i, R);
        Raa_Total_uncertainty_NucleusNucleus->SetBinError(i, dR_syst);}
}




    Raa_Total_uncertainty_NucleusNucleus->Write();
    Unnormalized_RAA_NucleusNucleus->Write();
    outputFile->Close();


  return 0;


}
