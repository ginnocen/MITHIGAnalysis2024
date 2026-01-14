//////////////////////////////////////////////////////////
//     ACCEPTANCE x EFFICIENCY MAP CALCULATION MACRO    //
////////////////////////////////////////////////////////// 

#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TLine.h>
#include <TStyle.h>
#include <TTreeFormula.h>

#include <iostream>
#include <vector>

using namespace std;
#include "CommandLine.h" 
#include "DimuonMessenger.h"
#include "Messenger.h"   
#include "ProgressBar.h" 

using namespace std;

vector<int> isSelected(DimuonJetMessenger *Jet, float muPtCut, int chargeSelection, bool isData){
    // RETURNS A 3-INT VECTOR THAT ENCODES AT EACH INDEX IF THE JET PASSES THE GIVEN SELECTIONS
    // 0 - Inclusive Jet selection
    // 1 - Dimuon Jet selection
    // 2 - Gen Dimuon Jet selection
    vector<int> indices = {0, 0, 0};

    if(isData){

        //EVENT SELECTION NOTE PV FILTER + BEAM SCRAPING FILTER APPLIED IN SKIM
        if(!(Jet->HLT_HIAK4PFJet80_v1)){ return indices;} // TRIGGER SELECTION (temporary)
        indices[0] = 1;

        // DIMUON SELECTIONS
        if(Jet->IsMuMuTagged == 1){
            indices[1] = 1;
            if(chargeSelection != 0 && Jet->muCharge1 * Jet->muCharge2 != chargeSelection){indices[1] = 0;} 
            if(Jet->muPt1 < muPtCut || Jet->muPt2 < muPtCut){indices[1] = 0;}
        }

        // NO GEN SELECTION FOR DATA
        indices[2] = 0;
    }
    

    if(!isData){

        // NO EVENT / JET SELECTION for MC right now
        indices[0] = 1;

        // DIMUON SELECTIONS
        if(Jet->IsMuMuTagged == 1){
            indices[1] = 1;
            if(Jet->muPt1 < muPtCut || Jet->muPt2 < muPtCut){indices[1] = 0;}
            if(chargeSelection != 0 && Jet->muCharge1 * Jet->muCharge2 != chargeSelection){indices[1] = 0;}
        }

        // GEN DIMUON SELECTIONS
        if(Jet->GenIsMuMuTagged == 1){
            indices[2] = 1;
            if(Jet->GenMuPt1 < muPtCut || Jet->GenMuPt2 < muPtCut){indices[2] = 0;}
        }
    }

    return indices;
}

int isGenSelected(GenDimuonJetMessenger *Jet){
    // RETURNS IF THE GEN JET IS SELECTED. FOR NOW NOTHING IS SPECIAl HERE 
    return 1;
}

