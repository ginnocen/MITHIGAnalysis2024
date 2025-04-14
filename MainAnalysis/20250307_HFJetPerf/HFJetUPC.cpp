#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>
// include statements to pick up the temporary JECs from Nick
#include "/afs/cern.ch/user/n/nbarnett/public/header_files/JetUncertainty.h"
#include "/afs/cern.ch/user/n/nbarnett/public/header_files/JetCorrector.h"

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
// for the moment the selections are handled in the actual functions, but could consider to move them here.
//============================================================//
bool jetSelection(DzeroJetUPCTreeMessenger *b, Parameters par, int j) {
  return true;
}

//======= triggerSelection =====================================//
// Here the event selection was done at the level of the skimmer
// do the trigger selection here
// could consider to do additional event selection for gammaN or Ngamma here too, but for now we don't (consider Ngamma and GammaN as combined)
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
  TH1D* hRefD0JetPt; // gen level match of a D0-tagged reco level jet
  TH1D* hGenJetPt; // generator level jet pT spectrum
  TH1D* hGenD0JetPt; // generator level D0 jet pT spectrum
  TH2D* h2PtJetPtRef; // 2d jet performance histogram
  TH2D* h2D0JetPtD0Ref; // 2d jet performance histogram for d0-tagged jets.
  TH2D* h2PtJetPtRefvsY; // JES and JER vs. Rapidity for inclusive jets
  TH2D* h2D0PtJetPtRefvsY; // JES and JER vs. Rapidity for D0 tagged jets


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

    std::vector<double> jetPtBins = {15, 20, 25, 35, 50};


    hNev = new TH1D("hNev", "", 1, 0, 1);

    hJetPt = new TH1D("hJetPt", "", jetPtBins.size()-1, jetPtBins.data());
    hJetPt->Sumw2();

    hD0JetPt =  new TH1D("hD0JetPt", "", jetPtBins.size()-1, jetPtBins.data());
    hD0JetPt->Sumw2();

    hRefJetPt = new TH1D("hRefJetPt", "", jetPtBins.size()-1, jetPtBins.data());
    hRefJetPt->Sumw2();

    hRefD0JetPt = new TH1D("hRefD0JetPt", "", jetPtBins.size()-1, jetPtBins.data());
    hRefD0JetPt->Sumw2();

    hGenJetPt = new TH1D("hGenJetPt", "", jetPtBins.size()-1, jetPtBins.data());
    hGenJetPt->Sumw2();

    hGenD0JetPt = new TH1D("hGenD0JetPt", "", jetPtBins.size()-1, jetPtBins.data());
    hGenD0JetPt->Sumw2();


    h2PtJetPtRef = new TH2D("h2PtJetPtRef", "", jetPtBins.size()-1, jetPtBins.data(), 50, 0.0, 2.0);
    h2PtJetPtRef->Sumw2();

    h2D0JetPtD0Ref = new TH2D("h2D0JetPtD0Ref", "",jetPtBins.size()-1, jetPtBins.data(), 50, 0.0, 2.0);
    h2D0JetPtD0Ref->Sumw2();


    // use eta jet  < 2 cut being consistent with https://arxiv.org/pdf/2102.13080
    std::vector<double> yBins = {-5.0, -4.0, -3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0};
    h2PtJetPtRefvsY = new TH2D("h2PtJetPtRefvsY", "", yBins.size()-1, yBins.data(), 50, 0.0, 2.0);
    h2PtJetPtRefvsY->Sumw2();

    h2D0PtJetPtRefvsY = new TH2D("h2D0PtJetPtRefvsY", "", yBins.size()-1, yBins.data(), 50, 0.0, 2.0);
    h2D0PtJetPtRefvsY->Sumw2();

     // get the necessary jet energy corrections
    vector<string> Files;
    Files.push_back("/afs/cern.ch/user/n/nbarnett/public/txt_files/L2L3_ppReco_2023ppRef/ParallelMC_L2Relative_AK4PF_pp_Reco_v0_12-21-2023.txt");
    JetCorrector JEC(Files);

    par.printParameters();
    unsigned long nEntry = MDzeroJetUPC->GetEntries() * par.scaleFactor;
    std::cout << "Processing " << nEntry << " events." << std::endl;
    ProgressBar Bar(cout, nEntry);
    Bar.SetStyle(1);
    // loop over the number of events
    int numberAccEvents = 0;
    for (unsigned long i = 0; i < nEntry; i++) {
      MDzeroJetUPC->GetEntry(i);

      if(i % 1000 == 0) std::cout << "On entry " << i << std::endl;

      //------------------------------------------------------
      //----- Data analysis-----------------------------------
      //------------------------------------------------------
      if (par.IsData && triggerSelection(MDzeroJetUPC, par)) {

        numberAccEvents++; // increment the number of events


        // jet loop
        for (unsigned long j = 0; j < MDzeroJetUPC->JetPt->size(); j++) {

          // jet acceptance cuts
          if(MDzeroJetUPC->JetPt->at(j) < par.MinJetPT || MDzeroJetUPC->JetPt->at(j) > par.MaxJetPT) continue;
          if(MDzeroJetUPC->JetY->at(j) < par.MinJetY || MDzeroJetUPC->JetY->at(j) > par.MaxJetY) continue;

	  // correct the data
          JEC.SetJetPT(MDzeroJetUPC->RawJetPt->at(j));
          JEC.SetJetEta(MDzeroJetUPC->JetEta->at(j));
          JEC.SetJetPhi(MDzeroJetUPC->JetPhi->at(j));

          float jet_pt_corr = JEC.GetCorrectedPT();

          hJetPt->Fill(jet_pt_corr);
          // if after looping over all of the D0s we found one within the cone, fill the jet pt
          if(MDzeroJetUPC->isD0TaggedGeomJet->at(j)){

            double DPt =  MDzeroJetUPC->Dpt->at(MDzeroJetUPC->TaggedLeadingD0GeomInJetIndex->at(j));
            double Dy  = MDzeroJetUPC->Dy->at(MDzeroJetUPC->TaggedLeadingD0GeomInJetIndex->at(j));

            // make the D0 acceptance cuts
            if(DPt < par.MinDzeroPT || DPt > par.MaxDzeroPT) continue;
            if(Dy < par.MinDzeroY || Dy > par.MaxDzeroY) continue;
            hD0JetPt->Fill(jet_pt_corr);

          }
        } // end of jet loop

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


        // reco jet loop
        for (unsigned long j = 0; j < MDzeroJetUPC->JetPt->size(); j++) {
          // jet acceptance cuts
          if(MDzeroJetUPC->JetPt->at(j) < par.MinJetPT || MDzeroJetUPC->JetPt->at(j) > par.MaxJetPT) continue;
          if(MDzeroJetUPC->JetY->at(j) < par.MinJetY || MDzeroJetUPC->JetY->at(j) > par.MaxJetY) continue;

          JEC.SetJetPT(MDzeroJetUPC->RawJetPt->at(j));
          JEC.SetJetEta(MDzeroJetUPC->JetEta->at(j));
          JEC.SetJetPhi(MDzeroJetUPC->JetPhi->at(j));

          float jet_pt_corr = JEC.GetCorrectedPT();

          hJetPt->Fill(jet_pt_corr);

          // if after looping over all of the D0s we found one within the cone, fill the jet pt
          if(MDzeroJetUPC->isD0TaggedGeomJet->at(j)){
            double DPt =  MDzeroJetUPC->Dpt->at(MDzeroJetUPC->TaggedLeadingD0GeomInJetIndex->at(j));
            double Dy  = MDzeroJetUPC->Dy->at(MDzeroJetUPC->TaggedLeadingD0GeomInJetIndex->at(j));
            // make the D0 acceptance cuts
            if(DPt < par.MinDzeroPT || DPt > par.MaxDzeroPT) continue;
            if(Dy < par.MinDzeroY || Dy > par.MaxDzeroY) continue;
            hD0JetPt->Fill(jet_pt_corr);
          }
        } // end of jet loop

      

        for (unsigned long j = 0; j < MDzeroJetUPC->RefJetPt->size(); j++) {

          if(MDzeroJetUPC->RefJetPt->at(j) < par.MinJetPT || MDzeroJetUPC->RefJetPt->at(j) > par.MaxJetPT) continue;
          if(MDzeroJetUPC->RefJetY->at(j) < par.MinJetY || MDzeroJetUPC->RefJetY->at(j) > par.MaxJetY) continue;

            // calculate the new reco jet pT by hand from the JECs provided by Nick
            JEC.SetJetPT(MDzeroJetUPC->RawJetPt->at(j));
            JEC.SetJetEta(MDzeroJetUPC->JetEta->at(j));
            JEC.SetJetPhi(MDzeroJetUPC->JetPhi->at(j));

            float jet_pt_corr = JEC.GetCorrectedPT();



            hRefJetPt->Fill(MDzeroJetUPC->RefJetPt->at(j));
            // jet perf (put back heree)
            h2PtJetPtRef->Fill(MDzeroJetUPC->RefJetPt->at(j), jet_pt_corr/MDzeroJetUPC->RefJetPt->at(j));
            h2PtJetPtRefvsY->Fill(MDzeroJetUPC->RefJetY->at(j), jet_pt_corr/MDzeroJetUPC->RefJetPt->at(j));


            if(MDzeroJetUPC->isD0TaggedGeomRefJet->at(j) == 1){
              double DPt =  MDzeroJetUPC->Gpt->at(MDzeroJetUPC->TaggedLeadingD0GeomInRefJetIndex->at(j));
              double Dy  = MDzeroJetUPC->Gy->at(MDzeroJetUPC->TaggedLeadingD0GeomInRefJetIndex->at(j));
              if(DPt < par.MinDzeroPT || DPt > par.MaxDzeroPT) continue;
              if(Dy < par.MinDzeroY || Dy > par.MaxDzeroY) continue;
              if(MDzeroJetUPC->RefJetY->at(j) < par.MinDzeroY || MDzeroJetUPC->RefJetY->at(j) > par.MaxDzeroY) continue;
              if(MDzeroJetUPC->isD0TaggedGeomRefJet->at(j)){
                h2D0JetPtD0Ref->Fill(MDzeroJetUPC->RefJetPt->at(j), jet_pt_corr/MDzeroJetUPC->RefJetPt->at(j));
                h2D0PtJetPtRefvsY->Fill(MDzeroJetUPC->RefJetY->at(j), jet_pt_corr/MDzeroJetUPC->RefJetPt->at(j));
              }
            }

        } // end of ref jet loop (gen level reco match mc)


        // gen level loop
        // if the size is bigger than 0 fill the pT hat histogram only with the leading jet
        for (unsigned long j = 0; j < MDzeroJetUPC->GenJetPt->size(); j++) {
            if(MDzeroJetUPC->GenJetPt->at(j) < par.MinJetPT || MDzeroJetUPC->GenJetPt->at(j) > par.MaxJetPT) continue;
            if(MDzeroJetUPC->GenJetY->at(j) < par.MinJetY || MDzeroJetUPC->GenJetY->at(j) > par.MaxJetY) continue;
            hGenJetPt->Fill(MDzeroJetUPC->GenJetPt->at(j));
            if(MDzeroJetUPC->isD0TaggedGeomGenJet->at(j) == 1){
              double DPt =  MDzeroJetUPC->Gpt->at(MDzeroJetUPC->TaggedLeadingD0GeomInGenJetIndex->at(j));
              double Dy  = MDzeroJetUPC->Gy->at(MDzeroJetUPC->TaggedLeadingD0GeomInGenJetIndex->at(j));
              if(DPt < par.MinDzeroPT || DPt > par.MaxDzeroPT) continue;
              if(Dy < par.MinDzeroY || Dy > par.MaxDzeroY) continue;
              hGenD0JetPt->Fill(MDzeroJetUPC->GenJetPt->at(j));
            }
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
    smartWrite(hRefD0JetPt);
    smartWrite(hGenD0JetPt);
    smartWrite(hGenJetPt);
    smartWrite(h2PtJetPtRef);
    smartWrite(h2D0JetPtD0Ref);
    smartWrite(h2D0PtJetPtRefvsY);
    smartWrite(h2PtJetPtRefvsY);
  }

private:
  void deleteHistograms() {
    delete hNev;
    delete hJetPt;
    delete hD0JetPt;
    delete hRefJetPt;
    delete hRefD0JetPt;
    delete hGenJetPt;
    delete hGenD0JetPt;
    delete h2PtJetPtRef;
    delete h2D0JetPtD0Ref;
    delete h2D0PtJetPtRefvsY;
    delete h2PtJetPtRefvsY;

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
