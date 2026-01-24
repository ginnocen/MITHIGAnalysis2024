void plotDataMC(TH1D* h_data, TH1D* h_mc, TH1D* h_mc2, const char* title, const char* xlabel, const char* ylabel, const char* outname) {
    TCanvas* c = new TCanvas("c","c",800,800);
    c->Divide(1,2);

    h_data->Scale(1.0 / h_data->Integral());
    h_mc->Scale(1.0 / h_mc->Integral());
    h_mc2->Scale(1.0 / h_mc2->Integral());
    
    // Top panel: main plot
    c->cd(1);
    gPad->SetPad(0.0, 0.3, 1.0, 1.0);
    gPad->SetBottomMargin(0.02);
    
    h_data->SetMarkerStyle(20);
    h_data->SetMarkerColor(kBlack);
    h_data->SetLineColor(kBlack);
    h_mc->SetLineColor(kRed);
    h_mc->SetFillColorAlpha(kRed, 0.3);

    h_mc2->SetLineColor(kBlue);
    h_mc2->SetFillColorAlpha(kBlue, 0.3);

    h_data->SetTitle(title);
    h_data->GetXaxis()->SetLabelSize(0);
    h_data->GetYaxis()->SetTitle(ylabel);

    h_data->SetMaximum(1.2 * TMath::Max(h_data->GetMaximum(), TMath::Max(h_mc->GetMaximum(), h_mc2->GetMaximum())));

    h_data->Draw("E");
    h_mc->Draw("HIST SAME");
    h_mc2->Draw("HIST SAME");

    TLegend* legend = new TLegend(0.7,0.7,0.9,0.9);
    legend->AddEntry(h_data, "Data", "lep");
    legend->AddEntry(h_mc, "MC Z weights", "f");
    legend->AddEntry(h_mc2, "MC", "f");
    legend->Draw();
    
    // Bottom panel: ratio
    c->cd(2);
    gPad->SetPad(0.0, 0.0, 1.0, 0.3);
    gPad->SetTopMargin(0.02);
    gPad->SetBottomMargin(0.3);
    
    TH1D* h_ratio = (TH1D*)h_mc->Clone("h_ratio");
    h_ratio->Divide(h_data);
    h_ratio->SetTitle("");
    h_ratio->GetXaxis()->SetTitle(xlabel);
    h_ratio->GetYaxis()->SetTitle("MC/Data");
    h_ratio->GetXaxis()->SetTitleSize(0.12);
    h_ratio->GetXaxis()->SetLabelSize(0.10);
    h_ratio->GetXaxis()->SetTitleOffset(1.0);
    h_ratio->GetYaxis()->SetTitleSize(0.12);
    h_ratio->GetYaxis()->SetLabelSize(0.10);
    h_ratio->GetYaxis()->SetTitleOffset(0.35);
    h_ratio->GetYaxis()->SetNdivisions(505);
    h_ratio->SetMinimum(0.5);
    h_ratio->SetMaximum(1.5);
    h_ratio->SetMarkerStyle(20);
    h_ratio->SetMarkerColor(kBlack);
    h_ratio->SetLineColor(kBlack);
    h_ratio->Draw("E");
    
    TLine* line = new TLine(h_ratio->GetXaxis()->GetXmin(), 1.0, h_ratio->GetXaxis()->GetXmax(), 1.0);
    line->SetLineStyle(2);
    line->SetLineColor(kBlack);
    line->Draw();

    c->SaveAs(outname);
    delete line;
    delete h_ratio;
    delete legend;
    delete c;
}


