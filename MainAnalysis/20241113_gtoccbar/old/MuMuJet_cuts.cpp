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
bool checkError(const Parameters &par) {
  return false;
}

//======= eventSelection =====================================//
// Check if the event pass eventSelection criteria
//============================================================//
bool eventSelection(MuMuJetMessenger *b, const Parameters &par) {
  return true;
}

class DataAnalyzer {
public:
  TFile *inf, *outf;

  //histograms
  TH1D *hMuMuMass;
  TH1D *hMuMupT;
  TH1D *hMupT1;
  TH1D *hMupT2;
  TH2D *hMuMupT1pT2;
  TH1D *hJetpT;
  TH1D *hAsymmetry;
  TH1D *hdR;
  TH1D *hdPhi;
  TH1D *hMuJetFraction;
  TH1D *hdpT;
  TH1D *hdca1;
  TH1D *hdca2;
  TH1D *hdca_product;
  TH1D *hdcaAsymmetry;
  TH1D *hdca1z;
  TH1D *hdca2z;
  TH1D *hdca_productz;
  TH1D *hdcaAsymmetryz;
  TH1D *hdcasig1;
  TH1D *hdcasig2;
  TH1D *hdcasig_product;
  TH1D *hdcasigAsymmetry;
  TH1D *hdcasig1z;
  TH1D *hdcasig2z;
  TH1D *hdcasig_productz;
  TH1D *hdcasig_Asymmetryz; 
  TH2D *hdca12;
  TH2D *hdRpT;
  TH2D *hdRjetpT;
  



  MuMuJetMessenger *MMuMuJet;
  TNtuple *nt;
  string title;

  DataAnalyzer(const char *filename, const char *outFilename,
               const char *mytitle = "Data")
      : inf(new TFile(filename)),
        MMuMuJet(new MuMuJetMessenger(*inf, string("Tree"))),
        title(mytitle), outf(new TFile(outFilename, "recreate")) {
    outf->cd();
    nt = new TNtuple("nt", "#mu^{+}#mu^{-} jet", "MuMuMass");
  }

  ~DataAnalyzer() {
    deleteHistograms();
    delete nt;
    inf->Close();
    outf->Close();
    delete MMuMuJet;
  }

