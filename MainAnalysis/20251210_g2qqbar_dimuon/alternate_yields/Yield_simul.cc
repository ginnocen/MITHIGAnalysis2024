/////////////////////////////////////////
//        EXTRACT DIMUON YIELDs        //
/////////////////////////////////////////

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
#include <RooNDKeysPdf.h>
#include <RooArgList.h>
#include <RooFitResult.h>
#include <RooAddition.h>
#include <RooMinimizer.h>

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

struct FitResult {
    double nLF;
    double nLF_err;
    double nOther;
    double nOther_err;
    double nC;
    double nC_err;
    double nCC;
    double nCC_err;
    double nB;
    double nB_err;
    double nBB;
    double nBB_err;
    double chi2_ndf;
};

struct FittingHists {

    const char* variable;
    TH1D* Yields_uds;
    TH1D* Yields_other;
    TH1D* Yields_c;
    TH1D* Yields_cc;
    TH1D* Yields_b;
    TH1D* Yields_bb;
    TH1D* Yields_full;
    TH1D* Chi2ndf;

};

FitResult Yields_Together(TFile* datafile, TFile* templatefile, float Jetptmin, float Jetptmax, TDirectory* plotDir = nullptr){

    // TUPLES
    TNtuple* data_nt = (TNtuple*)datafile->Get("ntDimuon");
    TNtuple* nt_uds = (TNtuple*)templatefile->Get("nt_uds");
    TNtuple* nt_other = (TNtuple*)templatefile->Get("nt_other");
    TNtuple* nt_c = (TNtuple*)templatefile->Get("nt_c");
    TNtuple* nt_cc = (TNtuple*)templatefile->Get("nt_cc");
    TNtuple* nt_b = (TNtuple*)templatefile->Get("nt_b");
    TNtuple* nt_bb = (TNtuple*)templatefile->Get("nt_bb");

    //DEFINE VARIABLES
    RooRealVar mass("mumuMass", "Dimuon Mass", 0, 10);
    RooRealVar DCA("muDiDxy1Dxy2Sig", "DCA Product Significance", -3, 4);
    RooRealVar DR("muDR", "Dimuon #DeltaR", 0, 0.6);
    
    RooRealVar jetpt("JetPT", "Jet pT", 0, 1000);
    RooRealVar weight("weight", "weight", 0, 1e10);
    RooArgSet vars_DCA(DCA, jetpt, weight);
    RooArgSet vars_Mass(mass, jetpt, weight);
    RooArgSet vars_DR(DR, jetpt, weight);
    mass.setRange("fitRange", 0, 10);
    mass.setRange("normRange", 0, 10); 
    DCA.setRange("fitRange", -3, 4); 
    DCA.setRange("normRange", -3, 4);
    DR.setRange("fitRange", 0, 0.6); 
    DR.setRange("normRange", 0, 0.6); 

    float kde_width_mass = 0.3;
    float kde_width_DCA = 1.5;
    float kde_width_DR = 1.5;

    // DATA
    RooDataSet data_DCA("data", "data", data_nt, vars_DCA, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet data_Mass("data", "data", data_nt, vars_Mass, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet data_DR("data", "data", data_nt, vars_DR, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");



    // TEMPLATES - DCA
    RooDataSet template_uds_DCA("tmp_uds_DCA", "", nt_uds, vars_DCA, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet template_other_DCA("tmp_other_DCA", "", nt_other, vars_DCA, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet template_c_DCA("tmp_c_DCA", "", nt_c, vars_DCA, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet template_cc_DCA("tmp_cc_DCA", "", nt_cc, vars_DCA, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet template_b_DCA("tmp_b_DCA", "", nt_b, vars_DCA, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet template_bb_DCA("tmp_bb_DCA", "", nt_bb, vars_DCA, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    // - MASS 
    RooDataSet template_uds_mass("tmp_uds_mass", "", nt_uds, vars_Mass, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet template_other_mass("tmp_other_mass", "", nt_other, vars_Mass, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet template_c_mass("tmp_c_mass", "", nt_c, vars_Mass, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet template_cc_mass("tmp_cc_mass", "", nt_cc, vars_Mass, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet template_b_mass("tmp_b_mass", "", nt_b, vars_Mass, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet template_bb_mass("tmp_bb_mass", "", nt_bb, vars_Mass, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    // - DR
    RooDataSet template_uds_DR("tmp_uds_DR", "", nt_uds, vars_DR, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet template_other_DR("tmp_other_DR", "", nt_other, vars_DR, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet template_c_DR("tmp_c_DR", "", nt_c, vars_DR, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet template_cc_DR("tmp_cc_DR", "", nt_cc, vars_DR, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet template_b_DR("tmp_b_DR", "", nt_b, vars_DR, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet template_bb_DR("tmp_bb_DR", "", nt_bb, vars_DR, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");



    // PDFs - DCA
    RooKeysPdf pdf_other_DCA("pdf_other_DCA", "other DCA PDF", DCA, template_other_DCA, RooKeysPdf::MirrorLeft, kde_width_DCA);
    RooKeysPdf pdf_uds_DCA("pdf_uds_DCA", "uds DCA PDF", DCA, template_uds_DCA, RooKeysPdf::MirrorLeft, kde_width_DCA);
    RooKeysPdf pdf_c_DCA("pdf_c_DCA", "c DCA PDF", DCA, template_c_DCA, RooKeysPdf::MirrorLeft, kde_width_DCA);
    RooKeysPdf pdf_cc_DCA("pdf_cc_DCA", "cc DCA PDF", DCA, template_cc_DCA, RooKeysPdf::MirrorLeft, kde_width_DCA);
    RooKeysPdf pdf_b_DCA("pdf_b_DCA", "b DCA PDF", DCA, template_b_DCA, RooKeysPdf::MirrorLeft, kde_width_DCA);
    RooKeysPdf pdf_bb_DCA("pdf_bb_DCA", "bb DCA PDF", DCA, template_bb_DCA, RooKeysPdf::MirrorLeft, kde_width_DCA);
    
    //  - Mass
    RooKeysPdf pdf_other_mass("pdf_other_mass", "other Mass PDF", mass, template_other_mass, RooKeysPdf::MirrorLeft, kde_width_mass);
    RooKeysPdf pdf_uds_mass("pdf_uds_mass", "uds Mass PDF", mass, template_uds_mass, RooKeysPdf::MirrorLeft, kde_width_mass);
    RooKeysPdf pdf_c_mass("pdf_c_mass", "c Mass PDF", mass, template_c_mass, RooKeysPdf::MirrorLeft, kde_width_mass);
    RooKeysPdf pdf_cc_mass("pdf_cc_mass", "cc Mass PDF", mass, template_cc_mass, RooKeysPdf::MirrorLeft, kde_width_mass);
    RooKeysPdf pdf_b_mass("pdf_b_mass", "b Mass PDF", mass, template_b_mass, RooKeysPdf::MirrorLeft, kde_width_mass);
    RooKeysPdf pdf_bb_mass("pdf_bb_mass", "bb Mass PDF", mass, template_bb_mass, RooKeysPdf::MirrorLeft, kde_width_mass);
    
    // PDFs - DR
    RooKeysPdf pdf_other_DR("pdf_other_DR", "other DR PDF", DR, template_other_DR, RooKeysPdf::MirrorLeft, kde_width_DR);
    RooKeysPdf pdf_uds_DR("pdf_uds_DR", "uds DR PDF", DR, template_uds_DR, RooKeysPdf::MirrorLeft, kde_width_DR);
    RooKeysPdf pdf_c_DR("pdf_c_DR", "c DR PDF", DR, template_c_DR, RooKeysPdf::MirrorLeft, kde_width_DR);
    RooKeysPdf pdf_cc_DR("pdf_cc_DR", "cc DR PDF", DR, template_cc_DR, RooKeysPdf::MirrorLeft, kde_width_DR);
    RooKeysPdf pdf_b_DR("pdf_b_DR", "b DR PDF", DR, template_b_DR, RooKeysPdf::MirrorLeft, kde_width_DR);
    RooKeysPdf pdf_bb_DR("pdf_bb_DR", "bb DR PDF", DR, template_bb_DR, RooKeysPdf::MirrorLeft, kde_width_DR);
    
    // YIELDs - SHARED ACROSS ALL THREE VARIABLES
    double nTotal = data_DCA.sumEntries();
    RooRealVar n_other("n_other", "N other", 0, 0, nTotal);
    RooRealVar n_uds("n_uds", "N light flavor", 0, 0, nTotal);  
    RooRealVar n_c("n_c", "N c", 0, 0, nTotal);
    RooRealVar n_cc("n_cc", "N cc", 0, 0, nTotal);
    RooRealVar n_b("n_b", "N b", nTotal, 0, nTotal);
    RooRealVar n_bb("n_bb", "N bb", 0, 0, nTotal);
    
    // MODELS - ONE PER VARIABLE, BUT SAME YIELDS!
    RooAddPdf model_DCA("model_DCA", "DCA Model", 
        RooArgList(pdf_uds_DCA, pdf_cc_DCA, pdf_b_DCA, pdf_bb_DCA),
        RooArgList(n_uds, n_cc, n_b, n_bb));
    
    RooAddPdf model_mass("model_mass", "Mass Model", 
        RooArgList(pdf_uds_mass, pdf_cc_mass, pdf_b_mass, pdf_bb_mass),
        RooArgList(n_uds, n_cc, n_b, n_bb));
    
    RooAddPdf model_DR("model_DR", "DR Model", 
        RooArgList(pdf_uds_DR, pdf_cc_DR, pdf_b_DR, pdf_bb_DR),
        RooArgList(n_uds, n_cc, n_b, n_bb));

    // SIMULTANEOUS FIT - CREATE AND COMBINE NLLs
    RooAbsReal* nll_DCA = model_DCA.createNLL(data_DCA);
    RooAbsReal* nll_mass = model_mass.createNLL(data_Mass);
    RooAbsReal* nll_DR = model_DR.createNLL(data_DR);
    
    RooAddition nll_total("nll_total", "Combined NLL", RooArgList(*nll_DCA, *nll_mass, *nll_DR));
    
    // MINIMIZE
    RooMinimizer minimizer(nll_total);
    minimizer.setVerbose(kTRUE);
    minimizer.setPrintLevel(1);
    minimizer.minimize("Minuit2", "Migrad");

    cout << "========== SIMULTANEOUS FIT RESULTS ==========" << endl;
    cout << "Light flavor (uds) yield: " << n_uds.getVal() << " +/- " << n_uds.getError() << endl;
    cout << "other yield: " << n_other.getVal() << " +/- " << n_other.getError() << endl;
    cout << "c yield: " << n_c.getVal() << " +/- " << n_c.getError() << endl;
    cout << "cc yield: " << n_cc.getVal() << " +/- " << n_cc.getError() << endl;
    cout << "b yield: " << n_b.getVal() << " +/- " << n_b.getError() << endl;
    cout << "bb yield: " << n_bb.getVal() << " +/- " << n_bb.getError() << endl;
    cout << "Total: " << n_uds.getVal() + n_cc.getVal() + n_b.getVal() + n_bb.getVal() << endl;

    // CREATE FIT PLOTS FOR ALL THREE VARIABLES
    if(plotDir != nullptr) {
        plotDir->cd();
        
        // ===== DCA PLOT =====
        TCanvas* c_DCA = new TCanvas(Form("SimultaneousFit_DCA_pt%.0f_%.0f", Jetptmin, Jetptmax), "", 800, 800);
        c_DCA->Divide(1,2); 
        c_DCA->cd(1);
        gPad->SetPad(0.0, 0.3, 1.0, 1.0);
        gPad->SetBottomMargin(0.02);
        RooPlot* frame_DCA = DCA.frame(RooFit::Title(Form("DCA Simultaneous Fit (%.0f < p_{T} < %.0f GeV)", Jetptmin, Jetptmax)));
        data_DCA.plotOn(frame_DCA, RooFit::Name("data"));
        model_DCA.plotOn(frame_DCA, RooFit::Name("model"));
        model_DCA.plotOn(frame_DCA, RooFit::Components(pdf_uds_DCA), RooFit::LineStyle(kDashed), RooFit::LineColor(kRed), RooFit::Name("uds"));
        model_DCA.plotOn(frame_DCA, RooFit::Components(pdf_cc_DCA), RooFit::LineStyle(kDashed), RooFit::LineColor(kMagenta), RooFit::Name("cc"));
        model_DCA.plotOn(frame_DCA, RooFit::Components(pdf_b_DCA), RooFit::LineStyle(kDashed), RooFit::LineColor(kCyan), RooFit::Name("b"));
        model_DCA.plotOn(frame_DCA, RooFit::Components(pdf_bb_DCA), RooFit::LineStyle(kDashed), RooFit::LineColor(kOrange), RooFit::Name("bb"));
        double chi2_ndf_DCA = frame_DCA->chiSquare("model", "data", 4);
        frame_DCA->Draw();
        TLegend* leg_DCA = new TLegend(0.55, 0.65, 0.80, 0.88);
        leg_DCA->AddEntry(frame_DCA->findObject("data"), "Data", "lep");
        leg_DCA->AddEntry(frame_DCA->findObject("model"), "Total Fit", "l");
        leg_DCA->AddEntry(frame_DCA->findObject("uds"), "uds", "l");
        leg_DCA->AddEntry(frame_DCA->findObject("cc"), "cc", "l");
        leg_DCA->AddEntry(frame_DCA->findObject("b"), "b", "l");
        leg_DCA->AddEntry(frame_DCA->findObject("bb"), "bb", "l");
        leg_DCA->Draw();
        c_DCA->cd(2);
        gPad->SetPad(0.0, 0.0, 1.0, 0.3);
        gPad->SetTopMargin(0.02);
        gPad->SetBottomMargin(0.3);
        RooPlot* framePull_DCA = DCA.frame(RooFit::Title(""));
        RooHist* hpull_DCA = frame_DCA->pullHist("data", "model");
        hpull_DCA->SetMarkerStyle(20);
        hpull_DCA->SetMarkerSize(0.8);
        framePull_DCA->addPlotable(hpull_DCA, "P0");
        framePull_DCA->SetMinimum(-5);
        framePull_DCA->SetMaximum(5);
        framePull_DCA->GetYaxis()->SetTitle("Pull");
        framePull_DCA->GetYaxis()->SetTitleSize(0.12);
        framePull_DCA->GetYaxis()->SetLabelSize(0.10);
        framePull_DCA->GetYaxis()->SetTitleOffset(0.35);
        framePull_DCA->GetYaxis()->SetNdivisions(505);
        framePull_DCA->GetXaxis()->SetTitleSize(0.12);
        framePull_DCA->GetXaxis()->SetLabelSize(0.10);
        framePull_DCA->GetXaxis()->SetTitleOffset(1.0);
        framePull_DCA->Draw();
        TLine* line0_DCA = new TLine(framePull_DCA->GetXaxis()->GetXmin(), 0, framePull_DCA->GetXaxis()->GetXmax(), 0);
        line0_DCA->SetLineStyle(2);
        line0_DCA->Draw();
        TLine* line3_DCA = new TLine(framePull_DCA->GetXaxis()->GetXmin(), 3, framePull_DCA->GetXaxis()->GetXmax(), 3);
        line3_DCA->SetLineColor(kRed);
        line3_DCA->SetLineStyle(2);
        line3_DCA->Draw();
        TLine* lineM3_DCA = new TLine(framePull_DCA->GetXaxis()->GetXmin(), -3, framePull_DCA->GetXaxis()->GetXmax(), -3);
        lineM3_DCA->SetLineColor(kRed);
        lineM3_DCA->SetLineStyle(2);
        lineM3_DCA->Draw();
        c_DCA->Write();
        c_DCA->SaveAs(Form("plots/SimultaneousFit_DCA_pt%.0f_%.0f.pdf", Jetptmin, Jetptmax));
        delete leg_DCA;
        delete c_DCA;

        // ===== MASS PLOT =====
        TCanvas* c_mass = new TCanvas(Form("SimultaneousFit_Mass_pt%.0f_%.0f", Jetptmin, Jetptmax), "", 800, 800);
        c_mass->Divide(1,2);
        c_mass->cd(1);
        gPad->SetPad(0.0, 0.3, 1.0, 1.0);
        gPad->SetBottomMargin(0.02);
        RooPlot* frame_mass = mass.frame(RooFit::Title(Form("Mass Simultaneous Fit (%.0f < p_{T} < %.0f GeV)", Jetptmin, Jetptmax)));
        data_Mass.plotOn(frame_mass, RooFit::Name("data"));
        model_mass.plotOn(frame_mass, RooFit::Name("model"));
        model_mass.plotOn(frame_mass, RooFit::Components(pdf_uds_mass), RooFit::LineStyle(kDashed), RooFit::LineColor(kRed), RooFit::Name("uds"));
        model_mass.plotOn(frame_mass, RooFit::Components(pdf_cc_mass), RooFit::LineStyle(kDashed), RooFit::LineColor(kMagenta), RooFit::Name("cc"));
        model_mass.plotOn(frame_mass, RooFit::Components(pdf_b_mass), RooFit::LineStyle(kDashed), RooFit::LineColor(kCyan), RooFit::Name("b"));
        model_mass.plotOn(frame_mass, RooFit::Components(pdf_bb_mass), RooFit::LineStyle(kDashed), RooFit::LineColor(kOrange), RooFit::Name("bb"));
        double chi2_ndf_mass = frame_mass->chiSquare("model", "data", 4);
        frame_mass->Draw();
        TLegend* leg_mass = new TLegend(0.55, 0.65, 0.80, 0.88);
        leg_mass->AddEntry(frame_mass->findObject("data"), "Data", "lep");
        leg_mass->AddEntry(frame_mass->findObject("model"), "Total Fit", "l");
        leg_mass->AddEntry(frame_mass->findObject("uds"), "uds", "l");
        leg_mass->AddEntry(frame_mass->findObject("cc"), "cc", "l");
        leg_mass->AddEntry(frame_mass->findObject("b"), "b", "l");
        leg_mass->AddEntry(frame_mass->findObject("bb"), "bb", "l");
        leg_mass->Draw();
        c_mass->cd(2);
        gPad->SetPad(0.0, 0.0, 1.0, 0.3);
        gPad->SetTopMargin(0.02);
        gPad->SetBottomMargin(0.3);
        RooPlot* framePull_mass = mass.frame(RooFit::Title(""));
        RooHist* hpull_mass = frame_mass->pullHist("data", "model");
        hpull_mass->SetMarkerStyle(20);
        hpull_mass->SetMarkerSize(0.8);
        framePull_mass->addPlotable(hpull_mass, "P0");
        framePull_mass->SetMinimum(-5);
        framePull_mass->SetMaximum(5);
        framePull_mass->GetYaxis()->SetTitle("Pull");
        framePull_mass->GetYaxis()->SetTitleSize(0.12);
        framePull_mass->GetYaxis()->SetLabelSize(0.10);
        framePull_mass->GetYaxis()->SetTitleOffset(0.35);
        framePull_mass->GetYaxis()->SetNdivisions(505);
        framePull_mass->GetXaxis()->SetTitleSize(0.12);
        framePull_mass->GetXaxis()->SetLabelSize(0.10);
        framePull_mass->GetXaxis()->SetTitleOffset(1.0);
        framePull_mass->Draw();
        TLine* line0_mass = new TLine(framePull_mass->GetXaxis()->GetXmin(), 0, framePull_mass->GetXaxis()->GetXmax(), 0);
        line0_mass->SetLineStyle(2);
        line0_mass->Draw();
        TLine* line3_mass = new TLine(framePull_mass->GetXaxis()->GetXmin(), 3, framePull_mass->GetXaxis()->GetXmax(), 3);
        line3_mass->SetLineColor(kRed);
        line3_mass->SetLineStyle(2);
        line3_mass->Draw();
        TLine* lineM3_mass = new TLine(framePull_mass->GetXaxis()->GetXmin(), -3, framePull_mass->GetXaxis()->GetXmax(), -3);
        lineM3_mass->SetLineColor(kRed);
        lineM3_mass->SetLineStyle(2);
        lineM3_mass->Draw();
        c_mass->Write();
        c_mass->SaveAs(Form("plots/SimultaneousFit_Mass_pt%.0f_%.0f.pdf", Jetptmin, Jetptmax));
        delete leg_mass;
        delete c_mass;

        // ===== DR PLOT =====
        TCanvas* c_DR = new TCanvas(Form("SimultaneousFit_DR_pt%.0f_%.0f", Jetptmin, Jetptmax), "", 800, 800);
        c_DR->Divide(1,2);
        c_DR->cd(1);
        gPad->SetPad(0.0, 0.3, 1.0, 1.0);
        gPad->SetBottomMargin(0.02);
        RooPlot* frame_DR = DR.frame(RooFit::Title(Form("DR Simultaneous Fit (%.0f < p_{T} < %.0f GeV)", Jetptmin, Jetptmax)));
        data_DR.plotOn(frame_DR, RooFit::Name("data"));
        model_DR.plotOn(frame_DR, RooFit::Name("model"));
        model_DR.plotOn(frame_DR, RooFit::Components(pdf_uds_DR), RooFit::LineStyle(kDashed), RooFit::LineColor(kRed), RooFit::Name("uds"));
        model_DR.plotOn(frame_DR, RooFit::Components(pdf_cc_DR), RooFit::LineStyle(kDashed), RooFit::LineColor(kMagenta), RooFit::Name("cc"));
        model_DR.plotOn(frame_DR, RooFit::Components(pdf_b_DR), RooFit::LineStyle(kDashed), RooFit::LineColor(kCyan), RooFit::Name("b"));
        model_DR.plotOn(frame_DR, RooFit::Components(pdf_bb_DR), RooFit::LineStyle(kDashed), RooFit::LineColor(kOrange), RooFit::Name("bb"));
        double chi2_ndf_DR = frame_DR->chiSquare("model", "data", 4);
        frame_DR->Draw();
        TLegend* leg_DR = new TLegend(0.55, 0.65, 0.80, 0.88);
        leg_DR->AddEntry(frame_DR->findObject("data"), "Data", "lep");
        leg_DR->AddEntry(frame_DR->findObject("model"), "Total Fit", "l");
        leg_DR->AddEntry(frame_DR->findObject("uds"), "uds", "l");
        leg_DR->AddEntry(frame_DR->findObject("cc"), "cc", "l");
        leg_DR->AddEntry(frame_DR->findObject("b"), "b", "l");
        leg_DR->AddEntry(frame_DR->findObject("bb"), "bb", "l");
        leg_DR->Draw();
        c_DR->cd(2);
        gPad->SetPad(0.0, 0.0, 1.0, 0.3);
        gPad->SetTopMargin(0.02);
        gPad->SetBottomMargin(0.3);
        RooPlot* framePull_DR = DR.frame(RooFit::Title(""));
        RooHist* hpull_DR = frame_DR->pullHist("data", "model");
        hpull_DR->SetMarkerStyle(20);
        hpull_DR->SetMarkerSize(0.8);
        framePull_DR->addPlotable(hpull_DR, "P0");
        framePull_DR->SetMinimum(-5);
        framePull_DR->SetMaximum(5);
        framePull_DR->GetYaxis()->SetTitle("Pull");
        framePull_DR->GetYaxis()->SetTitleSize(0.12);
        framePull_DR->GetYaxis()->SetLabelSize(0.10);
        framePull_DR->GetYaxis()->SetTitleOffset(0.35);
        framePull_DR->GetYaxis()->SetNdivisions(505);
        framePull_DR->GetXaxis()->SetTitleSize(0.12);
        framePull_DR->GetXaxis()->SetLabelSize(0.10);
        framePull_DR->GetXaxis()->SetTitleOffset(1.0);
        framePull_DR->Draw();
        TLine* line0_DR = new TLine(framePull_DR->GetXaxis()->GetXmin(), 0, framePull_DR->GetXaxis()->GetXmax(), 0);
        line0_DR->SetLineStyle(2);
        line0_DR->Draw();
        TLine* line3_DR = new TLine(framePull_DR->GetXaxis()->GetXmin(), 3, framePull_DR->GetXaxis()->GetXmax(), 3);
        line3_DR->SetLineColor(kRed);
        line3_DR->SetLineStyle(2);
        line3_DR->Draw();
        TLine* lineM3_DR = new TLine(framePull_DR->GetXaxis()->GetXmin(), -3, framePull_DR->GetXaxis()->GetXmax(), -3);
        lineM3_DR->SetLineColor(kRed);
        lineM3_DR->SetLineStyle(2);
        lineM3_DR->Draw();
        c_DR->Write();
        c_DR->SaveAs(Form("plots/SimultaneousFit_DR_pt%.0f_%.0f.pdf", Jetptmin, Jetptmax));
        delete leg_DR;
        delete c_DR;

        cout << "Chi^2/ndf DCA: " << chi2_ndf_DCA << endl;
        cout << "Chi^2/ndf Mass: " << chi2_ndf_mass << endl;
        cout << "Chi^2/ndf DR: " << chi2_ndf_DR << endl;

        return {n_uds.getVal(), n_uds.getError(), n_other.getVal(), n_other.getError(), n_c.getVal(), n_c.getError(), 
                n_cc.getVal(), n_cc.getError(), n_b.getVal(), n_b.getError(), 
                n_bb.getVal(), n_bb.getError(), (chi2_ndf_DCA + chi2_ndf_mass + chi2_ndf_DR)/3.0};
    } else {
        // Calculate chi2/ndf even when not plotting
        RooPlot* frame_DCA = DCA.frame();
        data_DCA.plotOn(frame_DCA, RooFit::Name("data"));
        model_DCA.plotOn(frame_DCA, RooFit::Name("model"));
        double chi2_ndf_DCA = frame_DCA->chiSquare("model", "data", 4);
        delete frame_DCA;
        
        RooPlot* frame_mass = mass.frame();
        data_Mass.plotOn(frame_mass, RooFit::Name("data"));
        model_mass.plotOn(frame_mass, RooFit::Name("model"));
        double chi2_ndf_mass = frame_mass->chiSquare("model", "data", 4);
        delete frame_mass;
        
        RooPlot* frame_DR = DR.frame();
        data_DR.plotOn(frame_DR, RooFit::Name("data"));
        model_DR.plotOn(frame_DR, RooFit::Name("model"));
        double chi2_ndf_DR = frame_DR->chiSquare("model", "data", 4);
        delete frame_DR;
        
        cout << "Chi^2/ndf DCA: " << chi2_ndf_DCA << endl;
        cout << "Chi^2/ndf Mass: " << chi2_ndf_mass << endl;
        cout << "Chi^2/ndf DR: " << chi2_ndf_DR << endl;

        return {n_uds.getVal(), n_uds.getError(), n_other.getVal(), n_other.getError(), n_c.getVal(), n_c.getError(),
                n_cc.getVal(), n_cc.getError(), n_b.getVal(), n_b.getError(), 
                n_bb.getVal(), n_bb.getError(), (chi2_ndf_DCA + chi2_ndf_mass + chi2_ndf_DR)/3.0};
    }
}

FittingHists CreateFittingHists(TFile* datafile, TFile* templatefile, vector<double> ptBins, TDirectory* plotDir = nullptr) {
    
    const char* variable = "Simul";
    FittingHists hists;
    hists.variable = variable;
    hists.Yields_uds = new TH1D(Form("udsYields_%s", variable), "", ptBins.size()-1, ptBins.data());
    hists.Yields_other = new TH1D(Form("OtherYields_%s", variable), "", ptBins.size()-1, ptBins.data());
    hists.Yields_c = new TH1D(Form("CYields_%s", variable), "", ptBins.size()-1, ptBins.data());
    hists.Yields_cc = new TH1D(Form("CCYields_%s", variable), "", ptBins.size()-1, ptBins.data());
    hists.Yields_b = new TH1D(Form("BYields_%s", variable), "", ptBins.size()-1, ptBins.data());
    hists.Yields_bb = new TH1D(Form("BBYields_%s", variable), "", ptBins.size()-1, ptBins.data());
    hists.Yields_full = new TH1D(Form("FullYields_%s", variable), "", ptBins.size()-1, ptBins.data());
    hists.Chi2ndf = new TH1D(Form("Chi2ndf_%s", variable), "", ptBins.size()-1, ptBins.data());

    for(int i = 0; i < ptBins.size()-1; i++){
        
        float ptMin = ptBins[i];
        float ptMax = ptBins[i+1];
        float binwidth = ptMax - ptMin;

        //FitResult result = Yields_Flexible(datafile, templatefile, variable, fitrange_min, fitrange_max, kde_width, ptMin, ptMax, plotDir);
        FitResult result = Yields_Together(datafile, templatefile, ptMin, ptMax, plotDir);


        hists.Yields_uds->SetBinContent(i+1, result.nLF / binwidth);
        hists.Yields_uds->SetBinError(i+1, result.nLF_err / binwidth);
        hists.Yields_other->SetBinContent(i+1, result.nOther / binwidth);
        hists.Yields_other->SetBinError(i+1, result.nOther_err / binwidth);
        hists.Yields_c->SetBinContent(i+1, result.nC / binwidth);
        hists.Yields_c->SetBinError(i+1, result.nC_err / binwidth);
        hists.Yields_cc->SetBinContent(i+1, result.nCC / binwidth);
        hists.Yields_cc->SetBinError(i+1, result.nCC_err / binwidth);
        hists.Yields_b->SetBinContent(i+1, result.nB / binwidth);
        hists.Yields_b->SetBinError(i+1, result.nB_err / binwidth);
        hists.Yields_bb->SetBinContent(i+1, result.nBB / binwidth);
        hists.Yields_bb->SetBinError(i+1, result.nBB_err / binwidth);
        hists.Yields_full->SetBinContent(i+1, (result.nLF + result.nOther + result.nC + result.nCC + result.nB + result.nBB) / binwidth);
        // Error propagation for sum
        hists.Yields_full->SetBinError(i+1, sqrt(pow(result.nLF_err,2) + pow(result.nOther_err,2) + pow(result.nC_err,2) + pow(result.nCC_err,2) + pow(result.nB_err,2) + pow(result.nBB_err,2)) / binwidth);
        hists.Chi2ndf->SetBinContent(i+1, result.chi2_ndf);
        hists.Chi2ndf->SetBinError(i+1, 0);
    
    }


    return hists;
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
    //bool doYields_DCA = CL.GetBool("doYields_DCA", true);
    //bool doYields_invMass = CL.GetBool("doYields_invMass", true);
    //bool doYields_DR = CL.GetBool("doYields_DR", true);
    bool makeplots = CL.GetBool("makeplots", true);

    // IMPORT FILES 
    TFile* input = TFile::Open(file.c_str());
    TFile* templatesFile = TFile::Open(templates.c_str());

    // DECLARE HISTOGRAMS
    TFile* outputFile = new TFile(output.c_str(), "RECREATE");
    outputFile->cd();
    
    // CREATE PLOTS DIRECTORY
    TDirectory* plotDir = nullptr;
    if(makeplots) {
        plotDir = outputFile->mkdir("plots");
    }

    // DO THE SIMULTANEOUS FITS 
    FittingHists Yields_Simul;
    Yields_Simul = CreateFittingHists(input, templatesFile, ptBins, plotDir);


    // WRITE TO FILE
    outputFile->cd();
        
    Yields_Simul.Yields_uds->Write("Yields_uds");
    Yields_Simul.Yields_other->Write("Yields_other");
    Yields_Simul.Yields_c->Write("Yields_c");
    Yields_Simul.Yields_cc->Write("Yields_cc");
    Yields_Simul.Yields_b->Write("Yields_b");
    Yields_Simul.Yields_bb->Write("Yields_bb");
    Yields_Simul.Yields_full->Write("Yields_full");
    Yields_Simul.Chi2ndf->Write("Chi2ndf");

    // SAVE COMMAND LINE PARAMS
    TNamed paramFile("InputFile", file.c_str());
    paramFile.Write();
    TNamed paramTemplates("Templates", templates.c_str());
    paramTemplates.Write();
    TNamed paramMakePlots("makeplots", makeplots ? "true" : "false");
    paramMakePlots.Write();

    if(makeplots){
        
        plotDir->cd();

        // UDS PLOT
        TCanvas* c_uds = new TCanvas("Yields_uds", "", 800, 600);
        c_uds->cd();
        
        Yields_Simul.Yields_uds->SetLineColor(kBlack);
        Yields_Simul.Yields_uds->SetMarkerColor(kBlack);
        Yields_Simul.Yields_uds->SetMarkerStyle(20);
        Yields_Simul.Yields_uds->SetTitle("Light Flavor Yields (Simultaneous Fit)");
        Yields_Simul.Yields_uds->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        Yields_Simul.Yields_uds->GetYaxis()->SetTitle("dN / dp_{T}");
        Yields_Simul.Yields_uds->SetMinimum(0);
        Yields_Simul.Yields_uds->Draw("E1");
        c_uds->Write();
        c_uds->SaveAs("plots/Yields_uds.pdf");
        delete c_uds;

        // Other 
        TCanvas* c_other = new TCanvas("Yields_other", "", 800, 600);
        c_other->cd();
        Yields_Simul.Yields_other->SetLineColor(kBlack);
        Yields_Simul.Yields_other->SetMarkerColor(kBlack);
        Yields_Simul.Yields_other->SetMarkerStyle(20);
        Yields_Simul.Yields_other->SetTitle("Other Yields (Simultaneous Fit)");
        Yields_Simul.Yields_other->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        Yields_Simul.Yields_other->GetYaxis()->SetTitle("dN / dp_{T}");
        Yields_Simul.Yields_other->SetMinimum(0);
        Yields_Simul.Yields_other->Draw("E1");
        c_other->Write();
        c_other->SaveAs("plots/Yields_other.pdf");
        delete c_other; 

        // C PLOT
        TCanvas* c_c = new TCanvas("Yields_c", "", 800, 600);
        c_c->cd();
        Yields_Simul.Yields_c->SetLineColor(kBlack);
        Yields_Simul.Yields_c->SetMarkerColor(kBlack);
        Yields_Simul.Yields_c->SetMarkerStyle(20);
        Yields_Simul.Yields_c->SetTitle("c Yields (Simultaneous Fit)");
        Yields_Simul.Yields_c->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        Yields_Simul.Yields_c->GetYaxis()->SetTitle("dN / dp_{T}");
        Yields_Simul.Yields_c->SetMinimum(0);
        Yields_Simul.Yields_c->Draw("E1");
        c_c->Write();
        c_c->SaveAs("plots/Yields_c.pdf");
        delete c_c; 

        // CC PLOT
        TCanvas* c_cc = new TCanvas("Yields_cc", "", 800, 600);
        c_cc->cd();
        Yields_Simul.Yields_cc->SetLineColor(kBlack);
        Yields_Simul.Yields_cc->SetMarkerColor(kBlack);
        Yields_Simul.Yields_cc->SetMarkerStyle(20);
        Yields_Simul.Yields_cc->SetTitle("cc Yields (Simultaneous Fit)");
        Yields_Simul.Yields_cc->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        Yields_Simul.Yields_cc->GetYaxis()->SetTitle("dN / dp_{T}");
        Yields_Simul.Yields_cc->SetMinimum(0);
        Yields_Simul.Yields_cc->Draw("E1");
        c_cc->Write();
        c_cc->SaveAs("plots/Yields_cc.pdf");
        delete c_cc;

        // B PLOT
        TCanvas* c_b = new TCanvas("Yields_b", "", 800, 600);
        c_b->cd();
        Yields_Simul.Yields_b->SetLineColor(kBlack);
        Yields_Simul.Yields_b->SetMarkerColor(kBlack);
        Yields_Simul.Yields_b->SetMarkerStyle(20);
        Yields_Simul.Yields_b->SetTitle("b Yields (Simultaneous Fit)");
        Yields_Simul.Yields_b->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        Yields_Simul.Yields_b->GetYaxis()->SetTitle("dN / dp_{T}");
        Yields_Simul.Yields_b->SetMinimum(0);
        Yields_Simul.Yields_b->Draw("E1");
        c_b->Write();
        c_b->SaveAs("plots/Yields_b.pdf");
        delete c_b; 

        // BB PLOT
        TCanvas* c_bb = new TCanvas("Yields_bb", "", 800, 600);
        c_bb->cd();
        Yields_Simul.Yields_bb->SetLineColor(kBlack);
        Yields_Simul.Yields_bb->SetMarkerColor(kBlack);
        Yields_Simul.Yields_bb->SetMarkerStyle(20);
        Yields_Simul.Yields_bb->SetTitle("bb Yields (Simultaneous Fit)");
        Yields_Simul.Yields_bb->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        Yields_Simul.Yields_bb->GetYaxis()->SetTitle("dN / dp_{T}");
        Yields_Simul.Yields_bb->SetMinimum(0);
        Yields_Simul.Yields_bb->Draw("E1");
        c_bb->Write();
        c_bb->SaveAs("plots/Yields_bb.pdf");
        delete c_bb;

        // FULL PLOT
        TCanvas* c_full = new TCanvas("Yields_full", "", 800, 600);
        c_full->cd();
        Yields_Simul.Yields_full->SetLineColor(kBlack);
        Yields_Simul.Yields_full->SetMarkerColor(kBlack);
        Yields_Simul.Yields_full->SetMarkerStyle(20);
        Yields_Simul.Yields_full->SetTitle("Full Yields (Simultaneous Fit)");
        Yields_Simul.Yields_full->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        Yields_Simul.Yields_full->GetYaxis()->SetTitle("dN / dp_{T}");
        Yields_Simul.Yields_full->SetMinimum(0);
        Yields_Simul.Yields_full->Draw("E1");
        c_full->Write();
        c_full->SaveAs("plots/Yields_full.pdf");
        delete c_full;  
        
        // Chi2/ndf PLOT
        TCanvas* c_chi2 = new TCanvas("Chi2ndf", "", 800, 600);
        c_chi2->cd();
        Yields_Simul.Chi2ndf->SetLineColor(kBlack);
        Yields_Simul.Chi2ndf->SetMarkerColor(kBlack);
        Yields_Simul.Chi2ndf->SetMarkerStyle(20);
        Yields_Simul.Chi2ndf->SetTitle("Chi^{2}/ndf (Simultaneous Fit)");
        Yields_Simul.Chi2ndf->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        Yields_Simul.Chi2ndf->GetYaxis()->SetTitle("Chi^{2}/ndf");
        Yields_Simul.Chi2ndf->SetMinimum(0);
        Yields_Simul.Chi2ndf->SetMaximum(5);
        Yields_Simul.Chi2ndf->Draw("E1");
        c_chi2->Write();
        c_chi2->SaveAs("plots/Chi2ndf.pdf");
        delete c_chi2;  

    
    }

    outputFile->Close();
    input->Close();
    templatesFile->Close();

    return 0;

}