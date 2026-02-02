
void fakescheck(){



        gStyle->SetOptStat(0);
    gErrorIgnoreLevel = kWarning;

    TFile* f = TFile::Open("smallskims.root");

    TTree* T_hi = (TTree*)f->Get("Tree_HighEG");
    TTree* T_lo = (TTree*)f->Get("Tree_LowEG");
    TTree* T_mc = (TTree*)f->Get("Tree_MC");

    int jetpthi = 200;
    int jetptlo = 160;

    TH1D* h_fakes_DCA = new TH1D("h_fakes_DCA", "Fakes DCA; log10(abs(muDiDxy1Dxy2 / mu DiDxy1Dxy2Err)) ; Entries", 20, -3, 4);
    TH1D* h_fakes_dR = new TH1D("h_fakes_dR", "Fakes dR; #DeltaR ; Entries", 20, 0, 0.6);
    TH1D* h_fakes_mass = new TH1D("h_fakes_mass", "Fakes Dimuon Mass; M_{#mu#mu} (GeV) ; Entries", 50, 0, 10);
    TH1D* h_fakes_pt = new TH1D("h_fakes_pt", "Fakes Dimuon pT; pT_{#mu#mu} (GeV) ; Entries", 50, 0, 200);
    TH1D* h_fakes_z = new TH1D("h_fakes_z", "Fakes Fragmentation Function; Z ; Entries", 20, 0, 1);

    TH1D* h_all_DCA = new TH1D("h_all_DCA", "All DCA; log10(abs(muDiDxy1Dxy2 / mu DiDxy1Dxy2Err)) ; Entries", 20, -3, 4);
    TH1D* h_all_dR = new TH1D("h_all_dR", "All dR; #DeltaR ; Entries", 20, 0, 0.6);
    TH1D* h_all_mass = new TH1D("h_all_mass", "All Dimuon Mass; M_{#mu#mu} (GeV) ; Entries", 50, 0, 10);
    TH1D* h_all_pt = new  TH1D("h_all_pt", "All Dimuon pT; pT_{#mu#mu} (GeV) ; Entries", 50, 0, 200);
    TH1D* h_all_z = new TH1D("h_all_z", "All Fragmentation Function; Z ; Entries", 20, 0, 1);

    T_mc->Project("h_all_DCA", "log10(abs(muDiDxy1Dxy2 / mu DiDxy1Dxy2Err))", Form("EventWeight * (IsMuMuTagged && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_all_dR", "muDR", Form("EventWeight * (IsMuMuTagged && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_all_mass", "mumuMass", Form("EventWeight * (IsMuMuTagged && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_all_pt", "mumuPt", Form("EventWeight * (IsMuMuTagged && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_all_z", "mumuPt / JetPT", Form("EventWeight * (IsMuMuTagged && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));

    T_mc->Project("h_fakes_DCA", "log10(abs(muDiDxy1Dxy2 / mu DiDxy1Dxy2Err))", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_dR", "muDR", Form("EventWeight * (IsMuMuTagged && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && !GenIsMuMuTagged && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_mass", "mumuMass", Form("EventWeight * (IsMuMuTagged && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && !GenIsMuMuTagged && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_pt", "mumuPt", Form("EventWeight * (IsMuMuTagged && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && !GenIsMuMuTagged && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_z", "mumuPt / JetPT", Form("EventWeight * (IsMuMuTagged && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && !GenIsMuMuTagged && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));

    TH1D* h_fakes_DCA_clone = (TH1D*)h_fakes_DCA->Clone("h_fakes_DCA_clone");
    h_fakes_DCA_clone->Scale(h_all_DCA->Integral() / h_fakes_DCA->Integral());

    TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
    c1->cd();
    h_fakes_DCA_clone->SetLineColor(kRed);
    h_fakes_DCA_clone->SetLineWidth(2);
    h_fakes_DCA_clone->SetLineStyle(2);
    h_fakes_DCA->SetLineColor(kRed);
    h_fakes_DCA->SetLineWidth(2);
    h_all_DCA->SetLineColor(kBlue);
    h_all_DCA->SetLineWidth(2);
    h_all_DCA->SetMaximum(1.5 * TMath::Max(h_fakes_DCA->GetMaximum(), h_all_DCA->GetMaximum()));
    h_all_DCA->Draw("HIST");
    h_fakes_DCA->Draw("HIST SAME");
    h_fakes_DCA_clone->Draw("HIST SAME");

    TLegend* L = new TLegend(0.55,0.65,0.88,0.88);
    L->AddEntry(h_all_DCA, "All", "l");
    L->AddEntry(h_fakes_DCA, "Fakes", "l");
    L->AddEntry(h_fakes_DCA_clone, "Fakes (Scaled)", "l");
    L->Draw();

    c1->SaveAs("fakescheck_DCA.pdf");

    TH1D* h_fakes_DCA_udsg = new TH1D("h_fakes_DCA_udsg", "Fakes DCA udsg; log10(abs(muDiDxy1Dxy2 / mu DiDxy1Dxy2Err)) ; Entries", 20, -3, 4);
    TH1D* h_fakes_DCA_c = new TH1D("h_fakes_DCA_c", "Fakes DCA c; log10(abs(muDiDxy1Dxy2 / mu DiDxy1Dxy2Err)) ; Entries", 20, -3, 4);
    TH1D* h_fakes_DCA_b = new TH1D("h_fakes_DCA_b", "Fakes DCA b; log10(abs(muDiDxy1Dxy2 / mu DiDxy1Dxy2Err)) ; Entries", 20, -3, 4);
    TH1D* h_fakes_DCA_bb = new TH1D("h_fakes_DCA_bb", "Fakes DCA bb; log10(abs(muDiDxy1Dxy2 / mu DiDxy1Dxy2Err)) ; Entries", 20, -3, 4);
    TH1D* h_fakes_DCA_cc = new TH1D("h_fakes_DCA_cc", "Fakes DCA cc; log10(abs(muDiDxy1Dxy2 / mu DiDxy1Dxy2Err)) ; Entries", 20, -3, 4);

    T_mc->Project("h_fakes_DCA_udsg", "log10(abs(muDiDxy1Dxy2 / mu DiDxy1Dxy2Err))", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 0 && NcHad == 0 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_DCA_c", "log10(abs(muDiDxy1Dxy2 / mu DiDxy1Dxy2Err))", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 0 && NcHad == 1 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_DCA_b", "log10(abs(muDiDxy1Dxy2 / mu DiDxy1Dxy2Err))", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 1 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_DCA_bb", "log10(abs(muDiDxy1Dxy2 / mu DiDxy1Dxy2Err))", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 2 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_DCA_cc", "log10(abs(muDiDxy1Dxy2 / mu DiDxy1Dxy2Err))", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 0 && NcHad == 2 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    TCanvas* c2 = new TCanvas("c2", "c2", 800, 600);
    c2->cd();
    h_fakes_DCA_udsg->SetLineColor(kBlue);
    h_fakes_DCA_udsg->SetLineWidth(2);
    h_fakes_DCA_c->SetLineColor(kCyan);
    h_fakes_DCA_c->SetLineWidth(2);
    h_fakes_DCA_b->SetLineColor(kGreen+2);
    h_fakes_DCA_b->SetLineWidth(2);
    h_fakes_DCA_bb->SetLineColor(kMagenta);
    h_fakes_DCA_bb->SetLineWidth(2);
    h_fakes_DCA_cc->SetLineColor(kRed);
    h_fakes_DCA_cc->SetLineWidth(2);
    h_fakes_DCA_udsg->SetMaximum(1.5 * TMath::Max(h_fakes_DCA_udsg->GetMaximum(), TMath::Max(h_fakes_DCA_c->GetMaximum(), TMath::Max(h_fakes_DCA_b->GetMaximum(), TMath::Max(h_fakes_DCA_bb->GetMaximum(), h_fakes_DCA_cc->GetMaximum())))));
    h_fakes_DCA_udsg->Draw("HIST E L");
    h_fakes_DCA_c->Draw("HIST E L SAME");
    h_fakes_DCA_b->Draw("HIST E L SAME");
    h_fakes_DCA_bb->Draw("HIST E L SAME");
    h_fakes_DCA_cc->Draw("HIST E L SAME");
    TLegend* L2 = new TLegend(0.55,0.55,0.88,0.88);
    L2->AddEntry(h_fakes_DCA_udsg, "udsg", "l");
    L2->AddEntry(h_fakes_DCA_c, "c", "l");
    L2->AddEntry(h_fakes_DCA_b, "b", "l");
    L2->AddEntry(h_fakes_DCA_bb, "bb", "l");
    L2->AddEntry(h_fakes_DCA_cc, "cc", "l");
    L2->Draw();

    c2->SaveAs("fakescheck_DCA_flavors.pdf");

    // dR plots
    TH1D* h_fakes_dR_clone = (TH1D*)h_fakes_dR->Clone("h_fakes_dR_clone");
    h_fakes_dR_clone->Scale(h_all_dR->Integral() / h_fakes_dR->Integral());

    TCanvas* c3 = new TCanvas("c3", "c3", 800, 600);
    c3->cd();
    h_fakes_dR_clone->SetLineColor(kRed);
    h_fakes_dR_clone->SetLineWidth(2);
    h_fakes_dR_clone->SetLineStyle(2);
    h_fakes_dR->SetLineColor(kRed);
    h_fakes_dR->SetLineWidth(2);
    h_all_dR->SetLineColor(kBlue);
    h_all_dR->SetLineWidth(2);
    h_all_dR->SetMaximum(1.5 * TMath::Max(h_fakes_dR->GetMaximum(), h_all_dR->GetMaximum()));
    h_all_dR->Draw("HIST");
    h_fakes_dR->Draw("HIST SAME");
    h_fakes_dR_clone->Draw("HIST SAME");

    TLegend* L3 = new TLegend(0.55,0.65,0.88,0.88);
    L3->AddEntry(h_all_dR, "All", "l");
    L3->AddEntry(h_fakes_dR, "Fakes", "l");
    L3->AddEntry(h_fakes_dR_clone, "Fakes (Scaled)", "l");
    L3->Draw();

    c3->SaveAs("fakescheck_dR.pdf");

    // dR flavor breakdown
    TH1D* h_fakes_dR_udsg = new TH1D("h_fakes_dR_udsg", "Fakes dR udsg; #DeltaR ; Entries", 20, 0, 0.6);
    TH1D* h_fakes_dR_c = new TH1D("h_fakes_dR_c", "Fakes dR c; #DeltaR ; Entries", 20, 0, 0.6);
    TH1D* h_fakes_dR_b = new TH1D("h_fakes_dR_b", "Fakes dR b; #DeltaR ; Entries", 20, 0, 0.6);
    TH1D* h_fakes_dR_bb = new TH1D("h_fakes_dR_bb", "Fakes dR bb; #DeltaR ; Entries", 20, 0, 0.6);
    TH1D* h_fakes_dR_cc = new TH1D("h_fakes_dR_cc", "Fakes dR cc; #DeltaR ; Entries", 20, 0, 0.6);

    T_mc->Project("h_fakes_dR_udsg", "muDR", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 0 && NcHad == 0 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_dR_c", "muDR", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 0 && NcHad == 1 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_dR_b", "muDR", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 1 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_dR_bb", "muDR", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 2 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_dR_cc", "muDR", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 0 && NcHad == 2 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));

    TCanvas* c4 = new TCanvas("c4", "c4", 800, 600);
    c4->cd();
    h_fakes_dR_udsg->SetLineColor(kBlue);
    h_fakes_dR_udsg->SetLineWidth(2);
    h_fakes_dR_c->SetLineColor(kCyan);
    h_fakes_dR_c->SetLineWidth(2);
    h_fakes_dR_b->SetLineColor(kGreen+2);
    h_fakes_dR_b->SetLineWidth(2);
    h_fakes_dR_bb->SetLineColor(kMagenta);
    h_fakes_dR_bb->SetLineWidth(2);
    h_fakes_dR_cc->SetLineColor(kRed);
    h_fakes_dR_cc->SetLineWidth(2);
    h_fakes_dR_udsg->SetMaximum(1.5 * TMath::Max(h_fakes_dR_udsg->GetMaximum(), TMath::Max(h_fakes_dR_c->GetMaximum(), TMath::Max(h_fakes_dR_b->GetMaximum(), TMath::Max(h_fakes_dR_bb->GetMaximum(), h_fakes_dR_cc->GetMaximum())))));
    h_fakes_dR_udsg->Draw("HIST E L");
    h_fakes_dR_c->Draw("HIST E L SAME");
    h_fakes_dR_b->Draw("HIST E L SAME");
    h_fakes_dR_bb->Draw("HIST E L SAME");
    h_fakes_dR_cc->Draw("HIST E L SAME");

    TLegend* L4 = new TLegend(0.55,0.55,0.88,0.88);
    L4->AddEntry(h_fakes_dR_udsg, "udsg", "l");
    L4->AddEntry(h_fakes_dR_c, "c", "l");
    L4->AddEntry(h_fakes_dR_b, "b", "l");
    L4->AddEntry(h_fakes_dR_bb, "bb", "l");
    L4->AddEntry(h_fakes_dR_cc, "cc", "l");
    L4->Draw();

    c4->SaveAs("fakescheck_dR_flavors.pdf");

    // Mass plots
    TH1D* h_fakes_mass_clone = (TH1D*)h_fakes_mass->Clone("h_fakes_mass_clone");
    h_fakes_mass_clone->Scale(h_all_mass->Integral() / h_fakes_mass->Integral());

    TCanvas* c5 = new TCanvas("c5", "c5", 800, 600);
    c5->cd();
    h_fakes_mass_clone->SetLineColor(kRed);
    h_fakes_mass_clone->SetLineWidth(2);
    h_fakes_mass_clone->SetLineStyle(2);
    h_fakes_mass->SetLineColor(kRed);
    h_fakes_mass->SetLineWidth(2);
    h_all_mass->SetLineColor(kBlue);
    h_all_mass->SetLineWidth(2);
    h_all_mass->SetMaximum(1.5 * TMath::Max(h_fakes_mass->GetMaximum(), h_all_mass->GetMaximum()));
    h_all_mass->Draw("HIST");
    h_fakes_mass->Draw("HIST SAME");
    h_fakes_mass_clone->Draw("HIST SAME");

    TLegend* L5 = new TLegend(0.55,0.65,0.88,0.88);
    L5->AddEntry(h_all_mass, "All", "l");
    L5->AddEntry(h_fakes_mass, "Fakes", "l");
    L5->AddEntry(h_fakes_mass_clone, "Fakes (Scaled)", "l");
    L5->Draw();

    c5->SaveAs("fakescheck_mass.pdf");

    // Mass flavor breakdown
    TH1D* h_fakes_mass_udsg = new TH1D("h_fakes_mass_udsg", "Fakes Mass udsg; M_{#mu#mu} (GeV) ; Entries", 50, 0, 10);
    TH1D* h_fakes_mass_c = new TH1D("h_fakes_mass_c", "Fakes Mass c; M_{#mu#mu} (GeV) ; Entries", 50, 0, 10);
    TH1D* h_fakes_mass_b = new TH1D("h_fakes_mass_b", "Fakes Mass b; M_{#mu#mu} (GeV) ; Entries", 50, 0, 10);
    TH1D* h_fakes_mass_bb = new TH1D("h_fakes_mass_bb", "Fakes Mass bb; M_{#mu#mu} (GeV) ; Entries", 50, 0, 10);
    TH1D* h_fakes_mass_cc = new TH1D("h_fakes_mass_cc", "Fakes Mass cc; M_{#mu#mu} (GeV) ; Entries", 50, 0, 10);

    T_mc->Project("h_fakes_mass_udsg", "mumuMass", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 0 && NcHad == 0 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_mass_c", "mumuMass", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 0 && NcHad == 1 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_mass_b", "mumuMass", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 1 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_mass_bb", "mumuMass", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 2 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_mass_cc", "mumuMass", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 0 && NcHad == 2 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));

    TCanvas* c6 = new TCanvas("c6", "c6", 800, 600);
    c6->cd();
    h_fakes_mass_udsg->SetLineColor(kBlue);
    h_fakes_mass_udsg->SetLineWidth(2);
    h_fakes_mass_c->SetLineColor(kCyan);
    h_fakes_mass_c->SetLineWidth(2);
    h_fakes_mass_b->SetLineColor(kGreen+2);
    h_fakes_mass_b->SetLineWidth(2);
    h_fakes_mass_bb->SetLineColor(kMagenta);
    h_fakes_mass_bb->SetLineWidth(2);
    h_fakes_mass_cc->SetLineColor(kRed);
    h_fakes_mass_cc->SetLineWidth(2);
    h_fakes_mass_udsg->SetMaximum(1.5 * TMath::Max(h_fakes_mass_udsg->GetMaximum(), TMath::Max(h_fakes_mass_c->GetMaximum(), TMath::Max(h_fakes_mass_b->GetMaximum(), TMath::Max(h_fakes_mass_bb->GetMaximum(), h_fakes_mass_cc->GetMaximum())))));
    h_fakes_mass_udsg->Draw("HIST E L");
    h_fakes_mass_c->Draw("HIST E L SAME");
    h_fakes_mass_b->Draw("HIST E L SAME");
    h_fakes_mass_bb->Draw("HIST E L SAME");
    h_fakes_mass_cc->Draw("HIST E L SAME");

    TLegend* L6 = new TLegend(0.55,0.55,0.88,0.88);
    L6->AddEntry(h_fakes_mass_udsg, "udsg", "l");
    L6->AddEntry(h_fakes_mass_c, "c", "l");
    L6->AddEntry(h_fakes_mass_b, "b", "l");
    L6->AddEntry(h_fakes_mass_bb, "bb", "l");
    L6->AddEntry(h_fakes_mass_cc, "cc", "l");
    L6->Draw();

    c6->SaveAs("fakescheck_mass_flavors.pdf");

    // pT plots
    TH1D* h_fakes_pt_clone = (TH1D*)h_fakes_pt->Clone("h_fakes_pt_clone");
    h_fakes_pt_clone->Scale(h_all_pt->Integral() / h_fakes_pt->Integral());

    TCanvas* c7 = new TCanvas("c7", "c7", 800, 600);
    c7->cd();
    h_fakes_pt_clone->SetLineColor(kRed);
    h_fakes_pt_clone->SetLineWidth(2);
    h_fakes_pt_clone->SetLineStyle(2);
    h_fakes_pt->SetLineColor(kRed);
    h_fakes_pt->SetLineWidth(2);
    h_all_pt->SetLineColor(kBlue);
    h_all_pt->SetLineWidth(2);
    h_all_pt->SetMaximum(1.5 * TMath::Max(h_fakes_pt->GetMaximum(), h_all_pt->GetMaximum()));
    h_all_pt->Draw("HIST");
    h_fakes_pt->Draw("HIST SAME");
    h_fakes_pt_clone->Draw("HIST SAME");

    TLegend* L7 = new TLegend(0.55,0.65,0.88,0.88);
    L7->AddEntry(h_all_pt, "All", "l");
    L7->AddEntry(h_fakes_pt, "Fakes", "l");
    L7->AddEntry(h_fakes_pt_clone, "Fakes (Scaled)", "l");
    L7->Draw();

    c7->SaveAs("fakescheck_pt.pdf");

    // pT flavor breakdown
    TH1D* h_fakes_pt_udsg = new TH1D("h_fakes_pt_udsg", "Fakes pT udsg; pT_{#mu#mu} (GeV) ; Entries", 50, 0, 200);
    TH1D* h_fakes_pt_c = new TH1D("h_fakes_pt_c", "Fakes pT c; pT_{#mu#mu} (GeV) ; Entries", 50, 0, 200);
    TH1D* h_fakes_pt_b = new TH1D("h_fakes_pt_b", "Fakes pT b; pT_{#mu#mu} (GeV) ; Entries", 50, 0, 200);
    TH1D* h_fakes_pt_bb = new TH1D("h_fakes_pt_bb", "Fakes pT bb; pT_{#mu#mu} (GeV) ; Entries", 50, 0, 200);
    TH1D* h_fakes_pt_cc = new TH1D("h_fakes_pt_cc", "Fakes pT cc; pT_{#mu#mu} (GeV) ; Entries", 50, 0, 200);

    T_mc->Project("h_fakes_pt_udsg", "mumuPt", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 0 && NcHad == 0 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_pt_c", "mumuPt", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 0 && NcHad == 1 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_pt_b", "mumuPt", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 1 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_pt_bb", "mumuPt", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 2 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_pt_cc", "mumuPt", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 0 && NcHad == 2 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));

    TCanvas* c8 = new TCanvas("c8", "c8", 800, 600);
    c8->cd();
    h_fakes_pt_udsg->SetLineColor(kBlue);
    h_fakes_pt_udsg->SetLineWidth(2);
    h_fakes_pt_c->SetLineColor(kCyan);
    h_fakes_pt_c->SetLineWidth(2);
    h_fakes_pt_b->SetLineColor(kGreen+2);
    h_fakes_pt_b->SetLineWidth(2);
    h_fakes_pt_bb->SetLineColor(kMagenta);
    h_fakes_pt_bb->SetLineWidth(2);
    h_fakes_pt_cc->SetLineColor(kRed);
    h_fakes_pt_cc->SetLineWidth(2);
    h_fakes_pt_udsg->SetMaximum(1.5 * TMath::Max(h_fakes_pt_udsg->GetMaximum(), TMath::Max(h_fakes_pt_c->GetMaximum(), TMath::Max(h_fakes_pt_b->GetMaximum(), TMath::Max(h_fakes_pt_bb->GetMaximum(), h_fakes_pt_cc->GetMaximum())))));
    h_fakes_pt_udsg->Draw("HIST E L");
    h_fakes_pt_c->Draw("HIST E L SAME");
    h_fakes_pt_b->Draw("HIST E L SAME");
    h_fakes_pt_bb->Draw("HIST E L SAME");
    h_fakes_pt_cc->Draw("HIST E L SAME");

    TLegend* L8 = new TLegend(0.55,0.55,0.88,0.88);
    L8->AddEntry(h_fakes_pt_udsg, "udsg", "l");
    L8->AddEntry(h_fakes_pt_c, "c", "l");
    L8->AddEntry(h_fakes_pt_b, "b", "l");
    L8->AddEntry(h_fakes_pt_bb, "bb", "l");
    L8->AddEntry(h_fakes_pt_cc, "cc", "l");
    L8->Draw();

    c8->SaveAs("fakescheck_pt_flavors.pdf");

    // Z plots
    TH1D* h_fakes_z_clone = (TH1D*)h_fakes_z->Clone("h_fakes_z_clone");
    h_fakes_z_clone->Scale(h_all_z->Integral() / h_fakes_z->Integral());

    TCanvas* c9 = new TCanvas("c9", "c9", 800, 600);
    c9->cd();
    h_fakes_z_clone->SetLineColor(kRed);
    h_fakes_z_clone->SetLineWidth(2);
    h_fakes_z_clone->SetLineStyle(2);
    h_fakes_z->SetLineColor(kRed);
    h_fakes_z->SetLineWidth(2);
    h_all_z->SetLineColor(kBlue);
    h_all_z->SetLineWidth(2);
    h_all_z->SetMaximum(1.5 * TMath::Max(h_fakes_z->GetMaximum(), h_all_z->GetMaximum()));
    h_all_z->Draw("HIST");
    h_fakes_z->Draw("HIST SAME");
    h_fakes_z_clone->Draw("HIST SAME");

    TLegend* L9 = new TLegend(0.55,0.65,0.88,0.88);
    L9->AddEntry(h_all_z, "All", "l");
    L9->AddEntry(h_fakes_z, "Fakes", "l");
    L9->AddEntry(h_fakes_z_clone, "Fakes (Scaled)", "l");
    L9->Draw();

    c9->SaveAs("fakescheck_z.pdf");

    // Z flavor breakdown
    TH1D* h_fakes_z_udsg = new TH1D("h_fakes_z_udsg", "Fakes Z udsg; Z ; Entries", 20, 0, 1);
    TH1D* h_fakes_z_c = new TH1D("h_fakes_z_c", "Fakes Z c; Z ; Entries", 20, 0, 1);
    TH1D* h_fakes_z_b = new TH1D("h_fakes_z_b", "Fakes Z b; Z ; Entries", 20, 0, 1);
    TH1D* h_fakes_z_bb = new TH1D("h_fakes_z_bb", "Fakes Z bb; Z ; Entries", 20, 0, 1);
    TH1D* h_fakes_z_cc = new TH1D("h_fakes_z_cc", "Fakes Z cc; Z ; Entries", 20, 0, 1);

    T_mc->Project("h_fakes_z_udsg", "mumuPt / JetPT", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 0 && NcHad == 0 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_z_c", "mumuPt / JetPT", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 0 && NcHad == 1 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_z_b", "mumuPt / JetPT", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 1 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_z_bb", "mumuPt / JetPT", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 2 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));
    T_mc->Project("h_fakes_z_cc", "mumuPt / JetPT", Form("EventWeight * (IsMuMuTagged && !GenIsMuMuTagged && NbHad == 0 && NcHad == 2 && muPt1 > 3.5 && muPt2 > 3.5 && JetPT > %d && JetPT < %d && (muCharge1 * muCharge2 == 1))", jetptlo, jetpthi));

    TCanvas* c10 = new TCanvas("c10", "c10", 800, 600);
    c10->cd();
    h_fakes_z_udsg->SetLineColor(kBlue);
    h_fakes_z_udsg->SetLineWidth(2);
    h_fakes_z_c->SetLineColor(kCyan);
    h_fakes_z_c->SetLineWidth(2);
    h_fakes_z_b->SetLineColor(kGreen+2);
    h_fakes_z_b->SetLineWidth(2);
    h_fakes_z_bb->SetLineColor(kMagenta);
    h_fakes_z_bb->SetLineWidth(2);
    h_fakes_z_cc->SetLineColor(kRed);
    h_fakes_z_cc->SetLineWidth(2);
    h_fakes_z_udsg->SetMaximum(1.5 * TMath::Max(h_fakes_z_udsg->GetMaximum(), TMath::Max(h_fakes_z_c->GetMaximum(), TMath::Max(h_fakes_z_b->GetMaximum(), TMath::Max(h_fakes_z_bb->GetMaximum(), h_fakes_z_cc->GetMaximum())))));
    h_fakes_z_udsg->Draw("HIST E L");
    h_fakes_z_c->Draw("HIST E L SAME");
    h_fakes_z_b->Draw("HIST E L SAME");
    h_fakes_z_bb->Draw("HIST E L SAME");
    h_fakes_z_cc->Draw("HIST E L SAME");

    TLegend* L10 = new TLegend(0.55,0.55,0.88,0.88);
    L10->AddEntry(h_fakes_z_udsg, "udsg", "l");
    L10->AddEntry(h_fakes_z_c, "c", "l");
    L10->AddEntry(h_fakes_z_b, "b", "l");
    L10->AddEntry(h_fakes_z_bb, "bb", "l");
    L10->AddEntry(h_fakes_z_cc, "cc", "l");
    L10->Draw();

    c10->SaveAs("fakescheck_z_flavors.pdf");

}