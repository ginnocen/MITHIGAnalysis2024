#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>

#include <iostream>

using namespace std;
#include "CommandLine.h" // Yi's Commandline bundle
#include "Messenger.h"   // Yi's Messengers for reading data files
#include "ProgressBar.h" // Yi's fish progress bar
#include "helpMessage.h" // Print out help message
#include "parameter.h"   // The parameters used in the analysis
#include "utilities.h"   // Yen-Jie's random utility functions


//======= jetSelection =====================================//
// Check if the jet pass selection criteria
// FIX Me: Will need to add actual selections when we come up with them
//============================================================//
bool jetSelection(DzeroJetUPCTreeMessenger *b, Parameters par, int j) {
  return true;
}

//======= triggerSelection =====================================//
// Here the event selection was done at the level of the skimmer
// do the trigger selection here
// could consider to do additional event selection for gammaN or Ngamma here too, but for now don't
//============================================================//
bool triggerSelection(DzeroJetUPCTreeMessenger *b, const Parameters &par) {
  if (par.IsData == false)
    return true;
  if (par.TriggerChoice == 1 && b->isL1ZDCOr == false)
    return false;
  if (par.TriggerChoice == 2 && b->isL1ZDCXORJet8 == false) return false;
  return true;
}

class DataAnalyzer {
public:
  TFile *inf, *outf;
  // histogram to keep track of the number of events
  TH1D *hNev;

  // general jet pT histograms
  TH1D* hJetPt; // inclusive jet pT spectrum
  TH1D* hD0JetPt; // d0 tagged jet pT spectrum
  TH1D* hRefJetPt; // gen level match of a reco level jet
  TH1D* hGenJetPt; // generator level jet pT spectrum
  TH1D* hD0inJetPt; // pT spectrum of D0s in the jet
  TH1D* hDeltaPhiLeadingJetvLeadingD; // Delta Phi between leading jet and D0 (should peak near 0)
  TH1D* hDeltaPhiSubleadingJetvLeadingD; // Delta Phi between sub-leading jet and leading D0 (should peak near pi)
  TH1D* hD0FragFraction; // fragmentation function for D0s in jets
  TH1D* hD0massInJet; // mass peak for the D0

  DzeroJetUPCTreeMessenger *MDzeroJetUPC;



  DataAnalyzer(const char *filename, const char *outFilename, const char *mytitle = "")
      : inf(new TFile(filename)), MDzeroJetUPC(new DzeroJetUPCTreeMessenger(*inf, string("Tree"))),
        outf(new TFile(outFilename, "recreate")) {
    outf->cd();
  }

  ~DataAnalyzer() {
    deleteHistograms();
    inf->Close();
    outf->Close();
    delete MDzeroJetUPC;
  }

