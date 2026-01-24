

void datamc_comparison2(){

    gStyle->SetOptStat(0);


    TFile* f_data = TFile::Open("../datadistros.root");
    TFile* f_mc = TFile::Open("../mcdistros.root");

    TFile* f_mc_same = TFile::Open("../mcdistros_same.root");
    TFile* f_mc_opp = TFile::Open("../mcdistros_opp.root");

    TFile* f_data_same = TFile::Open("../datadistros_same.root");
    TFile* f_data_opp = TFile::Open("../datadistros_opp.root");

    int ptbin = 3;
    const char* title = "Dimuon Mass ;m_{#mu#mu} [GeV];Entries";
    const char* histogram = "hInvMass";

    TH1D* h_mc_dr_ss = ((TH2D*)f_mc_same->Get(histogram))->ProjectionY("h_mc_dr_ss", ptbin, ptbin);
    TH1D* h_mc_dr_os = ((TH2D*)f_mc_opp->Get(histogram))->ProjectionY("h_mc_dr_os", ptbin, ptbin);
    TH1D* h_mc_dr = ((TH2D*)f_mc->Get(histogram))->ProjectionY("h_mc_dr", ptbin, ptbin);

    TH1D* h_data_dr_ss = ((TH2D*)f_data_same->Get(histogram))->ProjectionY("h_data_dr_ss", ptbin, ptbin);
    TH1D* h_data_dr_os = ((TH2D*)f_data_opp->Get(histogram))->ProjectionY("h_data_dr_os", ptbin, ptbin);
    TH1D* h_data_dr = ((TH2D*)f_data->Get(histogram))->ProjectionY("h_data_dr", ptbin, ptbin);
    
    const char* histogram_other = (Form("%s_%s",histogram,"other"));
    const char* histogram_udsg = (Form("%s_%s",histogram,"uds"));
    const char* histogram_c = (Form("%s_%s",histogram,"c"));
    const char* histogram_b = (Form("%s_%s",histogram,"b"));
    const char* histogram_bb = (Form("%s_%s",histogram,"bb"));
    const char* histogram_cc = (Form("%s_%s",histogram,"cc"));

    TH1D* h_mc_dr_other = ((TH2D*)f_mc->Get(histogram_other))->ProjectionY("h_mc_dr_other", ptbin, ptbin);
    TH1D* h_mc_dr_udsg = ((TH2D*)f_mc->Get(histogram_udsg))->ProjectionY("h_mc_dr_udsg", ptbin, ptbin);
    TH1D* h_mc_dr_c = ((TH2D*)f_mc->Get(histogram_c))->ProjectionY("h_mc_dr_c", ptbin, ptbin);
    TH1D* h_mc_dr_b = ((TH2D*)f_mc->Get(histogram_b))->ProjectionY("h_mc_dr_b", ptbin, ptbin);  
    TH1D* h_mc_dr_bb = ((TH2D*)f_mc->Get(histogram_bb))->ProjectionY("h_mc_dr_bb", ptbin, ptbin);
    TH1D* h_mc_dr_cc = ((TH2D*)f_mc->Get(histogram_cc))->ProjectionY("h_mc_dr_cc", ptbin, ptbin);

    TH1D* h_try = (TH1D*)h_mc_dr_other->Clone("h_try");
    h_try->Add(h_mc_dr_udsg);
    h_try->Add(h_mc_dr_c);
    h_try->Add(h_mc_dr_b);
    h_try->Add(h_mc_dr_bb);
    h_try->Add(h_mc_dr_cc);


    // COMPARE SAME SIGN OPPOSITE SIGN DISTRIBUTIONS 


    //h_data_dr_ss->Rebin(2);
    //h_data_dr_os->Rebin(2);
    //h_data_dr->Rebin(2);
    h_data_dr_ss->Scale(1.0/h_data_dr_ss->Integral());
    h_data_dr_os->Scale(1.0/h_data_dr_os->Integral());
    h_data_dr->Scale(1.0/h_data_dr->Integral());
    h_try->Scale(1.0/h_try->Integral());

    TCanvas* c = new TCanvas("c_dr_ss_os_comparison", "", 800, 600);
    h_data_dr_ss->SetLineColor(kRed);
    h_data_dr_ss->SetLineWidth(2);
    h_data_dr_ss->SetTitle(title);
    h_data_dr_ss->Draw("HIST");
    h_data_dr_os->SetLineColor(kBlue);
    h_data_dr_os->SetLineWidth(2);
    h_data_dr_os->Draw("HIST SAME");
    h_data_dr->SetLineColor(kBlack);
    h_data_dr->SetLineWidth(2);
    h_data_dr->Draw("HIST SAME");
    h_try->SetLineColor(kGreen+2);
    h_try->SetLineWidth(2);
    h_try->Draw("HIST SAME");
    TLegend* leg = new TLegend(0.65,0.65,0.88,0.88);
    leg->AddEntry(h_data_dr_ss, "Data Same Sign", "l");
    leg->AddEntry(h_data_dr_os, "Data Opposite Sign", "l");
    leg->AddEntry(h_data_dr, "Data Inclusive", "l");
    leg->Draw();
    c->SaveAs("data_dr_ss_os_comparison.pdf");
    delete c;    

    // COMPaRE SAME SIDE OPPOSITE SIGN DISTRIBUTIONS - MC 

    
    //h_mc_dr_ss->Rebin(2);
    //h_mc_dr_os->Rebin(2);
    //h_mc_dr->Rebin(2);
    h_mc_dr_ss->Scale(1.0/h_mc_dr_ss->Integral());
    h_mc_dr_os->Scale(1.0/h_mc_dr_os->Integral());
    h_mc_dr->Scale(1.0/h_mc_dr->Integral());

    TCanvas* c2 = new TCanvas("c_dr_ss_os_comparison_mc", "", 800, 600);
    h_mc_dr_ss->SetLineColor(kRed);
    h_mc_dr_ss->SetLineWidth(2);
    h_mc_dr_ss->SetTitle(title);
    h_mc_dr_ss->Draw("HIST");
    h_mc_dr_os->SetLineColor(kBlue);
    h_mc_dr_os->SetLineWidth(2);
    h_mc_dr_os->Draw("HIST SAME");
    h_mc_dr->SetLineColor(kBlack);
    h_mc_dr->SetLineWidth(2);
    h_mc_dr->Draw("HIST SAME");
    TLegend* leg2 = new TLegend(0.65,0.65,0.88,0.88);
    leg2->AddEntry(h_mc_dr_ss, "MC Same Sign", "l");
    leg2->AddEntry(h_mc_dr_os, "MC Opposite Sign", "l");
    leg2->AddEntry(h_mc_dr, "MC Inclusive", "l");
    leg2->Draw();
    c2->SaveAs("mc_dr_ss_os_comparison.pdf");
    delete c2;

    // COMPARE FLAVOR CLASS DISTRIBUTIONS - MC
    
    TCanvas* c3 = new TCanvas("flavors", "", 800, 600);
    h_mc_dr_udsg->SetMaximum(1.4*h_mc_dr_b->GetMaximum());
    h_mc_dr_udsg->SetLineColor(kBlue);
    h_mc_dr_udsg->SetLineWidth(2);
    h_mc_dr_udsg->Draw("HIST SAME");
    h_mc_dr_other->SetLineColor(kGray+1);
    h_mc_dr_other->SetLineWidth(2);
    h_mc_dr_other->SetTitle(title);
    h_mc_dr_other->Draw("HIST SAME");
    h_mc_dr_c->SetLineColor(kCyan);
    h_mc_dr_c->SetLineWidth(2);
    h_mc_dr_c->Draw("HIST SAME");
    h_mc_dr_b->SetLineColor(kGreen+2);
    h_mc_dr_b->SetLineWidth(2);
    h_mc_dr_b->Draw("HIST SAME");
    h_mc_dr_bb->SetLineColor(kRed);
    h_mc_dr_bb->SetLineWidth(2);
    h_mc_dr_bb->Draw("HIST SAME");
    h_mc_dr_cc->SetLineColor(kMagenta);
    h_mc_dr_cc->SetLineWidth(2);
    h_mc_dr_cc->Draw("HIST SAME");
    TLegend* leg3 = new TLegend(0.65,0.45,0.88,0.88);
    leg3->AddEntry(h_mc_dr_other, "MC Other", "l");
    leg3->AddEntry(h_mc_dr_udsg, "MC UDSG", "l");
    leg3->AddEntry(h_mc_dr_c, "MC C", "l");
    leg3->AddEntry(h_mc_dr_b, "MC B", "l");
    leg3->AddEntry(h_mc_dr_bb, "MC BB", "l");
    leg3->AddEntry(h_mc_dr_cc, "MC CC", "l");
    leg3->Draw();
    c3->SaveAs("mc_dr_flavor_comparison.pdf");
    delete c3;
    
}