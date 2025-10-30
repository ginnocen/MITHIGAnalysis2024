#include <fstream>
#include <string>
#include <vector>
#include <TChain.h>

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

TChain* createDataChain(const char* dataSource) {
    TChain* chain = new TChain("Tree");
    string source(dataSource);
    if (source.find(".txt") != string::npos) {
        ifstream infile(dataSource);
        string line;
        while (getline(infile, line)) {
            if (!line.empty() && line[0] != '#') {
                chain->Add(line.c_str());
            }
        }
    }
    return chain;
}

void MultReweight(
                TCut Datacut,
                TCut MCcut,
                const char* dataSource = "datafiles.txt",
                const char* ooskim = "/data00/OOsamples/Skims20250704/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root",
                const char* ooskim_arg = "/data00/OOsamples/Skims20250704/20250704_HiForest_250520_Pythia_Angantyr_OO_OO_5362GeV_250626.root",
                int doreweight = 1
                ){
    
    cout << "STARTING MULTIPLICITY REWEIGHT" << endl;
    
    // Enable ROOT multithreading for faster processing
    ROOT::EnableImplicitMT();

    //////// READ FILES - GET VZ REWEIGHTED HISTOGRAMS ////////
    TChain* chainData = createDataChain(dataSource);
    TFile* fOO = TFile::Open(ooskim);
    TFile* fOO_Arg = TFile::Open(ooskim_arg);
    TFile* fVZReweight = TFile::Open("../VZReweight/VZReweight.root");

    cout << "PROCESSING FILES:" << endl;
    cout << "Data: " << dataSource << endl;
    cout << "OO: " << ooskim << endl;
    cout << "OO Arg: " << ooskim_arg << endl;

    TTree* tOO = (TTree*)fOO->Get("Tree");
    TTree* tOO_Arg = (TTree*)fOO_Arg->Get("Tree");

    TH1D* vzReweight = (TH1D*)fVZReweight->Get("VZReweight");
    TH1D* vzReweight_Arg = (TH1D*)fVZReweight->Get("VZReweight_Arg");

    ///////// OBTAIN HISTOGRAMS FOR MULTIPLICITY INFO ////////
    const Int_t nMultBins = 34;
    const Double_t multBins[nMultBins + 1] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
        12, 14, 16, 18, 20, 24, 28, 32, 36, 
        40, 45, 50, 55, 60, 70, 80, 100,
        120, 150, 200, 250, 300, 400, 1000
    };

    TH1D* hDataMult = new TH1D("hDataMult", "Data Multiplicity", nMultBins, multBins);
    TH1D* hMCMult = new TH1D("hMCMult", "HIJING Multiplicity", nMultBins, multBins);
    TH1D* hMCARGMult = new TH1D("hMCARGMult", "Angantyr Multiplicity", nMultBins, multBins);

    // PRINT SELECTION CUTS
    cout << "SELECTION FOR DATA MULT HIST: " << endl;
    cout << Datacut.GetTitle() << endl;
    cout << "SELECTION FOR MC MULT HIST: " << endl;
    cout << MCcut.GetTitle() << endl;

    // OBTAIN DATA HISTOGRAM
    cout << "Creating data histogram from TChain with " << chainData->GetEntries() << " entries..." << endl;
    histfromtree(chainData, Datacut, hDataMult, "multiplicityEta2p4");

    // OBTAIN HIJING HISTOGRAM 
    TTreeFormula* mcCutFormula = new TTreeFormula("mcCutFormula", MCcut, tOO);
    int multiplicityEta2p4_mc;
    float VZ_mc;
    tOO->SetBranchAddress("multiplicityEta2p4", &multiplicityEta2p4_mc);
    tOO->SetBranchAddress("VZ", &VZ_mc);
    for(int i = 0; i < tOO->GetEntries(); i++){
        if(i % 1000 == 0) cout << i*1.0 / tOO->GetEntries() * 100.0 << "% PROCESSED FOR HIJING MULT HIST \r";
        tOO->GetEntry(i);
        if(mcCutFormula->EvalInstance() == 0) continue;
        float vzWeight = VZWeight(vzReweight, VZ_mc);
        hMCMult->Fill(multiplicityEta2p4_mc, fill_value(vzWeight, doreweight));
    }
    hMCMult->Scale(1.0/hMCMult->Integral());
    cout << endl;
    // OBTAIN ANGANTYR HISTOGRAM using cut formula with VZ reweighting
    TTreeFormula* mcArgCutFormula = new TTreeFormula("mcArgCutFormula", MCcut, tOO_Arg);
    int multiplicityEta2p4_arg;
    float VZ_arg;
    tOO_Arg->SetBranchAddress("multiplicityEta2p4", &multiplicityEta2p4_arg);
    tOO_Arg->SetBranchAddress("VZ", &VZ_arg);
    
    for(int i = 0; i < tOO_Arg->GetEntries(); i++){
        if(i % 1000 == 0) cout << i*1.0 / tOO_Arg->GetEntries() * 100.0 << "% PROCESSED FOR ANGANTYR MULT HIST \r";
        tOO_Arg->GetEntry(i);
        if(mcArgCutFormula->EvalInstance() == 0) continue;
        float vzWeight = VZWeight(vzReweight_Arg, VZ_arg);
        hMCARGMult->Fill(multiplicityEta2p4_arg, fill_value(vzWeight, doreweight));
    }
    hMCARGMult->Scale(1.0/hMCARGMult->Integral());
    cout << endl;

    //////// CREATE REWEIGHT FACTORS ////////
    TH1D* multReweight = (TH1D*)hDataMult->Clone("MultReweight");
    multReweight->SetTitle("Multiplicity Reweight Factor (Data/MC HIJING)");
    multReweight->Divide(hMCMult);

    TH1D* multReweight_Arg = (TH1D*)hDataMult->Clone("MultReweight_Arg");
    multReweight_Arg->SetTitle("Multiplicity Reweight Factor (Data/MC Angantyr)");
    multReweight_Arg->Divide(hMCARGMult);

    ///////// DRAW REWEIGHT HISTOGRAM ///////
    gStyle->SetOptStat(0);
    TCanvas* c = new TCanvas("c", "MULTIPLICITY Reweight Factor", 800, 600);
    c->SetLogx();
    
    // Find maximum ratio value for y-axis scaling
    double maxRatio = TMath::Max(multReweight->GetMaximum(), multReweight_Arg->GetMaximum());
    double yMax = maxRatio * 1.5; // Add 50% margin

    multReweight->GetXaxis()->SetTitle("Multiplicity |#eta| < 2.4");
    multReweight->GetYaxis()->SetTitle("Reweight Factor (Data/MC)");
    multReweight->GetXaxis()->SetRangeUser(1.0, 1000);
    multReweight->SetMaximum(yMax);
    multReweight->SetMinimum(0);
    multReweight->SetLineColor(kBlue);
    multReweight->SetLineWidth(2);
    multReweight->Draw("HIST");

    multReweight_Arg->SetLineColor(kOrange+7);
    multReweight_Arg->SetLineWidth(2);
    multReweight_Arg->Draw("HIST SAME");

    TLegend* leg = new TLegend(0.3, 0.7, 0.7, 0.9);
    leg->AddEntry(multReweight, "Data/MC Multiplicity HIJING", "l");
    leg->AddEntry(multReweight_Arg, "Data/MC Multiplicity Angantyr", "l");
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->Draw();
    
    // Add horizontal line at y=1
    TLine* line = new TLine(0.5, 1, 1000, 1);
    line->SetLineColor(kRed);
    line->SetLineStyle(2);
    line->Draw("same");

    ///////// DRAW MULTIPLICITY DISTRIBUTIONS ///////
    TCanvas* c2 = new TCanvas("c2", "Multiplicity Distributions", 800, 600);
    c2->SetLogy();
    c2->SetLogx();
    
    // Create copies for bin width normalization
    TH1D* hDataMult_norm = (TH1D*)hDataMult->Clone("hDataMult_norm");
    TH1D* hMCMult_norm = (TH1D*)hMCMult->Clone("hMCMult_norm");
    TH1D* hMCARGMult_norm = (TH1D*)hMCARGMult->Clone("hMCARGMult_norm");
    
    // Divide by bin width for each histogram
    for(int i = 1; i <= hDataMult_norm->GetNbinsX(); i++){
        double binWidth = hDataMult_norm->GetBinWidth(i);
        
        double dataContent = hDataMult_norm->GetBinContent(i);
        double dataError = hDataMult_norm->GetBinError(i);
        hDataMult_norm->SetBinContent(i, dataContent / binWidth);
        hDataMult_norm->SetBinError(i, dataError / binWidth);
        
        double mcContent = hMCMult_norm->GetBinContent(i);
        double mcError = hMCMult_norm->GetBinError(i);
        hMCMult_norm->SetBinContent(i, mcContent / binWidth);
        hMCMult_norm->SetBinError(i, mcError / binWidth);
        
        double mcArgContent = hMCARGMult_norm->GetBinContent(i);
        double mcArgError = hMCARGMult_norm->GetBinError(i);
        hMCARGMult_norm->SetBinContent(i, mcArgContent / binWidth);
        hMCARGMult_norm->SetBinError(i, mcArgError / binWidth);
    }
    
    // Set colors and styles for the distributions
    hDataMult_norm->SetLineColor(kBlack);
    hDataMult_norm->SetLineWidth(2);
    hDataMult_norm->SetMarkerStyle(20);
    hDataMult_norm->SetMarkerSize(0.8);
    hDataMult_norm->GetXaxis()->SetTitle("Multiplicity |#eta| < 2.4");
    hDataMult_norm->GetYaxis()->SetTitle("dN/dMult (Normalized Events)");
    hDataMult_norm->SetTitle("Multiplicity Distributions");
    
    hMCMult_norm->SetLineColor(kBlue);
    hMCMult_norm->SetLineWidth(2);
    
    hMCARGMult_norm->SetLineColor(kOrange+7);
    hMCARGMult_norm->SetLineWidth(2);
    
    // Find maximum for proper scaling
    double maxVal = hMCMult_norm->GetMaximum();
    hDataMult_norm->SetMaximum(maxVal * 15);
    hDataMult_norm->GetXaxis()->SetRangeUser(1.0,1000);

    hDataMult_norm->Draw("PE");
    hMCMult_norm->Draw("HIST SAME");
    hMCARGMult_norm->Draw("HIST SAME");
    
    TLegend* leg2 = new TLegend(0.65, 0.7, 0.85, 0.9);
    leg2->AddEntry(hDataMult_norm, "Data", "pe");
    leg2->AddEntry(hMCMult_norm, "MC HIJING (VZ reweighted)", "l");
    leg2->AddEntry(hMCARGMult_norm, "MC Angantyr (VZ reweighted)", "l");
    leg2->SetBorderSize(0);
    leg2->SetFillStyle(0);
    leg2->Draw();

    // Save the reweight histogram and canvas
    TFile* fout = TFile::Open("MultReweight.root", "RECREATE");
    multReweight->Write();
    multReweight_Arg->Write();
    hDataMult->Write();
    hMCMult->Write();
    hMCARGMult->Write();
    c->Write();
    c2->Write();
    fout->Close();
    c->SaveAs("MultReweight.pdf");
    c2->SaveAs("MultDistributions.pdf");

    cout << "DONE WITH MULT REWEIGHT" << endl;
}