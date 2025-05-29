        bool isJetTagged = false;
        float muPt1 = -999.;
        float muPt2 = -999.;
        float muEta1 = -999.;
        float muEta2 = -999.;
        float muPhi1 = -999.;
        float muPhi2 = -999.;
        float DRJetmu1 = -999.;
        float DRJetmu2 = -999.;
        
        // variable to identify the highest pt dimuon pair
        float maxmumuPt = 0.;
        int maxMu1Index = -1;
        int maxMu2Index = -1;

        int nSingleMu = MSingleMu.SingleMuPT->size();
        for (int isinglemu1 = 0; isinglemu1 < nSingleMu; isinglemu1++) {
          if (isMuonSelected(&MSingleMu, isinglemu1) == false)
            continue;
          for (int isinglemu2 = isinglemu1 + 1; isinglemu2 < nSingleMu; isinglemu2++) {
            if (isMuonSelected(&MSingleMu, isinglemu2) == false)
              continue;
            // int charge1 = MSingleMu.SingleMuCharge->at(isinglemu1);
            // int charge2 = MSingleMu.SingleMuCharge->at(isinglemu2);
            // if (charge1 == charge2)
            // continue;
            float jetPhi = MJet.JetPhi[ijet];
            float jetEta = MJet.JetEta[ijet];
            float muEta1 = MSingleMu.SingleMuEta->at(isinglemu1);
            float muPhi1 = MSingleMu.SingleMuPhi->at(isinglemu1);
            float muEta2 = MSingleMu.SingleMuEta->at(isinglemu2);
            float muPhi2 = MSingleMu.SingleMuPhi->at(isinglemu2);
            float dRMu1Jet = sqrt(dPhiMu1Jet_ * dPhiMu1Jet_ + dEtaMu1Jet_ * dEtaMu1Jet_);
            float dRMu2Jet = sqrt(dPhiMu2Jet_ * dPhiMu2Jet_ + dEtaMu2Jet_ * dEtaMu2Jet_);
            if (dRMu1Jet > 0.3)
              continue;
            if (dRMu2Jet > 0.3)
              continue;
            TLorentzVector Mu1, Mu2;
            Mu1.SetPtEtaPhiM(MSingleMu.SingleMuPT->at(isinglemu1), muEta1, muPhi1, M_MU);
            Mu2.SetPtEtaPhiM(MSingleMu.SingleMuPT->at(isinglemu2), muEta2, muPhi2, M_MU);
            TLorentzVector MuMu = Mu1 + Mu2;
            if (MuMu.M() > 130)
              continue;
            if (MuMu.Eta() > 2.4)
              continue;
            if (MuMu.Pt() > maxmumuPt) {
              maxmumuPt = MuMu.Pt();
              maxMu1Index = isinglemu1;
              maxMu2Index = isinglemu2;
            } // end if dimuon pT larger than current max
          } // end loop over single muon 2
        } // end loop over single muon 1

        if (maxmumuPt > 0. && maxMu1Index >= 0 && maxMu2Index >= 0) {
          isJetTagged = true;
          // cout << "isJetTagged = true" << endl;

          muPt1 = MSingleMu.SingleMuPT->at(maxMu1Index);
          muPt2 = MSingleMu.SingleMuPT->at(maxMu2Index);
          muEta1 = MSingleMu.SingleMuEta->at(maxMu1Index);
          muEta2 = MSingleMu.SingleMuEta->at(maxMu2Index);
          muPhi1 = MSingleMu.SingleMuPhi->at(maxMu1Index);
          muPhi2 = MSingleMu.SingleMuPhi->at(maxMu2Index);
          muCharge1 = MSingleMu.SingleMuCharge->at(maxMu1Index);
          muCharge2 = MSingleMu.SingleMuCharge->at(maxMu2Index);
          muDiDxy1 = MSingleMu.SingleMuDxy->at(maxMu1Index);
          muDiDxy1Err = MSingleMu.SingleMuDxyError->at(maxMu1Index);
          muDiDxy2 = MSingleMu.SingleMuDxy->at(maxMu2Index);
          muDiDxy2Err = MSingleMu.SingleMuDxyError->at(maxMu2Index);
          muDiDz1 = MSingleMu.SingleMuDz->at(maxMu1Index);
          muDiDz1Err = MSingleMu.SingleMuDzError->at(maxMu1Index);
          muDiDz2 = MSingleMu.SingleMuDz->at(maxMu2Index);
          muDiDz2Err = MSingleMu.SingleMuDzError->at(maxMu2Index);
          muDiDxy1Dxy2 = muDiDxy1 * muDiDxy2;
          muDiDxy1Dxy2Err =
              sqrt(muDiDxy1Err / muDiDxy1 * muDiDxy1Err / muDiDxy1 + muDiDxy2Err / muDiDxy2 * muDiDxy2Err / muDiDxy2) *
              muDiDxy1Dxy2;
          TLorentzVector Mu1, Mu2;
          Mu1.SetPtEtaPhiM(muPt1, muEta1, muPhi1, M_MU);
          Mu2.SetPtEtaPhiM(muPt2, muEta2, muPhi2, M_MU);
          TLorentzVector MuMu = Mu1 + Mu2;
          mumuMass = MuMu.M();
          mumuEta = MuMu.Eta();
          mumuY = MuMu.Rapidity();
          mumuPhi = MuMu.Phi();
          mumuPt = MuMu.Pt();
          mumuisOnia = isOnia(mumuMass);
          float jetEta = MJet.JetEta[ijet];
          float jetPhi = MJet.JetPhi[ijet];
          float muEta1 = MSingleMu.SingleMuEta->at(maxMu1Index);
          float muPhi1 = MSingleMu.SingleMuPhi->at(maxMu1Index);
          float muEta2 = MSingleMu.SingleMuEta->at(maxMu2Index);
          float muPhi2 = MSingleMu.SingleMuPhi->at(maxMu2Index);
          float dPhiMu1Jet_ = DeltaPhi(muPhi1, jetPhi);
          float dEtaMu1Jet_ = muEta1 - jetEta;
          float dPhiMu2Jet_ = DeltaPhi(muPhi2, jetPhi);
          float dEtaMu2Jet_ = muEta2 - jetEta;
          float dRMu1Jet = sqrt(dPhiMu1Jet_ * dPhiMu1Jet_ + dEtaMu1Jet_ * dEtaMu1Jet_);
          float dRMu2Jet = sqrt(dPhiMu2Jet_ * dPhiMu2Jet_ + dEtaMu2Jet_ * dEtaMu2Jet_);
          muDeta = muEta1 - muEta2;
          muDphi = DeltaPhi(muPhi1, muPhi2);
          muDR = sqrt(muDeta * muDeta + muDphi * muDphi);
        } // end if dimuon pair found




        bool isMuonSelected(vector<float> *pT, vector<float> *eta, vector<bool> *isTracker, vector<bool> *isGlobal, vector<bool> *hybridSoft, vector<bool> *isGood, int i) {
          if (pT->at(i) < 3.5){return false};
          if (fabs(eta->at(i)) > 2.3){return false};
          if (isTracker->at(i) == 0 || IsGlobal->at(i) == 0 || HybridSoft->at(i) == 0 || IsGood->at(i) == 0){return false};

        
          return true;
        }