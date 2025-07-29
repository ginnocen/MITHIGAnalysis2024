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
//#include "helpMessage.h"
#include "parameter.h" // Parameters for the analysis
#include "utilities.h" // Utility functions for the analysis

//#include "eventSelectionCorrection.h"

// Define binnings

const Int_t nPtBins = 37;
const Double_t pTBins_fine[nPtBins + 1] = {0.5,  0.6,  0.7, 0.8,  0.9,  1,    1.1,   1.2,   1.4, 1.6,  1.8,  2,  2.2,
                                           2.4,  3.2,  4,   4.8,  5.6,  6.4,  7.2,   9.6,   12,  14.4, 19.2, 24, 28.8,
                                           35.2, 41.6, 48,  60.8, 73.6, 86.4, 103.6, 120.8, 140, 165,  250,  400};

const Int_t nPtBins_log = 26;
const Double_t pTBins_log[nPtBins_log + 1] = {0.5,   0.603, 0.728, 0.879, 1.062, 1.284, 1.553, 1.878, 2.272,
                                              2.749, 3.327, 4.027, 4.872, 5.891, 7.117, 8.591, 10.36, 12.48,
                                              15.03, 18.08, 21.73, 26.08, 31.28, 37.48, 44.89, 53.73, 64.31};

bool checkError(const Parameters &par) { return false; }

//============================================================//
// Data analyzer class
//============================================================//

class DataAnalyzer {
public:
  TFile *inf, *outf;
  TH1D *hTrkPt, *hTrkEta, *hTrkPtUnweighted, *hTrkEtaUnweighted;
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

    // event loop
    for (unsigned long i = 0; i < nEntry; i++) {
      MChargedHadronRAA->GetEntry(i);
      if (!par.HideProgressBar && i % 1000 == 0) {
        Bar.Update(i);
        Bar.Print();
      }


      //check trigger
      if ( par.CollisionType == 0 /*&& MChargedHadronRAA->HLT_OxyZeroBias_v1 == false*/ ) continue;
      if ( par.CollisionType && par.TriggerChoice == 0 && MChargedHadronRAA->HLT_OxyZeroBias_v1 == false ) continue;
      if ( par.CollisionType && par.TriggerChoice == 1 && MChargedHadronRAA->HLT_MinimumBiasHF_OR_BptxAND_v1 == false ) continue;

	     
      //event selection, only for OO/NeNe
      if (par.CollisionType){
      if ( !(MChargedHadronRAA->passBaselineEventSelection)) continue; //all events have to pass this
      if ( par.ApplyEventSelection == 0 && !(MChargedHadronRAA->passHFAND_10_Offline) ) continue;
      if ( par.ApplyEventSelection == 1 && !(MChargedHadronRAA->passHFAND_13_Offline) ) continue; 
      if ( par.ApplyEventSelection == 2 && !(MChargedHadronRAA->passHFAND_19_Offline) ) continue;
      }

      float evtWeight = 1.0;
      if ( par.CollisionType == true && par.ApplyEventSelection == 0 && MChargedHadronRAA->passHFAND_10_Offline ) evtWeight *= 1.;//TODO, needs to be updated with Abraham's weights
      if ( par.CollisionType == true && par.ApplyEventSelection == 1 && MChargedHadronRAA->passHFAND_13_Offline ) evtWeight *= 1.;//TODO, needs to be updated with Abraham's weights
      if ( par.CollisionType == true && par.ApplyEventSelection == 2 && MChargedHadronRAA->passHFAND_19_Offline ) evtWeight *= 1.;//TODO, needs to be updated with Abraham's weights

      // track loop
      for (unsigned long j = 0; j < MChargedHadronRAA->trkPt->size(); j++) {

        // get track selection option
        float trkWeight = 0.0; //assume weight 0, i.e., the track only has nonzero weight if it satisfies the track selection
        if (par.UseTrackWeight) {
          if (par.TrackSelectionOption == 1 && MChargedHadronRAA->trkPassChargedHadron_Loose->at(j) )
            trkWeight = MChargedHadronRAA->trackingEfficiency_Loose->at(j); //nonzero weight
          else if (par.TrackSelectionOption == 2 && MChargedHadronRAA->trkPassChargedHadron_Nominal->at(j) )
            trkWeight = MChargedHadronRAA->trackingEfficiency_Nominal->at(j); //nonzero weight
          else if (par.TrackSelectionOption == 3 && MChargedHadronRAA->trkPassChargedHadron_Tight->at(j))
            trkWeight = MChargedHadronRAA->trackingEfficiency_Tight->at(j); //nonzero weight
        }

        //if (!trackSelection(MChargedHadronRAA, j, par, hNTrkPassCuts))
        //  continue;

        // eta hist before applying eta cut

        hTrkEta->Fill(MChargedHadronRAA->trkEta->at(j), trkWeight*evtWeight);
        hTrkEtaUnweighted->Fill(MChargedHadronRAA->trkEta->at(j) );

        // apply eta cut (last track selection)
        if (fabs(MChargedHadronRAA->trkEta->at(j)) > 1.0)
          continue;

        // fill dN/dpT
        hTrkPt->Fill(MChargedHadronRAA->trkPt->at(j), trkWeight*evtWeight);
        hTrkPtUnweighted->Fill(MChargedHadronRAA->trkPt->at(j) );

      } // end of track loop
    } // end of event loop

  } // end of analyze

  void writeHistograms(TFile *outf) {
    outf->cd();
    smartWrite(hTrkPt);
    smartWrite(hTrkPtUnweighted);
    smartWrite(hTrkEta);
    smartWrite(hTrkEtaUnweighted);
  }

private:
  void deleteHistograms() {
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

  //if (printHelpMessage(argc, argv))
  //  return 0;

  CommandLine CL(argc, argv);
  bool IsData = CL.GetBool("IsData", true);          // Determines whether the analysis is being run on actual data.
  int TriggerChoice = CL.GetInt("TriggerChoice", 0); // Flag indication choice of trigger
  float scaleFactor = CL.GetDouble("ScaleFactor", 1.0); // Fraction of the total number of events to be processed

  Parameters par(TriggerChoice, IsData, scaleFactor);
  par.input = CL.Get("Input", "input.root");    // Input file
  par.output = CL.Get("Output", "output.root"); // Output file
  par.CollisionType = CL.GetBool("CollisionSystem", "OO");  // Flag to indicate if the analysis is for Proton-Proton collisions, false for PP, true for OO/NeNe
  par.UseTrackWeight = CL.GetBool("UseTrackWeight", false);
  par.UseEventWeight = CL.GetBool("UseEventWeight", false);
  par.ApplyEventSelection = CL.GetInt("EventSelectionOption", 0);
  par.EventCorrectionFile = CL.Get("EventCorrectionFile", ""); // File containing event correction factors
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
