
void histfromtree(TTree* T, TCut t, TH1D* h, const char* branch){
     T->Project(h->GetName(), branch, t);
     h->Scale(1.0/h->Integral());
}

void VZCheck(){

    const char* f1_path = "/data00/kdeverea/OOsamples/Skims/output_20250730_Skim_OO_IonPhysics0_LowPtV2_250711_104114_MB_CROSSCHECK/merged_0-49.root";
    const char* fhijing_path = "/data00/kdeverea/OOsamples/Skims/output_20250728_Skim_OO_MinBias_OO_5p36TeV_hijing/20250728_Skim_OO_MinBias_OO_5p36TeV_hijing.root";
    const char* farg_path = "/data00/kdeverea/OOsamples/Skims/output_20250724_Skim_MinBias_Pythia_Angantyr_OO_5362GeV/20250724_Skim_MinBias_Pythia_Angantyr_OO_5362GeV.root";

    // Selection cuts (remove VZ cut for reweighting check)
    TCut cut_DATA("(PVFilter == 1) && (ClusterCompatibilityFilter == 1) && (HFEMaxPlus > 13.000000 && HFEMaxMinus > 13.000000)");
    TCut cut_MC("(PVFilter == 1) && (ClusterCompatibilityFilter == 1) && (HFEMaxPlus > 13.000000 && HFEMaxMinus > 13.000000)");

    // Open files
    TFile* f1 = TFile::Open(f1_path);
    TTree* tData = (TTree*)f1->Get("Tree");
    TFile* fhijing = TFile::Open(fhijing_path);
    TTree* tOO = (TTree*)fhijing->Get("Tree");
    TFile* farg = TFile::Open(farg_path);
    TTree* tOO_Arg = (TTree*)farg->Get("Tree");

    // Load the reweight histograms from VZReweight.root
    TFile* freweight = TFile::Open("VZReweight.root");
    TH1D* reweight = (TH1D*)freweight->Get("VZReweight");
    TH1D* reweight_Arg = (TH1D*)freweight->Get("VZReweight_Arg");
    if(!reweight || !reweight_Arg) {
        cerr << "Error: Could not load reweight histograms from VZReweight.root" << endl;
        return;
    }

    cout << "CREATING VZ DISTRIBUTIONS FOR CROSS-CHECK" << endl;

    // Apply same VZ shift as in VZReweight.C
    const double vzShift = +0.366387;
    
    // Create VZ histograms with same binning as reweight histograms
    TH1D* hDataVZ_check = new TH1D("hDataVZ_check", "VZ Distribution Data (shift corrected)", 50, -30, 30);
    histfromtree(tData, cut_DATA, hDataVZ_check, Form("VZ + %f", vzShift));

    TH1D* hMCVZ_check = new TH1D("hMCVZ_check", "VZ Distribution MC HIJING", 50, -30, 30);
    histfromtree(tOO, cut_MC, hMCVZ_check, "VZ");

    TH1D* hMCARGVZ_check = new TH1D("hMCARGVZ_check", "VZ Distribution MC Angantyr", 50, -30, 30);
    histfromtree(tOO_Arg, cut_MC, hMCARGVZ_check, "VZ");

    // Create reweighted MC histograms by looping through events
    TH1D* hMCVZ_reweighted = new TH1D("hMCVZ_reweighted", "VZ Distribution MC HIJING (Reweighted)", 50, -30, 30);
    TH1D* hMCARGVZ_reweighted = new TH1D("hMCARGVZ_reweighted", "VZ Distribution MC Angantyr (Reweighted)", 50, -30, 30);

    cout << "Applying event-by-event reweighting for HIJING MC..." << endl;
    
    // Create TTreeFormula for MC cuts
    TTreeFormula* mcCutFormula = new TTreeFormula("mcCutFormula", 
        "(PVFilter == 1) && (ClusterCompatibilityFilter == 1) && (HFEMaxPlus > 13.000000) && (HFEMaxMinus > 13.000000)", 
        tOO);
    
    // Event-by-event reweighting for HIJING
    float VZ_hijing;
    tOO->SetBranchAddress("VZ", &VZ_hijing);
    
    int nEntries_hijing = tOO->GetEntries();
    for (int i = 0; i < nEntries_hijing; i++) {
        tOO->GetEntry(i);
        
        if (mcCutFormula->EvalInstance()) {
            int bin = reweight->FindBin(VZ_hijing);
            float weight = reweight->GetBinContent(bin);
            hMCVZ_reweighted->Fill(VZ_hijing, weight);
        }
        
        if (i % 100000 == 0) cout << "Processed " << i << " / " << nEntries_hijing << " HIJING events" << endl;
    }
    
    cout << "Applying event-by-event reweighting for Angantyr MC..." << endl;
    
    // Create TTreeFormula for MC cuts (Angantyr)
    TTreeFormula* mcCutFormula_Arg = new TTreeFormula("mcCutFormula_Arg", 
        "(PVFilter == 1) && (ClusterCompatibilityFilter == 1) && (HFEMaxPlus > 13.000000) && (HFEMaxMinus > 13.000000)", 
        tOO_Arg);
    
    // Event-by-event reweighting for Angantyr
    float VZ_arg;
    tOO_Arg->SetBranchAddress("VZ", &VZ_arg);
    
    int nEntries_arg = tOO_Arg->GetEntries();
    for (int i = 0; i < nEntries_arg; i++) {
        tOO_Arg->GetEntry(i);
        
        // Apply cuts using TTreeFormula
        if (mcCutFormula_Arg->EvalInstance()) {
            // Get the reweight factor for this VZ value
            int bin = reweight_Arg->FindBin(VZ_arg);
            float weight = reweight_Arg->GetBinContent(bin);
            
            // Fill the reweighted histogram
            hMCARGVZ_reweighted->Fill(VZ_arg, weight);
        }
        
        if (i % 100000 == 0) cout << "Processed " << i << " / " << nEntries_arg << " Angantyr events" << endl;
    }
    
    // Normalize the reweighted histograms
    hMCVZ_reweighted->Scale(1.0/hMCVZ_reweighted->Integral());
    hMCARGVZ_reweighted->Scale(1.0/hMCARGVZ_reweighted->Integral());

    // Clean up TTreeFormulas
    delete mcCutFormula;
    delete mcCutFormula_Arg;

    // Create ratio histograms to check if reweighting worked
    TH1D* ratio_hijing = (TH1D*)hDataVZ_check->Clone("ratio_hijing");
    ratio_hijing->SetTitle("Data/MC HIJING Ratio After Reweighting");
    ratio_hijing->Divide(hMCVZ_reweighted);

    TH1D* ratio_angantyr = (TH1D*)hDataVZ_check->Clone("ratio_angantyr");
    ratio_angantyr->SetTitle("Data/MC Angantyr Ratio After Reweighting");
    ratio_angantyr->Divide(hMCARGVZ_reweighted);

    ///////// PLOTTING ///////
    gStyle->SetOptStat(0);
    
    // Plot 1: Original vs Reweighted distributions
    TCanvas* c1 = new TCanvas("c1", "VZ Distributions: Before and After Reweighting", 1200, 800);
    c1->Divide(2, 2);
    
    // Top left: Original distributions
    c1->cd(1);
    gPad->SetLogy();
    hDataVZ_check->SetLineColor(kBlack);
    hDataVZ_check->SetLineWidth(2);
    hDataVZ_check->SetMarkerStyle(20);
    hDataVZ_check->GetXaxis()->SetTitle("VZ (cm)");
    hDataVZ_check->GetYaxis()->SetTitle("Normalized Events");
    hDataVZ_check->SetTitle("Original Distributions");
    hDataVZ_check->Draw("PE");
    
    hMCVZ_check->SetLineColor(kBlue);
    hMCVZ_check->SetLineWidth(2);
    hMCVZ_check->Draw("HIST SAME");
    
    hMCARGVZ_check->SetLineColor(kOrange);
    hMCARGVZ_check->SetLineWidth(2);
    hMCARGVZ_check->Draw("HIST SAME");
    
    TLegend* leg1 = new TLegend(0.6, 0.7, 0.9, 0.9);
    leg1->AddEntry(hDataVZ_check, "Data", "pe");
    leg1->AddEntry(hMCVZ_check, "MC HIJING", "l");
    leg1->AddEntry(hMCARGVZ_check, "MC Angantyr", "l");
    leg1->SetBorderSize(0);
    leg1->SetFillStyle(0);
    leg1->Draw();
    
    // Top right: Reweighted distributions
    c1->cd(2);
    gPad->SetLogy();
    hDataVZ_check->Draw("PE");
    
    hMCVZ_reweighted->SetLineColor(kBlue);
    hMCVZ_reweighted->SetLineWidth(2);
    hMCVZ_reweighted->SetLineStyle(2);
    hMCVZ_reweighted->Draw("HIST SAME");
    
    hMCARGVZ_reweighted->SetLineColor(kOrange);
    hMCARGVZ_reweighted->SetLineWidth(2);
    hMCARGVZ_reweighted->SetLineStyle(2);
    hMCARGVZ_reweighted->Draw("HIST SAME");
    
    hDataVZ_check->SetTitle("After Reweighting");
    
    TLegend* leg2 = new TLegend(0.6, 0.7, 0.9, 0.9);
    leg2->AddEntry(hDataVZ_check, "Data", "pe");
    leg2->AddEntry(hMCVZ_reweighted, "MC HIJING (Reweighted)", "l");
    leg2->AddEntry(hMCARGVZ_reweighted, "MC Angantyr (Reweighted)", "l");
    leg2->SetBorderSize(0);
    leg2->SetFillStyle(0);
    leg2->Draw();
    
    // Bottom left: Ratio check for HIJING
    c1->cd(3);
    ratio_hijing->SetLineColor(kBlue);
    ratio_hijing->SetLineWidth(2);
    ratio_hijing->SetMarkerStyle(20);
    ratio_hijing->SetMarkerColor(kBlue);
    ratio_hijing->GetXaxis()->SetTitle("VZ (cm)");
    ratio_hijing->GetYaxis()->SetTitle("Data/MC Ratio");
    ratio_hijing->GetYaxis()->SetRangeUser(0.5, 1.5);
    ratio_hijing->Draw("PE");
    
    TLine* line1 = new TLine(-30, 1, 30, 1);
    line1->SetLineColor(kRed);
    line1->SetLineStyle(2);
    line1->Draw("same");
    
    // Bottom right: Ratio check for Angantyr
    c1->cd(4);
    ratio_angantyr->SetLineColor(kOrange);
    ratio_angantyr->SetLineWidth(2);
    ratio_angantyr->SetMarkerStyle(20);
    ratio_angantyr->SetMarkerColor(kOrange);
    ratio_angantyr->GetXaxis()->SetTitle("VZ (cm)");
    ratio_angantyr->GetYaxis()->SetTitle("Data/MC Ratio");
    ratio_angantyr->GetYaxis()->SetRangeUser(0.5, 1.5);
    ratio_angantyr->Draw("PE");
    
    TLine* line2 = new TLine(-30, 1, 30, 1);
    line2->SetLineColor(kRed);
    line2->SetLineStyle(2);
    line2->Draw("same");

    // Plot 2: Side-by-side ratio comparison
    TCanvas* c2 = new TCanvas("c2", "Data/MC Ratios After Reweighting", 800, 600);
    ratio_hijing->GetYaxis()->SetRangeUser(0.8, 1.2);
    ratio_hijing->Draw("PE");
    
    ratio_angantyr->Draw("PE SAME");
    
    TLegend* leg3 = new TLegend(0.6, 0.7, 0.9, 0.9);
    leg3->AddEntry(ratio_hijing, "Data/MC HIJING", "pe");
    leg3->AddEntry(ratio_angantyr, "Data/MC Angantyr", "pe");
    leg3->SetBorderSize(0);
    leg3->SetFillStyle(0);
    leg3->Draw();
    
    TLine* line3 = new TLine(-30, 1, 30, 1);
    line3->SetLineColor(kRed);
    line3->SetLineStyle(2);
    line3->Draw("same");

    // Save results
    TFile* fout = TFile::Open("VZCheck.root", "RECREATE");
    hDataVZ_check->Write();
    hMCVZ_check->Write();
    hMCARGVZ_check->Write();
    hMCVZ_reweighted->Write();
    hMCARGVZ_reweighted->Write();
    ratio_hijing->Write();
    ratio_angantyr->Write();
    c1->Write();
    c2->Write();
    fout->Close();
    
    c1->SaveAs("VZCheck_Comparison.pdf");
    c2->SaveAs("VZCheck_Ratios.pdf");

    cout << "VZ REWEIGHT CROSS-CHECK COMPLETE!" << endl;
    cout << "Check the ratio plots - they should be close to 1.0 if reweighting worked correctly." << endl;

}