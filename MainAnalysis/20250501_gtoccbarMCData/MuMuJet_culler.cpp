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

float dR(float eta1, float eta2, float phi1, float phi2){
    float deta = (eta1 - eta2)*(eta1 - eta2);
    float dphi = phi1 - phi2; 
    while (dphi > M_PI) dphi -= 2 * M_PI;
    while (dphi < -M_PI) dphi += 2 * M_PI;
    return sqrt(deta*deta + dphi*dphi);
}


class DataAnalyzer {
public:
  TFile *inf, *outf;
  MuMuJetMessenger *MMuMuJet;
  TNtuple *nt;
  string title;
  TTree*T;
  TTree*TJet;

  DataAnalyzer(const char *filename, const char *outFilename,
               const char *mytitle = "Data")
      : inf(new TFile(filename)),
        MMuMuJet(new MuMuJetMessenger(*inf, string("Tree"))),
        title(mytitle), outf(new TFile(outFilename, "recreate")) {
    outf->cd();
    nt = new TNtuple("nt","#mu^{+}#mu^{-} jet","MuMuMass");
    T = new TTree("mumutree","mumutree");   
    TJet = new TTree("jettree","jettree");
   
  }

  ~DataAnalyzer() {
    deleteHistograms();
    T->Delete();
    TJet->Delete();
    delete nt;
    inf->Close();
    outf->Close();
    delete MMuMuJet;
    
  }