  void analyze(Parameters &par,
  float min_dR, 
  float max_dR,
  float min_di_pT,
  float max_di_pT,
  float min_jet_pT,
  float max_jet_pT,
  float min_asymmetry,
  float max_asymmetry,
  float max_drjetmu,
  int nC_hadrons,
  int nB_hadrons,
  double dca_min,
  double dca_max,
  double dca_prod_min,
  double dca_prod_max,
  double muptmin,
  double muptmax
  ) {
    outf->cd();

    //histograms
    hMuMuMass = new TH1D(Form("hMuMuMass%s", title.c_str()), "", 500, 0, 50);
    hMuMupT = new TH1D(Form("hMuMuPt%s", title.c_str()), "", 50, 0, 200);
    hMupT1 = new TH1D(Form("hMuPt1%s", title.c_str()), "", 50, 0, 200);
    hMupT2 = new TH1D(Form("hMuPt2%s", title.c_str()), "", 50, 0, 200);
    hMuMupT1pT2 = new TH2D("hMupT1pT2","hMupT1pT2",50,0,200,50,0,200);
    hJetpT = new TH1D(Form("hJetpT%s", title.c_str()), "", 200, 0, 500);
    hAsymmetry = new TH1D(Form("hAsymmetry%s", title.c_str()), "", 20, 0, 1);
    
    hdR = new TH1D(Form("hdR%s", title.c_str()), "", 150, 0, 3.1416);
    hdPhi = new TH1D(Form("hdPhi%s", title.c_str()),"",150,0,3.1416);
    hdpT = new TH1D(Form("hdpT%s", title.c_str()), "", 50, 0, 200);
    hMuJetFraction = new TH1D(Form("hMuJetFraction%s", title.c_str()), "", 20, 0, 1);
    
    hdca1 = new TH1D(Form("hdca1%s", title.c_str()), "", 50, -10, 1);
    hdca2 = new TH1D(Form("hdca2%s", title.c_str()), "", 50, -10, 1);
    hdca_product = new TH1D(Form("hdca1x2%s", title.c_str()), "", 100, -20, 1);
    hdcaAsymmetry = new TH1D(Form("hdcaAsymmetry%s", title.c_str()), "", 50, 0, 1);

    hdca1z = new TH1D(Form("hdca1z%s", title.c_str()), "", 50, -10, 1);
    hdca2z = new TH1D(Form("hdca2z%s", title.c_str()), "", 50, -10, 1);
    hdca_productz = new TH1D(Form("hdca1x2z%s", title.c_str()), "", 100, -20, 1);
    hdcaAsymmetryz = new TH1D(Form("hdcaAsymmetryz%s", title.c_str()), "", 50, 0, 1);

    hdcasig1 = new TH1D(Form("hdcasig1%s", title.c_str()), "", 50, -10, 1);
    hdcasig2 = new TH1D(Form("hdcasig2%s", title.c_str()), "", 50, -10, 1);
    hdcasig_product = new TH1D(Form("hdcasig1x2%s", title.c_str()), "", 100, -20, 1);
    hdcasigAsymmetry = new TH1D(Form("hdcasigAsymmetry%s", title.c_str()), "", 50, 0, 1);

    hdcasig1z = new TH1D(Form("hdcasig1z%s", title.c_str()), "", 50, -10, 1);
    hdcasig2z = new TH1D(Form("hdcasig2z%s", title.c_str()), "", 50, -10, 1);
    hdcasig_productz = new TH1D(Form("hdcasig1x2z%s", title.c_str()), "", 100, -20, 1);
    hdcasigAsymmetryz = new TH1D(Form("hdcasigAsymmetryz%s", title.c_str()), "", 50, 0, 1);
  
    hdca12 = new TH2D(Form("hdca12%s", title.c_str()), "", 50, -10, 1, 50, -10, 1);
    hdRpT = new TH2D("hdRpT","hdRpT",150,0,3.1416,80,0,200);
    hdRjetpT = new TH2D("hdRjetpT","hdRjetpT",150,0,3.1416,200,0,500);
    
    
    
  

    unsigned long nentries = MMuMuJet->GetEntries();
    ProgressBar Bar(cout, nentries);
    Bar.SetStyle(1);

    float MuMu_dPhi = 0;
    float MuMu_dR = 0;
    float MuMu_di_pT = 0;
    float MuMu_asymmetry = 0;
    float aux1 = 0;
    float aux2 = 0;
    int ncs = 0;
    int nbs = 0;
    int counter = 0;
    int counter2 = 0;

   

    for (int i = 0; i < MMuMuJet->GetEntries(); i++) {
      MMuMuJet->GetEntry(i);
      if (i % 1000 == 0) {
	      Bar.Update(i);
	      Bar.Print();
      }
      if (!eventSelection(MMuMuJet, par))continue;
      for (int j = 0; j < MMuMuJet->mumuMass->size(); j++){

        counter +=1;
        
        // cut on # of C hadrons
        if(nC_hadrons != -1){
          if(MMuMuJet->MJTNcHad->at(j) != nC_hadrons){
            continue;
          }

        }
        // cut on # of B hadrons 
        if(nB_hadrons != -1){
          if(MMuMuJet->MJTNbHad->at(j) != nB_hadrons){
              continue;
            }
        }
        
        //jet pT condition
        if((MMuMuJet->JetPT->at(j) < min_jet_pT)||(MMuMuJet->JetPT->at(j) > max_jet_pT)){
          continue;
        }

        // OPTIONAL: UNCOMMENT TO EXCLUDE THE J/PSI MASS WINDOW
        //if((MMuMuJet->mumuMass->at(j) < 3.2)&&(MMuMuJet->mumuMass->at(j) > 3.0)){
        //  cout << "excluded: " <<  MMuMuJet->mumuMass->at(j) << endl;
        //    continue;
        //}

        
        // OPTIONAL: UNCOMMENT TO MAKE A STRICTER CUT ON DIMUON COLLINEARITY
        //if((MMuMuJet->DRJetmu1->at(j) > max_drjetmu)||(MMuMuJet->DRJetmu2->at(j) > max_drjetmu)){
        //    continue;
        //};
        
        // Individual Muon pT
        if((MMuMuJet->muPt1->at(j) < muptmin) || (MMuMuJet->muPt2->at(j)) < muptmin){
          continue;
        }
        if((MMuMuJet->muPt1->at(j) > muptmax) || (MMuMuJet->muPt2->at(j)) > muptmax){
          continue;
        }
        
        
        // dR condition
        aux1 = abs(MMuMuJet->muDR->at(j)-6.28319);
        MuMu_dR = min(MMuMuJet->muDR->at(j),aux1); 
        cout << endl;
        cout << MuMu_dR << endl;

        aux2 = abs(MMuMuJet->muDphi->at(j)-6.28319);
        MuMu_dPhi = min(MMuMuJet->muDphi->at(j),aux2);

        if((MuMu_dR > max_dR)||(MuMu_dR < min_dR)){ 
            continue;
        } 
        
        // pT condition 
        MuMu_di_pT = MMuMuJet->mumuPt->at(j);   
        if((MuMu_di_pT > max_di_pT)||(MuMu_di_pT < min_di_pT)){
            continue;
        } 
        // asymmetry condition
        MuMu_asymmetry = abs(MMuMuJet->muPt1->at(j) - MMuMuJet->muPt2->at(j))/(MMuMuJet->muPt1->at(j) + MMuMuJet->muPt2->at(j));      
        if((MuMu_asymmetry > max_asymmetry)||(MuMu_asymmetry < min_asymmetry)){
            continue;
        }
        
        // dca cuts
        if((abs(MMuMuJet->muDiDxy1->at(j)) > dca_max)||(abs(MMuMuJet->muDiDxy1->at(j)) < dca_min)){
            continue;
        };
        if((abs(MMuMuJet->muDiDxy2->at(j)) > dca_max)||(abs(MMuMuJet->muDiDxy2->at(j)) < dca_min)){
            continue;
        };
        
        // dca product condition
        if(((log10(sqrt(abs(MMuMuJet->muDiDxy1->at(j))*(abs(MMuMuJet->muDiDxy2->at(j)))))) < dca_prod_min)||((log10(sqrt(abs(MMuMuJet->muDiDxy1->at(j))*(abs(MMuMuJet->muDiDxy2->at(j)))))) > dca_prod_max)){
           continue;
        }
        
        counter2 += 1;
        

        hMuMuMass->Fill(MMuMuJet->mumuMass->at(j));
        hMuMupT->Fill(MMuMuJet->mumuPt->at(j));
        hMupT1->Fill(MMuMuJet->muPt1->at(j));
        hMupT2->Fill(MMuMuJet->muPt2->at(j));
        hMuMupT1pT2->Fill(MMuMuJet->muPt1->at(j),MMuMuJet->muPt2->at(j));
        hJetpT->Fill(MMuMuJet->JetPT->at(j));
        hAsymmetry->Fill(MuMu_asymmetry);
        hMuJetFraction->Fill(1.0*MMuMuJet->mumuPt->at(j)/MMuMuJet->JetPT->at(j));

        hdR->Fill(MuMu_dR);
        hdPhi->Fill(MuMu_dPhi);
        hdRpT->Fill(MuMu_dR,MMuMuJet->mumuPt->at(j));
        hdRjetpT->Fill(MuMu_dR,MMuMuJet->JetPT->at(j));
        hdpT->Fill(abs(MMuMuJet->muPt1->at(j) - MMuMuJet->muPt2->at(j)));


        /// NEEDA FIX ONG!
        hdca1->Fill(log10(abs(MMuMuJet->muDiDxy1->at(j))));
        hdca2->Fill(log10(abs(MMuMuJet->muDiDxy2->at(j))));
        hdcaAsymmetry->Fill(abs(log(abs(MMuMuJet->muDiDxy1->at(j))) - log(abs(MMuMuJet->muDiDxy2->at(j))))/abs((log(abs(MMuMuJet->muDiDxy1->at(j))) + log(abs(MMuMuJet->muDiDxy2->at(j))))));
        hdca_product->Fill(log10(sqrt(abs(MMuMuJet->muDiDxy1->at(j))*(abs(MMuMuJet->muDiDxy2->at(j))))));

        hdcasig1->Fill(log10(abs(MMuMuJet->muDiDxy1->at(j))));
        hdcasig2->Fill(log10(abs(MMuMuJet->muDiDxy2->at(j))));
        hdcasigAsymmetry->Fill(abs(log(abs(MMuMuJet->muDiDxy1->at(j))) - log(abs(MMuMuJet->muDiDxy2->at(j))))/abs((log(abs(MMuMuJet->muDiDxy1->at(j))) + log(abs(MMuMuJet->muDiDxy2->at(j))))));
        hdcasig_product->Fill(log10(sqrt(abs(MMuMuJet->muDiDxy1->at(j))*(abs(MMuMuJet->muDiDxy2->at(j))))));
        
        hdca1z->Fill(log10(abs(MMuMuJet->muDiDxy1->at(j))));
        hdca2z->Fill(log10(abs(MMuMuJet->muDiDxy2->at(j))));
        hdcaAsymmetryz->Fill(abs(log(abs(MMuMuJet->muDiDxy1->at(j))) - log(abs(MMuMuJet->muDiDxy2->at(j))))/abs((log(abs(MMuMuJet->muDiDxy1->at(j))) + log(abs(MMuMuJet->muDiDxy2->at(j))))));
        hdca_productz->Fill(log10(sqrt(abs(MMuMuJet->muDiDxy1->at(j))*(abs(MMuMuJet->muDiDxy2->at(j))))));

        hdcasig1z->Fill(log10(abs(MMuMuJet->muDiDxy1->at(j))));
        hdcasig2z->Fill(log10(abs(MMuMuJet->muDiDxy2->at(j))));
        hdcasigAsymmetryz->Fill(abs(log(abs(MMuMuJet->muDiDxy1->at(j))) - log(abs(MMuMuJet->muDiDxy2->at(j))))/abs((log(abs(MMuMuJet->muDiDxy1->at(j))) + log(abs(MMuMuJet->muDiDxy2->at(j))))));
        hdcasig_productz->Fill(log10(sqrt(abs(MMuMuJet->muDiDxy1->at(j))*(abs(MMuMuJet->muDiDxy2->at(j))))));
        
        hdca12->Fill(log10(abs(MMuMuJet->muDiDxy1->at(j))),log10(abs(MMuMuJet->muDiDxy2->at(j))));
        nt->Fill(MMuMuJet->mumuMass->at(j));
      }
    }
    cout << endl;
    cout << "total # of muons: " << counter << endl;
    cout << "counted: " << counter2 << endl; 
  }

