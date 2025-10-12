// Add to macros with:
//    #include "MITHIG_CMSStyle.h"
//
// Based on the official CMS TDRStyle (link below) and SetStyle.h by Yi Chen.
// https://cms-analysis.docs.cern.ch/guidelines/plotting/general/
// Author: Jordan Lang (MIT) jdlang@cern.ch
//
//  78) 1. Colors and Gradients
//  80)   - Official CMS Colors (renamed for easy use)
// 117)   - Gradient Maps in CMS Colors (unofficial)
//
// 194) 2. Header Functions (line 165)
// 196)   - AddCMSHeader(TPad* pad, TString subheader, bool insideFrame)
//          Draws "CMS" and an optional subheader to the top-left corner of
//          a plot, or above the top-left corner if insideFrame==false.
// 243)   - AddUPCHeader(TPad* pad, TString collText, TString lumiText)
//          Draws the collision and luminosity info above the top-right corner.
// 313)   - AddPythiaHeader(TPad* pad, TString extraText = " + EvtGen")
//          Draws "PYTHIA8" below the in-pad CMS header.
// 353)   - AddPlotLabel(TPad* pad, TString plotLabel)
//          Draws a line of text on the top-right corner of a plot.
// 387)   - AddPlotLabels(TPad* pad, vector<TString> plotLabels)
//          Draws multiple lines of text from the top-right corner of a plot.
//
// 427) 3. CMS Plot Style
// 431)   - SetTDRStyle(int canvasWidth = 600, int canvasHeight = 600)
//          Applies CMS TDR style with gStyle.
//
// 578) 4. Plot Styling Tools
// 580)   - Marker styles
// 605)   - Line styles
// 612)   - SetTH1Fonts(TH1* hist, float padScale = 1.)
// 629)   - SetTH2Fonts(TH1* hist, float padScale = 1.)
// 650)   - GetMarkerScale(int marker)
// 667)   - SetHistMarker(TH1* hist, Int_t histMarker)
//          Use to scale renamed markers to be visually uniform sizes.
// 675)   - StyleTH1(TPad* pad, TH1* hist, TString plotLabel,
//              Int_t histColor, Int_t histMarker)
// 710)   - StyleTH2(TPad* pad, TH2* hist, TString plotLabel,
//              bool showText, bool showError)
// 761)   - StyleTGraphAsymmErrors( TPad* pad, TGraphAsymmErrors* graph,
//              TString plotLabel, Int_t graphColor, Int_t graphMarker)
// 796)   - SaveTH1(TString outputPath, TH1* hist)
//          Use to quickly style and save any TH1. Include file name and desired
//          extension in outputPath (e.g. "plots/hData.pdf)
// 813)   - SaveTH2(TString outputPath, TH2* hist)
//          Use to quickly style and save any TH1. Include file name and desired
//          extension in outputPath (e.g. "plots/hData.pdf)
//
//

#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <iostream>

#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TMath.h"
#include "TGaxis.h"
#include "TPaletteAxis.h"
#include "TLine.h"
#include "THStack.h"
#include "TColor.h"

// =============================================================================
// 1. COLORS AND GRADIENTS -----------------------------------------------------