  void analyze(Parameters &par) {

    cout << "c1" << endl;

    //branches of the skim
    float _Run = 0;
    float _Event = 0;

    float _Lumi = 0;
    float _hiBin = 0;
    float _hiHF = 0;
    float _NVertex = 0;
    float _VX = 0;
    float _VY = 0;
    float _VZ = 0;
    float _VXError = 0;
    float _VYError = 0;
    float _VZError = 0;
    float _NPU = 0;
    float _NCollWeight = 0; 
    float _EventWeight = 0;
    float _TotalWeight = 0;
    float _PTHat = 0;
    float _leadingjet_PTHat = 0;
    float _leadingPT = 0;
    
    //VECTORS
    float _JetPT = 0;
    float _JetEta = 0;
    float _JetPhi = 0;
    float _IsMuMuTagged = 0;
    float _genJetPT = 0;
    float _genJetEta = 0;
    float _genJetPhi = 0;
    float _genIsMuMuTagged = 0;
    float _recogenPT = 0;
    float _murecogenPT = 0;
    float _isleading = 0;
    //float _issubleading = 0;

    float _muPt1 = 0;
    float _muPt2 = 0;
    float _muEta1 = 0;
    float _muEta2 = 0;
    float _muPhi1 = 0;
    float _muPhi2 = 0;

    float _muDiDxy1 = 0;
    float _muDiDxy1Err = 0;
    float _muDiDxy2 = 0;
    float _muDiDxy2Err = 0;
    float _muDiDz1 = 0;
    float _muDiDz1Err = 0;
    float _muDiDz2 = 0;
    float _muDiDz2Err = 0;
    float _muDiDxy1Dxy2 = 0;
    float _muDiDxy1Dxy2Err = 0;
    float _muDiDz1Dz2Err = 0;
    float _muDiDz1Dz2 = 0;
    float _muDiDxysignificance1 = 0;
    float _muDiDxysignificance2 = 0;
    float _muDiDzsignificance1 = 0;
    float _muDiDzsignificance2 = 0;
    float _muDxy1Dxy2significance = 0;
    float _muDz1Dz2significance = 0;
    float _mudcaxydiff = 0;
    float _mudcazdiff = 0;
    float _totaldca = 0;

    float _mumuang11 = 0;
    float _mumuang12 = 0;
    float _mumuang112 = 0;
    float _mumutotang11 = 0;
    float _mumutotang12 = 0;
    float _mumutotang112 = 0;

    float _mumuz = 0; 
    float _mumuKT = 0;
    float _mumuMass = 0;
    float _mumuMasstransverse = 0;
    float _mumuEta = 0;
    float _mumuY = 0;
    float _mumuPhi = 0;
    float _mumuPt = 0;
    float _DRJetmu1 = 0;
    float _DRJetmu2 = 0;
    float _DRJetmumu = 0;
    float _muDPT = 0;
    float _muDeta = 0;
    float _muDphi = 0;
    float _muDR = 0;
    float _muPDR = 0;

    float _genMuPt1 = 0;
    float _genMuPt2 = 0;
    float _genMuEta1 = 0;
    float _genMuEta2 = 0;
    float _genMuPhi1 = 0;
    float _genMuPhi2 = 0;

    float _genMuMuz = 0;
    float _genMuMuMass = 0;
    float _genMuMuEta = 0;
    float _genMuMuY = 0;
    float _genMuMuPhi = 0;
    float _genMuMuPt = 0;
    float _genMuDPT = 0;
    float _genMuDeta = 0;
    float _genMuDphi = 0;
    float _genMuDR = 0;

    float _MJTHadronFlavor = 0;
    float _MJTNcHad = 0;
    float _MJTNbHad = 0;

    TJet->Branch("Run", &_Run, "Run/F");
    TJet->Branch("Event", &_Event, "Event/F");
    TJet->Branch("Lumi", &_Lumi, "Lumi/F");
    TJet->Branch("hiBin", &_hiBin, "hiBin/F");
    TJet->Branch("hiHF", &_hiHF, "hiHF/F");
    TJet->Branch("NVertex", &_NVertex, "NVertex/F");
    TJet->Branch("VX", &_VX ,"VX/F");
    TJet->Branch("VY", &_VY ,"VY/F");
    TJet->Branch("VZ", &_VZ ,"VZ/F");
    TJet->Branch("VXError", &_VXError ,"VXError/F");
    TJet->Branch("VYError", &_VYError ,"VYError/F");
    TJet->Branch("VZError", &_VZError ,"VZError/F");
    TJet->Branch("NPU", &_NPU , "NPU/F");
    TJet->Branch("NCollWeight", &_NCollWeight, "NCollWeight/F");
    TJet->Branch("EventWeight", &_EventWeight, "EventWeight/F");
    TJet->Branch("TotalWeight", &_TotalWeight, "TotalWeight/F");
    TJet->Branch("PTHat", &_PTHat, "PTHat/F");
    TJet->Branch("leadingjet_PTHat", &_leadingjet_PTHat, "leadingjet_PTHat/F");
    TJet->Branch("leadingPT", &_leadingPT, "leadingPT/F");

    TJet->Branch("JetPT", &_JetPT, "JetPT/F");
    TJet->Branch("JetEta", &_JetEta, "JetEta/F");
    TJet->Branch("JetPhi", &_JetPhi, "JetPhi/F");
    TJet->Branch("IsMuMuTagged", &_IsMuMuTagged, "IsMuMuTagged/F");
    TJet->Branch("genIsMuMuTagged", &_genIsMuMuTagged, "genIsMuMuTagged/F");
    TJet->Branch("genJetPT", &_genJetPT, "genJetPT/F");
    TJet->Branch("genJetEta", &_genJetEta, "genJetEta/F");
    TJet->Branch("genJetPhi", &_genJetPhi, "genJetPhi/F");
    TJet->Branch("recogenPT", &_recogenPT, "recogenPT/F");
   

    TJet->Branch("isleading", &_isleading, "isleading/F");


    T->Branch("Run", &_Run, "Run/F");
    T->Branch("Event", &_Event, "Event/F");
    T->Branch("Lumi", &_Lumi, "Lumi/F");
    T->Branch("hiBin", &_hiBin, "hiBin/F");
    T->Branch("hiHF", &_hiHF, "hiHF/F");
    T->Branch("NVertex", &_NVertex, "NVertex/F");
    T->Branch("VX", &_VX ,"VX/F");
    T->Branch("VY", &_VY ,"VY/F");
    T->Branch("VZ", &_VZ ,"VZ/F");
    T->Branch("VXError", &_VXError ,"VXError/F");
    T->Branch("VYError", &_VYError ,"VYError/F");
    T->Branch("VZError", &_VZError ,"VZError/F");
    T->Branch("NPU", &_NPU , "NPU/F");
    T->Branch("NCollWeight", &_NCollWeight, "NCollWeight/F");
    T->Branch("EventWeight", &_EventWeight, "EventWeight/F");
    T->Branch("TotalWeight", &_TotalWeight, "TotalWeight/F");
    T->Branch("isleading", &_isleading, "isleading/F");
    T->Branch("PTHat", &_PTHat, "PTHat/F");
    T->Branch("leadingjet_PTHat", &_leadingjet_PTHat, "leadingjet_PTHat/F");
    T->Branch("leadingPT", &_leadingPT, "leadingPT/F");

    T->Branch("JetPT", &_JetPT, "JetPT/F");
    T->Branch("JetEta", &_JetEta, "JetEta/F");
    T->Branch("JetPhi", &_JetPhi, "JetPhi/F");
    T->Branch("IsMuMuTagged", &_IsMuMuTagged, "IsMuMuTagged/F");
    T->Branch("genIsMuMuTagged", &_genIsMuMuTagged, "genIsMuMuTagged/F");
    T->Branch("genJetPT", &_genJetPT, "genJetPT/F");
    T->Branch("genJetEta", &_genJetEta, "genJetEta/F");
    T->Branch("genJetPhi", &_genJetPhi, "genJetPhi/F");
    T->Branch("recogenPT", &_recogenPT, "recogenPT/F");


    T->Branch("muPt1", &_muPt1, "muPt1/F");
    T->Branch("muPt2", &_muPt2, "muPt2/F");
    T->Branch("muEta1", &_muEta1, "muEta1/F");
    T->Branch("muEta2", &_muEta2, "muEta2/F");
    T->Branch("muPhi1", &_muPhi1, "muPhi1/F");
    T->Branch("muPhi2", &_muPhi2, "muPhi2/F");

    T->Branch("muDiDxy1", &_muDiDxy1, "muDiDxy1/F");
    T->Branch("muDiDxy1Err", &_muDiDxy1Err, "muDiDxy1Err/F");
    T->Branch("muDiDxy2", &_muDiDxy2, "muDiDxy2/F");
    T->Branch("muDiDxy2Err", &_muDiDxy2Err, "muDiDxy2Err/F");
    T->Branch("muDiDz1", &_muDiDz1, "muDiDz1/F");
    T->Branch("muDiDz1Err", &_muDiDz1Err, "muDiDz1Err/F");
    T->Branch("muDiDz2", &_muDiDz2, "muDiDz2/F");
    T->Branch("muDiDz2Err", &_muDiDz2Err, "muDiDz2Err/F");
    T->Branch("muDiDxy1Dxy2", &_muDiDxy1Dxy2, "muDiDxy1Dxy2/F");
    T->Branch("muDiDxy1Dxy2Err", &_muDiDxy1Dxy2Err, "muDiDxy1Dxy2Err/F");
    T->Branch("muDiDz1Dz2", &_muDiDz1Dz2, "muDiDz1Dz2/F");
    T->Branch("muDiDz1Dz2Err", &_muDiDz1Dz2Err, "muDiDz1Dz2Err/F");
    T->Branch("muDiDxysignificance1", &_muDiDxysignificance1, "muDiDxysignificance1/F");
    T->Branch("muDiDxysignificance2", &_muDiDxysignificance2, "muDiDxysignificance2/F");
    T->Branch("muDiDzsignificance1", &_muDiDzsignificance1, "muDiDzsignificance1/F");
    T->Branch("muDiDzsignificance2", &_muDiDzsignificance2, "muDiDzsignificance2/F");
    T->Branch("muDxy1Dxy2significance", &_muDxy1Dxy2significance, "muDxy1Dxy2significance/F");
    T->Branch("muDz1Dz2significance", &_muDz1Dz2significance, "muDz1Dz2significance/F");
    T->Branch("mudcaxydiff", &_mudcaxydiff, "mudcaxydiff/F");
    T->Branch("mudcazdiff", &_mudcazdiff, "mudcazdiff/F");
    T->Branch("totaldca",&_totaldca,"totaldca/F");

    T->Branch("mumuang11", &_mumuang11, "mumuang11/F");
    T->Branch("mumuang12", &_mumuang12, "mumuang12/F");
    T->Branch("mumuang112", &_mumuang112, "mumuang112/F");

    T->Branch("mumutotang11", &_mumutotang11, "mumutotang11/F");
    T->Branch("mumutotang12", &_mumutotang12, "mumutotang12/F");
    T->Branch("mumutotang112", &_mumutotang112, "mumutotang112/F");


    T->Branch("mumuKt", &_mumuKT, "mumuKt/F");
    T->Branch("mumuMass", &_mumuMass, "mumuMass/F");
    T->Branch("mumuMasstransverse", &_mumuMasstransverse, "mumuMasstransverse/F");
    T->Branch("mumuEta", &_mumuEta, "mumuEta/F");
    T->Branch("mumuY", &_mumuY, "mumuY/F");
    T->Branch("mumuPhi", &_mumuPhi, "mumuPhi/F");
    T->Branch("mumuMass", &_mumuMass, "mumuMass/F");
    T->Branch("mumuPt", &_mumuPt, "mumuPt/F");
    T->Branch("mumuz", &_mumuz, "mumuz/F");
    T->Branch("DRJetmu1", &_DRJetmu1, "DRJetmu1/F");
    T->Branch("DRJetmu2", &_DRJetmu2, "DRJetmu2/F");
    T->Branch("DRJetmumu", &_DRJetmumu, "DRJetmumu/F");
    T->Branch("muDPT", &_muDPT, "muDPT/F");
    T->Branch("muDeta", &_muDeta, "muDeta/F");
    T->Branch("muDphi", &_muDphi, "muDphi/F");
    T->Branch("muDR", &_muDR, "muDR/F");
    T->Branch("muPDR", &_muPDR, "muPDR/F");

    T->Branch("MJTHadronFlavor", &_MJTHadronFlavor, "MJTHadronFlavor/F");
    T->Branch("MJTNcHad", &_MJTNcHad, "MJTNcHad/F");
    T->Branch("MJTNbHad", &_MJTNbHad, "MJTNbHad/F");
    
    T->Branch("genMuPt1", &_genMuPt1, "genMuPt1/F");
    T->Branch("genMuPt2", &_genMuPt2, "genMuPt2/F");
    T->Branch("genMuEta1", &_genMuEta1, "genMuEta1/F");
    T->Branch("genMuEta2", &_genMuEta2, "genMuEta2/F");
    T->Branch("genMuPhi1", &_genMuPhi1, "genMuPhi1/F");
    T->Branch("genMuPhi2", &_genMuPhi2, "genMuPhi2/F");
    T->Branch("genMuMuMass", &_genMuMuMass, "genMuMuMass/F");
    T->Branch("genMuMuEta", &_genMuMuEta, "genMuMuEta/F");
    T->Branch("genMuMuY", &_genMuMuY, "genMuMuY/F");
    T->Branch("genMuMuPhi", &_genMuMuPhi, "genMuMuPhi/F");
    T->Branch("genMuMuMass", &_genMuMuMass, "genMuMuMass/F");
    T->Branch("genMuMuPt", &_genMuMuPt, "genMuMuPt/F");
    T->Branch("genMuDPT", &_genMuDPT, "genMuDPT/F");
    T->Branch("genMuDeta", &_genMuDeta, "genMuDeta/F");
    T->Branch("genMuDphi", &_genMuDphi, "genMuDphi/F");
    T->Branch("genMuDR", &_genMuDR, "genMuDR/F");
    T->Branch("murecogenPT", &_murecogenPT, "murecogenPT/F");


    outf->cd();
    unsigned long nentries = MMuMuJet->GetEntries();
    ProgressBar Bar(cout, nentries);
    Bar.SetStyle(1);
    for (int i = 0; i < MMuMuJet->GetEntries(); i++) {
      MMuMuJet->GetEntry(i);
      if (i % 1000 == 0) {
        Bar.Update(i);
        Bar.Print();
      }

      if (!eventSelection(MMuMuJet, par)){continue;}
	
      _Run = static_cast<float>(MMuMuJet->Run);
      _Event = static_cast<float>(MMuMuJet->Event);
      _Lumi = static_cast<float>(MMuMuJet->Lumi);
      _hiBin = static_cast<float>(MMuMuJet->hiBin)*0.5;
      _hiHF = MMuMuJet->hiHF;
      _NVertex = static_cast<float>(MMuMuJet->NVertex);
      _VX = MMuMuJet->VX;
      _VY = MMuMuJet->VY;
      _VZ = MMuMuJet->VZ;
      _VXError = MMuMuJet->VXError;
      _VYError = MMuMuJet->VYError;
      _VZError = MMuMuJet->VZError;
      _NPU = static_cast<float>(MMuMuJet->NPU);
      _NCollWeight = static_cast<float>(MMuMuJet->NCollWeight);
      //_EventWeight = MMuMuJet->EventWeight;
      //_PTHat = MMuMuJet->PTHat;
      _EventWeight = 1;
      _PTHat = 40000;
      _TotalWeight = _NCollWeight * _EventWeight;
      

      if(MMuMuJet->JetPT->size() > 0){
        _leadingjet_PTHat = MMuMuJet->JetPT->at(0) / _PTHat;
        _leadingPT = MMuMuJet->JetPT->at(0);

        for (int j = 0; j < MMuMuJet->JetPT->size(); j++){

          _JetPT = MMuMuJet->JetPT->at(j);
          _JetEta = MMuMuJet->JetEta->at(j);
          _JetPhi = MMuMuJet->JetPhi->at(j);
          _IsMuMuTagged = static_cast<float>(MMuMuJet->IsMuMuTagged->at(j));
          _genIsMuMuTagged = static_cast<float>(MMuMuJet->genIsMuMuTagged->at(j));
          _genJetPT = MMuMuJet->genJetPT->at(j);
          _genJetEta = MMuMuJet->genJetEta->at(j);
          _genJetPhi = MMuMuJet->genJetPhi->at(j);
          _recogenPT = _JetPT / _genJetPT;
         

          if(j == 0){
            _isleading = 1;
          }
          else{
            _isleading = 0;
          }
          
          TJet->Fill();
          
          if(MMuMuJet->mumuMass->at(j) > 0){
            _muPt1 = MMuMuJet->muPt1->at(j);
            _muPt2 = MMuMuJet->muPt2->at(j);
            _muEta1 = MMuMuJet->muEta1->at(j);
            _muEta2 = MMuMuJet->muEta2->at(j);
            _muPhi1 = MMuMuJet->muPhi1->at(j);
            _muPhi2 = MMuMuJet->muPhi2->at(j);
            _muDiDxy1 = abs(MMuMuJet->muDiDxy1->at(j));
            _muDiDxy1Err = MMuMuJet->muDiDxy1Err->at(j);
            _muDiDxy2 = abs(MMuMuJet->muDiDxy2->at(j));
            _muDiDxy2Err = MMuMuJet->muDiDxy2Err->at(j);
            _muDiDz1 = abs(MMuMuJet->muDiDz1->at(j));
            _muDiDz1Err = MMuMuJet->muDiDz1Err->at(j);
            _muDiDz2 = abs(MMuMuJet->muDiDz2->at(j));
            _muDiDz2Err = MMuMuJet->muDiDz2Err->at(j);
            _muDiDxy1Dxy2 = abs(MMuMuJet->muDiDxy1Dxy2->at(j));
            _muDiDxy1Dxy2Err = abs(MMuMuJet->muDiDxy1Dxy2Err->at(j));


            _muDiDz1Dz2 = abs(_muDiDz1*_muDiDz2);
            _muDiDxysignificance1 = _muDiDxy1/_muDiDxy1Err;
            _muDiDxysignificance2 = _muDiDxy2/_muDiDxy2Err;
            _muDiDzsignificance1 = _muDiDz1/_muDiDz1Err;
            _muDiDzsignificance2 = _muDiDz2/_muDiDz2Err;
            _muDxy1Dxy2significance = _muDiDxy1Dxy2/_muDiDxy1Dxy2Err;
            _muDiDz1Dz2Err = sqrt(_muDiDz1Err*_muDiDz1Err*_muDiDz2*_muDiDz2 + _muDiDz2Err*_muDiDz2Err*_muDiDz1*_muDiDz1);
            _muDz1Dz2significance = _muDiDz1Dz2/_muDiDz1Dz2Err;
            _mudcaxydiff = abs(_muDiDxy1 - _muDiDxy2);
            _mudcazdiff = abs(_muDiDz1 - _muDiDz2);
            _totaldca = sqrt(_muDiDxy1*_muDiDxy1 + _muDiDxy2*_muDiDxy2 + _muDiDz1*_muDiDz2);

            _mumuMass = MMuMuJet->mumuMass->at(j);
            _mumuEta = MMuMuJet->mumuEta->at(j);
            _mumuY = MMuMuJet->mumuY->at(j);
            _mumuPhi = MMuMuJet->mumuPhi->at(j);
            _mumuPt = MMuMuJet->mumuPt->at(j);
            _mumuz = _mumuPt/_JetPT;
            //_DRJetmu1 = MMuMuJet->DRJetmu1->at(j); 
            //_DRJetmu2 = MMuMuJet->DRJetmu2->at(j);
            _muDPT = (abs(_muPt1 - _muPt2))/(_muPt1 + _muPt2);
            _muDeta = MMuMuJet->muDeta->at(j);
            _muDphi = MMuMuJet->muDphi->at(j);
            _muDR = MMuMuJet->muDR->at(j);
            _muPDR = _muDR*_mumuPt;
    
            _mumuMasstransverse = sqrt(_mumuMass*_mumuMass + _mumuPt*_mumuPt);

            _DRJetmumu = dR(_mumuEta, _JetEta, _mumuPhi, _JetPhi);
            _DRJetmu1 = dR(_muEta1, _JetEta, _muPhi1, _JetPhi);
            _DRJetmu2 = dR(_muEta2, _JetEta, _muPhi2, _JetPhi);

            _mumutotang11 = (_mumuPt / _JetPT) * sqrt(pow((_mumuEta - _JetEta),2)+pow((_mumuPhi - _JetPhi),2));
            _mumutotang12 = (_mumuPt / _JetPT) * (pow((_mumuEta - _JetEta),2)+pow((_mumuPhi - _JetPhi),2));
            _mumutotang112 = (_mumuPt / _JetPT) * sqrt(sqrt(pow((_mumuEta - _JetEta),2)+pow((_mumuPhi - _JetPhi),2)));
            
            _mumuang11 = (_muPt1/_JetPT) * (_DRJetmu1/0.3) + (_muPt2/_JetPT) * (_DRJetmu2/0.3);
            _mumuang12 = (_muPt1/_JetPT) * (_DRJetmu1/0.3)*(_DRJetmu1/0.3) + (_muPt2/_JetPT) * (_DRJetmu2/0.3)*(_DRJetmu2/0.3);
            _mumuang112 = (_muPt1/_JetPT) * sqrt(abs(_DRJetmu1/0.3)) +  (_muPt2/_JetPT) * sqrt(abs(_DRJetmu2/0.3));        

            _mumuKT = _mumuPt*_DRJetmumu;  

            _genMuPt1 = MMuMuJet->genMuPt1->at(j);
            _genMuPt2 = MMuMuJet->genMuPt2->at(j);
            _genMuEta1 = MMuMuJet->genMuEta1->at(j);
            _genMuEta2 = MMuMuJet->genMuEta2->at(j);
            _genMuPhi1 = MMuMuJet->genMuPhi1->at(j);
            _genMuPhi2 = MMuMuJet->genMuPhi2->at(j);
            _genMuMuMass = MMuMuJet->genMuMuMass->at(j);
            _genMuMuEta = MMuMuJet->genMuMuEta->at(j);
            _genMuMuY = MMuMuJet->genMuMuY->at(j);
            _genMuMuPhi = MMuMuJet->genMuMuPhi->at(j);
            _genMuMuPt = MMuMuJet->genMuMuPt->at(j);
            _genMuMuz = _genMuMuPt / _genJetPT;
            _genMuDPT = (abs(_genMuPt1 - _genMuPt2))/(_genMuPt1 + _genMuPt2);
            _genMuDeta = MMuMuJet->genMuDeta->at(j);
            _genMuDphi = MMuMuJet->genMuDphi->at(j);
            _genMuDR = MMuMuJet->genMuDR->at(j);
            _murecogenPT = _mumuPt / _genMuMuPt;


            _MJTHadronFlavor = static_cast<float>(MMuMuJet->MJTHadronFlavor->at(j));
            _MJTNcHad = static_cast<float>(MMuMuJet->MJTNcHad->at(j));
            _MJTNbHad = static_cast<float>(MMuMuJet->MJTNbHad->at(j));

            T->Fill();
          }
          
        }
      }

    }
  }

  void writeHistograms(TFile *outf) {
    outf->cd();
    smartWrite(T);
    smartWrite(TJet);
    smartWrite(nt);
  }

private:
  void deleteHistograms() {

  }
};

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {
  if (printHelpMessage(argc, argv))
    return 0;
  CommandLine CL(argc, argv);
  float MinJetPT = CL.GetDouble("MinJetPt", 80); // Minimum jet pT
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
  // Analyze Data
  DataAnalyzer analyzer(par.input.c_str(), par.output.c_str(), "Data");
  analyzer.analyze(par);
  analyzer.writeHistograms(analyzer.outf);
  saveParametersToHistograms(par, analyzer.outf);
  cout << "done!" << analyzer.outf->GetName() << endl;
}

