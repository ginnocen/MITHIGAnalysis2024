
void prompt_v_nonprompt(){

    TFile*f = TFile::Open("/data00/g2ccbar/data2018/supermerged.root");
    TTree*T = (TTree*)f->Get("Tree");

    float dcaprod = 0;
    float dcaproderr = 0;
    float mass = 0;
    T->SetBranchAddress("muDiDxy1Dxy2",&dcaprod);
    T->SetBranchAddress("muDiDxy1Dxy2Err",&dcaproderr);
    T->SetBranchAddress("mumuMass",&mass);
    bool isMuMuTagged = false;
    T->SetBranchAddress("IsMuMuTagged",&isMuMuTagged);
    bool isTriggered = false;
    T->SetBranchAddress("HLT_HIAK4PFJet80_v1",&isTriggered);

    TH1D*h_all_dca = new TH1D("h_all_dca","h_all_dca",100,-3,4);
    TH1D*h_prompt_dca = new TH1D("h_prompt_dca","h_prompt_dca",100,-3,4);
    
    TH1D* h_all_mass = new TH1D("h_all_mass","h_all_mass",70,0,7);
    TH1D* h_prompt_mass = new TH1D("h_prompt_mass","h_prompt_mass",70,0,7);

    //log10(abs(t->muDiDxy1Dxy2 / t->muDiDxy1Dxy2Err))

    for(int i = 0; i < T->GetEntries(); i++){

        if(i%100000 == 0){
            cout << "On entry " << i << " / " << T->GetEntries() << " " << 1.0*i/T->GetEntries() << endl;
        }
        T->GetEntry(i);
        if(!isTriggered){continue;}
        if(!isMuMuTagged){continue;}
        h_all_dca->Fill(log10(abs(dcaprod / dcaproderr)));
        h_all_mass->Fill(mass);
        if(mass > 3 && mass < 3.2){
            h_prompt_dca->Fill(log10(abs(dcaprod / dcaproderr)));
            h_prompt_mass->Fill(mass);
        }

    }

    TCanvas*c1 = new TCanvas("c1","c1",800,600);
    h_all_dca->SetLineColor(kBlack); 
    h_all_dca->SetLineWidth(2);
    h_all_dca->SetTitle("DCA Product Significance Distribution;log_{10}(|DCA_{1}#timesDCA_{2}|/#sigma);Entries");
    h_all_dca->Draw("HIST"); 
    h_prompt_dca->SetLineColor(kRed); 
    h_prompt_dca->SetLineWidth(2);
    h_prompt_dca->Draw("HIST SAME");    
    TLegend*leg = new TLegend(0.6,0.7,0.9,0.9);
    leg->AddEntry(h_all_dca,"All dimuons","l");
    leg->AddEntry(h_prompt_dca,"J/Psi (3 < m_{#mu#mu} < 3.2)","l");
    leg->Draw();   
    c1->SaveAs("dca_all.pdf");

    TCanvas*c2 = new TCanvas("c2","c2",800,600);
    h_all_mass->SetLineColor(kBlack); 
    h_all_mass->SetLineWidth(2);
    h_all_mass->SetTitle("Dimuon Invariant Mass Distribution;m_{#mu#mu} [GeV];Entries");
    h_prompt_mass->SetLineColor(kRed); 
    h_prompt_mass->SetLineWidth(2);
    h_prompt_mass->SetTitle("Dimuon Invariant Mass Distribution;m_{#mu#mu} [GeV];Entries");
    h_all_mass->Draw("HIST");    
    h_prompt_mass->Draw("HIST SAME"); 
    TLegend*leg2 = new TLegend(0.6,0.7,0.9,0.9);
    leg2->AddEntry(h_all_mass,"All dimuons","l");
    leg2->AddEntry(h_prompt_mass,"J/psi (2.8 < m_{#mu#mu} < 3.4)","l");    
    leg2->Draw();
    c2->SaveAs("mass_all.pdf");

    // new stuff
    TFile*f2 = TFile::Open("/data00/g2ccbar/mc2018/skim_011226_0/mergedfile.root");

    TTree*T2 = (TTree*)f2->Get("Tree");
    bool isMuMuTagged2 = false;
    T2->SetBranchAddress("IsMuMuTagged",&isMuMuTagged2);
    float mass2 = 0;
    T2->SetBranchAddress("mumuMass",&mass2);
    float dcaprod2 = 0;
    float dcaproderr2 = 0;
    T2->SetBranchAddress("muDiDxy1Dxy2",&dcaprod2);
    T2->SetBranchAddress("muDiDxy1Dxy2Err",&dcaproderr2);

    TH1D*h_mass_mc = new TH1D("h_mass_mc","h_mass_mc",70,0,7);
    TH1D*h_dca_mc = new TH1D("h_dca_mc","h_dca_mc",100,-3,4);
    TH1D*h_prompt_dca_mc = new TH1D("h_prompt_dca_mc","h_prompt_dca_mc",100,-3,4);
    TH1D*h_prompt_mass_mc = new TH1D("h_prompt_mass_mc","h_prompt_mass_mc",70,0,7);

    for(int i = 0; i < T2->GetEntries(); i++){

        if(i%100000 == 0){
            cout << "MC On entry " << i << " / " << T2->GetEntries() << " " << 1.0*i/T2->GetEntries() << endl;
        }
        T2->GetEntry(i);
        if(!isMuMuTagged2){continue;}
        h_mass_mc->Fill(mass2);
        h_dca_mc->Fill(log10(abs(dcaprod2 / dcaproderr2)));
        if(mass2 > 3 && mass2 < 3.2){
            h_prompt_dca_mc->Fill(log10(abs(dcaprod2 / dcaproderr2)));
            h_prompt_mass_mc->Fill(mass2);
        }

    }

    TCanvas*c3 = new TCanvas("c3","c3",800,600);
    h_mass_mc->SetLineColor(kBlack); 
    h_mass_mc->SetLineWidth(2);
    h_mass_mc->SetTitle("MC Dimuon Invariant Mass Distribution;m_{#mu#mu} [GeV];Entries");
    h_prompt_mass_mc->SetLineColor(kRed); 
    h_prompt_mass_mc->SetLineWidth(2);
    h_prompt_mass_mc->SetTitle("MC Dimuon Invariant Mass Distribution;m_{#mu#mu} [GeV];Entries");
    h_mass_mc->Draw("HIST");    
    h_prompt_mass_mc->Draw("HIST SAME"); 
    TLegend*leg3 = new TLegend(0.6,0.7,0.9,0.9);
    leg3->AddEntry(h_mass_mc,"All dimuons","l");
    leg3->AddEntry(h_prompt_mass_mc,"J/psi (3 < m_{#mu#mu} < 3.2)","l");    
    leg3->Draw();
    c3->SaveAs("mass_mc.pdf");

    TCanvas*c4 = new TCanvas("c4","c4",800,600);
    h_dca_mc->SetLineColor(kBlack); 
    h_dca_mc->SetLineWidth(2);
    h_dca_mc->SetTitle("MC DCA Product Significance Distribution;log_{10}(|DCA_{1}#timesDCA_{2}|/#sigma);Entries");
    h_dca_mc->Draw("HIST"); 
    h_prompt_dca_mc->SetLineColor(kRed); 
    h_prompt_dca_mc->SetLineWidth(2);
    h_prompt_dca_mc->Draw("HIST SAME");    
    TLegend*leg4 = new TLegend(0.6,0.7,0.9,0.9);
    leg4->AddEntry(h_dca_mc,"All dimuons","l");
    leg4->AddEntry(h_prompt_dca_mc,"J/Psi (3 < m_{#mu#mu} < 3.2)","l"); 
    leg4->Draw();   
    c4->SaveAs("dca_mc.pdf");

    TCanvas*c5 = new TCanvas("c5","c5",800,600);
    h_prompt_dca->Scale(1.0 / h_prompt_dca->Integral());
    h_prompt_dca_mc->Scale(1.0 / h_prompt_dca_mc->Integral());
    h_prompt_dca->SetLineColor(kBlack); 
    h_prompt_dca->SetLineWidth(2);
    h_prompt_dca->SetTitle("Prompt DCA Product Significance Distribution;log_{10}(|DCA_{1}#timesDCA_{2}|/#sigma);Entries"); 
    h_prompt_dca_mc->SetLineColor(kRed); 
    h_prompt_dca_mc->SetLineWidth(2);
    h_prompt_dca_mc->Draw("HIST");
    h_prompt_dca->Draw("HIST SAME"); 
    TLegend*leg5 = new TLegend(0.6,0.7,0.9,0.9);
    leg5->AddEntry(h_prompt_dca,"Data J/Psi (3 < m_{#mu#mu} < 3.2)","l");
    leg5->AddEntry(h_prompt_dca_mc,"MC J/Psi (3 < m_{#mu#mu} < 3.2)","l"); 
    leg5->Draw();   
    c5->SaveAs("dca_prompt_data_vs_mc.pdf");    

    TFile*outf = TFile::Open("prompt_v_nonprompt_output.root","RECREATE");
    h_prompt_dca->Write();
    h_prompt_dca_mc->Write();
    outf->Close();
    



}