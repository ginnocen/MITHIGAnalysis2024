///////////////////////////////////////////////////////////////////////
//        CONSTRUCTS DIMUON DCA AND INVARIANT MASS CURVES            //
///////////////////////////////////////////////////////////////////////

#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TTreeFormula.h>
#include <TStyle.h>

#include <iostream>
#include <vector>

using namespace std;
#include "CommandLine.h" 
#include "DimuonMessenger.h"
#include "Messenger.h"   
#include "ProgressBar.h" 

using namespace std;

bool isDimuonSelected(DimuonJetMessenger *Dimuon, float muPtCut, int chargeSelection, bool isData){

    // IMPLEMENT EVENT SELECTION HERE FOR DATA JETS
    // NOTHING HAPPENS HERE IF MC

    if(!Dimuon->IsMuMuTagged){
        return false;
    }

    if(Dimuon->muPt1 < muPtCut || Dimuon->muPt2 < muPtCut){
        return false;
    }

    if(Dimuon->muCharge1 * Dimuon->muCharge2 != chargeSelection && chargeSelection != 0){
        return false;
    }

    return true;
}

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

int main(int argc, char *argv[]) {

    gStyle->SetOptStat(0);

    // INPUTS
    cout << "Filling Distributions" << endl;
    CommandLine CL(argc, argv);
    string file = CL.Get("Input");
    string efficiencies = CL.Get("Input_Efficiency");
    string output = CL.Get("Output");
    bool isData = CL.GetBool("IsData");
    int chargeSelection = CL.GetInt("chargeSelection", 1);
    vector<double> ptBins = CL.GetDoubleVector("ptBins");
    float muPtSelection = CL.GetDouble("muPt",3.5);
    int useWeightVariation = CL.GetInt("useWeightVariation",0);
    bool makeplots = CL.GetBool("makeplots", false);

    // IMPORT TREE
    TFile* input = TFile::Open(file.c_str());
    DimuonJetMessenger *t = new DimuonJetMessenger(input, "Tree");

    // IMPORT EFFICIENCIES
    TFile* effFile = TFile::Open(efficiencies.c_str());
    TH2D* DimJetEfficiency = (TH2D*)effFile->Get("DimJetEfficiency");

    // OUTPUT FILE
    TFile* outFile = new TFile(output.c_str(), "RECREATE");
    outFile->cd();
    
    // HISTOGRAMS + NTUPLE
    TH2D* hInvMass = new TH2D("hInvMass", "", ptBins.size()-1, ptBins.front(), ptBins.back(), 70, 0, 10);
    TH2D* hDCAProductSig = new TH2D("hDCAProductSig", "", ptBins.size()-1, ptBins.front(), ptBins.back(), 50, -3, 4);
    TH2D* hmuDR = new TH2D("hmuDR", "", ptBins.size()-1, ptBins.front(), ptBins.back(), 50, 0, 0.6);
    hInvMass->GetXaxis()->Set(ptBins.size()-1, ptBins.data()); 
    hDCAProductSig->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
    hmuDR->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
    TH2D* hEfficiency = (TH2D*)DimJetEfficiency->Clone("hEfficiency");
    hEfficiency->Reset();
    TNtuple* ntDimuon = new TNtuple("ntDimuon", "", "mumuMass:muDiDxy1Dxy2Sig:muDR:JetPT:weight");

    // FLAVOR HISTOGRAMS + NTUPLES (FOR TEMPLATES)
    vector<TH2D*> hInvMass_flavors;     
    vector<TH2D*> hmuDCAProductSig_flavors;
    vector<TH2D*> hmuDR_flavors;
    vector<TH2D*> hEfficiency_flavors;
    vector<TNtuple*> nt_flavors;
    vector<string> flavorNames;
    flavorNames = {"other", "uds", "c", "cc", "b", "bb"};
    for(int i = 0; i < 6; i++) {
        hInvMass_flavors.push_back(new TH2D(Form("hInvMass_%s", flavorNames[i].c_str()), "", ptBins.size()-1, ptBins.front(), ptBins.back(), 70, 0, 10));
        hmuDCAProductSig_flavors.push_back(new TH2D(Form("hDCAProductSig_%s", flavorNames[i].c_str()), "", ptBins.size()-1, ptBins.front(), ptBins.back(), 50, -3, 4));
        hmuDR_flavors.push_back(new TH2D(Form("hmuDR_%s", flavorNames[i].c_str()), "", ptBins.size()-1, ptBins.front(), ptBins.back(), 50, 0, 0.6));
        hInvMass_flavors[i]->GetXaxis()->Set(ptBins.size()-1, ptBins.data()); 
        hmuDCAProductSig_flavors[i]->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
        hmuDR_flavors[i]->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
        hEfficiency_flavors.push_back((TH2D*)DimJetEfficiency->Clone(Form("hEfficiency_%s", flavorNames[i].c_str())));
        hEfficiency_flavors[i]->Reset();
        nt_flavors.push_back(new TNtuple(Form("nt_%s", flavorNames[i].c_str()), "", "mumuMass:muDiDxy1Dxy2Sig:muDR:JetPT:weight"));
    }

    // JETS LOOP
    float weight = 0;
    unsigned long nentries = t->GetEntries();
    ProgressBar Bar(cout, nentries);
    for(int i = 0; i < nentries; i++){

        if (i % 1000 == 0) {
            Bar.Update(i);
            Bar.Print();
        }

        t->GetEntry(i);

        if(isDimuonSelected(t, muPtSelection, chargeSelection, isData)){
            
            // WEIGHTS REMOVED FOR INITIAL FITTING CHECKS
            //weight = 1 / DimJetEfficiency->GetBinContent(DimJetEfficiency->FindBin(t->JetPT, t->JetEta)); // DISCUSS WHERE TO ENTER THE WEIGHTS 
            //if(!isData){weight *= t->MuMuWeight}; --> will definitely want to use these weights to generate a DATA template fom MC
            weight = 1;

            hInvMass->Fill(t->JetPT, t->mumuMass, weight);
            hDCAProductSig->Fill(t->JetPT, log10(abs(t->muDiDxy1Dxy2 / t->muDiDxy1Dxy2Err)), weight);
            hmuDR->Fill(t->JetPT, t->muDR, weight);
            ntDimuon->Fill(t->mumuMass, log10(abs(t->muDiDxy1Dxy2 / t->muDiDxy1Dxy2Err)), t->muDR, t->JetPT, weight); 
            hEfficiency->Fill(t->JetPT, t->JetEta, 1/weight);

            if(isData){continue;} // ONLY MAKE TEMPLATES WITH MC 

            int flavorclass = getFlavorClass(t->NbHad, t->NcHad);
            hInvMass_flavors[flavorclass]->Fill(t->JetPT, t->mumuMass, weight);
            hmuDCAProductSig_flavors[flavorclass]->Fill(t->JetPT, log10(abs(t->muDiDxy1Dxy2 / t->muDiDxy1Dxy2Err)), weight);
            hmuDR_flavors[flavorclass]->Fill(t->JetPT, t->muDR, weight);
            nt_flavors[flavorclass]->Fill(t->mumuMass, log10(abs(t->muDiDxy1Dxy2 / t->muDiDxy1Dxy2Err)), t->muDR, t->JetPT, weight);
            hEfficiency_flavors[flavorclass]->Fill(t->JetPT, t->JetEta, 1/weight);

        }
        
    }
    cout << " END OF JET LOOP: SAVING OUTPUTS TO FILE" << endl;

    outFile->cd();
    hInvMass->Write();
    hDCAProductSig->Write();
    hmuDR->Write();
    ntDimuon->Write();
    hEfficiency->Write();
    if(!isData){
        for(int i = 0; i < 6; i++) {
            hInvMass_flavors[i]->Write();
            hmuDCAProductSig_flavors[i]->Write();
            hmuDR_flavors[i]->Write();
            nt_flavors[i]->Write();
            hEfficiency_flavors[i]->Write();
        }
    }
    
    // SAVE COMMAND LINE PARAMS
    TNamed paramFile("InputFile", file.c_str());
    paramFile.Write();
    TNamed paramEffFile("Input_Efficiency", efficiencies.c_str());
    paramEffFile.Write();
    TNamed paramIsData("IsData", isData ? "true" : "false");
    paramIsData.Write();
    TNamed paramCharge("chargeSelection", std::to_string(chargeSelection).c_str());
    paramCharge.Write();
    TNamed paramMuPt("muPtSelection", std::to_string(muPtSelection).c_str());
    paramMuPt.Write();
    TNamed paramWeightVar("useWeightVariation", std::to_string(useWeightVariation).c_str());
    paramWeightVar.Write();
    TNamed makeplotsParam("makeplots", makeplots ? "true" : "false");
    makeplotsParam.Write();

    // MAKE PLOTS!
    if(makeplots){
        TDirectory* plotDir = outFile->mkdir("plots");
        plotDir->cd();

        // Create 1D projections for each pT bin
        for(int iBin = 1; iBin <= ptBins.size()-1; iBin++) {
            float ptMin = ptBins[iBin-1];
            float ptMax = ptBins[iBin];
            
            // Mass projection
            TCanvas* c1 = new TCanvas(Form("c_mass_incl_pt%.0f_%.0f", ptMin, ptMax), "", 800, 600);
            TH1D* h_mass = hInvMass->ProjectionY(Form("h_mass_incl_pt%.0f_%.0f", ptMin, ptMax), iBin, iBin);
            h_mass->SetLineColor(kBlack);
            h_mass->SetLineWidth(2);
            h_mass->SetTitle(Form("Dimuon Mass (%.0f < p_{T} < %.0f GeV);m_{#mu#mu} [GeV];Entries", ptMin, ptMax));
            h_mass->Draw("HIST");
            c1->SaveAs(Form("plots/mass_incl_pt%.0f_%.0f.pdf", ptMin, ptMax));
            delete c1;
            
            // DCA projection
            TCanvas* c2 = new TCanvas(Form("c_dca_incl_pt%.0f_%.0f", ptMin, ptMax), "", 800, 600);
            TH1D* h_dca = hDCAProductSig->ProjectionY(Form("h_dca_incl_pt%.0f_%.0f", ptMin, ptMax), iBin, iBin);
            h_dca->SetLineColor(kBlack);
            h_dca->SetLineWidth(2);
            h_dca->SetTitle(Form("DCA Product Sig (%.0f < p_{T} < %.0f GeV);log_{10}(|DCA_{1}#timesDCA_{2}|/#sigma);Entries", ptMin, ptMax));
            h_dca->Draw("HIST");
            c2->SaveAs(Form("plots/dca_incl_pt%.0f_%.0f.pdf", ptMin, ptMax));
            delete c2;
            
            // DR projection
            TCanvas* c3 = new TCanvas(Form("c_dr_incl_pt%.0f_%.0f", ptMin, ptMax), "", 800, 600);
            TH1D* h_dr = hmuDR->ProjectionY(Form("h_dr_incl_pt%.0f_%.0f", ptMin, ptMax), iBin, iBin);
            h_dr->SetLineColor(kBlack);
            h_dr->SetLineWidth(2);
            h_dr->SetTitle(Form("#mu#mu #DeltaR (%.0f < p_{T} < %.0f GeV);#DeltaR(#mu,#mu);Entries", ptMin, ptMax));
            h_dr->Draw("HIST");
            c3->SaveAs(Form("plots/dr_incl_pt%.0f_%.0f.pdf", ptMin, ptMax));
            delete c3;
        }

        if(!isData){
            
            // Overlay plots for each pT bin
            int colors[7] = {kBlack, kGray+1, kBlue, kCyan, kGreen+2, kRed, kMagenta};
            for(int iBin = 1; iBin <= ptBins.size()-1; iBin++) {
                float ptMin = ptBins[iBin-1];
                float ptMax = ptBins[iBin];
                
                // Mass overlay
                TCanvas* c_mass_overlay = new TCanvas(Form("c_mass_overlay_pt%.0f_%.0f", ptMin, ptMax), "", 800, 600);
                TH1D* h_mass_incl = hInvMass->ProjectionY(Form("h_mass_incl_pt%.0f_%.0f", ptMin, ptMax), iBin, iBin);
                h_mass_incl->SetLineColor(colors[0]);
                h_mass_incl->SetLineWidth(2);
                h_mass_incl->SetStats(0);
                h_mass_incl->SetTitle(Form("Dimuon Mass (%.0f < p_{T} < %.0f GeV);m_{#mu#mu} [GeV];Entries", ptMin, ptMax));
                h_mass_incl->Draw("HIST");
                
                TLegend* leg_mass = new TLegend(0.65, 0.45, 0.88, 0.88);
                leg_mass->AddEntry(h_mass_incl, "Inclusive", "l");
                
                vector<TH1D*> h_mass_flavors;
                for(int i = 0; i < 6; i++) {
                    TH1D* h = hInvMass_flavors[i]->ProjectionY(Form("h_mass_%s_pt%.0f_%.0f", flavorNames[i].c_str(), ptMin, ptMax), iBin, iBin);
                    h->SetLineColor(colors[i+1]);
                    h->SetLineWidth(2);
                    h->Draw("HIST SAME");
                    h_mass_flavors.push_back(h);
                    leg_mass->AddEntry(h, flavorNames[i].c_str(), "l");
                }
                leg_mass->Draw();
                c_mass_overlay->SaveAs(Form("plots/mass_overlay_pt%.0f_%.0f.pdf", ptMin, ptMax));
                delete c_mass_overlay;
                
                // DCA overlay
                TCanvas* c_dca_overlay = new TCanvas(Form("c_dca_overlay_pt%.0f_%.0f", ptMin, ptMax), "", 800, 600);
                TH1D* h_dca_incl = hDCAProductSig->ProjectionY(Form("h_dca_incl_pt%.0f_%.0f", ptMin, ptMax), iBin, iBin);
                h_dca_incl->SetLineColor(colors[0]);
                h_dca_incl->SetLineWidth(2);
                h_dca_incl->SetStats(0);
                h_dca_incl->SetTitle(Form("DCA Product Sig (%.0f < p_{T} < %.0f GeV);log_{10}(|DCA_{1}#timesDCA_{2}|/#sigma);Entries", ptMin, ptMax));
                h_dca_incl->Draw("HIST");
                
                TLegend* leg_dca = new TLegend(0.65, 0.45, 0.88, 0.88);
                leg_dca->AddEntry(h_dca_incl, "Inclusive", "l");
                
                vector<TH1D*> h_dca_flavors;
                for(int i = 0; i < 6; i++) {
                    TH1D* h = hmuDCAProductSig_flavors[i]->ProjectionY(Form("h_dca_%s_pt%.0f_%.0f", flavorNames[i].c_str(), ptMin, ptMax), iBin, iBin);
                    h->SetLineColor(colors[i+1]);
                    h->SetLineWidth(2);
                    h->Draw("HIST SAME");
                    h_dca_flavors.push_back(h);
                    leg_dca->AddEntry(h, flavorNames[i].c_str(), "l");
                }
                leg_dca->Draw();
                c_dca_overlay->SaveAs(Form("plots/dca_overlay_pt%.0f_%.0f.pdf", ptMin, ptMax));
                delete c_dca_overlay;
                
                // DR overlay
                TCanvas* c_dr_overlay = new TCanvas(Form("c_dr_overlay_pt%.0f_%.0f", ptMin, ptMax), "", 800, 600);
                TH1D* h_dr_incl = hmuDR->ProjectionY(Form("h_dr_incl_pt%.0f_%.0f", ptMin, ptMax), iBin, iBin);
                h_dr_incl->SetLineColor(colors[0]);
                h_dr_incl->SetLineWidth(2);
                h_dr_incl->SetStats(0);
                h_dr_incl->SetTitle(Form("#mu#mu #DeltaR (%.0f < p_{T} < %.0f GeV);#DeltaR(#mu,#mu);Entries", ptMin, ptMax));
                h_dr_incl->Draw("HIST");
                
                TLegend* leg_dr = new TLegend(0.65, 0.45, 0.88, 0.88);
                leg_dr->AddEntry(h_dr_incl, "Inclusive", "l");
                
                vector<TH1D*> h_dr_flavors;
                for(int i = 0; i < 6; i++) {
                    TH1D* h = hmuDR_flavors[i]->ProjectionY(Form("h_dr_%s_pt%.0f_%.0f", flavorNames[i].c_str(), ptMin, ptMax), iBin, iBin);
                    h->SetLineColor(colors[i+1]);
                    h->SetLineWidth(2);
                    h->Draw("HIST SAME");
                    h_dr_flavors.push_back(h);
                    leg_dr->AddEntry(h, flavorNames[i].c_str(), "l");
                }
                leg_dr->Draw();
                c_dr_overlay->SaveAs(Form("plots/dr_overlay_pt%.0f_%.0f.pdf", ptMin, ptMax));
                delete c_dr_overlay;
            }
        }

    }


    
    outFile->Close();
}