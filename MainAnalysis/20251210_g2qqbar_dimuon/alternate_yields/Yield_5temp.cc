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

FitResult Yields_Flexible(TFile* datafile, TFile* templatefile, const char* variable, float fitrange_min, float fitrange_max, float kde_width, float Jetptmin, float Jetptmax, TDirectory* plotDir = nullptr){

    // TUPLES
    TNtuple* data_nt = (TNtuple*)datafile->Get("ntDimuon");
    TNtuple* nt_uds = (TNtuple*)templatefile->Get("nt_uds");
    TNtuple* nt_other = (TNtuple*)templatefile->Get("nt_other");
    TNtuple* nt_c = (TNtuple*)templatefile->Get("nt_c");
    TNtuple* nt_cc = (TNtuple*)templatefile->Get("nt_cc");
    TNtuple* nt_b = (TNtuple*)templatefile->Get("nt_b");
    TNtuple* nt_bb = (TNtuple*)templatefile->Get("nt_bb");

    //DEFINE VARIABLES
    RooRealVar fittingvar(variable, variable, fitrange_min, fitrange_max);
    RooRealVar jetpt("JetPT", "Jet pT", 0, 1000);
    RooRealVar weight("weight", "weight", 0, 1e10);
    RooArgSet vars(fittingvar, jetpt, weight);
    fittingvar.setRange("fitRange", fitrange_min, fitrange_max); // Fit over full range
    fittingvar.setRange("normRange", fitrange_min, fitrange_max); // Normalize over full range

    // DATA
    RooDataSet data("data", "data", data_nt, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");

    // TEMPLATES
    RooDataSet template_uds("tmp_uds", "", nt_uds, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet template_other("tmp_other", "", nt_other, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet template_c("tmp_c", "", nt_c, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet template_cc("tmp_cc", "", nt_cc, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet template_b("tmp_b", "", nt_b, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet template_bb("tmp_bb", "", nt_bb, vars, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");

    // PDFs
    RooKeysPdf pdf_uds("Pdf_uds", "uds PDF", fittingvar, template_uds, RooKeysPdf::MirrorLeft, kde_width);
    RooKeysPdf pdf_other("Pdf_other", "other PDF", fittingvar, template_other, RooKeysPdf::MirrorLeft, kde_width);
    RooKeysPdf pdf_c("Pdf_c", "c PDF", fittingvar, template_c, RooKeysPdf::MirrorLeft, kde_width);
    RooKeysPdf pdf_cc("Pdf_cc", "cc PDF", fittingvar, template_cc, RooKeysPdf::MirrorLeft, kde_width);
    RooKeysPdf pdf_b("Pdf_b", "b PDF", fittingvar, template_b, RooKeysPdf::MirrorLeft, kde_width);
    RooKeysPdf pdf_bb("Pdf_bb", "bb PDF", fittingvar, template_bb, RooKeysPdf::MirrorLeft, kde_width);
    
    // YIELDs
    double nTotal = data.sumEntries();
    RooRealVar n_uds("n_uds", "N light flavor", 0, 0, nTotal);
    RooRealVar n_other("n_other", "N other", 0, 0, nTotal);
    RooRealVar n_c("n_c", "N c", 0, 0, nTotal);
    RooRealVar n_cc("n_cc", "N cc", 0, 0, nTotal);
    RooRealVar n_b("n_b", "N b", nTotal, 0, nTotal);
    RooRealVar n_bb("n_bb", "N bb", 0, 0, nTotal);
    RooAddPdf model("model", "Full Fit", RooArgList(pdf_uds, pdf_cc, pdf_b, pdf_bb), RooArgList(n_uds, n_cc, n_b, n_bb));

    // FIT
    RooFitResult* result = model.fitTo(data, 
                                        RooFit::Range("fitRange"),      // Fit only here
                                       RooFit::NormRange("normRange"),  // But yields are for full range
                                       RooFit::Save());

    cout << "Light flavor yield: " << n_uds.getVal() << " +/- " << n_uds.getError() << endl;
    cout << "Other yield: " << n_other.getVal() << " +/- " << n_other.getError() << endl;
    cout << "C yield: " << n_c.getVal() << " +/- " << n_c.getError() << endl;
    cout << "CC yield: " << n_cc.getVal() << " +/- " << n_cc.getError() << endl;
    cout << "B yield: " << n_b.getVal() << " +/- " << n_b.getError() << endl;
    cout << "BB yield: " << n_bb.getVal() << " +/- " << n_bb.getError() << endl;
    // CREATE FIT PLOT
    if(plotDir != nullptr) {
        plotDir->cd();
        TCanvas* c = new TCanvas(Form("TemplateFit_%s_pt%.0f_%.0f", variable, Jetptmin, Jetptmax), "", 800, 800);
        c->Divide(1,2); 

        // Top pad: main fit
        c->cd(1);
        gPad->SetPad(0.0, 0.3, 1.0, 1.0);
        gPad->SetBottomMargin(0.02);
        RooPlot* frame = fittingvar.frame(RooFit::Title(Form("Template Fit (%.0f < p_{T} < %.0f GeV)", Jetptmin, Jetptmax)));
        data.plotOn(frame, RooFit::Name("data"));
        model.plotOn(frame, RooFit::Range("fitRange"), RooFit::NormRange("normRange"), RooFit::Name("model"));
        model.plotOn(frame, RooFit::Components(pdf_uds), RooFit::LineStyle(kDashed), RooFit::LineColor(kRed), RooFit::Range("normRange"), RooFit::NormRange("normRange"), RooFit::Name("uds"));
        model.plotOn(frame, RooFit::Components(pdf_other), RooFit::LineStyle(kDashed), RooFit::LineColor(kGreen+2), RooFit::Range("normRange"), RooFit::NormRange("normRange"), RooFit::Name("other"));
        model.plotOn(frame, RooFit::Components(pdf_c), RooFit::LineStyle(kDashed), RooFit::LineColor(kBlue), RooFit::Range("normRange"), RooFit::NormRange("normRange"), RooFit::Name("c"));
        model.plotOn(frame, RooFit::Components(pdf_cc), RooFit::LineStyle(kDashed), RooFit::LineColor(kMagenta), RooFit::Range("normRange"), RooFit::NormRange("normRange"), RooFit::Name("cc"));
        model.plotOn(frame, RooFit::Components(pdf_b), RooFit::LineStyle(kDashed), RooFit::LineColor(kCyan), RooFit::Range("normRange"), RooFit::NormRange("normRange"), RooFit::Name("b"));
        model.plotOn(frame, RooFit::Components(pdf_bb), RooFit::LineStyle(kDashed), RooFit::LineColor(kOrange), RooFit::Range("normRange"), RooFit::NormRange("normRange"), RooFit::Name("bb"));
        
        // Calculate chi-square/ndf (6 floating parameters: nLF, nOther, nC, nCC, nB, nBB)
        double chi2_ndf = frame->chiSquare("model", "data", 6);
        cout << "Chi^2/ndf = " << chi2_ndf << endl;
        frame->Draw();

        // Add legend
        TLegend* leg = new TLegend(0.55, 0.65, 0.80, 0.88);
        leg->AddEntry(frame->findObject("data"), "Data", "lep");
        leg->AddEntry(frame->findObject("model"), "Total Fit", "l");
        leg->AddEntry(frame->findObject("uds"), "Light Flavor", "l");
        leg->AddEntry(frame->findObject("other"), "Other", "l");
        leg->AddEntry(frame->findObject("c"), "c", "l");
        leg->AddEntry(frame->findObject("cc"), "cc", "l");
        leg->AddEntry(frame->findObject("b"), "b", "l");
        leg->AddEntry(frame->findObject("bb"), "bb", "l");
        leg->Draw();

        // Bottom pad: pulls
        c->cd(2);
        gPad->SetPad(0.0, 0.0, 1.0, 0.3);
        gPad->SetTopMargin(0.02);
        gPad->SetBottomMargin(0.3);
        
        RooPlot* framePull = fittingvar.frame(RooFit::Title(""));
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
        c->SaveAs(Form("plots/TemplateFit_%s_pt%.0f_%.0f.pdf", variable, Jetptmin, Jetptmax));
        delete leg;
        delete frame;
        delete framePull;
        delete c;
        return {n_uds.getVal(), n_uds.getError(), n_other.getVal(), n_other.getError(), n_c.getVal(), n_c.getError(), n_cc.getVal(), n_cc.getError(), n_b.getVal(), n_b.getError(), n_bb.getVal(), n_bb.getError(), chi2_ndf};
    } else {
        // Calculate chi2/ndf even when not plotting
        RooPlot* frame = fittingvar.frame();
        data.plotOn(frame, RooFit::Name("data"));
        model.plotOn(frame, RooFit::Range("fitRange"), RooFit::NormRange("normRange"), RooFit::Name("model"));
        double chi2_ndf = frame->chiSquare("model", "data", 2);
        cout << "Chi^2/ndf = " << chi2_ndf << endl;
        delete frame;
        return {n_uds.getVal(), n_uds.getError(), n_other.getVal(), n_other.getError(), n_c.getVal(), n_c.getError(), n_cc.getVal(), n_cc.getError(), n_b.getVal(), n_b.getError(), n_bb.getVal(), n_bb.getError(), chi2_ndf};
    }
}


FittingHists CreateFittingHists(TFile* datafile, TFile* templatefile, const char* variable, vector<double> ptBins, float fitrange_min, float fitrange_max, float kde_width, TDirectory* plotDir = nullptr) {
    
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

        FitResult result = Yields_Flexible(datafile, templatefile, variable, fitrange_min, fitrange_max, kde_width, ptMin, ptMax, plotDir);

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
    bool doYields_DCA = CL.GetBool("doYields_DCA", true);
    bool doYields_invMass = CL.GetBool("doYields_invMass", true);
    bool doYields_DR = CL.GetBool("doYields_DR", true);
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

    // DO THE FITS 
    FittingHists Yields_DCA, Yields_InvMass, Yields_DR;
    if(doYields_DCA) Yields_DCA = CreateFittingHists(input, templatesFile, "muDiDxy1Dxy2Sig", ptBins, -3, 4, 1.5, plotDir);
    if(doYields_invMass) Yields_InvMass = CreateFittingHists(input, templatesFile, "mumuMass", ptBins, 0, 10, 0.3, plotDir);
    if(doYields_DR) Yields_DR = CreateFittingHists(input, templatesFile, "muDR", ptBins, 0, 0.6, 1.5, plotDir);

    // WRITE TO FILE
    outputFile->cd();
        
    if(doYields_DCA) {
        Yields_DCA.Yields_uds->Write();
        Yields_DCA.Yields_other->Write();
        Yields_DCA.Yields_c->Write();
        Yields_DCA.Yields_cc->Write();
        Yields_DCA.Yields_b->Write();
        Yields_DCA.Yields_bb->Write();
        Yields_DCA.Yields_full->Write();
        Yields_DCA.Chi2ndf->Write();
    }
    if(doYields_invMass) {
        Yields_InvMass.Yields_uds->Write();
        Yields_InvMass.Yields_other->Write();
        Yields_InvMass.Yields_c->Write();
        Yields_InvMass.Yields_cc->Write();
        Yields_InvMass.Yields_b->Write();
        Yields_InvMass.Yields_bb->Write();
        Yields_InvMass.Yields_full->Write();
        Yields_InvMass.Chi2ndf->Write();
    }
    if(doYields_DR) {
        Yields_DR.Yields_uds->Write();
        Yields_DR.Yields_other->Write();
        Yields_DR.Yields_c->Write();
        Yields_DR.Yields_cc->Write();
        Yields_DR.Yields_b->Write();
        Yields_DR.Yields_bb->Write();
        Yields_DR.Yields_full->Write();
        Yields_DR.Chi2ndf->Write();
    }

    // SAVE COMMAND LINE PARAMS
    TNamed paramFile("InputFile", file.c_str());
    paramFile.Write();
    TNamed paramTemplates("Templates", templates.c_str());
    paramTemplates.Write();
    TNamed paramDoYields_DCA("doYields_DCA", doYields_DCA ? "true" : "false");
    paramDoYields_DCA.Write();
    TNamed paramDoYieldsInvMass("doYields_invMass", doYields_invMass ? "true" : "false");
    paramDoYieldsInvMass.Write();
    TNamed paramDoYieldsDR("doYields_DR", doYields_DR ? "true" : "false");
    paramDoYieldsDR.Write();
    TNamed paramMakePlots("makeplots", makeplots ? "true" : "false");
    paramMakePlots.Write();

    if(makeplots){
        
        plotDir->cd();

        // UDS PLOT
        TCanvas* c_uds = new TCanvas("Yields_uds", "", 800, 600);
        //c_uds->SetLogy();
        c_uds->cd();
        
        TLegend* leg_uds = new TLegend(0.55, 0.75, 0.80, 0.88);
        leg_uds->SetHeader("Light Flavor Yields","C");


        if(doYields_DCA){
            Yields_DCA.Yields_uds->SetLineColor(kRed);
            Yields_DCA.Yields_uds->SetMarkerColor(kRed);
            Yields_DCA.Yields_uds->SetMarkerStyle(20);
            Yields_DCA.Yields_uds->SetTitle("Light Flavor Yields");
            Yields_DCA.Yields_uds->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_DCA.Yields_uds->GetYaxis()->SetTitle(" dN / dpt");
            Yields_DCA.Yields_uds->SetMinimum(0);
            leg_uds->AddEntry(Yields_DCA.Yields_uds, "DCA Fit", "lep");
            Yields_DCA.Yields_uds->Draw("E1");
        }
        if(doYields_invMass){
            Yields_InvMass.Yields_uds->SetLineColor(kBlue);
            Yields_InvMass.Yields_uds->SetMarkerColor(kBlue);
            Yields_InvMass.Yields_uds->SetMarkerStyle(21);
            Yields_InvMass.Yields_uds->SetTitle("Light Flavor Yields");
            Yields_InvMass.Yields_uds->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_InvMass.Yields_uds->GetYaxis()->SetTitle(" dN / dpt");
            Yields_InvMass.Yields_uds->SetMinimum(0);
            leg_uds->AddEntry(Yields_InvMass.Yields_uds, "Mass Fit", "lep");
            if(!doYields_DCA) {
                Yields_InvMass.Yields_uds->Draw("E1");
            } else {
                Yields_InvMass.Yields_uds->Draw("E1 SAME");
            }
        }
        if(doYields_DR){
            Yields_DR.Yields_uds->SetLineColor(kGreen+2);
            Yields_DR.Yields_uds->SetMarkerColor(kGreen+2);
            Yields_DR.Yields_uds->SetMarkerStyle(22);
            Yields_DR.Yields_uds->SetTitle("Light Flavor Yields");
            Yields_DR.Yields_uds->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_DR.Yields_uds->GetYaxis()->SetTitle(" dN / dpt");
            Yields_DR.Yields_uds->SetMinimum(0);
            leg_uds->AddEntry(Yields_DR.Yields_uds, "DR Fit", "lep");
            if(!doYields_DCA && !doYields_invMass) {
                Yields_DR.Yields_uds->Draw("E1");
            } else {
                Yields_DR.Yields_uds->Draw("E1 SAME");
            }
        }
        leg_uds->Draw();
        c_uds->Write();
        c_uds->SaveAs("plots/Yields_uds.pdf");
        delete leg_uds;
        delete c_uds;

        // Other 
        TCanvas* c_other = new TCanvas("Yields_other", "", 800, 600);
        //c_other->SetLogy();
        c_other->cd();
        TLegend* leg_other = new TLegend(0.55, 0.75, 0.80, 0.88);
        leg_other->SetHeader("Other Yields","C");  
        if(doYields_DCA){
            Yields_DCA.Yields_other->SetLineColor(kRed);
            Yields_DCA.Yields_other->SetMarkerColor(kRed);
            Yields_DCA.Yields_other->SetMarkerStyle(20);
            Yields_DCA.Yields_other->SetTitle("Other Yields");
            Yields_DCA.Yields_other->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_DCA.Yields_other->GetYaxis()->SetTitle(" dN / dpt");
            Yields_DCA.Yields_other->SetMinimum(0);
            leg_other->AddEntry(Yields_DCA.Yields_other, "DCA Fit", "lep");
            Yields_DCA.Yields_other->Draw("E1");
        }
        if(doYields_invMass){
            Yields_InvMass.Yields_other->SetLineColor(kBlue);
            Yields_InvMass.Yields_other->SetMarkerColor(kBlue);
            Yields_InvMass.Yields_other->SetMarkerStyle(21);
            Yields_InvMass.Yields_other->SetTitle("Other Yields");
            Yields_InvMass.Yields_other->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_InvMass.Yields_other->GetYaxis()->SetTitle(" dN / dpt");
            Yields_InvMass.Yields_other->SetMinimum(0);
            leg_other->AddEntry(Yields_InvMass.Yields_other, "Mass Fit", "lep");
            if(!doYields_DCA) {
                Yields_InvMass.Yields_other->Draw("E1");
            } else {
                Yields_InvMass.Yields_other->Draw("E1 SAME");
            }
        }
        if(doYields_DR){
            Yields_DR.Yields_other->SetLineColor(kGreen+2);
            Yields_DR.Yields_other->SetMarkerColor(kGreen+2);
            Yields_DR.Yields_other->SetMarkerStyle(22);
            Yields_DR.Yields_other->SetTitle("Other Yields");
            Yields_DR.Yields_other->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_DR.Yields_other->GetYaxis()->SetTitle(" dN / dpt");
            Yields_DR.Yields_other->SetMinimum(0);
            leg_other->AddEntry(Yields_DR.Yields_other, "DR Fit", "lep");
            if(!doYields_DCA && !doYields_invMass) {
                Yields_DR.Yields_other->Draw("E1");
            } else {
                Yields_DR.Yields_other->Draw("E1 SAME");
            }
        }
        leg_other->Draw();
        c_other->Write();
        c_other->SaveAs("plots/Yields_other.pdf");
        delete leg_other;
        delete c_other; 

        // C PLOT
        TCanvas* c_c = new TCanvas("Yields_c", "", 800, 600);
        //c_c->SetLogy();
        c_c->cd();
        TLegend* leg_c = new TLegend(0.55, 0.75, 0.80, 0.88);
        leg_c->SetHeader("c Yields","C");  
        if(doYields_DCA){
            Yields_DCA.Yields_c->SetLineColor(kRed);
            Yields_DCA.Yields_c->SetMarkerColor(kRed);
            Yields_DCA.Yields_c->SetMarkerStyle(20);
            Yields_DCA.Yields_c->SetTitle("c Yields");
            Yields_DCA.Yields_c->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_DCA.Yields_c->GetYaxis()->SetTitle(" dN / dpt");
            Yields_DCA.Yields_c->SetMinimum(0);
            leg_c->AddEntry(Yields_DCA.Yields_c, "DCA Fit", "lep");
            Yields_DCA.Yields_c->Draw("E1");
        }
        if(doYields_invMass){
            Yields_InvMass.Yields_c->SetLineColor(kBlue);
            Yields_InvMass.Yields_c->SetMarkerColor(kBlue);
            Yields_InvMass.Yields_c->SetMarkerStyle(21);
            Yields_InvMass.Yields_c->SetTitle("c Yields");
            Yields_InvMass.Yields_c->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_InvMass.Yields_c->GetYaxis()->SetTitle(" dN / dpt");
            Yields_InvMass.Yields_c->SetMinimum(0);
            leg_c->AddEntry(Yields_InvMass.Yields_c, "Mass Fit", "lep");
            if(!doYields_DCA) {
                Yields_InvMass.Yields_c->Draw("E1");
            } else {
                Yields_InvMass.Yields_c->Draw("E1 SAME");
            }
        }
        if(doYields_DR){
            Yields_DR.Yields_c->SetLineColor(kGreen+2);
            Yields_DR.Yields_c->SetMarkerColor(kGreen+2);
            Yields_DR.Yields_c->SetMarkerStyle(22);
            Yields_DR.Yields_c->SetTitle("c Yields");
            Yields_DR.Yields_c->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_DR.Yields_c->GetYaxis()->SetTitle(" dN / dpt");
            Yields_DR.Yields_c->SetMinimum(0);
            leg_c->AddEntry(Yields_DR.Yields_c, "DR Fit", "lep");
            if(!doYields_DCA && !doYields_invMass) {
                Yields_DR.Yields_c->Draw("E1");
            } else {
                Yields_DR.Yields_c->Draw("E1 SAME");
            }
        }
        leg_c->Draw();
        c_c->Write();
        c_c->SaveAs("plots/Yields_c.pdf");
        delete leg_c;
        delete c_c; 

        // CC PLOT
        TCanvas* c_cc = new TCanvas("Yields_cc", "", 800, 600);
        //c_cc->SetLogy();
        c_cc->cd();
        TLegend* leg_cc = new TLegend(0.55, 0.75, 0.80, 0.88);
        leg_cc->SetHeader("cc Yields","C");  
        if(doYields_DCA){
            Yields_DCA.Yields_cc->SetLineColor(kRed);
            Yields_DCA.Yields_cc->SetMarkerColor(kRed);
            Yields_DCA.Yields_cc->SetMarkerStyle(20);
            Yields_DCA.Yields_cc->SetTitle("cc Yields");
            Yields_DCA.Yields_cc->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_DCA.Yields_cc->GetYaxis()->SetTitle(" dN / dpt");
            Yields_DCA.Yields_cc->SetMinimum(0);
            leg_cc->AddEntry(Yields_DCA.Yields_cc, "DCA Fit", "lep");
            Yields_DCA.Yields_cc->Draw("E1");
        }
        if(doYields_invMass){
            Yields_InvMass.Yields_cc->SetLineColor(kBlue);
            Yields_InvMass.Yields_cc->SetMarkerColor(kBlue);
            Yields_InvMass.Yields_cc->SetMarkerStyle(21);
            Yields_InvMass.Yields_cc->SetTitle("cc Yields");
            Yields_InvMass.Yields_cc->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_InvMass.Yields_cc->GetYaxis()->SetTitle(" dN / dpt");
            Yields_InvMass.Yields_cc->SetMinimum(0);
            leg_cc->AddEntry(Yields_InvMass.Yields_cc, "Mass Fit", "lep");
            if(!doYields_DCA) { 
                Yields_InvMass.Yields_cc->Draw("E1");
            } else {
                Yields_InvMass.Yields_cc->Draw("E1 SAME");
            }
        }
        if(doYields_DR){
            Yields_DR.Yields_cc->SetLineColor(kGreen+2);
            Yields_DR.Yields_cc->SetMarkerColor(kGreen+2);
            Yields_DR.Yields_cc->SetMarkerStyle(22);
            Yields_DR.Yields_cc->SetTitle("cc Yields");
            Yields_DR.Yields_cc->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_DR.Yields_cc->GetYaxis()->SetTitle(" dN / dpt");
            Yields_DR.Yields_cc->SetMinimum(0);
            leg_cc->AddEntry(Yields_DR.Yields_cc, "DR Fit", "lep");
            if(!doYields_DCA && !doYields_invMass) {
                Yields_DR.Yields_cc->Draw("E1");
            } else {
                Yields_DR.Yields_cc->Draw("E1 SAME");
            }
        }
        leg_cc->Draw();
        c_cc->Write();
        c_cc->SaveAs("plots/Yields_cc.pdf");
        delete leg_cc;
        delete c_cc;

        // B PLOT
        TCanvas* c_b = new TCanvas("Yields_b", "", 800, 600);
        //c_b->SetLogy();
        c_b->cd();
        TLegend* leg_b = new TLegend(0.55, 0.75, 0.80, 0.88);
        leg_b->SetHeader("b Yields","C");  
        if(doYields_DCA){
            Yields_DCA.Yields_b->SetLineColor(kRed);
            Yields_DCA.Yields_b->SetMarkerColor(kRed);
            Yields_DCA.Yields_b->SetMarkerStyle(20);
            Yields_DCA.Yields_b->SetTitle("b Yields");
            Yields_DCA.Yields_b->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_DCA.Yields_b->GetYaxis()->SetTitle(" dN / dpt");
            Yields_DCA.Yields_b->SetMinimum(0);
            leg_b->AddEntry(Yields_DCA.Yields_b, "DCA Fit", "lep");
            Yields_DCA.Yields_b->Draw("E1");
        }
        if(doYields_invMass){
            Yields_InvMass.Yields_b->SetLineColor(kBlue);
            Yields_InvMass.Yields_b->SetMarkerColor(kBlue);
            Yields_InvMass.Yields_b->SetMarkerStyle(21);
            Yields_InvMass.Yields_b->SetTitle("b Yields");
            Yields_InvMass.Yields_b->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_InvMass.Yields_b->GetYaxis()->SetTitle(" dN / dpt");
            Yields_InvMass.Yields_b->SetMinimum(0);
            leg_b->AddEntry(Yields_InvMass.Yields_b, "Mass Fit", "lep");
            if(!doYields_DCA) {
                Yields_InvMass.Yields_b->Draw("E1");
            } else {
                Yields_InvMass.Yields_b->Draw("E1 SAME");
            }
        }
        if(doYields_DR){
            Yields_DR.Yields_b->SetLineColor(kGreen+2);
            Yields_DR.Yields_b->SetMarkerColor(kGreen+2);
            Yields_DR.Yields_b->SetMarkerStyle(22);
            Yields_DR.Yields_b->SetTitle("b Yields");
            Yields_DR.Yields_b->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_DR.Yields_b->GetYaxis()->SetTitle(" dN / dpt");
            Yields_DR.Yields_b->SetMinimum(0);
            leg_b->AddEntry(Yields_DR.Yields_b, "DR Fit", "lep");
            if(!doYields_DCA && !doYields_invMass) {
                Yields_DR.Yields_b->Draw("E1");
            } else {
                Yields_DR.Yields_b->Draw("E1 SAME");
            }
        }
        leg_b->Draw();
        c_b->Write();
        c_b->SaveAs("plots/Yields_b.pdf");
        delete leg_b;
        delete c_b;

        // BB PLOT
        TCanvas* c_bb = new TCanvas("Yields_bb", "", 800, 600);
        //c_bb->SetLogy();
        c_bb->cd();
        TLegend* leg_bb = new TLegend(0.55, 0.75, 0.80, 0.88);
        leg_bb->SetHeader("bb Yields","C");  
        if(doYields_DCA){
            Yields_DCA.Yields_bb->SetLineColor(kRed);
            Yields_DCA.Yields_bb->SetMarkerColor(kRed);
            Yields_DCA.Yields_bb->SetMarkerStyle(20);
            Yields_DCA.Yields_bb->SetTitle("bb Yields");
            Yields_DCA.Yields_bb->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_DCA.Yields_bb->GetYaxis()->SetTitle(" dN / dpt");
            Yields_DCA.Yields_bb->SetMinimum(0);
            leg_bb->AddEntry(Yields_DCA.Yields_bb, "DCA Fit", "lep");
            Yields_DCA.Yields_bb->Draw("E1");
        }
        if(doYields_invMass){
            Yields_InvMass.Yields_bb->SetLineColor(kBlue);
            Yields_InvMass.Yields_bb->SetMarkerColor(kBlue);
            Yields_InvMass.Yields_bb->SetMarkerStyle(21);
            Yields_InvMass.Yields_bb->SetTitle("bb Yields");
            Yields_InvMass.Yields_bb->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_InvMass.Yields_bb->GetYaxis()->SetTitle(" dN / dpt");
            Yields_InvMass.Yields_bb->SetMinimum(0);
            leg_bb->AddEntry(Yields_InvMass.Yields_bb, "Mass Fit", "lep");
            if(!doYields_DCA) {
                Yields_InvMass.Yields_bb->Draw("E1");
            } else {
                Yields_InvMass.Yields_bb->Draw("E1 SAME");
            }
        }
        if(doYields_DR){
            Yields_DR.Yields_bb->SetLineColor(kGreen+2);
            Yields_DR.Yields_bb->SetMarkerColor(kGreen+2);
            Yields_DR.Yields_bb->SetMarkerStyle(22);
            Yields_DR.Yields_bb->SetTitle("bb Yields");
            Yields_DR.Yields_bb->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_DR.Yields_bb->GetYaxis()->SetTitle(" dN / dpt");
            Yields_DR.Yields_bb->SetMinimum(0);
            leg_bb->AddEntry(Yields_DR.Yields_bb, "DR Fit", "lep");
            if(!doYields_DCA && !doYields_invMass) {
                Yields_DR.Yields_bb->Draw("E1");
            } else {
                Yields_DR.Yields_bb->Draw("E1 SAME");
            }
        }
        leg_bb->Draw();
        c_bb->Write();
        c_bb->SaveAs("plots/Yields_bb.pdf");
        delete leg_bb;
        delete c_bb;

        // FULL PLOT
        TCanvas* c_full = new TCanvas("Yields_full", "", 800, 600);
        //c_full->SetLogy();
        c_full->cd();
        TLegend* leg_full = new TLegend(0.55, 0.75, 0.80, 0.88);
        leg_full->SetHeader("Full Yields","C");  
        if(doYields_DCA){
            Yields_DCA.Yields_full->SetLineColor(kRed);
            Yields_DCA.Yields_full->SetMarkerColor(kRed);
            Yields_DCA.Yields_full->SetMarkerStyle(20);
            Yields_DCA.Yields_full->SetTitle("Full Yields");
            Yields_DCA.Yields_full->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_DCA.Yields_full->GetYaxis()->SetTitle(" dN / dpt");
            Yields_DCA.Yields_full->SetMinimum(0);
            leg_full->AddEntry(Yields_DCA.Yields_full, "DCA Fit", "lep");
            Yields_DCA.Yields_full->Draw("E1");
        }
        if(doYields_invMass){
            Yields_InvMass.Yields_full->SetLineColor(kBlue);
            Yields_InvMass.Yields_full->SetMarkerColor(kBlue);
            Yields_InvMass.Yields_full->SetMarkerStyle(21);
            Yields_InvMass.Yields_full->SetTitle("Full Yields");
            Yields_InvMass.Yields_full->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_InvMass.Yields_full->GetYaxis()->SetTitle(" dN / dpt");
            Yields_InvMass.Yields_full->SetMinimum(0);
            leg_full->AddEntry(Yields_InvMass.Yields_full, "Mass Fit", "lep");
            if(!doYields_DCA) {
                Yields_InvMass.Yields_full->Draw("E1");
            } else {
                Yields_InvMass.Yields_full->Draw("E1 SAME");
            }
        }
        if(doYields_DR){
            Yields_DR.Yields_full->SetLineColor(kGreen+2);
            Yields_DR.Yields_full->SetMarkerColor(kGreen+2);
            Yields_DR.Yields_full->SetMarkerStyle(22);
            Yields_DR.Yields_full->SetTitle("Full Yields");
            Yields_DR.Yields_full->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_DR.Yields_full->GetYaxis()->SetTitle(" dN / dpt");
            Yields_DR.Yields_full->SetMinimum(0);
            leg_full->AddEntry(Yields_DR.Yields_full, "DR Fit", "lep");
            if(!doYields_DCA && !doYields_invMass) {
                Yields_DR.Yields_full->Draw("E1");
            } else {
                Yields_DR.Yields_full->Draw("E1 SAME");
            }
        }
        leg_full->Draw();
        c_full->Write();
        c_full->SaveAs("plots/Yields_full.pdf");
        delete leg_full;
        delete c_full;  
        
        // Chi2/ndf PLOT
        TCanvas* c_chi2 = new TCanvas("Chi2ndf", "", 800, 600);
        c_chi2->cd();
        TLegend* leg_chi2 = new TLegend(0.55, 0.75, 0.80, 0.88);
        leg_chi2->SetHeader("Chi^{2}/ndf","C");  
        if(doYields_DCA){
            Yields_DCA.Chi2ndf->SetLineColor(kRed);
            Yields_DCA.Chi2ndf->SetMarkerColor(kRed);
            Yields_DCA.Chi2ndf->SetMarkerStyle(20);
            Yields_DCA.Chi2ndf->SetTitle("Chi^{2}/ndf");
            Yields_DCA.Chi2ndf->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_DCA.Chi2ndf->GetYaxis()->SetTitle("Chi^{2}/ndf");
            Yields_DCA.Chi2ndf->SetMinimum(0);
            Yields_DCA.Chi2ndf->SetMaximum(5);
            leg_chi2->AddEntry(Yields_DCA.Chi2ndf, "DCA Fit", "lep");

            Yields_DCA.Chi2ndf->Draw("E1");
        }
        if(doYields_invMass){
            Yields_InvMass.Chi2ndf->SetLineColor(kBlue);
            Yields_InvMass.Chi2ndf->SetMarkerColor(kBlue);
            Yields_InvMass.Chi2ndf->SetMarkerStyle(21);
            Yields_InvMass.Chi2ndf->SetTitle("Chi^{2}/ndf");
            Yields_InvMass.Chi2ndf->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_InvMass.Chi2ndf->GetYaxis()->SetTitle("Chi^{2}/ndf");
            Yields_InvMass.Chi2ndf->SetMinimum(0);
            Yields_InvMass.Chi2ndf->SetMaximum(5);
            leg_chi2->AddEntry(Yields_InvMass.Chi2ndf, "Mass Fit", "lep");

            if(!doYields_DCA) {
                Yields_InvMass.Chi2ndf->Draw("E1");
            } else {
                Yields_InvMass.Chi2ndf->Draw("E1 SAME");
            }
        }
        if(doYields_DR){
            Yields_DR.Chi2ndf->SetLineColor(kGreen+2);
            Yields_DR.Chi2ndf->SetMarkerColor(kGreen+2);
            Yields_DR.Chi2ndf->SetMarkerStyle(22);
            Yields_DR.Chi2ndf->SetTitle("Chi^{2}/ndf");
            Yields_DR.Chi2ndf->GetXaxis()->SetTitle("Jet p_{T}");
            Yields_DR.Chi2ndf->GetYaxis()->SetTitle("Chi^{2}/ndf");
            Yields_DR.Chi2ndf->SetMinimum(0);
            Yields_DR.Chi2ndf->SetMaximum(5);
            leg_chi2->AddEntry(Yields_DR.Chi2ndf, "DR Fit", "lep");
            if(!doYields_DCA && !doYields_invMass) {
                Yields_DR.Chi2ndf->Draw("E1");
            } else {
                Yields_DR.Chi2ndf->Draw("E1 SAME");
            }
        }
        leg_chi2->Draw();
        c_chi2->Write();
        c_chi2->SaveAs("plots/Chi2ndf.pdf");
        delete leg_chi2;
        delete c_chi2;  

    
    }

    outputFile->Close();
    input->Close();
    templatesFile->Close();

    return 0;

}