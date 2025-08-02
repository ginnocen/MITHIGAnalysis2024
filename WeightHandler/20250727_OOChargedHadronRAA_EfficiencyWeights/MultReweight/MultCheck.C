
void histfromtree(TTree* T, TCut t, TH1D* h, const char* branch){
     T->Project(h->GetName(), branch, t);
     h->Scale(1.0/h->Integral());
}


float Weight(TH1D* hist, float entry){
    int bin = hist->FindBin(entry);
    float weight = hist->GetBinContent(bin);
    return weight;
}

void MultCheck(){

    const char* f1_path = "/data00/kdeverea/OOsamples/Skims/output_20250730_Skim_OO_IonPhysics0_LowPtV2_250711_104114_MB_CROSSCHECK/merged_0-49.root";
    const char* fhijing_path = "/data00/kdeverea/OOsamples/Skims/output_20250728_Skim_OO_MinBias_OO_5p36TeV_hijing/20250728_Skim_OO_MinBias_OO_5p36TeV_hijing.root";
    const char* farg_path = "/data00/kdeverea/OOsamples/Skims/output_20250724_Skim_MinBias_Pythia_Angantyr_OO_5362GeV/20250724_Skim_MinBias_Pythia_Angantyr_OO_5362GeV.root";

    // Selection cuts (remove VZ cut for reweighting check)
    TCut cut_DATA("(VZ > -15 && VZ < 15) && (PVFilter == 1) && (ClusterCompatibilityFilter == 1) && (HFEMaxPlus > 13.000000 && HFEMaxMinus > 13.000000) &&  (HLT_MinimumBiasHF_OR_BptxAND_v1)");
    TCut cut_MC("(VZ > -15 && VZ < 15) && (PVFilter == 1) && (ClusterCompatibilityFilter == 1) && (HFEMaxPlus > 13.000000 && HFEMaxMinus > 13.000000)");

    // Open files
    TFile* f1 = TFile::Open(f1_path);
    TTree* tData = (TTree*)f1->Get("Tree");
    TFile* fhijing = TFile::Open(fhijing_path);
    TTree* tOO = (TTree*)fhijing->Get("Tree");
    TFile* farg = TFile::Open(farg_path);
    TTree* tOO_Arg = (TTree*)farg->Get("Tree");

    // Load the reweight histograms from VZReweight.root
    TFile* freweight = TFile::Open("../VZReweight/VZReweight.root");
    TH1D* VZReweight = (TH1D*)freweight->Get("VZReweight");
    TH1D* VZReweight_Arg = (TH1D*)freweight->Get("VZReweight_Arg");

    TFile* fMultReweight = TFile::Open("MultReweight.root");
    TH1D* multReweight = (TH1D*)fMultReweight->Get("MultReweight");
    TH1D* multReweight_Arg = (TH1D*)fMultReweight->Get("MultReweight_Arg");
    if(!VZReweight || !VZReweight_Arg || !multReweight || !multReweight_Arg) {
        cerr << "Error: Could not load reweight histograms from VZReweight.root or MultReweight.root" << endl;
        return;
    }

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

    histfromtree(tData, cut_DATA, hDataMult, "multiplicityEta2p4");
    histfromtree(tOO, cut_MC, hMCMult, "multiplicityEta2p4");
    histfromtree(tOO_Arg, cut_MC, hMCARGMult, "multiplicityEta2p4");

    TH1D* hMCMultReweighted = new TH1D("hMCMultReweighted", "HIJING Multiplicity (Reweighted)", nMultBins, multBins);
    TH1D* hMCARGMultReweighted = new TH1D("hMCARGMultReweighted", "Angantyr Multiplicity (Reweighted)", nMultBins, multBins);

    cout << "Applying event-by-event reweighting for HIJING MC..." << endl;

    
    TTreeFormula* mcCutFormula = new TTreeFormula("mcCutFormula", 
        "(VZ > -15 && VZ < 15) && (PVFilter == 1) && (ClusterCompatibilityFilter == 1) && (HFEMaxPlus > 13.000000) && (HFEMaxMinus > 13.000000)", 
        tOO);

    int multiplicity_hijing;
    tOO->SetBranchAddress("multiplicityEta2p4", &multiplicity_hijing);
    float vz_hijing;
    tOO->SetBranchAddress("VZ", &vz_hijing);


    for(int i = 0; i < tOO->GetEntries(); i++){
        tOO->GetEntry(i);
        if(mcCutFormula->EvalInstance()){

            float vzWeight = Weight(VZReweight, vz_hijing);
            float multWeight = Weight(multReweight, (float)multiplicity_hijing);
            hMCMultReweighted->Fill(multiplicity_hijing, vzWeight * multWeight);

        }

        if (i % 10000 == 0) {
            cout << "Processed " << i << " /  " << tOO->GetEntries() << " HIJING events" << endl;
        }

    }

    cout << "Applying event-by-event reweighting for Angantyr MC..." << endl;

    TTreeFormula* mcCutFormula_Arg = new TTreeFormula("mcCutFormula_Arg", 
        "(VZ > -15 && VZ < 15) && (PVFilter == 1) && (ClusterCompatibilityFilter == 1) && (HFEMaxPlus > 13.000000) && (HFEMaxMinus > 13.000000)", 
        tOO_Arg);

    int multiplicity_arg;
    tOO_Arg->SetBranchAddress("multiplicityEta2p4", &multiplicity_arg);
    float vz_arg;
    tOO_Arg->SetBranchAddress("VZ", &vz_arg);   

    for(int i = 0; i < tOO_Arg->GetEntries(); i++){
        tOO_Arg->GetEntry(i);
        if(mcCutFormula_Arg->EvalInstance()){

            float vzWeight = Weight(VZReweight_Arg, vz_arg);
            float multWeight = Weight(multReweight_Arg, (float)multiplicity_arg);
            hMCARGMultReweighted->Fill(multiplicity_arg, vzWeight * multWeight);

        }

        if (i % 10000 == 0) {
            cout << "Processed " << i << " /  " << tOO_Arg->GetEntries() << " Angantyr events" << endl;
        }

    }

    hMCMultReweighted->Scale(1.0/hMCMultReweighted->Integral());
    hMCARGMultReweighted->Scale(1.0/hMCARGMultReweighted->Integral());

    // Add small offset to avoid log(0) errors
    for(int i = 1; i <= hDataMult->GetNbinsX(); i++) {
        if(hDataMult->GetBinContent(i) <= 0) hDataMult->SetBinContent(i, 1e-10);
        if(hMCMult->GetBinContent(i) <= 0) hMCMult->SetBinContent(i, 1e-10);
        if(hMCARGMult->GetBinContent(i) <= 0) hMCARGMult->SetBinContent(i, 1e-10);
        if(hMCMultReweighted->GetBinContent(i) <= 0) hMCMultReweighted->SetBinContent(i, 1e-10);
        if(hMCARGMultReweighted->GetBinContent(i) <= 0) hMCARGMultReweighted->SetBinContent(i, 1e-10);
    }

    delete mcCutFormula;
    delete mcCutFormula_Arg;

    // Create ratio histograms to check if reweighting worked
    TH1D* ratio_hijing_mult = (TH1D*)hDataMult->Clone("ratio_hijing_mult");
    ratio_hijing_mult->SetTitle("Data/MC HIJING Multiplicity Ratio After Reweighting");
    ratio_hijing_mult->Divide(hMCMultReweighted);

    TH1D* ratio_angantyr_mult = (TH1D*)hDataMult->Clone("ratio_angantyr_mult");
    ratio_angantyr_mult->SetTitle("Data/MC Angantyr Multiplicity Ratio After Reweighting");
    ratio_angantyr_mult->Divide(hMCARGMultReweighted);

    ///////// PLOTTING ///////
    gStyle->SetOptStat(0);
    
    // Plot 1: Original vs Reweighted multiplicity distributions
    TCanvas* c1 = new TCanvas("c1", "Multiplicity Distributions: Before and After Reweighting", 1200, 800);
    c1->Divide(2, 2);
    
    // Top left: Original distributions
    c1->cd(1);
    gPad->SetLogy();
    gPad->SetLogx();
    hDataMult->SetLineColor(kBlack);
    hDataMult->SetLineWidth(2);
    hDataMult->SetMarkerStyle(20);
    hDataMult->GetXaxis()->SetTitle("Multiplicity");
    hDataMult->GetYaxis()->SetTitle("Normalized Events");
    hDataMult->SetTitle("Original Multiplicity Distributions");
    hDataMult->GetXaxis()->SetRangeUser(1, 1000);
    hDataMult->GetYaxis()->SetRangeUser(1e-6, 1);
    hDataMult->Draw("PE");
    
    hMCMult->SetLineColor(kBlue);
    hMCMult->SetLineWidth(2);
    hMCMult->Draw("HIST SAME");
    
    hMCARGMult->SetLineColor(kOrange);
    hMCARGMult->SetLineWidth(2);
    hMCARGMult->Draw("HIST SAME");
    
    TLegend* leg1 = new TLegend(0.6, 0.7, 0.9, 0.9);
    leg1->AddEntry(hDataMult, "Data", "pe");
    leg1->AddEntry(hMCMult, "MC HIJING", "l");
    leg1->AddEntry(hMCARGMult, "MC Angantyr", "l");
    leg1->SetBorderSize(0);
    leg1->SetFillStyle(0);
    leg1->Draw();
    
    // Top right: Reweighted distributions
    c1->cd(2);
    gPad->SetLogy();
    gPad->SetLogx();
    hDataMult->GetXaxis()->SetRangeUser(1, 1000);
    hDataMult->GetYaxis()->SetRangeUser(1e-6, 1);
    hDataMult->Draw("PE");
    
    hMCMultReweighted->SetLineColor(kBlue);
    hMCMultReweighted->SetLineWidth(2);
    hMCMultReweighted->SetLineStyle(2);
    hMCMultReweighted->Draw("HIST SAME");
    
    hMCARGMultReweighted->SetLineColor(kOrange);
    hMCARGMultReweighted->SetLineWidth(2);
    hMCARGMultReweighted->SetLineStyle(2);
    hMCARGMultReweighted->Draw("HIST SAME");
    
    hDataMult->SetTitle("After VZ+Mult Reweighting");
    
    TLegend* leg2 = new TLegend(0.6, 0.7, 0.9, 0.9);
    leg2->AddEntry(hDataMult, "Data", "pe");
    leg2->AddEntry(hMCMultReweighted, "MC HIJING (Reweighted)", "l");
    leg2->AddEntry(hMCARGMultReweighted, "MC Angantyr (Reweighted)", "l");
    leg2->SetBorderSize(0);
    leg2->SetFillStyle(0);
    leg2->Draw();
    
    // Bottom left: Ratio check for HIJING
    c1->cd(3);
    gPad->SetLogx();
    ratio_hijing_mult->SetLineColor(kBlue);
    ratio_hijing_mult->SetLineWidth(2);
    ratio_hijing_mult->SetMarkerStyle(20);
    ratio_hijing_mult->SetMarkerColor(kBlue);
    ratio_hijing_mult->GetXaxis()->SetTitle("Multiplicity");
    ratio_hijing_mult->GetYaxis()->SetTitle("Data/MC Ratio");
    ratio_hijing_mult->GetYaxis()->SetRangeUser(0.5, 1.5);
    ratio_hijing_mult->GetXaxis()->SetRangeUser(1, 200);
    ratio_hijing_mult->Draw("PE");
    
    TLine* line1 = new TLine(1, 1, 200, 1);
    line1->SetLineColor(kRed);
    line1->SetLineStyle(2);
    line1->Draw("same");
    
    // Bottom right: Ratio check for Angantyr
    c1->cd(4);
    gPad->SetLogx();
    ratio_angantyr_mult->SetLineColor(kOrange);
    ratio_angantyr_mult->SetLineWidth(2);
    ratio_angantyr_mult->SetMarkerStyle(20);
    ratio_angantyr_mult->SetMarkerColor(kOrange);
    ratio_angantyr_mult->GetXaxis()->SetTitle("Multiplicity");
    ratio_angantyr_mult->GetYaxis()->SetTitle("Data/MC Ratio");
    ratio_angantyr_mult->GetYaxis()->SetRangeUser(0.5, 1.5);
    ratio_angantyr_mult->GetXaxis()->SetRangeUser(1, 1000);
    ratio_angantyr_mult->Draw("PE");
    
    TLine* line2 = new TLine(1, 1, 1000, 1);
    line2->SetLineColor(kRed);
    line2->SetLineStyle(2);
    line2->Draw("same");

    // Plot 2: Side-by-side ratio comparison
    TCanvas* c2 = new TCanvas("c2", "Data/MC Multiplicity Ratios After Reweighting", 800, 600);
    gPad->SetLogx();
    ratio_hijing_mult->GetYaxis()->SetRangeUser(0.8, 1.2);
    ratio_hijing_mult->GetXaxis()->SetRangeUser(1, 1000);
    ratio_hijing_mult->Draw("PE");
    
    ratio_angantyr_mult->Draw("PE SAME");
    
    TLegend* leg3 = new TLegend(0.6, 0.7, 0.9, 0.9);
    leg3->AddEntry(ratio_hijing_mult, "Data/MC HIJING", "pe");
    leg3->AddEntry(ratio_angantyr_mult, "Data/MC Angantyr", "pe");
    leg3->SetBorderSize(0);
    leg3->SetFillStyle(0);
    leg3->Draw();
    
    TLine* line3 = new TLine(1, 1, 1000, 1);
    line3->SetLineColor(kRed);
    line3->SetLineStyle(2);
    line3->Draw("same");

    // Plot 3: Linear scale comparison for better visibility of low multiplicity
    TCanvas* c3 = new TCanvas("c3", "Multiplicity Distributions (Linear Scale)", 1200, 600);
    c3->Divide(2, 1);
    
    // Left: Original distributions (linear scale)
    c3->cd(1);
    hDataMult->GetYaxis()->SetRangeUser(0, hDataMult->GetMaximum() * 1.2);
    hDataMult->GetXaxis()->SetRangeUser(0, 1000);
    hDataMult->SetTitle("Original Multiplicity (Linear Scale)");
    hDataMult->Draw("PE");
    hMCMult->Draw("HIST SAME");
    hMCARGMult->Draw("HIST SAME");
    
    TLegend* leg4 = new TLegend(0.6, 0.7, 0.9, 0.9);
    leg4->AddEntry(hDataMult, "Data", "pe");
    leg4->AddEntry(hMCMult, "MC HIJING", "l");
    leg4->AddEntry(hMCARGMult, "MC Angantyr", "l");
    leg4->SetBorderSize(0);
    leg4->SetFillStyle(0);
    leg4->Draw();
    
    // Right: Reweighted distributions (linear scale)
    c3->cd(2);
    hDataMult->GetXaxis()->SetRangeUser(0, 1000);
    hDataMult->SetTitle("After VZ+Mult Reweighting (Linear Scale)");
    hDataMult->Draw("PE");
    hMCMultReweighted->Draw("HIST SAME");
    hMCARGMultReweighted->Draw("HIST SAME");
    
    TLegend* leg5 = new TLegend(0.6, 0.7, 0.9, 0.9);
    leg5->AddEntry(hDataMult, "Data", "pe");
    leg5->AddEntry(hMCMultReweighted, "MC HIJING (Reweighted)", "l");
    leg5->AddEntry(hMCARGMultReweighted, "MC Angantyr (Reweighted)", "l");
    leg5->SetBorderSize(0);
    leg5->SetFillStyle(0);
    leg5->Draw();

    // Save results
    TFile* fout = TFile::Open("MultCheck.root", "RECREATE");
    hDataMult->Write();
    hMCMult->Write();
    hMCARGMult->Write();
    hMCMultReweighted->Write();
    hMCARGMultReweighted->Write();
    ratio_hijing_mult->Write();
    ratio_angantyr_mult->Write();
    c1->Write();
    c2->Write();
    c3->Write();
    fout->Close();
    
    c1->SaveAs("MultCheck_Comparison.pdf");
    c2->SaveAs("MultCheck_Ratios.pdf");
    c3->SaveAs("MultCheck_LinearScale.pdf");

    cout << "MULTIPLICITY REWEIGHT CROSS-CHECK COMPLETE!" << endl;
    cout << "Check the ratio plots - they should be close to 1.0 if reweighting worked correctly." << endl;
    cout << "Note: This check applies BOTH VZ and multiplicity reweighting simultaneously." << endl;

}