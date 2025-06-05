#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TLorentzVector.h>
// include statements to pick up the temporary JECs from Nick
// #include "/afs/cern.ch/user/n/nbarnett/public/header_files/JetUncertainty.h"
// #include "/afs/cern.ch/user/n/nbarnett/public/header_files/JetCorrector.h"

#include <iostream>

using namespace std;
#include "CommandLine.h" // Yi's Commandline bundle
#include "Messenger.h"   // Yi's Messengers for reading data files
#include "TauHelperFunctions3.h"
#include "ProgressBar.h" // Yi's fish progress bar
#include "helpMessage.h" // Print out help message
#include "parameter.h"   // The parameters used in the analysis
#include "utilities.h"   // Yen-Jie's random utility functions


//======= findBin =====================================//
// function that scans the bins provided in order to know what bin to put things in 
//============================================================//
int FindBin(double Value, int NBins, double Bins[])
{
   for(int i = 0; i < NBins; i++)
      if(Value < Bins[i])
         return i - 1;
   return NBins;
}


//======= triggerSelection =====================================//
// Here the event selection was done at the level of the skimmer
// do the trigger selection here
// could consider to do additional event selection for gammaN or Ngamma here too, but for now we don't (consider Ngamma and GammaN as combined)
//============================================================//
bool triggerSelection(UPCEECTreeMessenger *b, const Parameters &par) {
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
  TH1D* hEEC; // EEC in the double log style 
  TH1D* hTrackPt; // track pT
  TH1D* hTrackEta; // track eta
  TH1D* hPtSum; // total scalar pT sum
  TH1D* hMtSum; // total transverse mass
  TH1D* hMult; // multi


  UPCEECTreeMessenger *MDzeroJetUPC;


  DataAnalyzer(const char *filename, const char *outFilename, const char *mytitle = "")
      : inf(new TFile(filename)), MDzeroJetUPC(new UPCEECTreeMessenger(*inf, string("Tree"))),
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


    hNev = new TH1D("hNev", "", 1, 0, 1);
    hEECInclusive = new TH1D("hEECInclusive", "", EECBins.size()-1, EECBins.data()); 
    hTrackPt = new TH1D("hTrackPt", "", 50, 0, 50); 
    hMult = new TH1D("hMult", "", 50, 0, 50);
    hTrackEta = new TH1D("hTrackEta", "", 20, -5, 5); 
    hPtSum =new TH1D("hPtSum", "", 50, 0, 200); 
    hMtSum = new TH1D("hMtSum", "", 50, 0, 200); 
    hEEC =  new TH1D("hEEC", "hEEC", 2 * BinCount, 0, 2 * BinCount);
    


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
      
      // only difference in the data part is the trigger
      if ((par.IsData && triggerSelection(MDzeroJetUPC, par)) || !par.IsData) {
         hMult->Fill(MDzeroJetUPC->Nch);
         
         numberAccEvents++; // increment the number of events
         
         // Step 1: Determine the right hard scale to use 
         // first loop over particle flow candidates to fill the scalar pT sum and the transverse mass sum
         double PtSum = 0.0; 
         double MtSum = 0.0; 
         for (unsigned long a = 0; a < MDzeroJetUPC->PT->size(); a++) {
            // for this cutoff use the 1 GeV threshold determined by Luna
            if(MDzeroJetUPC->PT->at(a) < 1.0) continue; 
            PtSum += MDzeroJetUPC->PT->at(a);
            MtSum += MDzeroJetUPC->M->at(a);
         } // end first loop over the number of tracks

         
         // Step 2: Make a cut based on the hard scale
         // skip event if it is outside of the desired hard scale
         // case 1: using visible energy as the hard scale
         if(par.UsePtSumHardScale && (PtSum < par.MinHardScale || PtSum > par.MaxHardScale)) continue; 
         // case 2: using the visible mass as the hard scale.
         if(!par.UsePtSumHardScale && (MtSum < par.MinHardScale|| MtSum > par.MaxHardScale)) continue; 
        

        if(PtSum < 1.0) std::cout << "Error: If statement not working! " << std::endl;
         
         // std::cout << "par.UsePtSumHardScale "  << par.UsePtSumHardScale << " PtSum: " << PtSum  << " par.MinPtSum " << par.MinPtSum << "  par.MaxMtSum: " <<  par.MaxMtSum << std::endl;
         // now fill the EEC if the event has the desired hard scale
          for (unsigned long a = 0; a < MDzeroJetUPC->Nch; a++) {
            // acceptance cuts on track a
            if( MDzeroJetUPC->trkPt->at(a) < par.MinTrackPT ) continue; 
            if(abs(MDzeroJetUPC->trkEta->at(a)) >  par.MaxTrackY) continue; 
            hTrackPt->Fill(MDzeroJetUPC->trkPt->at(a)); 
            hTrackEta->Fill(MDzeroJetUPC->trkEta->at(a)); 
            
            for (unsigned long b = a+1; b < MDzeroJetUPC->Nch; b++) {
              // acceptance cuts on track b
              if(MDzeroJetUPC->trkPt->at(b) < par.MinTrackPT ) continue; 
              if(abs(MDzeroJetUPC->trkEta->at(b)) >  par.MaxTrackY ) continue; 

              // fill the delta R EEC
              double deltaEta = MDzeroJetUPC->trkEta->at(a) - MDzeroJetUPC->trkEta->at(b); 
              double deltaPhi = MDzeroJetUPC->trkPhi->at(a) - MDzeroJetUPC->trkPhi->at(b); 
              double deltaR = sqrt(deltaEta*deltaEta + deltaPhi*deltaPhi); 
              double EEC = MDzeroJetUPC->trkPt->at(a)* MDzeroJetUPC->trkPt->at(b);
              hEECInclusive->Fill(deltaR, EEC); 
              
              // fill the double log EEC
              // need to create the four vector 
              FourVector Reco1; 
              FourVector Reco2; 
              // use pion mass assumption 138.04 MeV/c
              // use this value since it is roughly (2*charged pion mass) + neutral pion mass / 3
              Reco1.SetPtEtaPhiMass(MDzeroJetUPC->trkPt->at(a), MDzeroJetUPC->trkEta->at(a), MDzeroJetUPC->trkPhi->at(a), 0.13804); 
              Reco2.SetPtEtaPhiMass(MDzeroJetUPC->trkPt->at(b), MDzeroJetUPC->trkEta->at(b), MDzeroJetUPC->trkPhi->at(b), 0.13804); 
              double recoTheta = GetAngle(Reco1,Reco2);
              //std::cout << "recoTheta " << recoTheta << std::endl;
              int BinThetaReco = FindBin(recoTheta, 2 * BinCount, Bins);
              double TotalE; 
              if(par.UsePtSumHardScale)TotalE = PtSum; 
              else TotalE = MtSum; 

              //std::cout << "total E " << TotalE << std::endl;
              double  recoEEC  = MDzeroJetUPC->pfEnergy->at(a)*MDzeroJetUPC->pfEnergy->at(b)/(TotalE*TotalE);
              //std::cout << "Filling EEC in bin: " << BinThetaReco << " with entry count " << recoEEC << std::endl;
              hEEC->Fill(BinThetaReco, recoEEC);
            } // end of track loop   
          } // end of track loop
          
      } // end of event selection if statement
    }     // end of event loop

    hNev->SetBinContent(1, numberAccEvents);
    std::cout << "The number of accepted events is " << numberAccEvents << std::endl;

  }       // end of analyze
 
  void writeHistograms(TFile *outf) {
    outf->cd();
    smartWrite(hNev);
    smartWrite(hEECInclusive);
    smartWrite(hTrackPt); 
    smartWrite(hTrackEta); 
    smartWrite(hPtSum); 
    smartWrite(hMtSum);
    smartWrite(hMult);
    smartWrite(hEEC); 


  }

