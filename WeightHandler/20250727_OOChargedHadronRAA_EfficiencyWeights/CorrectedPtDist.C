
float getweight(float value, TH1D* hist) {
    int bin = hist->FindBin(value);
    float weight = 1.0/(hist->GetBinContent(bin));
    return weight;
}

void CorrectedPtDist(
                TCut Datacut,
                const char* dataskim = "/data00/bakovacs/OOsamples/Skims/20250705_OO_394153_PhysicsIonPhysics0_074244.root",
                const char* efficiencyfile = "hists/output.root",
                const char* effhistname = "hMult_Eff",
                int useMultiplicity = 1
                ) {
                  
    cout << "STARTING TRACK PT CORRECTION DISTRIBUTIONS" << endl;

    ///// OPEN FILES             
    TFile* fData = TFile::Open(dataskim);
    TTree* TData = (TTree*)fData->Get("Tree");
    TFile* fEff = TFile::Open(efficiencyfile);
    TH1D* hEff = (TH1D*)fEff->Get(effhistname);

    const Int_t nPtBins = 26;
    const Double_t ptBins[nPtBins + 1] = {
        0.5, 0.603, 0.728, 0.879, 1.062, 1.284, 1.553, 1.878, 2.272, 2.749, 
        3.327, 4.027, 4.872, 5.891, 7.117, 8.591, 10.36, 12.48, 
        15.03, 18.08, 21.73, 26.08, 31.28, 37.48, 44.89, 53.73, 64.31
    };

    TH1D* hCorrected = new TH1D("hCorrected", "Corrected p_{T} Distribution", nPtBins, ptBins);
    TH1D* hUncorrected = new TH1D("hUncorrected", "Uncorrected p_{T} Distribution", nPtBins, ptBins);
    int multiplicity = 0;
    float leadingpT = 0;
    vector<float>* trkPt = nullptr;
    TData->SetBranchAddress("trkPt", &trkPt);
    TData->SetBranchAddress("leadingPtEta1p0_sel", &leadingpT);
    TData->SetBranchAddress("multiplicityEta2p4", &multiplicity);

    TTreeFormula* CutFormula = new TTreeFormula("CutFormula", Datacut, TData);
    for(int i = 0; i < TData->GetEntries(); i++) {
        if(i%10000 == 0) {
            cout << "Processing entry " << i << "/" << TData->GetEntries() << endl;
        }
        TData->GetEntry(i);
        if(!CutFormula->EvalInstance()) continue;

        float weight = 1.0;
        if(useMultiplicity) {
            weight = getweight((float)multiplicity, hEff);
        }
        else{
            weight = getweight(leadingpT, hEff);
        }
        
        for(float pt : *trkPt) {
            hUncorrected->Fill(pt, 1);
            hCorrected->Fill(pt, weight);
        }

    }

    TH1D* hRatio = (TH1D*)hCorrected->Clone("hRatio");
    hRatio->Divide(hCorrected, hUncorrected, 1.0, 1.0, "B");

    for (int i = 1; i <= hCorrected->GetNbinsX(); ++i) {
        double binWidth = hCorrected->GetBinWidth(i);
        
        double corrContent = hCorrected->GetBinContent(i);
        double corrError = hCorrected->GetBinError(i);
        hCorrected->SetBinContent(i, corrContent / binWidth);
        hCorrected->SetBinError(i, corrError / binWidth);   
       
        double uncorrContent = hUncorrected->GetBinContent(i);
        double uncorrError = hUncorrected->GetBinError(i);
        hUncorrected->SetBinContent(i, uncorrContent / binWidth);
        hUncorrected->SetBinError(i, uncorrError / binWidth);
    }

    TFile* fEffUpdate = TFile::Open(efficiencyfile, "UPDATE");
    fEffUpdate->cd();
    hCorrected->Write();
    hUncorrected->Write();
    hRatio->Write();
    fEffUpdate->Close();
    
    gStyle->SetOptStat(0);
    TCanvas* c1 = new TCanvas("c1", "Track pT Spectra with Ratio", 800, 800);
    c1->Divide(1, 2);
    
    c1->cd(1);
    gPad->SetPad(0, 0.3, 1, 1);
    gPad->SetLogy();
    gPad->SetLogx();
    gPad->SetBottomMargin(0.02);
    
    hUncorrected->SetLineColor(kRed);
    hUncorrected->SetLineWidth(2);
    hUncorrected->SetTitle("");
    hUncorrected->GetYaxis()->SetTitle("1/N_{ev} dN/dp_{T}");
    hUncorrected->GetXaxis()->SetLabelSize(0);
    
    hCorrected->SetLineColor(kBlue);
    hCorrected->SetLineWidth(2);
    
    hUncorrected->Draw("HIST");
    hCorrected->Draw("HIST SAME");
    
    TLegend* leg = new TLegend(0.5, 0.75, 0.85, 0.9);
    leg->AddEntry(hUncorrected, "Uncorrected", "l");
    leg->AddEntry(hCorrected, "Efficiency Corrected", "l");
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->Draw();
    
    c1->cd(2);
    gPad->SetPad(0, 0, 1, 0.3);
    gPad->SetLogx();
    gPad->SetTopMargin(0.02);
    gPad->SetBottomMargin(0.3);
    
    hRatio->SetLineColor(kBlack);
    hRatio->SetLineWidth(2);
    hRatio->SetMarkerStyle(20);
    hRatio->SetMarkerSize(0.5);
    hRatio->GetYaxis()->SetTitle("Eff Corrected/Uncorrected");
    hRatio->GetYaxis()->SetTitleSize(0.08);
    hRatio->GetYaxis()->SetLabelSize(0.08);
    hRatio->GetYaxis()->SetTitleOffset(0.4);
    hRatio->GetYaxis()->SetNdivisions(505);
    hRatio->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hRatio->GetXaxis()->SetTitleSize(0.08);
    hRatio->GetXaxis()->SetLabelSize(0.08);
    hRatio->GetXaxis()->SetTitleOffset(1.0);
    hRatio->SetMaximum(1.05);
    hRatio->SetMinimum(0.95);

    hRatio->Draw("PE");

    TLine* line = new TLine(hRatio->GetXaxis()->GetXmin(), 1, hRatio->GetXaxis()->GetXmax(), 1);
    line->SetLineColor(kRed);
    line->SetLineStyle(2);
    line->SetLineWidth(2);
    line->Draw("same");
    
    c1->SaveAs("TrackPtSpectra.pdf");

    cout << "COMPLETED TRACK PT CORRECTION DISTRIBUTIONS" << endl;
    cout << endl;
    cout << endl;


}