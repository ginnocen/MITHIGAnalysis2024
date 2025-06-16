#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TMath.h"
#include "TFile.h"

#include "CommandLine.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm> // For std::max_element

using namespace std;

bool checkFileType(string fileType);

void plotGraph(TH1* inHist, const char* yAxisTitle, const char* xAxisTitle,
                 const char* latexText, const char* latexText2, const char* plotname, const int nDim);

void plotOverlapGraph(TH1D* inHist, TH1D* inHist2, string fileType, string fileType2, const char* yAxisTitle, const char* xAxisTitle,
                 const char* latexText, const char* latexText2, const char* plotname);

int main(int argc, char *argv[])
{
  CommandLine CL(argc, argv);
  string         PlotDir       = CL.Get    ("PlotDir", "");       // subdirectory under plot/
  float MinDzeroPT = CL.GetDouble("MinDzeroPT", 2);  // Minimum Dzero transverse momentum threshold for Dzero selection.
  float MaxDzeroPT = CL.GetDouble("MaxDzeroPT", 5);  // Maximum Dzero transverse momentum threshold for Dzero selection.
  float MinDzeroY = CL.GetDouble("MinDzeroY", 0);  // Minimum Dzero rapidity threshold for Dzero selection.
  float MaxDzeroY = CL.GetDouble("MaxDzeroY", 1);  // Maximum Dzero rapidity threshold for Dzero selection.
  bool IsGammaN = CL.GetBool("IsGammaN", true);      // GammaN analysis (or NGamma)
  string fileType = CL.Get("fileType", ""); // determine wether the file is Data, MC or MC_inclusive
  string fileType2 = CL.Get("fileType2", ""); // give second fileType only if want to overlap two plots /w ratio determine wether the file is Data, MC or MC_inclusive

  if (!checkFileType(fileType) || (fileType2 != "" && !checkFileType(fileType2) )) { std::cout << "[Error] Invalid fileType entered in config!" << std::endl; return -1; }

  int HFEMax = CL.GetInt("HFEMax",-999); // Read in HFEMax values for plotting

  string inputFileName = Form("rapGapScan/pt%d-%d_y%d-%d_IsGammaN%o_DoSystRapGap%d/%s.root", (int) MinDzeroPT, (int) MaxDzeroPT, (int) MinDzeroY, (int) MaxDzeroY, IsGammaN, (int) HFEMax, fileType.c_str() );

  TFile *inFile = new TFile(inputFileName.c_str());
  TH2D *hHFEmaxPlus_vs_EvtMult = (TH2D*) inFile->Get("hHFEmaxPlus_vs_EvtMult");
  TH1D *hHFEmaxPlus = (TH1D*) hHFEmaxPlus_vs_EvtMult->ProjectionX("hHFEmaxPlus");
  hHFEmaxPlus->Rebin(2);
  hHFEmaxPlus->Scale(1./hHFEmaxPlus->Integral());
  TH2D *hHFEmaxMinus_vs_EvtMult = (TH2D*) inFile->Get("hHFEmaxMinus_vs_EvtMult");
  TH1D *hHFEmaxMinus = (TH1D*) hHFEmaxMinus_vs_EvtMult->ProjectionX("hHFEmaxMinus");
  hHFEmaxMinus->Rebin(2);
  hHFEmaxMinus->Scale(1./hHFEmaxMinus->Integral());

  /////////////////////////////////
  // 1. Plot the HF energy distributions
  /////////////////////////////////

  const char* latexText = Form("%d < D_{p_{T}} < %d (GeV/#it{c})", (int) MinDzeroPT, (int) MaxDzeroPT);
  const char* latexText2 = Form("%d < D_{y} < %d", (int) MinDzeroY, (int) MaxDzeroY);

  if (fileType2=="") {
    plotGraph(hHFEmaxPlus, "Normalized counts", "HF E_{max} Plus [GeV]",
              latexText, latexText2,
              Form("%s/HFEmaxPlus_pt%d-%d_y%d-%d_IsGammaN%o_HFETh%d_%s.pdf",
                    PlotDir.c_str(),
                    (int) MinDzeroPT, (int) MaxDzeroPT, (int) MinDzeroY, (int) MaxDzeroY,
                    IsGammaN, (int) HFEMax, fileType.c_str()), 1);

    plotGraph(hHFEmaxMinus, "Normalized counts", "HF E_{max} Minus [GeV]",
              latexText, latexText2,
              Form("%s/HFEmaxMinus_pt%d-%d_y%d-%d_IsGammaN%o_HFETh%d_%s.pdf",
                    PlotDir.c_str(),
                    (int) MinDzeroPT, (int) MaxDzeroPT, (int) MinDzeroY, (int) MaxDzeroY,
                    IsGammaN, (int) HFEMax, fileType.c_str()), 1);

    plotGraph(hHFEmaxPlus_vs_EvtMult, "Charged hadron multiplicity", "HF E_{max} Plus [GeV]",
              latexText, latexText2,
              Form("%s/HFEmaxPlus_vs_EvtMult_pt%d-%d_y%d-%d_IsGammaN%o_HFETh%d_%s.pdf",
                    PlotDir.c_str(),
                    (int) MinDzeroPT, (int) MaxDzeroPT, (int) MinDzeroY, (int) MaxDzeroY,
                    IsGammaN, (int) HFEMax, fileType.c_str()), 2);
    
    plotGraph(hHFEmaxMinus_vs_EvtMult, "Charged hadron multiplicity", "HF E_{max} Minus [GeV]",
              latexText, latexText2,
              Form("%s/HFEmaxMinus_vs_EvtMult_pt%d-%d_y%d-%d_IsGammaN%o_HFETh%d_%s.pdf",
                    PlotDir.c_str(),
                    (int) MinDzeroPT, (int) MaxDzeroPT, (int) MinDzeroY, (int) MaxDzeroY,
                    IsGammaN, (int) HFEMax, fileType.c_str()), 2);
  } else {
    string inputFileName2 = Form("rapGapScan/pt%d-%d_y%d-%d_IsGammaN%o_DoSystRapGap%d/%s.root", (int) MinDzeroPT, (int) MaxDzeroPT, (int) MinDzeroY, (int) MaxDzeroY, IsGammaN, (int) HFEMax, fileType2.c_str() );
    TFile *inFile2 = new TFile(inputFileName2.c_str());
    TH1D *hHFEmaxPlus2 = (TH1D*) ((TH2D*) inFile2->Get("hHFEmaxPlus_vs_EvtMult"))->ProjectionX("hHFEmaxPlus2");
    hHFEmaxPlus2->Rebin(2);
    hHFEmaxPlus2->Scale(1./hHFEmaxPlus2->Integral());
    TH1D *hHFEmaxMinus2 = (TH1D*) ((TH2D*) inFile2->Get("hHFEmaxMinus_vs_EvtMult"))->ProjectionX("hHFEmaxMinus2");
    hHFEmaxMinus2->Rebin(2);
    hHFEmaxMinus2->Scale(1./hHFEmaxMinus2->Integral());

    plotOverlapGraph(hHFEmaxPlus, hHFEmaxPlus2, fileType, fileType2, "Normalized counts", "HF E_{max} Plus [GeV]",
                latexText, latexText2, Form("%s/HFEmaxPlus_pt%d-%d_y%d-%d_IsGammaN%o_HFETh%d_%s_over_%s.pdf",
                    PlotDir.c_str(),
                    (int) MinDzeroPT, (int) MaxDzeroPT, (int) MinDzeroY, (int) MaxDzeroY,
                    IsGammaN, (int) HFEMax, fileType.c_str(), fileType2.c_str()));
    plotOverlapGraph(hHFEmaxMinus, hHFEmaxMinus2, fileType, fileType2, "Normalized counts", "HF E_{max} Minus [GeV]",
                latexText, latexText2, Form("%s/HFEmaxMinus_pt%d-%d_y%d-%d_IsGammaN%o_HFETh%d_%s_over_%s.pdf",
                    PlotDir.c_str(),
                    (int) MinDzeroPT, (int) MaxDzeroPT, (int) MinDzeroY, (int) MaxDzeroY,
                    IsGammaN, (int) HFEMax, fileType.c_str(), fileType2.c_str()));
  }

  return 0;
}


