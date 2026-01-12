///////////////////////////////////////////////////////////////////////
//        EXTRACT DIMUON YIELD WITHOUT LF COMPONENT + J/PSI          //
///////////////////////////////////////////////////////////////////////

#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TLine.h>
#include <TStyle.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TTreeFormula.h>

#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooGaussian.h>
#include <RooPlot.h>
#include <RooHist.h>
#include <RooPolynomial.h>
#include <RooAddPdf.h>
#include <RooKeysPdf.h>
#include <RooArgList.h>
#include <RooFitResult.h>

#include <iostream>
#include <vector>
#include <utility>
#include <tuple>

using namespace std;
#include "CommandLine.h" 
#include "DimuonMessenger.h"
#include "Messenger.h"   
#include "ProgressBar.h" 

using namespace std;

tuple<float, float, float, float> LFYield_InvMass(TNtuple* data_nt, TNtuple* nt_uds, TNtuple* nt_other, TNtuple* nt_c, TNtuple* nt_cc, TNtuple* nt_b, TNtuple* nt_bb, float Jetptmin, float Jetptmax, TDirectory* plotDir = nullptr){

    //INVERSE MASS FITTING METHOD TO EXTRACT LF YIELD
    RooRealVar mass("mumuMass", "Dimuon mass [GeV]", 0, 10);
    RooRealVar jetpt("JetPT", "Jet pT", 0, 1000);
    RooRealVar weight("weight", "weight", 0, 1e10);
    RooArgSet vars(mass, jetpt, weight);
    mass.setRange("fitRange", 0, 10); // Fit over full mass range
    mass.setRange("normRange", 0, 10); // Normalize over full mass range

    // DATA
    RooDataSet data("data", "data", data_nt, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");

    // LIGHT FLAVOR TEMPLATE
    RooDataSet templateLF("templateLF", "Light flavor", nt_uds, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");

    // HEAVY FLAVOR TEMPLATE 
    RooDataSet templateHF_other("tmp_other", "", nt_other, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet templateHF_c("tmp_c", "", nt_c, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet templateHF_cc("tmp_cc", "", nt_cc, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet templateHF_b("tmp_b", "", nt_b, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet templateHF_bb("tmp_bb", "", nt_bb, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");

    // APPEND HF TEMPLATES
    RooDataSet templateHF("templateHF", "Heavy flavor", mass);
    templateHF.append(templateHF_other);
    templateHF.append(templateHF_c);
    templateHF.append(templateHF_cc);
    templateHF.append(templateHF_b);
    templateHF.append(templateHF_bb);   

    // PDFs
    RooKeysPdf lfPdf("lfPdf", "Light flavor PDF", mass, templateLF, RooKeysPdf::MirrorLeft, 0.6);
    RooKeysPdf hfPdf("hfPdf", "Heavy flavor PDF", mass, templateHF, RooKeysPdf::MirrorLeft, 0.6);
    
    // YIELDs
    double nTotal = data.sumEntries();
    RooRealVar nLF("nLF", "N light flavor", nTotal/2, 0, nTotal);
    RooRealVar nHF("nHF", "N heavy flavor", nTotal/2, 0, nTotal);
    RooAddPdf model("model", "LF+HF", RooArgList(lfPdf, hfPdf), RooArgList(nLF, nHF));

    // Fit to data in restricted range, but normalize over full range
    RooFitResult* result = model.fitTo(data, 
                                        RooFit::Range("fitRange"),      // Fit only here
                                       RooFit::NormRange("normRange"),  // But yields are for full range
                                       RooFit::Save());

    cout << "Light flavor yield: " << nLF.getVal() << " +/- " << nLF.getError() << endl;
    cout << "Heavy flavor yield: " << nHF.getVal() << " +/- " << nHF.getError() << endl;

    // CREATE FIT PLOT
    if(plotDir != nullptr) {
        plotDir->cd();
        TCanvas* c = new TCanvas(Form("LFFit_pt%.0f_%.0f", Jetptmin, Jetptmax), "", 800, 800);
        c->Divide(1,2); 

        // Top pad: main fit
        c->cd(1);
        gPad->SetPad(0.0, 0.3, 1.0, 1.0);
        gPad->SetBottomMargin(0.02);
        RooPlot* frame = mass.frame(RooFit::Title(Form("LF/HF Fit (%.0f < p_{T} < %.0f GeV)", Jetptmin, Jetptmax)));
        data.plotOn(frame, RooFit::Name("data"));
        model.plotOn(frame, RooFit::Range("fitRange"), RooFit::NormRange("normRange"), RooFit::Name("model"));
        model.plotOn(frame, RooFit::Components(lfPdf), RooFit::LineStyle(kDashed), RooFit::LineColor(kRed), RooFit::Range("normRange"), RooFit::NormRange("normRange"), RooFit::Name("LF"));
        model.plotOn(frame, RooFit::Components(hfPdf), RooFit::LineStyle(kDashed), RooFit::LineColor(kGreen+2), RooFit::Range("normRange"), RooFit::NormRange("normRange"), RooFit::Name("HF"));
        frame->Draw();

        // Add legend
        TLegend* leg = new TLegend(0.55, 0.65, 0.80, 0.88);
        leg->AddEntry(frame->findObject("data"), "Data", "lep");
        leg->AddEntry(frame->findObject("model"), "Total Fit (LF+HF)", "l");
        leg->AddEntry(frame->findObject("LF"), "Light Flavor", "l");
        leg->AddEntry(frame->findObject("HF"), "Heavy Flavor", "l");
        leg->Draw();

        // Bottom pad: pulls
        c->cd(2);
        gPad->SetPad(0.0, 0.0, 1.0, 0.3);
        gPad->SetTopMargin(0.02);
        gPad->SetBottomMargin(0.3);
        
        RooPlot* framePull = mass.frame(RooFit::Title(""));
        RooHist* hpull = frame->pullHist("data", "model");
        hpull->SetMarkerStyle(20);
        hpull->SetMarkerSize(0.8);
        framePull->addPlotable(hpull, "P0");
        framePull->SetMinimum(-5);
        framePull->SetMaximum(5);
        framePull->GetYaxis()->SetTitle("Pull");
        framePull->GetYaxis()->SetTitleSize(0.12);
        framePull->GetYaxis()->SetLabelSize(0.10);
        framePull->GetYaxis()->SetTitleOffset(0.35);
        framePull->GetYaxis()->SetNdivisions(505);
        framePull->GetXaxis()->SetTitleSize(0.12);
        framePull->GetXaxis()->SetLabelSize(0.10);
        framePull->GetXaxis()->SetTitleOffset(1.0);
        framePull->Draw();
        
        // Add horizontal lines at 0, +/-3
        TLine* line0 = new TLine(framePull->GetXaxis()->GetXmin(), 0, framePull->GetXaxis()->GetXmax(), 0);
        line0->SetLineColor(kBlack);
        line0->SetLineStyle(2);
        line0->Draw();
        
        TLine* line3 = new TLine(framePull->GetXaxis()->GetXmin(), 3, framePull->GetXaxis()->GetXmax(), 3);
        line3->SetLineColor(kRed);
        line3->SetLineStyle(2);
        line3->Draw();
        
        TLine* lineM3 = new TLine(framePull->GetXaxis()->GetXmin(), -3, framePull->GetXaxis()->GetXmax(), -3);
        lineM3->SetLineColor(kRed);
        lineM3->SetLineStyle(2);
        lineM3->Draw();

        c->Write();
        c->SaveAs(Form("plots/LFFit_invmass_pt%.0f_%.0f.pdf", Jetptmin, Jetptmax));
        delete leg;
        delete frame;
        delete framePull;
        delete c;
    } 
    return make_tuple(nLF.getVal(), nLF.getError(), nHF.getVal(), nHF.getError());
}


tuple<float, float, float, float> LFYield_DCA(TNtuple* data_nt, TNtuple* nt_uds, TNtuple* nt_other, TNtuple* nt_c, TNtuple* nt_cc, TNtuple* nt_b, TNtuple* nt_bb, float Jetptmin, float Jetptmax, TDirectory* plotDir = nullptr){
    
    RooRealVar fitVar("muDiDxy1Dxy2Sig", "muDiDxy1Dxy2Sig", -3, 4);
    RooRealVar jetpt("JetPT", "Jet pT", 0, 1000);
    RooRealVar weight("weight", "weight", 0, 1e10);
    RooArgSet vars(fitVar, jetpt, weight);
    fitVar.setRange("fitRange", -3, 4);
    fitVar.setRange("normRange", -3, 4);
    
    // DATA
    RooDataSet data("data", "data", data_nt, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    
    // LIGHT FLAVOR TEMPLATE
    RooDataSet templateLF("templateLF", "Light flavor", nt_uds, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    
    // HEAVY FLAVOR TEMPLATE 
    RooDataSet templateHF_other("tmp_other", "", nt_other, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet templateHF_c("tmp_c", "", nt_c, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet templateHF_cc("tmp_cc", "", nt_cc, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet templateHF_b("tmp_b", "", nt_b, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet templateHF_bb("tmp_bb", "", nt_bb, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    
    // APPEND HF TEMPLATES
    RooDataSet templateHF("templateHF", "Heavy flavor", fitVar);
    templateHF.append(templateHF_other);
    templateHF.append(templateHF_c);
    templateHF.append(templateHF_cc);
    templateHF.append(templateHF_b);
    templateHF.append(templateHF_bb);
    
    // PDFs
    RooKeysPdf lfPdf("lfPdf", "Light flavor PDF", fitVar, templateLF, RooKeysPdf::MirrorLeft, 1.0);
    RooKeysPdf hfPdf("hfPdf", "Heavy flavor PDF", fitVar, templateHF, RooKeysPdf::MirrorLeft, 1.0);
    
    // YIELDs
    double nTotal = data.sumEntries();
    RooRealVar nLF("nLF", "N light flavor", nTotal/2, 0, nTotal);
    RooRealVar nHF("nHF", "N heavy flavor", nTotal/2, 0, nTotal);
    RooAddPdf model("model", "LF+HF", RooArgList(lfPdf, hfPdf), RooArgList(nLF, nHF));
    
    // Fit to data in restricted range, but normalize over full range
    RooFitResult* result = model.fitTo(data, 
                                        RooFit::Range("fitRange"),      // Fit only here
                                       RooFit::NormRange("normRange"),  // But yields are for full range
                                       RooFit::Save());
    
    cout << "Light flavor yield: " << nLF.getVal() << " +/- " << nLF.getError() << endl;
    cout << "Heavy flavor yield: " << nHF.getVal() << " +/- " << nHF.getError() << endl;
    
    // CREATE FIT PLOT
    if(plotDir != nullptr) {
        plotDir->cd();
        TCanvas* c = new TCanvas(Form("LFFit_pt%.0f_%.0f", Jetptmin, Jetptmax), "", 800, 800);
        c->Divide(1,2);
        
        // Top pad: main fit
        c->cd(1);
        gPad->SetPad(0.0, 0.3, 1.0, 1.0);
        gPad->SetBottomMargin(0.02);
        
        RooPlot* frame = fitVar.frame(RooFit::Title(Form("LF/HF Fit (%.0f < p_{T} < %.0f GeV)", Jetptmin, Jetptmax)));
        data.plotOn(frame, RooFit::Name("data"));
        model.plotOn(frame, RooFit::Range("fitRange"), RooFit::NormRange("normRange"), RooFit::Name("model"));
        model.plotOn(frame, RooFit::Components(lfPdf), RooFit::LineStyle(kDashed), RooFit::LineColor(kRed), RooFit::Range("normRange"), RooFit::NormRange("normRange"), RooFit::Name("LF"));
        model.plotOn(frame, RooFit::Components(hfPdf), RooFit::LineStyle(kDashed), RooFit::LineColor(kGreen), RooFit::Range("normRange"), RooFit::NormRange("normRange"), RooFit::Name("HF"));
        
        // Plot full model over entire range (dotted)
        model.plotOn(frame, RooFit::LineStyle(kDotted), RooFit::LineColor(kBlue), RooFit::Range("normRange"), RooFit::NormRange("normRange"), RooFit::Name("model_full"));
        
        frame->GetXaxis()->SetLabelSize(0);
        frame->GetXaxis()->SetTitleSize(0);
        frame->Draw();
        
        // Add legend
        TLegend* leg = new TLegend(0.65, 0.65, 0.88, 0.88);
        leg->AddEntry(frame->findObject("data"), "Data", "lep");
        leg->AddEntry(frame->findObject("model"), "Total Fit", "l");
        leg->AddEntry(frame->findObject("LF"), "LF (scaled)", "l");
        leg->AddEntry(frame->findObject("HF"), "HF (scaled)", "l");
        leg->AddEntry(frame->findObject("model_full"), "Full PDF (extrapolated)", "l");
        leg->Draw();
        
        // Bottom pad: residuals/pulls
        c->cd(2);
        gPad->SetPad(0.0, 0.0, 1.0, 0.3);
        gPad->SetTopMargin(0.02);
        gPad->SetBottomMargin(0.3);
        
        RooPlot* framePull = fitVar.frame(RooFit::Title(""));
        RooHist* hpull = frame->pullHist("data", "model");
        hpull->SetMarkerStyle(20);
        hpull->SetMarkerSize(0.8);
        framePull->addPlotable(hpull, "P0");
        framePull->SetMinimum(-5);
        framePull->SetMaximum(5);
        framePull->GetYaxis()->SetTitle("Pull");
        framePull->GetYaxis()->SetTitleSize(0.12);
        framePull->GetYaxis()->SetLabelSize(0.10);
        framePull->GetYaxis()->SetTitleOffset(0.35);
        framePull->GetYaxis()->SetNdivisions(505);
        framePull->GetXaxis()->SetTitleSize(0.12);
        framePull->GetXaxis()->SetLabelSize(0.10);
        framePull->GetXaxis()->SetTitleOffset(1.0);
        framePull->Draw("lp");
        
        // Add horizontal lines at 0, +/-3
        TLine* line0 = new TLine(framePull->GetXaxis()->GetXmin(), 0, framePull->GetXaxis()->GetXmax(), 0);
        line0->SetLineColor(kBlack);
        line0->SetLineStyle(2);
        line0->Draw();
        
        TLine* line3 = new TLine(framePull->GetXaxis()->GetXmin(), 3, framePull->GetXaxis()->GetXmax(), 3);
        line3->SetLineColor(kRed);
        line3->SetLineStyle(2);
        line3->Draw();
        
        TLine* lineM3 = new TLine(framePull->GetXaxis()->GetXmin(), -3, framePull->GetXaxis()->GetXmax(), -3);
        lineM3->SetLineColor(kRed);
        lineM3->SetLineStyle(2);
        lineM3->Draw();
        
        c->Write();
        c->SaveAs(Form("plots/LFFit_DCA_pt%.0f_%.0f.pdf", Jetptmin, Jetptmax));
        delete leg;
        delete frame;
        delete framePull;
        delete c;
    }
    
    return make_tuple(nLF.getVal(), nLF.getError(), nHF.getVal(), nHF.getError());
}

int main(int argc, char *argv[]) {
    gStyle->SetOptStat(0);

    // INPUTS
    cout << "Beginning Acceptance x Efficiency" << endl;
    CommandLine CL(argc, argv);
    string file = CL.Get("Input"); // DISTRIBUTIONS TO BE FITTED (MC OR DATA)
    string output = CL.Get("Output"); 
    string templates = CL.Get("Templates"); // TEMPLATES TO HELP THE FITTING (MC)
    vector<double> ptBins = CL.GetDoubleVector("ptBins");
    bool doLF_DCA = CL.GetBool("doLF_DCA");
    bool doLF_invMass = CL.GetBool("doLF_invMass");
    bool makeplots = CL.GetBool("makeplots");

    // IMPORT TREE
    TFile* input = TFile::Open(file.c_str());
    TNtuple* nt = (TNtuple*)input->Get("ntDimuon");

    TFile* templatesFile = TFile::Open(templates.c_str());
    TNtuple* nt_other = (TNtuple*)templatesFile->Get("nt_other");
    TNtuple* nt_uds = (TNtuple*)templatesFile->Get("nt_uds");
    TNtuple* nt_c = (TNtuple*)templatesFile->Get("nt_c");
    TNtuple* nt_b = (TNtuple*)templatesFile->Get("nt_b");
    TNtuple* nt_cc = (TNtuple*)templatesFile->Get("nt_cc");
    TNtuple* nt_bb = (TNtuple*)templatesFile->Get("nt_bb");

    // DECLARE HISTOGRAMS
    TFile* outputFile = new TFile(output.c_str(), "RECREATE");
    outputFile->cd();
    TH1D* LightYields_DCA = new TH1D("LightYields_DCA", "Light Flavor Yields (DCA)", ptBins.size()-1, &ptBins[0]);
    TH1D* HeavyYields_DCA = new TH1D("HeavyYields_DCA", "Heavy Flavor Yields (DCA)", ptBins.size()-1, &ptBins[0]);
    TH1D* Fractions_DCA = new TH1D("Fractions_DCA", "HF Fraction (DCA)", ptBins.size()-1, &ptBins[0]);
    TH1D* FullYields_DCA = new TH1D("FullYields_DCA", "Total Yields (DCA)", ptBins.size()-1, &ptBins[0]);
    TH1D* LightYields_InvMass = new TH1D("LightYields_InvMass", "Light Flavor Yields (inv mass)", ptBins.size()-1, &ptBins[0]);
    TH1D* HeavyYields_InvMass = new TH1D("HeavyYields_InvMass", "Heavy Flavor Yields (inv mass)", ptBins.size()-1, &ptBins[0]);
    TH1D* Fractions_InvMass = new TH1D("Fractions_InvMass", "HF Fraction (inv mass)", ptBins.size()-1, &ptBins[0]);
    TH1D* FullYields_InvMass = new TH1D("FullYields_InvMass", "Total Yields (inv mass)", ptBins.size()-1, &ptBins[0]);
    
    // CREATE PLOTS DIRECTORY
    TDirectory* plotDir = nullptr;
    if(makeplots) {
        plotDir = outputFile->mkdir("plots");
    }

    float LightYield = 0;
    float LightYieldError = 0;
    float HeavyYield = 0;
    float HeavyYieldError = 0;
    for(int i = 0; i < ptBins.size()-1; i++){
        
        float ptMin = ptBins[i];
        float ptMax = ptBins[i+1];
        
        // TOTAL YIELD
        RooRealVar mass("mumuMass", "mass", 0, 10);
        RooRealVar jetpt("JetPT", "Jet pT", 0, 1000);
        RooRealVar weight("weight", "weight", 0, 1e10);
        RooArgSet vars(mass, jetpt, weight);
        RooDataSet dataBin("dataBin", "data", nt, vars, 
                          Form("JetPT < %f && JetPT >= %f", ptMax, ptMin), "weight");
        float totalYield = dataBin.sumEntries();
        
        // LF
        if(doLF_DCA) {
            auto lfResult = LFYield_DCA(nt, nt_uds, nt_other, nt_c, nt_cc, nt_b, nt_bb, ptMin, ptMax, plotDir);
            tie(LightYield, LightYieldError, HeavyYield, HeavyYieldError) = lfResult;
            LightYields_DCA->SetBinContent(i+1, LightYield);
            LightYields_DCA->SetBinError(i+1, LightYieldError);
            HeavyYields_DCA->SetBinContent(i+1, HeavyYield);
            HeavyYields_DCA->SetBinError(i+1, HeavyYieldError);
            Fractions_DCA->SetBinContent(i+1, HeavyYield / (LightYield + HeavyYield));
            Fractions_DCA->SetBinError(i+1, sqrt( pow((1/(LightYield + HeavyYield) - HeavyYield/((HeavyYield + LightYield)*(HeavyYield + LightYield))),2)*HeavyYieldError*HeavyYieldError + 
                                                  pow(( -1.0*HeavyYield/((HeavyYield + LightYield)*(HeavyYield + LightYield))),2)*LightYieldError*LightYieldError ));
            FullYields_DCA->SetBinContent(i+1, LightYield + HeavyYield);
            FullYields_DCA->SetBinError(i+1, sqrt(LightYieldError*LightYieldError + HeavyYieldError*HeavyYieldError)); // NOTE THESE UNCERTAINTIES ARE TREATED AS UNCORRELATED. THIS IS NOT TRUE AT ALL!
        }

        // LF VIA INVMASS METHOD
        if(doLF_invMass) {
            auto lfResultMass = LFYield_InvMass(nt, nt_uds, nt_other, nt_c, nt_cc, nt_b, nt_bb, ptMin, ptMax, plotDir);
            tie(LightYield, LightYieldError, HeavyYield, HeavyYieldError) = lfResultMass;
            LightYields_InvMass->SetBinContent(i+1, LightYield);
            LightYields_InvMass->SetBinError(i+1, LightYieldError);
            HeavyYields_InvMass->SetBinContent(i+1, HeavyYield);
            HeavyYields_InvMass->SetBinError(i+1, HeavyYieldError);
            Fractions_InvMass->SetBinContent(i+1, HeavyYield / (LightYield + HeavyYield));
            Fractions_InvMass->SetBinError(i+1, sqrt( pow((1/(LightYield + HeavyYield) - HeavyYield/((HeavyYield + LightYield)*(HeavyYield + LightYield))),2)*HeavyYieldError*HeavyYieldError + 
                                                  pow(( -1.0*HeavyYield/((HeavyYield + LightYield)*(HeavyYield + LightYield))),2)*LightYieldError*LightYieldError ));
            FullYields_InvMass->SetBinContent(i+1, LightYield + HeavyYield);
            FullYields_InvMass->SetBinError(i+1, sqrt(LightYieldError*LightYieldError + HeavyYieldError*HeavyYieldError)); // NOTE THESE UNCERTAINTIES ARE TREATED AS UNCORRELATED. THIS IS NOT TRUE AT ALL!
        }
        
        
    }

    // WRITE TO FILE
    outputFile->cd();
        
    if(doLF_DCA) {
        LightYields_DCA->Write();
        HeavyYields_DCA->Write();
        Fractions_DCA->Write();
        FullYields_DCA->Write();
    }
    if(doLF_invMass) {
        LightYields_InvMass->Write();
        HeavyYields_InvMass->Write();
        Fractions_InvMass->Write();
        FullYields_InvMass->Write();
    }

    // SAVE COMMAND LINE PARAMS
    TNamed paramFile("InputFile", file.c_str());
    paramFile.Write();
    TNamed paramTemplates("Templates", templates.c_str());
    paramTemplates.Write();
    TNamed paramDoLF_DCA("doLF_DCA", doLF_DCA ? "true" : "false");
    paramDoLF_DCA.Write();
    TNamed paramDoLFInvMass("doLF_invMass", doLF_invMass ? "true" : "false");
    paramDoLFInvMass.Write();
    TNamed paramMakePlots("makeplots", makeplots ? "true" : "false");
    paramMakePlots.Write();

    if(makeplots) {
        
        plotDir->cd();
        
        // Normalize yields by bin width
        if(doLF_DCA) {
            for(int i = 1; i <= LightYields_DCA->GetNbinsX(); i++) {
                double binWidth = LightYields_DCA->GetBinWidth(i);
                LightYields_DCA->SetBinContent(i, LightYields_DCA->GetBinContent(i) / binWidth);
                LightYields_DCA->SetBinError(i, LightYields_DCA->GetBinError(i) / binWidth);
                HeavyYields_DCA->SetBinContent(i, HeavyYields_DCA->GetBinContent(i) / binWidth);
                HeavyYields_DCA->SetBinError(i, HeavyYields_DCA->GetBinError(i) / binWidth);
                FullYields_DCA->SetBinContent(i, FullYields_DCA->GetBinContent(i) / binWidth);
                FullYields_DCA->SetBinError(i, FullYields_DCA->GetBinError(i) / binWidth);
            }
        }
        if(doLF_invMass) {
            for(int i = 1; i <= LightYields_InvMass->GetNbinsX(); i++) {
                double binWidth = LightYields_InvMass->GetBinWidth(i);
                LightYields_InvMass->SetBinContent(i, LightYields_InvMass->GetBinContent(i) / binWidth);
                LightYields_InvMass->SetBinError(i, LightYields_InvMass->GetBinError(i) / binWidth);
                HeavyYields_InvMass->SetBinContent(i, HeavyYields_InvMass->GetBinContent(i) / binWidth);
                HeavyYields_InvMass->SetBinError(i, HeavyYields_InvMass->GetBinError(i) / binWidth);
                FullYields_InvMass->SetBinContent(i, FullYields_InvMass->GetBinContent(i) / binWidth);
                FullYields_InvMass->SetBinError(i, FullYields_InvMass->GetBinError(i) / binWidth);
            }
        }
        
        TCanvas* c1 = new TCanvas("c1", "", 800, 600);
        TLegend* leg = new TLegend(0.6, 0.7, 0.88, 0.88);
        
        // Polish histograms
        if(doLF_DCA) {
            LightYields_DCA->SetMarkerStyle(20);
            LightYields_DCA->SetMarkerColor(kRed);
            LightYields_DCA->SetLineColor(kRed);
            HeavyYields_DCA->SetMarkerStyle(20);
            HeavyYields_DCA->SetMarkerColor(kRed);
            HeavyYields_DCA->SetLineColor(kRed);
            Fractions_DCA->SetMarkerStyle(20);
            Fractions_DCA->SetMarkerColor(kRed);
            Fractions_DCA->SetLineColor(kRed);
            FullYields_DCA->SetMarkerStyle(20);
            FullYields_DCA->SetMarkerColor(kRed);
            FullYields_DCA->SetLineColor(kRed);
        }
        if(doLF_invMass) {
            LightYields_InvMass->SetMarkerStyle(24);
            LightYields_InvMass->SetMarkerColor(kBlue);
            LightYields_InvMass->SetLineColor(kBlue);
            HeavyYields_InvMass->SetMarkerStyle(24);
            HeavyYields_InvMass->SetMarkerColor(kBlue);
            HeavyYields_InvMass->SetLineColor(kBlue);
            Fractions_InvMass->SetMarkerStyle(24);
            Fractions_InvMass->SetMarkerColor(kBlue);
            Fractions_InvMass->SetLineColor(kBlue);
            FullYields_InvMass->SetMarkerStyle(24);
            FullYields_InvMass->SetMarkerColor(kBlue);
            FullYields_InvMass->SetLineColor(kBlue);
        }
        
        // Light Flavor Yields
        if(doLF_DCA) {
            LightYields_DCA->SetTitle("Light Flavor Yields");
            LightYields_DCA->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
            LightYields_DCA->GetYaxis()->SetTitle("Light Flavor Yield");
            LightYields_DCA->Draw("E");
            leg->AddEntry(LightYields_DCA, "DCA Method", "lep");
        }
        if(doLF_invMass) {
            LightYields_InvMass->Draw("E SAME");
            leg->AddEntry(LightYields_InvMass, "Inv Mass Method", "lep");
        }
        if(doLF_DCA || doLF_invMass) {
            leg->Draw();
            c1->SaveAs("plots/LightFlavorYield.pdf");
            leg->Clear();
        }
        
        // Heavy Flavor Yields
        if(doLF_DCA) {
            HeavyYields_DCA->SetTitle("Heavy Flavor Yields");
            HeavyYields_DCA->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
            HeavyYields_DCA->GetYaxis()->SetTitle("Heavy Flavor Yield");
            HeavyYields_DCA->Draw("E");
            leg->AddEntry(HeavyYields_DCA, "DCA Method", "lep");
        }
        if(doLF_invMass) {
            HeavyYields_InvMass->Draw("E SAME");
            leg->AddEntry(HeavyYields_InvMass, "Inv Mass Method", "lep");
        }
        if(doLF_DCA || doLF_invMass) {
            leg->Draw();
            c1->SaveAs("plots/HeavyFlavorYield.pdf");
            leg->Clear();
        }
        
        // Heavy Flavor Fractions
        if(doLF_DCA) {
            Fractions_DCA->SetTitle("Heavy Flavor Fraction");
            Fractions_DCA->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
            Fractions_DCA->GetYaxis()->SetTitle("Heavy Flavor Fraction");
            Fractions_DCA->Draw("E");
            leg->AddEntry(Fractions_DCA, "DCA Method", "lep");
        }
        if(doLF_invMass) {
            Fractions_InvMass->Draw("E SAME");
            leg->AddEntry(Fractions_InvMass, "Inv Mass Method", "lep");
        }
        if(doLF_DCA || doLF_invMass) {
            leg->Draw();
            c1->SaveAs("plots/HeavyFlavorFraction.pdf");
            leg->Clear();
        }
        
        // Total Yields
        if(doLF_DCA) {
            FullYields_DCA->SetTitle("Total Yields");
            FullYields_DCA->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
            FullYields_DCA->GetYaxis()->SetTitle("Total Yield");
            FullYields_DCA->Draw("E");
            leg->AddEntry(FullYields_DCA, "DCA Method", "lep");
        }
        if(doLF_invMass) {
            FullYields_InvMass->Draw("E SAME");
            leg->AddEntry(FullYields_InvMass, "Inv Mass Method", "lep");
        }
        if(doLF_DCA || doLF_invMass) {
            leg->Draw();
            c1->SaveAs("plots/TotalYield.pdf");
        }
        
        delete leg;
        delete c1;
    }
    
    outputFile->Close();

}