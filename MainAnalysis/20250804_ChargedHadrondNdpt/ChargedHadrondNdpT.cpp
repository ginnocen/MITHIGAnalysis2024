#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TTree.h>

#include <iostream>

using namespace std;
#include "CommandLine.h"
#include "Messenger.h"
#include "ProgressBar.h"
#include "parameter.h" // Parameters for the analysis
#include "utilities.h" // Utility functions for the analysis


// Define binnings

const Int_t nPtBins = 68; // Vipul's binning

const Double_t pTBins_fine[nPtBins + 1] = {
    0.5,  0.6,  0.7,  0.8,  0.9,  1.0,  1.1,  1.2,  1.3,  1.4,   1.5,   1.6,  1.7,  1.8,  1.9, 2.0,  2.2,  2.4,
    2.6,  2.8,  3.0,  3.2,  3.4,  3.6,  3.8,  4.0,  4.4,  4.8,   5.2,   5.6,  6.0,  6.4,  6.8, 7.2,  7.6,  8.0,
    8.5,  9.0,  9.5,  10.0, 11.,  12.,  13.,  14.,  15.,  16.,   17.,   18.,  19.,  20.,  21., 22.6, 24.6, 26.6,
    28.6, 32.6, 36.6, 42.6, 48.6, 54.6, 60.6, 74.0, 86.4, 103.6, 120.8, 140., 165., 250., 400.};
const Int_t nPtBins_log = 68; // Vipul's binning
const Double_t pTBins_log[nPtBins_log + 1] = {
    0.5,  0.6,  0.7,  0.8,  0.9,  1.0,  1.1,  1.2,  1.3,  1.4,   1.5,   1.6,  1.7,  1.8,  1.9, 2.0,  2.2,  2.4,
    2.6,  2.8,  3.0,  3.2,  3.4,  3.6,  3.8,  4.0,  4.4,  4.8,   5.2,   5.6,  6.0,  6.4,  6.8, 7.2,  7.6,  8.0,
    8.5,  9.0,  9.5,  10.0, 11.,  12.,  13.,  14.,  15.,  16.,   17.,   18.,  19.,  20.,  21., 22.6, 24.6, 26.6,
    28.6, 32.6, 36.6, 42.6, 48.6, 54.6, 60.6, 74.0, 86.4, 103.6, 120.8, 140., 165., 250., 400.};

bool checkError(const Parameters &par) { return false; }

void NormalizeByBinWidth(TH1* hist) {
    for (int i = 1; i <= hist->GetNbinsX(); ++i) {
        double binContent = hist->GetBinContent(i);
        double binError   = hist->GetBinError(i);
        double binWidth   = hist->GetBinWidth(i);

        // Normalize
        hist->SetBinContent(i, binContent / binWidth);
        hist->SetBinError(i, binError / binWidth);
    }
}

//============================================================//
// Data analyzer class
//============================================================//

class DataAnalyzer {
public:
  TFile *inf, *outf;
  TH1D *hTrkPt, *hTrkPtNoTrk, *hTrkPtNoEvt, *hTrkPtNoPartSpecies, *hTrkEta, *hTrkPtUnweighted, *hTrkEtaUnweighted;
  ChargedHadronRAATreeMessenger *MChargedHadronRAA;
  string title;

  DataAnalyzer(const char *filename, const char *outFilename, const char *mytitle = "")
      : inf(new TFile(filename)),
        MChargedHadronRAA(new ChargedHadronRAATreeMessenger(*inf, string("Tree"), false, false, 2)), title(mytitle),
        outf(new TFile(outFilename, "recreate")) {
    outf->cd();
  }

  ~DataAnalyzer() {
    deleteHistograms();
    inf->Close();
    outf->Close();
    delete MChargedHadronRAA;
  }

