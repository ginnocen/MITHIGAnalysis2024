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

bool isDimuonSelected(DimuonJetMessenger *Dimuon, float muPtCut, int chargeSelection, bool isData, int DataTrigger){

    if(!Dimuon->IsMuMuTagged){
        return false;
    }

    if(Dimuon->muPt1 < muPtCut || Dimuon->muPt2 < muPtCut){
        return false;
    }

    if(Dimuon->muCharge1 * Dimuon->muCharge2 != chargeSelection && chargeSelection != 0){
        return false;
    }

    if(isData) {
        if(DataTrigger == 80){
            if(!Dimuon->HLT_HIAK4PFJet80_v1){
                return false;
            }
        }
        if(DataTrigger == 60){ // IF 60 TRIGGER, MAKE SURE 80 DIDN'T FIRE FOR LOWEG SAMPLE
            if(!Dimuon->HLT_HIAK4PFJet60_v1 || Dimuon->HLT_HIAK4PFJet80_v1){
                return false;
            }
        }
        if(DataTrigger == 40){ // IF 40 TRIGGER, WE NEED AT LEAST 60 OR 40 FIRING 
            if(!Dimuon->HLT_HIAK4PFJet40_v1 && !Dimuon->HLT_HIAK4PFJet60_v1 || Dimuon->HLT_HIAK4PFJet80_v1){
                return false;
            }
        }
    }

    if(!isData){
        if(Dimuon->JetPT / Dimuon->PTHat > 3.0){
            return false;
        }
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
    string output = CL.Get("Output");
    bool isData = CL.GetBool("IsData");
    int DataTrigger = CL.GetInt("DataTrigger", 0);
    int chargeSelection = CL.GetInt("chargeSelection", 1);
    vector<double> ptBins = CL.GetDoubleVector("ptBins");
    float muPtSelection = CL.GetDouble("muPt",3.5);
    bool weightMC = CL.GetBool("weightMC", false);
    bool makeplots = CL.GetBool("makeplots", false);

    // IMPORT TREE
    TFile* input = TFile::Open(file.c_str());
    DimuonJetMessenger *t = new DimuonJetMessenger(input, "Tree");

    // OUTPUT FILE
    TFile* outFile = new TFile(output.c_str(), "RECREATE");
    outFile->cd();
    
    // HISTOGRAMS + NTUPLE
    TH2D* hInvMass = new TH2D("hInvMass", "", ptBins.size()-1, ptBins.front(), ptBins.back(), 70, 0, 10);
    TH2D* hDCAProductSig = new TH2D("hDCAProductSig", "", ptBins.size()-1, ptBins.front(), ptBins.back(), 50, -3, 4);
    TH2D* hmuDR = new TH2D("hmuDR", "", ptBins.size()-1, ptBins.front(), ptBins.back(), 50, 0, 0.6);
    TH2D* hmumuPt = new TH2D("hmumuPt", "", ptBins.size()-1, ptBins.front(), ptBins.back(), 50, 0, 200);
    TH2D* hmumuZ = new TH2D("hmumuZ", "", ptBins.size()-1, ptBins.front(), ptBins.back(), 50, 0, 1);
    TH2D* hCharges = new TH2D("hCharges", "", ptBins.size()-1, ptBins.front(), ptBins.back(), 3, -1, 2);

    hInvMass->GetXaxis()->Set(ptBins.size()-1, ptBins.data()); 
    hDCAProductSig->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
    hmuDR->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
    hmumuPt->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
    hmumuZ->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
    hCharges->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
    TNtuple* ntDimuon = new TNtuple("ntDimuon", "", "mumuMass:muDiDxy1Dxy2Sig:muDR:mumuPt:mumuZ:Charge:JetPT:weight");

    // FLAVOR HISTOGRAMS + NTUPLES (FOR TEMPLATES)
    vector<TH2D*> hInvMass_flavors;     
    vector<TH2D*> hmuDCAProductSig_flavors;
    vector<TH2D*> hmuDR_flavors;
    vector<TH2D*> hmumuPt_flavors;
    vector<TH2D*> hmumuZ_flavors;
    vector<TH2D*> hCharges_flavors;
    vector<TNtuple*> nt_flavors;
    vector<string> flavorNames;
    flavorNames = {"other", "uds", "c", "cc", "b", "bb"};
    for(int i = 0; i < 6; i++) {
        
        hInvMass_flavors.push_back(new TH2D(Form("hInvMass_%s", flavorNames[i].c_str()), "", ptBins.size()-1, ptBins.front(), ptBins.back(), 70, 0, 10));
        hmuDCAProductSig_flavors.push_back(new TH2D(Form("hDCAProductSig_%s", flavorNames[i].c_str()), "", ptBins.size()-1, ptBins.front(), ptBins.back(), 50, -3, 4));
        hmuDR_flavors.push_back(new TH2D(Form("hmuDR_%s", flavorNames[i].c_str()), "", ptBins.size()-1, ptBins.front(), ptBins.back(), 50, 0, 0.6));
        hmumuPt_flavors.push_back(new TH2D(Form("hmumuPt_%s", flavorNames[i].c_str()), "", ptBins.size()-1, ptBins.front(), ptBins.back(), 50, 0, 200));
        hmumuZ_flavors.push_back(new TH2D(Form("hmumuZ_%s", flavorNames[i].c_str()), "", ptBins.size()-1, ptBins.front(), ptBins.back(), 50, 0, 1));
        hCharges_flavors.push_back(new TH2D(Form("hCharges_%s", flavorNames[i].c_str()), "", ptBins.size()-1, ptBins.front(), ptBins.back(), 3, -1, 2));

        hInvMass_flavors[i]->GetXaxis()->Set(ptBins.size()-1, ptBins.data()); 
        hmuDCAProductSig_flavors[i]->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
        hmuDR_flavors[i]->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
        hmumuPt_flavors[i]->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
        hmumuZ_flavors[i]->GetXaxis()->Set(ptBins.size()-1, ptBins.data());
        hCharges_flavors[i]->GetXaxis()->Set(ptBins.size()-1, ptBins.data());

        nt_flavors.push_back(new TNtuple(Form("nt_%s", flavorNames[i].c_str()), "", "mumuMass:muDiDxy1Dxy2Sig:muDR:mumuPt:mumuZ:Charge:JetPT:weight"));
    }

    // JETS LOOP
    float weight = 1;
    unsigned long nentries = t->GetEntries();

    ProgressBar Bar(cout, nentries);
    for(int i = 0; i < nentries; i++){

        if (i % 1000 == 0) {
            Bar.Update(i);
            Bar.Print();
        }

        t->GetEntry(i);

        if(isDimuonSelected(t, muPtSelection, chargeSelection, isData, DataTrigger)){
            
            // WEIGHTS REMOVED FOR INITIAL FITTING CHECKS
            weight = 1;
            
            if(!isData && weightMC){
                weight*=10000; // to avoid small weight problems
                //weight *= t->MuMuWeight;
                //float z = t->mumuPt / t->JetPT;
                //weight *= (weight_histo->GetBinContent(weight_histo->FindBin(z))); 
                weight *= t->EventWeight;
                
                //cout << "weight: " << weight << endl;
            }
            if(isData && DataTrigger == 60){
                weight *= 6.338; // so that we can easily hadd the two trigger batches
            }
            if(isData && DataTrigger == 40){ /// BLOCk TO FACILITATE LOWEG SAMPLE
                if(t->HLT_HIAK4PFJet60_v1) {
                    weight *= 1; // to account for prescale of 60 trigger within 40 trigger sample
                }
                else{
                    weight *= 33.910;
                } 
            }

            hInvMass->Fill(t->JetPT, t->mumuMass, weight);
            hDCAProductSig->Fill(t->JetPT, log10(abs(t->muDiDxy1Dxy2 / t->muDiDxy1Dxy2Err)), weight);
            hmuDR->Fill(t->JetPT, t->muDR, weight);
            hmumuPt->Fill(t->JetPT, t->mumuPt, weight);
            hmumuZ->Fill(t->JetPT, t->mumuPt / t->JetPT, weight);
            hCharges->Fill(t->JetPT, t->muCharge1 * t->muCharge2, weight);
            ntDimuon->Fill(t->mumuMass, log10(abs(t->muDiDxy1Dxy2 / t->muDiDxy1Dxy2Err)), t->muDR, t->mumuPt, t->mumuPt / t->JetPT, t->muCharge1 * t->muCharge2, t->JetPT, weight);

            if(isData){continue;} // ONLY MAKE TEMPLATES WITH MC 

            int flavorclass = getFlavorClass(t->NbHad, t->NcHad);
            hInvMass_flavors[flavorclass]->Fill(t->JetPT, t->mumuMass, weight);
            hmuDCAProductSig_flavors[flavorclass]->Fill(t->JetPT, log10(abs(t->muDiDxy1Dxy2 / t->muDiDxy1Dxy2Err)), weight);
            hmuDR_flavors[flavorclass]->Fill(t->JetPT, t->muDR, weight);
            hmumuPt_flavors[flavorclass]->Fill(t->JetPT, t->mumuPt, weight);
            hmumuZ_flavors[flavorclass]->Fill(t->JetPT, t->mumuPt / t->JetPT, weight);
            hCharges_flavors[flavorclass]->Fill(t->JetPT, t->muCharge1 * t->muCharge2, weight);
            nt_flavors[flavorclass]->Fill(t->mumuMass, log10(abs(t->muDiDxy1Dxy2 / t->muDiDxy1Dxy2Err)), t->muDR, t->mumuPt, t->mumuPt / t->JetPT, t->muCharge1 * t->muCharge2, t->JetPT, weight);

        }
        
    }
    cout << " END OF JET LOOP: SAVING OUTPUTS TO FILE" << endl;

    outFile->cd();
    hInvMass->Write();
    hDCAProductSig->Write();
    hmuDR->Write();
    hmumuPt->Write();
    hmumuZ->Write();
    hCharges->Write();
    ntDimuon->Write();
    if(!isData){
        for(int i = 0; i < 6; i++) {
            hInvMass_flavors[i]->Write();
            hmuDCAProductSig_flavors[i]->Write();
            hmuDR_flavors[i]->Write();
            hmumuPt_flavors[i]->Write();
            hmumuZ_flavors[i]->Write();
            hCharges_flavors[i]->Write();
            nt_flavors[i]->Write();
        }
    }
    
    // SAVE COMMAND LINE PARAMS
    TNamed paramFile("InputFile", file.c_str());
    paramFile.Write();
    TNamed paramIsData("IsData", isData ? "true" : "false");
    paramIsData.Write();
    TNamed paramCharge("chargeSelection", std::to_string(chargeSelection).c_str());
    paramCharge.Write();
    TNamed paramMuPt("muPtSelection", std::to_string(muPtSelection).c_str());
    paramMuPt.Write();
    TNamed paramDataTrigger("DataTrigger", std::to_string(DataTrigger).c_str());
    paramDataTrigger.Write();
    TNamed paramWeightMC("weightMC", weightMC ? "true" : "false");
    paramWeightMC.Write();
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
            c1->Write(("mass_incl_pt"+to_string((int)ptMin)+"_"+to_string((int)ptMax)).c_str());
            delete c1;
            
            // DCA projection
            TCanvas* c2 = new TCanvas(Form("c_dca_incl_pt%.0f_%.0f", ptMin, ptMax), "", 800, 600);
            TH1D* h_dca = hDCAProductSig->ProjectionY(Form("h_dca_incl_pt%.0f_%.0f", ptMin, ptMax), iBin, iBin);
            h_dca->SetLineColor(kBlack);
            h_dca->SetLineWidth(2);
            h_dca->SetTitle(Form("DCA Product Sig (%.0f < p_{T} < %.0f GeV);log_{10}(|DCA_{1}#timesDCA_{2}|/#sigma);Entries", ptMin, ptMax));
            h_dca->Draw("HIST");
            c2->SaveAs(Form("plots/dca_incl_pt%.0f_%.0f.pdf", ptMin, ptMax));
            c2->Write(("dca_incl_pt"+to_string((int)ptMin)+"_"+to_string((int)ptMax)).c_str());
            delete c2;
            
            // DR projection
            TCanvas* c3 = new TCanvas(Form("c_dr_incl_pt%.0f_%.0f", ptMin, ptMax), "", 800, 600);
            TH1D* h_dr = hmuDR->ProjectionY(Form("h_dr_incl_pt%.0f_%.0f", ptMin, ptMax), iBin, iBin);
            h_dr->SetLineColor(kBlack);
            h_dr->SetLineWidth(2);
            h_dr->SetTitle(Form("#mu#mu #DeltaR (%.0f < p_{T} < %.0f GeV);#DeltaR(#mu,#mu);Entries", ptMin, ptMax));
            h_dr->Draw("HIST");
            c3->SaveAs(Form("plots/dr_incl_pt%.0f_%.0f.pdf", ptMin, ptMax));
            c3->Write(("dr_incl_pt"+to_string((int)ptMin)+"_"+to_string((int)ptMax)).c_str());
            delete c3;

            // mumuPt projection
            TCanvas* c4 = new TCanvas(Form("c_mumuPt_incl_pt%.0f_%.0f", ptMin, ptMax), "", 800, 600);
            TH1D* h_mumuPt = hmumuPt->ProjectionY(Form("h_mumuPt_incl_pt%.0f_%.0f", ptMin, ptMax), iBin, iBin);
            h_mumuPt->SetLineColor(kBlack);
            h_mumuPt->SetLineWidth(2);
            h_mumuPt->SetTitle(Form("Dimuon p_{T} (%.0f < p_{T} < %.0f GeV);p_{T,#mu#mu} [GeV];Entries", ptMin, ptMax));
            h_mumuPt->Draw("HIST");
            c4->SaveAs(Form("plots/mumuPt_incl_pt%.0f_%.0f.pdf", ptMin, ptMax));
            c4->Write(("mumuPt_incl_pt"+to_string((int)ptMin)+"_"+to_string((int)ptMax)).c_str());
            delete c4;

            // mumuZ projection
            TCanvas* c6 = new TCanvas(Form("c_mumuZ_incl_pt%.0f_%.0f", ptMin, ptMax), "", 800, 600);
            TH1D* h_mumuZ = hmumuZ->ProjectionY(Form("h_mumuZ_incl_pt%.0f_%.0f", ptMin, ptMax), iBin, iBin);
            h_mumuZ->SetLineColor(kBlack);
            h_mumuZ->SetLineWidth(2);
            h_mumuZ->SetTitle(Form("Dimuon Rapidity (%.0f < p_{T} < %.0f GeV);y_{#mu#mu};Entries", ptMin, ptMax));
            h_mumuZ->Draw("HIST");
            c6->SaveAs(Form("plots/mumuZ_incl_pt%.0f_%.0f.pdf", ptMin, ptMax));
            c6->Write(("mumuZ_incl_pt"+to_string((int)ptMin)+"_"+to_string((int)ptMax)).c_str());
            delete c6;  

            // Charges
            TCanvas* c5 = new TCanvas(Form("c_charges_incl_pt%.0f_%.0f", ptMin, ptMax), "", 800, 600);
            TH1D* h_charges = hCharges->ProjectionY(Form("h_charges_incl_pt%.0f_%.0f", ptMin, ptMax), iBin, iBin);
            h_charges->SetLineColor(kBlack);
            h_charges->SetLineWidth(2);
            h_charges->SetTitle(Form("Dimuon Charge Product (%.0f < p_{T} < %.0f GeV);Charge Product;Entries", ptMin, ptMax));
            h_charges->Draw("HIST");
            c5->SaveAs(Form("plots/charges_incl_pt%.0f_%.0f.pdf", ptMin, ptMax));
            c5->Write(("charges_incl_pt"+to_string((int)ptMin)+"_"+to_string((int)ptMax)).c_str());
            delete c5;
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
                c_mass_overlay->Write(("mass_overlay_pt"+to_string((int)ptMin)+"_"+to_string((int)ptMax)).c_str());
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
                c_dca_overlay->Write(("dca_overlay_pt"+to_string((int)ptMin)+"_"+to_string((int)ptMax)).c_str());
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
                c_dr_overlay->Write(("dr_overlay_pt"+to_string((int)ptMin)+"_"+to_string((int)ptMax)).c_str());
                delete c_dr_overlay;

                // mumuPt overlay
                TCanvas* c_mumuPt_overlay = new TCanvas(Form("c_mumuPt_overlay_pt%.0f_%.0f", ptMin, ptMax), "", 800, 600);
                TH1D* h_mumuPt_incl = hmumuPt->ProjectionY(Form("h_mumuPt_incl_pt%.0f_%.0f", ptMin, ptMax), iBin, iBin);
                h_mumuPt_incl->SetLineColor(colors[0]);
                h_mumuPt_incl->SetLineWidth(2);
                h_mumuPt_incl->SetStats(0);
                h_mumuPt_incl->SetTitle(Form("Dimuon p_{T} (%.0f < p_{T} < %.0f GeV);p_{T,#mu#mu} [GeV];Entries", ptMin, ptMax));
                h_mumuPt_incl->Draw("HIST");
                
                TLegend* leg_mumuPt = new TLegend(0.65, 0.45, 0.88, 0.88);
                leg_mumuPt->AddEntry(h_mumuPt_incl, "Inclusive", "l");

                vector<TH1D*> h_mumuPt_flavors;
                for(int i = 0; i < 6; i++) {
                    TH1D* h = hmumuPt_flavors[i]->ProjectionY(Form("h_mumuPt_%s_pt%.0f_%.0f", flavorNames[i].c_str(), ptMin, ptMax), iBin, iBin);
                    h->SetLineColor(colors[i+1]);
                    h->SetLineWidth(2);
                    h->Draw("HIST SAME");
                    h_mumuPt_flavors.push_back(h);  
                    leg_mumuPt->AddEntry(h, flavorNames[i].c_str(), "l");
                }
                leg_mumuPt->Draw();
                c_mumuPt_overlay->SaveAs(Form("plots/mumuPt_overlay_pt%.0f_%.0f.pdf", ptMin, ptMax));
                c_mumuPt_overlay->Write(("mumuPt_overlay_pt"+to_string((int)ptMin)+"_"+to_string((int)ptMax)).c_str());
                delete c_mumuPt_overlay;

                // mumuZ overlay
                TCanvas* c_mumuZ_overlay = new TCanvas(Form("c_mumuZ_overlay_pt%.0f_%.0f", ptMin, ptMax), "", 800, 600);
                TH1D* h_mumuZ_incl = hmumuZ->ProjectionY(Form("h_mumuZ_incl_pt%.0f_%.0f", ptMin, ptMax), iBin, iBin);
                h_mumuZ_incl->SetLineColor(colors[0]);
                h_mumuZ_incl->SetLineWidth(2);
                h_mumuZ_incl->SetStats(0);
                h_mumuZ_incl->SetTitle(Form("Dimuon Rapidity (%.0f < p_{T} < %.0f GeV);y_{#mu#mu};Entries", ptMin, ptMax));
                h_mumuZ_incl->Draw("HIST");

                TLegend* leg_mumuZ = new TLegend(0.65, 0.45, 0.88, 0.88);
                leg_mumuZ->AddEntry(h_mumuZ_incl, "Inclusive", "l");

                vector<TH1D*> h_mumuZ_flavors;
                for(int i = 0; i < 6; i++) {
                    TH1D* h = hmumuZ_flavors[i]->ProjectionY(Form("h_mumuZ_%s_pt%.0f_%.0f", flavorNames[i].c_str(), ptMin, ptMax), iBin, iBin);
                    h->SetLineColor(colors[i+1]);
                    h->SetLineWidth(2);
                    h->Draw("HIST SAME");
                    h_mumuZ_flavors.push_back(h);  
                    leg_mumuZ->AddEntry(h, flavorNames[i].c_str(), "l");
                }
                leg_mumuZ->Draw();
                c_mumuZ_overlay->SaveAs(Form("plots/mumuZ_overlay_pt%.0f_%.0f.pdf", ptMin, ptMax));
                c_mumuZ_overlay->Write(("mumuZ_overlay_pt"+to_string((int)ptMin)+"_"+to_string((int)ptMax)).c_str());
                delete c_mumuZ_overlay;

                // Charge overlay
                TCanvas* c_charge_overlay = new TCanvas(Form("c_charge_overlay_pt%.0f_%.0f", ptMin, ptMax), "", 800, 600);
                TH1D* h_charge_incl = hCharges->ProjectionY(Form("h_charge_incl_pt%.0f_%.0f", ptMin, ptMax), iBin, iBin);
                h_charge_incl->SetLineColor(colors[0]);
                h_charge_incl->SetLineWidth(2);
                h_charge_incl->SetStats(0);
                h_charge_incl->SetTitle(Form("Dimuon Charge Product (%.0f < p_{T} < %.0f GeV);Charge Product;Entries", ptMin, ptMax));
                h_charge_incl->Draw("HIST");

                TLegend* leg_charge = new TLegend(0.65, 0.45, 0.88, 0.88);
                leg_charge->AddEntry(h_charge_incl, "Inclusive", "l");

                vector<TH1D*> h_charge_flavors;
                for(int i = 0; i < 6; i++) {
                    TH1D* h = hCharges_flavors[i]->ProjectionY(Form("h_charge_%s_pt%.0f_%.0f", flavorNames[i].c_str(), ptMin, ptMax), iBin, iBin);
                    h->SetLineColor(colors[i+1]);
                    h->SetLineWidth(2);
                    h->Draw("HIST SAME");
                    h_charge_flavors.push_back(h);  
                    leg_charge->AddEntry(h, flavorNames[i].c_str(), "l");
                }
                leg_charge->Draw();
                c_charge_overlay->SaveAs(Form("plots/charge_overlay_pt%.0f_%.0f.pdf", ptMin, ptMax));
                c_charge_overlay->Write(("charge_overlay_pt"+to_string((int)ptMin)+"_"+to_string((int)ptMax)).c_str());
                delete c_charge_overlay;

            }
        }

    }


    
    outFile->Close();
}