void datamc_comparison(){

    int chargesel = 0;
    TFile* file_data;
    TFile* file_mc;
    TFile* file_mc2;
    if(chargesel == -1){
        file_data = TFile::Open("../datadistros_opp.root");
        file_mc = TFile::Open("../mcdistros_opp_weighted.root");
        file_mc2 = TFile::Open("../mcdistros_opp.root");
    }
    else if(chargesel == 1){
        file_data = TFile::Open("../datadistros_same.root");
        file_mc = TFile::Open("../mcdistros_same_weighted.root");
        file_mc2 = TFile::Open("../mcdistros_same.root");
    }
    else{
        file_data = TFile::Open("../datadistros.root");
        file_mc = TFile::Open("../mcdistros_weighted.root");
        file_mc2 = TFile::Open("../mcdistros.root");
    }
    vector<double> ptBins = {60,80,100,120,160,200,250,300};


    gStyle->SetOptStat(0);

    for(int i = 0; i < ptBins.size()-1; i++){

        double ptMin = ptBins[i];
        double ptMax = ptBins[i+1];

        TH1D* h_dr_data = ((TH2D*)file_data->Get("hmuDR"))->ProjectionY("h_dr_data", i+1, i+1);
        TH1D* h_dr_mc = ((TH2D*)file_mc->Get("hmuDR"))->ProjectionY("h_dr_mc", i+1, i+1);
        TH1D* h_dr_mc2 = ((TH2D*)file_mc2->Get("hmuDR"))->ProjectionY("h_dr_mc2", i+1, i+1);

        plotDataMC(h_dr_data, h_dr_mc, h_dr_mc2,
                    Form("Dimuon #DeltaR (%.0f < p_{T} < %.0f GeV)", ptMin, ptMax),
                    "DeltaR", "Entries",
                    Form("datamcplots/datamc_dr_pt%.0f_%.0f.pdf", ptMin, ptMax));

        TH1D* h_mumuPt_data = ((TH2D*)file_data->Get("hmumuPt"))->ProjectionY("h_mumuPt_data", i+1, i+1);
        TH1D* h_mumuPt_mc = ((TH2D*)file_mc->Get("hmumuPt"))->ProjectionY("h_mumuPt_mc", i+1, i+1);
        TH1D* h_mumuPt_mc2 = ((TH2D*)file_mc2->Get("hmumuPt"))->ProjectionY("h_mumuPt_mc2", i+1, i+1);

        plotDataMC(h_mumuPt_data, h_mumuPt_mc, h_mumuPt_mc2,
                    Form("Dimuon p_{T} (%.0f < p_{T} < %.0f GeV)", ptMin, ptMax),
                    "p_{T,#mu#mu} [GeV]", "Entries",
                    Form("datamcplots/datamc_mumuPt_pt%.0f_%.0f.pdf", ptMin, ptMax));

        TH1D* h_dca_data = ((TH2D*)file_data->Get("hDCAProductSig"))->ProjectionY("h_dca_data", i+1, i+1);
        TH1D* h_dca_mc = ((TH2D*)file_mc->Get("hDCAProductSig"))->ProjectionY("h_dca_mc", i+1, i+1);
            TH1D* h_dca_mc2 = ((TH2D*)file_mc2->Get("hDCAProductSig"))->ProjectionY("h_dca_mc2", i+1, i+1);

        plotDataMC(h_dca_data, h_dca_mc, h_dca_mc2,
                    Form("DCA Product Significance (%.0f < p_{T} < %.0f GeV)", ptMin, ptMax),
                    "log_{10}(|DCA_{1}#timesDCA_{2}|/#sigma)", "Entries",
                    Form("datamcplots/datamc_dca_pt%.0f_%.0f.pdf", ptMin, ptMax));

        TH1D* h_mass_data = ((TH2D*)file_data->Get("hInvMass"))->ProjectionY("h_mass_data", i+1, i+1);
        TH1D* h_mass_mc = ((TH2D*)file_mc->Get("hInvMass"))->ProjectionY("h_mass_mc", i+1, i+1);
        TH1D* h_mass_mc2 = ((TH2D*)file_mc2->Get("hInvMass"))->ProjectionY("h_mass_mc2", i+1, i+1);

        plotDataMC(h_mass_data, h_mass_mc, h_mass_mc2,
                    Form("Dimuon Mass (%.0f < p_{T} < %.0f GeV)", ptMin, ptMax),
                    "m_{#mu#mu} [GeV]", "Entries",
                    Form("datamcplots/datamc_mass_pt%.0f_%.0f.pdf", ptMin, ptMax));

        TH1D* h_mumuZ_data = ((TH2D*)file_data->Get("hmumuZ"))->ProjectionY("h_mumuZ_data", i+1, i+1);
        TH1D* h_mumuZ_mc = ((TH2D*)file_mc->Get("hmumuZ"))->ProjectionY("h_mumuZ_mc", i+1, i+1);
        TH1D* h_mumuZ_mc2 = ((TH2D*)file_mc2->Get("hmumuZ"))->ProjectionY("h_mumuZ_mc2", i+1, i+1);

        plotDataMC(h_mumuZ_data, h_mumuZ_mc, h_mumuZ_mc2, 
                    Form("Dimuon Rapidity (%.0f < p_{T} < %.0f GeV)", ptMin, ptMax),
                    "y_{#mu#mu}", "Entries",
                    Form("datamcplots/datamc_mumuZ_pt%.0f_%.0f.pdf", ptMin, ptMax));

    }

    cout << "done" << endl;

}