#include <fstream>
#include <string>
#include <vector>
#include <TChain.h>
#include <ROOT/RDataFrame.hxx>

float fill_value(float value = 1.0, int doreweight = 1) {
    if (doreweight == 1) {
        return value;
    } else {
        return 1.0;
    }
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

bool passCut(TChain* chain, Long64_t entry, TTreeFormula* formula) {
    Long64_t localEntry = chain->LoadTree(entry);
    if (localEntry < 0) return false; 
    formula->UpdateFormulaLeaves();
    chain->GetEntry(entry);
    return formula->EvalInstance();
}

TChain* createDataChain(const char* dataSource) {
    TChain* chain = new TChain("Tree");
    string source(dataSource);
    
    cout << "Creating TChain from: " << dataSource << endl;
    
    if (source.find(".txt") != string::npos) {
        ifstream infile(dataSource);
        if (!infile.is_open()) {
            cout << "ERROR: Cannot open file: " << dataSource << endl;
            return nullptr;
        }
        string line;
        int fileCount = 0;
        while (getline(infile, line)) {
            if (!line.empty() && line[0] != '#') {
                cout << "Adding file: " << line << endl;
                int result = chain->Add(line.c_str());
                cout << "  -> Result: " << result << " entries" << endl;
                fileCount++;
            }
        }
        cout << "Added " << fileCount << " files to chain" << endl;
    } else {
        cout << "Adding single file: " << source << endl;
        int result = chain->Add(source.c_str());
        cout << "  -> Result: " << result << " entries" << endl;
    }
    
    cout << "TChain has " << chain->GetEntries() << " total entries from " << chain->GetNtrees() << " files" << endl;
    
    if (chain->GetEntries() == 0) {
        cout << "ERROR: TChain is empty!" << endl;
        delete chain;
        return nullptr;
    }
    
    return chain;
}

void TrkPtReweight(
                TCut Datacut,
                TCut MCcut,
                const char* dataSource = "datafiles.txt",
                const char* ooskim = "/data00/OOsamples/Skims20250704/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root",
                const char* ooskim_arg = "/data00/OOsamples/Skims20250704/20250704_HiForest_250520_Pythia_Angantyr_OO_OO_5362GeV_250626.root",
                int doreweight = 1
                 ){

    cout << "STARTING TRACK PT REWEIGHT" << endl;
    
    // Enable ROOT multithreading for faster processing
    ROOT::EnableImplicitMT();

    //////// READ FILES - GET VZ REWEIGHTED HISTOGRAMS ////////
    TChain* chainData = createDataChain(dataSource);
    TFile* fOO = TFile::Open(ooskim);
    TFile* fOO_Arg = TFile::Open(ooskim_arg);
    TFile* fVZReweight = TFile::Open("../VZReweight/VZReweight.root");
    TFile* fMultReweight = TFile::Open("../MultReweight/MultReweight.root");

    cout << "PROCESSING FILES:" << endl;
    cout << "Data: " << dataSource << endl;
    cout << "OO: " << ooskim << endl;
    cout << "OO Arg: " << ooskim_arg << endl;

    TTree* tOO = (TTree*)fOO->Get("Tree");
    TTree* tOO_Arg = (TTree*)fOO_Arg->Get("Tree");

    TH1D* vzReweight = (TH1D*)fVZReweight->Get("VZReweight");
    TH1D* vzReweight_Arg = (TH1D*)fVZReweight->Get("VZReweight_Arg");

    TH1D* multReweight = (TH1D*)fMultReweight->Get("MultReweight");
    TH1D* multReweight_Arg = (TH1D*)fMultReweight->Get("MultReweight_Arg");

    /// PRINT SELECTION CUTS
    cout << "SELECTION FOR DATA TRACK PT HIST: " << endl;
    cout << Datacut.GetTitle() << endl;
    cout << "SELECTION FOR MC TRACK PT HIST: " << endl;
    cout << MCcut.GetTitle() << endl;

    ///////// OBTAIN HISTOGRAMS FOR TRACK PT INFO ////////
    const Int_t nPtBins_log = 48;
    const Double_t pTBins_log[nPtBins_log+ 1] = {
        3.0,3.2,3.4,3.6,3.8,4.0,4.4,4.8,5.2,5.6,6.0,6.4,6.8,7.2,7.6,8.0,
        8.5,9.0,9.5,10.0,11.,12.,13.,14.,15.,16.,17.,18.,19.,20.,21.,22.6,24.6,
        26.6,28.6,32.6,36.6,42.6,48.6,54.6,60.6,74.0,86.4,103.6,120.8,140.,165.,
        250.,400.
    };
    
    /*const Int_t nPtBins_log = 26;
    const Double_t pTBins_log[nPtBins_log + 1] = {
        0.5, 0.603, 0.728, 0.879, 1.062, 1.284, 1.553, 1.878, 2.272, 2.749, 3.327, 4.027, 
        4.872, 5.891, 7.117, 8.591, 10.36, 12.48, 15.03, 18.08, 21.73, 26.08, 31.28, 
        37.48, 44.89, 53.73, 64.31
    };*/

    TH1D* hDataTrkPt = new TH1D("hDataTrkPt", "Data Track p_{T}", nPtBins_log, pTBins_log);
    TH1D* hMCTrkPt = new TH1D("hMCTrkPt", "HIJING Track p_{T}", nPtBins_log, pTBins_log);
    TH1D* hMCARGTrkPt = new TH1D("hMCARGTrkPt", "Angantyr Track p_{T}", nPtBins_log, pTBins_log);

    // OBTAIN DATA HISTOGRAM using RDataFrame
    ROOT::RDataFrame dfData(*chainData);
    
    // Convert TCut to string for RDataFrame
    std::string dataCutStr = Datacut.GetTitle();
    
    auto dfDataFiltered = dfData.Filter(dataCutStr);
    
    // Check if the branch exists
    bool hasDataTrackPass = chainData->GetBranch("trkPassChargedHadron_Nominal") != nullptr;
    
    auto fillDataHist = [&](const ROOT::RVec<float>& trkPt, const ROOT::RVec<bool>& trkPass) {
        if(trkPt.empty()) return;
        for(size_t j = 0; j < trkPt.size(); j++) {
            bool passTrack = hasDataTrackPass ? (j < trkPass.size() && trkPass[j]) : true; // Default to true if branch doesn't exist
            if(passTrack) {
                hDataTrkPt->Fill(trkPt[j], fill_value(1.0, doreweight));
            }
        }
    };
    
    if(hasDataTrackPass) {
        dfDataFiltered.Foreach(fillDataHist, {"trkPt", "trkPassChargedHadron_Nominal"});
    } else {
        cout << "WARNING: trkPassChargedHadron_Nominal branch not found in data, filling all tracks" << endl;
        auto fillDataHistNoPass = [&](const ROOT::RVec<float>& trkPt) {
            if(trkPt.empty()) return;
            for(size_t j = 0; j < trkPt.size(); j++) {
                hDataTrkPt->Fill(trkPt[j], fill_value(1.0, doreweight));
            }
        };
        dfDataFiltered.Foreach(fillDataHistNoPass, {"trkPt"});
    }
    hDataTrkPt->Scale(1.0/hDataTrkPt->Integral());

    // OBTAIN HIJING HISTOGRAM using RDataFrame
    ROOT::RDataFrame dfMC(*tOO);
    
    // Convert TCut to string for RDataFrame
    std::string mcCutStr = MCcut.GetTitle();
    
    auto dfMCFiltered = dfMC.Filter(mcCutStr);
    
    // Check if the branch exists
    bool hasMCTrackPass = tOO->GetBranch("trkPassChargedHadron_Nominal") != nullptr;
    
    auto fillMCHist = [&](const ROOT::RVec<float>& trkPt, const ROOT::RVec<bool>& trkPass, int mult, float vz) {
        if(trkPt.empty()) return;
        float multWeight = MultWeight(multReweight, mult);
        float vzWeight = VZWeight(vzReweight, vz);
        for(size_t j = 0; j < trkPt.size(); j++) {
            bool passTrack = hasMCTrackPass ? (j < trkPass.size() && trkPass[j]) : true; // Default to true if branch doesn't exist
            if(passTrack) {
                hMCTrkPt->Fill(trkPt[j], fill_value(multWeight * vzWeight, doreweight));
            }
        }
    };
    
    if(hasMCTrackPass) {
        dfMCFiltered.Foreach(fillMCHist, {"trkPt", "trkPassChargedHadron_Nominal", "multiplicityEta2p4", "VZ"});
    } else {
        cout << "WARNING: trkPassChargedHadron_Nominal branch not found in MC, filling all tracks" << endl;
        auto fillMCHistNoPass = [&](const ROOT::RVec<float>& trkPt, int mult, float vz) {
            if(trkPt.empty()) return;
            float multWeight = MultWeight(multReweight, mult);
            float vzWeight = VZWeight(vzReweight, vz);
            for(size_t j = 0; j < trkPt.size(); j++) {
                hMCTrkPt->Fill(trkPt[j], fill_value(multWeight * vzWeight, doreweight));
            }
        };
        dfMCFiltered.Foreach(fillMCHistNoPass, {"trkPt", "multiplicityEta2p4", "VZ"});
    }
    hMCTrkPt->Scale(1.0/hMCTrkPt->Integral());
    cout << endl;

    // OBTAIN ANGANTYR HISTOGRAM using RDataFrame
    ROOT::RDataFrame dfMCArg(*tOO_Arg);
    
    // Convert TCut to string for RDataFrame  
    std::string mcArgCutStr = MCcut.GetTitle();
    
    auto dfMCArgFiltered = dfMCArg.Filter(mcArgCutStr);
    
    // Check if the branch exists
    bool hasMCArgTrackPass = tOO_Arg->GetBranch("trkPassChargedHadron_Nominal") != nullptr;
    
    auto fillMCArgHist = [&](const ROOT::RVec<float>& trkPt, const ROOT::RVec<bool>& trkPass, int mult, float vz) {
        if(trkPt.empty()) return;
        float multWeight = MultWeight(multReweight_Arg, mult);
        float vzWeight = VZWeight(vzReweight_Arg, vz);
        for(size_t j = 0; j < trkPt.size(); j++) {
            bool passTrack = hasMCArgTrackPass ? (j < trkPass.size() && trkPass[j]) : true; // Default to true if branch doesn't exist
            if(passTrack) {
                hMCARGTrkPt->Fill(trkPt[j], fill_value(multWeight * vzWeight, doreweight));
            }
        }
    };
    
    if(hasMCArgTrackPass) {
        dfMCArgFiltered.Foreach(fillMCArgHist, {"trkPt", "trkPassChargedHadron_Nominal", "multiplicityEta2p4", "VZ"});
    } else {
        cout << "WARNING: trkPassChargedHadron_Nominal branch not found in Angantyr MC, filling all tracks" << endl;
        auto fillMCArgHistNoPass = [&](const ROOT::RVec<float>& trkPt, int mult, float vz) {
            if(trkPt.empty()) return;
            float multWeight = MultWeight(multReweight_Arg, mult);
            float vzWeight = VZWeight(vzReweight_Arg, vz);
            for(size_t j = 0; j < trkPt.size(); j++) {
                hMCARGTrkPt->Fill(trkPt[j], fill_value(multWeight * vzWeight, doreweight));
            }
        };
        dfMCArgFiltered.Foreach(fillMCArgHistNoPass, {"trkPt", "multiplicityEta2p4", "VZ"});
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
    
    TH1D* hDataTrkPt_norm = (TH1D*)hDataTrkPt->Clone("hDataTrkPt_norm");
    TH1D* hMCTrkPt_norm = (TH1D*)hMCTrkPt->Clone("hMCTrkPt_norm");
    TH1D* hMCARGTrkPt_norm = (TH1D*)hMCARGTrkPt->Clone("hMCARGTrkPt_norm");
    
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
    
    double maxVal = hMCTrkPt_norm->GetMaximum();
    hDataTrkPt_norm->SetMaximum(maxVal * 15);
    hDataTrkPt_norm->GetXaxis()->SetRangeUser(0.5, 400);

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

    // Close input files
    fOO->Close();
    fOO_Arg->Close();
    fVZReweight->Close();
    fMultReweight->Close();

    cout << "DONE WITH TRACK PT REWEIGHT" << endl;
}