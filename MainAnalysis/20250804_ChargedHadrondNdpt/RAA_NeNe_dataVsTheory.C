#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TColor.h"
#include "TLine.h"
#include "TGraphMultiErrors.h"
#include "TBox.h"

void RAA_NeNe_dataVsTheory(){
  float lowPtLumi = 1.;//261927.26;
  float highPtLumi = 1.;//1571563.58;

  setTDRStyle();

  //gStyle->SetErrorX(0);
  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetStatBorderSize(0);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);

  TFile * f = TFile::Open("ResultsUIC/pp_OO_raa_systematics_20250813-2.root","read");
  //get raa data
  TH1D * data = (TH1D*)f->Get("normalized_RAA");


  //load data
  TFile * fNucleusNucleus = TFile::Open("Results/NeNeOverPPRatio.root","read");
  //get raa data
  TH1D * dataNucleusNucleus = (TH1D*)fNucleusNucleus->Get("normalized_RAA_NucleusNucleus");
 
  //for testing, comment for actual data 
  //for(int i = 0; i<data->GetSize(); i++){
  // data->SetBinContent(i,356);
  // data->SetBinError(i,2.6);
  //}

  //A squared scaling
//  data->Scale(1.0/256.0);
//  data->Print("All");

//  dataNucleusNucleus->Scale(1.0/400.0);
 // dataNucleusNucleus->Print("All");
  

  //load systematics
  TH1D * raaSyst = (TH1D*)f->Get("Raa_Total_uncertainty");
  TH1D * raaSystNucleusNucleus = (TH1D*)fNucleusNucleus->Get("Raa_Total_uncertainty_NucleusNucleus");