  void analyze(Parameters &par) {
    outf->cd();

    hNev = new TH1D("hNev", "", 1, 0, 1);

    hJetPt = new TH1D("hJetPt", "", 100, 0, 100);
    hJetPt->Sumw2();

    hD0JetPt =  new TH1D("hD0JetPt", "", 100, 0, 100);
    hD0JetPt->Sumw2();

    hD0inJetPt = new TH1D("hD0inJetPt", "", 100, 0, 100);
    hD0inJetPt->Sumw2();

    hRefJetPt = new TH1D("hRefJetPt", "", 100, 0, 100);
    hRefJetPt->Sumw2();

    hGenJetPt = new TH1D("hGenJetPt", "", 100, 0, 100);
    hGenJetPt->Sumw2();

    hDeltaPhiLeadingJetvLeadingD = new TH1D("hDeltaPhiLeadingJetvLeadingD", "", 35, 0, 3.5);
    hDeltaPhiLeadingJetvLeadingD->Sumw2();

    hDeltaPhiSubleadingJetvLeadingD = new TH1D("hDeltaPhiSubleadingJetvLeadingD", "", 35, 0, 3.5);
    hDeltaPhiSubleadingJetvLeadingD->Sumw2();

    hD0FragFraction = new TH1D("hD0FragFraction", "", 20, 0, 1);
    hD0FragFraction->Sumw2();

    hD0massInJet = new TH1D("hDmassInJet", "", 60, 1.7, 2.0);
    hD0massInJet->Sumw2();

    par.printParameters();
    unsigned long nEntry = MDzeroJetUPC->GetEntries() * par.scaleFactor;
    std::cout << "Processing " << nEntry << " events." << std::endl;
    ProgressBar Bar(cout, nEntry);
    Bar.SetStyle(1);
    // loop over the number of events
    int numberAccEvents = 0;
    for (unsigned long i = 0; i < nEntry; i++) {
      MDzeroJetUPC->GetEntry(i);
      if (i % 1000 == 0) {
        Bar.Update(i);
        Bar.Print();
      }

      //----- Data analysis-----
      if (par.IsData && triggerSelection(MDzeroJetUPC, par)) {

        numberAccEvents++; // increment the number of events


        // jet loop
        for (unsigned long j = 0; j < MDzeroJetUPC->JetPt->size(); j++) {
          // jet acceptance cuts
          if(MDzeroJetUPC->JetPt->at(j) < par.MinJetPT || MDzeroJetUPC->JetPt->at(j) > par.MaxJetPT) continue;
          if(MDzeroJetUPC->JetY->at(j) < par.MinJetY || MDzeroJetUPC->JetY->at(j) > par.MaxJetY) continue;

          hJetPt->Fill(MDzeroJetUPC->JetPt->at(j));
          // if after looping over all of the D0s we found one within the cone, fill the jet pt
          if(MDzeroJetUPC->isD0Tagged->at(j)){
            hD0JetPt->Fill(MDzeroJetUPC->JetPt->at(j));
            double DPt =  MDzeroJetUPC->Dpt->at(MDzeroJetUPC->TaggedD0Index->at(j));
            double Dy  = MDzeroJetUPC->Dy->at(MDzeroJetUPC->TaggedD0Index->at(j));

            // make the D0 acceptance cuts
            if(DPt < par.MinDzeroPT || DPt > par.MaxDzeroPT) continue;
            if(Dy < par.MinDzeroY || Dy > par.MaxDzeroY) continue;

            double Dphi = MDzeroJetUPC->Dphi->at(MDzeroJetUPC->TaggedD0Index->at(j));
            double Dmass = MDzeroJetUPC->Dmass->at(MDzeroJetUPC->TaggedD0Index->at(j));
            hD0FragFraction->Fill(DPt/MDzeroJetUPC->JetPt->at(j));
            hD0inJetPt->Fill(DPt);
            hD0massInJet->Fill(Dmass);
          }
        } // end of jet loop

        // only fill the delta phi histograms if we have a jet and a D0
        // first case, the leading jet is a d0 tagged jet
        if(MDzeroJetUPC->isD0Tagged->size() > 0){
          bool leadJetPassesCut = true;
          if(MDzeroJetUPC->JetPt->at(0) < par.MinJetPT || MDzeroJetUPC->JetPt->at(0) > par.MaxJetPT) leadJetPassesCut = false;
          if(MDzeroJetUPC->JetY->at(0) < par.MinJetY || MDzeroJetUPC->JetY->at(0) > par.MaxJetY) leadJetPassesCut = false;
          if(MDzeroJetUPC->isD0Tagged->size() > 1 && MDzeroJetUPC->isD0Tagged->at(0) && leadJetPassesCut == true){

            double DPt =  MDzeroJetUPC->Dpt->at(MDzeroJetUPC->TaggedD0Index->at(0));
            double Dphi = MDzeroJetUPC->Dphi->at(MDzeroJetUPC->TaggedD0Index->at(0));
            double Dy  = MDzeroJetUPC->Dy->at(MDzeroJetUPC->TaggedD0Index->at(0));
            // make the D0 acceptance cuts
            if(DPt < par.MinDzeroPT || DPt > par.MaxDzeroPT) continue;
            if(Dy < par.MinDzeroY || Dy > par.MaxDzeroY) continue;
            double phiJet = MDzeroJetUPC->JetPhi->at(0);
            hDeltaPhiLeadingJetvLeadingD->Fill(abs(Dphi-phiJet));
            if(MDzeroJetUPC->JetPt->size() > 1){
              double phiSubleadingJet = MDzeroJetUPC->JetPhi->at(1);
              hDeltaPhiSubleadingJetvLeadingD->Fill(Dphi-phiSubleadingJet);
            }
          }
        }

      }   // end of data
      // -----------------------

      //-----------------------------------
      //----- MC analysis-----
      //-----------------------------------
      // in MC we have the following jet collections
      // JetPt: Reco level MC
      // RefPt: Gen Match of RecoLevel Jet
      // GenPt: Gen Level Jet Pt (doesn't need to match)
      // right now there is no additional event selection on the MC, need to add rapidity gap
      if (!par.IsData) {
        numberAccEvents++; // increment the number of events

        for (unsigned long j = 0; j < MDzeroJetUPC->JetPt->size(); j++) {
          // jet acceptance cuts
          if(MDzeroJetUPC->JetPt->at(j) < par.MinJetPT || MDzeroJetUPC->JetPt->at(j) > par.MaxJetPT) continue;
          if(MDzeroJetUPC->JetY->at(j) < par.MinJetY || MDzeroJetUPC->JetY->at(j) > par.MaxJetY) continue;

          hJetPt->Fill(MDzeroJetUPC->JetPt->at(j));

          // if after looping over all of the D0s we found one within the cone, fill the jet pt
          if(MDzeroJetUPC->isD0Tagged->at(j)){
            hD0JetPt->Fill(MDzeroJetUPC->JetPt->at(j));
            double DPt =  MDzeroJetUPC->Dpt->at(MDzeroJetUPC->TaggedD0Index->at(j));
            double Dy  = MDzeroJetUPC->Dy->at(MDzeroJetUPC->TaggedD0Index->at(j));
            // make the D0 acceptance cuts
            if(DPt < par.MinDzeroPT || DPt > par.MaxDzeroPT) continue;
            if(Dy < par.MinDzeroY || Dy > par.MaxDzeroY) continue;

            double Dphi = MDzeroJetUPC->Dphi->at(MDzeroJetUPC->TaggedD0Index->at(j));
            double Dmass = MDzeroJetUPC->Dmass->at(MDzeroJetUPC->TaggedD0Index->at(j));
            hD0FragFraction->Fill(DPt/MDzeroJetUPC->JetPt->at(j));
            hD0inJetPt->Fill(DPt);
            hD0massInJet->Fill(Dmass);
          }
        } // end of jet loop

        // only fill the delta phi histograms if we have a jet and a D0
        // first case, the leading jet is a d0 tagged jet
       if(MDzeroJetUPC->isD0Tagged->size() > 0){
          bool leadJetPassesCut = true;
          if(MDzeroJetUPC->JetPt->at(0) < par.MinJetPT || MDzeroJetUPC->JetPt->at(0) > par.MaxJetPT) leadJetPassesCut = false;
          if(MDzeroJetUPC->JetY->at(0) < par.MinJetY || MDzeroJetUPC->JetY->at(0) > par.MaxJetY) leadJetPassesCut = false;
          if(MDzeroJetUPC->isD0Tagged->size() > 1 && MDzeroJetUPC->isD0Tagged->at(0) && leadJetPassesCut == true){
            double DPt =  MDzeroJetUPC->Dpt->at(MDzeroJetUPC->TaggedD0Index->at(0));
            double Dphi = MDzeroJetUPC->Dphi->at(MDzeroJetUPC->TaggedD0Index->at(0));
            double phiJet = MDzeroJetUPC->JetPhi->at(0);
            hDeltaPhiLeadingJetvLeadingD->Fill(abs(Dphi-phiJet));
            if(MDzeroJetUPC->JetPt->size() > 1){
              double phiSubleadingJet = MDzeroJetUPC->JetPhi->at(1);
              hDeltaPhiSubleadingJetvLeadingD->Fill(Dphi-phiSubleadingJet);
            }
         }
       }

        // ref jet pT
        for (unsigned long j = 0; j < MDzeroJetUPC->RefJetPt->size(); j++) {
            hRefJetPt->Fill(MDzeroJetUPC->RefJetPt->at(j));

        } // end of ref jet loop (gen level reco match mc)

        // gen level loop
        for (unsigned long j = 0; j < MDzeroJetUPC->GenJetPt->size(); j++) {
            hGenJetPt->Fill(MDzeroJetUPC->GenJetPt->at(j));
        } // end of gen-level jet loop
      }   // end of mc
      // -----------------------

    }     // end of event loop

    hNev->SetBinContent(1, numberAccEvents);
    std::cout << "The number of accepted events is " << numberAccEvents << std::endl;

  }       // end of analyze

