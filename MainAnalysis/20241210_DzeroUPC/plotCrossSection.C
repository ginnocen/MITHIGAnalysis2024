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

#include "plotCrossSection.h"

using namespace std;


int main(int argc, char *argv[])
{
  CommandLine CL(argc, argv);
  string         PlotDir       = CL.Get    ("PlotDir", "");       // subdirectory under plot/
  float MinDzeroPT = CL.GetDouble("MinDzeroPT", 2);  // Minimum Dzero transverse momentum threshold for Dzero selection.
  float MaxDzeroPT = CL.GetDouble("MaxDzeroPT", 5);  // Maximum Dzero transverse momentum threshold for Dzero selection.
  bool IsGammaN = CL.GetBool("IsGammaN", true);      // GammaN analysis (or NGamma)
  bool UseMaxFitUncert = CL.GetBool("UseMaxFitUncert", false);

  vector<string> inputPoints      = CL.GetStringVector("InputPoints",    ""); // Input corrected yields md files

  double         wSystLumi        = CL.GetDouble("wSystLumi", 0.05);             // Include luminosity systematics (relative uncertainty)
  double         wSystTrk         = CL.GetDouble("wSystTrk", 0.046);             // Include tracking systematics (relative uncertainty)
  double         wSystBR          = CL.GetDouble("wSystBR", 0.0076);             // Include branching ratio systematics (relative uncertainty)
  bool           wSystEvtSel      = CL.GetBool("wSystEvtSel", true);             // Include event selection systematics
  vector<string> wSystRapGapSel   = CL.GetStringVector("wSystRapGapSel", "systRapGapLoose,systRapGapTight");  // Include rapidity gap selection systematics -- the uncertainty is the average variation of the two
  string         wSystDsvpv       = CL.Get    ("wSystDsvpv", "systDsvpv");       // Include D selection systematics (with the variation on svpv significance). The input is the result directory name
  string         wSystDtrkPt      = CL.Get    ("wSystDtrkPt", "systDtrkPt");     // Include D selection systematics (with the variation on trkPt). The input is the result directory name
  string         wSystDalpha      = CL.Get    ("wSystDalpha", "systDalpha");     // Include D selection systematics (with the variation on alpha). The input is the result directory name
  string         wSystDchi2cl     = CL.Get    ("wSystDchi2cl", "systDchi2cl");   // Include D selection systematics (with the variation on chi2cl). The input is the result directory name
  string         wSystFitSigMean  = CL.Get    ("wSystFitSigMean", "MassFit_systFitSigMean"); // Include fit systematics of different signal modeling. The input is the fit directory name
  string         wSystFitSigAlpha = CL.Get    ("wSystFitSigAlpha", "MassFit_systFitSigAlpha"); // Include fit systematics of different signal modeling. The input is the fit directory name
  string         wSystFitComb     = CL.Get    ("wSystFitComb", "MassFit_systComb");// Include fit systematics of different combinatorics modeling. The input is the fit directory name
  string         wSystFitPkBg     = CL.Get    ("wSystFitPkBg", "MassFit_systPkBg");// Include fit systematics of different background KK and pipi peak modeling. The input is the fit directory name
  string         wSystMassWindow  = CL.Get    ("wSystMassWindow", "MassFit_systMassWindow");
  string         nominalSampleRST = CL.Get    ("nominalSampleRST", "fullAnalysis");// Nominal sample directory name
  string         nominalFitRST    = CL.Get    ("nominalFitRST", "MassFit");        // Nominal fit directory name

  /////////////////////////////////
  // 0. Extract the points from the vector of .md
  /////////////////////////////////

  // nominal central values
  const int nPoints = inputPoints.size();
  std::vector<Point> PointsArr = getPointArr(MinDzeroPT, MaxDzeroPT, IsGammaN, inputPoints);

  vector<double> yValues = getDoubleArr(PointsArr, 
                           [](Point& p) -> double { return (p.ymax + p.ymin)/2.;} );
  vector<double> yErrors = getDoubleArr(PointsArr, 
                           [](Point& p) -> double { return (p.ymax - p.ymin)/2.;} );
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

  vector<double> RFBXbins;
  vector<double> RFBXbinErrors;
  vector<double> RFBValues;
  vector<double> RFBErrors;
  for (int i = 0; i < nPoints; ++i)
  {
    // only compute for the forward region: +y for gammaN, -y for Ngamma
    if ( IsGammaN && yValues[i]<0) continue;
    if (!IsGammaN && yValues[i]>0) continue;

    auto reflected_it = std::find(yValues.begin(), yValues.end(), -yValues[i]);

    if (reflected_it!=yValues.end())
    {
      int reflected_idx = std::distance(yValues.begin(), reflected_it);
      RFBXbins.push_back(yValues[i]);
      RFBXbinErrors.push_back(yErrors[i]);
      double RFB = correctedYieldValues[i]/correctedYieldValues[reflected_idx];
      RFBValues.push_back(RFB);
      RFBErrors.push_back(
        RFB * TMath::Sqrt( TMath::Power(correctedYieldErrors[i]/correctedYieldValues[i], 2) +
                           TMath::Power(correctedYieldErrors[reflected_idx]/correctedYieldValues[reflected_idx], 2) )
        );
    }
  }

  /////////////////////////////////
  // 1. Calculate the systematics uncertainties for each componenet
  /////////////////////////////////
  // rapidity gap
  vector<vector<double> > systRapGapCorrectedYieldValues;
  for (auto rapGapConfig: wSystRapGapSel)
  {
    systRapGapCorrectedYieldValues.push_back( 
      getAltCorrectedYieldArr(inputPoints, 
                    nominalSampleRST, rapGapConfig,
                    MinDzeroPT, MaxDzeroPT, IsGammaN) 
    );
  }

  // Dsvpv
  vector<double> systDsvpvCorrectedYieldValues = getAltCorrectedYieldArr(inputPoints, 
                    nominalSampleRST, wSystDsvpv,
                    MinDzeroPT, MaxDzeroPT, IsGammaN);

  // DtrkPt
  vector<double> systDtrkPtCorrectedYieldValues = getAltCorrectedYieldArr(inputPoints, 
                    nominalSampleRST, wSystDtrkPt,
                    MinDzeroPT, MaxDzeroPT, IsGammaN);

  // Dalpha
  vector<double> systDalphaCorrectedYieldValues = getAltCorrectedYieldArr(inputPoints,
                    nominalSampleRST, wSystDalpha,
                    MinDzeroPT, MaxDzeroPT, IsGammaN);

  // Dchi2cl
  vector<double> systDchi2clCorrectedYieldValues = getAltCorrectedYieldArr(inputPoints,
                    nominalSampleRST, wSystDchi2cl,
                    MinDzeroPT, MaxDzeroPT, IsGammaN);

  // FitSigMean
  vector<double> systFitSigMeanCorrectedYieldValues = getAltCorrectedYieldArr(inputPoints,
                    nominalFitRST, wSystFitSigMean,
                    MinDzeroPT, MaxDzeroPT, IsGammaN);
  
  // FitSigAlpha
  vector<double> systFitSigAlphaCorrectedYieldValues = getAltCorrectedYieldArr(inputPoints,
                    nominalFitRST, wSystFitSigAlpha,
                    MinDzeroPT, MaxDzeroPT, IsGammaN);

  // FitComb
  vector<double> systFitCombCorrectedYieldValues = getAltCorrectedYieldArr(inputPoints, 
                    nominalFitRST, wSystFitComb,
                    MinDzeroPT, MaxDzeroPT, IsGammaN);
  
  // FitPkBg
  vector<double> systFitPkBgCorrectedYieldValues = getAltCorrectedYieldArr(inputPoints,
                    nominalFitRST, wSystFitPkBg,
                    MinDzeroPT, MaxDzeroPT, IsGammaN);
  // MassWindow
  vector<double> systMassWindowCorrectedYieldValues = getAltCorrectedYieldArr(inputPoints,
                    nominalFitRST, wSystMassWindow,
                    MinDzeroPT, MaxDzeroPT, IsGammaN);

  vector<double> systEvtSelUncert(nPoints);
  vector<double> systRapGapUncert(nPoints);
  vector<double> systDsvpvUncert(nPoints);
  vector<double> systDtrkPtUncert(nPoints);
  vector<double> systDalphaUncert(nPoints);
  vector<double> systDchi2clUncert(nPoints);
  vector<double> systFitUncert(nPoints);
  vector<double> systFitSigMeanUncert(nPoints);
  vector<double> systFitSigAlphaUncert(nPoints);
  vector<double> systFitCombUncert(nPoints);
  vector<double> systFitPkBgUncert(nPoints);
  vector<double> systMassWindowUncert(nPoints);
  for (int i = 0; i < nPoints; ++i)
  {
    systEvtSelUncert[i]   = (wSystEvtSel)? effEvtErrors[i]/effEvtValues[i]*correctedYieldValues[i]: 0;

    for (auto altValues: systRapGapCorrectedYieldValues)
    {
      float thisUncert = TMath::Abs(altValues[i] - correctedYieldValues[i]);
      if (thisUncert > systRapGapUncert[i]) systRapGapUncert[i] = thisUncert;
    }

    systDsvpvUncert[i]    = TMath::Abs(systDsvpvCorrectedYieldValues[i] - correctedYieldValues[i]);
    systDtrkPtUncert[i]   = TMath::Abs(systDtrkPtCorrectedYieldValues[i] - correctedYieldValues[i]);
    systDalphaUncert[i]   = TMath::Abs(systDalphaCorrectedYieldValues[i] - correctedYieldValues[i]);
    systDchi2clUncert[i]  = TMath::Abs(systDchi2clCorrectedYieldValues[i] - correctedYieldValues[i]);
    systFitSigMeanUncert[i]  = TMath::Abs(systFitSigMeanCorrectedYieldValues[i] - correctedYieldValues[i]);
    systFitSigAlphaUncert[i] = TMath::Abs(systFitSigAlphaCorrectedYieldValues[i] - correctedYieldValues[i]);
    systFitCombUncert[i]  = TMath::Abs(systFitCombCorrectedYieldValues[i] - correctedYieldValues[i]);
    systFitPkBgUncert[i]  = TMath::Abs(systFitPkBgCorrectedYieldValues[i] - correctedYieldValues[i]);
    systMassWindowUncert[i]  = TMath::Abs(systMassWindowCorrectedYieldValues[i] - correctedYieldValues[i]);
    systFitUncert[i]      = TMath::Sqrt(
      systFitSigMeanUncert[i] * systFitSigMeanUncert[i] +
      systFitSigAlphaUncert[i] * systFitSigAlphaUncert[i] + 
      systFitCombUncert[i] * systFitCombUncert[i] + 
      systFitPkBgUncert[i] * systFitPkBgUncert[i] +
      systMassWindowUncert[i] * systMassWindowUncert[i]
    );
    if (UseMaxFitUncert) systFitUncert[i] = max({
      systFitSigMeanUncert[i], systFitSigAlphaUncert[i],
      systFitCombUncert[i], systFitPkBgUncert[i], systMassWindowUncert[i]});
  }

  printArr(correctedYieldValues, ", ", "correctedYieldValues: ");
  printArr(systRapGapCorrectedYieldValues[0], ", ", "systRapGapLoose: ");
  printArr(systRapGapCorrectedYieldValues[1], ", ", "systRapGapTight: ");
  printRatioArr(systEvtSelUncert, correctedYieldValues,   "  ", " EvtSel       ", "  ");
  printRatioArr(systRapGapUncert, correctedYieldValues,   "  ", " RapGap       ", "  ");
  printRatioArr(systDsvpvUncert, correctedYieldValues,    "  ", " Dsvpv        ", "  ");
  printRatioArr(systDtrkPtUncert, correctedYieldValues,   "  ", " DtrkPt       ", "  ");
  printRatioArr(systDalphaUncert, correctedYieldValues,   "  ", " Dalpha       ", "  ");
  printRatioArr(systDchi2clUncert, correctedYieldValues,  "  ", " Dchi2cl      ", "  ");
  printRatioArr(systFitUncert, correctedYieldValues,      "  ", " Fit(Total)   ", "  ");
  vector<vector<double>> systList = {
    systEvtSelUncert,
    systRapGapUncert,
    systDsvpvUncert,
    systDtrkPtUncert,
    systDalphaUncert,
    systDchi2clUncert,
    systFitUncert
  };
  vector<double> systTotUncert(nPoints);
  for (int i = 0; i < nPoints; ++i)
  {
    double systLumiUncert = wSystLumi * correctedYieldValues[i];
    double systTrkUncert  = wSystTrk * correctedYieldValues[i];
    double systBRUncert   = wSystBR * correctedYieldValues[i];
    double systPromptFrac = 0.05 * correctedYieldValues[i]; // [TODO] replaced the rel. syst. to the new study
    systTotUncert[i] = (
      systLumiUncert * systLumiUncert +
      systTrkUncert * systTrkUncert +
      systBRUncert * systBRUncert +
      systPromptFrac * systPromptFrac
    );
    for (int j = 0; j < systList.size(); ++j) {
      systTotUncert[i] += systList[j][i] * systList[j][i];
    }
    systTotUncert[i] = TMath::Sqrt(systTotUncert[i]);
  }
  printRatioArr(systTotUncert, correctedYieldValues,          "  ", " Total        ", "  ");
  printRatioArr(systFitSigMeanUncert, correctedYieldValues,   "  ", " Fit:SigMean  ", "  ");
  printRatioArr(systFitSigAlphaUncert, correctedYieldValues,  "  ", " Fit:SigAlpha ", "  ");
  printRatioArr(systFitCombUncert, correctedYieldValues,      "  ", " Fit:Comb     ", "  ");
  printRatioArr(systFitPkBgUncert, correctedYieldValues,      "  ", " Fit:PkBg     ", "  ");
  printRatioArr(systMassWindowUncert, correctedYieldValues,   "  ", " Fit:MassWin  ", "  ");
  printArr(systTotUncert, ", ", "systTotUncert: ");

  /////////////////////////////////
  // 2. Plot the cross section
  /////////////////////////////////
  // Create a canvas
  TCanvas* c1 = new TCanvas("c1", "D0 Cross Section", 800, 800);
  c1->SetLeftMargin(0.13);
  c1->SetRightMargin(0.04);
  c1->SetBottomMargin(0.12);
  c1->SetTopMargin(0.08);

  TH1F* hFrame = new TH1F("hFrame", " ", 100, -2.2, 2.2);
  hFrame->GetYaxis()->SetTitle("d^{2}#sigma/dydp_{T} (mb/GeV)");
  hFrame->GetXaxis()->SetTitle("D^{0} y");
  hFrame->SetStats(0);
  hFrame->GetYaxis()->SetTitleOffset(1.5);
  hFrame->GetYaxis()->SetRangeUser(0, 3.5);
  hFrame->Draw();

  TGraphErrors* gr = new TGraphErrors(nPoints, yValues.data(), correctedYieldValues.data(), yErrors.data(), correctedYieldErrors.data());
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(1.2);
  gr->SetLineColor(kRed);
  gr->SetMarkerColor(kRed);
  gr->SetLineWidth(2);

  gr->Draw("P E1 SAME");

  // Create the uncertainty band (systematic)
  TGraphErrors* gr_uncert = new TGraphErrors(nPoints, yValues.data(), correctedYieldValues.data(), yErrors.data(), correctedYieldErrors.data());
  for (int i = 0; i < nPoints; ++i) {
      gr_uncert->SetPoint(i, yValues[i], correctedYieldValues[i]); // Set the upper bound of the uncertainty
      gr_uncert->SetPointError(i, yErrors[i], systTotUncert[i]); // Error is the systematic uncertainty
  }
  gr_uncert->SetFillColorAlpha(kRed,0.3); // Set color for uncertainty band (you can adjust it)
  gr_uncert->Draw("2 SAME"); // Draw the uncertainty band

  /////////////////////////////////
  // [TODO] Need to add a flag to decide whether to overlay the HIN-24-003 result
  /////////////////////////////////
  TGraphErrors* gr_ref = (IsGammaN)? &gr_ref_gammaN: &gr_ref_Ngamma;
  gr_ref->SetMarkerStyle(20);
  gr_ref->SetMarkerSize(1.2);
  gr_ref->SetLineColor(kBlack);
  gr_ref->SetMarkerColor(kBlack);
  gr_ref->SetLineWidth(2);

  gr_ref->Draw("P E1 SAME");

  // Create the uncertainty band (systematic)
  TGraphErrors* gr_uncert_ref = (IsGammaN)? &gr_uncert_ref_gammaN: &gr_uncert_ref_Ngamma;
  gr_uncert_ref->SetFillColorAlpha(kBlack,0.3); // Set color for uncertainty band (you can adjust it)
  gr_uncert_ref->Draw("2 SAME"); // Draw the uncertainty band


  TLegend* leg = new TLegend(0.2, 0.78, 0.55, 0.90);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->AddEntry(gr, "HIN-25-002", "P");
  leg->AddEntry(gr_ref, "HIN-24-003", "P");
  leg->Draw();

  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.035);
  latex.SetTextFont(42);
  // latex.DrawLatex(0.15, 0.92, "CMS #it{Preliminary} 1.38 nb^{-1} (5.36 TeV PbPb)");
  // latex.DrawLatex(0.15, 0.86, "UPCs, ZDC Xn0n w/ gap");
  // latex.DrawLatex(0.15, 0.82, "Global uncert. #pm 5.05%");
  latex.DrawLatex(0.6, 0.82, Form("%d < D_{p_{T}} < %d (GeV)", (int) MinDzeroPT, (int) MaxDzeroPT));

  c1->Update();
  c1->SaveAs(Form("%s/correctedYieldValuesPlot_pt%d-%d_IsGammaN%o.pdf",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT,
                  IsGammaN));
  delete gr;
  delete hFrame;

  /////////////////////////////////
  // 3. Plot RFB
  /////////////////////////////////
  TH1F* hFrame2 = new TH1F("hFrame2", " ", 100, (IsGammaN)? -0.2: -2.2, 
                                                (IsGammaN)?  2.2:  0.2);
  hFrame2->GetYaxis()->SetTitle("RFB");
  hFrame2->GetXaxis()->SetTitle(Form("%sD^{0} y", (IsGammaN)? "+": "-"));
  hFrame2->SetStats(0);
  hFrame2->GetYaxis()->SetTitleOffset(1.5);
  hFrame2->GetYaxis()->SetRangeUser(0, 0.8);
  hFrame2->Draw();

  TGraphErrors* gr2 = new TGraphErrors(RFBXbins.size(), RFBXbins.data(), RFBValues.data(), RFBXbinErrors.data(), RFBErrors.data());
  gr2->Print("all");
  gr2->SetMarkerStyle(20);
  gr2->SetMarkerSize(1.2);
  gr2->SetLineColor(kBlack);
  gr2->SetMarkerColor(kBlack);
  gr2->SetLineWidth(2);
  gr2->Draw("P E1 SAME");

  latex.DrawLatex(0.6, 0.82, Form("%d < D_{p_{T}} < %d (GeV)", (int) MinDzeroPT, (int) MaxDzeroPT));

  c1->Update();
  c1->SaveAs(Form("%s/RFBPlot_pt%d-%d_IsGammaN%o.pdf",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT,
                  IsGammaN));
  delete gr2;
  delete hFrame2;

  /////////////////////////////////
  // 4. Plot other relevant plots
  /////////////////////////////////
  auto plotGraph = [](const char* yAxisTitle, const char* xAxisTitle,
                 double yMin, double yMax,
                 const std::vector<double>& xValues, const std::vector<double>& yValues,
                 const std::vector<double>& xErrors, const std::vector<double>& yErrors,
                 const char* latexText, const char* plotname,
                 int nBinsX=100, double xMin=-2.2, double xMax=2.2)
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
    hFrame->GetYaxis()->SetRangeUser(yMin, yMax);
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
    latex.DrawLatex(0.6, 0.82, latexText);

    // Update and save the canvas
    canvas->Update();
    canvas->SaveAs(plotname);

    // Clean up
    delete graph;
    delete hFrame;
    delete canvas;
  };

  const char* latexText = Form("%d < D_{p_{T}} < %d (GeV/#it{c})", (int) MinDzeroPT, (int) MaxDzeroPT);

  plotGraph("#varepsilon_{event}", "D^{0} y",
            0.95, 1.07,
            yValues, effEvtValues, yErrors, effEvtErrors,
            latexText,
            Form("%s/evtEff_pt%d-%d_IsGammaN%o.pdf",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT,
                  IsGammaN));

  plotGraph("Numerator N_{event}", "D^{0} y",
            0, (*std::max_element(numEvtValues.begin(), numEvtValues.end()))*1.3,
            yValues, numEvtValues, yErrors, numEvtErrors,
            latexText,
            Form("%s/evtNum_pt%d-%d_IsGammaN%o.pdf",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT,
                  IsGammaN));

  plotGraph("Denominator N_{event}", "D^{0} y",
            0, (*std::max_element(denEvtValues.begin(), denEvtValues.end()))*1.3,
            yValues, denEvtValues, yErrors, denEvtErrors,
            latexText,
            Form("%s/evtDen_pt%d-%d_IsGammaN%o.pdf",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT,
                  IsGammaN));

  plotGraph("#varepsilon_{D}", "D^{0} y",
            0, 1.05,
            yValues, effDValues, yErrors, effDErrors,
            latexText,
            Form("%s/DEff_pt%d-%d_IsGammaN%o.pdf",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT,
                  IsGammaN));

  plotGraph("#varepsilon_{D}", "D^{0} y",
            0, 0.2, //(*std::max_element(effDValues.begin(), effDValues.end()))*1.3,
            yValues, effDValues, yErrors, effDErrors,
            latexText,
            Form("%s/DEff_zoom_pt%d-%d_IsGammaN%o.pdf",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT,
                  IsGammaN));

  plotGraph("Numerator N_{D}", "D^{0} y",
            0, (*std::max_element(numDValues.begin(), numDValues.end()))*1.3,
            yValues, numDValues, yErrors, numDErrors,
            latexText,
            Form("%s/DNum_pt%d-%d_IsGammaN%o.pdf",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT,
                  IsGammaN));

  plotGraph("Denominator N_{D}", "D^{0} y",
            0, (*std::max_element(denDValues.begin(), denDValues.end()))*1.3,
            yValues, denDValues, yErrors, denDErrors,
            latexText,
            Form("%s/DDen_pt%d-%d_IsGammaN%o.pdf",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT,
                  IsGammaN));

  plotGraph("Raw yield", "D^{0} y",
            0, (*std::max_element(rawYieldValues.begin(), rawYieldValues.end()))*1.3,
            yValues, rawYieldValues, yErrors, rawYieldErrors,
            latexText,
            Form("%s/RawYield_pt%d-%d_IsGammaN%o.pdf",
                  PlotDir.c_str(),
                  (int) MinDzeroPT, (int) MaxDzeroPT,
                  IsGammaN));


  return 0;
}
