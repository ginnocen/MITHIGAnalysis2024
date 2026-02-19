int getFlavorClass(int NbHad, int NcHad) {
  if (NbHad == 2)
    return 5; // bbbar
  else if (NbHad == 1)
    return 4; // b
  else if ((NcHad == 2) && (NbHad == 0))
    return 3; // ccbar
  else if ((NcHad == 1) && (NbHad == 0))
    return 2; // c
  else if ((NbHad == 0) && (NcHad == 0))
    return 1; // light
  else 
    return 0; // other
}

void flavordistros(){

    gStyle->SetOptStat(0);

    TChain* chain = new TChain("Tree"); // Replace YourTreeName with actual tree name
    chain->Add("/data00/g2ccbar/mc2018/skim_011226_0/mergedfile.root");
    chain->Add("/data00/g2ccbar/mc2018/skim_011226_1/mergedfile.root");
    chain->Add("/data00/g2ccbar/mc2018/skim_011226_2/mergedfile.root");
    chain->Add("/data00/g2ccbar/mc2018/skim_011226_3/mergedfile.root");
    chain->Add("/data00/g2ccbar/mc2018/skim_011226_4/mergedfile.root");
    chain->Add("/data00/g2ccbar/mc2018/skim_011226_5/mergedfile.root");
    chain->Add("/data00/g2ccbar/mc2018/skim_011226_6/mergedfile.root");

    vector<double> ptBins = {60,80,100,120,160,200,250,300};

    TH1D* h_uds = new TH1D("h_uds","",ptBins.size()-1, ptBins.data());
    TH1D* h_other = new TH1D("h_other","",ptBins.size()-1, ptBins.data());
    TH1D* h_c = new TH1D("h_c","",ptBins.size()-1, ptBins.data());
    TH1D* h_cc = new TH1D("h_cc","",ptBins.size()-1, ptBins.data());
    TH1D* h_b = new TH1D("h_b","",ptBins.size()-1, ptBins.data());
    TH1D* h_bb = new TH1D("h_bb","",ptBins.size()-1, ptBins.data());


    TH1D* h_uds_dimuon = new TH1D("h_uds_dimuon","",ptBins.size()-1, ptBins.data());
    TH1D* h_other_dimuon = new TH1D("h_other_dimuon","",ptBins.size()-1, ptBins.data());
    TH1D* h_c_dimuon = new TH1D("h_c_dimuon","",ptBins.size()-1, ptBins.data());
    TH1D* h_cc_dimuon = new TH1D("h_cc_dimuon","",ptBins.size()-1, ptBins.data()); 
    TH1D* h_b_dimuon = new TH1D("h_b_dimuon","",ptBins.size()-1, ptBins.data());
    TH1D* h_bb_dimuon = new TH1D("h_bb_dimuon","",ptBins.size()-1, ptBins.data());

    int nch = 0; int nbh = 0;
    bool isMuMuTagged = false;
    bool isGenMuMuTagged = false;
    int nmu = 0;
    float jetpt = 0.0;

    chain->SetBranchAddress("NcHad", &nch);
    chain->SetBranchAddress("NbHad", &nbh);
    chain->SetBranchAddress("IsMuMuTagged", &isMuMuTagged);
    chain->SetBranchAddress("GenIsMuMuTagged", &isGenMuMuTagged);
    chain->SetBranchAddress("JetPT", &jetpt);

    
    for(int i = 0; i < chain->GetEntries(); i++){
        chain->GetEntry(i);
        
        if(i % 10000 == 0){
            cout << "Processing event " << i << " / " << chain->GetEntries() << " " << i*1.0/chain->GetEntries() << endl;
        }

        int flavor = getFlavorClass(nbh, nch);
        if(flavor == 1) {
            h_uds->Fill(jetpt);
            if(isMuMuTagged) h_uds_dimuon->Fill(jetpt);
        }
        else if(flavor == 0) {
            h_other->Fill(jetpt);
            if(isMuMuTagged) h_other_dimuon->Fill(jetpt);
        }
        else if(flavor == 2) {
            h_c->Fill(jetpt);
            if(isMuMuTagged) h_c_dimuon->Fill(jetpt);
        }
        else if(flavor == 3) {
            h_cc->Fill(jetpt);
            if(isMuMuTagged) h_cc_dimuon->Fill(jetpt);
        }
        else if(flavor == 4) {
            h_b->Fill(jetpt);
            if(isMuMuTagged) h_b_dimuon->Fill(jetpt);
        }
        else if(flavor == 5) {
            h_bb->Fill(jetpt);
            if(isMuMuTagged) h_bb_dimuon->Fill(jetpt);
        }



    }

    for(int i = 0; i < ptBins.size()-1; i++){
        
        h_uds->SetBinContent(i+1, h_uds->GetBinContent(i+1) / (ptBins[i+1] - ptBins[i]));
        h_other->SetBinContent(i+1, h_other->GetBinContent(i+1) / (ptBins[i+1] - ptBins[i]));
        h_c->SetBinContent(i+1, h_c->GetBinContent(i+1) / (ptBins[i+1] - ptBins[i]));
        h_cc->SetBinContent(i+1, h_cc->GetBinContent(i+1) / (ptBins[i+1] - ptBins[i]));
        h_b->SetBinContent(i+1, h_b->GetBinContent(i+1) / (ptBins[i+1] - ptBins[i]));
        h_bb->SetBinContent(i+1, h_bb->GetBinContent(i+1) / (ptBins[i+1] - ptBins[i]));

        h_uds_dimuon->SetBinContent(i+1, h_uds_dimuon->GetBinContent(i+1) / (ptBins[i+1] - ptBins[i]));
        h_other_dimuon->SetBinContent(i+1, h_other_dimuon->GetBinContent(i+1) / (ptBins[i+1] - ptBins[i]));
        h_c_dimuon->SetBinContent(i+1, h_c_dimuon->GetBinContent(i+1) / (ptBins[i+1] - ptBins[i]));
        h_cc_dimuon->SetBinContent(i+1, h_cc_dimuon->GetBinContent(i+1) / (ptBins[i+1] - ptBins[i]));
        h_b_dimuon->SetBinContent(i+1, h_b_dimuon->GetBinContent(i+1) / (ptBins[i+1] - ptBins[i]));
        h_bb_dimuon->SetBinContent(i+1, h_bb_dimuon->GetBinContent(i+1) / (ptBins[i+1] - ptBins[i]));

    }

    TCanvas* c1 = new TCanvas("c1","c1",800,600);
    c1->SetLogy();
    h_uds->SetMinimum(100);
    h_uds->SetLineColor(kBlue);
    h_other->SetLineColor(kRed);
    h_c->SetLineColor(kGreen+2);
    h_cc->SetLineColor(kMagenta);
    h_b->SetLineColor(kOrange);
    h_bb->SetLineColor(kCyan);
    h_uds->SetLineWidth(2);
    h_other->SetLineWidth(2);
    h_c->SetLineWidth(2);
    h_cc->SetLineWidth(2);
    h_b->SetLineWidth(2);
    h_bb->SetLineWidth(2);
    h_uds->SetTitle("Flavor Jet pT Distributions;Jet pT [GeV];Entries");
    h_uds->Draw("HIST");
    h_other->Draw("HIST SAME");
    h_c->Draw("HIST SAME");
    h_cc->Draw("HIST SAME");
    h_b->Draw("HIST SAME");
    h_bb->Draw("HIST SAME");
    TLegend* leg = new TLegend(0.65, 0.65, 0.88, 0.88);
    leg->AddEntry(h_uds, "Light Flavor (uds)", "l");
    leg->AddEntry(h_other, "other", "l");
    leg->AddEntry(h_c, "c", "l");
    leg->AddEntry(h_cc, "cc", "l");
    leg->AddEntry(h_b, "b", "l");
    leg->AddEntry(h_bb, "bb", "l");
    leg->Draw();
    c1->SaveAs("flavordistros_jetpt.pdf");  

    TCanvas* c2 = new TCanvas("c2","c2",800,600);
    c2->SetLogy();
    h_uds_dimuon->SetMaximum(1000);
    h_uds_dimuon->SetLineColor(kBlue);
    h_other_dimuon->SetLineColor(kRed);
    h_c_dimuon->SetLineColor(kGreen+2);
    h_cc_dimuon->SetLineColor(kMagenta);
    h_b_dimuon->SetLineColor(kOrange);
    h_bb_dimuon->SetLineColor(kCyan);
    h_uds_dimuon->SetLineWidth(2);
    h_other_dimuon->SetLineWidth(2);
    h_c_dimuon->SetLineWidth(2);
    h_cc_dimuon->SetLineWidth(2);
    h_b_dimuon->SetLineWidth(2);
    h_bb_dimuon->SetLineWidth(2);
    h_uds_dimuon->SetTitle("Flavor Jet pT Distributions (Dimuon Tagged);Jet pT [GeV];Entries");
    h_uds_dimuon->Draw("HIST");
    h_other_dimuon->Draw("HIST SAME");
    h_c_dimuon->Draw("HIST SAME");
    h_cc_dimuon->Draw("HIST SAME");
    h_b_dimuon->Draw("HIST SAME");
    h_bb_dimuon->Draw("HIST SAME");
    TLegend* leg2 = new TLegend(0.65, 0.65, 0.88, 0.88);
    leg2->AddEntry(h_uds_dimuon, "Light Flavor (uds)", "l");
    leg2->AddEntry(h_other_dimuon, "other", "l");
    leg2->AddEntry(h_c_dimuon, "c", "l");
    leg2->AddEntry(h_cc_dimuon, "cc", "l");
    leg2->AddEntry(h_b_dimuon, "b", "l");
    leg2->AddEntry(h_bb_dimuon, "bb", "l");
    leg2->Draw();
    c2->SaveAs("flavordistros_jetpt_dimuon.pdf");

    TH1D* h_sum = (TH1D*)h_uds->Clone("h_sum");
    h_sum->Add(h_other);
    h_sum->Add(h_c);
    h_sum->Add(h_cc);
    h_sum->Add(h_b);
    h_sum->Add(h_bb);   

    TH1D* h_fraction_uds = (TH1D*)h_uds->Clone("h_fraction_uds");
    TH1D* h_fraction_other = (TH1D*)h_other->Clone("h_fraction_other");
    TH1D* h_fraction_c = (TH1D*)h_c->Clone("h_fraction_c");
    TH1D* h_fraction_cc = (TH1D*)h_cc->Clone("h_fraction_cc");
    TH1D* h_fraction_b = (TH1D*)h_b->Clone("h_fraction_b");
    TH1D* h_fraction_bb = (TH1D*)h_bb->Clone("h_fraction_bb");

    h_fraction_uds->Divide(h_sum);
    h_fraction_other->Divide(h_sum);
    h_fraction_c->Divide(h_sum);
    h_fraction_cc->Divide(h_sum);
    h_fraction_b->Divide(h_sum);
    h_fraction_bb->Divide(h_sum);

    TH1D* h_sum_dimuon = (TH1D*)h_uds_dimuon->Clone("h_sum_dimuon");
    h_sum_dimuon->Add(h_other_dimuon);
    h_sum_dimuon->Add(h_c_dimuon);
    h_sum_dimuon->Add(h_cc_dimuon);
    h_sum_dimuon->Add(h_b_dimuon);
    h_sum_dimuon->Add(h_bb_dimuon);

    TH1D* h_fraction_uds_dimuon = (TH1D*)h_uds_dimuon->Clone("h_fraction_uds");
    TH1D* h_fraction_other_dimuon = (TH1D*)h_other_dimuon->Clone("h_fraction_other");
    TH1D* h_fraction_c_dimuon = (TH1D*)h_c_dimuon->Clone("h_fraction_c");
    TH1D* h_fraction_cc_dimuon = (TH1D*)h_cc_dimuon->Clone("h_fraction_cc");
    TH1D* h_fraction_b_dimuon = (TH1D*)h_b_dimuon->Clone("h_fraction_b");
    TH1D* h_fraction_bb_dimuon = (TH1D*)h_bb_dimuon->Clone("h_fraction_bb");

    h_fraction_uds_dimuon->Divide(h_sum_dimuon);
    h_fraction_other_dimuon->Divide(h_sum_dimuon);
    h_fraction_c_dimuon->Divide(h_sum_dimuon);
    h_fraction_cc_dimuon->Divide(h_sum_dimuon);
    h_fraction_b_dimuon->Divide(h_sum_dimuon);
    h_fraction_bb_dimuon->Divide(h_sum_dimuon);

    TCanvas* c6 = new TCanvas("c6","c6",800,600);
    h_fraction_uds->SetMaximum(0.95);
    h_fraction_uds->SetMinimum(0.85);
    h_fraction_uds->Draw("HIST");
    c6->SaveAs("flavordistros_jetpt_fractions_linear.pdf");
    
    TCanvas* c3 = new TCanvas("c3","c3",800,600);
    h_fraction_uds_dimuon->SetMaximum(1.0);
    h_fraction_uds_dimuon->SetMinimum(0.0);
    h_fraction_uds_dimuon->SetLineColor(kBlue);
    h_fraction_other_dimuon->SetLineColor(kRed);
    h_fraction_c_dimuon->SetLineColor(kGreen+2);
    h_fraction_cc_dimuon->SetLineColor(kMagenta);
    h_fraction_b_dimuon->SetLineColor(kOrange);
    h_fraction_bb_dimuon->SetLineColor(kCyan);
    h_fraction_uds_dimuon->SetLineWidth(2);
    h_fraction_other_dimuon->SetLineWidth(2);
    h_fraction_c_dimuon->SetLineWidth(2);
    h_fraction_cc_dimuon->SetLineWidth(2);
    h_fraction_b_dimuon->SetLineWidth(2);
    h_fraction_bb_dimuon->SetLineWidth(2);
    h_fraction_uds_dimuon->SetTitle("Flavor Jet pT Fractions (Dimuon Tagged);Jet pT [GeV];Fraction");
    h_fraction_uds_dimuon->Draw("HIST");
    h_fraction_other_dimuon->Draw("HIST SAME");
    h_fraction_c_dimuon->Draw("HIST SAME");
    h_fraction_cc_dimuon->Draw("HIST SAME");
    h_fraction_b_dimuon->Draw("HIST SAME");
    h_fraction_bb_dimuon->Draw("HIST SAME");
    TLegend* leg3 = new TLegend(0.65, 0.65, 0.88, 0.88);
    leg3->AddEntry(h_fraction_uds_dimuon, "Light Flavor (uds)", "l");
    leg3->AddEntry(h_fraction_other_dimuon, "other", "l");
    leg3->AddEntry(h_fraction_c_dimuon, "c", "l");
    leg3->AddEntry(h_fraction_cc_dimuon, "cc", "l");
    leg3->AddEntry(h_fraction_b_dimuon, "b", "l");
    leg3->AddEntry(h_fraction_bb_dimuon, "bb", "l");
    leg3->Draw();
    c3->SaveAs("flavordistros_jetpt_dimuon_fractions.pdf");

    TCanvas* c4 = new TCanvas("c4","c4",800,600);
    h_fraction_uds->SetMaximum(0.1);
    h_fraction_uds->SetMinimum(0.0);
    h_fraction_uds->SetLineColor(kBlue);
    h_fraction_other->SetLineColor(kRed);
    h_fraction_c->SetLineColor(kGreen+2);
    h_fraction_cc->SetLineColor(kMagenta);
    h_fraction_b->SetLineColor(kOrange);
    h_fraction_bb->SetLineColor(kCyan);
    h_fraction_uds->SetLineWidth(2);
    h_fraction_other->SetLineWidth(2);
    h_fraction_c->SetLineWidth(2);
    h_fraction_cc->SetLineWidth(2);
    h_fraction_b->SetLineWidth(2);
    h_fraction_bb->SetLineWidth(2);
    h_fraction_uds->SetTitle("Flavor Jet pT Fractions;Jet pT [GeV];Fraction");
    h_fraction_uds->Draw("HIST");
    h_fraction_other->Draw("HIST SAME");
    h_fraction_c->Draw("HIST SAME");
    h_fraction_cc->Draw("HIST SAME");
    h_fraction_b->Draw("HIST SAME");
    h_fraction_bb->Draw("HIST SAME");
    TLegend* leg4 = new TLegend(0.65, 0.65, 0.88, 0.88);
    leg4->AddEntry(h_fraction_uds, "Light Flavor (uds)", "l");
    leg4->AddEntry(h_fraction_other, "other", "l");
    leg4->AddEntry(h_fraction_c, "c", "l");
    leg4->AddEntry(h_fraction_cc, "cc", "l");
    leg4->AddEntry(h_fraction_b, "b", "l");
    leg4->AddEntry(h_fraction_bb, "bb", "l");
    leg4->Draw();
    c4->SaveAs("flavordistros_jetpt_fractions.pdf"); 
    
}