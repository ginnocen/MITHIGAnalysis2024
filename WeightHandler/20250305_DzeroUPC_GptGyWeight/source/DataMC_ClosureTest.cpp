#include <string>
#include <iostream>
#include <algorithm>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLegend.h"
#include "TLine.h"
#include "TStyle.h"


#include "WeightHandler2D.h"
#include "CommandLine.h" // Yi's Commandline bundle
#include "HistoHelper.h"

using namespace std;

/* + Description:
 *    Macro to do the Gpt,Gy reweight (2D) comparisons (btw MC & Data)
 */

int main(int argc, char *argv[])
{
  CommandLine CL(argc, argv);
  string WeightFileName     = CL.Get      ("WeightFileName", "Weights/testWeight.root");
  string UnweightedFileName = CL.Get      ("UnweightedFileName", "MC.root");
  string TargetFileName     = CL.Get      ("TargetFileName", "Data.root");
  string TreeName           = CL.Get      ("TreeName", "ntReweighting");
  string PlotDir            = CL.Get      ("PlotDir", "");
  bool IsGammaN             = CL.GetBool  ("IsGammaN", true);


  TFile *fUnweighted = TFile::Open(UnweightedFileName.c_str(), "READ");
  if (!fUnweighted || fUnweighted->IsZombie())
  {
    std::cerr << "Error: Could not open unweighted file!" << std::endl;
    return 1;
  }
  
  TTree *tUnweighted = (TTree*) fUnweighted->Get(TreeName.c_str());
  if (!tUnweighted)
  {
    std::cerr << "Error: Could not retrieve tree!" << std::endl;
    return 1;
  }

  std::vector<float> *Dpt = nullptr;
  std::vector<float> *Dy = nullptr;
  std::vector<float> *DCA = nullptr;
  float leadingGpt, leadingGy;
  tUnweighted->SetBranchAddress("Dpt", &Dpt);
  tUnweighted->SetBranchAddress("Dy", &Dy);
  tUnweighted->SetBranchAddress("DCA", &DCA);
  tUnweighted->SetBranchAddress("leadingGpt", &leadingGpt);
  tUnweighted->SetBranchAddress("leadingGy", &leadingGy);

  TFile *fTarget = TFile::Open(TargetFileName.c_str(), "READ");
  if (!fTarget || fTarget->IsZombie())
  {
    std::cerr << "Error: Could not open Target file!" << std::endl;
    return 1;
  }

  TTree *tTarget = (TTree*) fTarget->Get(TreeName.c_str());
  if (!tTarget)
  {
    std::cerr << "Error: Could not retrieve tree!" << std::endl;
    return 1;
  }

  tTarget->SetBranchAddress("Dpt", &Dpt);
  tTarget->SetBranchAddress("Dy", &Dy);
  tTarget->SetBranchAddress("DCA", &DCA);

  WeightHandler2D GptGyWH;
  GptGyWH.LoadFromFile(WeightFileName);

  // Create histogram by using the same histogram binnings
  TH1D *h_unweighted_Dpt  = new TH1D("h_unweighted_Dpt", ";D^{0} p_{T};", 40, 0, 8.0);
  TH1D *h_weighted_Dpt    = new TH1D("h_weighted_Dpt", ";D^{0} p_{T};", 40, 0, 8.0);
  TH1D *h_target_Dpt      = new TH1D("h_target_Dpt", ";D^{0} p_{T};", 40, 0, 8.0);

  TH1D *h_unweighted_Dy  = new TH1D("h_unweighted_Dy", ";D^{0} y;", 30, -3.0, 3.0);
  TH1D *h_weighted_Dy    = new TH1D("h_weighted_Dy", ";D^{0} y;", 30, -3.0, 3.0);
  TH1D *h_target_Dy      = new TH1D("h_target_Dy", ";D^{0} y;", 30, -3.0, 3.0);

  double _xBins[] = { 0.0, 0.0025, 0.005, 0.0075, 0.01, 0.015, 0.02, // 0.028,
                      0.036, // 0.045,
                      0.06 , // 0.08,
                      0.10};
  const int _nXBins = sizeof(_xBins)/sizeof(double) - 1;

  TH1D *h_unweighted_DCA  = new TH1D("h_unweighted_DCA", ";DCA;", _nXBins, _xBins);
  TH1D *h_weighted_DCA    = new TH1D("h_weighted_DCA", ";DCA;", _nXBins, _xBins);
  TH1D *h_target_DCA      = new TH1D("h_target_DCA", ";DCA;", _nXBins, _xBins);

  Long64_t nEntries = tUnweighted->GetEntries();
  for (Long64_t i = 0; i < nEntries; i++)
  {
    tUnweighted->GetEntry(i);

    double GptGyWeight = GptGyWH.GetWeight(leadingGpt, leadingGy,
                                          (!IsGammaN));

    for (int iD = 0; iD < Dpt->size(); ++iD)
    {
      h_unweighted_Dpt  ->Fill(Dpt->at(iD));
      h_weighted_Dpt    ->Fill(Dpt->at(iD), GptGyWeight);
      h_unweighted_Dy   ->Fill(Dy->at(iD));
      h_weighted_Dy     ->Fill(Dy->at(iD),  GptGyWeight);
      h_unweighted_DCA  ->Fill(DCA->at(iD));
      h_weighted_DCA    ->Fill(DCA->at(iD), GptGyWeight);
    }
  }

  nEntries = tTarget->GetEntries();
  for (Long64_t i = 0; i < nEntries; i++)
  {
    tTarget->GetEntry(i);

    for (int iD = 0; iD < Dpt->size(); ++iD)
    {
      h_target_Dpt    ->Fill(Dpt->at(iD));
      h_target_Dy     ->Fill(Dy->at(iD));
      h_target_DCA    ->Fill(DCA->at(iD));
    }
  }

  h_unweighted_Dpt->Scale(1/h_unweighted_Dpt->Integral());
  h_weighted_Dpt->Scale(1/h_weighted_Dpt->Integral());
  h_target_Dpt->Scale(1/h_target_Dpt->Integral());

  h_unweighted_Dy->Scale(1/h_unweighted_Dy->Integral());
  h_weighted_Dy->Scale(1/h_weighted_Dy->Integral());
  h_target_Dy->Scale(1/h_target_Dy->Integral());

  h_unweighted_DCA->Scale(1/h_unweighted_DCA->Integral());
  h_weighted_DCA->Scale(1/h_weighted_DCA->Integral());
  h_target_DCA->Scale(1/h_target_DCA->Integral());

  normalizeHistoBinWidth(h_unweighted_DCA);
  normalizeHistoBinWidth(h_weighted_DCA);
  normalizeHistoBinWidth(h_target_DCA);

  // Closure test
  auto plotClosure = [](TH1D * h_unweighted,
                        TH1D * h_weighted,
                        TH1D * h_target,
                        bool plotLogy, string plotName
  )
  {
    h_unweighted->SetLineStyle(1);
    h_unweighted->SetLineWidth(2);
    h_weighted->SetLineStyle(1);
    h_weighted->SetLineWidth(2);
    h_target->SetLineStyle(1);
    h_target->SetLineWidth(2);

    TH1D *h_closure     = (TH1D*) h_target->Clone("h_closure");
    h_closure->Reset();

    h_closure->Divide(h_weighted, h_target, 1.0, 1.0, "B");

    TCanvas *c = new TCanvas("c", "Closure Test", 800, 800);
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    c->SetTickx(1);
    c->SetTicky(1);

    TPad *pad1 = new TPad("pad1", "Distribution", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0);
    pad1->SetLeftMargin(0.15);
    pad1->SetRightMargin(0.05);
    if (plotLogy) pad1->SetLogy();
    pad1->SetTickx(1);
    pad1->SetTicky(1);
    pad1->Draw();
    pad1->cd();

    c->cd();
    TPad *pad2 = new TPad("pad2", "Ratio", 0, 0, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.3);
    pad2->SetLeftMargin(0.15);
    pad2->SetRightMargin(0.05);
    pad2->SetTickx(1);
    pad2->SetTicky(1);
    pad2->Draw();
    pad2->cd();

    pad1->cd();
    h_unweighted->SetLineColor(kBlue);
    h_weighted->SetLineColor(kRed);
    h_target->SetLineColor(kBlack);

    double maxVal = std::max({h_unweighted->GetMaximum(), h_weighted->GetMaximum(), h_target->GetMaximum()});
    double minVal = std::min({h_unweighted->GetMinimum(), h_weighted->GetMinimum(), h_target->GetMinimum()});
    if (plotLogy)
    {
      maxVal*=2.0;
      if (minVal>0) minVal*=2.0;
      else          minVal=1e-5;
    } else {
      maxVal*=1.2;
      minVal*=0.1;
    }

    h_unweighted->GetYaxis()->SetRangeUser(minVal,maxVal);
    h_weighted->GetYaxis()->SetRangeUser(minVal,maxVal);
    h_target->GetYaxis()->SetRangeUser(minVal,maxVal);

    h_unweighted->Draw("h");
    h_weighted->Draw("h same");
    h_target->Draw("h same");

    TLegend *leg = new TLegend(0.65, 0.65, 0.85, 0.85);
    leg->AddEntry(h_unweighted, "Unweighted", "l");
    leg->AddEntry(h_weighted, "Weighted", "l");
    leg->AddEntry(h_target, "Target", "l");
    formatLegend(leg);
    leg->Draw();

    pad2->cd();
    h_closure->GetYaxis()->SetNdivisions(505);
    h_closure->GetYaxis()->SetLabelSize(0.1);
    h_closure->GetXaxis()->SetLabelSize(0.1);
    h_closure->GetXaxis()->SetTitleSize(0.1);
    h_closure->GetXaxis()->SetTitleOffset(1.3);
    h_closure->SetLineColor(kBlack);
    h_closure->SetMinimum(0.5);
    h_closure->SetMaximum(1.5);
    h_closure->Draw("ple");

    TLine *line = new TLine(h_target->GetXaxis()->GetXmin(),1,
                            h_target->GetXaxis()->GetXmax(),1);
    line->SetLineStyle(2);
    line->SetLineColor(kGray+2);
    line->Draw("same");

    c->SaveAs(plotName.c_str());

    delete leg;
    delete pad1;
    delete pad2;
    delete c;
  };

  system(Form("mkdir -p img/%s/", PlotDir.c_str()));
  plotClosure(h_unweighted_Dpt, h_weighted_Dpt,
              h_target_Dpt, false, 
              Form("img/%s/DataMC_ClosureTest_Dpt.pdf", PlotDir.c_str()));

  plotClosure(h_unweighted_Dy, h_weighted_Dy,
              h_target_Dy, false, 
              Form("img/%s/DataMC_ClosureTest_Dy.pdf", PlotDir.c_str()));

  plotClosure(h_unweighted_DCA, h_weighted_DCA,
              h_target_DCA, true, 
              Form("img/%s/DataMC_ClosureTest_DCA.pdf", PlotDir.c_str()));

	return 0;
}


