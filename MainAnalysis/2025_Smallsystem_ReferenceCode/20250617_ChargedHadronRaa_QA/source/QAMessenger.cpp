#include <algorithm>
#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"

#include "Messenger.h"
#include "QAMessenger.h"

QAMessenger::QAMessenger(const std::string &title) {
    
    hQA_hiBin = nullptr;
    hQA_VZ = nullptr;
    hQA_VZError = nullptr;
    hQA_ptSumVtx = nullptr;
    hQA_TracksVtx = nullptr;

    hQA_HFEMax = nullptr;
    hQA_HFEMaxPlus = nullptr;
    hQA_HFEMaxMinus = nullptr;
    hQA_onlineHFEMaxPlus = nullptr;
    hQA_onlineHFEMaxMinus = nullptr;
    hQA_onlineHFEMax_hi = nullptr;
    hQA_onlineHFEMax_lo = nullptr;
    hQA_HFETot = nullptr;

    hQA_ZDCPlus = nullptr;
    hQA_ZDCMinus = nullptr;
    hQA_ZDCMax = nullptr;

    hQA_Ncoll = nullptr;
    hQA_Npart = nullptr;

    hQA_TrkMult = nullptr;
    hQA_TrkPt = nullptr;
    hQA_TrkEta = nullptr;
    hQA_Sphericity = nullptr;
    hQA_Qvec = nullptr;
    hQA_Thrust = nullptr;

    hQA_TrkDxy = nullptr;
    hQA_TrkDz = nullptr;
    hQA_TrkDxySig = nullptr;
    hQA_TrkDzSig = nullptr;

    hQA_TrkPtEta = nullptr;
    hQA_CentEsum = nullptr;

    Initialize(title);

} // Constructor

