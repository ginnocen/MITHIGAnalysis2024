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

float JpsiYield(TNtuple* nt, float Jetptmin, float Jetptmax, TDirectory* plotDir = nullptr){

    //ESTIMATE JPSI USING SIDEBANDS
    RooRealVar mass("mumuMass", "Dimuon mass [GeV]", 0, 15);
    RooRealVar jetpt("JetPT", "Jet pT", 0, 1000);
    RooRealVar weight("weight", "weight", 0, 1e10);
    RooArgSet vars(mass, jetpt, weight);
    RooDataSet data("data", "data", nt, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");

    mass.setRange("signal", 2.95, 3.25);
    mass.setRange("leftSB", 2.5, 2.90);
    mass.setRange("rightSB", 3.3, 3.7);

    double nSignal = data.sumEntries("", "signal");
    double nLeftSB = data.sumEntries("", "leftSB");
    double nRightSB = data.sumEntries("", "rightSB");
    double nBkgInSignal = (nLeftSB + nRightSB) * (0.375);

    //GAUSSIAN JPSI PEAK
    RooRealVar mean("mean", "J/psi mass", 3.1, 2.6, 3.7);
    RooRealVar sigma("sigma", "resolution", 0.09, 0.01, 0.2);
    RooRealVar nJpsi("nJpsi", "N J/psi", nSignal - nBkgInSignal, 0, nSignal);
    RooRealVar nBkg("nBkg", "N bkg", nBkgInSignal);
    nBkg.setConstant(true);  // Fixed from sideband estimate

    RooRealVar c0("c0", "constant", 1);
    RooPolynomial bkgPdf("bkgPdf", "background", mass, c0);
    RooGaussian jpsiPdf("jpsiPdf", "J/psi", mass, mean, sigma);
    RooAddPdf model("model", "jpsi+bkg", RooArgList(jpsiPdf, bkgPdf), RooArgList(nJpsi, nBkg));

    RooFitResult* result = model.fitTo(data, RooFit::Range("signal"), RooFit::Save());
    cout << "J/psi yield: " << nJpsi.getVal() << " +/- " << nJpsi.getError() << endl;
    cout << "Background yield: " << nBkg.getVal() << " (fixed)" << endl;
    cout << "Total: " << nJpsi.getVal() + nBkg.getVal() << endl;

    // CREATE FIT PLOT
    if(plotDir != nullptr) {
        plotDir->cd();
        TCanvas* c = new TCanvas(Form("JpsiFit_pt%.0f_%.0f", Jetptmin, Jetptmax), "", 800, 600);
        RooPlot* frame = mass.frame(RooFit::Title(Form("J/psi Fit (%.0f < p_{T} < %.0f GeV)", Jetptmin, Jetptmax)));
        data.plotOn(frame, RooFit::Name("data"));
        model.plotOn(frame, RooFit::Name("model"));
        model.plotOn(frame, RooFit::Components(jpsiPdf), RooFit::LineStyle(kDashed), RooFit::LineColor(kRed), RooFit::Name("jpsi"));
        model.plotOn(frame, RooFit::Components(bkgPdf), RooFit::LineStyle(kDashed), RooFit::LineColor(kGreen), RooFit::Name("bkg"));
        
        frame->Draw();
        
        // Add legend
        TLegend* leg = new TLegend(0.55, 0.65, 0.80, 0.88);
        leg->AddEntry(frame->findObject("data"), "Data", "lep");
        leg->AddEntry(frame->findObject("model"), "Total Fit (Gaussian+Constant)", "l");
        leg->AddEntry(frame->findObject("jpsi"), "J/#psi Gaussian", "l");
        leg->AddEntry(frame->findObject("bkg"), "Constant Bkg", "l");
        leg->Draw();
        
        c->Write();
        c->SaveAs(Form("JpsiFit_pt%.0f_%.0f.pdf", Jetptmin, Jetptmax));
        delete c;
    }

    return nJpsi.getVal();
}

tuple<float, float, float, float> LFYield_invMass(TNtuple* data_nt, TNtuple* nt_uds, TNtuple* nt_other, TNtuple* nt_c, TNtuple* nt_cc, TNtuple* nt_b, TNtuple* nt_bb, float Jetptmin, float Jetptmax, TDirectory* plotDir = nullptr){

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
        model.plotOn(frame, RooFit::Components(hfPdf), RooFit::LineStyle(kDashed), RooFit::LineColor(kBlue), RooFit::Range("normRange"), RooFit::NormRange("normRange"), RooFit::Name("HF"));
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
        c->SaveAs(Form("LFFit_invmass_pt%.0f_%.0f.pdf", Jetptmin, Jetptmax));
        delete c;
    } 
    return make_tuple(nLF.getVal(), nLF.getError(), nHF.getVal(), nHF.getError());
}


