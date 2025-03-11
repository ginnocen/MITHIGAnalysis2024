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

  // EEC histograms
  TH1D* hEECInclusive; // inclusive EEC for gamma N UPC events
  TH1D* hEEC_D0Tagged; // EEC in inclusive 



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
    
    int numBins = 25;  // Number of bins
    double start = 0.005, stop = 5.0;

    // Compute logarithmic bin edges
    std::vector<double> EECBins;
    for (int i = 0; i < numBins; ++i) {
        double binEdge = start * pow(stop / start, static_cast<double>(i) / (numBins - 1));
        EECBins.push_back(binEdge);
    }


    hNev = new TH1D("hNev", "", 1, 0, 1);
    hEECInclusive = new TH1D("hEECInclusive", "", EECBins.size()-1, EECBins.data()); 
    hEEC_D0Tagged = new TH1D("hEEC_D0Tagged", "", EECBins.size()-1, EECBins.data()); 


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
        // for now we only use the jets to tag the event
        // create flags for whether or not the event is tagged
        bool fillEEC = false; 
        bool fillD0EEC = false; 
        for (unsigned long j = 0; j < MDzeroJetUPC->JetPt->size(); j++) {
          // jet acceptance cuts
          if(MDzeroJetUPC->JetPt->at(j) < par.MinJetPT || MDzeroJetUPC->JetPt->at(j) > par.MaxJetPT) continue;
          if(MDzeroJetUPC->JetY->at(j) < par.MinJetY || MDzeroJetUPC->JetY->at(j) > par.MaxJetY) continue;
          fillEEC = true; 

          // if after looping over all of the D0s we found one within the cone, fill the jet pt
          if(MDzeroJetUPC->isD0TaggedGeomJet->at(j)){
            double DPt =  MDzeroJetUPC->Dpt->at(MDzeroJetUPC->TaggedLeadingD0GeomInJetIndex->at(j));
            double Dy  = MDzeroJetUPC->Dy->at(MDzeroJetUPC->TaggedLeadingD0GeomInJetIndex->at(j));
            // make the D0 acceptance cuts
            if(DPt < par.MinDzeroPT || DPt > par.MaxDzeroPT) continue;
            if(Dy < par.MinDzeroY || Dy > par.MaxDzeroY) continue;
            fillD0EEC = true; 
            break; // don't  need to keep looking after we've found one
          }
        } // end of jet loop
        
        if(fillD0EEC && ! fillEEC)std::cout << "Error: Logic to fill the EEC is incorrect" << std::endl; 

        if(fillEEC){
          //std::cout << "Found a jet meeting requirements, now moving on to EEC - looping over " << MDzeroJetUPC->Nch << " particles " << std::endl;
          //std::cout << MDzeroJetUPC->trkEta->size() << std::endl;
          for (unsigned long a = 0; a < MDzeroJetUPC->Nch; a++) {
            for (unsigned long b = a+1; b < MDzeroJetUPC->Nch; b++) {
              // skip tracks outside of the barrel
              if(abs(MDzeroJetUPC->trkEta->at(a)) > 2.4 || abs(MDzeroJetUPC->trkEta->at(b)) > 2.4 ) continue; 
              //if(MDzeroJetUPC->trkPt->at(a) > 20 || MDzeroJetUPC->trkPt->at(a) > 20  ) continue; 
              double deltaEta = MDzeroJetUPC->trkEta->at(a) - MDzeroJetUPC->trkEta->at(b); 
              double deltaPhi = MDzeroJetUPC->trkPhi->at(a) - MDzeroJetUPC->trkPhi->at(b); 
              double deltaR = sqrt(deltaEta*deltaEta + deltaPhi*deltaPhi); 
              //std::cout << "Pair " << a << " , " << b << " ) has a delta R of " << deltaR << std::endl;
              double EEC = MDzeroJetUPC->trkPt->at(a)* MDzeroJetUPC->trkPt->at(b);
              hEECInclusive->Fill(deltaR, EEC); 
              if(fillD0EEC)hEEC_D0Tagged->Fill(deltaR, EEC); 
              
            } // end of track loop   
          } // end of track loop
          ///std::cout << "Done with EEC loop" << std::endl;
        }

      

        
      }   // end of mc
      // -----------------------

    }     // end of event loop

    hNev->SetBinContent(1, numberAccEvents);
    std::cout << "The number of accepted events is " << numberAccEvents << std::endl;

  }       // end of analyze

  void writeHistograms(TFile *outf) {
    outf->cd();
    smartWrite(hNev);
    smartWrite(hEECInclusive);
    smartWrite(hEEC_D0Tagged);

  }

private:
  void deleteHistograms() {
    delete hNev;
    delete hEECInclusive;
    delete hEEC_D0Tagged;


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