// CMS OFFICIAL PLOT COLORS
Int_t cmsBlue     = TColor::GetColor("#5790fc"); // blue
Int_t cmsYellow   = TColor::GetColor("#f89c20"); // yellow-orange
Int_t cmsRed      = TColor::GetColor("#e42536"); // red
Int_t cmsPurple   = TColor::GetColor("#964a8b"); // warm purple
Int_t cmsGray     = TColor::GetColor("#9c9ca1"); // cool gray
Int_t cmsViolet   = TColor::GetColor("#7a21dd"); // vibrant cool purple
Int_t cmsTeal     = TColor::GetColor("#16a085"); // teal-green
Int_t cmsPaleBlue = TColor::GetColor("#92dadd"); // pale blue/teal
Int_t cmsBlack    = kBlack; // for consistency
Int_t cmsWhite    = kWhite; // for consistency
Int_t cmsAltBlack = TColor::GetColor("#313135"); // unofficial!
// UNOFFICIAL LIGHT VARIATIONS
Int_t cmsBlueL1   = TColor::GetColor("#80B5FF"); // light blue
Int_t cmsYellowL1 = TColor::GetColor("#FEBA4D"); // light yellow-orange
Int_t cmsRedL1    = TColor::GetColor("#F05660"); // light red
Int_t cmsPurpleL1 = TColor::GetColor("#C36FB5"); // light warm purple
Int_t cmsGrayL1   = TColor::GetColor("#CFCFD3"); // light cool gray
Int_t cmsVioletL1 = TColor::GetColor("#9F5DE9"); // light vibrant cool purple
Int_t cmsTealL1   = TColor::GetColor("#21DEB8"); // light teal-green
Int_t cmsAltBlackL1 = TColor::GetColor("#4B4B4E");
// UNOFFICIAL DARK VARIATIONS
Int_t cmsBlueD1   = TColor::GetColor("#2067F3"); // dark blue
Int_t cmsYellowD1 = TColor::GetColor("#D0740B"); // dark yellow-orange
Int_t cmsRedD1    = TColor::GetColor("#B61B30"); // dark red
Int_t cmsPurpleD1 = TColor::GetColor("#6C3766"); // dark warm purple
Int_t cmsGrayD1   = TColor::GetColor("#68686E"); // dark cool gray
Int_t cmsVioletD1 = TColor::GetColor("#5C1DA5"); // dark vibrant cool purple
Int_t cmsTealD1   = TColor::GetColor("#116955"); // dark teal-green
Int_t cmsAltBlackD1 = TColor::GetColor("#0E0E10");

Int_t ColorStrings[6] = {
  cmsBlue, cmsYellow, cmsRed, cmsPurple, cmsGray, cmsTeal};
// Note: Official guidelines have cmsViolet as the last color,
// but that's too similar to cmsPurple so I swapped them. -jdlang

// CMS-COLORED GRADIENT MAPS (UNOFFICIAL, made by jdlang)
// These should be perceptually linear and colorblind safe.
// To use gradient maps, first add this line to the start of main():
//    BuildPalettes();
// Then use a palette by adding one of these lines to your plotting script:
//    gStyle->SetPalette(nCont, cmsRedGrad);
//    gStyle->SetPalette(nCont, cmsBlueGrad);
const int nCont = 256;
// Gradient map: cmsSunset (cmsPurple -> cmsRed -> cmsYellow)
Int_t cmsSunset[nCont];
const int nrgbsSunset = 6;
Double_t srgbSunset[4][nrgbsSunset] = {
  {    0.00,     0.15,     0.40,     0.55,     0.75,     1.00},
  { 78./255, 106./255, 232./255, 244./255, 250./255, 255./255},
  { 29./255,  51./255,  67./255, 105./255, 160./255, 209./255},
  { 99./255, 130./255,  83./255,  84./255,  43./255, 139./255} };
Int_t colorTableSunset = TColor::CreateGradientColorTable(
  nrgbsSunset, srgbSunset[0], srgbSunset[1], srgbSunset[2],
  srgbSunset[3], nCont);
  
// Gradient map: cmsRedGrad (cmsRed(dark) -> cmsRed -> cmsRed(light))
Int_t cmsRedGrad[nCont];
const int nrgbsRedGrad = 3;
Double_t srgbRedGrad[4][nrgbsRedGrad] = {
  {    0.00,     0.28,     1.00},
  {139./255, 226./255, 255./255},
  {  6./255,  39./255, 205./255},
  { 34./255,  58./255, 199./255} };
Int_t colorTableRedGrad = TColor::CreateGradientColorTable(
  nrgbsRedGrad, srgbRedGrad[0], srgbRedGrad[1], srgbRedGrad[2],
  srgbRedGrad[3], nCont);
  