//  raaSystNucleusNucleus->Scale(1.0/400.0);

  //set up canvas and pads
  TCanvas * canv2 = new TCanvas("canv2","canv2",800,800);
  canv2->SetBorderSize(0);
  canv2->SetLineWidth(0);
  canv2->SetLeftMargin(0.15);
  canv2->SetTopMargin(0.07);
  canv2->SetBorderSize(0);
  canv2->SetBottomMargin(0.15);
  canv2->SetLogx();

  //dummy histogram to define the frame
  TH1D * ppSpecD = new TH1D("specDummy1","",3,2,120);
  //TH1D * ppSpecD = new TH1D("specDummy1","",3,0.5,120);
  ppSpecD->GetYaxis()->SetTitle("R_{AA}");
  ppSpecD->GetYaxis()->SetTitleOffset(1.4);
  ppSpecD->GetYaxis()->SetTitleSize(0.045);
  ppSpecD->GetYaxis()->SetLabelSize(0.04);
  ppSpecD->GetYaxis()->CenterTitle();
  ppSpecD->GetYaxis()->SetLabelOffset(0.004);
 
  ppSpecD->GetYaxis()->SetRangeUser(0.4,1.6);

  ppSpecD->GetXaxis()->SetRangeUser(0.5,120);
  ppSpecD->GetXaxis()->SetTitleFont(42);
  ppSpecD->GetXaxis()->SetTitle("p_{T} (GeV)");
  ppSpecD->GetXaxis()->SetTitleSize(0.045);
  ppSpecD->GetXaxis()->SetLabelSize(0.04);
  ppSpecD->GetXaxis()->SetTitleOffset(1.2);
  ppSpecD->GetXaxis()->CenterTitle();
  ppSpecD->Draw();

  float normUncert = 0.075;
  TBox * b = new TBox(2.2,1-normUncert,2.4, 1+normUncert);
  b->SetFillStyle(1001);
  b->SetFillColor(kGray);
  b->Draw("same");

  // Create TGraphMultiErrors
  const int N = data->GetNbinsX();
  TGraphMultiErrors *gme = new TGraphMultiErrors("gme", "Converted from TH1D", N , 2);

  for (int i = 1; i <= N; ++i) {
      double x = data->GetBinCenter(i);
      double y = data->GetBinContent(i);
      //double y = 1.0;//for RAA=1 test

      double ex = data->GetBinWidth(i) / 2.0;
      //ex = 0;//disable x errors
      
      double ey = data->GetBinError(i);//for data
      //double ey = data->GetBinError(i)/data->GetBinContent(i);//for RAA=1 test
   
      //luminosity scaling
      if( x < 10.0){
        y = y/lowPtLumi;
        ey = ey/lowPtLumi;
      }else{
        y = y/highPtLumi;
        ey = ey/highPtLumi;
      } 

      //manually zero out some data points (used for blinding subsets of data)
      //if( x < 10.0){
      //  y = 0;
      //  ey = 0;
      //}else{
      //  y = y/highPtLumi;
      //  ey = ey/highPtLumi;
      //} 

      //divide by 100 and multiply by central value to get uncertainty
      double eySyst = raaSyst->GetBinContent(i)/100.0*y;

      std::cout << ex << " " << ey << std::endl;

      // Set point and errors (name, index, value, exLow, exHigh, eyLow, eyHigh)
      gme->SetPoint(i - 1, x, y);
      gme->SetPointEX(i-1,ex,ex);
      gme->SetPointEY(i-1,0,ey,ey);
      gme->SetPointEY(i-1,1,eySyst,eySyst);
  }

  //cut points above 100 and below 3 GeV
  for(int i = N-1; i>-1; i--){
  //  if(gme->GetPointX(i)>100) gme->SetPoint(i,0,0);
    if(gme->GetPointX(i)<3 || gme->GetPointX(i)>100) gme->SetPoint(i,0,0);
  }
  


  gme->SetMarkerStyle(20);
  gme->SetMarkerSize(1.3);
  gme->SetMarkerColor(kBlack);
  gme->SetLineColor(kBlack);
  gme->Print("All");
  gme->GetAttLine(0)->SetLineWidth(2);
  gme->GetAttLine(1)->SetLineColor(TColor::GetColor("#5790fc"));
  gme->GetAttFill(1)->SetFillColor(TColor::GetColor("#5790fc"));
  gme->GetAttFill(1)->SetFillStyle(1001);
  gme->Draw("PZs s=0.01 same;2 s=1");
  
  TLine * l = new TLine(2,1,120,1);
  l->SetLineStyle(2);
  l->Draw("same");
  
  gme->Draw("PZs s=0.01 same;X");



  dataNucleusNucleus->SetMarkerStyle(20);
  dataNucleusNucleus->SetMarkerSize(1.3);
  dataNucleusNucleus->SetLineWidth(2);
  dataNucleusNucleus->SetLineColor(kViolet+2);
  dataNucleusNucleus->SetMarkerColor(kViolet+2);

  raaSystNucleusNucleus->SetFillColorAlpha(kViolet-9, 0.8);
  raaSystNucleusNucleus->SetLineColor(kBlack);
  raaSystNucleusNucleus->SetMarkerSize(1.3);
  raaSystNucleusNucleus->SetMarkerStyle(20);
  raaSystNucleusNucleus->SetMarkerColor(kViolet+2);
  raaSystNucleusNucleus->SetMarkerColor(kViolet+2);
  raaSystNucleusNucleus->GetXaxis()->SetRangeUser(3,100);
  dataNucleusNucleus->GetXaxis()->SetRangeUser(3,100);


  dataNucleusNucleus->Draw("ep same");
  raaSystNucleusNucleus->Draw("e2 same");
  gme->Draw("PZs s=0.01 same;2 s=1");
  gme->Draw("PZs s=0.01 same;X");


  //drawing data
  //raa->SetMarkerStyle(8);
  //raa->SetMarkerColor(kBlack);
  //raa->SetLineWidth(2);
  //raa->SetLineColor(kBlack);
  //raa->GetXaxis()->SetRangeUser(3,100);
  //raa->Draw("p same");

    TFile* f1 = TFile::Open("../../Theory/20250819_OONeNePredictions/20250821_CUJET/MinBias_CUJET.root");
    TFile* f2 = TFile::Open("../../Theory/20250819_OONeNePredictions/20250820_Zakharov/MinBias_Zakharov.root");
    TFile* f3 = TFile::Open("../../Theory/20250819_OONeNePredictions/20250819_FaradayHorowitz/MinBias.root");

    // === Fetch graphs from MinBias.root ===
    TGraphAsymmErrors* gNeNe     = (TGraphAsymmErrors*) f1->Get("NeNeMinBias");
    TGraphAsymmErrors* gOO       = (TGraphAsymmErrors*) f1->Get("OOMinBias");

    // === Fetch graphs from MinBias_Zakharov.root ===
    TGraphAsymmErrors* gNeNe_z = (TGraphAsymmErrors*) f2->Get("NeNeMinBias");
    TGraphAsymmErrors* gOO_z   = (TGraphAsymmErrors*) f2->Get("OOMinBias");
    TGraphAsymmErrors* gNeNe_mQGP = (TGraphAsymmErrors*) f2->Get("NeNeMinBias_mQGP");
    TGraphAsymmErrors* gOO_mQGP   = (TGraphAsymmErrors*) f2->Get("OOMinBias_mQGP");

    // === Fetch graphs from MinBias_Zakharov.root ===
    TGraphAsymmErrors* gNeNeFaraday = (TGraphAsymmErrors*) f3->Get("NeNeMinBias");
    gNeNeFaraday->SetName("NeNeMinBias_Faraday"); 
    TGraphAsymmErrors* gOOFaraday   = (TGraphAsymmErrors*) f3->Get("OOMinBias");
    gOOFaraday->SetName("OOMinBias_Faraday");

  
    gOO->SetLineColor(kCyan+3);
    gOO->SetLineWidth(3);
    gOO->SetLineStyle(9);
    gOO->SetFillColorAlpha(kCyan+3, 0.3);

    gNeNe->SetLineColor(kMagenta-4); // purple/red
    gNeNe->SetLineWidth(3);
    gNeNe->SetLineStyle(9);
    gNeNe->SetFillColorAlpha(kMagenta-4, 0.3);

    // === Style MinBias_Zakharov.root graphs ===
    gOO_mQGP->SetLineColor(kAzure+8);
    gOO_mQGP->SetLineStyle(2); // dashed to distinguish
    gOO_mQGP->SetLineWidth(3);
    gOO_mQGP->SetFillColorAlpha(kAzure+8, 0.3);

    gNeNe_mQGP->SetLineColor(kPink+7);
    gNeNe_mQGP->SetLineStyle(2); // dashed
    gNeNe_mQGP->SetLineWidth(3);
    gNeNe_mQGP->SetFillColorAlpha(kPink+6, 0.3);

    gOO_z->SetLineColor(kAzure);
    gOO_z->SetLineWidth(3);
    gOO_z->SetFillColorAlpha(kAzure, 0.3);

    gNeNe_z->SetLineColor(kViolet-6);
    gNeNe_z->SetLineWidth(3);
    gNeNe_z->SetFillColorAlpha(kViolet-6, 0.3);
  
    gOOFaraday->SetLineColor(kBlue+1);
    gOOFaraday->SetLineWidth(4);
    gOOFaraday->SetFillColorAlpha(kBlue+1, 0.25);
    
   
    gNeNeFaraday->SetLineColor(kMagenta+2); // purple/red
    gNeNeFaraday->SetLineWidth(4);
    gNeNeFaraday->SetFillColorAlpha(kMagenta-9, 0.4);



    //
    //
    // CHANGE THE MODE TO CHANGE WHICH PLOTS GETS PRODUCED.
    //
    //
    int mode = 3; // 0 = faraday, 1 = CUJET, 2 = mQGP, 3 = no mQGP
    if(mode == 0){
     gOOFaraday->Draw("LE3 same");     // axis + line + filled area
     gNeNeFaraday->Draw("Le3 SAME"); // line + filled area on same plot
    }
    else if(mode == 1){
      // CUJET
      gOO->Draw("le3 SAME");     // axis + line + filled area
      gNeNe->Draw("le3 SAME"); // line + filled area on same plot
    }
    else if(mode == 2){
      gOO_mQGP->Draw("le3 SAME"); 
      gNeNe_mQGP->Draw("le3 SAME");  
    }
    else if(mode == 3){
      gOO_z->Draw("le3 SAME"); 
      gNeNe_z->Draw("le3 SAME"); 
    }
    else{
      std::cout << "Warning: Mode not recognized." << std::endl;
    }

  dataNucleusNucleus->Draw("ep same");
  gme->Draw("PZs s=0.01 same;X");
  gme->SetFillColor(TColor::GetColor("#5790fc"));
  //legends
  TLegend * specLeg = new TLegend(0.2,0.55,0.5,0.84);
  specLeg->SetTextFont(42);
  specLeg->SetTextSize(0.05);
  if(mode == 3)specLeg->SetTextSize(0.047);
  specLeg->SetFillStyle(0);
  if(mode < 2)specLeg->AddEntry((TObject*)0,"|#eta| < 1",""); 
  else specLeg->AddEntry((TObject*)0,"          | #eta | < 1",""); 
  specLeg->AddEntry(gme, "OO data (HIN-25-008)", "p f" );
  specLeg->AddEntry(raaSystNucleusNucleus, "NeNe data", "p f");
  if(mode == 0){
    specLeg->AddEntry(gOOFaraday,        "Faraday & Horowitz, OO",       "lf");
    specLeg->AddEntry(gNeNeFaraday,      "Faraday & Horowitz, NeNe",     "lf");
  }
  else if(mode == 1){
    // CUJET
    specLeg->AddEntry(gOO, "CUJET/CIBJET, OO", "lf");
    specLeg->AddEntry(gNeNe, "CUJET/CIBJET, NeNe", "lf"); 
  }
  else if(mode == 2){
   specLeg->AddEntry(gOO_mQGP,   "LCPI + mQGP, OO (Zakharov)",   "lf");
   specLeg->AddEntry(gNeNe_mQGP, "LCPI + mQGP, NeNe (Zakharov)", "lf");
  }
    else if(mode == 3){
   specLeg->AddEntry(gOO_z,   "LCPI + no mQGP, OO (Zakharov)",   "lf");
   specLeg->AddEntry(gNeNe_z, "LCPI + no mQGP, NeNe (Zakharov)", "lf");
  }
  else{
    std::cout << "Warning: Mode not recognized." << std::endl;
  }
  
  

  specLeg->SetFillStyle(0);
  specLeg->Draw("same"); 

  //int iPeriod = 0;
  lumi_sqrtS = "0.8 nb^{-1} (5.36 TeV NeNe), 1.07 pb^{-1} (5.36 TeV pp)";
  //writeExtraText = true;  
  //extraText  = "Preliminary";
  CMS_lumi( canv2, 0,11);
  
  canv2->RedrawAxis();


  
  if(mode == 0){
    canv2->SaveAs("plots/Figure_002_theory_Faraday.pdf");
    canv2->SaveAs("plots/Figure_002_theory_Faraday.png");
    canv2->SaveAs("plots/Figure_002_theory_Faraday.C");
  }
  else if(mode == 1){
    // CUJET
    canv2->SaveAs("plots/Figure_002_theory_CUJET.pdf");
    canv2->SaveAs("plots/Figure_002_theory_CUJET.png");
    canv2->SaveAs("plots/Figure_002_theory_CUJET.C");
  }
  else if(mode == 2){
    canv2->SaveAs("plots/Figure_002_theory_Zakharov.pdf");
    canv2->SaveAs("plots/Figure_002_theory_Zakharov.png");
    canv2->SaveAs("plots/Figure_002_theory_Zakharov.C");
  }
  else if(mode == 3){
    canv2->SaveAs("plots/Figure_002_theory_Zakharov_nomQGP.pdf");
    canv2->SaveAs("plots/Figure_002_theory_Zakharov_nomQGP.png");
    canv2->SaveAs("plots/Figure_002_theory_Zakharov_nomQGP.C");
  }
  else{
    std::cout << "Warning: Mode not recognized." << std::endl;
  }


}