  void writeHistograms(TFile *outf) {
    outf->cd();
    smartWrite(hNev);
    smartWrite(hJetPt);
    smartWrite(hD0JetPt);
    smartWrite(hRefJetPt);
    smartWrite(hGenJetPt);
    smartWrite(hDeltaPhiLeadingJetvLeadingD);
    smartWrite(hDeltaPhiSubleadingJetvLeadingD);
    smartWrite(hD0FragFraction);
    smartWrite(hD0inJetPt);
    smartWrite(hD0massInJet);
  }

private:
  void deleteHistograms() {
    delete hNev;
    delete hJetPt;
    delete hD0JetPt;
    delete hRefJetPt;
    delete hGenJetPt;
    delete hDeltaPhiLeadingJetvLeadingD;
    delete hDeltaPhiSubleadingJetvLeadingD;
    delete hD0FragFraction;
    delete hD0inJetPt;
    delete hD0massInJet;


  }
};

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {
  if (printHelpMessage(argc, argv))
    return 0;
  CommandLine CL(argc, argv);
  float MinDzeroPT = CL.GetDouble("MinDzeroPT", 0.0);  // Minimum Dzero transverse momentum threshold for Dzero selection.
  float MaxDzeroPT = CL.GetDouble("MaxDzeroPT", 1000);  // Maximum Dzero transverse momentum threshold for Dzero selection.
  float MinDzeroY = CL.GetDouble("MinDzeroY", -2);   // Minimum Dzero rapidity threshold for Dzero selection.
  float MaxDzeroY = CL.GetDouble("MaxDzeroY", +2);   // Maximum Dzero rapidity threshold for Dzero selection.
  bool IsGammaN = CL.GetBool("IsGammaN", true);      // GammaN analysis (or NGamma)
  float MinJetPt = CL.GetDouble("MinJetPT", 0.0); // Minimum Jet PT
  float MaxJetPt = CL.GetDouble("MaxJetPT", 10000); // Max Jet PT
  float MinJetY = CL.GetDouble("MinJetY", -2.4); // min jet y
  float MaxJetY = CL.GetDouble("MaxJetY", 2.4); // max jet y
  int TriggerChoice = CL.GetInt("TriggerChoice", 2); // 0 = no trigger sel, 1 = isL1ZDCOr, 2 = isL1ZDCXORJet8
  float scaleFactor = CL.GetDouble("scaleFactor", 1); // Scale factor for the number of events to be processed.
  bool IsData = CL.GetBool("IsData", 0);              // Data or MC
  Parameters par(MinDzeroPT, MaxDzeroPT, MinDzeroY, MaxDzeroY, MinJetPt, MaxJetPt, MinJetY, MaxJetY, IsGammaN, TriggerChoice, IsData, scaleFactor);
  par.input = CL.Get("Input", "mergedSample.root"); // Input file
  par.output = CL.Get("Output", "output.root");     // Output file
  par.nThread = CL.GetInt("nThread", 1);            // The number of threads to be used for parallel processing.
  par.nChunk =  CL.GetInt("nChunk", 1); // Specifies which chunk (segment) of the data to process, used in parallel processing.
  std::cout << "Parameters are set" << std::endl;
  // Analyze Data
  DataAnalyzer analyzer(par.input.c_str(), par.output.c_str(), "");
  analyzer.analyze(par);
  analyzer.writeHistograms(analyzer.outf);
  par.printParameters();
  saveParametersToHistograms(par, analyzer.outf);
  cout << "done!" << analyzer.outf->GetName() << endl;
}
