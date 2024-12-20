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
#include <regex>
#include <algorithm> // For std::find

using namespace std;

struct Point {
  double ptmin, ptmax, ymin, ymax;
  double effEvt, effD, rawYield, correctedYield;
  double effEvtError, effDError, rawYieldError, correctedYieldError;
  double numEvt, denEvt, numD, denD;

  void print() const {
    std::cout << "ptmin: " << ptmin << ", ptmax: " << ptmax
              << ", ymin: " << ymin << ", ymax: " << ymax << std::endl;
    std::cout << "effEvt: " << effEvt << " +/- " << effEvtError
              << ", effD: " << effD << " +/- " << effDError
              << ", raw yield: " << rawYield << " +/- " << rawYieldError
              << ", corrected yield: " << correctedYield << " +/- " << correctedYieldError << std::endl;
    std::cout << "Num #(Evt): " << numEvt << ", Den #(Evt): " << denEvt
              << ", Num #(D): " << numD << ", Den #(D): " << denD << std::endl;
  }
};


// Function to trim leading and trailing spaces
std::string trim(const std::string& str) {
  size_t first = str.find_first_not_of(' ');
  if (first == std::string::npos) return ""; // No content

  size_t last = str.find_last_not_of(' ');
  return str.substr(first, (last - first + 1));
}

// Function to extract number from string with '+/-' sign
// Function to extract number and error from a string with '+/-' sign
void parseNumberAndError(const std::string& str, double &value, double &error) {
  std::regex rgx("([+-]?\\d*\\.?\\d+)\\s*\\+/-\\s*([+-]?\\d*\\.?\\d+)");
  std::smatch match;
  if (std::regex_search(str, match, rgx)) {
    value = std::stod(match.str(1));   // Extract the main value
    error = std::stod(match.str(2));   // Extract the error value
  } else {
    value = 0.0;
    error = 0.0;
  }
}

void getPoint(string& mdInput, Point& p) {
  printf("read %s >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", mdInput.c_str());
  std::ifstream inputFile(mdInput.c_str());
  std::string line;

  // Skip header lines
  std::getline(inputFile, line);
  std::getline(inputFile, line);

  // Extract ptmin, ptmax, ymin, ymax
  std::getline(inputFile, line);
  // printf("read %s\n", line.c_str());
  std::stringstream ss1(line);
  std::string value;
  
  // Read the values separated by '|'
  std::getline(ss1, value, '|');
  std::getline(ss1, value, '|'); p.ptmin = std::stod(trim(value));
  std::getline(ss1, value, '|'); p.ptmax = std::stod(trim(value));
  std::getline(ss1, value, '|'); p.ymin = std::stod(trim(value));
  std::getline(ss1, value, '|'); p.ymax = std::stod(trim(value));


  // Extract effEvt, effD, rawYield, correctedYield
  std::getline(inputFile, line);
  std::getline(inputFile, line);
  // printf("read %s\n", line.c_str());
  std::stringstream ss2(line);
  std::getline(ss2, value, '|');
  std::getline(ss2, value, '|'); parseNumberAndError(trim(value), p.effEvt, p.effEvtError);
  std::getline(ss2, value, '|'); parseNumberAndError(trim(value), p.effD, p.effDError);
  std::getline(ss2, value, '|'); parseNumberAndError(trim(value), p.rawYield, p.rawYieldError);
  std::getline(ss2, value, '|'); parseNumberAndError(trim(value), p.correctedYield, p.correctedYieldError);

  // Extract numEvt, denEvt, numD, denD
  std::getline(inputFile, line);
  std::getline(inputFile, line);
  // printf("read %s\n", line.c_str());
  std::stringstream ss3(line);
  
  // Read the values separated by '|'
  std::getline(ss3, value, '|');
  std::getline(ss3, value, '|'); p.numEvt = std::stod(trim(value));
  std::getline(ss3, value, '|'); p.denEvt = std::stod(trim(value));
  std::getline(ss3, value, '|'); p.numD = std::stod(trim(value));
  std::getline(ss3, value, '|'); p.denD = std::stod(trim(value));

  // Output extracted data
  p.print();
}

