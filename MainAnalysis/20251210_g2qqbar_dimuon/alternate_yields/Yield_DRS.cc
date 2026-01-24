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
#include <RooSimultaneous.h>
#include <RooCategory.h>

#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include <map>

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
    double nHf;
    double nHf_err;
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
    TH1D* Yields_hf;
    TH1D* HF_Fraction;
    TH1D* Chi2ndf;

};

FitResult Yields_HFLF(TFile* datafile, TFile* templatefile, vector<const char*> variables, vector<float> fitrange_min, vector<float> fitrange_max, vector<float> kdes, float Jetptmin, float Jetptmax, int chargesel, TDirectory* plotDir = nullptr){
    
    // TUPLES
    TNtuple* data_nt = (TNtuple*)datafile->Get("ntDimuon");
    TNtuple* nt_uds = (TNtuple*)templatefile->Get("nt_uds");
    TNtuple* nt_other = (TNtuple*)templatefile->Get("nt_other");
    TNtuple* nt_c = (TNtuple*)templatefile->Get("nt_c");
    TNtuple* nt_cc = (TNtuple*)templatefile->Get("nt_cc");
    TNtuple* nt_b = (TNtuple*)templatefile->Get("nt_b");
    TNtuple* nt_bb = (TNtuple*)templatefile->Get("nt_bb");

    RooRealVar jetpt("JetPT", "Jet pT", 0, 5000);
    RooRealVar weight("weight", "weight", 0, 1e10);
    RooRealVar chargeprod("Charge", "Dimuon Charge Product", -2, 2);

    // MAKE VARIABLES AND ARGSETS
    vector<RooRealVar*> fittingvars;
    vector<RooArgSet*> varsets;
    
    for(int i = 0; i < variables.size(); i++){

        const char* variable = variables[i];
        float fit_min = fitrange_min[i];
        float fit_max = fitrange_max[i];
        float kde_width = kdes[i];

        RooRealVar* fittingvar = new RooRealVar(variable, variable, fit_min, fit_max);
        fittingvar->setRange("fitRange", fit_min, fit_max);
        fittingvar->setRange("normRange", fit_min, fit_max);
        fittingvars.push_back(fittingvar);
        RooArgSet* vars = new RooArgSet(*fittingvar, jetpt, weight, chargeprod);
        varsets.push_back(vars);
        
    }

    // DATA
    vector<RooDataSet*> data_sets;
    for(int i = 0; i < variables.size(); i++){
        RooDataSet* data;
        if(chargesel == 0){
            data = new RooDataSet(Form("data_%d", i), "data", data_nt, *varsets[i], Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
        }
        else{
            data = new RooDataSet(Form("data_%d", i), "data", data_nt, *varsets[i], Form("JetPT < %f && JetPT >= %f && Charge == %d", Jetptmax, Jetptmin, chargesel), "weight");
        }
        data_sets.push_back(data);
    }

    // TEMPLATES
    vector<RooDataSet*> template_uds_sets;
    vector<RooDataSet*> template_other_sets;
    vector<RooDataSet*> template_c_sets;
    vector<RooDataSet*> template_cc_sets;
    vector<RooDataSet*> template_b_sets;
    vector<RooDataSet*> template_bb_sets;
    for(int i = 0; i < variables.size(); i++){
        if(chargesel == 0){
            template_uds_sets.push_back(new RooDataSet(Form("tmp_uds_%d", i), "", nt_uds, *varsets[i], Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight"));
            template_other_sets.push_back(new RooDataSet(Form("tmp_other_%d", i), "", nt_other, *varsets[i], Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight"));
            template_c_sets.push_back(new RooDataSet(Form("tmp_c_%d", i), "", nt_c, *varsets[i], Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight"));
            template_cc_sets.push_back(new RooDataSet(Form("tmp_cc_%d", i), "", nt_cc, *varsets[i], Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight"));
            template_b_sets.push_back(new RooDataSet(Form("tmp_b_%d", i), "", nt_b, *varsets[i], Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight"));
            template_bb_sets.push_back(new RooDataSet(Form("tmp_bb_%d", i), "", nt_bb, *varsets[i], Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight"));
        }
        else{
            template_uds_sets.push_back(new RooDataSet(Form("tmp_uds_%d", i), "", nt_uds, *varsets[i], Form("JetPT < %f && JetPT >= %f && Charge == %d", Jetptmax, Jetptmin, chargesel), "weight"));
            template_other_sets.push_back(new RooDataSet(Form("tmp_other_%d", i), "", nt_other, *varsets[i], Form("JetPT < %f && JetPT >= %f && Charge == %d", Jetptmax, Jetptmin, chargesel), "weight"));
            template_c_sets.push_back(new RooDataSet(Form("tmp_c_%d", i), "", nt_c, *varsets[i], Form("JetPT < %f && JetPT >= %f && Charge == %d", Jetptmax, Jetptmin, chargesel), "weight"));
            template_cc_sets.push_back(new RooDataSet(Form("tmp_cc_%d", i), "", nt_cc, *varsets[i], Form("JetPT < %f && JetPT >= %f && Charge == %d", Jetptmax, Jetptmin, chargesel), "weight"));
            template_b_sets.push_back(new RooDataSet(Form("tmp_b_%d", i), "", nt_b, *varsets[i], Form("JetPT < %f && JetPT >= %f && Charge == %d", Jetptmax, Jetptmin, chargesel), "weight"));
            template_bb_sets.push_back(new RooDataSet(Form("tmp_bb_%d", i), "", nt_bb, *varsets[i], Form("JetPT < %f && JetPT >= %f && Charge == %d", Jetptmax, Jetptmin, chargesel), "weight"));
        }
    }

    vector<RooDataSet*> template_hf_sets;
    for(int i = 0; i < variables.size(); i++){
        RooDataSet* template_hf = new RooDataSet(Form("template_hf_%d", i), "Heavy flavor", *fittingvars[i]);
        template_hf->append(*template_other_sets[i]);
        template_hf->append(*template_c_sets[i]);
        template_hf->append(*template_cc_sets[i]);
        template_hf->append(*template_b_sets[i]);
        template_hf->append(*template_bb_sets[i]);
        template_hf_sets.push_back(template_hf);
    }

    // PDFs
    vector<RooKeysPdf*> pdf_uds;
    vector<RooKeysPdf*> pdf_hf;
    for(int i = 0; i < variables.size(); i++){
        RooKeysPdf* lfPdf = new RooKeysPdf(Form("lfPdf_%d", i), "Light flavor PDF", *fittingvars[i], *template_uds_sets[i], RooKeysPdf::MirrorLeft, kdes[i]);
        RooKeysPdf* hfPdf = new RooKeysPdf(Form("hfPdf_%d", i), "Heavy flavor PDF", *fittingvars[i], *template_hf_sets[i], RooKeysPdf::MirrorLeft, kdes[i]);
        pdf_uds.push_back(lfPdf);
        pdf_hf.push_back(hfPdf);
    }

    // YIELD VARIABLES (shared across all fits)
    double nTotal = data_sets[0]->sumEntries();
    cout << "Total data entries: " << nTotal << endl;
    
    RooRealVar n_LF("n_LF", "N light flavor", nTotal/2, 0, nTotal);
    RooRealVar n_HF("n_HF", "N heavy flavor", nTotal/2, 0, nTotal);
    
    // BUILD INDIVIDUAL PDFS FOR EACH VARIABLE
    vector<RooAddPdf*> models;
    for(int i = 0; i < variables.size(); i++){
        RooAddPdf* model = new RooAddPdf(Form("model_%d", i), "LF+HF", 
            RooArgList(*pdf_uds[i], *pdf_hf[i]), 
            RooArgList(n_LF, n_HF));
        model->fixCoefNormalization(RooArgSet(*fittingvars[i]));
        models.push_back(model);
    }
    
    // BUILD SIMULTANEOUS FITTING PDF + FIT
    RooCategory sample("sample", "sample");
    for(int i = 0; i < variables.size(); i++){
        sample.defineType(variables[i]);
    }
    RooSimultaneous simPdf("simPdf", "Simultaneous PDF", sample);
    for(int i = 0; i < variables.size(); i++){
        simPdf.addPdf(*models[i], variables[i]);
    }
    map<string, RooDataSet*> dataMap;
    for(int i = 0; i < variables.size(); i++){
        dataMap[variables[i]] = data_sets[i];
    }
    RooDataSet combData("combData", "Combined data", RooArgSet(*fittingvars[0]), RooFit::Index(sample), RooFit::Import(dataMap));
    RooFitResult* fitResult = simPdf.fitTo(combData, RooFit::Save(), RooFit::PrintLevel(1), RooFit::Verbose(kTRUE));

    cout << "========== SIMULTANEOUS FIT RESULTS ==========" << endl;
    cout << "Light flavor yield: " << n_LF.getVal() << " +/- " << n_LF.getError() << endl;
    cout << "Heavy flavor yield: " << n_HF.getVal() << " +/- " << n_HF.getError() << endl;
    cout << "Total: " << n_LF.getVal() + n_HF.getVal() << endl;

    // CREATE FIT PLOTS FOR EACH VARIABLE
    vector<double> chi2_ndfs;
    if(plotDir != nullptr){
        plotDir->cd();
        
        for(int i = 0; i < variables.size(); i++){
            const char* varname = variables[i];
            
            // Create canvas with 2 panels (fit + pull)
            TCanvas* c = new TCanvas(Form("LFHF_Fit_%s_pt%.0f_%.0f", varname, Jetptmin, Jetptmax), "", 800, 800);
            c->Divide(1,2);
            
            // Top panel: fit
            c->cd(1);
            gPad->SetPad(0.0, 0.3, 1.0, 1.0);
            gPad->SetBottomMargin(0.02);
            
            RooPlot* frame = fittingvars[i]->frame(RooFit::Title(Form("%s LF+HF Fit (%.0f < p_{T} < %.0f GeV)", varname, Jetptmin, Jetptmax)), RooFit::Bins(30));
            data_sets[i]->plotOn(frame, RooFit::Name("data"));
            models[i]->plotOn(frame, RooFit::Name("model"));
            
            // Plot individual components
            models[i]->plotOn(frame, RooFit::Components(*pdf_uds[i]), RooFit::LineStyle(kDashed), RooFit::LineColor(kRed), RooFit::Name("LF"));
            models[i]->plotOn(frame, RooFit::Components(*pdf_hf[i]), RooFit::LineStyle(kDashed), RooFit::LineColor(kGreen + 2), RooFit::Name("HF"));
            
            double chi2_ndf = frame->chiSquare("model", "data", 2);
            chi2_ndfs.push_back(chi2_ndf);
            
            frame->Draw();
            
            // Legend
            TLegend* leg = new TLegend(0.55, 0.65, 0.80, 0.88);
            leg->AddEntry(frame->findObject("data"), "Data", "lep");
            leg->AddEntry(frame->findObject("model"), "Total Fit", "l");
            leg->AddEntry(frame->findObject("LF"), "Light Flavor", "l");
            leg->AddEntry(frame->findObject("HF"), "Heavy Flavor", "l");
            leg->Draw();
            
            // Bottom panel: pull
            c->cd(2);
            gPad->SetPad(0.0, 0.0, 1.0, 0.3);
            gPad->SetTopMargin(0.02);
            gPad->SetBottomMargin(0.3);
            
            RooPlot* framePull = fittingvars[i]->frame(RooFit::Title(""));
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
            
            TLine* line0 = new TLine(framePull->GetXaxis()->GetXmin(), 0, framePull->GetXaxis()->GetXmax(), 0);
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
            c->SaveAs(Form("plots/LFHF_Fit_%s_pt%.0f_%.0f.pdf", varname, Jetptmin, Jetptmax));
            
            delete leg;
            delete c;
        }
        
        // Print chi2 summary
        cout << "Chi^2/ndf for each variable:" << endl;
        for(int i = 0; i < variables.size(); i++){
            cout << "  " << variables[i] << ": " << chi2_ndfs[i] << endl;
        }
    } else {
        // Calculate chi2 even when not plotting
        for(int i = 0; i < variables.size(); i++){
            RooPlot* frame = fittingvars[i]->frame();
            data_sets[i]->plotOn(frame, RooFit::Name("data"));
            models[i]->plotOn(frame, RooFit::Name("model"));
            double chi2_ndf = frame->chiSquare("model", "data", 2);
            chi2_ndfs.push_back(chi2_ndf);
            delete frame;
        }
    }
    
    // Calculate average chi2
    double avg_chi2 = 0;
    for(auto chi2 : chi2_ndfs) avg_chi2 += chi2;
    avg_chi2 /= chi2_ndfs.size();

    // Cleanup: delete all dynamically allocated objects to avoid memory leaks
    delete fitResult;
    for(auto* model : models) delete model;
    for(auto* pdf : pdf_hf) delete pdf;
    for(auto* pdf : pdf_uds) delete pdf;
    for(auto* ds : template_hf_sets) delete ds;
    for(auto* ds : template_bb_sets) delete ds;
    for(auto* ds : template_b_sets) delete ds;
    for(auto* ds : template_cc_sets) delete ds;
    for(auto* ds : template_c_sets) delete ds;
    for(auto* ds : template_other_sets) delete ds;
    for(auto* ds : template_uds_sets) delete ds;
    for(auto* ds : data_sets) delete ds;
    for(auto* vs : varsets) delete vs;
    for(auto* var : fittingvars) delete var;

    return {n_LF.getVal(), n_LF.getError(), 
            0.0, 0.0,  // no "other" in LF+HF fit
            0.0, 0.0,  // no separate c
            0.0, 0.0,  // no separate cc
            0.0, 0.0,  // no separate b
            0.0, 0.0,  // no separate bb
            n_HF.getVal(), n_HF.getError(),
            avg_chi2};
}

FitResult Yields_Float(TFile* datafile, TFile* templatefile, vector<const char*> variables, vector<float> fitrange_min, vector<float> fitrange_max, vector<float> kdes, float Jetptmin, float Jetptmax, int chargesel, TDirectory* plotDir = nullptr){

    // TUPLES
    TNtuple* data_nt = (TNtuple*)datafile->Get("ntDimuon");
    TNtuple* nt_uds = (TNtuple*)templatefile->Get("nt_uds");
    TNtuple* nt_other = (TNtuple*)templatefile->Get("nt_other");
    TNtuple* nt_c = (TNtuple*)templatefile->Get("nt_c");
    TNtuple* nt_cc = (TNtuple*)templatefile->Get("nt_cc");
    TNtuple* nt_b = (TNtuple*)templatefile->Get("nt_b");
    TNtuple* nt_bb = (TNtuple*)templatefile->Get("nt_bb");

    RooRealVar jetpt("JetPT", "Jet pT", 0, 5000);
    RooRealVar weight("weight", "weight", 0, 1e10);
    RooRealVar chargeprod("Charge", "Dimuon Charge Product", -2, 2);

    // MAKE VARIABLES AND ARGSETS
    vector<RooRealVar*> fittingvars;
    vector<RooArgSet*> varsets;
    
    for(int i = 0; i < variables.size(); i++){

        const char* variable = variables[i];
        float fit_min = fitrange_min[i];
        float fit_max = fitrange_max[i];
        float kde_width = kdes[i];

        RooRealVar* fittingvar = new RooRealVar(variable, variable, fit_min, fit_max);
        fittingvar->setRange("fitRange", fit_min, fit_max);
        fittingvar->setRange("normRange", fit_min, fit_max);
        fittingvars.push_back(fittingvar);
        RooArgSet* vars = new RooArgSet(*fittingvar, jetpt, weight, chargeprod);
        varsets.push_back(vars);
        
    }

    // DATA
    vector<RooDataSet*> data_sets;
    for(int i = 0; i < variables.size(); i++){
        RooDataSet* data;
        if(chargesel == 0){
            data = new RooDataSet(Form("data_%d", i), "data", data_nt, *varsets[i], Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
        }
        else{
            data = new RooDataSet(Form("data_%d", i), "data", data_nt, *varsets[i], Form("JetPT < %f && JetPT >= %f && Charge == %d", Jetptmax, Jetptmin, chargesel), "weight");
        }
        data_sets.push_back(data);
    }

    // TEMPLATES
    vector<RooDataSet*> template_uds_sets;
    vector<RooDataSet*> template_other_sets;
    vector<RooDataSet*> template_c_sets;
    vector<RooDataSet*> template_cc_sets;
    vector<RooDataSet*> template_b_sets;
    vector<RooDataSet*> template_bb_sets;
    for(int i = 0; i < variables.size(); i++){
        if(chargesel == 0){
            template_uds_sets.push_back(new RooDataSet(Form("tmp_uds_%d", i), "", nt_uds, *varsets[i], Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight"));
            template_other_sets.push_back(new RooDataSet(Form("tmp_other_%d", i), "", nt_other, *varsets[i], Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight"));
            template_c_sets.push_back(new RooDataSet(Form("tmp_c_%d", i), "", nt_c, *varsets[i], Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight"));
            template_cc_sets.push_back(new RooDataSet(Form("tmp_cc_%d", i), "", nt_cc, *varsets[i], Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight"));
            template_b_sets.push_back(new RooDataSet(Form("tmp_b_%d", i), "", nt_b, *varsets[i], Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight"));
            template_bb_sets.push_back(new RooDataSet(Form("tmp_bb_%d", i), "", nt_bb, *varsets[i], Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight"));
        }
        else{
            template_uds_sets.push_back(new RooDataSet(Form("tmp_uds_%d", i), "", nt_uds, *varsets[i], Form("JetPT < %f && JetPT >= %f && Charge == %d", Jetptmax, Jetptmin, chargesel), "weight"));
            template_other_sets.push_back(new RooDataSet(Form("tmp_other_%d", i), "", nt_other, *varsets[i], Form("JetPT < %f && JetPT >= %f && Charge == %d", Jetptmax, Jetptmin, chargesel), "weight"));
            template_c_sets.push_back(new RooDataSet(Form("tmp_c_%d", i), "", nt_c, *varsets[i], Form("JetPT < %f && JetPT >= %f && Charge == %d", Jetptmax, Jetptmin, chargesel), "weight"));
            template_cc_sets.push_back(new RooDataSet(Form("tmp_cc_%d", i), "", nt_cc, *varsets[i], Form("JetPT < %f && JetPT >= %f && Charge == %d", Jetptmax, Jetptmin, chargesel), "weight"));
            template_b_sets.push_back(new RooDataSet(Form("tmp_b_%d", i), "", nt_b, *varsets[i], Form("JetPT < %f && JetPT >= %f && Charge == %d", Jetptmax, Jetptmin, chargesel), "weight"));
            template_bb_sets.push_back(new RooDataSet(Form("tmp_bb_%d", i), "", nt_bb, *varsets[i], Form("JetPT < %f && JetPT >= %f && Charge == %d", Jetptmax, Jetptmin, chargesel), "weight"));
        }
    }
    
    // PDFs 
    vector<RooKeysPdf*> pdf_uds;
    vector<RooKeysPdf*> pdf_other;
    vector<RooKeysPdf*> pdf_c;
    vector<RooKeysPdf*> pdf_cc;
    vector<RooKeysPdf*> pdf_b;
    vector<RooKeysPdf*> pdf_bb;

    for(int i = 0; i < variables.size(); i++){
        pdf_uds.push_back(new RooKeysPdf(Form("pdf_uds_%d", i), "uds PDF", *fittingvars[i], *template_uds_sets[i], RooKeysPdf::MirrorLeft, kdes[i]));
        pdf_other.push_back(new RooKeysPdf(Form("pdf_other_%d", i), "other PDF", *fittingvars[i], *template_other_sets[i], RooKeysPdf::MirrorLeft, kdes[i]));
        pdf_c.push_back(new RooKeysPdf(Form("pdf_c_%d", i), "c PDF", *fittingvars[i], *template_c_sets[i], RooKeysPdf::MirrorLeft, kdes[i]));
        pdf_cc.push_back(new RooKeysPdf(Form("pdf_cc_%d", i), "cc PDF", *fittingvars[i], *template_cc_sets[i], RooKeysPdf::MirrorLeft, kdes[i]));
        pdf_b.push_back(new RooKeysPdf(Form("pdf_b_%d", i), "b PDF", *fittingvars[i], *template_b_sets[i], RooKeysPdf::MirrorLeft, kdes[i]));
        pdf_bb.push_back(new RooKeysPdf(Form("pdf_bb_%d", i), "bb PDF", *fittingvars[i], *template_bb_sets[i], RooKeysPdf::MirrorLeft, kdes[i]));
    }

    // YIELD VARIABLES (shared across all fits)
    double nTotal = data_sets[0]->sumEntries();
    cout << "Total data entries: " << nTotal << endl;
    
    RooRealVar n_uds("n_uds", "n_uds", nTotal/4, 0, nTotal);
    RooRealVar n_other("n_other", "n_other", 0, 0, nTotal);
    RooRealVar n_c("n_c", "n_c", 0, 0, nTotal);
    RooRealVar n_cc("n_cc", "n_cc", nTotal/4, 0, nTotal);
    RooRealVar n_b("n_b", "n_b", nTotal/4, 0, nTotal);
    RooRealVar n_bb("n_bb", "n_bb", nTotal/4, 0, nTotal);
    
    // Print template entries for debugging
    cout << "Template entries:" << endl;
    cout << "  uds: " << template_uds_sets[0]->sumEntries() << endl;
    cout << "  other: " << template_other_sets[0]->sumEntries() << endl;
    cout << "  c: " << template_c_sets[0]->sumEntries() << endl;
    cout << "  cc: " << template_cc_sets[0]->sumEntries() << endl;
    cout << "  b: " << template_b_sets[0]->sumEntries() << endl;
    cout << "  bb: " << template_bb_sets[0]->sumEntries() << endl;

    // BUILD INDIVIDUAL PDFS FOR EACH VARIABLE
    vector<RooAddPdf*> model_pdfs;
    for(int i = 0; i < variables.size(); i++){
        RooAddPdf* model = new RooAddPdf(Form("model_%d", i), "Full Fit", 
        RooArgList(*pdf_uds[i], *pdf_other[i], *pdf_c[i], *pdf_cc[i], *pdf_b[i], *pdf_bb[i]), 
        RooArgList(n_uds, n_other, n_c, n_cc, n_b, n_bb));
        model->fixCoefNormalization(RooArgSet(*fittingvars[i]));
        model_pdfs.push_back(model);
    }
    RooCategory sample("sample", "sample");
    for(int i = 0; i < variables.size(); i++){
        sample.defineType(variables[i]);
    }
    RooSimultaneous simPdf("simPdf", "Simultaneous PDF", sample);
    for(int i = 0; i < variables.size(); i++){
        simPdf.addPdf(*model_pdfs[i], variables[i]);
    }

    // COMBINE + FIT
    map<string, RooDataSet*> dataMap;
    for(int i = 0; i < variables.size(); i++){
        dataMap[variables[i]] = data_sets[i];
    }
    RooDataSet combData("combData", "Combined data", RooArgSet(*fittingvars[0]), RooFit::Index(sample), RooFit::Import(dataMap));
    RooFitResult* fitResult = simPdf.fitTo(combData, RooFit::Save(), RooFit::PrintLevel(1), RooFit::Verbose(kTRUE));

    cout << "========== SIMULTANEOUS FIT RESULTS ==========" << endl;
    cout << "Light flavor (uds) yield: " << n_uds.getVal() << " +/- " << n_uds.getError() << endl;
    cout << "other yield: " << n_other.getVal() << " +/- " << n_other.getError() << endl;
    cout << "c yield: " << n_c.getVal() << " +/- " << n_c.getError() << endl;
    cout << "cc yield: " << n_cc.getVal() << " +/- " << n_cc.getError() << endl;
    cout << "b yield: " << n_b.getVal() << " +/- " << n_b.getError() << endl;
    cout << "bb yield: " << n_bb.getVal() << " +/- " << n_bb.getError() << endl;
    cout << "Total: " << n_uds.getVal() + n_cc.getVal() + n_b.getVal() + n_bb.getVal() << endl;

    // CREATE FIT PLOTS FOR EACH VARIABLE
    vector<double> chi2_ndfs;
    if(plotDir != nullptr){
        plotDir->cd();
        
        for(int i = 0; i < variables.size(); i++){
            const char* varname = variables[i];
            
            // Create canvas with 2 panels (fit + pull)
            TCanvas* c = new TCanvas(Form("SimultaneousFit_%s_pt%.0f_%.0f", varname, Jetptmin, Jetptmax), "", 800, 800);
            c->Divide(1,2);
            
            // Top panel: fit
            c->cd(1);
            gPad->SetPad(0.0, 0.3, 1.0, 1.0);
            gPad->SetBottomMargin(0.02);
            
            RooPlot* frame = fittingvars[i]->frame(RooFit::Title(Form("%s Simultaneous Fit (%.0f < p_{T} < %.0f GeV)", varname, Jetptmin, Jetptmax)), RooFit::Bins(30));
            data_sets[i]->plotOn(frame, RooFit::Name("data"));
            model_pdfs[i]->plotOn(frame, RooFit::Name("model"));
            
            // Plot individual components
            model_pdfs[i]->plotOn(frame, RooFit::Components(*pdf_uds[i]), RooFit::LineStyle(kDashed), RooFit::LineColor(kRed), RooFit::Name("uds"));
            model_pdfs[i]->plotOn(frame, RooFit::Components(*pdf_other[i]), RooFit::LineStyle(kDashed), RooFit::LineColor(kGreen+2), RooFit::Name("other"));
            model_pdfs[i]->plotOn(frame, RooFit::Components(*pdf_c[i]), RooFit::LineStyle(kDashed), RooFit::LineColor(kYellow+2), RooFit::Name("c"));
            model_pdfs[i]->plotOn(frame, RooFit::Components(*pdf_cc[i]), RooFit::LineStyle(kDashed), RooFit::LineColor(kMagenta), RooFit::Name("cc"));
            model_pdfs[i]->plotOn(frame, RooFit::Components(*pdf_b[i]), RooFit::LineStyle(kDashed), RooFit::LineColor(kCyan), RooFit::Name("b"));
            model_pdfs[i]->plotOn(frame, RooFit::Components(*pdf_bb[i]), RooFit::LineStyle(kDashed), RooFit::LineColor(kOrange), RooFit::Name("bb"));
            
            double chi2_ndf = frame->chiSquare("model", "data", 6);
            chi2_ndfs.push_back(chi2_ndf);
            
            frame->Draw();
            
            // Legend
            TLegend* leg = new TLegend(0.55, 0.55, 0.80, 0.88);
            leg->AddEntry(frame->findObject("data"), "Data", "lep");
            leg->AddEntry(frame->findObject("model"), "Total Fit", "l");
            leg->AddEntry(frame->findObject("uds"), "uds", "l");
            leg->AddEntry(frame->findObject("other"), "other", "l");
            leg->AddEntry(frame->findObject("c"), "c", "l");
            leg->AddEntry(frame->findObject("cc"), "cc", "l");
            leg->AddEntry(frame->findObject("b"), "b", "l");
            leg->AddEntry(frame->findObject("bb"), "bb", "l");
            leg->Draw();
            
            // Bottom panel: pull
            c->cd(2);
            gPad->SetPad(0.0, 0.0, 1.0, 0.3);
            gPad->SetTopMargin(0.02);
            gPad->SetBottomMargin(0.3);
            
            RooPlot* framePull = fittingvars[i]->frame(RooFit::Title(""));
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
            
            TLine* line0 = new TLine(framePull->GetXaxis()->GetXmin(), 0, framePull->GetXaxis()->GetXmax(), 0);
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
            c->SaveAs(Form("plots/SimultaneousFit_%s_pt%.0f_%.0f.pdf", varname, Jetptmin, Jetptmax));
            
            delete leg;
            delete c;
        }
        
        // Print chi2 summary
        cout << "Chi^2/ndf for each variable:" << endl;
        for(int i = 0; i < variables.size(); i++){
            cout << "  " << variables[i] << ": " << chi2_ndfs[i] << endl;
        }
    } else {
        // Calculate chi2 even when not plotting
        for(int i = 0; i < variables.size(); i++){
            RooPlot* frame = fittingvars[i]->frame();
            data_sets[i]->plotOn(frame, RooFit::Name("data"));
            model_pdfs[i]->plotOn(frame, RooFit::Name("model"));
            double chi2_ndf = frame->chiSquare("model", "data", 6);
            chi2_ndfs.push_back(chi2_ndf);
            delete frame;
        }
    }
    
    // Calculate average chi2
    double avg_chi2 = 0;
    for(auto chi2 : chi2_ndfs) avg_chi2 += chi2;
    avg_chi2 /= chi2_ndfs.size();

    // Cleanup: delete all dynamically allocated objects to avoid memory leaks
    delete fitResult;
    for(auto* model : model_pdfs) delete model;
    for(auto* pdf : pdf_bb) delete pdf;
    for(auto* pdf : pdf_b) delete pdf;
    for(auto* pdf : pdf_cc) delete pdf;
    for(auto* pdf : pdf_c) delete pdf;
    for(auto* pdf : pdf_other) delete pdf;
    for(auto* pdf : pdf_uds) delete pdf;
    for(auto* ds : template_bb_sets) delete ds;
    for(auto* ds : template_b_sets) delete ds;
    for(auto* ds : template_cc_sets) delete ds;
    for(auto* ds : template_c_sets) delete ds;
    for(auto* ds : template_other_sets) delete ds;
    for(auto* ds : template_uds_sets) delete ds;
    for(auto* ds : data_sets) delete ds;
    for(auto* vs : varsets) delete vs;
    for(auto* var : fittingvars) delete var;

    return {n_uds.getVal(), n_uds.getError(), 
            n_other.getVal(), n_other.getError(), 
            n_c.getVal(), n_c.getError(), 
            n_cc.getVal(), n_cc.getError(), 
            n_b.getVal(), n_b.getError(), 
            n_bb.getVal(), n_bb.getError(), 
            n_c.getVal() + n_cc.getVal() + n_b.getVal() + n_bb.getVal(),
            sqrt(pow(n_c.getError(),2) + pow(n_cc.getError(),2) + pow(n_b.getError(),2) + pow(n_bb.getError(),2)), // HF error (not calculated here)
            avg_chi2};
}

void FillHists(FitResult fitResult, FittingHists& hists, int ptBinIndex, float binwidth) {
    
    hists.Yields_uds->SetBinContent(ptBinIndex, fitResult.nLF / binwidth);
    hists.Yields_uds->SetBinError(ptBinIndex, fitResult.nLF_err / binwidth);
    hists.Yields_other->SetBinContent(ptBinIndex, fitResult.nOther / binwidth);
    hists.Yields_other->SetBinError(ptBinIndex, fitResult.nOther_err / binwidth);
    hists.Yields_c->SetBinContent(ptBinIndex, fitResult.nC / binwidth);
    hists.Yields_c->SetBinError(ptBinIndex, fitResult.nC_err / binwidth);
    hists.Yields_cc->SetBinContent(ptBinIndex, fitResult.nCC / binwidth);
    hists.Yields_cc->SetBinError(ptBinIndex, fitResult.nCC_err / binwidth);
    hists.Yields_b->SetBinContent(ptBinIndex, fitResult.nB / binwidth);
    hists.Yields_b->SetBinError(ptBinIndex, fitResult.nB_err / binwidth);
    hists.Yields_bb->SetBinContent(ptBinIndex, fitResult.nBB / binwidth);
    hists.Yields_bb->SetBinError(ptBinIndex, fitResult.nBB_err / binwidth);
    hists.Yields_full->SetBinContent(ptBinIndex, (fitResult.nLF + fitResult.nOther + fitResult.nC + fitResult.nCC + fitResult.nB + fitResult.nBB) / binwidth);
    hists.Yields_full->SetBinError(ptBinIndex, sqrt(pow(fitResult.nLF_err,2) + pow(fitResult.nOther_err,2) + pow(fitResult.nC_err,2) + pow(fitResult.nCC_err,2) + pow(fitResult.nB_err,2) + pow(fitResult.nBB_err,2)) / binwidth);
    hists.Yields_hf->SetBinContent(ptBinIndex, fitResult.nHf / binwidth);
    hists.Yields_hf->SetBinError(ptBinIndex, fitResult.nHf_err / binwidth);
    
    // Calculate HF fraction with proper error propagation
    // For f = A/(A+B), σ_f^2 = [(1-f)/(A+B)]^2 * σ_A^2 + [f/(A+B)]^2 * σ_B^2
    double fraction = fitResult.nHf / (fitResult.nHf + fitResult.nLF);
    double denom = fitResult.nHf + fitResult.nLF;
    double fraction_err = sqrt(pow((1.0 - fraction) / denom * fitResult.nHf_err, 2) + 
                               pow(fraction / denom * fitResult.nLF_err, 2));
    
    hists.HF_Fraction->SetBinContent(ptBinIndex, fraction);
    hists.HF_Fraction->SetBinError(ptBinIndex, fraction_err);
    hists.Chi2ndf->SetBinContent(ptBinIndex, fitResult.chi2_ndf);

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
    bool makeplots = CL.GetBool("makeplots", true);

    // IMPORT FILES 
    TFile* input = TFile::Open(file.c_str());
    TFile* templatesFile = TFile::Open(templates.c_str());

    // DECLARE HISTOGRAMS
    TFile* outputFile = new TFile(output.c_str(), "RECREATE");
    outputFile->cd();
    
    FittingHists Yields;
    
    // Initialize histograms
    int nBins = ptBins.size() - 1;
    Yields.Yields_uds = new TH1D("Yields_uds", "uds Yields", nBins, ptBins.data());
    Yields.Yields_other = new TH1D("Yields_other", "other Yields", nBins, ptBins.data());
    Yields.Yields_c = new TH1D("Yields_c", "c Yields", nBins, ptBins.data());
    Yields.Yields_cc = new TH1D("Yields_cc", "cc Yields", nBins, ptBins.data());
    Yields.Yields_b = new TH1D("Yields_b", "b Yields", nBins, ptBins.data());
    Yields.Yields_bb = new TH1D("Yields_bb", "bb Yields", nBins, ptBins.data());
    Yields.Yields_full = new TH1D("Yields_full", "Full Yields", nBins, ptBins.data());
    Yields.Yields_hf = new TH1D("Yields_hf", "Heavy Flavor Yields", nBins, ptBins.data());
    Yields.HF_Fraction = new TH1D("HF_Fraction", "HF Fraction", nBins, ptBins.data());
    Yields.Chi2ndf = new TH1D("Chi2ndf", "Chi^{2}/ndf", nBins, ptBins.data());

    // CREATE PLOTS DIRECTORY
    TDirectory* plotDir = nullptr;
    if(makeplots) {
        plotDir = outputFile->mkdir("plots");
    }

    for(int i = 0; i < ptBins.size() - 1; i++) {
        float ptMin = ptBins[i];
        float ptMax = ptBins[i+1];

        vector<const char*> variables = {"muDR"};
        vector<float> fitrange_min = {0.0};
        vector<float> fitrange_max = {0.6};
        vector<float> kdes = {1.6};

        FitResult result = Yields_HFLF(input, templatesFile, variables, fitrange_min, fitrange_max, kdes, ptMin, ptMax, 1, plotDir);
        float binwidth = ptMax - ptMin;

        // Fill histograms
        FillHists(result, Yields, i + 1, binwidth);

    }

    // WRITE TO FILE
    outputFile->cd();
    Yields.Yields_uds->Write("Yields_uds");
    Yields.Yields_other->Write("Yields_other");
    Yields.Yields_c->Write("Yields_c");
    Yields.Yields_cc->Write("Yields_cc");
    Yields.Yields_b->Write("Yields_b");
    Yields.Yields_bb->Write("Yields_bb");
    Yields.Yields_full->Write("Yields_full");
    Yields.Yields_hf->Write("Yields_hf");
    Yields.HF_Fraction->Write("HF_Fraction");
    Yields.Chi2ndf->Write("Chi2ndf");

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
        
        Yields.Yields_uds->SetLineColor(kBlack);
        Yields.Yields_uds->SetMarkerColor(kBlack);
        Yields.Yields_uds->SetMarkerStyle(20);
        Yields.Yields_uds->SetTitle("Light Flavor Yields (Simultaneous Fit)");
        Yields.Yields_uds->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        Yields.Yields_uds->GetYaxis()->SetTitle("dN / dp_{T}");
        Yields.Yields_uds->SetMinimum(0);
        Yields.Yields_uds->Draw("E1");
        c_uds->Write();
        c_uds->SaveAs("plots/Yields_uds.pdf");
        delete c_uds;

        // Other 
        TCanvas* c_other = new TCanvas("Yields_other", "", 800, 600);
        c_other->cd();
        Yields.Yields_other->SetLineColor(kBlack);
        Yields.Yields_other->SetMarkerColor(kBlack);
        Yields.Yields_other->SetMarkerStyle(20);
        Yields.Yields_other->SetTitle("Other Yields (Simultaneous Fit)");
        Yields.Yields_other->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        Yields.Yields_other->GetYaxis()->SetTitle("dN / dp_{T}");
        Yields.Yields_other->SetMinimum(0);
        Yields.Yields_other->Draw("E1");
        c_other->Write();
        c_other->SaveAs("plots/Yields_other.pdf");
        delete c_other; 

        // C PLOT
        TCanvas* c_c = new TCanvas("Yields_c", "", 800, 600);
        c_c->cd();
        Yields.Yields_c->SetLineColor(kBlack);
        Yields.Yields_c->SetMarkerColor(kBlack);
        Yields.Yields_c->SetMarkerStyle(20);
        Yields.Yields_c->SetTitle("c Yields (Simultaneous Fit)");
        Yields.Yields_c->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        Yields.Yields_c->GetYaxis()->SetTitle("dN / dp_{T}");
        Yields.Yields_c->SetMinimum(0);
        Yields.Yields_c->Draw("E1");
        c_c->Write();
        c_c->SaveAs("plots/Yields_c.pdf");
        delete c_c; 

        // CC PLOT
        TCanvas* c_cc = new TCanvas("Yields_cc", "", 800, 600);
        c_cc->cd();
        Yields.Yields_cc->SetLineColor(kBlack);
        Yields.Yields_cc->SetMarkerColor(kBlack);
        Yields.Yields_cc->SetMarkerStyle(20);
        Yields.Yields_cc->SetTitle("cc Yields (Simultaneous Fit)");
        Yields.Yields_cc->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        Yields.Yields_cc->GetYaxis()->SetTitle("dN / dp_{T}");
        Yields.Yields_cc->SetMinimum(0);
        Yields.Yields_cc->Draw("E1");
        c_cc->Write();
        c_cc->SaveAs("plots/Yields_cc.pdf");
        delete c_cc;

        // B PLOT
        TCanvas* c_b = new TCanvas("Yields_b", "", 800, 600);
        c_b->cd();
        Yields.Yields_b->SetLineColor(kBlack);
        Yields.Yields_b->SetMarkerColor(kBlack);
        Yields.Yields_b->SetMarkerStyle(20);
        Yields.Yields_b->SetTitle("b Yields (Simultaneous Fit)");
        Yields.Yields_b->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        Yields.Yields_b->GetYaxis()->SetTitle("dN / dp_{T}");
        Yields.Yields_b->SetMinimum(0);
        Yields.Yields_b->Draw("E1");
        c_b->Write();
        c_b->SaveAs("plots/Yields_b.pdf");
        delete c_b; 

        // BB PLOT
        TCanvas* c_bb = new TCanvas("Yields_bb", "", 800, 600);
        c_bb->cd();
        Yields.Yields_bb->SetLineColor(kBlack);
        Yields.Yields_bb->SetMarkerColor(kBlack);
        Yields.Yields_bb->SetMarkerStyle(20);
        Yields.Yields_bb->SetTitle("bb Yields (Simultaneous Fit)");
        Yields.Yields_bb->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        Yields.Yields_bb->GetYaxis()->SetTitle("dN / dp_{T}");
        Yields.Yields_bb->SetMinimum(0);
        Yields.Yields_bb->Draw("E1");
        c_bb->Write();
        c_bb->SaveAs("plots/Yields_bb.pdf");
        delete c_bb;

        // FULL PLOT
        TCanvas* c_full = new TCanvas("Yields_full", "", 800, 600);
        c_full->cd();
        Yields.Yields_full->SetLineColor(kBlack);
        Yields.Yields_full->SetMarkerColor(kBlack);
        Yields.Yields_full->SetMarkerStyle(20);
        Yields.Yields_full->SetTitle("Full Yields (Simultaneous Fit)");
        Yields.Yields_full->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        Yields.Yields_full->GetYaxis()->SetTitle("dN / dp_{T}");
        Yields.Yields_full->SetMinimum(0);
        Yields.Yields_full->Draw("E1");
        c_full->Write();
        c_full->SaveAs("plots/Yields_full.pdf");
        delete c_full;  
        
        // HF PLOT
        TCanvas* c_hf = new TCanvas("Yields_hf", "", 800, 600);
        c_hf->cd();
        Yields.Yields_hf->SetLineColor(kBlack);
        Yields.Yields_hf->SetMarkerColor(kBlack);
        Yields.Yields_hf->SetMarkerStyle(20);
        Yields.Yields_hf->SetTitle("Heavy Flavor Yields (Simultaneous Fit)");
        Yields.Yields_hf->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        Yields.Yields_hf->GetYaxis()->SetTitle("dN / dp_{T}");
        Yields.Yields_hf->SetMinimum(0);
        Yields.Yields_hf->Draw("E1");
        c_hf->Write();
        c_hf->SaveAs("plots/Yields_hf.pdf");
        delete c_hf;
        
        // HF FRACTION PLOT
        TCanvas* c_hf_frac = new TCanvas("HF_Fraction", "", 800, 600);
        c_hf_frac->cd();
        Yields.HF_Fraction->SetLineColor(kBlack);
        Yields.HF_Fraction->SetMarkerColor(kBlack);
        Yields.HF_Fraction->SetMarkerStyle(20);
        Yields.HF_Fraction->SetTitle("Heavy Flavor Fraction (Simultaneous Fit)");
        Yields.HF_Fraction->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        Yields.HF_Fraction->GetYaxis()->SetTitle("HF Fraction");
        Yields.HF_Fraction->SetMinimum(0);
        Yields.HF_Fraction->SetMaximum(1);
        Yields.HF_Fraction->Draw("E1");
        c_hf_frac->Write();
        c_hf_frac->SaveAs("plots/HF_Fraction.pdf");
        delete c_hf_frac;
        
        // Chi2/ndf PLOT
        TCanvas* c_chi2 = new TCanvas("Chi2ndf", "", 800, 600);
        c_chi2->cd();
        Yields.Chi2ndf->SetLineColor(kBlack);
        Yields.Chi2ndf->SetMarkerColor(kBlack);
        Yields.Chi2ndf->SetMarkerStyle(20);
        Yields.Chi2ndf->SetTitle("Chi^{2}/ndf (Simultaneous Fit)");
        Yields.Chi2ndf->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        Yields.Chi2ndf->GetYaxis()->SetTitle("Chi^{2}/ndf");
        Yields.Chi2ndf->SetMinimum(0);
        Yields.Chi2ndf->SetMaximum(5);
        Yields.Chi2ndf->Draw("E1");
        c_chi2->Write();
        c_chi2->SaveAs("plots/Chi2ndf.pdf");
        delete c_chi2;  

    
    }

    outputFile->Close();
    input->Close();
    templatesFile->Close();

    return 0;

}