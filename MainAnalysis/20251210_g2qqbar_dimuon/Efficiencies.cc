//////////////////////////////////////////////////////////
//     ACCEPTANCE x EFFICIENCY MAP CALCULATION MACRO    //
////////////////////////////////////////////////////////// 

#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TTreeFormula.h>

#include <iostream>
#include <vector>

using namespace std;
#include "CommandLine.h" 
#include "DimuonMessenger.h"
#include "Messenger.h"   
#include "ProgressBar.h" 

using namespace std;

vector<int> isSelected(DimuonJetMessenger *Jet, float muPtCut, bool isData){
    // RETURNS A 3-INT VECTOR THAT ENCODES AT EACH INDEX IF THE JET PASSES THE GIVEN SELECTIONS
    // 0 - Inclusive Jet selection
    // 1 - Dimuon Jet selection
    // 2 - Gen Dimuon Jet selection
    vector<int> indices = {0, 0, 0};

    // TODO APPLY EVENT SELECTION HERE 
    // none for MC, ?? for Data.
    indices[0] = 1;

    if(!isData){

        // DIMUON SELECTIONS
        if(Jet->IsMuMuTagged == 1){
            indices[1] = 1;
            if(Jet->muPt1 < muPtCut || Jet->muPt2 < muPtCut){indices[1] = 0;}
            //if(!(Jet->mumuIsGenMatched)){indices[1] = 0;}
        }

        // GEN DIMUON SELECTIONS
        if(Jet->GenIsMuMuTagged == 1){
            indices[2] = 1;
            if(Jet->GenMuPt1 < muPtCut || Jet->GenMuPt2 < muPtCut){indices[2] = 0;}
        }
    }

    return indices;
}

int isGenSelected(GenDimuonJetMessenger *Jet){
    // RETURNS IF THE GEN JET IS SELECTED
    // TODO: POPULATE WITH UPDATED TRIGGER INFO
    return 1;
}

int main(int argc, char *argv[]) {

    // INPUTS
    cout << "Beginning Acceptance x Efficiency" << endl;
    CommandLine CL(argc, argv);
    string file = CL.Get("Input");
    string output = CL.Get("Output");
    bool isData = CL.GetBool("isData", false);
    vector<double> ptBins = CL.GetDoubleVector("ptBins");
    //int chargeSelection = CL.GetInt("chargeSelection",0);
    float muPtSelection = CL.GetDouble("muPt",3.5);
    int useWeightVariation = CL.GetInt("useWeightVariation",-1);

    // IMPORT TREE
    TFile* input = TFile::Open(file.c_str());
    DimuonJetMessenger *t = new DimuonJetMessenger(input, "Tree");
    GenDimuonJetMessenger *tgen = new GenDimuonJetMessenger(input, "GenTree");

    // DECLARE RECO HISTS
    TFile* outFile = new TFile(output.c_str(), "RECREATE");
    outFile->cd();
    TH2D* hRecoInclusiveJets = new TH2D("hRecoInclusiveJets","hRecoInclusiveJets", ptBins.size()-1, ptBins.front(), ptBins.back(), 10, -2.4, 2.4);
    TH2D* hGenDimJets = new TH2D("hGenDimJets","hGenDimJets", ptBins.size()-1, ptBins.front(), ptBins.back(), 10, -2.4, 2.4);
    TH2D* hRecoDimJets = new TH2D("hRecoDimJets","hRecoDimJets", ptBins.size()-1, ptBins.front(), ptBins.back(), 10, -2.4, 2.4);
    hRecoInclusiveJets->GetXaxis()->Set(ptBins.size()-1, ptBins.data()); 
    hGenDimJets->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
    hRecoDimJets->GetXaxis()->Set(ptBins.size()-1, ptBins.data());

    // DECLARE GEN HIST
    TH2D* hGenInclusiveJets = new TH2D("hGenInclusiveJets","hGenInclusiveJets", ptBins.size()-1, ptBins.front(), ptBins.back(), 10, -2.4, 2.4);
    hGenInclusiveJets->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
    
    // DECLARE NTUPLES
    TNtuple* ntGenInclusiveJets = new TNtuple("ntGenInclusiveJets","ntGenInclusiveJets", "JetPT:JetEta");
    TNtuple* ntRecoInclusiveJets = new TNtuple("ntRecoInclusiveJets","ntRecoInclusiveJets", "JetPT:JetEta"); // Trees only important for Inclusive jet unfolding. 

    // JET LOOP
    float weight = 0;
    unsigned long nentries = t->GetEntries();
    ProgressBar Bar(cout, nentries);
    for(int i = 0; i < t->GetEntries(); i++) { 

        if (i % 1000 == 0) {
            Bar.Update(i);
            Bar.Print();
        }
        
        t->GetEntry(i);
        vector<int> selected = isSelected(t, muPtSelection, isData); 

        if(selected[0] == 1){
            hRecoInclusiveJets->Fill(t->JetPT, t->JetEta);
            ntRecoInclusiveJets->Fill(t->JetPT, t->JetEta);
        }

        if(selected[1] == 1){
            hRecoDimJets->Fill(t->JetPT, t->JetEta);
        }

        if(selected[2] == 1){
            hGenDimJets->Fill(t->JetPT, t->JetEta);
        }
        
    }
    cout << " finished with reco jet loop" << endl;

    // GEN JET LOOP
    unsigned long genentries = tgen->GetEntries();
    ProgressBar Bar2(cout, genentries);
    for(int i = 0; i < genentries; i++) { 

        if(isData){break;}

        if(i % 1000 == 0) {
            Bar2.Update(i);
            Bar2.Print();
        }
        
        tgen->GetEntry(i);
        if(isGenSelected(tgen) == 1){
            hGenInclusiveJets->Fill(tgen->GenJetPT, tgen->GenJetEta);
            ntGenInclusiveJets->Fill(tgen->GenJetPT, tgen->GenJetEta);
        }

    }
    cout << " finished with gen jet loop" << endl;

    if(!isData){

        // TAKE RATIOS
        TH2D* JetEfficiency = (TH2D*)hRecoInclusiveJets->Clone();
        TH2D* DimJetEfficiency = (TH2D*)hRecoDimJets->Clone();
        JetEfficiency->Divide(hGenInclusiveJets);
        DimJetEfficiency->Divide(hGenDimJets);
        JetEfficiency->SetName("JetEfficiency");
        DimJetEfficiency->SetName("DimJetEfficiency");

        // SAVE TO FILE
        outFile->cd();
        hGenInclusiveJets->Write();
        ntGenInclusiveJets->Write();
        hGenDimJets->Write();
        hRecoDimJets->Write();
        JetEfficiency->Write();
        DimJetEfficiency->Write();
    }
    hRecoInclusiveJets->Write(); // WILL BE WRITTEN FOR BOTH DATA AND MC
    ntRecoInclusiveJets->Write();
    

    // SAVE COMMAND LINE PARAMS
    TNamed paramFile("InputFile", file.c_str());
    paramFile.Write();
    TNamed paramIsData("isData", isData ? "true" : "false");
    paramIsData.Write();
    //TNamed paramCharge("chargeSelection", std::to_string(chargeSelection).c_str());
    //paramCharge.Write();
    TNamed paramMuPt("muPtSelection", std::to_string(muPtSelection).c_str());
    paramMuPt.Write();
    TNamed paramWeightVar("useWeightVariation", std::to_string(useWeightVariation).c_str());
    paramWeightVar.Write();

    outFile->Close();
}








