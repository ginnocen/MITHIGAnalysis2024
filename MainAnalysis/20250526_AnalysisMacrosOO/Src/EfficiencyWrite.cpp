#include <iostream>
#include <TFile.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLegend.h>
#include <utility>
#include <tuple>
#include <string>
#include "EfficiencyCounting.h"
#include "Parameters.h"
#include "EventSelection.h"
#include "EfficiencyWrite.h"

using namespace std;

std::string WriteEfficiencyRoot(
    FileNames filename,
    vector<Parameters> par_vector,
    int trkptcut,
    string outfolder,
    string outFileAdd) 
{

    TFile *file_signal = TFile::Open(filename.SignalFile.c_str());
    TFile *file_bkgSD = TFile::Open(filename.BackgroundFiles[0].c_str());
    TFile *file_bkgDD = TFile::Open(filename.BackgroundFiles[1].c_str());
    TFile *file_bkgaO = TFile::Open(filename.BackgroundFiles[2].c_str());

    ChargedHadronRAATreeMessenger* ch_hijing = new ChargedHadronRAATreeMessenger(file_signal, "Tree", false);
    ChargedHadronRAATreeMessenger* ch_bkgSD = new ChargedHadronRAATreeMessenger(file_bkgSD, "Tree", false);
    ChargedHadronRAATreeMessenger* ch_bkgDD = new ChargedHadronRAATreeMessenger(file_bkgDD, "Tree", false);
    ChargedHadronRAATreeMessenger* ch_bkgaO = new ChargedHadronRAATreeMessenger(file_bkgaO , "Tree", false);

    // Variables for branches
    Float_t HFEmax_Offline_min1, HFEmax_Offline_min2;
    Float_t HFEmax_Online_min1, HFEmax_Online_min2;
    Float_t Eff_Hijing, Eff_SD, Eff_DD, Eff_AlphaO;
    Int_t CentralityHijing;
    int N = par_vector.size();
    Parameters par;

    string rootfilename = outfolder + Form("EfficiencyPurityData_N%d_trkpt%d_", N-1, trkptcut) + outFileAdd + ".root";
    TFile *rootfile = new TFile(rootfilename.c_str(), "RECREATE");
    TTree *tree = new TTree("effTree", "Efficiency and Purity Data");

    // Create branches
    tree->Branch("HFEmax_Offline_min1", &HFEmax_Offline_min1, "HFEmax_Offline_min1/F");
    tree->Branch("HFEmax_Offline_min2", &HFEmax_Offline_min2, "HFEmax_Offline_min2/F");
    tree->Branch("HFEmax_Online_min1", &HFEmax_Online_min1, "HFEmax_Online_min1/F");
    tree->Branch("HFEmax_Online_min2", &HFEmax_Online_min2, "HFEmax_Online_min2/F");
    tree->Branch("Eff_Hijing", &Eff_Hijing, "Eff_Hijing/F");
    tree->Branch("Eff_SD", &Eff_SD, "Eff_SD/F");
    tree->Branch("Eff_DD", &Eff_DD, "Eff_DD/F");
    tree->Branch("Eff_AlphaO", &Eff_AlphaO, "Eff_AlphaO/F");

    cout << "------- Writing Efficiency and Purity Data to ROOT file -------" << endl;
    cout << "Output file: " << rootfilename << endl;

    for (int i = 0; i < N; ++i) {
        par = par_vector[i];
        HFEmax_Offline_min1 = par.HFEmax_Offline_min1;
        HFEmax_Offline_min2 = par.HFEmax_Offline_min2;
        HFEmax_Online_min1 = par.HFEmax_Online_min1;
        HFEmax_Online_min2 = par.HFEmax_Online_min2;
        Eff_Hijing = calculateEfficiency(ch_hijing, par, true, trkptcut).first;
        Eff_SD     = calculateEfficiency(ch_bkgSD, par, false, trkptcut).first;
        Eff_DD     = calculateEfficiency(ch_bkgDD, par, false, trkptcut).first;
        Eff_AlphaO = calculateEfficiency(ch_bkgaO, par, false, trkptcut).first;
        tree->Fill();
    }

    rootfile->Write();
    rootfile->Close();
    delete rootfile;

    return rootfilename;
}
