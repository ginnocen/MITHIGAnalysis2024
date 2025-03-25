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
#include "Messenger.h"   // Yi's Messengers for reading data files
#include "CommandLine.h" // Yi's Commandline bundle
#include "HistoHelper.h"

using namespace std;

/* + Description:
 *    Macro to do the gen-level closure test after reweighting
 */

TH1D* FlipHistogram(const TH1D* h)
{
  int nBins = h->GetNbinsX();
  double xMin = h->GetBinLowEdge(1);
  double xMax = h->GetBinLowEdge(nBins + 1);

  TH1D* hFlipped = new TH1D("hFlipped", h->GetTitle(), nBins, xMin, xMax);

  for (int i = 1; i <= nBins; ++i)
  {
    hFlipped->SetBinContent(i, h->GetBinContent(nBins - i + 1));
    hFlipped->SetBinError(i, h->GetBinError(nBins - i + 1));
  }

  return hFlipped;
}


int main(int argc, char *argv[])
{
  CommandLine CL(argc, argv);
  string WeightFileName     = CL.Get      ("WeightFileName", "Weights/testWeight.root");
  string UnweightedFileName = CL.Get      ("UnweightedFileName", "/data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/SkimsMC/20250213_v4_Pthat0_ForcedD0DecayD0Filtered_BeamA/mergedfile.root");
  string TreeName           = CL.Get      ("TreeName", "Tree");
  bool IsGammaN             = CL.GetBool  ("IsGammaN", true);


  TFile *fUnweighted = TFile::Open(UnweightedFileName.c_str(), "READ");
  if (!fUnweighted || fUnweighted->IsZombie())
  {
    std::cerr << "Error: Could not open unweighted file!" << std::endl;
    return 1;
  }

  DzeroUPCTreeMessenger *MDzeroUPC = new DzeroUPCTreeMessenger(*fUnweighted, TreeName);

  WeightHandler2D GptGyWH;
  GptGyWH.LoadFromFile(WeightFileName);

  // Create histogram by using the same histogram binnings
  TH1D *h_unweighted_leadingGpt  = (TH1D*) ((TH2D*) GptGyWH.h_num->Clone())->ProjectionX("h_unweighted_leadingGpt");  h_unweighted_leadingGpt->Reset();
  TH1D *h_weighted_leadingGpt    = (TH1D*) ((TH2D*) GptGyWH.h_num->Clone())->ProjectionX("h_weighted_leadingGpt");    h_weighted_leadingGpt->Reset();
  TH1D *h_target_leadingGpt      = (TH1D*) ((TH2D*) GptGyWH.h_num->Clone())->ProjectionX("h_target_leadingGpt");
  h_unweighted_leadingGpt->GetXaxis()->SetTitle("D^{0} p_{T}^{gen}");
  h_weighted_leadingGpt->GetXaxis()->SetTitle("D^{0} p_{T}^{gen}");
  h_target_leadingGpt->GetXaxis()->SetTitle("D^{0} p_{T}^{gen}");

  TH1D *h_unweighted_leadingGy  = (TH1D*) ((TH2D*) GptGyWH.h_num->Clone())->ProjectionY("h_unweighted_leadingGy");   h_unweighted_leadingGy->Reset();
  TH1D *h_weighted_leadingGy    = (TH1D*) ((TH2D*) GptGyWH.h_num->Clone())->ProjectionY("h_weighted_leadingGy");     h_weighted_leadingGy->Reset();
  TH1D *h_target_leadingGy      = (TH1D*) ((TH2D*) GptGyWH.h_num->Clone())->ProjectionY("h_target_leadingGy");
  h_unweighted_leadingGy->GetXaxis()->SetTitle("D^{0} y^{gen}");
  h_weighted_leadingGy->GetXaxis()->SetTitle("D^{0} y^{gen}");
  h_target_leadingGy->GetXaxis()->SetTitle("D^{0} y^{gen}");
  if (!IsGammaN) h_target_leadingGy = FlipHistogram(h_target_leadingGy);

  TH1D *h_unweighted_leadingGyIn2to5  = (TH1D*) ((TH2D*) GptGyWH.h_num->Clone())->ProjectionY("h_unweighted_leadingGyIn2to5",
                                                         GptGyWH.h_num->GetXaxis()->FindBin(2.0+0.001),
                                                         GptGyWH.h_num->GetXaxis()->FindBin(5.0-0.001));   h_unweighted_leadingGyIn2to5->Reset();
  TH1D *h_weighted_leadingGyIn2to5    = (TH1D*) ((TH2D*) GptGyWH.h_num->Clone())->ProjectionY("h_weighted_leadingGyIn2to5",
                                                         GptGyWH.h_num->GetXaxis()->FindBin(2.0+0.001),
                                                         GptGyWH.h_num->GetXaxis()->FindBin(5.0-0.001));   h_weighted_leadingGyIn2to5->Reset();
  TH1D *h_target_leadingGyIn2to5      = (TH1D*) ((TH2D*) GptGyWH.h_num->Clone())->ProjectionY("h_target_leadingGyIn2to5",
                                                         GptGyWH.h_num->GetXaxis()->FindBin(2.0+0.001),
                                                         GptGyWH.h_num->GetXaxis()->FindBin(5.0-0.001));
  h_unweighted_leadingGyIn2to5->GetXaxis()->SetTitle("D^{0} y^{gen}");
  h_weighted_leadingGyIn2to5->GetXaxis()->SetTitle("D^{0} y^{gen}");
  h_target_leadingGyIn2to5->GetXaxis()->SetTitle("D^{0} y^{gen}");
  if (!IsGammaN) h_target_leadingGyIn2to5 = FlipHistogram(h_target_leadingGyIn2to5);

  unsigned long nEntries = MDzeroUPC->GetEntries();
  for (unsigned long i = 0; i < nEntries; i++)
  {
    MDzeroUPC->GetEntry(i);
    if(i % 1000 == 0) std::cout << "\r event processed : "<< i << "/" << nEntries << std::flush;

    double leadingGpt = -999.;
    double leadingGy  = -999.;
    for (unsigned long j = 0; j < MDzeroUPC->Gpt->size(); j++)
    {
      if (MDzeroUPC->GisSignalCalc->at(j) == false)
        continue;
      if (MDzeroUPC->Gpt->at(j) > leadingGpt)
      {
        leadingGpt = MDzeroUPC->Gpt->at(j);
        leadingGy  = MDzeroUPC->Gy ->at(j);
      }
    }


    if (leadingGpt >= GptGyWH.h_num_norm->GetXaxis()->GetXmin() &&
        leadingGpt <= GptGyWH.h_num_norm->GetXaxis()->GetXmax() &&
        leadingGy  >= GptGyWH.h_num_norm->GetYaxis()->GetXmin() &&
        leadingGy  <= GptGyWH.h_num_norm->GetYaxis()->GetXmax() )
    {
      double GptGyWeight = GptGyWH.GetWeight(leadingGpt, leadingGy,
                                            (!IsGammaN));
      h_unweighted_leadingGpt  ->Fill(leadingGpt);
      h_weighted_leadingGpt    ->Fill(leadingGpt, GptGyWeight);
      h_unweighted_leadingGy   ->Fill(leadingGy);
      h_weighted_leadingGy     ->Fill(leadingGy,  GptGyWeight);

      if (leadingGpt >= 2 && leadingGpt <= 5)
      {
        h_unweighted_leadingGyIn2to5   ->Fill(leadingGy);
        h_weighted_leadingGyIn2to5     ->Fill(leadingGy,  GptGyWeight);
      }
    }
  }

  h_unweighted_leadingGpt->Scale(1/h_unweighted_leadingGpt->Integral());
  h_weighted_leadingGpt->Scale(1/h_weighted_leadingGpt->Integral());
  h_target_leadingGpt->Scale(1/h_target_leadingGpt->Integral());

  h_unweighted_leadingGy->Scale(1/h_unweighted_leadingGy->Integral());
  h_weighted_leadingGy->Scale(1/h_weighted_leadingGy->Integral());
  h_target_leadingGy->Scale(1/h_target_leadingGy->Integral());

  h_unweighted_leadingGyIn2to5->Scale(1/h_unweighted_leadingGyIn2to5->Integral());
  h_weighted_leadingGyIn2to5->Scale(1/h_weighted_leadingGyIn2to5->Integral());
  h_target_leadingGyIn2to5->Scale(1/h_target_leadingGyIn2to5->Integral());

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

  plotClosure(h_unweighted_leadingGpt, h_weighted_leadingGpt,
              h_target_leadingGpt, false, 
              Form("img/ClosureTest_IsGammaN%d_leadingGpt.pdf", (int) IsGammaN));

  plotClosure(h_unweighted_leadingGpt, h_weighted_leadingGpt,
              h_target_leadingGpt, true, 
              Form("img/ClosureTest_IsGammaN%d_leadingGpt_logy.pdf", (int) IsGammaN));

  plotClosure(h_unweighted_leadingGy, h_weighted_leadingGy,
              h_target_leadingGy, false, 
              Form("img/ClosureTest_IsGammaN%d_leadingGy.pdf", (int) IsGammaN));

  plotClosure(h_unweighted_leadingGyIn2to5, h_weighted_leadingGyIn2to5,
              h_target_leadingGyIn2to5, false, 
              Form("img/ClosureTest_IsGammaN%d_leadingGyIn2to5.pdf", (int) IsGammaN));

  delete MDzeroUPC;

	return 0;
}


