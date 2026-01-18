double PhiRangeSymmetric(double Phi)
{
   if(Phi < -M_PI)   Phi = Phi + 2 * M_PI;
   if(Phi > +M_PI)   Phi = Phi - 2 * M_PI;
   return Phi;
}

double DeltaPhi(double Phi1, double Phi2)
{
   return PhiRangeSymmetric(Phi1 - Phi2);
}

float dR(float eta1, float phi1, float eta2, float phi2) {
  float dEta = eta1 - eta2;
  float dPhi = DeltaPhi(phi1, phi2);
  return sqrt(dEta * dEta + dPhi * dPhi);
}



void dimefficiency(){

    gStyle->SetOptStat(0);

    TChain* t = new TChain("muonAnalyzer/MuonTree");
    
    TString basePath = "/eos/cms/store/group/phys_heavyions/aholterm/g2qqbar/QCD_pThat-15_Dijet_TuneCP5_5p02TeV-pythia8/crab_btagged_and_svtagged_jets_MC_HFfinders/251128_191749/0003/";
    
    for(int i = 3000; i < 3007; i++){
        TString filePath = basePath + Form("HiForestMiniAOD_%d.root", i);
        if(!gSystem->AccessPathName(filePath)){
            t->Add(filePath);
            cout << "Added: " << filePath << endl;
        }
    }
    
    cout << "Total entries: " << t->GetEntries() << endl;
    /*
    TChain* tjet = new TChain("ak4PFJetAnalyzer/t");
    for(int i = 3000; i < 3007; i++){
        TString filePath = basePath + Form("HiForestMiniAOD_%d.root", i);
        if(!gSystem->AccessPathName(filePath)){
            tjet->Add(filePath);
        }
    }

    cout << "Total jet entries: " << tjet->GetEntries() << endl;

    float jetPt = 0;
    tjet->SetBranchAddress("jtpt", &jetPt);
    float jetEta = 0;
    tjet->SetBranchAddress("jteta", &jetEta);
    float jetPhi = 0;
    tjet->SetBranchAddress("jtphi", &jetPhi);*/

    ////////////////////////////////////
    ////////////////////////////////////

    int nreco = 0;
    t->SetBranchAddress("nReco", &nreco);

    int ngen = 0;
    t->SetBranchAddress("nGen", &ngen);
    
    vector<float>* mupt = 0;
    t->SetBranchAddress("recoPt", &mupt);

    vector<float>* genmupt = 0;
    t->SetBranchAddress("genPt", &genmupt);

    vector<float>* mueta = 0;
    t->SetBranchAddress("recoEta", &mueta);

    vector<float>* genmueta = 0;
    t->SetBranchAddress("genEta", &genmueta);

    vector<float>* muphi = 0;
    t->SetBranchAddress("recoPhi", &muphi);

    vector<float>* genmuphi = 0;
    t->SetBranchAddress("genPhi", &genmuphi);


    vector<bool>* isGood = {};
    vector<bool>* isTracker = {};
    vector<bool>* isGlobal = {};
    vector<bool>* isHybridSoft = {};

    t->SetBranchAddress("recoIsGood", &isGood);
    t->SetBranchAddress("recoIsTracker", &isTracker);
    t->SetBranchAddress("recoIsGlobal", &isGlobal);
    t->SetBranchAddress("recoIDHybridSoft", &isHybridSoft);


    vector<bool>* isSoft = {};
    vector<bool>* isTight = {};
    vector<bool>* isMedium = {};
    vector<bool>* isLoose = {};
    vector<bool>* isInTime = {};

    t->SetBranchAddress("recoIDSoft", &isSoft);
    t->SetBranchAddress("recoIDTight", &isTight);
    t->SetBranchAddress("recoIDMedium", &isMedium);
    t->SetBranchAddress("recoIDLoose", &isLoose);


    TH1D* fail_pt = new TH1D("fail_pt", "fail_pt;muon pT ", 50, 0, 100);
    TH1D* miss_pt = new TH1D("miss_pt", "miss_pt;muon pT ", 50, 0, 100);
    TH1D* match_pt = new TH1D("match_pt", "match_pt;muon pT ", 50, 0, 100);
    TH1D* fail_Eta = new TH1D("fail_Eta", "fail_Eta;muon Eta ", 24, -2.4, 2.4);
    TH1D* match_Eta = new TH1D("match_Eta", "match_Eta;muon Eta ", 24, -2.4, 2.4);
    TH1D* miss_Eta = new TH1D("miss_Eta", "miss_Eta;muon Eta ", 24, -2.4, 2.4);

    int matched = 0;
    int missed = 0;
    int fake = 0;
    bool ismatched = 0;
    bool isgenmatched = 0;

    int goodrecos = 0;
    int goodgens = 0;

    

    for(int i = 0; i < t->GetEntries(); i++){
        t->GetEntry(i);

        if(i%100 == 0){
            cout << "Processing entry " << i << " / " << t->GetEntries() << endl;
        }
        
        if (nreco > 0 && ngen == 0) {

            for (int ir = 0; ir < nreco; ir++) {

                if(isGood->at(ir) && isTracker->at(ir) && isGlobal->at(ir) && isTight->at(ir) && isHybridSoft->at(ir) && mupt->at(ir) > 3.5 && fabs(mueta->at(ir)) < 2.4){
                //if(isGood->at(ir) && isTracker->at(ir) && isGlobal->at(ir) && isSoft->at(ir) && mupt->at(ir) > 3.5 && fabs(mueta->at(ir)) < 2.4){
                // if(isGood->at(ir) && isTracker->at(ir) && isGlobal->at(ir) && isHybridSoft->at(ir) && mupt->at(ir) > 3.5 && fabs(mueta->at(ir)) < 2.4){
                    fake++;
                    goodrecos++;
                    fail_pt->Fill(mupt->at(ir));
                    fail_Eta->Fill(mueta->at(ir));
                }
            }
        }

        if (ngen > 0 && nreco == 0){

            for(int ig = 0; ig < ngen; ig++){

                if(genmupt->at(ig) > 3.5 && fabs(genmueta->at(ig)) < 2.4){
                    missed++;
                    goodgens++;
                    miss_pt->Fill(genmupt->at(ig));
                    miss_Eta->Fill(genmueta->at(ig));
             
                }

            }

        }

        if(nreco > 0 && ngen > 0){

            for(int ir = 0; ir < nreco; ir++){

                if(isGood->at(ir) && isTracker->at(ir) && isGlobal->at(ir) && isTight->at(ir) && isHybridSoft->at(ir) && mupt->at(ir) > 3.5 && fabs(mueta->at(ir)) < 2.4){
                // if(isGood->at(ir) && isTracker->at(ir) && isGlobal->at(ir) && isHybridSoft->at(ir) && mupt->at(ir) > 3.5 && fabs(mueta->at(ir)) < 2.4){
                    goodrecos++;

                    for(int ig = 0; ig < ngen; ig++){

                        if(genmupt->at(ig) > 3.5 && fabs(genmueta->at(ig)) < 2.4){

                            float deltaR = dR(mueta->at(ir), muphi->at(ir), genmueta->at(ig), genmuphi->at(ig));

                            if(deltaR < 0.3){
                                matched++;
                                ismatched = true;
                                match_pt->Fill(mupt->at(ir));
                                match_Eta->Fill(mueta->at(ir));
                                break;
                            }

                        }

                    } // end gen loop


                    if(!ismatched){
                        fake++;
                        fail_pt->Fill(mupt->at(ir));
                        fail_Eta->Fill(mueta->at(ir));
                    }

                } // end good reco muon

            } // end reco loop  


            for(int ig = 0; ig < ngen; ig++){

                if(genmupt->at(ig) > 3.5 && fabs(genmueta->at(ig)) < 2.4){

                    isgenmatched = false;
                    goodgens++;

                    for(int ir = 0; ir < nreco; ir++){

                        //if(isGood->at(ir) && isTracker->at(ir) && isGlobal->at(ir) && isHybridSoft->at(ir) && mupt->at(ir) > 3.5 && fabs(mueta->at(ir)) < 2.4){
                        if(isGood->at(ir) && isTracker->at(ir) && isGlobal->at(ir) && isTight->at(ir) && isHybridSoft->at(ir) && mupt->at(ir) > 3.5 && fabs(mueta->at(ir)) < 2.4){
                            float deltaR = dR(mueta->at(ir), muphi->at(ir), genmueta->at(ig), genmuphi->at(ig));

                            if(deltaR < 0.3){
                                isgenmatched = true;
                                break;
                            }

                        }

                    } // end reco loop

                    if(!isgenmatched){
                        missed++;
                        miss_pt->Fill(genmupt->at(ig));
                        miss_Eta->Fill(genmueta->at(ig));
                    }


                }

            } // end gen loop

        }

    } // end tree loop


    TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
    c1->SetLogy();
    
    fail_pt->SetLineColor(kRed);
    miss_pt->SetLineColor(kBlue);
    match_pt->SetLineColor(kGreen+2);
    match_pt->Draw("HIST");
    fail_pt->Draw("HIST SAME");
    miss_pt->Draw("HIST SAME");
    
    TLegend* leg = new TLegend(0.6,0.7,0.9,0.9);
    leg->AddEntry(fail_pt, "Fake Reco Muons", "l");
    leg->AddEntry(miss_pt, "Missed Gen Muons", "l");
    leg->AddEntry(match_pt, "Matched Muons", "l");
    leg->Draw();
    c1->SaveAs("dimuon_efficiency_pt_tight.pdf");

    TCanvas* c2 = new TCanvas("c2", "c2", 800, 600);
    c2->SetLogy();
    fail_Eta->SetLineColor(kRed);
    miss_Eta->SetLineColor(kBlue);
    match_Eta->SetLineColor(kGreen+2);
    match_Eta->Draw("HIST");
    fail_Eta->Draw("HIST SAME");
    miss_Eta->Draw("HIST SAME");
    TLegend* leg2 = new TLegend(0.6,0.7,0.9,0.9);
    leg2->AddEntry(fail_Eta, "Fake Reco Muons", "l");
    leg2->AddEntry(miss_Eta, "Missed Gen Muons", "l");
    leg2->AddEntry(match_Eta, "Matched Muons", "l");
    leg2->Draw();
    c2->SaveAs("dimuon_efficiency_eta_tight.pdf");


    cout << "Total matched muons: " << matched << endl;
    cout << "Total missed muons: " << missed << endl;
    cout << "Total fake muons: " << fake << endl;
    cout << "Total good reco muons: " << goodrecos << endl;
    cout << "Total good gen muons: " << goodgens << endl;

}