void plotGraph(TH1* inHist, const char* yAxisTitle, const char* xAxisTitle,
                 const char* latexText, const char* latexText2, const char* plotname, const int nDim)
{
  // Create canvas
  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 800);
  canvas->SetLeftMargin(0.13);
  if (nDim==1) {
    canvas->SetRightMargin(0.04);
  } else if (nDim==2) {
    canvas->SetRightMargin(0.12);
  }
  canvas->SetBottomMargin(0.12);
  canvas->SetTopMargin(0.08);
  gPad->SetLogy();
  if (nDim==2) {
    gPad->SetLogz();
  }

  // Create and configure the histogram frame
  inHist->GetYaxis()->SetTitle(yAxisTitle);
  inHist->GetXaxis()->SetTitle(xAxisTitle);
  inHist->SetStats(0);
  inHist->GetYaxis()->SetTitleOffset(1.5);
  if (nDim==1) {
    inHist->Draw();
  } else if (nDim==2) {
    inHist->Draw("colz");
  }

  // Add TLatex for additional text
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.035);
  latex.SetTextFont(42);
  latex.DrawLatex(0.6, 0.32, latexText);
  TLatex latex2;
  latex2.SetNDC();
  latex2.SetTextSize(0.035);
  latex2.SetTextFont(42);
  latex2.DrawLatex(0.6, 0.27, latexText2);

  // Update and save the canvas
  canvas->Update();
  canvas->SaveAs(plotname);

  // Clean up
  delete canvas;
}