/////////////////////////////////
// [TODO] this is the global variables for the archived results
// I think I will want to put them into a .h file (also the above utility functions)
/////////////////////////////////
double yValues_gammaN[1]       = {0};
double crossSection_gammaN[1]  = {1.07585};
double yErrors_gammaN[1]       = {1};
double crossErrors_gammaN[1]   = {0.10225};
TGraphErrors gr_ref_gammaN = TGraphErrors(1, 
                             yValues_gammaN, crossSection_gammaN,
                             yErrors_gammaN, crossErrors_gammaN);

double yValues_Ngamma[1]       = {0};
double crossSection_Ngamma[1]  = {1.02555};
double yErrors_Ngamma[1]       = {1};
double crossErrors_Ngamma[1]   = {0.100667};
TGraphErrors gr_ref_Ngamma = TGraphErrors(1, 
                             yValues_Ngamma, crossSection_Ngamma,
                             yErrors_Ngamma, crossErrors_Ngamma);


void plotCrossSection(vector<string>& inputPoints) {
  /////////////////////////////////
  // [TODO] Put the part of reading inputs in a function
  /////////////////////////////////
  /////////////////////////////////
  // 0. Extract the points from the vector of .md
  /////////////////////////////////
  const int nPoints = inputPoints.size();
  std::vector<Point> PointsArr(nPoints);

  /////////////////////////////////
  // [TODO] Change all the containers into vector
  /////////////////////////////////
  double fix_ptmin;
  double fix_ptmax;
  bool fix_IsGammaN;
  vector<double> yValues(nPoints);
  vector<double> yErrors(nPoints);
  vector<double> crossSection(nPoints);
  vector<double> crossErrors(nPoints);
  for (int i = 0; i < nPoints; ++i)
  {
    getPoint(inputPoints[i], PointsArr[i]);
    
    Point& p(PointsArr[i]);
    if (i==0)
    {
      fix_ptmin = p.ptmin;
      fix_ptmax = p.ptmax;
      fix_IsGammaN = inputPoints[i].find("IsGammaN1") != string::npos;
    } else {
      if (fix_ptmin != p.ptmin || 
          fix_ptmax != p.ptmax ||
          fix_IsGammaN != (inputPoints[i].find("IsGammaN1") != string::npos)) {
        printf("[Error] Unexpected usage: Concatenating different pt bins or inconsistent gammaN/Ngamma results together! (%d,%d,%o),(%d,%d,%o) Exiting ...\n",
          p.ptmin, p.ptmax, (inputPoints[i].find("IsGammaN1") != string::npos),
          fix_ptmin, fix_ptmax, fix_IsGammaN);
        exit(1);
      }
    }

    yValues[i] = (p.ymax + p.ymin)/2.;
    yErrors[i] = (p.ymax - p.ymin)/2.;
    crossSection[i] = p.correctedYield;
    crossErrors[i]  = p.correctedYieldError;
  }
  
  vector<double> RFBXbins;
  vector<double> RFBXbinErrors;
  vector<double> RFBValues;
  vector<double> RFBErrors;
  for (int i = 0; i < nPoints; ++i)
  {
    // only compute for the forward region: +y for gammaN, -y for Ngamma
    if ( fix_IsGammaN && yValues[i]<0) continue;
    if (!fix_IsGammaN && yValues[i]>0) continue;

    auto reflected_it = std::find(yValues.begin(), yValues.end(), -yValues[i]);

    if (reflected_it!=yValues.end())
    {
      int reflected_idx = std::distance(yValues.begin(), reflected_it);
      RFBXbins.push_back(yValues[i]);
      RFBXbinErrors.push_back(yErrors[i]);
      double RFB = crossSection[i]/crossSection[reflected_idx];
      RFBValues.push_back(RFB);
      RFBErrors.push_back(
        RFB * TMath::Sqrt( TMath::Power(crossErrors[i]/crossSection[i], 2) +
                           TMath::Power(crossErrors[reflected_idx]/crossSection[reflected_idx], 2) )
        );
    }
  }

  /////////////////////////////////
  // 1. Plot the cross section
  /////////////////////////////////
  // Create a canvas
  TCanvas* c1 = new TCanvas("c1", "D0 Cross Section", 800, 800);
  c1->SetLeftMargin(0.13);
  c1->SetRightMargin(0.04);
  c1->SetBottomMargin(0.12);
  c1->SetTopMargin(0.08);

  // Create a histogram for axis setup
  TH1F* hFrame = new TH1F("hFrame", " ", 100, -2.2, 2.2);
  hFrame->GetYaxis()->SetTitle("d^{2}#sigma/dydp_{T} (mb/GeV)");
  hFrame->GetXaxis()->SetTitle("D^{0} y");
  hFrame->SetStats(0);
  hFrame->GetYaxis()->SetTitleOffset(1.5);
  hFrame->GetYaxis()->SetRangeUser(0, 3.0); // Y-axis range

  hFrame->Draw(); // Draw the frame

  // Create a TGraphErrors object for data points
  TGraphErrors* gr = new TGraphErrors(nPoints, yValues.data(), crossSection.data(), yErrors.data(), crossErrors.data());
  gr->SetMarkerStyle(20); // Solid circle marker
  gr->SetMarkerSize(1.2);
  gr->SetLineColor(kRed);
  gr->SetMarkerColor(kRed);
  gr->SetLineWidth(2);

  gr->Draw("P E1 SAME"); // Draw with error bars

  /////////////////////////////////
  // [TODO] Need to add a flag to decide whether to overlay the HIN-24-003 result
  /////////////////////////////////
  TGraphErrors* gr_ref = (fix_IsGammaN)? &gr_ref_gammaN: &gr_ref_Ngamma;
  gr_ref->SetMarkerStyle(20); // Solid circle marker
  gr_ref->SetMarkerSize(1.2);
  gr_ref->SetLineColor(kBlack);
  gr_ref->SetMarkerColor(kBlack);
  gr_ref->SetLineWidth(2);

  gr_ref->Draw("P E1 SAME"); // Draw with error bars

  // Add legend
  TLegend* leg = new TLegend(0.1, 0.15, 0.48, 0.28);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->AddEntry(gr, "New framework", "P");
  leg->AddEntry(gr_ref, "HIN-24-003", "P");
  leg->Draw();

  // Add CMS Preliminary text
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.035);
  latex.SetTextFont(42);
  // latex.DrawLatex(0.15, 0.92, "CMS #it{Preliminary} 1.38 nb^{-1} (5.36 TeV PbPb)");
  // latex.DrawLatex(0.15, 0.86, "UPCs, ZDC Xn0n w/ gap");
  // latex.DrawLatex(0.15, 0.82, "Global uncert. #pm 5.05%");
  latex.DrawLatex(0.6, 0.82, Form("%d < D_{p_{T}} < %d (GeV/#it{c})", (int) fix_ptmin, (int) fix_ptmax));

  /////////////////////////////////
  // [TODO] The handling of naming could be done better, maybe outside the scope of the function
  // and need to add pt bin info
  /////////////////////////////////
  // Update canvas
  c1->Update();
  c1->SaveAs(Form("crossSectionPlot_%s.pdf",
                  (fix_IsGammaN)? "gammaN": "Ngamma")); // Save the plot

  /////////////////////////////////
  // 2. Plot RFB
  /////////////////////////////////
  // Create a histogram for axis setup
  TH1F* hFrame2 = new TH1F("hFrame2", " ", 100, (fix_IsGammaN)? -0.2: -2.2, 
                                                (fix_IsGammaN)?  2.2:  0.2);
  hFrame2->GetYaxis()->SetTitle("RFB");
  hFrame2->GetXaxis()->SetTitle(Form("%sD^{0} y", (fix_IsGammaN)? "+": "-"));
  hFrame2->SetStats(0);
  hFrame2->GetYaxis()->SetTitleOffset(1.5);
  hFrame2->GetYaxis()->SetRangeUser(0, 0.6); // Y-axis range

  hFrame2->Draw(); // Draw the frame

  // Create a TGraphErrors object for data points
  TGraphErrors* gr3 = new TGraphErrors(RFBXbins.size(), RFBXbins.data(), RFBValues.data(), RFBXbinErrors.data(), RFBErrors.data());
  gr3->Print("all");
  gr3->SetMarkerStyle(20); // Solid circle marker
  gr3->SetMarkerSize(1.2);
  gr3->SetLineColor(kBlack);
  gr3->SetMarkerColor(kBlack);
  gr3->SetLineWidth(2);
  gr3->Draw("P E1 SAME"); // Draw with error bars

  // Add legend
  // TLegend* leg2 = new TLegend(0.1, 0.15, 0.48, 0.28);
  // leg2->SetFillStyle(0);
  // leg2->SetBorderSize(0);
  // leg2->AddEntry(gr3, "New framework", "P");
  // leg2->Draw();

  // Add CMS Preliminary text
  latex.DrawLatex(0.6, 0.82, Form("%d < D_{p_{T}} < %d (GeV/#it{c})", (int) fix_ptmin, (int) fix_ptmax));

  /////////////////////////////////
  // [TODO] The handling of naming could be done better, maybe outside the scope of the function
  // and need to add pt bin info
  /////////////////////////////////
  // Update canvas
  c1->Update();
  c1->SaveAs(Form("RFBPlot_%s.pdf",
                  (fix_IsGammaN)? "gammaN": "Ngamma")); // Save the plot

}


