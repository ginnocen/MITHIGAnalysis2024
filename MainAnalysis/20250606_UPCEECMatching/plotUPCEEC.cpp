#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TGraphAsymmErrors.h>
#include <TLine.h>
#include <TF1.h>
#include <TProfile.h>
#include <TGaxis.h>

#include <iostream>
#include <string>

using namespace std;
#include "CommandLine.h" // Yi's Commandline bundle
#include "Messenger.h"   // Yi's Messengers for reading data files
#include "SetStyle.h"
#include "ProgressBar.h" // Yi's fish progress bar
#include "helpMessage.h" // Print out help message
#include "parameter.h"   // The parameters used in the analysis
#include "utilities.h"   // Yen-Jie's random utility functions


void DivideByBin(TH1D* H, double Bins[]); 
void MakeCanvas(vector<TH1D *> Histograms, vector<string> Labels, string Output,
   string X, string Y, double WorldMin, double WorldMax, bool DoRatio, bool LogX, double minHardScale, double maxHardScale, bool UsePtSumHardScale, double minTrackPT, double yMax); 
  
void SetPad(TPad &P); 

void plotHFJetPerf(std::vector<string> input, string tag, int pthatmin){

    // input files
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    
        
    // for the double log version, copy the theta binning from the e+e- analysis
    const int BinCount = 100;
    double Bins[2*BinCount+1];
    double BinMin = 0.002;
    double BinMax = M_PI / 2;
    
    for(int i = 0; i <= BinCount; i++){
      // theta double log binning
      Bins[i] = exp(log(BinMin) + (log(BinMax) - log(BinMin)) / BinCount * i);
      Bins[2*BinCount-i] = BinMax * 2 - exp(log(BinMin) + (log(BinMax) - log(BinMin)) / BinCount * i);
    }


    // open the files
    TFile* inputFile = TFile::Open(input.at(0).c_str());
    inputFile->ls();
    
    TDirectoryFile* paramDirectory = (TDirectoryFile*)inputFile->Get("par");
    TH1D* hTrigChoice = (TH1D*)paramDirectory->Get("parTriggerChoice");
    TH1D* hMinTrackPt = (TH1D*)paramDirectory->Get("parMinTrackPT");
    TH1D* hMaxTrackPt = (TH1D*)paramDirectory->Get("parMaxTrackPT");
    TH1D* hMaxTrackY = (TH1D*)paramDirectory->Get("parMaxTrackY");
    TH1D* hMinHardScale = (TH1D*)paramDirectory->Get("parMinHardScale");
    TH1D* hMaxHardScale = (TH1D*)paramDirectory->Get("parMaxHardScale");
    TH1D* hUsePtSumHardScale = (TH1D*)paramDirectory->Get("parUsePtSumHardScale");
    TH1D* hIsData = (TH1D*)paramDirectory->Get("parIsData");
    
    TFile* inputFileMC = TFile::Open(input.at(1).c_str());
    inputFileMC->ls();

    static vector<int> Colors = GetCVDColors6();


    //--------------------------------
    // loop over all of the input files
    //-------------------------------

    // get the histograms
    TH1D* hNev = (TH1D*)inputFile->Get("hNev");
    hNev->SetName(Form("hNev_%d", 0));
    int nEventsData = hNev->GetBinContent(1);

    TH1D* hNevMC = (TH1D*)inputFileMC->Get("hNev");
    hNevMC->SetName(Form("hNevMC_%d", 0));
    int nEventsMC= hNevMC->GetBinContent(1);
    
    
    TH1D* hEECData = (TH1D*)inputFile->Get("hEEC"); 
    hEECData->SetName("hEECData"); 
    
    TH1D* hEECMC = (TH1D*)inputFileMC->Get("hEEC"); 
    hEECMC->SetName("hEECMC"); 
    
    hEECData->SetMarkerStyle(20); 
    hEECData->SetMarkerColor(kBlack); 
    hEECData->SetLineColor(kBlack); 
    
    hEECMC->SetMarkerStyle(20); 
    hEECMC->SetMarkerColor(kAzure);
    hEECMC->SetLineColor(kAzure); 
    
    std::cout << "nEventsData: " << nEventsData << " nEventsMC: " << nEventsMC << std::endl;
    

    
    DivideByBin(hEECData, Bins); 
    DivideByBin(hEECMC, Bins); 
    
    hEECData->Scale(1./nEventsData); 
    hEECMC->Scale(1./nEventsMC); 
    

    // handle the trigger selection with a string we then pass to the legend
    TLatex* TrigString;
    if(hTrigChoice->GetBinContent(1) == 0)TrigString = new TLatex(0.62,0.80,"No Trigger Selection");
    else if(hTrigChoice->GetBinContent(1) == 1)TrigString = new TLatex(0.65,0.78,"Data, ZDC 1n OR");
    else if(hTrigChoice->GetBinContent(1) == 2)TrigString = new TLatex(0.65,0.80,"ZDC XOR + Jet 8");
    else{
      std::cout << "[Error]: Unrecognized trigger option " << hTrigChoice->GetBinContent(1) << std::endl;
      return;
    }
    
 
    
   // draw the canvas
    MakeCanvas({hEECMC, hEECData}, {"MC", TrigString->GetTitle()} , Form("DoubleLogFirstTry_%s", tag.c_str()),
    "#theta_{L}", "#frac{1}{N_{event}} #frac{d(#sum E_{i}E_{j}/#sum p_{T}^{2})}{d #theta_{L}}", 2e-3, 1e2, true, true, hMinHardScale->GetBinContent(1), hMaxHardScale->GetBinContent(1), hUsePtSumHardScale->GetBinContent(1), hMinTrackPt->GetBinContent(1), hMaxTrackY->GetBinContent(1));
   
    
   // now investigate also the hard scale
   TLatex* cms; 
   if(hIsData->GetBinContent(1) == 0) cms = new TLatex(0.12,0.92,"#bf{CMS} #it{Simulation} PYTHIA8 #gammaN (5.36 TeV)");
   else cms = new TLatex(0.12,0.92,"#bf{CMS} UPC PbPb (5.36 TeV)");
   cms->SetNDC();
   cms->SetTextSize(0.05);
   cms->SetTextFont(42);

   TH1D* hHardScaleData = (TH1D*)inputFile->Get("hHardScale"); 
   hHardScaleData->SetName("hHardScaleData"); 

   TH1D* hHardScaleMC = (TH1D*)inputFileMC->Get("hHardScale"); 
   hHardScaleMC->SetName("hHardScaleMC"); 
    
   // check the hard scale
   TCanvas* c4 = new TCanvas("c4", "c4", 600, 600);
   c4->SetTickx(1);
   c4->SetTicky(1);
   c4->SetLogy();
   c4->SetRightMargin(0.05);
   c4->SetLeftMargin(0.13);
   c4->SetBottomMargin(0.15);
   
   TLegend* legDataMC = new TLegend(0.56, 0.75, 0.7, .85);
   legDataMC->SetBorderSize(0);
   legDataMC->SetFillStyle(0);
   legDataMC->SetTextFont(42);
   legDataMC->SetTextSize(0.035);
   
   legDataMC->AddEntry(hHardScaleData, TrigString->GetTitle()); 
   legDataMC->AddEntry(hHardScaleMC, Form("Monte Carlo, #hat{#it{p}}_{T, min} = %d", pthatmin)); 
       
   hHardScaleData->Scale(1.0/nEventsData, "width"); 
   hHardScaleData->SetMarkerColor(Colors[0]); 
   hHardScaleData->SetLineColor(Colors[0]); 
   hHardScaleData->SetLineWidth(2); 
   hHardScaleData->SetMarkerStyle(20); 
   
   hHardScaleMC->Scale(1.0/nEventsMC, "width"); 
   hHardScaleMC->SetMarkerColor(Colors[1]); 
   hHardScaleMC->SetLineColor(Colors[1]); 
   hHardScaleMC->SetLineWidth(2); 
   hHardScaleMC->SetMarkerStyle(20); 
   
   hHardScaleData->GetXaxis()->SetTitle("#sum #it{p}_{T} (GeV)"); 
   hHardScaleData->GetXaxis()->SetTitleOffset(1.4); 
   hHardScaleData->GetYaxis()->SetTitle("1/N_{event} and Bin Width Norm.");
   hHardScaleData->GetYaxis()->SetRangeUser(1e-3, 10);
   std::cout << "hHardScaleData: " << hHardScaleData->Integral() << std::endl;
   hHardScaleData->Draw("same");
   hHardScaleMC->Draw("same"); 
   cms->Draw(); 
   legDataMC->Draw("same");
   c4->SaveAs(Form("HardScale_%s.pdf", tag.c_str() ));
   
   //----------------------------------------------------
   // hMult distribution
   //----------------------------------------------------

   TH1D* hMultData = (TH1D*)inputFile->Get("hMult"); 
   hMultData->SetName("hMultData"); 

   TH1D* hMultMC = (TH1D*)inputFileMC->Get("hMult"); 
   hMultMC->SetName("hMultMC"); 
    
   TCanvas* c5 = new TCanvas("c5", "c5", 600, 600);
   c5->SetTickx(1);
   c5->SetTicky(1);
   c5->SetLogy();
   c5->SetRightMargin(0.05);
   c5->SetLeftMargin(0.13);
   c5->SetBottomMargin(0.15);
   
   TLatex* VisString; 
   if(hUsePtSumHardScale->GetBinContent(1) == 1)VisString = new TLatex(0.16,0.82,Form("%0.1f < #sum #it{p}_{T} < %0.1f", hMinHardScale->GetBinContent(1), hMaxHardScale->GetBinContent(1)));
   else VisString = new TLatex(0.16,0.82,Form("%0.1f < #it{M}_{T} < %0.1f", hMinHardScale->GetBinContent(1), hMaxHardScale->GetBinContent(1)));
   VisString->SetNDC();
   VisString->SetTextSize(0.035);
   VisString->SetTextFont(42);
   
   hMultData->Scale(1.0/nEventsData, "width"); 
   hMultData->SetMarkerColor(Colors[0]); 
   hMultData->SetLineColor(Colors[0]); 
   hMultData->SetLineWidth(2); 
   hMultData->SetMarkerStyle(20); 

   hMultMC->Scale(1.0/nEventsMC, "width"); 
   hMultMC->SetMarkerColor(Colors[1]); 
   hMultMC->SetLineColor(Colors[1]); 
   hMultMC->SetLineWidth(2); 
   hMultMC->SetMarkerStyle(20); 

   hMultData->GetXaxis()->SetTitle("#it{N}_{ch}"); 
   hMultData->GetXaxis()->SetTitleOffset(1.4); 
   hMultData->GetYaxis()->SetTitle("1/N_{event} and Bin Width Norm.");
   hMultData->GetYaxis()->SetRangeUser(1e-6, 1e0);
   hMultData->Draw("same");
   hMultMC->Draw("same"); 
   VisString->Draw("same");
   cms->Draw(); 
   legDataMC->Draw("same");
   c5->SaveAs(Form("Mult_%s.pdf", tag.c_str() ));

}

