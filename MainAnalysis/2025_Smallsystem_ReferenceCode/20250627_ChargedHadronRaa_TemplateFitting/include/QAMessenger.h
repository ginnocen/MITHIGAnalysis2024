#ifndef QAMESSENGER_H_AAGSDFAJ
#define QAMESSENGER_H_AAGSDFAJ

#include <iostream>
#include <vector>
#include "Messenger.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"

class ChargedHadronRAATreeMessenger;

class QAMessenger
{
    public:
        // Event wide
        TH1D *hQA_hiBin;
        TH1D *hQA_VZ;
        TH1D *hQA_VZError;
        TH1D *hQA_ptSumVtx;
        TH1D *hQA_TracksVtx;

        // HFE 
        TH1D *hQA_HFEMax;
        TH1D *hQA_HFEMaxPlus;
        TH1D *hQA_HFEMaxMinus;
        TH1D *hQA_onlineHFEMaxPlus;
        TH1D *hQA_onlineHFEMaxMinus;
        TH1D *hQA_onlineHFEMax_hi;
        TH1D *hQA_onlineHFEMax_lo;
        TH1D *hQA_HFETot;
        
        // ZDC 
        TH1D *hQA_ZDCPlus;
        TH1D *hQA_ZDCMinus;
        TH1D *hQA_ZDCMax;

        // Npart stuff
        TH1D *hQA_Ncoll;
        TH1D *hQA_Npart;
            
        // track derived
        TH1D *hQA_TrkMult;
        TH1D *hQA_TrkPt;
        TH1D *hQA_TrkEta;
        TH1D *hQA_Sphericity;
        TH1D *hQA_Qvec;
        TH1D *hQA_Thrust;

        // DCA? 
        TH1D *hQA_TrkDxy;
        TH1D *hQA_TrkDz;
        TH1D *hQA_TrkDxySig;
        TH1D *hQA_TrkDzSig;
        
        // Ignoring the debug mode variables 

        TH2D *hQA_TrkPtEta;
        TH2D* hQA_CentEsum;
        

    public:
        QAMessenger();
        QAMessenger(const std::string& title);
        bool Initialize(const std::string &title);
        bool AnalyzeEvent(ChargedHadronRAATreeMessenger *ch, float weight);
        bool WriteHistograms(TFile *outf);
        bool DeleteHistograms();
        ~QAMessenger();

};

#endif

