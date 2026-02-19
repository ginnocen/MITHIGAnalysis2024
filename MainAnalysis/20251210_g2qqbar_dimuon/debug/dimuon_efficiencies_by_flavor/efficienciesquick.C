
void efficienciesquick(){

    TFile* f1 = TFile::Open("../miniskims/pythiamini_softmvaID.root");
    TFile* f2 = TFile::Open("../miniskims/pythiamini_hybridsoft.root");

    TTree* T_mc = (TTree*)f1->Get("Tree");
    TTree* T_mc_hybrid = (TTree*)f2->Get("Tree");


    vector<float> ptbins = {60,80,100,120,160,200,250,300,400,500};

    TH2D* hAccepted = new TH2D("hAccepted","hAccepted;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    hAccepted->GetXaxis()->Set(ptbins.size()-1, ptbins.data());

    TH2D* hEverything = new TH2D("hEverything","hEverything;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    hEverything->GetXaxis()->Set(ptbins.size()-1, ptbins.data());

    TH2D* hFakes = new TH2D("hFakes","hFakes;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    hFakes->GetXaxis()->Set(ptbins.size()-1, ptbins.data());

    
    ////////


    TH2D* hAccepted_udsg = new TH2D("hAccepted_udsg","hAccepted_udsg;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    TH2D* hAccepted_other = new TH2D("hAccepted_other","hAccepted_other;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    TH2D* hAccepted_c = new TH2D("hAccepted_c","hAccepted_c;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    TH2D* hAccepted_cc = new TH2D("hAccepted_cc","hAccepted_cc;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    TH2D* hAccepted_b = new TH2D("hAccepted_b","hAccepted_b;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    TH2D* hAccepted_bb = new TH2D("hAccepted_bb","hAccepted_bb;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    hAccepted_udsg->GetXaxis()->Set(ptbins.size()-1, ptbins.data());
    hAccepted_other->GetXaxis()->Set(ptbins.size()-1, ptbins.data());
    hAccepted_c->GetXaxis()->Set(ptbins.size()-1, ptbins.data());
    hAccepted_cc->GetXaxis()->Set(ptbins.size()-1, ptbins.data());
    hAccepted_b->GetXaxis()->Set(ptbins.size()-1, ptbins.data());
    hAccepted_bb->GetXaxis()->Set(ptbins.size()-1, ptbins.data());

    TH2D* hEverything_udsg = new TH2D("hEverything_udsg","hEverything_udsg;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    TH2D* hEverything_other = new TH2D("hEverything_other","hEverything_other;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    TH2D* hEverything_c = new TH2D("hEverything_c","hEverything_c;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    TH2D* hEverything_cc = new TH2D("hEverything_cc","hEverything_cc;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    TH2D* hEverything_b = new TH2D("hEverything_b","hEverything_b;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    TH2D* hEverything_bb = new TH2D("hEverything_bb","hEverything_bb;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    hEverything_udsg->GetXaxis()->Set(ptbins.size()-1, ptbins.data());
    hEverything_other->GetXaxis()->Set(ptbins.size()-1, ptbins.data());
    hEverything_c->GetXaxis()->Set(ptbins.size()-1, ptbins.data());
    hEverything_cc->GetXaxis()->Set(ptbins.size()-1, ptbins.data());
    hEverything_b->GetXaxis()->Set(ptbins.size()-1, ptbins.data());
    hEverything_bb->GetXaxis()->Set(ptbins.size()-1, ptbins.data());

    TH2D* hFakes_udsg = new TH2D("hFakes_udsg","hFakes_udsg;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    TH2D* hFakes_other = new TH2D("hFakes_other","hFakes_other;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    TH2D* hFakes_c = new TH2D("hFakes_c","hFakes_c;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    TH2D* hFakes_cc = new TH2D("hFakes_cc","hFakes_cc;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    TH2D* hFakes_b = new TH2D("hFakes_b","hFakes_b;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    TH2D* hFakes_bb = new TH2D("hFakes_bb","hFakes_bb;Jet pT;Jet Eta;Efficiency", ptbins.size()-1, ptbins.front(), ptbins.back(), 10, -2.0, 2.0);
    hFakes_udsg->GetXaxis()->Set(ptbins.size()-1, ptbins.data());
    hFakes_other->GetXaxis()->Set(ptbins.size()-1, ptbins.data());
    hFakes_c->GetXaxis()->Set(ptbins.size()-1, ptbins.data());
    hFakes_cc->GetXaxis()->Set(ptbins.size()-1, ptbins.data());
    hFakes_b->GetXaxis()->Set(ptbins.size()-1, ptbins.data());
    hFakes_bb->GetXaxis()->Set(ptbins.size()-1, ptbins.data());
    hFakes_bb->SetLineColor(kRed);
    //////// 

    float mupt1 = 4.0;
    float mupt2 = 4.0;
    int chargesel = 1;

    // Loop over both trees
    vector<TTree*> trees = {T_mc, T_mc_hybrid};
    vector<string> treeNames = {"softmvaID", "hybridsoft"};
    
    for(int iTree = 0; iTree < trees.size(); iTree++){
        TTree* currentTree = trees[iTree];
        string treeName = treeNames[iTree];
        
        // Store graphs for comparison
        vector<TGraphAsymmErrors*> efficiencyGraphs;
        vector<TGraphAsymmErrors*> purityGraphs;
        vector<string> selectionLabels;
        vector<int> colors = {kBlue, kRed, kGreen+2, kMagenta+2, kOrange+2, kCyan+2};
        vector<int> markers = {20, 21, 22, 23, 24, 25};
        
        // Loop over different selections
        vector<string> selectionTypes = {"Inclusive", "NbHad==2"};
        
        for(int iSel = 0; iSel < selectionTypes.size(); iSel++){
            string selType = selectionTypes[iSel];
            
            string flavorCut = "";
            if(selType == "NbHad==2") flavorCut = " && NbHad == 2";
            
            string Sel = Form(" && muPt1 > %f && muPt2 > %f && (muCharge1 * muCharge2 == %d || %d == 0)", mupt1, mupt2, chargesel, chargesel) + flavorCut;
            string GenSel = Form("&& (GenMuCharge1 * GenMuCharge2 == %d || %d == 0)", chargesel, chargesel) + flavorCut;

            string sFake = Form("IsMuMuTagged && !GenIsMuMuTagged ") + Sel;
            string sAcceptable = Form("IsMuMuTagged && GenIsMuMuTagged ") + Sel;
            string sEverything = Form("GenIsMuMuTagged") + GenSel;
        
            // Reset histograms
            hAccepted->Reset();
            hFakes->Reset();
            hEverything->Reset();
        
            currentTree->Project("hAccepted","JetEta:JetPT",sAcceptable.c_str());
            currentTree->Project("hFakes","JetEta:JetPT",sFake.c_str());
            currentTree->Project("hEverything","JetEta:JetPT",sEverything.c_str());
    
            ////////////

            // Project onto x-axis (pT)
            TH1D* hAccepted_pT = hAccepted->ProjectionX(Form("hAccepted_pT_%s_%s", treeName.c_str(), selType.c_str()));
            TH1D* hFakes_pT = hFakes->ProjectionX(Form("hFakes_pT_%s_%s", treeName.c_str(), selType.c_str()));
            TH1D* hEverything_pT = hEverything->ProjectionX(Form("hEverything_pT_%s_%s", treeName.c_str(), selType.c_str()));

            // Calculate efficiency with proper binomial errors
            TGraphAsymmErrors* gEfficiency = new TGraphAsymmErrors();
            gEfficiency->Divide(hAccepted_pT, hEverything_pT, "cl=0.683 b(1,1) mode");
            gEfficiency->SetTitle("Efficiency vs Jet pT;Jet pT [GeV];Efficiency");
            gEfficiency->SetName(Form("gEfficiency_%s_%s", treeName.c_str(), selType.c_str()));
            gEfficiency->SetLineColor(colors[iSel]);
            gEfficiency->SetLineWidth(2);
            gEfficiency->SetMarkerStyle(markers[iSel]);
            gEfficiency->SetMarkerColor(colors[iSel]);
            gEfficiency->SetMarkerSize(1.2);

            // Calculate purity with proper binomial errors
            TH1D* hTagged_pT = (TH1D*)hAccepted_pT->Clone(Form("hTagged_pT_%s_%s", treeName.c_str(), selType.c_str()));
            hTagged_pT->Add(hFakes_pT);
    
            TGraphAsymmErrors* gPurity = new TGraphAsymmErrors();
            gPurity->Divide(hAccepted_pT, hTagged_pT, "cl=0.683 b(1,1) mode");
            gPurity->SetTitle("Purity vs Jet pT;Jet pT [GeV];Purity");
            gPurity->SetName(Form("gPurity_%s_%s", treeName.c_str(), selType.c_str()));
            gPurity->SetLineColor(colors[iSel]);
            gPurity->SetLineWidth(2);
            gPurity->SetMarkerStyle(markers[iSel]);
            gPurity->SetMarkerColor(colors[iSel]);
            gPurity->SetMarkerSize(1.2);
            
            efficiencyGraphs.push_back(gEfficiency);
            purityGraphs.push_back(gPurity);
            selectionLabels.push_back(selType);
            
        } // End loop over selections

        // Plot Efficiency comparison
        TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
        c1->SetLeftMargin(0.12);
        c1->SetBottomMargin(0.12);
    
        efficiencyGraphs[0]->GetYaxis()->SetRangeUser(0, 1.1);
        efficiencyGraphs[0]->SetTitle(";Jet pT [GeV];Efficiency");
        efficiencyGraphs[0]->Draw("APE");
        for(int i = 1; i < efficiencyGraphs.size(); i++){
            efficiencyGraphs[i]->Draw("PE same");
        }
    
        TLegend* leg1 = new TLegend(0.18, 0.18, 0.51, 0.35);
        leg1->SetBorderSize(0);
        leg1->SetFillStyle(0);
        leg1->SetTextFont(42);
        leg1->SetTextSize(0.04);
        for(int i = 0; i < efficiencyGraphs.size(); i++){
            leg1->AddEntry(efficiencyGraphs[i], selectionLabels[i].c_str(), "lep");
        }
        leg1->Draw();
    
        TLatex* latex = new TLatex();
        latex->SetNDC();
        latex->SetTextFont(42);
        latex->SetTextSize(0.04);
        latex->DrawLatex(0.18, 0.85, Form("#mu pT > %.1f GeV", mupt1));
    
        c1->SaveAs(Form("efficiency_comparison_%s.pdf", treeName.c_str()));

        // Plot Purity comparison
        TCanvas* c2 = new TCanvas("c2", "c2", 800, 600);
        c2->SetLeftMargin(0.12);
        c2->SetBottomMargin(0.12);
    
        purityGraphs[0]->GetYaxis()->SetRangeUser(0, 1.1);
        purityGraphs[0]->SetTitle(";Jet pT [GeV];Purity");
        purityGraphs[0]->Draw("APE");
        for(int i = 1; i < purityGraphs.size(); i++){
            purityGraphs[i]->Draw("PE same");
        }
    
        TLegend* leg2 = new TLegend(0.18, 0.18, 0.51, 0.35);
        leg2->SetBorderSize(0);
        leg2->SetFillStyle(0);
        leg2->SetTextFont(42);
        leg2->SetTextSize(0.04);
        for(int i = 0; i < purityGraphs.size(); i++){
            leg2->AddEntry(purityGraphs[i], selectionLabels[i].c_str(), "lep");
        }
        leg2->Draw();
    
        latex->DrawLatex(0.18, 0.85, Form("#mu pT > %.1f GeV", mupt1));
    
        c2->SaveAs(Form("purity_comparison_%s.pdf", treeName.c_str()));

        // Plot both efficiency and purity on same canvas
            TCanvas* c3 = new TCanvas("c3", "c3", 1200, 600);
        c3->Divide(2, 1);
    
        c3->cd(1);
        gPad->SetLeftMargin(0.12);
        gPad->SetBottomMargin(0.12);
        efficiencyGraphs[0]->Draw("APE");
        for(int i = 1; i < efficiencyGraphs.size(); i++){
            efficiencyGraphs[i]->Draw("PE same");
        }
        leg1->Draw();
        latex->DrawLatex(0.18, 0.85, Form("#mu pT > %.1f GeV", mupt1));
    
        c3->cd(2);
        gPad->SetLeftMargin(0.12);
        gPad->SetBottomMargin(0.12);
        purityGraphs[0]->Draw("APE");
        for(int i = 1; i < purityGraphs.size(); i++){
            purityGraphs[i]->Draw("PE same");
        }
        leg2->Draw();
        latex->DrawLatex(0.18, 0.85, Form("#mu pT > %.1f GeV", mupt1));
    
        c3->SaveAs(Form("efficiency_purity_comparison_%s.pdf", treeName.c_str()));

            // Print summary
        for(int iSel = 0; iSel < selectionLabels.size(); iSel++){
            cout << "\n=== Summary for " << treeName << " - " << selectionLabels[iSel] << " ===" << endl;
            cout << "Jet pT bin [GeV]  |  Efficiency  |  Purity" << endl;
            cout << "------------------------------------------------" << endl;
            for(int i = 0; i < efficiencyGraphs[iSel]->GetN(); i++){
                double x, eff, pur;
                efficiencyGraphs[iSel]->GetPoint(i, x, eff);
                purityGraphs[iSel]->GetPoint(i, x, pur);
                double eff_err_low = efficiencyGraphs[iSel]->GetErrorYlow(i);
                double eff_err_high = efficiencyGraphs[iSel]->GetErrorYhigh(i);
                double pur_err_low = purityGraphs[iSel]->GetErrorYlow(i);
                double pur_err_high = purityGraphs[iSel]->GetErrorYhigh(i);
                cout << Form("%.0f  |  %.3f +%.3f -%.3f  |  %.3f +%.3f -%.3f", 
                             x, eff, eff_err_high, eff_err_low, pur, pur_err_high, pur_err_low) << endl;
            }
            cout << endl;
        }
    
    } // End loop over trees

}