// Gradient map: cmsBlueGrad (cmsBlue(dark) -> cmsBlue -> cmsBlue(light))
Int_t cmsBlueGrad[nCont];
const int nrgbsBlueGrad = 3;
Double_t srgbBlueGrad[4][nrgbsBlueGrad] = {
  {    0.00,     0.45,     1.00},
  {  7./255,  51./255, 168./255},
  { 59./255, 146./255, 233./255},
  {136./255, 230./255, 255./255} };
Int_t colorTableBlueGrad = TColor::CreateGradientColorTable(
  nrgbsBlueGrad, srgbBlueGrad[0], srgbBlueGrad[1], srgbBlueGrad[2],
  srgbBlueGrad[3], nCont);
  
// Include BuildPalettes() in main function to use palettes:
void BuildPalettes(
) {
  for (int i=0; i<nCont; i++) cmsSunset[i]    = colorTableSunset + i;
  for (int i=0; i<nCont; i++) cmsRedGrad[i]   = colorTableRedGrad + i;
  for (int i=0; i<nCont; i++) cmsBlueGrad[i]  = colorTableBlueGrad + i;
}

// Standardized pad sizes
int squareWidth         = 600;
int squareHeight        = 600;
int squareWidthTH2      = 660;
int squareHeightTH2     = 660;
// Margins
double marginLeft       = 0.18;
double marginRight      = 0.02;
double marginTop        = 0.06;
double marginBottom     = 0.14;
double paletteWidth     = 0.04;
// Font Sizes and Positions
double plotTextSize     = 0.045;
double plotTextOffset   = 0.015;
double titleSize        = 0.050;
double axisTitleSize    = 0.050;
double axisLabelSize    = 0.045;
double axisTitleOffsetX = 1.3;
double axisTitleOffsetY = 1.3;
double axisTitleOffsetZ = 1.3;
double legendSize       = plotTextSize;
double histMarkerSize   = 1.75; // sets font size for TH2 bin text
double tickLength       = 0.02;

// =============================================================================
// 2. HEADER FUNCTIONS ---------------------------------------------------------

// CMS HEADER
void AddCMSHeader(
  TPad* pad,
  TString subheader = "",
  bool insideFrame = true, // Set false to put CMS label outside of plot frame
  float textSize = plotTextSize,
  float textOffset = 0.015
) {
  pad->cd();
  float H = pad->GetWh();
  float W = pad->GetWw();
  float l = pad->GetLeftMargin();
  float t = pad->GetTopMargin();
  float r = pad->GetRightMargin();
  float b = pad->GetBottomMargin();
  
  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);
  latex.SetTextFont(42);
  latex.SetTextSize(textSize);
  
  if (insideFrame) {
    latex.SetTextAlign(13);
    latex.DrawLatex(
      l + (1 - l - r) * 0.05,
      1 - (t + (1 - t - b) * 0.05),
      "#font[61]{#scale[1.25]{CMS}}"
    );
    latex.DrawLatex(
      l + (1 - l - r) * 0.05,
      1 - (t + (1 - t - b) * 0.05) - 1.25 * textSize,
      Form("#font[52]{%s}", subheader.Data())
    );
  }
  else {
    latex.SetTextAlign(11);
    latex.DrawLatex(
      l,
      1 - t + textOffset,
      Form("#font[61]{#scale[1.25]{CMS}} #font[52]{%s}", subheader.Data())
    );
  }
}