  void writeHistograms(TFile *outf) {
    outf->cd();
    smartWrite(hMuMuMass);
    smartWrite(hMuMupT);
    smartWrite(hMupT1);
    smartWrite(hMupT2);
    smartWrite(hMuMupT1pT2);
    smartWrite(hJetpT);
    smartWrite(hAsymmetry);
    smartWrite(hdR);
    smartWrite(hdPhi);
    smartWrite(hMuJetFraction);
    smartWrite(hdRpT);
    smartWrite(hdRjetpT);
    smartWrite(hdpT);
    smartWrite(hdca1);
    smartWrite(hdca2);
    smartWrite(hdca12);
    smartWrite(hdca_product);
    smartWrite(hdcaAsymmetry);
    smartWrite(nt);
  }

private:
  void deleteHistograms() { 
    delete hMuMuMass; 
    delete hMuMupT;
    delete hMupT1;
    delete hMupT2;
    delete hMuMupT1pT2;
    delete hJetpT;
    delete hAsymmetry;
    delete hdR;
    delete hdPhi;
    delete hMuJetFraction;
    delete hdRpT;
    delete hdRjetpT;
    delete hdca1;
    delete hdca2;
    delete hdca12;
    delete hdca_product;
    delete hdcaAsymmetry;
    delete hdpT; }
};

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {
  if (printHelpMessage(argc, argv))
    return 0;
  CommandLine CL(argc, argv);
  float MinJetPT = CL.GetDouble("MinJetPt", 40); // Minimum jet pT
  bool IsData = CL.GetBool("IsData", 1); // Data or MC
  bool TriggerChoice = CL.GetBool("TriggerChoice", 1); // Which trigger to use
  float scaleFactor = CL.GetDouble("scaleFactor", 1.); // Scale factor for the output
  Parameters par(MinJetPT, TriggerChoice, IsData, scaleFactor);
  par.input         = CL.Get      ("Input",   "mergedSample.root");            // Input file
  par.output        = CL.Get      ("Output",  "output.root");                             	// Output file
  par.nThread       = CL.GetInt   ("nThread", 1);         // The number of threads to be used for parallel processing.
  par.nChunk        = CL.GetInt   ("nChunk", 1);          // Specifies which chunk (segment) of the data to process, used in parallel processing.
  if (checkError(par)) return -1;
  std::cout << "Parameters are set" << std::endl;

  float min_dR = CL.GetDouble("min_dR", 0);
  float max_dR = CL.GetDouble("max_dR", 3.1416);
  float min_di_pT = CL.GetDouble("min_di_pT", 0);
  float max_di_pT = CL.GetDouble("max_di_pT", 5000);
  float min_jet_pT = CL.GetDouble("min_jet_pT", 0);
  float max_jet_pT = CL.GetDouble("max_jet_pT", 5000);
  float min_asymmetry = CL.GetDouble("min_asymmetry", -1);
  float max_asymmetry = CL.GetDouble("max_asymmetry", 2);
  float max_drjetmu = CL.GetDouble("max_drjetmu", 100);
  int nC_hadrons = CL.GetDouble("nCh", -1);
  int nB_hadrons = CL.GetDouble("nBh", -1);
  double dca_min = CL.GetDouble("min_dca", -1);
  double dca_max = CL.GetDouble("max_dca", 100);
  double dca_prod_min = CL.GetDouble("min_dca_prod",-1000);
  double dca_prod_max = CL.GetDouble("max_dca_prod",2000);
  double muptmin = CL.GetDouble("min_mu_pT",0);
  double muptmax = CL.GetDouble("max_mu_pT",1000);
 


  
  // Analyze Data
  DataAnalyzer analyzer(par.input.c_str(), par.output.c_str(), "Data");
  analyzer.analyze(par, min_dR, max_dR, min_di_pT, max_di_pT, min_jet_pT, max_jet_pT, min_asymmetry, max_asymmetry, max_drjetmu, nC_hadrons, nB_hadrons, dca_min, dca_max, dca_prod_min, dca_prod_max, muptmin,muptmax);
  analyzer.writeHistograms(analyzer.outf);
  saveParametersToHistograms(par, analyzer.outf);
  cout << "done!" << analyzer.outf->GetName() << endl;
}