void plotOverlapGraph(TH1D* inHist, TH1D* inHist2, string fileType, string fileType2, const char* yAxisTitle, const char* xAxisTitle,
                 const char* latexText, const char* latexText2, const char* plotname)
{
  // Create canvas
  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 800);
  canvas->SetLeftMargin(0.13);
  canvas->SetRightMargin(0.04);
  canvas->SetBottomMargin(0.12);
  canvas->SetTopMargin(0.08);

  TPad* padUp = new TPad("padUp", "padUp", 0, 0.3, 1, 1);
  padUp->SetBottomMargin(0);
  padUp->Draw();
  padUp->cd();
  gPad->SetLogy();

  auto styleUpHist = [](TH1 *hist) {
    hist->GetXaxis()->SetTitleSize(0.045);
    hist->GetXaxis()->SetLabelSize(0.04);
    hist->GetYaxis()->SetTitleSize(0.045);
    hist->GetYaxis()->SetLabelSize(0.04);
    hist->GetYaxis()->ChangeLabel(1,0,0);
  };

  auto styleDownHist = [](TH1 *hist) {
    hist->GetXaxis()->SetTitleSize(0.05 / 0.4);
    hist->GetXaxis()->SetLabelSize(0.045 / 0.4);
    hist->GetYaxis()->SetTitleSize(0.05 / 0.4);
    hist->GetYaxis()->SetLabelSize(0.045 / 0.4);
    hist->GetXaxis()->SetLabelOffset(0.01);
    hist->GetYaxis()->SetTitleOffset(.3);
    hist->GetYaxis()->ChangeLabel(-1,0,0);
    hist->GetYaxis()->SetNdivisions(505, true);
  };

  // Configure the histogram frame
  inHist->GetYaxis()->SetTitle(yAxisTitle);
  inHist->GetXaxis()->SetTitle(xAxisTitle);
  inHist->SetStats(0);
  inHist->SetLineColor(kBlue);
  inHist->GetYaxis()->SetTitleOffset(1.);
  inHist->GetYaxis()->SetRangeUser(1e-4,1.2*TMath::Max(inHist->GetMaximum(),inHist2->GetMaximum()));
  styleUpHist(inHist);
  inHist->Draw("hist f e");

  inHist2->GetYaxis()->SetTitle(yAxisTitle);
  inHist2->GetXaxis()->SetTitle(xAxisTitle);
  inHist2->SetStats(0);
  inHist2->SetLineColor(kRed);
  inHist2->GetYaxis()->SetTitleOffset(1.);
  inHist2->Draw("same");

  TLegend* legend = new TLegend(0.3, 0.7, 0.6, 0.9);
  legend->AddEntry(inHist, fileType.c_str(), "l");
  legend->AddEntry(inHist2, fileType2.c_str(), "l");
  legend->SetFillStyle(0);  // Transparent fill
  legend->SetLineStyle(0);  // Transparent border
  legend->SetBorderSize(0); // No border
  legend->SetTextSize(0.045);
  legend->Draw();

  // Add TLatex for additional text
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.045);
  latex.SetTextFont(42);
  TLatex latex2;
  latex2.SetNDC();
  latex2.SetTextSize(0.045);
  latex2.SetTextFont(42);
  latex.DrawLatex(0.6, 0.77, latexText);
  latex2.DrawLatex(0.6, 0.82, latexText2);

  canvas->cd();
  TPad* padDown = new TPad("padDown", "padDown", 0, 0.05, 1, .3);
  padDown->SetTopMargin(0);
  padDown->SetBottomMargin(.4);
  padDown->Draw();
  padDown->cd();

  TH1D* ratioHist = (TH1D*) inHist->Clone("ratioHist");
  ratioHist->SetStats(0);
  ratioHist->Sumw2();
  ratioHist->GetYaxis()->SetTitle(Form("%s/%s",fileType.c_str(),fileType2.c_str()));
  ratioHist->Divide(inHist2);
  ratioHist->GetYaxis()->SetRangeUser(0, 2);
  styleDownHist(ratioHist);
  ratioHist->Draw("ep");

  // Update and save the canvas
  canvas->Update();
  canvas->SaveAs(plotname);

  // Clean up
  delete padUp;
  delete padDown;
  delete legend;
  delete canvas;
}

bool checkFileType(string fileType) {
  if (fileType == "Data" || fileType == "MC" || fileType == "MC_inclusive") {
    return true;
  }
  return false;
}