// UPC HEADER (CMS STYLE) FOR COLLISION & LUMI INFO
void AddUPCHeader(
  TPad* pad,
  TString collText = "5.36 TeV", // "#font[52]{#sqrt{s_{NN}}} = 5.36 TeV PbPb"
  TString lumiText = "PbPb UPCs 1.38 nb^{-1}", // set to "" to ignore
  double lumiValue = -1, // units of pb^{-1}, set to -1 to ignore
  float textSize = plotTextSize,
  float textOffset = 0.015
) {
  pad->cd();
  float H = pad->GetWh();
  float W = pad->GetWw();
  float l = pad->GetLeftMargin();
  float t = pad->GetTopMargin();
  float r = pad->GetRightMargin();
  float b = pad->GetBottomMargin();
  
  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);
  latex.SetTextFont(42);
  latex.SetTextAlign(31);
  latex.SetTextSize(textSize);
  
  // Makes lumi text string from
  TString lumiUnit;
  if (lumiText.CompareTo("") && lumiValue > 0) {
    lumiUnit = "pb^{-1}";
    if (lumiValue < 1 && lumiValue >= 0.001) {
      lumiUnit = "nb^{-1}";
      lumiValue = lumiValue * 1000;
    }
    else if (lumiValue < 0.001 && lumiValue >= 0.000001) {
      lumiUnit = "#mu b^{-1}";
      lumiValue = lumiValue * 1000000;
    }
    else if (lumiValue < 0.000001 && lumiValue >= 0.000000001) {
      lumiUnit = "mb^{-1}";
      lumiValue = lumiValue * 1000000000;
    }
    else if (lumiValue < 1000000 && lumiValue >= 1000) {
      lumiUnit = "fb^{-1}";
      lumiValue = lumiValue / 1000;
    }
    else if (lumiValue < 1000000000 && lumiValue >= 1000000) {
      lumiUnit = "ab^{-1}";
      lumiValue = lumiValue / 1000000;
    }
  }
  if (lumiValue > 0 && lumiValue < 20) {
    latex.DrawLatex(
      1 - r, 1 - t + textOffset,
      Form("%.1f %s %s", lumiValue, lumiUnit.Data(), collText.Data())
    );
  }
  else if (lumiValue >= 0) {
    latex.DrawLatex(
      1 - r, 1 - t + textOffset,
      Form("%.0f %s %s", lumiValue, lumiUnit.Data(), collText.Data())
    );
  }
  else {
    latex.DrawLatex(
      1 - r, 1 - t + textOffset,
      Form("%s (%s)", lumiText.Data(), collText.Data())
    );
  }
}

// PYTHIA HEADER
void AddPythiaHeader(
  TPad* pad,
  TString extraText = " + EvtGen",
  bool insideFrame = true, // Set false to put CMS label outside of plot frame
  float textSize = plotTextSize,
  float textOffset = 0.015
) {
  pad->cd();
  float H = pad->GetWh();
  float W = pad->GetWw();
  float l = pad->GetLeftMargin();
  float t = pad->GetTopMargin();
  float r = pad->GetRightMargin();
  float b = pad->GetBottomMargin();
  
  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);
  latex.SetTextFont(42);
  latex.SetTextSize(textSize);
  
  if (insideFrame) {
    latex.SetTextAlign(13);
    latex.DrawLatex(
      l + (1 - l - r) * 0.05,
      1 - (t + (1 - t - b) * 0.05) - 2.35 * textSize,
      Form("P#scale[0.8]{YTHIA}8%s", extraText.Data())
    );
  }
  else {
    latex.SetTextAlign(11);
    latex.DrawLatex(
      l,
      1 - t + textOffset,
      Form("P#scale[0.8]{YTHIA}8%s", extraText.Data())
    );
  }
}

void AddPlotLabel(
  TPad* pad,
  TString plotLabel,
  float textSize = plotTextSize,
  float textOffset = 0.015,
  float textX = 0.,
  float textY = 0.
) {
  pad->cd();
  float H = pad->GetWh();
  float W = pad->GetWw();
  float l = pad->GetLeftMargin();
  float t = pad->GetTopMargin();
  float r = pad->GetRightMargin();
  float b = pad->GetBottomMargin();
  
  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);
  latex.SetTextFont(42);
  latex.SetTextAlign(33);
  latex.SetTextSize(textSize);
  
  if (textX == 0.) textX = 1 - r - (1 - l - r) * 0.05;
  if (textY == 0.) textY = 1 - t - (1 - t - b) * 0.05;
  
  latex.DrawLatex(
    textX,
    textY,
    plotLabel
  );
}

