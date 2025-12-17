///////////////////////////////////////////////////////////////////////
//        CONSTRUCTS DIMUON DCA AND INVARIANT MASS CURVES            //
///////////////////////////////////////////////////////////////////////

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

bool isDimuonSelected(DimuonJetMessenger *Dimuon, float muPtCut, int chargeSelection, bool isData){

    // IMPLEMENT EVENT SELECTION HERE FOR DATA JETS
    // NOTHING HAPPENS HERE IF MC

    if(!Dimuon->IsMuMuTagged){
        return false;
    }

    if(Dimuon->muPt1 < muPtCut || Dimuon->muPt2 < muPtCut){
        return false;
    }

    if(Dimuon->muCharge1 * Dimuon->muCharge2 != chargeSelection){
        return false;
    }

    return true;
}

int getFlavorClass(int NbHad, int NcHad) {
  if (NbHad == 2)
    return 5; // bbbar
  else if (NbHad == 1)
    return 4; // b
  else if ((NcHad == 2) && (NbHad == 0))
    return 3; // ccbar
  else if ((NcHad == 1) && (NbHad == 0))
    return 2; // c
  else if ((NbHad == 0) && (NcHad == 0))
    return 1; // light
  else 
    return 0; // other
}

int main(int argc, char *argv[]) {

    // INPUTS
    cout << "Filling Distributions" << endl;
    CommandLine CL(argc, argv);
    string file = CL.Get("Input");
    string efficiencies = CL.Get("Input_Efficiency");
    string output = CL.Get("Output");
    bool isData = CL.GetBool("IsData");
    int chargeSelection = CL.GetInt("chargeSelection", 1);
    vector<double> ptBins = CL.GetDoubleVector("ptBins");
    float muPtSelection = CL.GetDouble("muPt",3.5);
    int useWeightVariation = CL.GetInt("useWeightVariation",0);

    // IMPORT TREE
    TFile* input = TFile::Open(file.c_str());
    DimuonJetMessenger *t = new DimuonJetMessenger(input, "Tree");

    // IMPORT EFFICIENCIES
    TFile* effFile = TFile::Open(efficiencies.c_str());
    TH2D* DimJetEfficiency = (TH2D*)effFile->Get("DimJetEfficiency");

    // OUTPUT FILE
    TFile* outFile = new TFile(output.c_str(), "RECREATE");
    outFile->cd();
    
    // HISTOGRAMS + NTUPLE
    TH2D* hInvMass = new TH2D("hInvMass", "", ptBins.size()-1, ptBins.front(), ptBins.back(), 70, 0, 10);
    TH2D* hDCAProductSig = new TH2D("hDCAProductSig", "", ptBins.size()-1, ptBins.front(), ptBins.back(), 50, -3, 4);
    TH2D* hmuDR = new TH2D("hmuDR", "", ptBins.size()-1, ptBins.front(), ptBins.back(), 50, 0, 0.6);
    hInvMass->GetXaxis()->Set(ptBins.size()-1, ptBins.data()); 
    hDCAProductSig->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
    hmuDR->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
    TH2D* hEfficiency = (TH2D*)DimJetEfficiency->Clone("hEfficiency");
    hEfficiency->Reset();
    TNtuple* ntDimuon = new TNtuple("ntDimuon", "", "mumuMass:muDiDxy1Dxy2Sig:muDR:JetPT:weight");

    // FLAVOR HISTOGRAMS + NTUPLES (FOR TEMPLATES)
    vector<TH2D*> hInvMass_flavors;     
    vector<TH2D*> hmuDCAProductSig_flavors;
    vector<TH2D*> hmuDR_flavors;
    vector<TH2D*> hEfficiency_flavors;
    vector<TNtuple*> nt_flavors;
    vector<string> flavorNames;
    flavorNames = {"other", "uds", "c", "cc", "b", "bb"};
    for(int i = 0; i < 6; i++) {
        hInvMass_flavors.push_back(new TH2D(Form("hInvMass_%s", flavorNames[i].c_str()), "", ptBins.size()-1, ptBins.front(), ptBins.back(), 70, 0, 10));
        hmuDCAProductSig_flavors.push_back(new TH2D(Form("hDCAProductSig_%s", flavorNames[i].c_str()), "", ptBins.size()-1, ptBins.front(), ptBins.back(), 50, -3, 4));
        hmuDR_flavors.push_back(new TH2D(Form("hmuDR_%s", flavorNames[i].c_str()), "", ptBins.size()-1, ptBins.front(), ptBins.back(), 50, 0, 0.6));
        hInvMass_flavors[i]->GetXaxis()->Set(ptBins.size()-1, ptBins.data()); 
        hmuDCAProductSig_flavors[i]->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
        hmuDR_flavors[i]->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
        hEfficiency_flavors.push_back((TH2D*)DimJetEfficiency->Clone(Form("hEfficiency_%s", flavorNames[i].c_str())));
        hEfficiency_flavors[i]->Reset();
        nt_flavors.push_back(new TNtuple(Form("nt_%s", flavorNames[i].c_str()), "", "mumuMass:muDiDxy1Dxy2Sig:muDR:JetPT:weight"));
    }

    // JETS LOOP
    float weight = 0;
    unsigned long nentries = t->GetEntries();
    ProgressBar Bar(cout, nentries);
    for(int i = 0; i < nentries; i++){

        if (i % 1000 == 0) {
            Bar.Update(i);
            Bar.Print();
        }

        t->GetEntry(i);

        if(isDimuonSelected(t, muPtSelection, chargeSelection, isData)){
            
            weight = 1 / DimJetEfficiency->GetBinContent(DimJetEfficiency->FindBin(t->JetPT, t->JetEta)); // MAY WANT TO MAKE MORE ROBUST FOR DIVIDE BY ZEROS
            weight *= t->MuMuWeight;

            hInvMass->Fill(t->JetPT, t->mumuMass, weight);
            hDCAProductSig->Fill(t->JetPT, log10(abs(t->muDiDxy1Dxy2 / t->muDiDxy1Dxy2Err)), weight);
            hmuDR->Fill(t->JetPT, t->muDR, weight);
            ntDimuon->Fill(t->mumuMass, log10(abs(t->muDiDxy1Dxy2 / t->muDiDxy1Dxy2Err)), t->muDR, t->JetPT, weight); // NOTE MAY WANT TO JUST PUT THE SIGNIFICANCE WITHOUT THE LOG FOR FUTURE REFERENCE
            hEfficiency->Fill(t->JetPT, t->JetEta, 1/weight);

            if(isData){continue;} // ONLY MAKE TEMPLATES WITH MC 

            int flavorclass = getFlavorClass(t->NcHad, t->NbHad);
            hInvMass_flavors[flavorclass]->Fill(t->JetPT, t->mumuMass, weight);
            hmuDCAProductSig_flavors[flavorclass]->Fill(t->JetPT, log10(abs(t->muDiDxy1Dxy2 / t->muDiDxy1Dxy2Err)), weight);
            hmuDR_flavors[flavorclass]->Fill(t->JetPT, t->muDR, weight);
            nt_flavors[flavorclass]->Fill(t->mumuMass, log10(abs(t->muDiDxy1Dxy2 / t->muDiDxy1Dxy2Err)), t->muDR, t->JetPT, weight);
            hEfficiency_flavors[flavorclass]->Fill(t->JetPT, t->JetEta, 1/weight);

        }
        
    }
    cout << " END OF JET LOOP: SAVING OUTPUTS TO FILE" << endl;

    outFile->cd();
    hInvMass->Write();
    hDCAProductSig->Write();
    hmuDR->Write();
    ntDimuon->Write();
    hEfficiency->Write();
    if(!isData){
        for(int i = 0; i < 6; i++) {
            hInvMass_flavors[i]->Write();
            hmuDCAProductSig_flavors[i]->Write();
            hmuDR_flavors[i]->Write();
            nt_flavors[i]->Write();
            hEfficiency_flavors[i]->Write();
        }
    }
    
    // SAVE COMMAND LINE PARAMS
    TNamed paramFile("InputFile", file.c_str());
    paramFile.Write();
    TNamed paramEffFile("Input_Efficiency", efficiencies.c_str());
    paramEffFile.Write();
    TNamed paramIsData("IsData", isData ? "true" : "false");
    paramIsData.Write();
    TNamed paramCharge("chargeSelection", std::to_string(chargeSelection).c_str());
    paramCharge.Write();
    TNamed paramMuPt("muPtSelection", std::to_string(muPtSelection).c_str());
    paramMuPt.Write();
    TNamed paramWeightVar("useWeightVariation", std::to_string(useWeightVariation).c_str());
    paramWeightVar.Write();
    
    outFile->Close();
}