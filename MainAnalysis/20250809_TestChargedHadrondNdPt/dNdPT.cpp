#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TSystem.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TH1D.h>

using namespace std;

int dNdPT(std::string inputFileName,
        bool ApplyEventWeight = true,
        bool ApplyTrackWeight = true,
        bool ApplyTrackSpeciesWeight = true,
        float trkptcut = 0.4,
        float trketacut = 2.4,
        std::string outFileName = "dNdPT_output.root",
        bool debug = true) {

    /// INPUTING FILES 
    cout << "————————Inputting files————————" << endl;
    TFile *file = TFile::Open(inputFileName.c_str());
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file: " << inputFileName << std::endl;
        return 1;
    }

    TTree *tree = (TTree*)file->Get("Tree");
    if (!tree) {
        std::cerr << "Error retrieving tree from file: " << inputFileName << std::endl;
        return 1;
    }
    cout << "————————Inputting files done————————" << endl;

    const Double_t ptBins[] = {
        0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9,
        1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9,
        2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0,
        12.0, 14.0, 16.0, 18.0, 20.0, 25.0, 30.0, 35.0, 40.0, 50.0
    };
    const Int_t nBins = sizeof(ptBins)/sizeof(ptBins[0]) - 1;

    TH1D* h_dndpt = new TH1D("h_dndpt", "dN/dpT; pT (GeV/c); dN/dpT", nBins, ptBins);  
    h_dndpt->Sumw2();

    Bool_t HLT_MinimumBiasHF_OR_BptxAND_v1, pBES;
    Int_t PVFilter, ClusterCompatibilityFilter;

    Float_t VZ, HFEMaxPlus, HFEMaxMinus, eventEfficiencyWeight_Nominal;
    vector<float>* trackingEfficiency_Nominal = nullptr;
    vector<float>* TrkSpeciesWeight_dNdEta40 = nullptr;
    vector<float>* trkPt = nullptr;
    vector<float>* trkEta = nullptr;
    vector<bool>* trkPassChargedHadron_Nominal = nullptr;
    tree->SetBranchAddress("PVFilter", &PVFilter);
    tree->SetBranchAddress("ClusterCompatibilityFilter", &ClusterCompatibilityFilter);
    tree->SetBranchAddress("VZ", &VZ);
    tree->SetBranchAddress("HFEMaxPlus", &HFEMaxPlus);
    tree->SetBranchAddress("HFEMaxMinus", &HFEMaxMinus);
    tree->SetBranchAddress("HLT_MinimumBiasHF_OR_BptxAND_v1", &HLT_MinimumBiasHF_OR_BptxAND_v1);
    tree->SetBranchAddress("passBaselineEventSelection", &pBES);
    tree->SetBranchAddress("eventEfficiencyWeight_Nominal", &eventEfficiencyWeight_Nominal);
    tree->SetBranchAddress("trkPassChargedHadron_Nominal", &trkPassChargedHadron_Nominal);
    tree->SetBranchAddress("trackingEfficiency_Nominal", &trackingEfficiency_Nominal);
    tree->SetBranchAddress("TrkSpeciesWeight_dNdEta40", &TrkSpeciesWeight_dNdEta40);
    tree->SetBranchAddress("trkPt", &trkPt);
    tree->SetBranchAddress("trkEta", &trkEta);

    cout << "InputFile: " << inputFileName << endl;
    cout << "TrkPt cut: " << trkptcut << endl;
    cout << "TrkEta cut: " << trketacut << endl;
    cout << "Apply Event Weight: " << ApplyEventWeight << endl;
    cout << "Apply Track Weight: " << ApplyTrackWeight << endl;
    cout << "Apply Track Species Weight: " << ApplyTrackSpeciesWeight << endl;
    /// Event Selection

    int nEvents = 0;
    for (Long64_t i = 0; i < tree->GetEntries(); i++) {
        tree->GetEntry(i);
        // Event Selection
        if (i%500000 == 0) {
            cout << "Processing event " << i << endl;
        }
        if (debug && i == 1000) {
            cout << "Debug mode: stopping after 1000 events." << endl;
            break; // For debugging, stop after processing 1000 events
        }
        if (!HLT_MinimumBiasHF_OR_BptxAND_v1) continue;
        if (!pBES) continue;
        if (!(HFEMaxPlus > 13.0 && HFEMaxMinus > 13.0)) continue;

  
        // Event Level Reweight
        float eventWeight = 1.0;
        if (ApplyEventWeight) {
            eventWeight = eventEfficiencyWeight_Nominal;
        }
        nEvents++;
        // Track Level Reweight
        for (size_t j = 0; j < trkPt->size(); ++j) {
            //Track Level Selection
            if (debug){
                cout << "Processing track " << j << endl;
                cout << "trkPt: " << trkPt->at(j) << ", trkEta: " << trkEta->at(j) << endl;
            }
            if (!trkPassChargedHadron_Nominal->at(j)) continue;
            if (trkPt->at(j) < trkptcut) continue; 
            if (trkEta->at(j) > trketacut) continue;

            float trackWeight = 1.0;
            if (ApplyTrackWeight) {
                trackWeight =  trackingEfficiency_Nominal->at(j); 
            }
            
            float trackSpeciesWeight = 1.0;
            if (ApplyTrackSpeciesWeight) {
                trackSpeciesWeight *= TrkSpeciesWeight_dNdEta40->at(j); // species weight
            }

            float totalWeight = eventWeight * trackWeight * trackSpeciesWeight;
            if (debug){cout << "Filling histogram for track " << j << " with weight: " << totalWeight << endl;}
            h_dndpt->Fill(trkPt->at(j), totalWeight);
        }
    }

    //Normalize
    h_dndpt->Scale(1.0 / h_dndpt->Integral());
    for (int i = 1; i <= h_dndpt->GetNbinsX(); ++i) {
        double binWidth = h_dndpt->GetBinWidth(i);
        if (binWidth > 0) {
            h_dndpt->SetBinContent(i, h_dndpt->GetBinContent(i) / binWidth);
            h_dndpt->SetBinError(i, h_dndpt->GetBinError(i) / binWidth);
        }
    }


    // Save the histogram to a file
    TFile *outFile = TFile::Open(outFileName.c_str(), "RECREATE");
    if (!outFile || outFile->IsZombie()) {
        std::cerr << "Error creating output file: " << outFileName << std::endl;  
        return 1;
    }
    
    h_dndpt->Write();
    outFile->Close();
    file->Close();
    cout << "Output saved to " << outFileName << std::endl;
    return 0;
}