void AddPlotLabels(
  TPad* pad,
  std::vector<TString> plotLabels,
  float textSize = plotTextSize,
  float textOffset = plotTextOffset,
  float textX = 0.,
  float textY = 0.
) {
  pad->cd();
  float H = pad->GetWh();
  float W = pad->GetWw();
  float l = pad->GetLeftMargin();
  float t = pad->GetTopMargin();
  float r = pad->GetRightMargin();
  float b = pad->GetBottomMargin();
  
  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);
  latex.SetTextFont(42);
  latex.SetTextAlign(13);
  latex.SetTextSize(textSize);
  
  if (textX == 0.) {
    textX = 1 - r - (1 - l - r) * 0.05;
    latex.SetTextAlign(33);
  }
  if (textY == 0.) textY = 1 - t - (1 - t - b) * 0.05;
  
  for (int i = 0; i < plotLabels.size(); i++) {
    latex.DrawLatex(
      textX,
      textY - (textSize + textOffset) * i,
      plotLabels[i]
    );
  }
}

// =============================================================================
// 3. CMS PLOT STYLE -----------------------------------------------------------

// CMS TDR STYLE
// This has been slightly modified for the scale of a 600 x 600 canvas
void SetTDRStyle(
  int canvasWidth = 600,
  int canvasHeight = 600,
  bool setStatStyle = true,
  bool setMarginStyle = false,
  bool setHistStyle = false,
  bool setFitStyle = false,
  bool setDateStyle = false
) {
  // For the canvas:
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetCanvasDefW(canvasWidth); //Width of canvas
  gStyle->SetCanvasDefH(canvasHeight); //Height of canvas
  gStyle->SetCanvasDefX(0); //Position on screen
  gStyle->SetCanvasDefY(0); //Position on screen

  // For the Pad:
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(0);
  gStyle->SetPadGridX(false);
  gStyle->SetPadGridY(false);
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);

  // For the frame:
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);
  
  // For the global title:
  gStyle->SetOptTitle(0);
  gStyle->SetTitleFont(42);
  gStyle->SetTitleColor(1);
  gStyle->SetTitleTextColor(1);
  gStyle->SetTitleFillColor(10);
  gStyle->SetTitleFontSize(titleSize);
  // gStyle->SetTitleH(0); // Set the height of the title box
  // gStyle->SetTitleW(0); // Set the width of the title box
  // gStyle->SetTitleX(0); // Set the position of the title box
  // gStyle->SetTitleY(0.985); // Set the position of the title box
  // gStyle->SetTitleStyle(Style_t style = 1001);

  // For the axis titles:
  gStyle->SetTitleColor(1, "XYZ");
  gStyle->SetTitleFont(42, "XYZ");
  gStyle->SetTitleSize(axisTitleSize, "x");
  gStyle->SetTitleSize(axisTitleSize, "y");
  gStyle->SetTitleSize(axisTitleSize, "z");
  gStyle->SetTitleOffset(axisTitleOffsetX, "x");
  gStyle->SetTitleOffset(axisTitleOffsetY, "y");
  gStyle->SetTitleOffset(axisTitleOffsetZ, "z");

  // For the axis labels:
  gStyle->SetLabelColor(1, "XYZ");
  gStyle->SetLabelFont(42, "XYZ");
  gStyle->SetLabelSize(axisLabelSize, "x");
  gStyle->SetLabelSize(axisLabelSize, "y");
  gStyle->SetLabelSize(axisLabelSize, "z");
  gStyle->SetLabelOffset(0.01, "x");
  gStyle->SetLabelOffset(0.01, "y");
  gStyle->SetLabelOffset(0.01, "z");

  // For the axis:
  gStyle->SetAxisColor(1, "XYZ");
  gStyle->SetStripDecimals(kTRUE);
  gStyle->SetTickLength(0.03, "XYZ");
  gStyle->SetNdivisions(510, "XYZ");
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  
  // For the statistics box:
  if (setStatStyle) {
    gStyle->SetOptFile(0);
    gStyle->SetOptStat(0); // To display the mean and RMS: SetOptStat("mr");
    gStyle->SetStatColor(0);
    gStyle->SetStatFont(42);
    gStyle->SetStatFontSize(0.025);
    gStyle->SetStatTextColor(1);
    gStyle->SetStatFormat("6.4g");
    gStyle->SetStatBorderSize(1);
    gStyle->SetStatH(0.1);
    gStyle->SetStatW(0.15);
    // gStyle->SetStatStyle(Style_t style = 1001);
    // gStyle->SetStatX(Float_t x = 0);
    // gStyle->SetStatY(Float_t y = 0);
  }

  // For the margins:
  if (setMarginStyle) {
    gStyle->SetPadTopMargin(marginTop);//0.05
    gStyle->SetPadBottomMargin(marginBottom);//0.13
    gStyle->SetPadLeftMargin(marginLeft);//0.16
    gStyle->SetPadRightMargin(marginRight);//0.02
  }
  
  // For the hist:
  if (setHistStyle) {
    gStyle->SetHistFillColor(1);
    gStyle->SetHistFillStyle(0);
    gStyle->SetHistLineColor(1);
    gStyle->SetHistLineStyle(0);
    gStyle->SetHistLineWidth(1);
    gStyle->SetMarkerColor(1);
    gStyle->SetMarkerStyle(20);
    gStyle->SetEndErrorSize(2);
    gStyle->SetErrorX(0.);
  }
  
  // For the fit/function:
  if (setFitStyle) {
    gStyle->SetOptFit(1);
    gStyle->SetFitFormat("5.4g");
    gStyle->SetFuncColor(2);
    gStyle->SetFuncStyle(1);
    gStyle->SetFuncWidth(1);
  }

  // For the date:
  if (setDateStyle) {
    gStyle->SetOptDate(0);
  }

  // Postscript options:
  // gStyle->SetPaperSize(20.,20.);
  // gStyle->SetLineScalePS(Float_t scale = 3);
  // gStyle->SetLineStyleString(Int_t i, const char* text);
  // gStyle->SetHeaderPS(const char* header);
  // gStyle->SetTitlePS(const char* pstitle);

  // gStyle->SetBarOffset(Float_t baroff = 0.5);
  // gStyle->SetBarWidth(Float_t barwidth = 0.5);
  // gStyle->SetPaintTextFormat(const char* format = "g");
  // gStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // gStyle->SetTimeOffset(Double_t toffset);
  // gStyle->SetHistMinimumZero(kTRUE);

  // gStyle->SetHatchesLineWidth(5);
  // gStyle->SetHatchesSpacing(0.05);
}

