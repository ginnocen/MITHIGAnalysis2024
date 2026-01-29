

void datamc_comparison2(){

    gStyle->SetOptStat(0);


    TFile* f_data = TFile::Open("../datadistros.root");
    TFile* f_mc = TFile::Open("../mcdistros.root");

    TFile* f_mc_same = TFile::Open("../mcdistros_same.root");
    TFile* f_mc_opp = TFile::Open("../mcdistros_opp.root");

    TFile* f_data_same = TFile::Open("../datadistros_same.root");
    TFile* f_data_opp = TFile::Open("../datadistros_opp.root");

    int ptbinlo = 4;
    int ptbinhi = 4;
    const char* title = "DCA product Sig; Log of DCA Product Significance;Entries";
    const char* histogram = "hDCAProductSig";

    TH1D* h_mc_dr_ss = ((TH2D*)f_mc_same->Get(histogram))->ProjectionY("h_mc_dr_ss", ptbinlo, ptbinhi);
    TH1D* h_mc_dr_os = ((TH2D*)f_mc_opp->Get(histogram))->ProjectionY("h_mc_dr_os", ptbinlo, ptbinhi);
    TH1D* h_mc_dr = ((TH2D*)f_mc->Get(histogram))->ProjectionY("h_mc_dr", ptbinlo, ptbinhi);

    TH1D* h_data_dr_ss = ((TH2D*)f_data_same->Get(histogram))->ProjectionY("h_data_dr_ss", ptbinlo, ptbinhi);
    TH1D* h_data_dr_os = ((TH2D*)f_data_opp->Get(histogram))->ProjectionY("h_data_dr_os", ptbinlo, ptbinhi);
    TH1D* h_data_dr = ((TH2D*)f_data->Get(histogram))->ProjectionY("h_data_dr", ptbinlo, ptbinhi);
    
    const char* histogram_other = (Form("%s_%s",histogram,"other"));
    const char* histogram_udsg = (Form("%s_%s",histogram,"uds"));
    const char* histogram_c = (Form("%s_%s",histogram,"c"));
    const char* histogram_b = (Form("%s_%s",histogram,"b"));
    const char* histogram_bb = (Form("%s_%s",histogram,"bb"));
    const char* histogram_cc = (Form("%s_%s",histogram,"cc"));

    TH1D* h_mc_dr_other = ((TH2D*)f_mc->Get(histogram_other))->ProjectionY("h_mc_dr_other", ptbinlo, ptbinhi);
    TH1D* h_mc_dr_udsg = ((TH2D*)f_mc->Get(histogram_udsg))->ProjectionY("h_mc_dr_udsg", ptbinlo, ptbinhi);
    TH1D* h_mc_dr_c = ((TH2D*)f_mc->Get(histogram_c))->ProjectionY("h_mc_dr_c", ptbinlo, ptbinhi);
    TH1D* h_mc_dr_b = ((TH2D*)f_mc->Get(histogram_b))->ProjectionY("h_mc_dr_b", ptbinlo, ptbinhi);  
    TH1D* h_mc_dr_bb = ((TH2D*)f_mc->Get(histogram_bb))->ProjectionY("h_mc_dr_bb", ptbinlo, ptbinhi);
    TH1D* h_mc_dr_cc = ((TH2D*)f_mc->Get(histogram_cc))->ProjectionY("h_mc_dr_cc", ptbinlo, ptbinhi);

    TH1D* h_mc_dr_other_ss = ((TH2D*)f_mc_same->Get(histogram_other))->ProjectionY("h_mc_dr_other_ss", ptbinlo, ptbinhi);
    TH1D* h_mc_dr_udsg_ss = ((TH2D*)f_mc_same->Get(histogram_udsg))->ProjectionY("h_mc_dr_udsg_ss", ptbinlo, ptbinhi);
    TH1D* h_mc_dr_c_ss = ((TH2D*)f_mc_same->Get(histogram_c))->ProjectionY("h_mc_dr_c_ss", ptbinlo, ptbinhi);
    TH1D* h_mc_dr_b_ss = ((TH2D*)f_mc_same->Get(histogram_b))->ProjectionY("h_mc_dr_b_ss", ptbinlo, ptbinhi);
    TH1D* h_mc_dr_bb_ss = ((TH2D*)f_mc_same->Get(histogram_bb))->ProjectionY("h_mc_dr_bb_ss", ptbinlo, ptbinhi);
    TH1D* h_mc_dr_cc_ss = ((TH2D*)f_mc_same->Get(histogram_cc))->ProjectionY("h_mc_dr_cc_ss", ptbinlo, ptbinhi);

    TH1D* h_mc_dr_other_os = ((TH2D*)f_mc_opp->Get(histogram_other))->ProjectionY("h_mc_dr_other_os", ptbinlo, ptbinhi);
    TH1D* h_mc_dr_udsg_os = ((TH2D*)f_mc_opp->Get(histogram_udsg))->ProjectionY("h_mc_dr_udsg_os", ptbinlo, ptbinhi);
    TH1D* h_mc_dr_c_os = ((TH2D*)f_mc_opp->Get(histogram_c))->ProjectionY("h_mc_dr_c_os", ptbinlo, ptbinhi);
    TH1D* h_mc_dr_b_os = ((TH2D*)f_mc_opp->Get(histogram_b))->ProjectionY("h_mc_dr_b_os", ptbinlo, ptbinhi);
    TH1D* h_mc_dr_bb_os = ((TH2D*)f_mc_opp->Get(histogram_bb))->ProjectionY("h_mc_dr_bb_os", ptbinlo, ptbinhi);
    TH1D* h_mc_dr_cc_os = ((TH2D*)f_mc_opp->Get(histogram_cc))->ProjectionY("h_mc_dr_cc_os", ptbinlo, ptbinhi);



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
    //h_try->Draw("HIST SAME");
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
    h_mc_dr_udsg->SetTitle(";DCA Product Significance; Entries");
    h_mc_dr_udsg->SetLineColor(kBlue);
    h_mc_dr_udsg->SetLineWidth(2);
    h_mc_dr_udsg->Draw("HIST E L SAME");
    h_mc_dr_other->SetLineColor(kGray+1);
    h_mc_dr_other->SetLineWidth(2);
    h_mc_dr_other->SetTitle(title);
    h_mc_dr_other->Draw("HIST E L SAME");
    h_mc_dr_c->SetLineColor(kCyan);
    h_mc_dr_c->SetLineWidth(2);
    h_mc_dr_c->Draw("HIST E L SAME");
    h_mc_dr_b->SetLineColor(kGreen+2);
    h_mc_dr_b->SetLineWidth(2);
    h_mc_dr_b->Draw("HIST E L SAME");
    h_mc_dr_bb->SetLineColor(kRed);
    h_mc_dr_bb->SetLineWidth(2);
    h_mc_dr_bb->Draw("HIST E L  SAME");
    h_mc_dr_cc->SetLineColor(kMagenta);
    h_mc_dr_cc->SetLineWidth(2);
    h_mc_dr_cc->Draw("HIST E L SAME");
    TLegend* leg3 = new TLegend(0.65,0.45,0.88,0.88);
    leg3->AddEntry(h_mc_dr_other, "Other", "l");
    leg3->AddEntry(h_mc_dr_udsg, "udsg", "l");
    leg3->AddEntry(h_mc_dr_c, "c", "l");
    leg3->AddEntry(h_mc_dr_b, "b", "l");
    leg3->AddEntry(h_mc_dr_bb, "bb", "l");
    leg3->AddEntry(h_mc_dr_cc, "cc", "l");
    leg3->Draw();
    c3->SaveAs("mc_dr_flavor_comparison.pdf");
    delete c3;
    
    // compare flavor same-sign distributions - MC 
    
    TCanvas* c4 = new TCanvas("flavors_ss", "", 800, 600);
    h_mc_dr_udsg_ss->SetMaximum(1.4*h_mc_dr_bb_ss->GetMaximum());
    h_mc_dr_udsg_ss->SetLineColor(kBlue);
    h_mc_dr_udsg_ss->SetLineWidth(2);
    h_mc_dr_udsg_ss->Draw("HIST SAME");
    h_mc_dr_other_ss->SetLineColor(kGray+1);
    h_mc_dr_other_ss->SetLineWidth(2);
    h_mc_dr_other_ss->SetTitle(title);
    h_mc_dr_other_ss->Draw("HIST SAME");
    h_mc_dr_c_ss->SetLineColor(kCyan);
    h_mc_dr_c_ss->SetLineWidth(2);
    h_mc_dr_c_ss->Draw("HIST SAME");
    h_mc_dr_b_ss->SetLineColor(kGreen+2);
    h_mc_dr_b_ss->SetLineWidth(2);
    h_mc_dr_b_ss->Draw("HIST SAME");
    h_mc_dr_bb_ss->SetLineColor(kRed);
    h_mc_dr_bb_ss->SetLineWidth(2);
    h_mc_dr_bb_ss->Draw("HIST SAME");
    h_mc_dr_cc_ss->SetLineColor(kMagenta);
    h_mc_dr_cc_ss->SetLineWidth(2);
    h_mc_dr_cc_ss->Draw("HIST SAME");
    TLegend* leg4 = new TLegend(0.65,0.45,0.88,0.88);
    leg4->AddEntry(h_mc_dr_other_ss, "MC Other same sign", "l");
    leg4->AddEntry(h_mc_dr_udsg_ss, "MC UDSG same sign", "l");
    leg4->AddEntry(h_mc_dr_c_ss, "MC C same sign", "l");
    leg4->AddEntry(h_mc_dr_b_ss, "MC B same sign", "l");
    leg4->AddEntry(h_mc_dr_bb_ss, "MC BB same sign", "l");
    leg4->AddEntry(h_mc_dr_cc_ss, "MC CC same sign", "l");
    leg4->Draw();
    c4->SaveAs("mc_dr_flavor_ss_comparison.pdf");
    delete c4;

    // compare opposite sign distributions - mc
    TCanvas* c5 = new TCanvas("flavors_os", "", 800, 600);
    h_mc_dr_udsg_os->SetMaximum(1.4*h_mc_dr_b_os->GetMaximum());
    h_mc_dr_udsg_os->SetLineColor(kBlue);
    h_mc_dr_udsg_os->SetLineWidth(2);
    h_mc_dr_udsg_os->Draw("HIST SAME");
    h_mc_dr_other_os->SetLineColor(kGray+1);
    h_mc_dr_other_os->SetLineWidth(2);
    h_mc_dr_other_os->SetTitle(title);
    h_mc_dr_other_os->Draw("HIST SAME");
    h_mc_dr_c_os->SetLineColor(kCyan);
    h_mc_dr_c_os->SetLineWidth(2);
    h_mc_dr_c_os->Draw("HIST SAME");
    h_mc_dr_b_os->SetLineColor(kGreen+2);
    h_mc_dr_b_os->SetLineWidth(2);
    h_mc_dr_b_os->Draw("HIST SAME");
    h_mc_dr_bb_os->SetLineColor(kRed);
    h_mc_dr_bb_os->SetLineWidth(2);
    h_mc_dr_bb_os->Draw("HIST SAME");
    h_mc_dr_cc_os->SetLineColor(kMagenta);
    h_mc_dr_cc_os->SetLineWidth(2);
    h_mc_dr_cc_os->Draw("HIST SAME");
    TLegend* leg5 = new TLegend(0.65,0.45,0.88,0.88);
    leg5->AddEntry(h_mc_dr_other_os, "MC Other opposite sign", "l");
    leg5->AddEntry(h_mc_dr_udsg_os, "MC UDSG opposite sign", "l");
    leg5->AddEntry(h_mc_dr_c_os, "MC C opposite sign", "l");
    leg5->AddEntry(h_mc_dr_b_os, "MC B opposite sign", "l");
    leg5->AddEntry(h_mc_dr_bb_os, "MC BB opposite sign", "l");
    leg5->AddEntry(h_mc_dr_cc_os, "MC CC opposite sign", "l");
    leg5->Draw();
    c5->SaveAs("mc_dr_flavor_os_comparison.pdf");
    delete c5;  

    cout << h_mc_dr_bb->GetEntries() << " " << h_mc_dr_bb->Integral() << endl;

}