bool QAMessenger::Initialize(const std::string &title){

    if (hQA_hiBin)      { delete hQA_hiBin;      hQA_hiBin = nullptr; }
    if (hQA_VZ)         { delete hQA_VZ;         hQA_VZ = nullptr; }
    if (hQA_VZError)    { delete hQA_VZError;    hQA_VZError = nullptr; }
    if (hQA_ptSumVtx)   { delete hQA_ptSumVtx;   hQA_ptSumVtx = nullptr; }
    if (hQA_TracksVtx)  { delete hQA_TracksVtx;  hQA_TracksVtx = nullptr; }
    if (hQA_HFEMax)     { delete hQA_HFEMax;     hQA_HFEMax = nullptr; }
    if (hQA_HFEMaxPlus) { delete hQA_HFEMaxPlus; hQA_HFEMaxPlus = nullptr; }
    if (hQA_HFEMaxMinus){ delete hQA_HFEMaxMinus;hQA_HFEMaxMinus = nullptr; }
    if (hQA_onlineHFEMaxPlus) { delete hQA_onlineHFEMaxPlus; hQA_onlineHFEMaxPlus = nullptr; }
    if (hQA_onlineHFEMaxMinus) { delete hQA_onlineHFEMaxMinus; hQA_onlineHFEMaxMinus = nullptr; }
    if (hQA_onlineHFEMax_hi) { delete hQA_onlineHFEMax_hi; hQA_onlineHFEMax_hi = nullptr; }
    if (hQA_onlineHFEMax_lo) { delete hQA_onlineHFEMax_lo; hQA_onlineHFEMax_lo = nullptr; }
    if (hQA_HFETot)     { delete hQA_HFETot;     hQA_HFETot = nullptr; }
    if (hQA_ZDCPlus)    { delete hQA_ZDCPlus;    hQA_ZDCPlus = nullptr; }
    if (hQA_ZDCMinus)   { delete hQA_ZDCMinus;   hQA_ZDCMinus = nullptr; }
    if (hQA_ZDCMax)     { delete hQA_ZDCMax;     hQA_ZDCMax = nullptr; }
    if (hQA_Ncoll)      { delete hQA_Ncoll;      hQA_Ncoll = nullptr; }
    if (hQA_Npart)      { delete hQA_Npart;      hQA_Npart = nullptr; }
    if (hQA_TrkMult)    { delete hQA_TrkMult;    hQA_TrkMult = nullptr; }
    if (hQA_TrkPt)      { delete hQA_TrkPt;      hQA_TrkPt = nullptr; }
    if (hQA_TrkEta)     { delete hQA_TrkEta;     hQA_TrkEta = nullptr; }
    if (hQA_Sphericity) { delete hQA_Sphericity; hQA_Sphericity = nullptr; }
    if (hQA_Qvec)       { delete hQA_Qvec;       hQA_Qvec = nullptr; }
    if (hQA_Thrust)     { delete hQA_Thrust;     hQA_Thrust = nullptr; }
    if (hQA_TrkDxy)     { delete hQA_TrkDxy;     hQA_TrkDxy = nullptr; }
    if (hQA_TrkDz)      { delete hQA_TrkDz;      hQA_TrkDz = nullptr; }
    if (hQA_TrkDxySig)  { delete hQA_TrkDxySig;  hQA_TrkDxySig = nullptr; }
    if (hQA_TrkDzSig)   { delete hQA_TrkDzSig;   hQA_TrkDzSig = nullptr; }
    if (hQA_TrkPtEta)   { delete hQA_TrkPtEta;   hQA_TrkPtEta = nullptr; }
    if (hQA_CentEsum)   { delete hQA_CentEsum;   hQA_CentEsum = nullptr; }

    hQA_hiBin = new TH1D(Form("QA_hiBin%s", title.c_str()), "hiBin;Centrality %; Events", 101, -0.5, 100.5);
    hQA_VZ = new TH1D(Form("QA_VZ%s", title.c_str()), " VZ Distribution; VZ; Events", 50, -20.0, 20.0);
    hQA_VZError = new TH1D(Form("QA_VZError%s", title.c_str()), "Log 10 VZ Error Distribution;Log10(VZError);Events", 50, -5, 1);
    hQA_ptSumVtx = new TH1D(Form("QA_ptSumVtx%s", title.c_str()), "Sum of pT per vertex;ptSumVtx;Events", 100, 0, 500);
    hQA_TracksVtx = new TH1D(Form("QA_TracksVtx%s", title.c_str()), "Number of tracks per vertex;Number of Tracks per Vertex;Events", 100, 0, 1000);

    hQA_HFEMax = new TH1D(Form("QA_HFEMax%s", title.c_str()), "Max HFE Energy;HFE Energy;Events", 100, 0, 1000);
    hQA_HFEMaxPlus = new TH1D(Form("QA_HFEMaxPlus%s", title.c_str()), "Max HFE Energy Plus;HFE Energy Plus;Events", 100, 0, 1000);
    hQA_HFEMaxMinus = new TH1D(Form("QA_HFEMaxMinus%s", title.c_str()), "Max HFE Energy Minus;HFE Energy Minus;Events", 100, 0, 1000);
    hQA_onlineHFEMaxPlus = new TH1D(Form("QA_onlineHFEMaxPlus%s", title.c_str()), "Online Max HFE Energy Plus;Online HFE Energy Plus (GeV);Events", 100, 0, 1000);
    hQA_onlineHFEMaxMinus = new TH1D(Form("QA_onlineHFEMaxMinus%s", title.c_str()), "Online Max HFE Energy Minus;Online HFE Energy Minus (GeV);Events", 100, 0, 1000);
    hQA_onlineHFEMax_hi = new TH1D(Form("QA_onlineHFEMax_hi%s", title.c_str()), "Online Max HFE Energy High;Online HFE Energy High (GeV);Events", 100, -0.5, 200.5);
    hQA_onlineHFEMax_lo = new TH1D(Form("QA_onlineHFEMax_lo%s", title.c_str()), "Online Max HFE Energy Low;Online HFE Energy Low (GeV);Events", 100, -0.5, 200.5);
    hQA_HFETot = new TH1D(Form("QA_HFETot%s", title.c_str()), "Total HFE Energy;Total HFE Energy;Events", 100, 0, 2000);

    hQA_ZDCPlus = new TH1D(Form("QA_ZDCPlus%s", title.c_str()), "ZDC Plus Energy;ZDC Plus Energy;Events", 100, 0, 1000);
    hQA_ZDCMinus = new TH1D(Form("QA_ZDCMinus%s", title.c_str()), "ZDC Minus Energy;ZDC Minus Energy;Events", 100, 0, 1000);
    hQA_ZDCMax = new TH1D(Form("QA_ZDCMax%s", title.c_str()), "Total ZDC Energy;Total ZDC Energy;Events", 100, 0, 2000);
    hQA_Ncoll = new TH1D(Form("QA_Ncoll%s", title.c_str()), "Number of Collisions;Ncoll;Events", 61, -0.5, 60.5);
    hQA_Npart = new TH1D(Form("QA_Npart%s", title.c_str()), "Number of Participants;Npart;Events", 50, -0.5, 49.5);

    hQA_TrkMult = new TH1D(Form("QA_TrkMult%s", title.c_str()), "Track Multiplicity;Track Multiplicity;Events", 100, 0, 500);
    hQA_TrkPt = new TH1D(Form("QA_TrkPt%s", title.c_str()), "Track pT;Track pT (GeV/c);Tracks", 100, 0, 10);
    hQA_TrkEta = new TH1D(Form("QA_TrkEta%s", title.c_str()), "Track Eta;Track Eta;Tracks", 100, -5, 5);
    hQA_Sphericity = new TH1D(Form("QA_Sphericity%s", title.c_str()), "Sphericity;Sphericity;Events", 100, 0, 1);
    hQA_Qvec = new TH1D(Form("QA_Qvec%s", title.c_str()), "Q vector;Q vector;Events", 100, 0, 10);
    hQA_Thrust = new TH1D(Form("QA_Thrust%s", title.c_str()), "Thrust;Thrust;Events", 100, 0, 1);

    hQA_TrkDxy = new TH1D(Form("QA_TrkDxy%s", title.c_str()), "Track DCA xy;DCA xy (cm);Tracks", 100, -1, 1);
    hQA_TrkDz = new TH1D(Form("QA_TrkDz%s", title.c_str()), "Track DCA z;DCA z (cm);Tracks", 100, -1, 1);
    hQA_TrkDxySig = new TH1D(Form("QA_TrkDxySig%s", title.c_str()), "Track DCA xy significance;DCA xy significance;Tracks", 100, -5, 5);
    hQA_TrkDzSig = new TH1D(Form("QA_TrkDzSig%s", title.c_str()), "Track DCA z significance;DCA z significance;Tracks", 100, -5, 5);

    hQA_TrkPtEta = new TH2D(Form("QA_TrkPtEta%s", title.c_str()), "Track pT vs Eta;Track pT (GeV/c);Track Eta", 40, 0, 20, 50, -4.0, 4.0);
    hQA_CentEsum = new TH2D(Form("QA_CentEsum%s", title.c_str()), "Centrality vs Energy Sum;Centrality;Energy Sum", 101, -0.5, 100.5, 200, 0, 1000);

    // Set sumw2 for histograms 
    hQA_hiBin->Sumw2();
    hQA_VZ->Sumw2();
    hQA_VZError->Sumw2();    
    hQA_ptSumVtx->Sumw2();
    hQA_TracksVtx->Sumw2();
    hQA_HFEMax->Sumw2();
    hQA_HFEMaxPlus->Sumw2();
    hQA_HFEMaxMinus->Sumw2();
    hQA_onlineHFEMaxPlus->Sumw2();
    hQA_onlineHFEMaxMinus->Sumw2();
    hQA_onlineHFEMax_hi->Sumw2();
    hQA_onlineHFEMax_lo->Sumw2();
    hQA_HFETot->Sumw2();
    hQA_ZDCPlus->Sumw2();
    hQA_ZDCMinus->Sumw2();
    hQA_ZDCMax->Sumw2();
    hQA_Ncoll->Sumw2();
    hQA_Npart->Sumw2();
    hQA_TrkMult->Sumw2();
    hQA_TrkPt->Sumw2();
    hQA_TrkEta->Sumw2();
    hQA_Sphericity->Sumw2();
    hQA_Qvec->Sumw2();
    hQA_Thrust->Sumw2();
    hQA_TrkDxy->Sumw2();
    hQA_TrkDz->Sumw2();
    hQA_TrkDxySig->Sumw2();   
    hQA_TrkDzSig->Sumw2();
    hQA_TrkPtEta->Sumw2();
    hQA_CentEsum->Sumw2();

    return true;

}

