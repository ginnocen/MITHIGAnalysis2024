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

//============================================================//
// Function to check for configuration errors
//============================================================//
bool checkError(const Parameters &par) { return false; }

//======= trackSelection =====================================//
// Check if the track pass selection criteria
//============================================================//
bool dzeroSelection(DzeroUPCTreeMessenger *b, Parameters par, int j) { return true; }

//======= eventSelection =====================================//
// Check if the event pass eventSelection criteria
//============================================================//
bool eventSelection(DzeroUPCTreeMessenger *b, const Parameters &par) {
  if (par.IsData)
  {
    if (par.TriggerChoice == 1 && b->isL1ZDCOr == false)
      return false;
    if (par.TriggerChoice == 2 && b->isL1ZDCXORJet8 == false)
      return false;
  }

  if (b->selectedBkgFilter == false || b->selectedVtxFilter == false)
    return false;

  if (par.DoSystRapGap==-1)
  {
    // alternative (loose) rapidity gap selection
    if (par.IsGammaN && b->gammaN_EThreshSyst15() == false)
      return false;
    if (!par.IsGammaN && b->Ngamma_EThreshSyst15() == false)
      return false;
  }
  else if (par.DoSystRapGap==1)
  {
    // alternative (tight) rapidity gap selection
    if (par.IsGammaN && b->gammaN_EThreshSyst5p5() == false)
      return false;
    if (!par.IsGammaN && b->Ngamma_EThreshSyst5p5() == false)
      return false;
  } 
  else if (par.DoSystRapGap > 9) {
    // Custom rapidity gap threshold decision
    if (par.IsGammaN && b->gammaN_EThreshCustom(((float)par.DoSystRapGap)/10.) == false)
      return false;
    if (!par.IsGammaN && b->Ngamma_EThreshCustom(((float)par.DoSystRapGap)/10.) == false)
      return false;
  } 
  else
  {
    // nominal rapidity gap selection
    if (par.IsGammaN && (b->ZDCgammaN && b->gapgammaN) == false)
      return false;
    if (!par.IsGammaN && (b->ZDCNgamma && b->gapNgamma) == false)
      return false;
  }

  if (b->nVtx >= 3) return false;
  return true;
}

class DataAnalyzer {
public:
  TFile *inf, *outf;
  TH1D *hDmass;
  DzeroUPCTreeMessenger *MDzeroUPC;
  TNtuple *nt;
  string title;
  TH2D *hHFEmaxPlus_vs_EvtMult;
  TH2D *hHFEmaxMinus_vs_EvtMult;
  TH1D *hDenEvtEff;
  TH1D *hNumEvtEff;
  TH1D *hRatioEvtEff;
  TH1D *hDenDEff;
  TH1D *hNumDEff;
  TH1D *hRatioDEff;

  DataAnalyzer(const char *filename, const char *outFilename, const char *mytitle = "")
      : inf(new TFile(filename)), MDzeroUPC(new DzeroUPCTreeMessenger(*inf, string("Tree"))), title(mytitle),
        outf(new TFile(outFilename, "recreate")) {
    outf->cd();
    nt = new TNtuple("nt", "D0 mass tree", "Dmass:Dgen");
  }

  ~DataAnalyzer() {
    deleteHistograms();
    delete nt;
    inf->Close();
    outf->Close();
    delete MDzeroUPC;
  }

