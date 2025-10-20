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

float TrkPtWeight(TH1D* trkPtHist, float trkPtEntry){
    int bin = trkPtHist->FindBin(trkPtEntry);
    float weight = trkPtHist->GetBinContent(bin);
    return weight;
}

void TrkPtCheck(){

    cout << "trust" << endl;

    TCut MCCUT("(VZ > -15 && VZ < 15) && (PVFilter == 1) && (ClusterCompatibilityFilter == 1) && (HFEMaxPlus > 13.000000 && HFEMaxMinus > 13.000000)");
    TCut DATACUT("(VZ > -15 && VZ < 15) && (PVFilter == 1) && (ClusterCompatibilityFilter == 1) && (HFEMaxPlus > 13.000000 && HFEMaxMinus > 13.000000) &&  (HLT_MinimumBiasHF_OR_BptxAND_v1)"); 

    cout << "trust" << endl;

    TFile* f = TFile::Open("/data00/kdeverea/OOsamples/Skims/output_20250730_Skim_OO_IonPhysics0_LowPtV2_250711_104114_MB_CROSSCHECK/merged_0-49.root");
    TTree* TData = (TTree*)f->Get("Tree");

    TFile* fhijings = TFile::Open("/data00/kdeverea/OOsamples/Skims/output_20250728_Skim_OO_MinBias_OO_5p36TeV_hijing/20250728_Skim_OO_MinBias_OO_5p36TeV_hijing.root");
    TTree* TMC = (TTree*)fhijings->Get("Tree");

    TFile* fArg = TFile::Open("/data00/kdeverea/OOsamples/Skims/output_20250724_Skim_MinBias_Pythia_Angantyr_OO_5362GeV/20250724_Skim_MinBias_Pythia_Angantyr_OO_5362GeV.root");
    TTree* TMC_Arg = (TTree*)fArg->Get("Tree");

    TFile* fVZReweight = TFile::Open("../VZReweight/VZReweight.root");
    TH1D* vzReweight = (TH1D*)fVZReweight->Get("VZReweight");
    TH1D* vzReweight_Arg = (TH1D*)fVZReweight->Get("VZReweight_Arg");

    TFile* fMultReweight = TFile::Open("../MultReweight/MultReweight.root");
    TH1D* multReweight = (TH1D*)fMultReweight->Get("MultReweight");
    TH1D* multReweight_Arg = (TH1D*)fMultReweight->Get("MultReweight_Arg");

    TFile* fTrkPtReweight = TFile::Open("TrkPtReweight.root");
    TH1D* TrkPtReweight = (TH1D*)fTrkPtReweight->Get("TrkPtReweight");
    TH1D* TrkPtReweight_Arg = (TH1D*)fTrkPtReweight->Get("TrkPtReweight_Arg");

    cout << "trust" << endl;

    const Int_t nPtBins_log = 68;
    const Double_t pTBins_log[nPtBins_log + 1] = {
        0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.2,2.4,
        2.6,2.8,3.0,3.2,3.4,3.6,3.8,4.0,4.4,4.8,5.2,5.6,6.0,6.4,6.8,7.2,7.6,8.0,
        8.5,9.0,9.5,10.0,11.,12.,13.,14.,15.,16.,17.,18.,19.,20.,21.,22.6,24.6,
        26.6,28.6,32.6,36.6,42.6,48.6,54.6,60.6,74.0,86.4,103.6,120.8,140.,165.,
        250.,400.
    };

    TH1D* dataTrkPt = new TH1D("dataTrkPt", "Data Track pT Distribution", nPtBins_log, pTBins_log);
    dataTrkPt->Sumw2();
    TTreeFormula* dataformula = new TTreeFormula("dataformula", DATACUT, TData);

    TH1D* hTrkPt = new TH1D("hTrkPt", "Track pT Distribution", nPtBins_log, pTBins_log);
    hTrkPt->Sumw2();
    TTreeFormula* cutFormula = new TTreeFormula("cutFormula", MCCUT, TMC);  

    TH1D* hTrkPt_Arg = new TH1D("hTrkPt_Arg", "Track pT Distribution Angantyr", nPtBins_log, pTBins_log);
    hTrkPt_Arg->Sumw2();
    TTreeFormula* cutFormula_Arg = new TTreeFormula("cutFormula_Arg", MCCUT, TMC_Arg);


    TH1D* hTrkPt_uncorrected = new TH1D("hTrkPt_uncorrected", "Track pT Distribution Uncorrected", nPtBins_log, pTBins_log);
    hTrkPt_uncorrected->Sumw2();

    TH1D* hTrkPt_Arg_uncorrected = new TH1D("hTrkPt_Arg_uncorrected", "Track pT Distribution Angantyr Uncorrected", nPtBins_log, pTBins_log);
    hTrkPt_Arg_uncorrected->Sumw2();
    
    cout << "trust" << endl;

    /// FILL DATA
    vector<float>* trkPt = nullptr;
    vector<float>* quality = nullptr;
    float VZ = 0;
    int mult = 0;
    TData->SetBranchAddress("trkPt", &trkPt);
    TData->SetBranchAddress("trkPassChargedHadron_Nominal", &quality);
    TData->SetBranchAddress("VZ", &VZ);
    TData->SetBranchAddress("multiplicityEta2p4", &mult);

    for(int i = 0; i < TData->GetEntries(); i++){
        TData->GetEntry(i);
        if(i % 10000 == 0) cout << "Processing Data Entry: " << i << " out of " << TData->GetEntries() << endl;
        if(dataformula->EvalInstance() == 0) continue;
        if (trkPt == nullptr || trkPt->empty()) continue;    
        // Fill data histogram
        for(int j = 0; j< trkPt->size(); j++){
            if(quality && j < quality->size() && quality->at(j)){
                dataTrkPt->Fill(trkPt->at(j), 1);
            }
        }
    }
    dataTrkPt->Scale(1.0/dataTrkPt->Integral());

    // FILL HIJING 
    vector<float>* trkPt_mc = nullptr;
    vector<bool>* quality_mc = nullptr;
    float VZ_mc = 0;
    int mult_mc = 0;
    TMC->SetBranchAddress("trkPt", &trkPt_mc);
    TMC->SetBranchAddress("trkPassChargedHadron_Nominal", &quality_mc);
    TMC->SetBranchAddress("VZ", &VZ_mc);
    TMC->SetBranchAddress("multiplicityEta2p4", &mult_mc);
    for(int i = 0; i < TMC->GetEntries(); i++){
        TMC->GetEntry(i);
        if(i % 10000 == 0) cout << "Processing MC Entry: " << i << " out of " << TMC->GetEntries() << endl;
        if(cutFormula->EvalInstance() == 0) continue;
        if (trkPt_mc == nullptr || trkPt_mc->empty()) continue;
        float vzweight = VZWeight(vzReweight, VZ_mc);
        float multweight = MultWeight(multReweight, mult_mc);

        for(int j = 0; j < trkPt_mc->size(); j++){
            if(quality_mc && j < quality_mc->size() && quality_mc->at(j)){
                hTrkPt->Fill(trkPt_mc->at(j), multweight * vzweight * TrkPtWeight(TrkPtReweight, trkPt_mc->at(j)));
                hTrkPt_uncorrected->Fill(trkPt_mc->at(j), multweight * vzweight);
            }
        }
    }
    hTrkPt->Scale(1.0/hTrkPt->Integral());
    hTrkPt_uncorrected->Scale(1.0/hTrkPt_uncorrected->Integral());

    // FILL ANGANTYR
    vector<float>* trkPt_mc_arg = nullptr;
    vector<bool>* quality_mc_arg = nullptr;
    float VZ_mc_arg = 0;
    int mult_mc_arg = 0;
    TMC_Arg->SetBranchAddress("trkPt", &trkPt_mc_arg);
    TMC_Arg->SetBranchAddress("trkPassChargedHadron_Nominal", &quality_mc_arg);
    TMC_Arg->SetBranchAddress("VZ", &VZ_mc_arg);
    TMC_Arg->SetBranchAddress("multiplicityEta2p4", &mult_mc_arg);
    for(int i = 0; i < TMC_Arg->GetEntries(); i++){
        TMC_Arg->GetEntry(i);
        if(i % 10000 == 0) cout << "Processing Angantyr Entry: " << i << " out of " << TMC_Arg->GetEntries() << endl;
        if(cutFormula_Arg->EvalInstance() == 0) continue;
        if (trkPt_mc_arg == nullptr || trkPt_mc_arg->empty()) continue;
        float vzweight = VZWeight(vzReweight_Arg, VZ_mc_arg);
        float multweight = MultWeight(multReweight_Arg, mult_mc_arg);
        for(int j = 0; j < trkPt_mc_arg->size(); j++){
            if(quality_mc_arg && j < quality_mc_arg->size() && quality_mc_arg->at(j)){
                hTrkPt_Arg->Fill(trkPt_mc_arg->at(j), multweight * vzweight * TrkPtWeight(TrkPtReweight_Arg, trkPt_mc_arg->at(j)));
                hTrkPt_Arg_uncorrected->Fill(trkPt_mc_arg->at(j), multweight * vzweight);
            }
        }
    }
    hTrkPt_Arg->Scale(1.0/hTrkPt_Arg->Integral());
    hTrkPt_Arg_uncorrected->Scale(1.0/hTrkPt_Arg_uncorrected->Integral());


    cout << "Drawing Histograms" << endl;

    // Create canvas for HIJING vs Data comparison
    TCanvas* c1 = new TCanvas("c1", "HIJING Track pT Comparison with Data", 1200, 800);
    c1->Divide(2, 2);
    
    // Top left: Corrected HIJING vs Data
    c1->cd(1);
    gPad->SetLogy();
    gPad->SetLogx();
    
    // Clone data histogram to avoid title conflicts
    TH1D* dataTrkPt_clone1 = (TH1D*)dataTrkPt->Clone("dataTrkPt_clone1");
    dataTrkPt_clone1->SetLineColor(kBlack);
    dataTrkPt_clone1->SetMarkerColor(kBlack);
    dataTrkPt_clone1->SetMarkerStyle(20);
    dataTrkPt_clone1->GetXaxis()->SetRangeUser(0.5, 400.0);
    dataTrkPt_clone1->SetTitle("Corrected HIJING vs Data;Track p_{T} (GeV/c);Normalized Counts");
    dataTrkPt_clone1->Draw("EP");
    
    hTrkPt->SetLineColor(kRed);
    hTrkPt->SetMarkerColor(kRed);
    hTrkPt->SetMarkerStyle(21);
    hTrkPt->Draw("EP SAME");
    
    TLegend* leg1 = new TLegend(0.6, 0.7, 0.9, 0.9);
    leg1->AddEntry(dataTrkPt_clone1, "Data", "lep");
    leg1->AddEntry(hTrkPt, "HIJING (Corrected)", "lep");
    leg1->Draw();
    
    // Top right: Uncorrected HIJING vs Data
    c1->cd(2);
    gPad->SetLogy();
    gPad->SetLogx();
    
    TH1D* dataTrkPt_clone2 = (TH1D*)dataTrkPt->Clone("dataTrkPt_clone2");
    dataTrkPt_clone2->SetLineColor(kBlack);
    dataTrkPt_clone2->SetMarkerColor(kBlack);
    dataTrkPt_clone2->SetMarkerStyle(20);
    dataTrkPt_clone2->GetXaxis()->SetRangeUser(0.5, 400.0);
    dataTrkPt_clone2->SetTitle("Uncorrected HIJING vs Data;Track p_{T} (GeV/c);Normalized Counts");
    dataTrkPt_clone2->Draw("EP");
    
    hTrkPt_uncorrected->SetLineColor(kBlue);
    hTrkPt_uncorrected->SetMarkerColor(kBlue);
    hTrkPt_uncorrected->SetMarkerStyle(22);
    hTrkPt_uncorrected->Draw("EP SAME");
    
    TLegend* leg2 = new TLegend(0.6, 0.7, 0.9, 0.9);
    leg2->AddEntry(dataTrkPt_clone2, "Data", "lep");
    leg2->AddEntry(hTrkPt_uncorrected, "HIJING (Uncorrected)", "lep");
    leg2->Draw();
    
    // Bottom left: Ratio Corrected HIJING/Data
    c1->cd(3);
    gPad->SetLogx();
    TH1D* ratio_hijing_corr = (TH1D*)hTrkPt->Clone("ratio_hijing_corr");
    ratio_hijing_corr->Divide(dataTrkPt);
    ratio_hijing_corr->SetTitle("Ratio Corrected HIJING/Data;Track p_{T} (GeV/c);MC/Data");
    ratio_hijing_corr->SetLineColor(kRed);
    ratio_hijing_corr->SetMarkerColor(kRed);
    ratio_hijing_corr->SetMarkerStyle(21);
    ratio_hijing_corr->GetYaxis()->SetRangeUser(0.5, 1.5);
    ratio_hijing_corr->GetXaxis()->SetRangeUser(0.5, 400.0);
    ratio_hijing_corr->Draw("EP");
    
    TLine* line1 = new TLine(0.5, 1.0, 400.0, 1.0);
    line1->SetLineStyle(2);
    line1->SetLineColor(kBlack);
    line1->Draw();
    
    // Bottom right: Ratio Uncorrected HIJING/Data
    c1->cd(4);
    gPad->SetLogx();
    TH1D* ratio_hijing_uncorr = (TH1D*)hTrkPt_uncorrected->Clone("ratio_hijing_uncorr");
    ratio_hijing_uncorr->Divide(dataTrkPt);
    ratio_hijing_uncorr->SetTitle("Ratio Uncorrected HIJING/Data;Track p_{T} (GeV/c);MC/Data");
    ratio_hijing_uncorr->SetLineColor(kBlue);
    ratio_hijing_uncorr->SetMarkerColor(kBlue);
    ratio_hijing_uncorr->SetMarkerStyle(22);
    ratio_hijing_uncorr->GetYaxis()->SetRangeUser(0.5, 1.5);
    ratio_hijing_uncorr->GetXaxis()->SetRangeUser(0.5, 400.0);
    ratio_hijing_uncorr->Draw("EP");
    
    TLine* line2 = new TLine(0.5, 1.0, 400.0, 1.0);
    line2->SetLineStyle(2);
    line2->SetLineColor(kBlack);
    line2->Draw();
    
    // Save canvas
    c1->SaveAs("TrkPt_HIJING_Comparison.pdf");
    
    cout << "Canvas saved as TrkPt_HIJING_Comparison.pdf" << endl;




}