bool QAMessenger::AnalyzeEvent(ChargedHadronRAATreeMessenger *ch, float weight) {
    
    if (!ch) {
        std::cerr << "ChargedHadronRAATreeMessenger not found" << std::endl;
        return false;
    }

    // EVENT DATA

    hQA_hiBin->Fill(ch->hiBin/2.0, weight);
    hQA_VZ->Fill(ch->VZ, weight);
    hQA_VZError->Fill(log10(ch->VZError), weight);
    hQA_ptSumVtx->Fill(ch->ptSumVtx, weight);
    hQA_TracksVtx->Fill(ch->nTracksVtx, weight);

    hQA_HFEMax->Fill(std::max(ch->HFEMaxPlus,ch->HFEMaxMinus),weight);
    hQA_HFEMaxPlus->Fill(ch->HFEMaxPlus,weight);
    hQA_HFEMaxMinus->Fill(ch->HFEMaxMinus,weight);
    hQA_onlineHFEMaxPlus->Fill(ch->mMaxL1HFAdcPlus, weight);
    hQA_onlineHFEMaxMinus->Fill(ch->mMaxL1HFAdcMinus, weight);
    hQA_onlineHFEMax_hi->Fill(std::max(ch->mMaxL1HFAdcPlus, ch->mMaxL1HFAdcMinus), weight);
    hQA_onlineHFEMax_lo->Fill(std::min(ch->mMaxL1HFAdcPlus, ch->mMaxL1HFAdcMinus), weight);
    hQA_HFETot->Fill(ch->hiHF_pf, weight);

    hQA_ZDCPlus->Fill(ch->ZDCsumPlus, weight);
    hQA_ZDCMinus->Fill(ch->ZDCsumMinus, weight);
    hQA_ZDCMax->Fill(std::max(ch->ZDCsumPlus,ch->ZDCsumMinus), weight);
    
    hQA_Ncoll->Fill(ch->Ncoll, weight);
    hQA_Npart->Fill(ch->Npart, weight);

    int mult = 0;

    for(int i = 0; i< ch->trkPt->size(); i++){

        // TRACKWEIGHTS NOT IMPLEMENTED YET
        //float trueweight = ch->trackWeight->at(i);
        float trueweight = weight;
        if(ch->trkPt->at(i) > 0.5) {mult +=1;} // Minimum pT cut
        hQA_TrkPt->Fill(ch->trkPt->at(i), weight);
        hQA_TrkEta->Fill(ch->trkEta->at(i), weight);
        hQA_TrkDxy->Fill(ch->trkDxyAssociatedVtx->at(i), weight);
        hQA_TrkDz->Fill(ch->trkDzAssociatedVtx->at(i), weight);
        hQA_TrkDxySig->Fill((ch->trkDxyAssociatedVtx->at(i)/ch->trkDxyErrAssociatedVtx->at(i)), weight);
        hQA_TrkDzSig->Fill((ch->trkDzAssociatedVtx->at(i)/ch->trkDzErrAssociatedVtx->at(i)), weight);
        hQA_TrkPtEta->Fill(ch->trkPt->at(i), ch->trkEta->at(i), weight);

    }

    hQA_TrkMult->Fill(mult, weight);
    hQA_CentEsum->Fill(ch->hiBin/2.0, ch->hiHF_pf, weight);    

    //////  TO BE IMPLEMENTED //////
    //hSphericity->Fill(ch->Sphericity, weight);
    //hQvec->Fill(ch->Qvec, weight);
    //hThrust->Fill(ch->Thrust, weight); 

    return true;
}

