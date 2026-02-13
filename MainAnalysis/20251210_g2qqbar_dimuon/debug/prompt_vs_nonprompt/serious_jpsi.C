

void serious_jpsi(){

    TFile*f = TFile::Open("prompt_v_nonprompt_output.root");
    TH1D* h_prompt_dca = (TH1D*)f->Get("h_prompt_dca");
    TH1D* h_prompt_dca_mc = (TH1D*)f->Get("h_prompt_dca_mc");

    h_prompt_dca->Rebin(3);
    h_prompt_dca_mc->Rebin(3);

    // Find scaling factor to match h_prompt_dca in range (0.2, 2.0)
    int bin_low = h_prompt_dca->GetXaxis()->FindBin(0.4);
    int bin_high = h_prompt_dca->GetXaxis()->FindBin(2.0);
    
    double integral_data = h_prompt_dca->Integral(bin_low, bin_high);
    double integral_mc = h_prompt_dca_mc->Integral(bin_low, bin_high);
    
    double scale_factor = integral_data / integral_mc;
    
    std::cout << "Integral of data in range (0.2, 2.0): " << integral_data << std::endl;
    std::cout << "Integral of MC in range (0.2, 2.0): " << integral_mc << std::endl;
    std::cout << "Scale factor: " << scale_factor << std::endl;
    
    // Scale the MC histogram
    h_prompt_dca_mc->Scale(scale_factor);

    TCanvas*c5 = new TCanvas("c5","c5",800,600);
    h_prompt_dca->SetLineColor(kBlack);
    h_prompt_dca->SetLineWidth(2);
    h_prompt_dca->SetMarkerStyle(20);
    h_prompt_dca->SetMarkerColor(kBlack);
    h_prompt_dca->SetTitle("Prompt DCA Comparison;DCA;Entries");
    h_prompt_dca->Draw("PE");
    
    h_prompt_dca_mc->SetLineColor(kRed);
    h_prompt_dca_mc->SetLineWidth(2);
    h_prompt_dca_mc->SetMarkerStyle(20);
    h_prompt_dca_mc->SetMarkerColor(kRed);
    h_prompt_dca_mc->Draw("HIST SAME");
    
    TLegend* leg = new TLegend(0.6, 0.7, 0.85, 0.85);
    leg->AddEntry(h_prompt_dca, "Data", "lp");
    leg->AddEntry(h_prompt_dca_mc, Form("MC (scaled by %.3f)", scale_factor), "l");
    leg->Draw();
    
    c5->SaveAs("prompt_dca_comparison.pdf");

    cout << h_prompt_dca->Integral() - h_prompt_dca_mc->Integral() << endl;




}