  void analyze(Parameters &par) {
    outf->cd();
    hDmass = new TH1D(Form("hDmass%s", title.c_str()), "", 60, 1.7, 2.0);
    hDenEvtEff = new TH1D(Form("hDenEvtEff%s", title.c_str()), "", 1, 0.5, 1.5);
    hNumEvtEff = new TH1D(Form("hNumEvtEff%s", title.c_str()), "", 1, 0.5, 1.5);
    hRatioEvtEff = (TH1D*) hNumEvtEff->Clone("hRatioEvtEff");
    hDenDEff = new TH1D(Form("hDenDEff%s", title.c_str()), "", 1, 0.5, 1.5);
    hNumDEff = new TH1D(Form("hNumDEff%s", title.c_str()), "", 1, 0.5, 1.5);
    hRatioDEff = (TH1D*) hNumDEff->Clone("hRatioDEff");
    hHFEmaxMinus_vs_EvtMult = nullptr;
    hHFEmaxPlus_vs_EvtMult = nullptr;

    bool doHFEmaxDistributions=(par.DoSystRapGap > 9);
    if (doHFEmaxDistributions) {
      hHFEmaxMinus_vs_EvtMult = new TH2D(Form("hHFEmaxMinus_vs_EvtMult%s", title.c_str()), "", 80, 0, 20, 200, 0, 1000);
      hHFEmaxPlus_vs_EvtMult = new TH2D(Form("hHFEmaxPlus_vs_EvtMult%s", title.c_str()), "", 80, 0, 20, 200, 0, 1000);

      hHFEmaxMinus_vs_EvtMult->Sumw2();
      hHFEmaxPlus_vs_EvtMult->Sumw2();
    }

    hDmass->Sumw2();
    hDenEvtEff->Sumw2();
    hNumEvtEff->Sumw2();
    hDenDEff->Sumw2();
    hNumDEff->Sumw2();

    par.printParameters();
    unsigned long nEntry = MDzeroUPC->GetEntries() * par.scaleFactor;
    ProgressBar Bar(cout, nEntry);
    Bar.SetStyle(1);

    for (unsigned long i = 0; i < nEntry; i++) {
      MDzeroUPC->GetEntry(i);
      if (i % 1000 == 0) {
        Bar.Update(i);
        Bar.Print();
      }

      // Check if the event is a signal MC event, a.k.a., having at least one gen-level D candidate that falls into the (pt,y) bin of interest
      bool isSigMCEvt = false;
      if (!par.IsData){
        for (unsigned long j = 0; j < MDzeroUPC->Gpt->size(); j++) {
          if (MDzeroUPC->Gpt->at(j) < par.MinDzeroPT)
            continue;
          if (MDzeroUPC->Gpt->at(j) > par.MaxDzeroPT)
            continue;
          if (MDzeroUPC->Gy->at(j) < par.MinDzeroY)
            continue;
          if (MDzeroUPC->Gy->at(j) > par.MaxDzeroY)
            continue;
          if (MDzeroUPC->GisSignalCalc->at(j) == false)
            continue;
          isSigMCEvt = true;
        }
      }

      if (!par.IsData && isSigMCEvt) hDenEvtEff->Fill(1);

      // Check if the event passes the selection criteria
      if (eventSelection(MDzeroUPC, par)) {
        if (!par.IsData && isSigMCEvt) hNumEvtEff->Fill(1);

        for (unsigned long j = 0; j < MDzeroUPC->Dalpha->size(); j++) {
          if (MDzeroUPC->Dpt->at(j) < par.MinDzeroPT)
            continue;
          if (MDzeroUPC->Dpt->at(j) > par.MaxDzeroPT)
            continue;
          if (MDzeroUPC->Dy->at(j) < par.MinDzeroY)
            continue;
          if (MDzeroUPC->Dy->at(j) > par.MaxDzeroY)
            continue;
          // if (MDzeroUPC->DpassCut23PAS->at(j) == false)
          if (par.DoSystD==0 && MDzeroUPC->DpassCut23PAS->at(j) == false) continue;
          // if (par.DoSystD==0 && MDzeroUPC->DpassCut23LowPt->at(j) == false) continue;
          if (par.DoSystD==1 && MDzeroUPC->DpassCut23PASSystDsvpvSig->at(j) == false) continue;
          if (par.DoSystD==2 && MDzeroUPC->DpassCut23PASSystDtrkPt->at(j) == false) continue;
          if (par.DoSystD==3 && MDzeroUPC->DpassCut23PASSystDalpha->at(j) == false) continue;
          if (par.DoSystD==4 && MDzeroUPC->DpassCut23PASSystDchi2cl->at(j) == false) continue;

          hDmass->Fill((*MDzeroUPC->Dmass)[j]);
          if (!par.IsData) {
            nt->Fill((*MDzeroUPC->Dmass)[j], (*MDzeroUPC->Dgen)[j]);
            if (MDzeroUPC->Dgen->at(j) == 23333) {
              hNumDEff->Fill(1);
            }
          } else
            nt->Fill((*MDzeroUPC->Dmass)[j], 0);

          // Fill HF E_max distributions for data
          if(doHFEmaxDistributions && par.IsData) {
            hHFEmaxMinus_vs_EvtMult->Fill(MDzeroUPC->HFEMaxMinus, MDzeroUPC->nTrackInAcceptanceHP);
            hHFEmaxPlus_vs_EvtMult->Fill(MDzeroUPC->HFEMaxPlus, MDzeroUPC->nTrackInAcceptanceHP);
          }
        } // end of reco-level Dzero loop

        if (!par.IsData && isSigMCEvt) {
          for (unsigned long j = 0; j < MDzeroUPC->Gpt->size(); j++) {
            if (MDzeroUPC->Gpt->at(j) < par.MinDzeroPT)
              continue;
            if (MDzeroUPC->Gpt->at(j) > par.MaxDzeroPT)
              continue;
            if (MDzeroUPC->Gy->at(j) < par.MinDzeroY)
              continue;
            if (MDzeroUPC->Gy->at(j) > par.MaxDzeroY)
              continue;
            if (MDzeroUPC->GisSignalCalc->at(j) == false)
              continue;
            hDenDEff->Fill(1);
            // Fill HF E_max distributions for MC
            if(doHFEmaxDistributions) {
              hHFEmaxMinus_vs_EvtMult->Fill(MDzeroUPC->HFEMaxMinus, MDzeroUPC->nTrackInAcceptanceHP);
              hHFEmaxPlus_vs_EvtMult->Fill(MDzeroUPC->HFEMaxPlus, MDzeroUPC->nTrackInAcceptanceHP);
            }
          } // end of gen-level Dzero loop
        }   // end of gen-level Dzero loop
      }   // end of event selection
    }     // end of event loop
  }       // end of analyze