// =============================================================================
// 4. PLOT STYLING TOOLS -------------------------------------------------------

// RENAMED MARKER STYLES
Int_t mCircleFill       = 20;
Int_t mCircleLine       = 24;
Int_t mSquareFill       = 21;
Int_t mSquareLine       = 25;
Int_t mDiamondFill      = 33;
Int_t mDiamondLine      = 27;
Int_t mStarFill         = 43;
Int_t mStarLine         = 42;
Int_t mCrossFill        = 34;
Int_t mCrossLine        = 28;
Int_t mXCrossFill       = 47;
Int_t mXCrossLine       = 46;
Int_t mTriangleUpFill   = 22;
Int_t mTriangleUpLine   = 26;
Int_t mTriangleDownFill = 23;
Int_t mTriangleDownLine = 32;
// Scale factors for perceptually uniform marker sizes
float mCircleScale      = 1.0;
float mSquareScale      = 0.9;
float mDiamondScale     = 1.5;
float mStarScale        = 1.4;
float mCrossScale       = 0.9;
float mTriangleScale    = 1.1;

// RENAMED LINE STYLES
Int_t lSolid      = 1; // ===========
Int_t lDash       = 7; // == == == ==
Int_t lNarrowDash = 3; // : : : : : :
Int_t lSquareDash = 2; // = = = = = =
Int_t lWideDash   = 9; // === === ===