void MakeCanvas(vector<TH1D *> Histograms, vector<string> Labels, string Output,
   string X, string Y, double WorldMin, double WorldMax, bool DoRatio, bool LogX, double minHardScale, double maxHardScale, bool UsePtSumHardScale, double minTrackPT, double yMax)
{
   int NLine = Histograms.size();
   int N = Histograms[0]->GetNbinsX();

   double MarginL = 180;
   double MarginR = 90;
   double MarginB = 120;
   double MarginT = 90;

   double WorldXMin = LogX ? 0 : 0;
   double WorldXMax = LogX ? N : 180;
   
   double PadWidth = 1200;
   double PadHeight = DoRatio ? 640 : 640 + 240;
   double PadRHeight = DoRatio ? 240 : 0.001;

   double CanvasWidth = MarginL + PadWidth + MarginR;
   double CanvasHeight = MarginT + PadHeight + PadRHeight + MarginB;

   MarginL = MarginL / CanvasWidth;
   MarginR = MarginR / CanvasWidth;
   MarginT = MarginT / CanvasHeight;
   MarginB = MarginB / CanvasHeight;

   PadWidth   = PadWidth / CanvasWidth;
   PadHeight  = PadHeight / CanvasHeight;
   PadRHeight = PadRHeight / CanvasHeight;

   TCanvas Canvas("Canvas", "", CanvasWidth, CanvasHeight);
   // Canvas.SetLogy();
   // Canvas.SetRightMargin(MarginR);
   // Canvas.SetLeftMargin(MarginL);
   // Canvas.SetTopMargin(MarginT);
   // Canvas.SetBottomMargin(MarginB);

   TPad Pad("Pad", "", MarginL, MarginB + PadRHeight, MarginL + PadWidth, MarginB + PadHeight + PadRHeight);
   Pad.SetLogy();
   SetPad(Pad);
   
   TPad PadR("PadR", "", MarginL, MarginB, MarginL + PadWidth, MarginB + PadRHeight);
   if(DoRatio)
      SetPad(PadR);

   Pad.cd();

   TH2D HWorld("HWorld", "", N, WorldXMin, WorldXMax, 100, WorldMin, WorldMax);
   HWorld.SetStats(0);
   HWorld.GetXaxis()->SetTickLength(0);
   HWorld.GetXaxis()->SetLabelSize(0);

   HWorld.Draw("axis");
   for(TH1D *H : Histograms){
      std::cout << "Drawing hist with integral " << H->Integral() << std::endl;
      H->Draw("same");
   }





   TGraph G;
   G.SetPoint(0, LogX ? N / 2 : 180.0/2, 0);
   G.SetPoint(1, LogX ? N / 2 : 180.0/2, 1000);
   G.SetLineStyle(kDashed);
   G.SetLineColor(kGray);
   G.SetLineWidth(1);
   G.Draw("l");

   if(DoRatio)
      PadR.cd();

   double WorldRMin = 0.6;
   double WorldRMax = 1.4;
   
   TH2D HWorldR("HWorldR", "", N, WorldXMin, WorldXMax, 100, WorldRMin, WorldRMax);
   TGraph G2;
   
   if(DoRatio)
   {
      HWorldR.SetStats(0);
      HWorldR.GetXaxis()->SetTickLength(0);
      HWorldR.GetXaxis()->SetLabelSize(0);
      HWorldR.GetYaxis()->SetNdivisions(505);

      HWorldR.Draw("axis");
      for(int i = 1; i < NLine; i++)
      {
         TH1D *H = (TH1D *)Histograms[i]->Clone();
         H->Divide(Histograms[0]);
         H->Draw("same");
      }

      G.Draw("l");

      G2.SetPoint(0, 0, 1);
      G2.SetPoint(1, 99999, 1);
      G2.Draw("l");
   }

   double xRange;  
   if(!LogX) xRange =  180.0; 
   else xRange = M_PI; 
   
   double BinMin    = 0.002;
   double BinMiddle = xRange / 2;
   double BinMax    = xRange - 0.002;

   Canvas.cd();
   TGaxis X1(MarginL, MarginB, MarginL + PadWidth / 2, MarginB, BinMin, BinMiddle, 510, "GS");
   TGaxis X2(MarginL + PadWidth, MarginB, MarginL + PadWidth / 2, MarginB, BinMin, BinMiddle, 510, "-GS");
   TGaxis X3(MarginL, MarginB + PadRHeight, MarginL + PadWidth / 2, MarginB + PadRHeight, BinMin, BinMiddle, 510, "+-GS");
   TGaxis X4(MarginL + PadWidth, MarginB + PadRHeight, MarginL + PadWidth / 2, MarginB + PadRHeight, BinMin, BinMiddle, 510, "+-GS");
   TGaxis Y1(MarginL, MarginB, MarginL, MarginB + PadRHeight, WorldRMin, WorldRMax, 505, "");
   TGaxis Y2(MarginL, MarginB + PadRHeight, MarginL, MarginB + PadRHeight + PadHeight, WorldMin, WorldMax, 510, "G");

   TGaxis XL1(MarginL, MarginB, MarginL + PadWidth, MarginB, 0, xRange, 510, "S");
   TGaxis XL2(MarginL, MarginB + PadRHeight, MarginL + PadWidth, MarginB + PadRHeight, 0, xRange, 510, "+-S");

 
   Y1.SetLabelFont(42);
   Y2.SetLabelFont(42);
   XL1.SetLabelFont(42);
   XL2.SetLabelFont(42);

   X1.SetLabelSize(0);
   X2.SetLabelSize(0);
   X3.SetLabelSize(0);
   X4.SetLabelSize(0);
   // XL1.SetLabelSize(0);
   XL2.SetLabelSize(0);

   X1.SetTickSize(0.06);
   X2.SetTickSize(0.06);
   X3.SetTickSize(0.06);
   X4.SetTickSize(0.06);
   XL1.SetTickSize(0.03);
   XL2.SetTickSize(0.03);

   if(LogX == true)
   {
      X1.Draw();
      X2.Draw();
      if(DoRatio) X3.Draw();
      if(DoRatio) X4.Draw();
   }
   if(LogX == false)
   {
      XL1.Draw();
      if(DoRatio)
         XL2.Draw();
   }
   if(DoRatio)
      Y1.Draw();
   Y2.Draw();

   TLatex Latex;
   Latex.SetNDC();
   Latex.SetTextFont(42);
   Latex.SetTextSize(0.035);
   Latex.SetTextAlign(23);
   if(LogX) Latex.DrawLatex(MarginL + (1 - MarginR - MarginL) * 0.115, MarginB - 0.01, "0.01");
   if(LogX) Latex.DrawLatex(MarginL + (1 - MarginR - MarginL) * 0.290, MarginB - 0.01, "0.1");
   if(LogX) Latex.DrawLatex(MarginL + (1 - MarginR - MarginL) * 0.465, MarginB - 0.01, "1");
   if(LogX) Latex.DrawLatex(MarginL + (1 - MarginR - MarginL) * 0.535, MarginB - 0.01, "#pi - 1");
   if(LogX) Latex.DrawLatex(MarginL + (1 - MarginR - MarginL) * 0.710, MarginB - 0.01, "#pi - 0.1");
   if(LogX) Latex.DrawLatex(MarginL + (1 - MarginR - MarginL) * 0.885, MarginB - 0.01, "#pi - 0.01");

   Latex.SetTextAlign(12);
   Latex.SetTextAngle(270);
   Latex.SetTextColor(kGray);
   if(!LogX)Latex.DrawLatex(MarginL + (1 - MarginR - MarginL) * 0.5 + 0.0175, 1 - MarginT - 0.015, "#theta_{L} = 180");
   else Latex.DrawLatex(MarginL + (1 - MarginR - MarginL) * 0.5 + 0.0175, 1 - MarginT - 0.015, "#theta_{L} = #pi/2");

   
   
   Latex.SetTextAlign(22);
   Latex.SetTextAngle(0);
   Latex.SetTextColor(kBlack);
   Latex.SetTextSize(0.03);
   Latex.DrawLatex(MarginL + PadWidth * 0.5, MarginB * 0.3, X.c_str());

   
   
   Latex.SetTextAlign(22);
   Latex.SetTextAngle(90);
   Latex.SetTextColor(kBlack);
   if(DoRatio)
      Latex.DrawLatex(MarginL * 0.3, MarginB + PadRHeight * 0.5, "Ratio");
   Latex.DrawLatex(MarginL * 0.3, MarginB + PadRHeight + PadHeight * 0.5, Y.c_str());

   Latex.SetTextAlign(11);
   Latex.SetTextAngle(0);
   Latex.SetTextSize(0.045);
   Latex.DrawLatex(MarginL, MarginB + PadRHeight + PadHeight + 0.02, "#bf{CMS}, UPC, #sqrt{s_{NN}} = 5.36 TeV, Work-in-progress");
   
   Latex.SetTextSize(0.035);
   // draw out the hard scale
   if(UsePtSumHardScale)Latex.DrawLatex(MarginL + 0.57*PadWidth, MarginB + PadRHeight + PadHeight * 0.85, Form("%0.1f < #sum #it{p}_{T} < %0.1f GeV", minHardScale, maxHardScale));
   else Latex.DrawLatex(MarginL + 0.57*PadWidth, MarginB + PadRHeight + PadHeight * 0.85, Form("%0.1f < #sum #it{M}_{T, track} < %0.1f GeV", minHardScale, maxHardScale));
   
   Latex.DrawLatex(MarginL + 0.57*PadWidth, MarginB + PadRHeight + PadHeight * 0.7, Form("#it{p}_{T, track} > %0.1f GeV, |#it{#eta}_{Track}| < %0.1f", minTrackPT, yMax));
   
   Latex.SetTextAlign(11);
   Latex.SetTextAngle(0);
   Latex.SetTextColor(19);
   Latex.SetTextSize(0.02);
   Latex.DrawLatex(0.01, 0.01, "Double Log First Look");

   TLegend Legend(0.15, 0.90, 0.35, 0.90 - 0.06 * min(NLine, 4));
   Legend.SetTextFont(42);
   Legend.SetTextSize(0.035);
   Legend.SetFillStyle(0);
   Legend.SetBorderSize(0);
   for(int i = 0; i < NLine && i < 4; i++)
      Legend.AddEntry(Histograms[i], Labels[i].c_str(), "pl");

   Legend.Draw();

   TLegend Legend2(0.7, 0.90, 0.9, 0.90 - 0.06 * (NLine - 4));
   Legend2.SetTextFont(42);
   Legend2.SetTextSize(0.035);
   Legend2.SetFillStyle(0);
   Legend2.SetBorderSize(0);
   if(NLine >= 4)
   {
      for(int i = 4; i < NLine; i++)
         Legend2.AddEntry(Histograms[i], Labels[i].c_str(), "pl");
      Legend2.Draw();
   }

   Canvas.SaveAs((Output + ".pdf").c_str());
}

void DivideByBin(TH1D* H, double Bins[])
{
   int N = H->GetNbinsX();
   for(int i = 1; i <= N; i++)
   {
      double L = Bins[i-1];
      double R = Bins[i];
      H->SetBinContent(i, H->GetBinContent(i) / (R - L));
      H->SetBinError(i, H->GetBinError(i) / (R - L));
   }
}


void SetPad(TPad &P)
{
   P.SetLeftMargin(0);
   P.SetTopMargin(0);
   P.SetRightMargin(0);
   P.SetBottomMargin(0);
   P.SetTickx();
   P.SetTicky();
   P.Draw();
}



//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {
  if (printHelpMessage(argc, argv))
    return 0;
  CommandLine CL(argc, argv);
  // input to this step is the result of running
  std::vector<string> input = CL.GetStringVector("Input", ""); // Input files
  string tag = CL.Get("tag", "test");
  int pthatmin = CL.GetInt("PtHat", 5); 
  plotHFJetPerf(input, tag, pthatmin);
}
