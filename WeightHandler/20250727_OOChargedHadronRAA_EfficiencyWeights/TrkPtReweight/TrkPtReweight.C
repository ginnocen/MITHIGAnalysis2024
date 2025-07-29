float fill_value(float value = 1.0, int doreweight = 1) {
    if (doreweight == 1) {
        return value;
    } else {
        return 1.0;
    }
}

void histfromtree(TTree* T, TCut t, TH1D* h, const char* branch){
     T->Project(h->GetName(), branch, t);
     h->Scale(1.0/h->Integral());
}

float VZWeight(TH1D* VZ, float vzentry){
    int bin = VZ->FindBin(vzentry);
    float weight = VZ->GetBinContent(bin);
    return weight;
}

float MultWeight(TH1D* multHist, int multEntry){
    int bin = multHist->FindBin(multEntry);
    float weight = multHist->GetBinContent(bin);
    return weight;
}

void TrkPtReweight(
                TCut Datacut,
                TCut MCcut,
                const char* dataskim = "/data00/bakovacs/OOsamples/Skims/20250705_OO_394153_PhysicsIonPhysics0_074244.root",
                const char* ooskim = "/data00/OOsamples/Skims20250704/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root",
                const char* ooskim_arg = "/data00/OOsamples/Skims20250704/20250704_HiForest_250520_Pythia_Angantyr_OO_OO_5362GeV_250626.root",
                int doreweight = 1
                 ){

    cout << "STARTING TRACK PT REWEIGHT" << endl;

    //////// READ FILES - GET VZ REWEIGHTED HISTOGRAMS ////////
    TFile* fData = TFile::Open(dataskim); 
    TFile* fOO = TFile::Open(ooskim);
    TFile* fOO_Arg = TFile::Open(ooskim_arg);
    TFile* fVZReweight = TFile::Open("../VZReweight/VZReweight.root");
    TFile* fMultReweight = TFile::Open("../MultReweight/MultReweight.root");

    cout << "PROCESSING FILES:" << endl;
    cout << "Data: " << dataskim << endl;
    cout << "OO: " << ooskim << endl;
    cout << "OO Arg: " << ooskim_arg << endl;

    TTree* tData = (TTree*)fData->Get("Tree");
    TTree* tOO = (TTree*)fOO->Get("Tree");
    TTree* tOO_Arg = (TTree*)fOO_Arg->Get("Tree");

    TH1D* vzReweight = (TH1D*)fVZReweight->Get("reweight");
    TH1D* vzReweight_Arg = (TH1D*)fVZReweight->Get("reweight_Arg");

    TH1D* multReweight = (TH1D*)fMultReweight->Get("multReweight");
    TH1D* multReweight_Arg = (TH1D*)fMultReweight->Get("multReweight_Arg");

    /// PRINT SELECTION CUTS
    cout << "SELECTION FOR DATA TRACK PT HIST: " << endl;
    cout << Datacut.GetTitle() << endl;
    cout << "SELECTION FOR MC TRACK PT HIST: " << endl;
    cout << MCcut.GetTitle() << endl;

    ///////// OBTAIN HISTOGRAMS FOR TRACK PT INFO ////////

    const Int_t nPtBins_log = 26;
    const Double_t pTBins_log[nPtBins_log + 1] = {
        0.5, 0.603, 0.728, 0.879, 1.062, 1.284, 1.553, 1.878, 2.272, 2.749, 3.327, 4.027, 
        4.872, 5.891, 7.117, 8.591, 10.36, 12.48, 15.03, 18.08, 21.73, 26.08, 31.28, 
        37.48, 44.89, 53.73, 64.31
    };

    TH1D* hDataTrkPt = new TH1D("hDataTrkPt", "Data Track p_{T}", nPtBins_log, pTBins_log);
    TH1D* hMCTrkPt = new TH1D("hMCTrkPt", "HIJING Track p_{T}", nPtBins_log, pTBins_log);
    TH1D* hMCARGTrkPt = new TH1D("hMCARGTrkPt", "Angantyr Track p_{T}", nPtBins_log, pTBins_log);

    // OBTAIN DATA HISTOGRAM
    histfromtree(tData, Datacut, hDataTrkPt, "trkPt");
    hDataTrkPt->Scale(1.0/hDataTrkPt->Integral());

    // OBTAIN HIJING HISTOGRAM 
    TTreeFormula* mcCutFormula = new TTreeFormula("mcCutFormula", MCcut, tOO);
    vector<float>* trackPt_mc = nullptr;
    int multiplicityEta2p4_mc;
    float VZ_mc;
    tOO->SetBranchAddress("trkPt", &trackPt_mc);
    tOO->SetBranchAddress("multiplicityEta2p4", &multiplicityEta2p4_mc);
    tOO->SetBranchAddress("VZ", &VZ_mc);
    for(int i = 0; i < tOO->GetEntries(); i++){
        if(i % 1000 == 0) cout << i*1.0 / tOO->GetEntries() * 100.0 << "% PROCESSED FOR HIJING TRACK PT HIST \r";
        tOO->GetEntry(i);
        if(mcCutFormula->EvalInstance() == 0) continue;
        if(!trackPt_mc || trackPt_mc->empty()) continue;
        float multWeight = MultWeight(multReweight, multiplicityEta2p4_mc);
        float vzWeight = VZWeight(vzReweight, VZ_mc);
        for(auto& pt : *trackPt_mc) {
            hMCTrkPt->Fill(pt, fill_value(multWeight * vzWeight, doreweight));
        }
    }
    hMCTrkPt->Scale(1.0/hMCTrkPt->Integral());
    cout << endl;

    // OBTAIN ANGANTYR HISTOGRAM using cut formula with VZ reweighting
    TTreeFormula* mcArgCutFormula = new TTreeFormula("mcArgCutFormula", MCcut, tOO_Arg);
    vector<float>* trackPt_mc_arg = nullptr;
    int multiplicityEta2p4_arg;
    float VZ_arg;
    tOO_Arg->SetBranchAddress("trkPt", &trackPt_mc_arg);
    tOO_Arg->SetBranchAddress("multiplicityEta2p4", &multiplicityEta2p4_arg);
    tOO_Arg->SetBranchAddress("VZ", &VZ_arg);
    for(int i = 0; i < tOO_Arg->GetEntries(); i++){
        if(i % 1000 == 0) cout << i*1.0 / tOO_Arg->GetEntries() * 100.0 << "% PROCESSED FOR ANGANTYR TRACK PT HIST \r";
        tOO_Arg->GetEntry(i);
        if(mcArgCutFormula->EvalInstance() == 0) continue;
        if(!trackPt_mc_arg || trackPt_mc_arg->empty()) continue;
        float multWeight = MultWeight(multReweight_Arg, multiplicityEta2p4_arg);
        float vzWeight = VZWeight(vzReweight_Arg, VZ_arg);
        for(auto& pt : *trackPt_mc_arg) {
            hMCARGTrkPt->Fill(pt, fill_value(multWeight * vzWeight, doreweight));
        }
    }
    hMCARGTrkPt->Scale(1.0/hMCARGTrkPt->Integral());
    cout << endl;

    //////// CREATE REWEIGHT FACTORS ////////
    TH1D* TrkPtReweight = (TH1D*)hDataTrkPt->Clone("TrkPtReweight");
    TrkPtReweight->Divide(hDataTrkPt, hMCTrkPt);

    TH1D* TrkPtReweight_Arg = (TH1D*)hDataTrkPt->Clone("TrkPtReweight_Arg");
    TrkPtReweight_Arg->Divide(hDataTrkPt, hMCARGTrkPt);

    ///////// DRAW REWEIGHT HISTOGRAM ///////
    gStyle->SetOptStat(0);
    TCanvas* c = new TCanvas("c", "Track pT  Reweight Factor", 800, 600);
    c->SetLogx();
    
    // Find maximum ratio value for y-axis scaling
    double maxRatio = TMath::Max(TrkPtReweight->GetMaximum(), TrkPtReweight_Arg->GetMaximum());
    double yMax = maxRatio * 1.5; // Add 50% margin

    TrkPtReweight->GetXaxis()->SetTitle("Track p_{T} (GeV/c)");
    TrkPtReweight->GetYaxis()->SetTitle("Reweight Factor (Data/MC)");
    TrkPtReweight->GetXaxis()->SetRangeUser(0.4, 64.31);
    TrkPtReweight->SetMaximum(yMax);
    TrkPtReweight->SetMinimum(0);
    TrkPtReweight->SetLineColor(kBlue);
    TrkPtReweight->SetLineWidth(2);
    TrkPtReweight->Draw("HIST");

    TrkPtReweight_Arg->SetLineColor(kOrange+7);
    TrkPtReweight_Arg->SetLineWidth(2);
    TrkPtReweight_Arg->Draw("HIST SAME");

    TLegend* leg = new TLegend(0.3, 0.7, 0.7, 0.9);
    leg->AddEntry(TrkPtReweight, "Data/MC Track pT HIJING", "l");
    leg->AddEntry(TrkPtReweight_Arg, "Data/MC Track pT Angantyr", "l");
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->Draw();
    
    // Add horizontal line at y=1
    TLine* line = new TLine(0.4, 1, 64.31, 1);
    line->SetLineColor(kRed);
    line->SetLineStyle(2);
    line->Draw("same");

    ///////// DRAW TRACK pT DISTRIBUTIONS ///////
    TCanvas* c2 = new TCanvas("c2", "Track pT Distributions", 800, 600);
    c2->SetLogy();
    c2->SetLogx();
    
    // Create copies for bin width normalization
    TH1D* hDataTrkPt_norm = (TH1D*)hDataTrkPt->Clone("hDataTrkPt_norm");
    TH1D* hMCTrkPt_norm = (TH1D*)hMCTrkPt->Clone("hMCTrkPt_norm");
    TH1D* hMCARGTrkPt_norm = (TH1D*)hMCARGTrkPt->Clone("hMCARGTrkPt_norm");
    
    // Divide by bin width for each histogram
    for(int i = 1; i <= hDataTrkPt_norm->GetNbinsX(); i++){
        double binWidth = hDataTrkPt_norm->GetBinWidth(i);
        
        double dataContent = hDataTrkPt_norm->GetBinContent(i);
        double dataError = hDataTrkPt_norm->GetBinError(i);
        hDataTrkPt_norm->SetBinContent(i, dataContent / binWidth);
        hDataTrkPt_norm->SetBinError(i, dataError / binWidth);
        
        double mcContent = hMCTrkPt_norm->GetBinContent(i);
        double mcError = hMCTrkPt_norm->GetBinError(i);
        hMCTrkPt_norm->SetBinContent(i, mcContent / binWidth);
        hMCTrkPt_norm->SetBinError(i, mcError / binWidth);
        
        double mcArgContent = hMCARGTrkPt_norm->GetBinContent(i);
        double mcArgError = hMCARGTrkPt_norm->GetBinError(i);
        hMCARGTrkPt_norm->SetBinContent(i, mcArgContent / binWidth);
        hMCARGTrkPt_norm->SetBinError(i, mcArgError / binWidth);
    }
    
    // Set colors and styles for the distributions
    hDataTrkPt_norm->SetLineColor(kBlack);
    hDataTrkPt_norm->SetLineWidth(2);
    hDataTrkPt_norm->SetMarkerStyle(20);
    hDataTrkPt_norm->SetMarkerSize(0.8);
    hDataTrkPt_norm->GetXaxis()->SetTitle("Track p_{T} (GeV/c)");
    hDataTrkPt_norm->GetYaxis()->SetTitle("dN/dp_{T} (Normalized Events)");
    hDataTrkPt_norm->SetTitle("Track p_{T} Distributions");

    hMCTrkPt_norm->SetLineColor(kBlue);
    hMCTrkPt_norm->SetLineWidth(2);
    
    hMCARGTrkPt_norm->SetLineColor(kOrange+7);
    hMCARGTrkPt_norm->SetLineWidth(2);
    
    // Find maximum for proper scaling
    double maxVal = hMCTrkPt_norm->GetMaximum();
    hDataTrkPt_norm->SetMaximum(maxVal * 15);
    hDataTrkPt_norm->GetXaxis()->SetRangeUser(0.5, 64.31);

    hDataTrkPt_norm->Draw("PE");
    hMCTrkPt_norm->Draw("HIST SAME");
    hMCARGTrkPt_norm->Draw("HIST SAME");
    
    TLegend* leg2 = new TLegend(0.65, 0.7, 0.85, 0.9);
    leg2->AddEntry(hDataTrkPt_norm, "Data", "pe");
    leg2->AddEntry(hMCTrkPt_norm, "MC HIJING (VZ+Mult reweighted)", "l");
    leg2->AddEntry(hMCARGTrkPt_norm, "MC Angantyr (VZ+Mult reweighted)", "l");
    leg2->SetBorderSize(0);
    leg2->SetFillStyle(0);
    leg2->Draw();

    // Save the reweight histogram and canvas
    TFile* fout = TFile::Open("TrkPtReweight.root", "RECREATE");
    TrkPtReweight->Write();
    TrkPtReweight_Arg->Write();
    hDataTrkPt->Write();
    hMCTrkPt->Write();
    hMCARGTrkPt->Write();
    c->Write();
    c2->Write();
    fout->Close();
    c->SaveAs("TrkPTReweight.pdf");
    c2->SaveAs("TrkPTDistributions.pdf");


    cout << "DONE WITH TRACK PT REWEIGHT" << endl;

}