void SetTH1Fonts(
  TH1* hist,
  float padScale = 1.
) {
  gStyle->SetTextFont(42);
  hist->SetTitleSize(titleSize);
  hist->GetXaxis()->SetTitleSize(axisTitleSize / padScale);
  hist->GetXaxis()->SetTitleOffset(axisTitleOffsetX / padScale);
  hist->GetXaxis()->SetLabelSize(axisLabelSize / padScale);
  hist->GetXaxis()->SetLabelOffset(0.01 / padScale);
  hist->GetYaxis()->SetTitleSize(axisTitleSize / padScale);
  hist->GetYaxis()->SetTitleOffset(axisTitleOffsetY / padScale);
  hist->GetYaxis()->SetLabelSize(axisLabelSize / padScale);
  hist->GetYaxis()->SetLabelOffset(0.01 / padScale);
  return;
}

void SetTH2Fonts(
  TH2* hist,
  float padScale = 1.
) {
  gStyle->SetTextFont(42);
  hist->SetTitleSize(titleSize);
  hist->GetXaxis()->SetTitleSize(axisTitleSize / padScale);
  hist->GetXaxis()->SetTitleOffset(axisTitleOffsetX / padScale);
  hist->GetXaxis()->SetLabelSize(axisLabelSize / padScale);
  hist->GetXaxis()->SetLabelOffset(0.01 / padScale);
  hist->GetYaxis()->SetTitleSize(axisTitleSize / padScale);
  hist->GetYaxis()->SetTitleOffset(axisTitleOffsetY / padScale);
  hist->GetYaxis()->SetLabelSize(axisLabelSize / padScale);
  hist->GetYaxis()->SetLabelOffset(0.01 / padScale);
  hist->GetZaxis()->SetTitleSize(axisTitleSize / padScale);
  hist->GetZaxis()->SetTitleOffset(axisTitleOffsetZ / padScale);
  hist->GetZaxis()->SetLabelSize(axisLabelSize / padScale);
  hist->GetZaxis()->SetLabelOffset(0.01 / padScale);
  return;
}

float GetMarkerScale(
  Int_t m
) {
  float markerSize = 1.0;
  // Circles
  if (m==20 || m==24) markerSize = 1.0;
  // Squares and Crosses
  else if (m==21 || m==25 || m==34 || m==28 || m==47 || m==46) markerSize = 0.9;
  // Diamonds
  else if (m==33 || m==27) markerSize = 1.5;
  // 4-pointed Stars
  else if (m==33 || m==27) markerSize = 1.4;
  // Triangles
  else if (m==22 || m==26 || m==23 || m==32) markerSize = 1.1;
  return markerSize;
}

void SetHistMarker(
  TH1* hist,
  Int_t histMarker
) {
  hist->SetMarkerStyle(histMarker);
  hist->SetMarkerSize(GetMarkerScale(histMarker));
}

void StyleTH1(
  TPad* pad,
  TH1*  hist,
  TString plotLabel = "",
  Int_t histColor = cmsBlack,
  Int_t histMarker = mCircleFill,
  float padScale = 1.
) {
  pad->cd();
  // Set margins
  pad->SetMargin(
    marginLeft * padScale,
    marginRight * padScale,
    marginBottom * padScale,
    marginTop * padScale
  );
  SetTDRStyle(pad->GetWw(), pad->GetWh());
  SetTH1Fonts(hist, padScale);
  // Set pad styles
  TGaxis::SetMaxDigits(3);
  gStyle->SetPaintTextFormat("3.3g");
  // Set hist style
  SetHistMarker(hist, histMarker);
  hist->SetMarkerColor(histColor);
  hist->SetLineColor(histColor);
  hist->SetLineWidth(1);
  // Draw hist
  hist->Draw();
  // Add header
  if (!plotLabel.CompareTo("")) AddPlotLabel(pad, plotLabel);
  gPad->RedrawAxis();
  pad->Update();
  return;
}

