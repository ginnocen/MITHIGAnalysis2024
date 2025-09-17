#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TLine.h"
#include "TMath.h"

#include "CommandLine.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm> // For std::max_element

#include "plotCrossSection.h"

using namespace std;

TGraphErrors* GetRatioGraph(TGraphErrors* gr1, TGraphErrors* gr2)
{
  if (!gr1 || !gr2)
  {
    std::cerr << "Error: One of the graphs is null!" << std::endl;
    return nullptr;
  }

  int n1 = gr1->GetN();
  int n2 = gr2->GetN();
  
  if (n1 != n2)
  {
    std::cerr << "Error: Graphs have different number of points!" << std::endl;
    return nullptr;
  }

  std::vector<double> xVals, yVals, xErrs, yErrs;

  for (int i = 0; i < n1; i++)
  {
    double x1, y1, x2, y2;
    gr1->GetPoint(i, x1, y1);
    gr2->GetPoint(i, x2, y2);
    
    double yErr1 = gr1->GetErrorY(i);
    double yErr2 = gr2->GetErrorY(i);
    double xErr1 = gr1->GetErrorX(i); // Assuming same x error for both
    double xErr2 = gr2->GetErrorX(i);

    if (x1 != -x2)
    {
      std::cerr << "Warning: X-values do not match at index " << i << std::endl;
      continue;
    }

    if (y2 == 0)
    { // Avoid division by zero
      std::cerr << "Warning: Division by zero at x = " << x1 << std::endl;
      continue;
    }

    double ratio = y1 / y2;
    double ratioErr = ratio * sqrt(pow(yErr1 / y1, 2) + pow(yErr2 / y2, 2));

    xVals.push_back(x2); // with respect to the gammaN rapidity
    yVals.push_back(ratio);
    xErrs.push_back((xErr1 + xErr2) / 2.0); // Taking average x error
    yErrs.push_back(ratioErr);
  }

  return new TGraphErrors(xVals.size(), xVals.data(), yVals.data(), xErrs.data(), yErrs.data());
}