int main(int argc, char *argv[]) {
    gStyle->SetOptStat(0);

    // INPUTS
    cout << "Beginning Acceptance x Efficiency" << endl;
    CommandLine CL(argc, argv);
    string file = CL.Get("Input");
    string output = CL.Get("Output");
    bool isData = CL.GetBool("isData", false);
    vector<double> ptBins = CL.GetDoubleVector("ptBins");
    int chargeSelection = CL.GetInt("chargeSelection",0);
    float muPtSelection = CL.GetDouble("muPt",3.5);
    bool makeplots = CL.GetBool("makeplots", false);

    // IMPORT TREE
    TFile* input = TFile::Open(file.c_str());
    DimuonJetMessenger *t = new DimuonJetMessenger(input, "Tree");
    GenDimuonJetMessenger *tgen = new GenDimuonJetMessenger(input, "GenTree");

    // DECLARE RECO HISTS
    TFile* outFile = new TFile(output.c_str(), "RECREATE");
    outFile->cd();
    TH2D* hRecoInclusiveJets = new TH2D("hRecoInclusiveJets","hRecoInclusiveJets", ptBins.size()-1, ptBins.front(), ptBins.back(), 10, -2.4, 2.4);
    TH2D* hFakeInclusiveJets = new TH2D("hFakeInclusiveJets","hFakeInclusiveJets", ptBins.size()-1, ptBins.front(), ptBins.back(), 10, -2.4, 2.4);
    TH2D* hGenDimJets = new TH2D("hGenDimJets","hGenDimJets", ptBins.size()-1, ptBins.front(), ptBins.back(), 10, -2.4, 2.4);
    TH2D* hRecoDimJets = new TH2D("hRecoDimJets","hRecoDimJets", ptBins.size()-1, ptBins.front(), ptBins.back(), 10, -2.4, 2.4);
    TH2D* hFakeDimJets = new TH2D("hFakeDimJets","hFakeDimJets", ptBins.size()-1, ptBins.front(), ptBins.back(), 10, -2.4, 2.4);
    hRecoInclusiveJets->GetXaxis()->Set(ptBins.size()-1, ptBins.data()); 
    hFakeInclusiveJets->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
    hGenDimJets->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
    hRecoDimJets->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
    hFakeDimJets->GetXaxis()->Set(ptBins.size()-1, ptBins.data());

    // DECLARE GEN HIST
    TH2D* hGenInclusiveJets = new TH2D("hGenInclusiveJets","hGenInclusiveJets", ptBins.size()-1, ptBins.front(), ptBins.back(), 10, -2.4, 2.4);
    hGenInclusiveJets->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
    
    // DECLARE NTUPLES
    TNtuple* ntGenInclusiveJets = new TNtuple("ntGenInclusiveJets","ntGenInclusiveJets", "JetPT:JetEta");
    TNtuple* ntRecoInclusiveJets = new TNtuple("ntRecoInclusiveJets","ntRecoInclusiveJets", "JetPT:JetEta:IsFake"); // Trees only important for Inclusive jet unfolding. 

    // JET LOOP
    unsigned long nentries = t->GetEntries();
    ProgressBar Bar(cout, nentries);
    for(int i = 0; i < t->GetEntries(); i++) { 

        if (i % 1000 == 0) {
            Bar.Update(i);
            Bar.Print();
        }
        
        t->GetEntry(i);
        vector<int> selected = isSelected(t, muPtSelection, chargeSelection, isData); 

        if(selected[0] == 1){
            hRecoInclusiveJets->Fill(t->JetPT, t->JetEta);
            if(!(t->JetIsGenMatched)){ // NOTE THE MATCHING BRANCHES ARE FALSE BY DEFAULT i.e. WHEN USING DATA SAMPLES
                hFakeInclusiveJets->Fill(t->JetPT, t->JetEta);
                ntRecoInclusiveJets->Fill(t->JetPT, t->JetEta, 1); // FILL WITH FAKE FLAG.
            }
            else{
                ntRecoInclusiveJets->Fill(t->JetPT, t->JetEta, 0); 
            }
        }

        if(selected[1] == 1){
            hRecoDimJets->Fill(t->JetPT, t->JetEta);
            if(!(t->mumuIsGenMatched)){ // NOTE THE MATCHING BRANCHES ARE FALSE BY DEFAULT i.e. WHEN USING DATA SAMPLES 
                hFakeDimJets->Fill(t->JetPT, t->JetEta);
            }
        }

        if(selected[2] == 1){
            hGenDimJets->Fill(t->JetPT, t->JetEta);
        }
        
    }
    cout << " finished with reco jet loop" << endl;

    // GEN JET LOOP
    unsigned long genentries = tgen->GetEntries();
    ProgressBar Bar2(cout, genentries);
    for(int i = 0; i < genentries; i++) { 

        if(isData){break;}

        if(i % 1000 == 0) {
            Bar2.Update(i);
            Bar2.Print();
        }
        
        tgen->GetEntry(i);
        if(isGenSelected(tgen) == 1){
            hGenInclusiveJets->Fill(tgen->GenJetPT, tgen->GenJetEta);
            ntGenInclusiveJets->Fill(tgen->GenJetPT, tgen->GenJetEta);
        }

    }
    cout << " finished with gen jet loop" << endl;


    // TAKE RATIOS + SAVE HISTOGRAMS
    outFile->cd();
    hRecoInclusiveJets->Write(); // THE RECO HISTS WILL BE WRITTEN REGARDLESS OF DATA/MC STATUS
    ntRecoInclusiveJets->Write();
    hRecoDimJets->Write();

    TH2D* JetEfficiency = (TH2D*)hRecoInclusiveJets->Clone(); // DECLARE 
    TH2D* JetPurity = (TH2D*)hRecoInclusiveJets->Clone();
    TH2D* DimJetEfficiency = (TH2D*)hRecoDimJets->Clone();
    TH2D* DimJetPurity = (TH2D*)hRecoDimJets->Clone();

    if(!isData){

        hGenInclusiveJets->Write(); // GEN JET HISTOGRAMS
        ntGenInclusiveJets->Write();
        hFakeInclusiveJets->Write(); // FAKE INCLUSIVE JET HISTOGRAMS

        hGenDimJets->Write(); // GEN DIMUON JET HISTOGRAMS
        hFakeDimJets->Write(); // FAKE DIMUON JET HISTOGRAMS


        JetEfficiency->Divide(hGenInclusiveJets);
        JetEfficiency->SetName("JetEfficiency");
        JetEfficiency->Write();
        
        JetPurity->Add(hFakeInclusiveJets, -1);
        JetPurity->Divide(hRecoInclusiveJets);
        JetPurity->SetName("JetPurity");
        JetPurity->Write();

        DimJetEfficiency->Divide(hGenDimJets);
        DimJetEfficiency->SetName("DimJetEfficiency");
        DimJetEfficiency->Write();

        DimJetPurity->Add(hFakeDimJets, -1);
        DimJetPurity->Divide(hRecoDimJets);
        DimJetPurity->SetName("DimJetPurity");
        DimJetPurity->Write();

    }
    
    
    // SAVE COMMAND LINE PARAMS
    TNamed paramFile("InputFile", file.c_str());
    paramFile.Write();
    TNamed paramIsData("isData", isData ? "true" : "false");
    paramIsData.Write();
    TNamed paramCharge("chargeSelection", std::to_string(chargeSelection).c_str());
    paramCharge.Write();
    TNamed paramMuPt("muPtSelection", std::to_string(muPtSelection).c_str());
    paramMuPt.Write();
    TNamed paramMakePlots("makeplots", makeplots ? "true" : "false");
    paramMakePlots.Write();

    if(makeplots){
        TDirectory* plotDir = outFile->mkdir("plots");
        plotDir->cd();

        // 1.) Inclusive Jet pT spectrum aggregated over all Eta bins
        TCanvas* c_incl_pt = new TCanvas("c_incl_pt", "Inclusive Jet pT", 800, 600);
        c_incl_pt->SetLogy();
        TH1D* h_reco_incl_pt = hRecoInclusiveJets->ProjectionX("h_reco_incl_pt");
        h_reco_incl_pt->SetLineColor(kBlack);
        h_reco_incl_pt->SetLineWidth(2);
        h_reco_incl_pt->SetTitle("Inclusive Jet p_{T} Spectrum;Jet p_{T} [GeV];Jets");
        h_reco_incl_pt->SetMinimum(1);
        h_reco_incl_pt->Draw("HIST");
        
        TLegend* leg1 = new TLegend(0.6, 0.6, 0.85, 0.85);
        leg1->AddEntry(h_reco_incl_pt, "Reco Jets", "l");
        
        if(!isData){
            TH1D* h_gen_incl_pt = hGenInclusiveJets->ProjectionX("h_gen_incl_pt");
            h_gen_incl_pt->SetLineColor(kBlue);
            h_gen_incl_pt->SetLineWidth(2);
            h_gen_incl_pt->Draw("HIST SAME");
            
            TH1D* h_fake_incl_pt = hFakeInclusiveJets->ProjectionX("h_fake_incl_pt");
            h_fake_incl_pt->SetLineColor(kRed);
            h_fake_incl_pt->SetLineWidth(2);
            h_fake_incl_pt->Draw("HIST SAME");
            
            leg1->AddEntry(h_gen_incl_pt, "Gen Jets", "l");
            leg1->AddEntry(h_fake_incl_pt, "Fake Jets", "l");
        }
        
        leg1->Draw();
        c_incl_pt->SaveAs("plots/inclusive_jet_pt.pdf");
        c_incl_pt->Write();

        // 2.) Dimuon Jet pT spectrum aggregated over all Eta bins
        TCanvas* c_dim_pt = new TCanvas("c_dim_pt", "Dimuon Jet pT", 800, 600);
        c_dim_pt->SetLogy();
        TH1D* h_reco_dim_pt = hRecoDimJets->ProjectionX("h_reco_dim_pt");
        h_reco_dim_pt->SetLineColor(kBlack);
        h_reco_dim_pt->SetLineWidth(2);
        h_reco_dim_pt->SetTitle("Dimuon Jet p_{T} Spectrum;Jet p_{T} [GeV];Jets");
        h_reco_dim_pt->SetMinimum(1);
        h_reco_dim_pt->Draw("HIST");
        
        TLegend* leg2 = new TLegend(0.6, 0.6, 0.85, 0.85);
        leg2->AddEntry(h_reco_dim_pt, "Reco Dimuon Jets", "l");
        
        if(!isData){
            TH1D* h_gen_dim_pt = hGenDimJets->ProjectionX("h_gen_dim_pt");
            h_gen_dim_pt->SetLineColor(kBlue);
            h_gen_dim_pt->SetLineWidth(2);
            h_gen_dim_pt->Draw("HIST SAME");
            
            TH1D* h_fake_dim_pt = hFakeDimJets->ProjectionX("h_fake_dim_pt");
            h_fake_dim_pt->SetLineColor(kRed);
            h_fake_dim_pt->SetLineWidth(2);
            h_fake_dim_pt->Draw("HIST SAME");
            
            leg2->AddEntry(h_gen_dim_pt, "Gen Dimuon Jets", "l");
            leg2->AddEntry(h_fake_dim_pt, "Fake Dimuon Jets", "l");
        }
        
        leg2->Draw();
        c_dim_pt->SaveAs("plots/dimuon_jet_pt.pdf");
        c_dim_pt->Write();

        if(!isData){
            // Calculate matched jets for efficiency with/without fakes
            TH2D* hMatchedInclusiveJets = (TH2D*)hRecoInclusiveJets->Clone("hMatchedInclusiveJets");
            hMatchedInclusiveJets->Add(hFakeInclusiveJets, -1);
            
            TH2D* JetEfficiency_NoFakes = (TH2D*)hMatchedInclusiveJets->Clone();
            JetEfficiency_NoFakes->Divide(hGenInclusiveJets);
            
            TH2D* hMatchedDimJets = (TH2D*)hRecoDimJets->Clone("hMatchedDimJets");
            hMatchedDimJets->Add(hFakeDimJets, -1);
            
            TH2D* DimJetEfficiency_NoFakes = (TH2D*)hMatchedDimJets->Clone();
            DimJetEfficiency_NoFakes->Divide(hGenDimJets);

            // 3.) Inclusive jet efficiency and purity vs eta for each pT bin
            for(int iBin = 1; iBin <= ptBins.size()-1; iBin++) {
                float ptMin = ptBins[iBin-1];
                float ptMax = ptBins[iBin];
                
                // Efficiency vs eta
                TCanvas* c_eff_eta = new TCanvas(Form("c_incl_eff_eta_pt%.0f_%.0f", ptMin, ptMax), "", 800, 600);
                TH1D* h_eff_nofakes = JetEfficiency_NoFakes->ProjectionY(Form("h_eff_nofakes_pt%.0f_%.0f", ptMin, ptMax), iBin, iBin);
                h_eff_nofakes->SetLineColor(kBlue);
                h_eff_nofakes->SetLineWidth(2);
                h_eff_nofakes->SetTitle(Form("Inclusive Jet Efficiency (%.0f < p_{T} < %.0f GeV);Jet #eta;Efficiency", ptMin, ptMax));
                h_eff_nofakes->SetMinimum(0);
                h_eff_nofakes->SetMaximum(1.2);
                h_eff_nofakes->Draw("HIST");
                
                TH1D* h_eff_withfakes = JetEfficiency->ProjectionY(Form("h_eff_withfakes_pt%.0f_%.0f", ptMin, ptMax), iBin, iBin);
                h_eff_withfakes->SetLineColor(kRed);
                h_eff_withfakes->SetLineWidth(2);
                h_eff_withfakes->SetLineStyle(2);
                h_eff_withfakes->Draw("HIST SAME");
                
                TLine* line_eff_eta = new TLine(h_eff_nofakes->GetXaxis()->GetXmin(), 1.0, h_eff_nofakes->GetXaxis()->GetXmax(), 1.0);
                line_eff_eta->SetLineStyle(2);
                line_eff_eta->SetLineColor(kGray+2);
                line_eff_eta->Draw();
                
                TLegend* leg_eff = new TLegend(0.5, 0.2, 0.85, 0.35);
                leg_eff->AddEntry(h_eff_nofakes, "Efficiency (no fakes)", "l");
                leg_eff->AddEntry(h_eff_withfakes, "Efficiency (with fakes)", "l");
                leg_eff->Draw();
                
                c_eff_eta->SaveAs(Form("plots/incl_efficiency_eta_pt%.0f_%.0f.pdf", ptMin, ptMax));
                c_eff_eta->Write();
                
                // Purity vs eta
                TCanvas* c_pur_eta = new TCanvas(Form("c_incl_pur_eta_pt%.0f_%.0f", ptMin, ptMax), "", 800, 600);
                TH1D* h_purity_eta = JetPurity->ProjectionY(Form("h_purity_eta_pt%.0f_%.0f", ptMin, ptMax), iBin, iBin);
                h_purity_eta->SetLineColor(kBlack);
                h_purity_eta->SetLineWidth(2);
                h_purity_eta->SetTitle(Form("Inclusive Jet Purity (%.0f < p_{T} < %.0f GeV);Jet #eta;Purity", ptMin, ptMax));
                //h_purity_eta->SetMinimum(0);
                //h_purity_eta->SetMaximum(1.2);
                h_purity_eta->Draw("HIST");
                
                TLine* line_pur_eta = new TLine(h_purity_eta->GetXaxis()->GetXmin(), 1.0, h_purity_eta->GetXaxis()->GetXmax(), 1.0);
                line_pur_eta->SetLineStyle(2);
                line_pur_eta->SetLineColor(kGray+2);
                line_pur_eta->Draw();
                
                c_pur_eta->SaveAs(Form("plots/incl_purity_eta_pt%.0f_%.0f.pdf", ptMin, ptMax));
                c_pur_eta->Write();
            }

            // 4.) Inclusive jet efficiency and purity vs pT (aggregated over eta)
            TCanvas* c_eff_pt = new TCanvas("c_incl_eff_pt", "", 800, 600);
            TH1D* h_matched_pt = hMatchedInclusiveJets->ProjectionX("h_matched_pt");
            TH1D* h_gen_pt = hGenInclusiveJets->ProjectionX("h_gen_pt");
            TH1D* h_eff_pt = (TH1D*)h_matched_pt->Clone("h_eff_pt");
            h_eff_pt->Divide(h_gen_pt);
            h_eff_pt->SetLineColor(kBlack);
            h_eff_pt->SetLineWidth(2);
            h_eff_pt->SetTitle("Inclusive Jet Efficiency;Jet p_{T} [GeV];Efficiency");
            h_eff_pt->SetMinimum(0);
            h_eff_pt->SetMaximum(1.2);
            h_eff_pt->Draw("HIST");
            
            TLine* line_eff_pt = new TLine(h_eff_pt->GetXaxis()->GetXmin(), 1.0, h_eff_pt->GetXaxis()->GetXmax(), 1.0);
            line_eff_pt->SetLineStyle(2);
            line_eff_pt->SetLineColor(kGray+2);
            line_eff_pt->Draw();
            
            c_eff_pt->SaveAs("plots/incl_efficiency_pt.pdf");
            c_eff_pt->Write();
            
            TCanvas* c_pur_pt = new TCanvas("c_incl_pur_pt", "", 800, 600);
            TH1D* h_matched_pur_pt = hMatchedInclusiveJets->ProjectionX("h_matched_pur_pt");
            TH1D* h_reco_pur_pt = hRecoInclusiveJets->ProjectionX("h_reco_pur_pt");
            TH1D* h_pur_pt = (TH1D*)h_matched_pur_pt->Clone("h_pur_pt");
            h_pur_pt->Divide(h_reco_pur_pt);
            h_pur_pt->SetLineColor(kBlack);
            h_pur_pt->SetLineWidth(2);
            h_pur_pt->SetTitle("Inclusive Jet Purity;Jet p_{T} [GeV];Purity");
            h_pur_pt->SetMinimum(0);
            h_pur_pt->SetMaximum(1.2);
            h_pur_pt->Draw("HIST");
            
            TLine* line_pur_pt = new TLine(h_pur_pt->GetXaxis()->GetXmin(), 1.0, h_pur_pt->GetXaxis()->GetXmax(), 1.0);
            line_pur_pt->SetLineStyle(2);
            line_pur_pt->SetLineColor(kGray+2);
            line_pur_pt->Draw();
            
            c_pur_pt->SaveAs("plots/incl_purity_pt.pdf");
            c_pur_pt->Write();

            // 5.) Dimuon jet efficiency and purity vs eta for each pT bin
            for(int iBin = 1; iBin <= ptBins.size()-1; iBin++) {
                float ptMin = ptBins[iBin-1];
                float ptMax = ptBins[iBin];
                
                // Efficiency vs eta
                TCanvas* c_dim_eff_eta = new TCanvas(Form("c_dim_eff_eta_pt%.0f_%.0f", ptMin, ptMax), "", 800, 600);
                TH1D* h_dim_eff_nofakes = DimJetEfficiency_NoFakes->ProjectionY(Form("h_dim_eff_nofakes_pt%.0f_%.0f", ptMin, ptMax), iBin, iBin);
                h_dim_eff_nofakes->SetLineColor(kBlue);
                h_dim_eff_nofakes->SetLineWidth(2);
                h_dim_eff_nofakes->SetTitle(Form("Dimuon Jet Efficiency (%.0f < p_{T} < %.0f GeV);Jet #eta;Efficiency", ptMin, ptMax));
                h_dim_eff_nofakes->SetMinimum(0);
                h_dim_eff_nofakes->SetMaximum(1.2);
                h_dim_eff_nofakes->Draw("HIST");
                
                TH1D* h_dim_eff_withfakes = DimJetEfficiency->ProjectionY(Form("h_dim_eff_withfakes_pt%.0f_%.0f", ptMin, ptMax), iBin, iBin);
                h_dim_eff_withfakes->SetLineColor(kRed);
                h_dim_eff_withfakes->SetLineWidth(2);
                h_dim_eff_withfakes->SetLineStyle(2);
                h_dim_eff_withfakes->Draw("HIST SAME");
                
                TLine* line_dim_eff_eta = new TLine(h_dim_eff_nofakes->GetXaxis()->GetXmin(), 1.0, h_dim_eff_nofakes->GetXaxis()->GetXmax(), 1.0);
                line_dim_eff_eta->SetLineStyle(2);
                line_dim_eff_eta->SetLineColor(kGray+2);
                line_dim_eff_eta->Draw();
                
                TLegend* leg_dim_eff = new TLegend(0.5, 0.2, 0.85, 0.35);
                leg_dim_eff->AddEntry(h_dim_eff_nofakes, "Efficiency (no fakes)", "l");
                leg_dim_eff->AddEntry(h_dim_eff_withfakes, "Efficiency (with fakes)", "l");
                leg_dim_eff->Draw();
                
                c_dim_eff_eta->SaveAs(Form("plots/dim_efficiency_eta_pt%.0f_%.0f.pdf", ptMin, ptMax));
                c_dim_eff_eta->Write();
                
                // Purity vs eta
                TCanvas* c_dim_pur_eta = new TCanvas(Form("c_dim_pur_eta_pt%.0f_%.0f", ptMin, ptMax), "", 800, 600);
                TH1D* h_dim_purity_eta = DimJetPurity->ProjectionY(Form("h_dim_purity_eta_pt%.0f_%.0f", ptMin, ptMax), iBin, iBin);
                h_dim_purity_eta->SetLineColor(kBlack);
                h_dim_purity_eta->SetLineWidth(2);
                h_dim_purity_eta->SetTitle(Form("Dimuon Jet Purity (%.0f < p_{T} < %.0f GeV);Jet #eta;Purity", ptMin, ptMax));
                h_dim_purity_eta->SetMinimum(0);
                h_dim_purity_eta->SetMaximum(1.2);
                h_dim_purity_eta->Draw("HIST");
                
                TLine* line_dim_pur_eta = new TLine(h_dim_purity_eta->GetXaxis()->GetXmin(), 1.0, h_dim_purity_eta->GetXaxis()->GetXmax(), 1.0);
                line_dim_pur_eta->SetLineStyle(2);
                line_dim_pur_eta->SetLineColor(kGray+2);
                line_dim_pur_eta->Draw();
                
                c_dim_pur_eta->SaveAs(Form("plots/dim_purity_eta_pt%.0f_%.0f.pdf", ptMin, ptMax));
                c_dim_pur_eta->Write();
            }

            // Dimuon jet efficiency and purity vs pT (aggregated over eta)
            TCanvas* c_dim_eff_pt = new TCanvas("c_dim_eff_pt", "", 800, 600);
            TH1D* h_dim_matched_pt = hMatchedDimJets->ProjectionX("h_dim_matched_pt");
            TH1D* h_dim_gen_pt = hGenDimJets->ProjectionX("h_dim_gen_pt");
            TH1D* h_dim_eff_pt = (TH1D*)h_dim_matched_pt->Clone("h_dim_eff_pt");
            h_dim_eff_pt->Divide(h_dim_gen_pt);
            h_dim_eff_pt->SetLineColor(kBlack);
            h_dim_eff_pt->SetLineWidth(2);
            h_dim_eff_pt->SetTitle("Dimuon Jet Efficiency;Jet p_{T} [GeV];Efficiency");
            h_dim_eff_pt->SetMinimum(0);
            h_dim_eff_pt->SetMaximum(1.2);
            h_dim_eff_pt->Draw("HIST");
            
            TLine* line_dim_eff_pt = new TLine(h_dim_eff_pt->GetXaxis()->GetXmin(), 1.0, h_dim_eff_pt->GetXaxis()->GetXmax(), 1.0);
            line_dim_eff_pt->SetLineStyle(2);
            line_dim_eff_pt->SetLineColor(kGray+2);
            line_dim_eff_pt->Draw();
            
            c_dim_eff_pt->SaveAs("plots/dim_efficiency_pt.pdf");
            c_dim_eff_pt->Write();
            
            TCanvas* c_dim_pur_pt = new TCanvas("c_dim_pur_pt", "", 800, 600);
            TH1D* h_dim_matched_pur_pt = hMatchedDimJets->ProjectionX("h_dim_matched_pur_pt");
            TH1D* h_dim_reco_pur_pt = hRecoDimJets->ProjectionX("h_dim_reco_pur_pt");
            TH1D* h_dim_pur_pt = (TH1D*)h_dim_matched_pur_pt->Clone("h_dim_pur_pt");
            h_dim_pur_pt->Divide(h_dim_reco_pur_pt);
            h_dim_pur_pt->SetLineColor(kBlack);
            h_dim_pur_pt->SetLineWidth(2);
            h_dim_pur_pt->SetTitle("Dimuon Jet Purity;Jet p_{T} [GeV];Purity");
            h_dim_pur_pt->SetMinimum(0);
            h_dim_pur_pt->SetMaximum(1.2);
            h_dim_pur_pt->Draw("HIST");
            
            TLine* line_dim_pur_pt = new TLine(h_dim_pur_pt->GetXaxis()->GetXmin(), 1.0, h_dim_pur_pt->GetXaxis()->GetXmax(), 1.0);
            line_dim_pur_pt->SetLineStyle(2);
            line_dim_pur_pt->SetLineColor(kGray+2);
            line_dim_pur_pt->Draw();
            
            c_dim_pur_pt->SaveAs("plots/dim_purity_pt.pdf");
            c_dim_pur_pt->Write();
        }
    }

    outFile->Close();
}





