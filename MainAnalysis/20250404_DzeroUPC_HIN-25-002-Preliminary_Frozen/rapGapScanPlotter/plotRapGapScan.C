#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TMath.h"

#include "CommandLine.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm> // For std::max_element

#include "plotRapGapScan.h"

using namespace std;


int main(int argc, char *argv[])
{
  CommandLine CL(argc, argv);
  string         PlotDir       = CL.Get    ("PlotDir", "");       // subdirectory under plot/
  float MinDzeroPT = CL.GetDouble("MinDzeroPT", 2);  // Minimum Dzero transverse momentum threshold for Dzero selection.
  float MaxDzeroPT = CL.GetDouble("MaxDzeroPT", 5);  // Maximum Dzero transverse momentum threshold for Dzero selection.
  float MinDzeroY = CL.GetDouble("MinDzeroY", 0);  // Minimum Dzero rapidity threshold for Dzero selection.
  float MaxDzeroY = CL.GetDouble("MaxDzeroY", 1);  // Maximum Dzero rapidity threshold for Dzero selection.
  bool IsGammaN = CL.GetBool("IsGammaN", true);      // GammaN analysis (or NGamma)

  vector<int> HFEMax = CL.GetIntVector("HFEMax",""); // Read in HFEMax values for plotting

  vector<string> inputPoints; // Input corrected yields md files

  for (int iFile=0; iFile < HFEMax.size(); iFile++) {
    inputPoints.push_back( (string) Form("rapGapScan/pt%d-%d_y%d-%d_IsGammaN%o_DoSystRapGap%d/MassFit/correctedYields.md", (int) MinDzeroPT, (int) MaxDzeroPT, (int) MinDzeroY, (int) MaxDzeroY, IsGammaN, (int) HFEMax[iFile] ));
  }

  /////////////////////////////////
  // 0. Extract the points from the vector of .md
  /////////////////////////////////

  // nominal central values
  const int nPoints = inputPoints.size();
  std::vector<Point> PointsArr = getPointArr(MinDzeroPT, MaxDzeroPT, IsGammaN, HFEMax, inputPoints);

  vector<double> HFEMaxValues = getDoubleArr(PointsArr, 
                           [](Point& p) -> double { return p.HFEMax;} );
  vector<double> HFEMaxErrors = getDoubleArr(PointsArr, 
                           [](Point& p) -> double { return 0.25;} );
  vector<double> correctedYieldValues = getDoubleArr(PointsArr, 
                           [](Point& p) -> double { return p.correctedYield;} );
  vector<double> correctedYieldErrors = getDoubleArr(PointsArr, 
                           [](Point& p) -> double { return p.correctedYieldError;} );
  vector<double> rawYieldValues = getDoubleArr(PointsArr,
                           [](Point& p) -> double { return p.rawYield;} );
  vector<double> rawYieldErrors = getDoubleArr(PointsArr,
                           [](Point& p) -> double { return p.rawYieldError;} );
  vector<double> effEvtValues = getDoubleArr(PointsArr,
                           [](Point& p) -> double { return p.effEvt;} );
  vector<double> effEvtErrors = getDoubleArr(PointsArr,
                           [](Point& p) -> double { return p.effEvtError;} );
  vector<double> numEvtValues = getDoubleArr(PointsArr,
                           [](Point& p) -> double { return p.numEvt;} );
  vector<double> numEvtErrors = getDoubleArr(PointsArr,
                           [](Point& p) -> double { return TMath::Sqrt(p.numEvt);} );
  vector<double> denEvtValues = getDoubleArr(PointsArr,
                           [](Point& p) -> double { return p.denEvt;} );
  vector<double> denEvtErrors = getDoubleArr(PointsArr,
                           [](Point& p) -> double { return TMath::Sqrt(p.denEvt);} );
  vector<double> effDValues = getDoubleArr(PointsArr,
                           [](Point& p) -> double { return p.effD;} );
  vector<double> effDErrors = getDoubleArr(PointsArr,
                           [](Point& p) -> double { return p.effDError;} );
  vector<double> numDValues = getDoubleArr(PointsArr,
                           [](Point& p) -> double { return p.numD;} );
  vector<double> numDErrors = getDoubleArr(PointsArr,
                           [](Point& p) -> double { return TMath::Sqrt(p.numD);} );
  vector<double> denDValues = getDoubleArr(PointsArr,
                           [](Point& p) -> double { return p.denD;} );
  vector<double> denDErrors = getDoubleArr(PointsArr,
                           [](Point& p) -> double { return TMath::Sqrt(p.denD);} );

  printArr(correctedYieldValues, ", ", "correctedYieldValues: ");

  /////////////////////////////////
  // 1. Plot the cross section
  /////////////////////////////////
  // Create a canvas
  TCanvas* c1 = new TCanvas("c1", "D0 Cross Section", 800, 800);
  c1->SetLeftMargin(0.13);
  c1->SetRightMargin(0.04);
  c1->SetBottomMargin(0.12);
  c1->SetTopMargin(0.08);

  TH1F* hFrame = new TH1F("hFrame", " ", 100, 0, 17);
  hFrame->GetYaxis()->SetTitle("d^{2}#sigma/dydp_{T} (mb/GeV)");
  hFrame->GetXaxis()->SetTitle("HF Energy threshold [GeV]");
  hFrame->SetStats(0);
  hFrame->GetYaxis()->SetTitleOffset(1.5);
  hFrame->GetYaxis()->SetRangeUser(0, (*std::max_element(correctedYieldValues.begin(), correctedYieldValues.end()))*1.2);
  hFrame->Draw();

  TGraphErrors* gr = new TGraphErrors(nPoints, HFEMaxValues.data(), correctedYieldValues.data(), HFEMaxErrors.data(), correctedYieldErrors.data());
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(1.2);
  gr->SetLineColor(kRed);
  gr->SetMarkerColor(kRed);
  gr->SetLineWidth(2);

  gr->Draw("P E1 SAME");

  /*TLegend* leg = new TLegend(0.1, 0.15, 0.48, 0.28);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->AddEntry(gr, "New framework", "P");
  leg->Draw();*/

  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.035);
  latex.SetTextFont(42);
  TLatex latex2;
  latex2.SetNDC();
  latex2.SetTextSize(0.035);
  latex2.SetTextFont(42);
  // latex.DrawLatex(0.15, 0.92, "CMS #it{Preliminary} 1.38 nb^{-1} (5.36 TeV PbPb)");
  // latex.DrawLatex(0.15, 0.86, "UPCs, ZDC Xn0n w/ gap");
  // latex.DrawLatex(0.15, 0.82, "Global uncert. #pm 5.05%");
  latex.DrawLatex(0.6, 0.32, Form("%d < D_{p_{T}} < %d (GeV/#it{c})", (int) MinDzeroPT, (int) MaxDzeroPT));
  latex2.DrawLatex(0.6, 0.27, Form("%d < D_{y} < %d", (int) MinDzeroY, (int) MaxDzeroY));

  c1->Update();
  c1->SaveAs(Form("%s/rapGapScanCorrectedYields_pt%d-%d_y%d-%d_IsGammaN%o.pdf",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT, (int) MinDzeroY, (int) MaxDzeroY,
                  IsGammaN));
  delete gr;
  delete hFrame;

  /////////////////////////////////
  // 4. Plot other relevant plots
  /////////////////////////////////
  auto plotGraph = [](const char* yAxisTitle, const char* xAxisTitle,
                 double Emin, double Emax,
                 const std::vector<double>& xValues, const std::vector<double>& yValues,
                 const std::vector<double>& xErrors, const std::vector<double>& yErrors,
                 const char* latexText, const char* latexText2, const char* plotname,
                 int nBinsX=100, double xMin=0, double xMax=17)
  {
    // Create canvas
    TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 800);
    canvas->SetLeftMargin(0.13);
    canvas->SetRightMargin(0.04);
    canvas->SetBottomMargin(0.12);
    canvas->SetTopMargin(0.08);

    // Create and configure the histogram frame
    TH1F* hFrame = new TH1F("hFrame", "", nBinsX, xMin, xMax);
    hFrame->GetYaxis()->SetTitle(yAxisTitle);
    hFrame->GetXaxis()->SetTitle(xAxisTitle);
    hFrame->SetStats(0);
    hFrame->GetYaxis()->SetTitleOffset(1.5);
    hFrame->GetYaxis()->SetRangeUser(Emin, Emax);
    hFrame->Draw();

    // Create TGraphErrors for data points
    TGraphErrors* graph = new TGraphErrors(xValues.size(), xValues.data(), yValues.data(),
                                           xErrors.data(), yErrors.data());
    graph->Draw("P E1 SAME");

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
    delete graph;
    delete hFrame;
    delete canvas;
  };

  const char* latexText = Form("%d < D_{p_{T}} < %d (GeV/#it{c})", (int) MinDzeroPT, (int) MaxDzeroPT);
  const char* latexText2 = Form("%d < D_{y} < %d", (int) MinDzeroY, (int) MaxDzeroY);

  plotGraph("#varepsilon_{event}", "HF Energy threshold [GeV]",
            0., 1.05,
            HFEMaxValues, effEvtValues, HFEMaxErrors, effEvtErrors,
            latexText, latexText2,
            Form("%s/evtEff_pt%d-%d_y%d-%d_IsGammaN%o.pdf",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT, (int) MinDzeroY, (int) MaxDzeroY,
                  IsGammaN));

  plotGraph("Numerator N_{event}", "HF Energy threshold [GeV]",
            0, (*std::max_element(numEvtValues.begin(), numEvtValues.end()))*1.3,
            HFEMaxValues, numEvtValues, HFEMaxErrors, numEvtErrors,
            latexText, latexText2,
            Form("%s/evtNum_pt%d-%d_y%d-%d_IsGammaN%o.pdf",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT, (int) MinDzeroY, (int) MaxDzeroY,
                  IsGammaN));

  plotGraph("Denominator N_{event}", "HF Energy threshold [GeV]",
            0, (*std::max_element(denEvtValues.begin(), denEvtValues.end()))*1.3,
            HFEMaxValues, denEvtValues, HFEMaxErrors, denEvtErrors,
            latexText, latexText2,
            Form("%s/evtDen_pt%d-%d_y%d-%d_IsGammaN%o.pdf",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT, (int) MinDzeroY, (int) MaxDzeroY,
                  IsGammaN));

  plotGraph("#varepsilon_{D}", "HF Energy threshold [GeV]",
            0, 1.05,
            HFEMaxValues, effDValues, HFEMaxErrors, effDErrors,
            latexText, latexText2,
            Form("%s/DEff_pt%d-%d_y%d-%d_IsGammaN%o.pdf",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT, (int) MinDzeroY, (int) MaxDzeroY,
                  IsGammaN));

  plotGraph("#varepsilon_{D}", "HF Energy threshold [GeV]",
            0, 0.2, //(*std::max_element(effDValues.begin(), effDValues.end()))*1.3,
            HFEMaxValues, effDValues, HFEMaxErrors, effDErrors,
            latexText, latexText2,
            Form("%s/DEff_zoom_pt%d-%d_y%d-%d_IsGammaN%o.pdf",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT, (int) MinDzeroY, (int) MaxDzeroY,
                  IsGammaN));

  plotGraph("Numerator N_{D}", "HF Energy threshold [GeV]",
            0, (*std::max_element(numDValues.begin(), numDValues.end()))*1.3,
            HFEMaxValues, numDValues, HFEMaxErrors, numDErrors,
            latexText, latexText2,
            Form("%s/DNum_pt%d-%d_y%d-%d_IsGammaN%o.pdf",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT, (int) MinDzeroY, (int) MaxDzeroY,
                  IsGammaN));

  plotGraph("Denominator N_{D}", "HF Energy threshold [GeV]",
            0, (*std::max_element(denDValues.begin(), denDValues.end()))*1.3,
            HFEMaxValues, denDValues, HFEMaxErrors, denDErrors,
            latexText, latexText2,
            Form("%s/DDen_pt%d-%d_y%d-%d_IsGammaN%o.pdf",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT, (int) MinDzeroY, (int) MaxDzeroY,
                  IsGammaN));

  plotGraph("Raw yield", "HF Energy threshold [GeV]",
            0, (*std::max_element(rawYieldValues.begin(), rawYieldValues.end()))*1.3,
            HFEMaxValues, rawYieldValues, HFEMaxErrors, rawYieldErrors,
            latexText, latexText2,
            Form("%s/RawYield_pt%d-%d_y%d-%d_IsGammaN%o.pdf",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT, (int) MinDzeroY, (int) MaxDzeroY,
                  IsGammaN));


  return 0;
}