bool QAMessenger::WriteHistograms(TFile *outf) {
    if (!outf || !outf->IsOpen()) {
        std::cerr << "Output file is not open" << std::endl;
        return false;
    }

    TDirectory *qaDir = outf->GetDirectory("QAHistograms");
    if (!qaDir) qaDir = outf->mkdir("QAHistograms");
    qaDir->cd();

    // Write and detach all histograms if they exist
    if (hQA_hiBin)      { hQA_hiBin->Write();      hQA_hiBin->SetDirectory(nullptr); }
    if (hQA_VZ)         { hQA_VZ->Write();         hQA_VZ->SetDirectory(nullptr); }
    if (hQA_VZError)    { hQA_VZError->Write();    hQA_VZError->SetDirectory(nullptr); }
    if (hQA_ptSumVtx)   { hQA_ptSumVtx->Write();   hQA_ptSumVtx->SetDirectory(nullptr); }
    if (hQA_TracksVtx)  { hQA_TracksVtx->Write();  hQA_TracksVtx->SetDirectory(nullptr); }

    if (hQA_HFEMax)         { hQA_HFEMax->Write();         hQA_HFEMax->SetDirectory(nullptr); }
    if (hQA_HFEMaxPlus)     { hQA_HFEMaxPlus->Write();     hQA_HFEMaxPlus->SetDirectory(nullptr); }
    if (hQA_HFEMaxMinus)    { hQA_HFEMaxMinus->Write();    hQA_HFEMaxMinus->SetDirectory(nullptr); }
    if (hQA_onlineHFEMaxPlus) { hQA_onlineHFEMaxPlus->Write(); hQA_onlineHFEMaxPlus->SetDirectory(nullptr); }
    if (hQA_onlineHFEMaxMinus) { hQA_onlineHFEMaxMinus->Write(); hQA_onlineHFEMaxMinus->SetDirectory(nullptr); }
    if (hQA_onlineHFEMax_hi) { hQA_onlineHFEMax_hi->Write(); hQA_onlineHFEMax_hi->SetDirectory(nullptr); }
    if (hQA_onlineHFEMax_lo) { hQA_onlineHFEMax_lo->Write(); hQA_onlineHFEMax_lo->SetDirectory(nullptr); }
    if (hQA_HFETot)         { hQA_HFETot->Write();         hQA_HFETot->SetDirectory(nullptr); }

    if (hQA_ZDCPlus)    { hQA_ZDCPlus->Write();    hQA_ZDCPlus->SetDirectory(nullptr); }
    if (hQA_ZDCMinus)   { hQA_ZDCMinus->Write();   hQA_ZDCMinus->SetDirectory(nullptr); }
    if (hQA_ZDCMax)     { hQA_ZDCMax->Write();     hQA_ZDCMax->SetDirectory(nullptr); }

    if (hQA_Ncoll)      { hQA_Ncoll->Write();      hQA_Ncoll->SetDirectory(nullptr); }
    if (hQA_Npart)      { hQA_Npart->Write();      hQA_Npart->SetDirectory(nullptr); }

    if (hQA_TrkMult)    { hQA_TrkMult->Write();    hQA_TrkMult->SetDirectory(nullptr); }
    if (hQA_TrkPt)      { hQA_TrkPt->Write();      hQA_TrkPt->SetDirectory(nullptr); }
    if (hQA_TrkEta)     { hQA_TrkEta->Write();     hQA_TrkEta->SetDirectory(nullptr); }
    if (hQA_Sphericity) { hQA_Sphericity->Write(); hQA_Sphericity->SetDirectory(nullptr); }
    if (hQA_Qvec)       { hQA_Qvec->Write();       hQA_Qvec->SetDirectory(nullptr); }
    if (hQA_Thrust)     { hQA_Thrust->Write();     hQA_Thrust->SetDirectory(nullptr); }

    if (hQA_TrkDxy)     { hQA_TrkDxy->Write();     hQA_TrkDxy->SetDirectory(nullptr); }
    if (hQA_TrkDz)      { hQA_TrkDz->Write();      hQA_TrkDz->SetDirectory(nullptr); }
    if (hQA_TrkDxySig)  { hQA_TrkDxySig->Write();  hQA_TrkDxySig->SetDirectory(nullptr); }
    if (hQA_TrkDzSig)   { hQA_TrkDzSig->Write();   hQA_TrkDzSig->SetDirectory(nullptr); }

    if (hQA_TrkPtEta)   { hQA_TrkPtEta->Write();   hQA_TrkPtEta->SetDirectory(nullptr); }
    if (hQA_CentEsum)   { hQA_CentEsum->Write();   hQA_CentEsum->SetDirectory(nullptr); }

    outf->cd();
    return true;
}