  void analyze(Parameters &par) {
    outf->cd();

    hTrkPt = new TH1D(Form("hTrkPt%s", title.c_str()), "", nPtBins_log, pTBins_log);
    hTrkPtNoEvt = new TH1D(Form("hTrkPt%sNoEvt", title.c_str()), "", nPtBins_log, pTBins_log);
    hTrkPtNoTrk = new TH1D(Form("hTrkPt%sNoTrk", title.c_str()), "", nPtBins_log, pTBins_log);
    hTrkPtNoPartSpecies = new TH1D(Form("hTrkPt%sNoPartSpecies", title.c_str()), "", nPtBins_log, pTBins_log);
    hTrkEta = new TH1D(Form("hTrkEta%s", title.c_str()), "", 50, -3.0, 3.0);

    hTrkPtUnweighted = new TH1D(Form("hTrkPt%sUnweighted", title.c_str()), "", nPtBins_log, pTBins_log);
    hTrkEtaUnweighted = new TH1D(Form("hTrkEta%sUnweighted", title.c_str()), "", 50, -3.0, 3.0);

    hTrkPt->Sumw2();
    hTrkEta->Sumw2();

    hTrkPtUnweighted->Sumw2();
    hTrkEtaUnweighted->Sumw2();

    par.printParameters();
    unsigned long nEntry = MChargedHadronRAA->GetEntries() * par.scaleFactor;
    ProgressBar Bar(cout, nEntry);
    Bar.SetStyle(1);
    cout << nEntry << endl;
    // event loop
    for (unsigned long i = 0; i < nEntry; i++) {
      MChargedHadronRAA->GetEntry(i);
      if (!par.HideProgressBar && i % 1000 == 0) {
        Bar.Update(i);
        Bar.Print();
      }

      // check trigger
      if (par.CollisionType && par.TriggerChoice == 0 && MChargedHadronRAA->HLT_OxyZeroBias_v1 == false)
        continue;
      if (par.CollisionType && par.TriggerChoice == 1 && MChargedHadronRAA->HLT_MinimumBiasHF_OR_BptxAND_v1 == false)
        continue;
      if (!(MChargedHadronRAA->passBaselineEventSelection))
        continue;

      // event selection, only for OO/NeNe
      if (par.CollisionType) {
        if (par.ApplyEventSelection == 1 && !(MChargedHadronRAA->passHFAND_10_Offline))
          continue;
        if (par.ApplyEventSelection == 1 && !(MChargedHadronRAA->passHFAND_13_Offline))
          continue;
        if (par.ApplyEventSelection == 1 && !(MChargedHadronRAA->passHFAND_19_Offline))
          continue;
      }

      float evtWeight = 1.0;
      if (par.CollisionType == true && par.ApplyEventSelection == 1 && par.EventSelectionOption == 1 &&
          MChargedHadronRAA->passHFAND_10_Offline)
        evtWeight *= MChargedHadronRAA->eventEfficiencyWeight_Loose;
      if (par.CollisionType == true && par.ApplyEventSelection == 1 && par.EventSelectionOption == 2 &&
          MChargedHadronRAA->passHFAND_13_Offline)
        evtWeight *= MChargedHadronRAA->eventEfficiencyWeight_Nominal;
      if (par.CollisionType == true && par.ApplyEventSelection == 1 && par.EventSelectionOption == 3 &&
          MChargedHadronRAA->passHFAND_19_Offline)
        evtWeight *= MChargedHadronRAA->eventEfficiencyWeight_Tight;
        
      // track loop
      for (unsigned long j = 0; j < MChargedHadronRAA->trkPt->size(); j++) {
        // get track selection option
        float trkWeight = 0.0; //assume weight 0, i.e., the track only has nonzero weight if it satisfies the track selection below

	    if (par.TrackSelectionOption == 1 && MChargedHadronRAA->trkPassChargedHadron_Loose->at(j) == false) continue;
	    if (par.TrackSelectionOption == 2 && MChargedHadronRAA->trkPassChargedHadron_Nominal->at(j) == false) continue;
	    if (par.TrackSelectionOption == 3 && MChargedHadronRAA->trkPassChargedHadron_Tight->at(j) == false) continue;

	    if (par.UseTrackWeight)
	    {
          if (par.TrackSelectionOption == 1 ) trkWeight = MChargedHadronRAA->trackingEfficiency_Loose->at(j); 
          if (par.TrackSelectionOption == 2 ) trkWeight = MChargedHadronRAA->trackingEfficiency_Nominal->at(j); 
          if (par.TrackSelectionOption == 3 ) trkWeight = MChargedHadronRAA->trackingEfficiency_Tight->at(j); 
        }

        float partSpeciesWeight = 1.;

        if (par.UseSpeciesWeight && MChargedHadronRAA->trkPt->at(j) < 20.0) {
          if (par.SpeciesCorrectionOption == 1)
            partSpeciesWeight = MChargedHadronRAA->TrkSpeciesWeight_pp->at(j); // ppRef
          else if (par.SpeciesCorrectionOption == 2)
            partSpeciesWeight = MChargedHadronRAA->TrkSpeciesWeight_dNdEta40->at(j); // default in OO
          else if (par.SpeciesCorrectionOption == 3)
            partSpeciesWeight = MChargedHadronRAA->TrkSpeciesWeight_dNdEta100->at(j); //  variation, central OO-like
        }

        // eta hist before applying eta cut
        hTrkEta->Fill(MChargedHadronRAA->trkEta->at(j), trkWeight * evtWeight * partSpeciesWeight);
        hTrkEtaUnweighted->Fill(MChargedHadronRAA->trkEta->at(j));
        // apply eta cut (last track selection)
        if (fabs(MChargedHadronRAA->trkEta->at(j)) > 1.0)
          continue;

        // fill dN/dpT
        double pT = MChargedHadronRAA->trkPt->at(j);
        if (par.CollisionType == false)
          evtWeight = 1.0; // event weight = 1 for ppRef, placeholder.
        hTrkPtNoEvt->Fill(MChargedHadronRAA->trkPt->at(j), evtWeight / pT);
        hTrkPtNoTrk->Fill(MChargedHadronRAA->trkPt->at(j), trkWeight / pT);
        hTrkPtNoPartSpecies->Fill(MChargedHadronRAA->trkPt->at(j), partSpeciesWeight / pT);
        hTrkPt->Fill(MChargedHadronRAA->trkPt->at(j), trkWeight * evtWeight * partSpeciesWeight / pT);
        hTrkPtUnweighted->Fill(MChargedHadronRAA->trkPt->at(j), 1. / pT);

      } // end of track loop
    } // end of event loop

  } // end of analyze