  void writeHistograms(TFile *outf) {
    outf->cd();
    smartWrite(hDmass);
    hRatioEvtEff->Divide(hNumEvtEff, hDenEvtEff, 1, 1, "B");
    hRatioDEff->Divide(hNumDEff, hDenDEff, 1, 1, "B");
    hDenEvtEff->Write();
    hNumEvtEff->Write();
    hRatioEvtEff->Write();
    hDenDEff->Write();
    hNumDEff->Write();
    hRatioDEff->Write();
    smartWrite(nt);
    smartWrite(hHFEmaxMinus_vs_EvtMult);
    smartWrite(hHFEmaxPlus_vs_EvtMult);
  }

private:
  void deleteHistograms() {
    delete hDmass;
    delete hDenEvtEff;
    delete hNumEvtEff;
    delete hRatioEvtEff;
    delete hDenDEff;
    delete hNumDEff;
    delete hRatioDEff;
    if (hHFEmaxMinus_vs_EvtMult != nullptr) {
      delete hHFEmaxMinus_vs_EvtMult;
    }
    if (hHFEmaxPlus_vs_EvtMult != nullptr) {
      delete hHFEmaxPlus_vs_EvtMult;
    }
  }
};

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {
  if (printHelpMessage(argc, argv))
    return 0;
  CommandLine CL(argc, argv);
  float MinDzeroPT = CL.GetDouble("MinDzeroPT", 1);  // Minimum Dzero transverse momentum threshold for Dzero selection.
  float MaxDzeroPT = CL.GetDouble("MaxDzeroPT", 2);  // Maximum Dzero transverse momentum threshold for Dzero selection.
  float MinDzeroY = CL.GetDouble("MinDzeroY", -2);   // Minimum Dzero rapidity threshold for Dzero selection.
  float MaxDzeroY = CL.GetDouble("MaxDzeroY", +2);   // Maximum Dzero rapidity threshold for Dzero selection.
  bool IsGammaN = CL.GetBool("IsGammaN", true);      // GammaN analysis (or NGamma)
  int TriggerChoice = CL.GetInt("TriggerChoice", 2); // 0 = no trigger sel, 1 = isL1ZDCOr, 2 = isL1ZDCXORJet8
  float scaleFactor = CL.GetDouble("scaleFactor", 1); // Scale factor for the number of events to be processed.
  int DoSystRapGap = CL.GetInt("DoSystRapGap", 0);   // Systematic study: apply the alternative event selections
                                                     // 0 = nominal, 1 = tight, -1: loose
                                                     // 9 < DoSystRapGap: use custom HF energy threshold, the threshold value will be DoSystRapGap/10.
  int DoSystD = CL.GetInt("DoSystD", 0);             // Systematic study: apply the alternative D selections
                                                     // 0 = nominal, 1 = Dsvpv variation, 2: DtrkPt variation
                                                     // 3 = Dalpha variation, 4: Dchi2cl variation

  bool IsData = CL.GetBool("IsData", 0);              // Data or MC
  Parameters par(MinDzeroPT, MaxDzeroPT, MinDzeroY, MaxDzeroY, IsGammaN, TriggerChoice, IsData, scaleFactor,
                 DoSystRapGap, DoSystD);
  par.input = CL.Get("Input", "mergedSample.root"); // Input file
  par.output = CL.Get("Output", "output.root");     // Output file
  par.nThread = CL.GetInt("nThread", 1);            // The number of threads to be used for parallel processing.
  par.nChunk =
      CL.GetInt("nChunk", 1); // Specifies which chunk (segment) of the data to process, used in parallel processing.
  if (checkError(par))
    return -1;
  std::cout << "Parameters are set" << std::endl;
  // Analyze Data
  DataAnalyzer analyzer(par.input.c_str(), par.output.c_str(), "");
  analyzer.analyze(par);
  analyzer.writeHistograms(analyzer.outf);
  saveParametersToHistograms(par, analyzer.outf);
  cout << "done!" << analyzer.outf->GetName() << endl;
}