private:
  void deleteHistograms() {
    delete hNev;
    delete hEECInclusive;
    delete hPtSum; 
    delete hMult; 
    delete hTrackPt; 
    delete hTrackEta;



  }
};

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {
  if (printHelpMessage(argc, argv))
    return 0;
  CommandLine CL(argc, argv);
  bool IsGammaN = CL.GetBool("IsGammaN", true);      // GammaN analysis (or NGamma)
  float MinTrackPt = CL.GetDouble("MinTrackPT", 0.0); // Minimum track PT
  float MaxTrackPt = CL.GetDouble("MaxTrackPT", 10000); // Max track PT
  float MinTrackY = CL.GetDouble("MinTrackY", -2.4); // min track y
  float MaxTrackY = CL.GetDouble("MaxTrackY", 2.4); // max track y
  float MinHardScale = CL.GetDouble("MinHardScale", 0.0); // Minimum visible energy
  float MaxHardScale = CL.GetDouble("MaxHardScale", 10000); // Max visible energy
  bool UsePtSumHardScale = CL.GetBool("UsePtSumHardScale", 1); 
  int TriggerChoice = CL.GetInt("TriggerChoice", 2); // 0 = no trigger sel, 1 = isL1ZDCOr, 2 = isL1ZDCXORJet8
  float scaleFactor = CL.GetDouble("scaleFactor", 1); // Scale factor for the number of events to be processed.
  bool IsData = CL.GetBool("IsData", 0);              // Data or MC
  Parameters par(MinTrackPt, MaxTrackPt, MinTrackY, MaxTrackY, MinHardScale, MaxHardScale, UsePtSumHardScale, IsGammaN, TriggerChoice, IsData, scaleFactor);
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