bool QAMessenger::DeleteHistograms() {
    delete hQA_hiBin;      hQA_hiBin = nullptr;
    delete hQA_VZ;         hQA_VZ = nullptr;
    delete hQA_VZError;    hQA_VZError = nullptr;
    delete hQA_ptSumVtx;   hQA_ptSumVtx = nullptr;
    delete hQA_TracksVtx;  hQA_TracksVtx = nullptr;
    delete hQA_HFEMax;     hQA_HFEMax = nullptr;
    delete hQA_HFEMaxPlus; hQA_HFEMaxPlus = nullptr;
    delete hQA_HFEMaxMinus;hQA_HFEMaxMinus = nullptr;
    delete hQA_onlineHFEMaxPlus; hQA_onlineHFEMaxPlus = nullptr;
    delete hQA_onlineHFEMaxMinus; hQA_onlineHFEMaxMinus = nullptr;
    delete hQA_onlineHFEMax_hi; hQA_onlineHFEMax_hi = nullptr;
    delete hQA_onlineHFEMax_lo; hQA_onlineHFEMax_lo = nullptr;
    delete hQA_HFETot;     hQA_HFETot = nullptr;
    delete hQA_ZDCPlus;    hQA_ZDCPlus = nullptr;
    delete hQA_ZDCMinus;   hQA_ZDCMinus = nullptr;
    delete hQA_ZDCMax;     hQA_ZDCMax = nullptr;
    delete hQA_Ncoll;      hQA_Ncoll = nullptr;
    delete hQA_Npart;      hQA_Npart = nullptr;
    delete hQA_TrkMult;    hQA_TrkMult = nullptr;
    delete hQA_TrkPt;      hQA_TrkPt = nullptr;
    delete hQA_TrkEta;     hQA_TrkEta = nullptr;
    delete hQA_Sphericity; hQA_Sphericity = nullptr;
    delete hQA_Qvec;       hQA_Qvec = nullptr;
    delete hQA_Thrust;     hQA_Thrust = nullptr;
    delete hQA_TrkDxy;     hQA_TrkDxy = nullptr;
    delete hQA_TrkDz;      hQA_TrkDz = nullptr;
    delete hQA_TrkDxySig;  hQA_TrkDxySig = nullptr;
    delete hQA_TrkDzSig;   hQA_TrkDzSig = nullptr;
    delete hQA_TrkPtEta;   hQA_TrkPtEta = nullptr;
    delete hQA_CentEsum;   hQA_CentEsum = nullptr;

    return true;
}

QAMessenger::~QAMessenger() {
    DeleteHistograms();
}