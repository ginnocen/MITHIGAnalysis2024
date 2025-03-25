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

    xVals.push_back(x1);
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
            "fullAnalysis/pt2-5_y-2--1_IsGammaN1/MassFit/correctedYields.md",
            "fullAnalysis/pt2-5_y-1-0_IsGammaN1/MassFit/correctedYields.md",
            "fullAnalysis/pt2-5_y0-1_IsGammaN1/MassFit/correctedYields.md",
            "fullAnalysis/pt2-5_y1-2_IsGammaN1/MassFit/correctedYields.md"};
  vector<string> inputPoints_Ngamma      = {
            "fullAnalysis/pt2-5_y1-2_IsGammaN0/MassFit/correctedYields.md",
            "fullAnalysis/pt2-5_y0-1_IsGammaN0/MassFit/correctedYields.md",
            "fullAnalysis/pt2-5_y-1-0_IsGammaN0/MassFit/correctedYields.md",
            "fullAnalysis/pt2-5_y-2--1_IsGammaN0/MassFit/correctedYields.md"};

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

  std::vector<Point> PointsArr_Ngamma = getPointArr(MinDzeroPT, MaxDzeroPT, false, inputPoints_Ngamma);

  vector<double> yValues_Ngamma = getDoubleArr(PointsArr_Ngamma, 
                           [](Point& p) -> double { return (p.ymax + p.ymin)/2.;} );
  vector<double> yErrors_Ngamma = getDoubleArr(PointsArr_Ngamma, 
                           [](Point& p) -> double { return (p.ymax - p.ymin)/2.;} );
  vector<double> correctedYieldValues_Ngamma = getDoubleArr(PointsArr_Ngamma, 
                           [](Point& p) -> double { return p.correctedYield;} );
  vector<double> correctedYieldErrors_Ngamma = getDoubleArr(PointsArr_Ngamma, 
                           [](Point& p) -> double { return p.correctedYieldError;} );

  /////////////////////////////////
  // 2. Plot the cross section
  /////////////////////////////////
  TCanvas* c1 = new TCanvas("c1", "D0 Cross Section", 800, 800);
  c1->SetLeftMargin(0.13);
  c1->SetRightMargin(0.04);
  c1->SetBottomMargin(0.12);
  c1->SetTopMargin(0.08);

  TH1F* hFrame1 = new TH1F("hFrame1", " ", 100, -2.2, 2.2);
  hFrame1->GetYaxis()->SetTitle("d^{2}#sigma/dydp_{T} (mb/GeV)");
  hFrame1->GetXaxis()->SetTitle("y_{D^{0}}");
  hFrame1->SetStats(0);
  hFrame1->GetYaxis()->SetTitleOffset(1.5);
  hFrame1->GetYaxis()->SetRangeUser(0, 3.5);

  TGraphErrors* gr_gammaN = new TGraphErrors(nPoints, yValues_gammaN.data(), correctedYieldValues_gammaN.data(), yErrors_gammaN.data(), correctedYieldErrors_gammaN.data());
  gr_gammaN->SetMarkerStyle(20);
  gr_gammaN->SetMarkerSize(1.2);
  gr_gammaN->SetLineColor(kRed);
  gr_gammaN->SetMarkerColor(kRed);
  gr_gammaN->SetLineWidth(2);

  hFrame1->SetTitle("#gammaN");
  hFrame1->Draw();
  gr_gammaN->Draw("P E1 SAME");
  c1->Update();
  c1->SaveAs(Form("correctedYieldValuesPlotNoSyst_pt%d-%d_IsGammaN1_ANv2.pdf",
                  (int) MinDzeroPT, (int) MaxDzeroPT));


  TGraphErrors* gr_Ngamma = new TGraphErrors(nPoints, yValues_Ngamma.data(), correctedYieldValues_Ngamma.data(), yErrors_Ngamma.data(), correctedYieldErrors_Ngamma.data());
  gr_Ngamma->SetMarkerStyle(20);
  gr_Ngamma->SetMarkerSize(1.2);
  gr_Ngamma->SetLineColor(kRed);
  gr_Ngamma->SetMarkerColor(kRed);
  gr_Ngamma->SetLineWidth(2);

  hFrame1->SetTitle("N#gamma");
  hFrame1->Draw();
  gr_Ngamma->Draw("P E1 SAME");
  c1->Update();
  c1->SaveAs(Form("correctedYieldValuesPlotNoSyst_pt%d-%d_IsGammaN0_ANv2.pdf",
                  (int) MinDzeroPT, (int) MaxDzeroPT));

  TCanvas* c2 = new TCanvas("c2", "D0 Cross Section", 900, 800);
  c2->SetLeftMargin(0.18);
  c2->SetRightMargin(0.04);
  c2->SetBottomMargin(0.12);
  c2->SetTopMargin(0.08);

  TH1F* hFrame2 = new TH1F("hFrame2", " ", 100, -2.2, 2.2);
  hFrame2->GetYaxis()->SetTitle("#frac{(d^{2}#sigma/dydp_{T})_{N #gamma (y #rightarrow -y)}}{(d^{2}#sigma/dydp_{T})_{#gamma N}}");
  hFrame2->GetXaxis()->SetTitle("y_{D^{0}}");
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
  gr_ratio->SetMarkerStyle(21);
  gr_ratio->SetMarkerSize(1.2);
  gr_ratio->SetLineColor(kBlue);
  gr_ratio->SetMarkerColor(kBlue);
  gr_ratio->SetLineWidth(2);

  gr_ratio->Draw("P E1 SAME");

  c2->Update();
  c2->SaveAs(Form("correctedYieldValuesPlotRatio_pt%d-%d_ANv2.pdf",
                  (int) MinDzeroPT, (int) MaxDzeroPT));

  delete gr_gammaN; delete gr_Ngamma; delete gr_ratio;
  delete line;
  delete hFrame1; delete c1;
  delete hFrame2; delete c2;

  return 0;
}