  void writeHistograms(TFile *outf) {
    outf->cd();

    hTrkPtNoEvt->Scale(1.0 / (4 * TMath::Pi()));
    hTrkPtNoTrk->Scale(1.0 / (4 * TMath::Pi()));
    hTrkPtNoPartSpecies->Scale(1.0 / (4 * TMath::Pi()));
    hTrkPt->Scale(1.0 / (4 * TMath::Pi()));
    hTrkPtUnweighted->Scale(1.0 / (4 * TMath::Pi()));
	  
    NormalizeByBinWidth(hTrkPtNoEvt);
    NormalizeByBinWidth(hTrkPtNoTrk);
    NormalizeByBinWidth(hTrkPtNoPartSpecies);
    NormalizeByBinWidth(hTrkPt);
    NormalizeByBinWidth(hTrkPtUnweighted);
    NormalizeByBinWidth(hTrkEta);
    NormalizeByBinWidth(hTrkEtaUnweighted);


    smartWrite(hTrkPtNoEvt);
    smartWrite(hTrkPtNoTrk);
    smartWrite(hTrkPtNoPartSpecies);
    smartWrite(hTrkPt);
    smartWrite(hTrkPtUnweighted);
    smartWrite(hTrkEta);
    smartWrite(hTrkEtaUnweighted);
  }

private:
  void deleteHistograms() {
    delete hTrkPtNoEvt;
    delete hTrkPtNoTrk;
    delete hTrkPtNoPartSpecies;
    delete hTrkPt;
    delete hTrkEta;
    delete hTrkPtUnweighted;
    delete hTrkEtaUnweighted;
  }
};

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {

  // if (printHelpMessage(argc, argv))
  //   return 0;

  CommandLine CL(argc, argv);     
  int TriggerChoice = CL.GetInt("TriggerChoice");  // Flag indication choice of trigger
  float scaleFactor = CL.GetDouble("ScaleFactor"); // Fraction of the total number of events to be processed

  Parameters par(TriggerChoice, scaleFactor);
  par.input = CL.Get("Input", "input.root");         // Input file
  par.output = CL.Get("Output", "output.root");      // Output file
  par.CollisionType = CL.GetBool("CollisionSystem"); // Flag to indicate if the analysis is for Proton-Proton
                                                     // collisions, false for PP, true for OO/NeNe
  par.ApplyEventSelection = CL.GetBool("ApplyEventSelection"); 
  par.UseSpeciesWeight = CL.GetBool("UseSpeciesWeight");
  par.UseTrackWeight = CL.GetBool("UseTrackWeight");
  par.UseEventWeight = CL.GetBool("UseEventWeight");
  par.EventSelectionOption = CL.GetInt("EventSelectionOption"); 
  par.TrackSelectionOption = CL.GetInt("TrackWeightSelection");
  par.SpeciesCorrectionOption = CL.GetInt("SpeciesCorrectionOption");
  par.HideProgressBar = CL.GetBool("HideProgressBar", false);

  if (checkError(par))
    return -1;
  std::cout << "Parameters are set" << std::endl;

  // Analyze Data
  DataAnalyzer analyzer(par.input.c_str(), par.output.c_str(), "");
  analyzer.analyze(par);
  analyzer.writeHistograms(analyzer.outf);
  saveParametersToHistograms(par, analyzer.outf);
  cout << "done!" << analyzer.outf->GetName() << endl;

  return 0;
}