tuple<float, float, float, float> LFYield(TNtuple* data_nt, TNtuple* nt_uds, TNtuple* nt_other, TNtuple* nt_c, TNtuple* nt_cc, TNtuple* nt_b, TNtuple* nt_bb, float Jetptmin, float Jetptmax, TDirectory* plotDir = nullptr){
    
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
        c->SaveAs(Form("LFFit_pt%.0f_%.0f.pdf", Jetptmin, Jetptmax));
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
    bool doJpsi = CL.GetBool("doJpsi");
    bool doLF = CL.GetBool("doLF");
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
    TH1D* JpsiYields = new TH1D("JpsiYields", "J/psi Yields", ptBins.size()-1, &ptBins[0]);
    TH1D* LightYields = new TH1D("LightYields", "Light Flavor Yields", ptBins.size()-1, &ptBins[0]);
    TH1D* HeavyYields = new TH1D("HeavyYields", "Heavy Flavor Yields", ptBins.size()-1, &ptBins[0]);
    TH1D* LightYields_mass = new TH1D("LightYields_mass", "Light Flavor Yields (inv mass method)", ptBins.size()-1, &ptBins[0]);
    TH1D* HeavyYields_mass = new TH1D("HeavyYields_mass", "Heavy Flavor Yields (inv mass method)", ptBins.size()-1, &ptBins[0]);
    TH1D* SplittingYields = new TH1D("SplittingYields", "Splitting Yields", ptBins.size()-1, &ptBins[0]);
    
    // CREATE PLOTS DIRECTORY
    TDirectory* plotDir = nullptr;
    if(makeplots) {
        plotDir = outputFile->mkdir("plots");
    }

    float JspiYield = 0;
    float LightYield = 0;
    float LightYieldError = 0;
    float HeavyYield = 0;
    float HeavyYieldError = 0;
    float LightYield_mass = 0;
    float LightYieldError_mass = 0;
    float HeavyYield_mass = 0;
    float HeavyYieldError_mass = 0;
    float SplittingYield = 0;
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
        
        // JPSI
        if(doJpsi) {
            JspiYield = JpsiYield(nt, ptMin, ptMax, plotDir);
            JpsiYields->SetBinContent(i+1, JspiYield);
        }
        
        // LF
        if(doLF) {
            auto lfResult = LFYield(nt, nt_uds, nt_other, nt_c, nt_cc, nt_b, nt_bb, ptMin, ptMax, plotDir);
            tie(LightYield, LightYieldError, HeavyYield, HeavyYieldError) = lfResult;
            LightYields->SetBinContent(i+1, LightYield);
            LightYields->SetBinError(i+1, LightYieldError);
            HeavyYields->SetBinContent(i+1, HeavyYield);
            HeavyYields->SetBinError(i+1, HeavyYieldError);
        }

        // LF VIA INVMASS METHOD
        if(doLF_invMass) {
            auto lfResultMass = LFYield_invMass(nt, nt_uds, nt_other, nt_c, nt_cc, nt_b, nt_bb, ptMin, ptMax, plotDir);
            tie(LightYield_mass, LightYieldError_mass, HeavyYield_mass, HeavyYieldError_mass) = lfResultMass;
            LightYields_mass->SetBinContent(i+1, LightYield_mass);
            LightYields_mass->SetBinError(i+1, LightYieldError_mass);
            HeavyYields_mass->SetBinContent(i+1, HeavyYield_mass);
            HeavyYields_mass->SetBinError(i+1, HeavyYieldError_mass);
        }
        
        // REMAINING YIELD
        SplittingYield = totalYield - JspiYield - LightYield;
        SplittingYields->SetBinContent(i+1, SplittingYield);
        
    }

    // WRITE TO FILE
    outputFile->cd();
    if(doJpsi) JpsiYields->Write();
    if(doLF) {
        LightYields->Write();
        HeavyYields->Write();
    }
    if(doLF_invMass) {
        LightYields_mass->Write();
        HeavyYields_mass->Write();
    }
    SplittingYields->Write();

    // SAVE COMMAND LINE PARAMS
    TNamed paramFile("InputFile", file.c_str());
    paramFile.Write();
    TNamed paramTemplates("Templates", templates.c_str());
    paramTemplates.Write();
    TNamed paramDoJpsi("doJpsi", doJpsi ? "true" : "false");
    paramDoJpsi.Write();
    TNamed paramDoLF("doLF", doLF ? "true" : "false");
    paramDoLF.Write();
    TNamed paramDoLFInvMass("doLF_invMass", doLF_invMass ? "true" : "false");
    paramDoLFInvMass.Write();
    TNamed paramMakePlots("makeplots", makeplots ? "true" : "false");
    paramMakePlots.Write();

    outputFile->Close();

}