void plotEff(vector<string>& inputPoints) {
  /////////////////////////////////
  // [TODO] Put the part of reading inputs in a function
  /////////////////////////////////
  const int nPoints = inputPoints.size();
  std::vector<Point> PointsArr(nPoints);

  double fix_ptmin;
  double fix_ptmax;
  bool fix_IsGammaN;
  double yValues[nPoints];
  double yErrors[nPoints];
  double rawYieldValues[nPoints];
  double rawYieldErrors[nPoints];
  double effEvt[nPoints];
  double effEvtError[nPoints];
  double numEvt[nPoints];
  double numEvtError[nPoints];
  double denEvt[nPoints];
  double denEvtError[nPoints];
  double effD[nPoints];
  double effDError[nPoints];
  double numD[nPoints];
  double numDError[nPoints];
  double denD[nPoints];
  double denDError[nPoints];
  for (int i = 0; i < nPoints; ++i)
  {
    getPoint(inputPoints[i], PointsArr[i]);
    
    Point& p(PointsArr[i]);
    if (i==0)
    {
      fix_ptmin = p.ptmin;
      fix_ptmax = p.ptmax;
      fix_IsGammaN = inputPoints[i].find("IsGammaN1") != string::npos;
    } else {
      if (fix_ptmin != p.ptmin || 
          fix_ptmax != p.ptmax ||
          fix_IsGammaN != (inputPoints[i].find("IsGammaN1") != string::npos)) {
        printf("[Error] Unexpected usage: Concatenating different pt bins or inconsistent gammaN/Ngamma results together! (%d,%d,%o),(%d,%d,%o) Exiting ...\n",
          p.ptmin, p.ptmax, (inputPoints[i].find("IsGammaN1") != string::npos),
          fix_ptmin, fix_ptmax, fix_IsGammaN);
        exit(1);
      }
    }

    yValues[i] = (p.ymax + p.ymin)/2.;
    yErrors[i] = (p.ymax - p.ymin)/2.;
    rawYieldValues[i] = p.rawYield;
    rawYieldErrors[i] = p.rawYieldError;
    effEvt[i] = p.effEvt;
    effEvtError[i]  = p.effEvtError;
    numEvt[i] = p.numEvt;
    numEvtError[i]  = TMath::Sqrt(p.numEvt);
    denEvt[i] = p.denEvt;
    denEvtError[i]  = TMath::Sqrt(p.denEvt);
    effD[i] = p.effD;
    effDError[i]  = p.effDError;
    numD[i] = p.numD;
    numDError[i]  = TMath::Sqrt(p.numD);
    denD[i] = p.denD;
    denDError[i]  = TMath::Sqrt(p.denD);
  }
  
  /////////////////////////////////
  // [TODO] Templatizing the plotting part (now is verbose)
  /////////////////////////////////
  /////////////////////////////////
  // 1. Plot the event-selection efficiency
  /////////////////////////////////
  // Create a canvas
  TCanvas* c1 = new TCanvas("c1", "Event-selection efficiency", 800, 800);
  c1->SetLeftMargin(0.13);
  c1->SetRightMargin(0.04);
  c1->SetBottomMargin(0.12);
  c1->SetTopMargin(0.08);

  // Create a histogram for axis setup
  TH1F* hFrame = new TH1F("hFrame", " ", 100, -2.2, 2.2);
  hFrame->GetYaxis()->SetTitle("#varepsilon_{event}");
  hFrame->GetXaxis()->SetTitle("D^{0} y");
  hFrame->SetStats(0);
  hFrame->GetYaxis()->SetTitleOffset(1.5);
  hFrame->GetYaxis()->SetRangeUser(0, 1.05); // Y-axis range

  hFrame->Draw(); // Draw the frame

  // Create a TGraphErrors object for data points
  TGraphErrors* gr = new TGraphErrors(nPoints, yValues, effEvt, yErrors, effEvtError);
  gr->Draw("P E1 SAME"); // Draw with error bars


  // Add legend
  TLegend* leg = new TLegend(0.1, 0.15, 0.48, 0.28);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->AddEntry(gr, "New framework", "P");
  leg->Draw();

  // Add CMS Preliminary text
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.035);
  latex.SetTextFont(42);
  // latex.DrawLatex(0.15, 0.92, "CMS #it{Preliminary} 1.38 nb^{-1} (5.36 TeV PbPb)");
  // latex.DrawLatex(0.15, 0.86, "UPCs, ZDC Xn0n w/ gap");
  // latex.DrawLatex(0.15, 0.82, "Global uncert. #pm 5.05%");
  latex.DrawLatex(0.6, 0.82, Form("%d < D_{p_{T}} < %d (GeV/#it{c})", (int) fix_ptmin, (int) fix_ptmax));

  // Update canvas
  c1->Update();
  c1->SaveAs(Form("evtEff_%s.pdf",
                  (fix_IsGammaN)? "gammaN": "Ngamma")); // Save the plot

  /////////////////////////////////
  // 2. Plot the numerator of n events
  /////////////////////////////////
  hFrame->GetYaxis()->SetTitle("Numerator N_{event}");
  hFrame->GetXaxis()->SetTitle("D^{0} y");
  hFrame->GetYaxis()->SetRangeUser(0, 500); // Y-axis range

  hFrame->Draw(); // Draw the frame

  // Create a TGraphErrors object for data points
  gr = new TGraphErrors(nPoints, yValues, numEvt, yErrors, numEvtError);
  gr->Draw("P E1 SAME"); // Draw with error bars
  leg->Draw();
  latex.DrawLatex(0.6, 0.82, Form("%d < D_{p_{T}} < %d (GeV/#it{c})", (int) fix_ptmin, (int) fix_ptmax));

  // Update canvas
  c1->Update();
  c1->SaveAs(Form("evtNum_%s.pdf",
                  (fix_IsGammaN)? "gammaN": "Ngamma")); // Save the plot

  /////////////////////////////////
  // 3. Plot the denominator of n events
  /////////////////////////////////
  hFrame->GetYaxis()->SetTitle("Denominator N_{event}");
  hFrame->GetXaxis()->SetTitle("D^{0} y");
  hFrame->GetYaxis()->SetRangeUser(0, 500); // Y-axis range

  hFrame->Draw(); // Draw the frame

  // Create a TGraphErrors object for data points
  gr = new TGraphErrors(nPoints, yValues, denEvt, yErrors, denEvtError);
  gr->Draw("P E1 SAME"); // Draw with error bars
  leg->Draw();
  latex.DrawLatex(0.6, 0.82, Form("%d < D_{p_{T}} < %d (GeV/#it{c})", (int) fix_ptmin, (int) fix_ptmax));

  // Update canvas
  c1->Update();
  c1->SaveAs(Form("evtDen_%s.pdf",
                  (fix_IsGammaN)? "gammaN": "Ngamma")); // Save the plot

  /////////////////////////////////
  // 4. Plot the D efficiency
  /////////////////////////////////
  hFrame->GetYaxis()->SetTitle("#varepsilon_{D}");
  hFrame->GetXaxis()->SetTitle("D^{0} y");
  hFrame->GetYaxis()->SetRangeUser(0, 1.05); // Y-axis range

  hFrame->Draw(); // Draw the frame

  // Create a TGraphErrors object for data points
  gr = new TGraphErrors(nPoints, yValues, effD, yErrors, effDError);
  gr->Draw("P E1 SAME"); // Draw with error bars
  leg->Draw();
  latex.DrawLatex(0.6, 0.82, Form("%d < D_{p_{T}} < %d (GeV/#it{c})", (int) fix_ptmin, (int) fix_ptmax));

  // Update canvas
  c1->Update();
  c1->SaveAs(Form("DEff_%s.pdf",
                  (fix_IsGammaN)? "gammaN": "Ngamma")); // Save the plot

  /////////////////////////////////
  // 5. Plot the numerator of n events
  /////////////////////////////////
  hFrame->GetYaxis()->SetRangeUser(0, 0.2); // Y-axis range
  hFrame->Draw(); // Draw the frame

  gr->Draw("P E1 SAME"); // Draw with error bars
  leg->Draw();
  latex.DrawLatex(0.6, 0.82, Form("%d < D_{p_{T}} < %d (GeV/#it{c})", (int) fix_ptmin, (int) fix_ptmax));

  // Update canvas
  c1->Update();
  c1->SaveAs(Form("DEff_zoom_%s.pdf",
                  (fix_IsGammaN)? "gammaN": "Ngamma")); // Save the plot

  /////////////////////////////////
  // 5. Plot the numerator of n Ds
  /////////////////////////////////
  hFrame->GetYaxis()->SetTitle("Numerator N_{D}");
  hFrame->GetXaxis()->SetTitle("D^{0} y");
  hFrame->GetYaxis()->SetRangeUser(0, 40); // Y-axis range

  hFrame->Draw(); // Draw the frame

  // Create a TGraphErrors object for data points
  gr = new TGraphErrors(nPoints, yValues, numD, yErrors, numDError);
  gr->Draw("P E1 SAME"); // Draw with error bars
  leg->Draw();
  latex.DrawLatex(0.6, 0.82, Form("%d < D_{p_{T}} < %d (GeV/#it{c})", (int) fix_ptmin, (int) fix_ptmax));

  // Update canvas
  c1->Update();
  c1->SaveAs(Form("DNum_%s.pdf",
                  (fix_IsGammaN)? "gammaN": "Ngamma")); // Save the plot

  /////////////////////////////////
  // 6. Plot the denominator of n Ds
  /////////////////////////////////
  hFrame->GetYaxis()->SetTitle("Denominator N_{D}");
  hFrame->GetXaxis()->SetTitle("D^{0} y");
  hFrame->GetYaxis()->SetRangeUser(0, 500); // Y-axis range

  hFrame->Draw(); // Draw the frame

  // Create a TGraphErrors object for data points
  gr = new TGraphErrors(nPoints, yValues, denD, yErrors, denDError);
  gr->Draw("P E1 SAME"); // Draw with error bars
  leg->Draw();
  latex.DrawLatex(0.6, 0.82, Form("%d < D_{p_{T}} < %d (GeV/#it{c})", (int) fix_ptmin, (int) fix_ptmax));

  // Update canvas
  c1->Update();
  c1->SaveAs(Form("DDen_%s.pdf",
                  (fix_IsGammaN)? "gammaN": "Ngamma")); // Save the plot

  /////////////////////////////////
  // 7. Plot the raw yield
  /////////////////////////////////
  hFrame->GetYaxis()->SetTitle("Raw yield");
  hFrame->GetXaxis()->SetTitle("D^{0} y");
  hFrame->GetYaxis()->SetRangeUser(0, 280); // Y-axis range

  hFrame->Draw(); // Draw the frame

  // Create a TGraphErrors object for data points
  gr = new TGraphErrors(nPoints, yValues, rawYieldValues, yErrors, rawYieldErrors);
  gr->Draw("P E1 SAME"); // Draw with error bars
  leg->Draw();
  latex.DrawLatex(0.6, 0.82, Form("%d < D_{p_{T}} < %d (GeV/#it{c})", (int) fix_ptmin, (int) fix_ptmax));

  // Update canvas
  c1->Update();
  c1->SaveAs(Form("RawYield_%s.pdf",
                  (fix_IsGammaN)? "gammaN": "Ngamma")); // Save the plot


}


int main(int argc, char *argv[])
{
  CommandLine CL(argc, argv);
  vector<string> inputPoints    = CL.GetStringVector("InputPoints",    ""); // Input corrected yields md files

  plotCrossSection(inputPoints);
  plotEff(inputPoints);
  return 0;
}