void StyleTH2(
  TPad* pad,
  TH2*  hist,
  TString plotLabel = "",
  bool showText = true,
  bool showError = true,
  int colorPalette = 0, //-1:kViridis, 0:cmsSunset, 1:cmsRedGrad, 2:cmsBlueGrad
  float padScale = 1.
) {
  BuildPalettes();
  pad->cd();
  pad->SetMargin(
    marginLeft / padScale,
    marginLeft / padScale,
    marginBottom / padScale,
    marginTop / padScale
  );
  SetTDRStyle(pad->GetWw(), pad->GetWh());
  SetTH2Fonts(hist, padScale);
  // Set pad styles
  TGaxis::SetMaxDigits(3);
  gStyle->SetPaintTextFormat("3.3g");
  hist->SetMarkerSize(histMarkerSize / padScale);
  // Set Palette
  if (colorPalette == 0) gStyle->SetPalette(nCont, cmsSunset);
  else if (colorPalette == 1) gStyle->SetPalette(nCont, cmsRedGrad);
  else if (colorPalette == 2) gStyle->SetPalette(nCont, cmsBlueGrad);
  else gStyle->SetPalette(kViridis);
  // Draw hist
  if (showText && showError) hist->Draw("colz text00 error");
  else if (showText) hist->Draw("colz text00");
  else hist->Draw("colz");
  gPad->Update();
  // Position z-axis palette
  TPaletteAxis* palette =
    (TPaletteAxis*) hist->GetListOfFunctions()->FindObject("palette");
  if (palette) {
    palette->SetX1NDC(1 - marginLeft / padScale
      + (1./pad->GetWw())); // starts just outside plot line
    palette->SetX2NDC(1 - marginLeft / padScale + paletteWidth / padScale);
    palette->SetY1NDC(marginBottom / padScale);
    palette->SetY2NDC(1 - marginTop / padScale);
  }
  // Add header
  if (!plotLabel.CompareTo(""))
    AddPlotLabel(pad, plotLabel, plotTextSize / padScale);
  gPad->RedrawAxis();
  pad->Update();
  return;
}

void StyleTGraphAsymmErrors(
  TPad* pad,
  TGraphAsymmErrors* graph,
  TString plotLabel = "",
  Int_t graphColor = cmsBlack,
  Int_t graphMarker = mCircleFill,
  float padScale = 1.
) {
  pad->cd();
  // Set margins
  pad->SetMargin(
    marginLeft / padScale,
    marginRight / padScale,
    marginBottom / padScale,
    marginTop / padScale
  );
  SetTDRStyle(squareWidth, squareHeight);
  SetTH1Fonts(graph->GetHistogram(), padScale);
  // Set pad styles
  TGaxis::SetMaxDigits(3);
  gStyle->SetPaintTextFormat("3.3g");
  // Set hist style
  SetHistMarker(graph->GetHistogram(), graphMarker);
  graph->SetMarkerColor(graphColor);
  graph->SetLineColor(graphColor);
  graph->SetLineWidth(1);
  // Draw hist
  graph->Draw();
  // Add header
  if (!plotLabel.CompareTo("")) AddPlotLabel(pad, plotLabel);
  gPad->RedrawAxis();
  pad->Update();
  return;
}

void SaveTH1(
  TString outputPath,
  TH1*  hist,
  Int_t histColor = cmsBlack,
  Int_t histMarker = 20
) {
  TCanvas* canvas;
  canvas = new TCanvas("canvas", "canvas", squareWidth, squareHeight);
  TPad* pad = (TPad*) canvas->GetPad(0);
  StyleTH1(pad, hist, "", histColor, histMarker, 1.);
  AddCMSHeader(pad);
  AddUPCHeader(pad);
  pad->Update();
  canvas->SaveAs(outputPath);
  return;
}

void SaveTH2(
  TString outputPath,
  TH2*  hist,
  bool showText = true,
  bool showError = true,
  int colorPalette = -1 //-1:kViridis, 0:cmsSunset, 1:cmsRedGrad, 2:cmsBlueGrad
) {
  TCanvas* canvas;
  canvas = new TCanvas("canvas", "canvas", squareWidthTH2, squareHeightTH2);
  TPad* pad = (TPad*) canvas->GetPad(0);
  StyleTH2(pad, hist, "", showText, showError, colorPalette, 1.);
  AddCMSHeader(pad, "", false);
  AddUPCHeader(pad);
  pad->Update();
  canvas->SaveAs(outputPath);
  return;
}