int main(int argc, char *argv[])
{
  double MinDzeroPT = 2;
  double MaxDzeroPT = 5;
  vector<string> inputPoints_gammaN      = {
            "RST_redo0326_MCReweighting_cf_PR101_nomp3_ANv4GL/fullAnalysis/pt2-5_y-2--1_IsGammaN1/MassFit/correctedYields.md",
            "RST_redo0326_MCReweighting_cf_PR101_nomp3_ANv4GL/fullAnalysis/pt2-5_y-1-0_IsGammaN1/MassFit/correctedYields.md",
            "RST_redo0326_MCReweighting_cf_PR101_nomp3_ANv4GL/fullAnalysis/pt2-5_y0-1_IsGammaN1/MassFit/correctedYields.md",
            "RST_redo0326_MCReweighting_cf_PR101_nomp3_ANv4GL/fullAnalysis/pt2-5_y1-2_IsGammaN1/MassFit/correctedYields.md"};
  vector<string> inputPoints_Ngamma      = {
            "RST_redo0326_MCReweighting_cf_PR101_nomp3_ANv4GL/fullAnalysis/pt2-5_y1-2_IsGammaN0/MassFit/correctedYields.md",
            "RST_redo0326_MCReweighting_cf_PR101_nomp3_ANv4GL/fullAnalysis/pt2-5_y0-1_IsGammaN0/MassFit/correctedYields.md",
            "RST_redo0326_MCReweighting_cf_PR101_nomp3_ANv4GL/fullAnalysis/pt2-5_y-1-0_IsGammaN0/MassFit/correctedYields.md",
            "RST_redo0326_MCReweighting_cf_PR101_nomp3_ANv4GL/fullAnalysis/pt2-5_y-2--1_IsGammaN0/MassFit/correctedYields.md"};
  string PlotDir = "RST_redo0326_MCReweighting_cf_PR101_nomp3_ANv4GL/plot/";

  /////////////////////////////////
  // 0. Extract the points from the vector of .md
  /////////////////////////////////
  // nominal central values
  const int nPoints = inputPoints_gammaN.size();
  std::vector<Point> PointsArr_gammaN = getPointArr(MinDzeroPT, MaxDzeroPT, true, inputPoints_gammaN);

  vector<double> yValues_gammaN = getDoubleArr(PointsArr_gammaN, 
                           [](Point& p) -> double { return (p.ymax + p.ymin)/2.;} );
  vector<double> yErrors_gammaN = getDoubleArr(PointsArr_gammaN, 
                           [](Point& p) -> double { return (p.ymax - p.ymin)/2.;} );
  vector<double> correctedYieldValues_gammaN = getDoubleArr(PointsArr_gammaN, 
                           [](Point& p) -> double { return p.correctedYield;} );
  vector<double> correctedYieldErrors_gammaN = getDoubleArr(PointsArr_gammaN, 
                           [](Point& p) -> double { return p.correctedYieldError;} );
  vector<double> rawYieldValues_gammaN = getDoubleArr(PointsArr_gammaN,
                           [](Point& p) -> double { return p.rawYield;} );
  vector<double> rawYieldErrors_gammaN = getDoubleArr(PointsArr_gammaN,
                           [](Point& p) -> double { return p.rawYieldError;} );
  vector<double> effEvtValues_gammaN = getDoubleArr(PointsArr_gammaN,
                           [](Point& p) -> double { return p.effEvt;} );
  vector<double> effEvtErrors_gammaN = getDoubleArr(PointsArr_gammaN,
                           [](Point& p) -> double { return p.effEvtError;} );
  vector<double> numEvtValues_gammaN = getDoubleArr(PointsArr_gammaN,
                           [](Point& p) -> double { return p.numEvt;} );
  vector<double> numEvtErrors_gammaN = getDoubleArr(PointsArr_gammaN,
                           [](Point& p) -> double { return TMath::Sqrt(p.numEvt);} );
  vector<double> denEvtValues_gammaN = getDoubleArr(PointsArr_gammaN,
                           [](Point& p) -> double { return p.denEvt;} );
  vector<double> denEvtErrors_gammaN = getDoubleArr(PointsArr_gammaN,
                           [](Point& p) -> double { return TMath::Sqrt(p.denEvt);} );
  vector<double> effDValues_gammaN = getDoubleArr(PointsArr_gammaN,
                           [](Point& p) -> double { return p.effD;} );
  vector<double> effDErrors_gammaN = getDoubleArr(PointsArr_gammaN,
                           [](Point& p) -> double { return p.effDError;} );
  vector<double> numDValues_gammaN = getDoubleArr(PointsArr_gammaN,
                           [](Point& p) -> double { return p.numD;} );
  vector<double> numDErrors_gammaN = getDoubleArr(PointsArr_gammaN,
                           [](Point& p) -> double { return TMath::Sqrt(p.numD);} );
  vector<double> denDValues_gammaN = getDoubleArr(PointsArr_gammaN,
                           [](Point& p) -> double { return p.denD;} );
  vector<double> denDErrors_gammaN = getDoubleArr(PointsArr_gammaN,
                           [](Point& p) -> double { return TMath::Sqrt(p.denD);} );


  std::vector<Point> PointsArr_Ngamma = getPointArr(MinDzeroPT, MaxDzeroPT, false, inputPoints_Ngamma);

  vector<double> yValues_Ngamma = getDoubleArr(PointsArr_Ngamma, 
                           [](Point& p) -> double { return (p.ymax + p.ymin)/2.;} );
  vector<double> yErrors_Ngamma = getDoubleArr(PointsArr_Ngamma, 
                           [](Point& p) -> double { return (p.ymax - p.ymin)/2.;} );
  vector<double> correctedYieldValues_Ngamma = getDoubleArr(PointsArr_Ngamma, 
                           [](Point& p) -> double { return p.correctedYield;} );
  vector<double> correctedYieldErrors_Ngamma = getDoubleArr(PointsArr_Ngamma, 
                           [](Point& p) -> double { return p.correctedYieldError;} );
  vector<double> rawYieldValues_Ngamma = getDoubleArr(PointsArr_Ngamma,
                           [](Point& p) -> double { return p.rawYield;} );
  vector<double> rawYieldErrors_Ngamma = getDoubleArr(PointsArr_Ngamma,
                           [](Point& p) -> double { return p.rawYieldError;} );
  vector<double> effEvtValues_Ngamma = getDoubleArr(PointsArr_Ngamma,
                           [](Point& p) -> double { return p.effEvt;} );
  vector<double> effEvtErrors_Ngamma = getDoubleArr(PointsArr_Ngamma,
                           [](Point& p) -> double { return p.effEvtError;} );
  vector<double> numEvtValues_Ngamma = getDoubleArr(PointsArr_Ngamma,
                           [](Point& p) -> double { return p.numEvt;} );
  vector<double> numEvtErrors_Ngamma = getDoubleArr(PointsArr_Ngamma,
                           [](Point& p) -> double { return TMath::Sqrt(p.numEvt);} );
  vector<double> denEvtValues_Ngamma = getDoubleArr(PointsArr_Ngamma,
                           [](Point& p) -> double { return p.denEvt;} );
  vector<double> denEvtErrors_Ngamma = getDoubleArr(PointsArr_Ngamma,
                           [](Point& p) -> double { return TMath::Sqrt(p.denEvt);} );
  vector<double> effDValues_Ngamma = getDoubleArr(PointsArr_Ngamma,
                           [](Point& p) -> double { return p.effD;} );
  vector<double> effDErrors_Ngamma = getDoubleArr(PointsArr_Ngamma,
                           [](Point& p) -> double { return p.effDError;} );
  vector<double> numDValues_Ngamma = getDoubleArr(PointsArr_Ngamma,
                           [](Point& p) -> double { return p.numD;} );
  vector<double> numDErrors_Ngamma = getDoubleArr(PointsArr_Ngamma,
                           [](Point& p) -> double { return TMath::Sqrt(p.numD);} );
  vector<double> denDValues_Ngamma = getDoubleArr(PointsArr_Ngamma,
                           [](Point& p) -> double { return p.denD;} );
  vector<double> denDErrors_Ngamma = getDoubleArr(PointsArr_Ngamma,
                           [](Point& p) -> double { return TMath::Sqrt(p.denD);} );
  /////////////////////////////////
  // 2. Plot the cross section
  /////////////////////////////////
  auto plotGraph = [](const char* yAxisTitle, const char* xAxisTitle,
                 double yMin, double yMax,
                 const std::vector<double>& xValues_gammaN, const std::vector<double>& yValues_gammaN,
                 const std::vector<double>& xErrors_gammaN, const std::vector<double>& yErrors_gammaN,
                 const std::vector<double>& xValues_Ngamma, const std::vector<double>& yValues_Ngamma,
                 const std::vector<double>& xErrors_Ngamma, const std::vector<double>& yErrors_Ngamma,
                 const char* latexText, const char* plotprefix,
                 int nBinsX=100, double xMin=-2.2, double xMax=2.2)
  {
    TCanvas* c1 = new TCanvas("c1", "D0 Cross Section", 800, 800);
    c1->SetLeftMargin(0.13);
    c1->SetRightMargin(0.04);
    c1->SetBottomMargin(0.12);
    c1->SetTopMargin(0.08);

    TH1F* hFrame1 = new TH1F("hFrame1", " ", 100, -2.2, 2.2);
    hFrame1->GetYaxis()->SetTitle(yAxisTitle);
    hFrame1->GetXaxis()->SetTitle(xAxisTitle);
    hFrame1->SetStats(0);
    hFrame1->GetYaxis()->SetTitleOffset(1.5);
    hFrame1->GetYaxis()->SetRangeUser(yMin, yMax);
    hFrame1->Draw();

    TGraphErrors* gr_gammaN = new TGraphErrors(xValues_gammaN.size(), xValues_gammaN.data(), yValues_gammaN.data(),
                                           xErrors_gammaN.data(), yErrors_gammaN.data());
    gr_gammaN->SetMarkerStyle(20);
    gr_gammaN->SetMarkerSize(1.2);
    gr_gammaN->SetLineColor(kRed);
    gr_gammaN->SetMarkerColor(kRed);
    gr_gammaN->SetLineWidth(2);

    hFrame1->SetTitle("#gammaN");
    hFrame1->Draw();
    gr_gammaN->Draw("P E1 SAME");
    c1->Update();
    c1->SaveAs(Form("%sIsGammaN1_ANv2.pdf", plotprefix));


    TGraphErrors* gr_Ngamma = new TGraphErrors(xValues_Ngamma.size(), xValues_Ngamma.data(), yValues_Ngamma.data(),
                                           xErrors_Ngamma.data(), yErrors_Ngamma.data());
    gr_Ngamma->SetMarkerStyle(20);
    gr_Ngamma->SetMarkerSize(1.2);
    gr_Ngamma->SetLineColor(kRed);
    gr_Ngamma->SetMarkerColor(kRed);
    gr_Ngamma->SetLineWidth(2);

    hFrame1->SetTitle("N#gamma");
    hFrame1->Draw();
    gr_Ngamma->Draw("P E1 SAME");
    c1->Update();
    c1->SaveAs(Form("%sIsGammaN0_ANv2.pdf", plotprefix));

    TCanvas* c2 = new TCanvas("c2", "D0 Cross Section", 900, 800);
    c2->SetLeftMargin(0.18);
    c2->SetRightMargin(0.04);
    c2->SetBottomMargin(0.12);
    c2->SetTopMargin(0.08);

    TH1F* hFrame2 = new TH1F("hFrame2", " ", 100, -2.2, 2.2);
    hFrame2->GetYaxis()->SetTitle(Form("#frac{#bf{O}_{N #gamma (y #rightarrow -y)}}{#bf{O}_{#gamma N}}, #bf{O}=%s", yAxisTitle));
    hFrame2->GetXaxis()->SetTitle(xAxisTitle);
    hFrame2->SetStats(0);
    hFrame2->GetYaxis()->SetTitleOffset(1.8);
    hFrame2->GetYaxis()->SetRangeUser(0, 2.2);
    hFrame2->Draw();

    TLine* line = new TLine(-2.2, 1.0, 2.2, 1.0);
    line->SetLineColor(kGray);
    line->SetLineStyle(2);
    line->SetLineWidth(2);
    line->Draw("SAME");

    TGraphErrors* gr_ratio = GetRatioGraph(gr_Ngamma, gr_gammaN);
    // gr_gammaN->Print("all");
    // gr_Ngamma->Print("all");
    // gr_ratio->Print("all");
    gr_ratio->SetMarkerStyle(21);
    gr_ratio->SetMarkerSize(1.2);
    gr_ratio->SetLineColor(kBlue);
    gr_ratio->SetMarkerColor(kBlue);
    gr_ratio->SetLineWidth(2);

    gr_ratio->Draw("P E1 SAME");

    c2->Update();
    c2->SaveAs(Form("%sratio_ANv2.pdf", plotprefix));

    delete gr_gammaN; delete gr_Ngamma; delete gr_ratio;
    delete line;
    delete hFrame1; delete c1;
    delete hFrame2; delete c2;
  };

  const char* latexText = Form("%d < D_{p_{T}} < %d (GeV/#it{c})", (int) MinDzeroPT, (int) MaxDzeroPT);

  plotGraph("d^{2}#sigma/dydp_{T} (mb/GeV)", "D^{0} y",
            0, 3.5,
            yValues_gammaN, correctedYieldValues_gammaN, yErrors_gammaN, correctedYieldErrors_gammaN,
            yValues_Ngamma, correctedYieldValues_Ngamma, yErrors_Ngamma, correctedYieldErrors_Ngamma,
            latexText,
            Form("%s/correctedYieldValuesPlotNoSyst_pt%d-%d_",
                    PlotDir.c_str(),
                    (int) MinDzeroPT, (int) MaxDzeroPT));

  plotGraph("#varepsilon_{event}", "D^{0} y",
            0.95, 1.07,
            yValues_gammaN, effEvtValues_gammaN, yErrors_gammaN, effEvtErrors_gammaN,
            yValues_Ngamma, effEvtValues_Ngamma, yErrors_Ngamma, effEvtErrors_Ngamma,
            latexText,
            Form("%s/evtEff_pt%d-%d_",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT));


  plotGraph("Numerator N_{event}", "D^{0} y",
            0, (*std::max_element(numEvtValues_gammaN.begin(), numEvtValues_gammaN.end()))*1.3,
            yValues_gammaN, numEvtValues_gammaN, yErrors_gammaN, numEvtErrors_gammaN,
            yValues_Ngamma, numEvtValues_Ngamma, yErrors_Ngamma, numEvtErrors_Ngamma,
            latexText,
            Form("%s/evtNum_pt%d-%d_",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT));

  plotGraph("Denominator N_{event}", "D^{0} y",
            0, (*std::max_element(denEvtValues_gammaN.begin(), denEvtValues_gammaN.end()))*1.3,
            yValues_gammaN, denEvtValues_gammaN, yErrors_gammaN, denEvtErrors_gammaN,
            yValues_Ngamma, denEvtValues_Ngamma, yErrors_Ngamma, denEvtErrors_Ngamma,
            latexText,
            Form("%s/evtDen_pt%d-%d_",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT));

  plotGraph("#varepsilon_{D}", "D^{0} y",
            0, 0.2, //(*std::max_element(effDValues_gammaN.begin(), effDValues_gammaN.end()))*1.3,
            yValues_gammaN, effDValues_gammaN, yErrors_gammaN, effDErrors_gammaN,
            yValues_Ngamma, effDValues_Ngamma, yErrors_Ngamma, effDErrors_Ngamma,
            latexText,
            Form("%s/DEff_zoom_pt%d-pdf",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT));

  plotGraph("Numerator N_{D}", "D^{0} y",
            0, (*std::max_element(numDValues_gammaN.begin(), numDValues_gammaN.end()))*1.3,
            yValues_gammaN, numDValues_gammaN, yErrors_gammaN, numDErrors_gammaN,
            yValues_Ngamma, numDValues_Ngamma, yErrors_Ngamma, numDErrors_Ngamma,
            latexText,
            Form("%s/DNum_pt%d-%d_",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT));

  plotGraph("Denominator N_{D}", "D^{0} y",
            0, (*std::max_element(denDValues_gammaN.begin(), denDValues_gammaN.end()))*1.3,
            yValues_gammaN, denDValues_gammaN, yErrors_gammaN, denDErrors_gammaN,
            yValues_Ngamma, denDValues_Ngamma, yErrors_Ngamma, denDErrors_Ngamma,
            latexText,
            Form("%s/DDen_pt%d-%d_",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT));

  plotGraph("Raw yield", "D^{0} y",
            0, (*std::max_element(rawYieldValues_gammaN.begin(), rawYieldValues_gammaN.end()))*1.3,
            yValues_gammaN, rawYieldValues_gammaN, yErrors_gammaN, rawYieldErrors_gammaN,
            yValues_Ngamma, rawYieldValues_Ngamma, yErrors_Ngamma, rawYieldErrors_Ngamma,
            latexText,
            Form("%s/RawYield_pt%d-%df",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT));

  return 0;
}
