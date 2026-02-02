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
#include <RooFormulaVar.h>

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
    double nHf;
    double nHf_err;
    double chi2_ndf;
};


struct FittingHists {

    const char* variable;
    TH1D* Yields_uds;
    TH1D* Yields_full;
    TH1D* Yields_hf;
    TH1D* HF_Fraction;
    TH1D* Chi2ndf;

};

FitResult Yields(TFile* datafile, TFile* templatefile, vector<const char*> variables, vector<double> fitrange_min, vector<double> fitrange_max, vector<double> kdes, float Jetptmin, float Jetptmax, int chargesel, TDirectory* plotDir = nullptr){
    
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
        template_uds_sets.push_back(new RooDataSet(Form("tmp_uds_%d", i), "", nt_uds, *varsets[i], Form("JetPT < %f && JetPT >= %f && (Charge == %d || %d == 0)", Jetptmax, Jetptmin, chargesel, chargesel), "weight"));
        template_other_sets.push_back(new RooDataSet(Form("tmp_other_%d", i), "", nt_other, *varsets[i], Form("JetPT < %f && JetPT >= %f && (Charge == %d || %d == 0)", Jetptmax, Jetptmin, chargesel, chargesel), "weight"));
        template_c_sets.push_back(new RooDataSet(Form("tmp_c_%d", i), "", nt_c, *varsets[i], Form("JetPT < %f && JetPT >= %f && (Charge == %d || %d == 0)", Jetptmax, Jetptmin, chargesel, chargesel), "weight"));
        template_cc_sets.push_back(new RooDataSet(Form("tmp_cc_%d", i), "", nt_cc, *varsets[i], Form("JetPT < %f && JetPT >= %f && (Charge == %d || %d == 0)", Jetptmax, Jetptmin, chargesel, chargesel), "weight"));
        template_b_sets.push_back(new RooDataSet(Form("tmp_b_%d", i), "", nt_b, *varsets[i], Form("JetPT < %f && JetPT >= %f && (Charge == %d || %d == 0)", Jetptmax, Jetptmin, chargesel, chargesel), "weight"));
        template_bb_sets.push_back(new RooDataSet(Form("tmp_bb_%d", i), "", nt_bb, *varsets[i], Form("JetPT < %f && JetPT >= %f && (Charge == %d || %d == 0)", Jetptmax, Jetptmin, chargesel, chargesel), "weight"));
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
    int nEvents = data_sets[0]->numEntries();
    cout << "Total data entries (weighted): " << nTotal << endl;
    cout << "Total data entries (unweighted): " << nEvents << endl;
    
    RooRealVar n_LF("n_LF", "N light flavor", nTotal/2, 0, nTotal*2);
    RooRealVar n_HF("n_HF", "N heavy flavor", nTotal/2, 0, nTotal*2);
    
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
            
            RooPlot* frame = fittingvars[i]->frame(RooFit::Title(Form("%s Fit (%.0f < p_{T} < %.0f GeV)", varname, Jetptmin, Jetptmax)), RooFit::Bins(20));
            data_sets[i]->plotOn(frame, RooFit::Name("data"), RooFit::DataError(RooAbsData::SumW2));
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
            
            // Bottom panel: ratio
            c->cd(2);
            gPad->SetPad(0.0, 0.0, 1.0, 0.3);
            gPad->SetTopMargin(0.02);
            gPad->SetBottomMargin(0.3);
            
            // Create histograms for data and model
            int nBins = 20; // Same as frame
            double xmin = fittingvars[i]->getMin();
            double xmax = fittingvars[i]->getMax();
            
            TH1D* h_data = (TH1D*)data_sets[i]->createHistogram(Form("h_data_%s_temp", varname), *fittingvars[i], RooFit::Binning(nBins, xmin, xmax));
            TH1D* h_model = (TH1D*)models[i]->createHistogram(Form("h_model_%s_temp", varname), *fittingvars[i], RooFit::Binning(nBins, xmin, xmax));
            
            // Normalize model histogram to match total data entries
            double dataIntegral = h_data->Integral();
            double modelIntegral = h_model->Integral();
            if(modelIntegral > 0) {
                h_model->Scale(dataIntegral / modelIntegral);
            }
            
            // Create ratio histogram
            TH1D* h_ratio = (TH1D*)h_model->Clone(Form("h_ratio_%s", varname));
            h_ratio->SetTitle(Form(";%s;Model/Data", varname));
            h_ratio->Divide(h_data);
            h_ratio->SetMarkerStyle(20);
            h_ratio->SetMarkerSize(0.8);
            h_ratio->SetLineColor(kBlack);
            h_ratio->SetMinimum(0.5);
            h_ratio->SetMaximum(1.5);
            h_ratio->GetYaxis()->SetTitle("Model / Data");
            h_ratio->GetYaxis()->SetTitleSize(0.12);
            h_ratio->GetYaxis()->SetLabelSize(0.10);
            h_ratio->GetYaxis()->SetTitleOffset(0.35);
            h_ratio->GetYaxis()->SetNdivisions(505);
            h_ratio->GetXaxis()->SetTitleSize(0.12);
            h_ratio->GetXaxis()->SetLabelSize(0.10);
            h_ratio->GetXaxis()->SetTitleOffset(1.0);
            h_ratio->Draw("P");
            
            delete h_data;
            delete h_model;
            
            TLine* line0 = new TLine(xmin, 1, xmax, 1);
            line0->SetLineStyle(2);
            line0->Draw();
            
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
            n_HF.getVal(), n_HF.getError(),
            avg_chi2};
}


void FillHists(FitResult fitResult, FittingHists& hists, int ptBinIndex, float binwidth) {
    
    hists.Yields_uds->SetBinContent(ptBinIndex, fitResult.nLF / binwidth);
    hists.Yields_uds->SetBinError(ptBinIndex, fitResult.nLF_err / binwidth);
    hists.Yields_full->SetBinContent(ptBinIndex, (fitResult.nLF + fitResult.nHf) / binwidth);
    hists.Yields_full->SetBinError(ptBinIndex, sqrt(pow(fitResult.nLF_err,2) + pow(fitResult.nHf_err,2)) / binwidth);
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

    // TEMPLATE FITTING PARAMETERS
    vector<string> variables_str = CL.GetStringVector("variables");
    vector<double> kdes = CL.GetDoubleVector("kde");
    vector<double> fitrange_min = CL.GetDoubleVector("fitRangeMin");;
    vector<double> fitrange_max = CL.GetDoubleVector("fitRangeMax");
    int chargesel = CL.GetInt("chargeSelection", 0);

    // IMPORT FILES 
    TFile* input = TFile::Open(file.c_str());
    TFile* templatesFile = TFile::Open(templates.c_str());

    // DECLARE HISTOGRAMS
    TFile* outputFile = new TFile(output.c_str(), "RECREATE");
    outputFile->cd();
    
    FittingHists yieldsHists;
    
    // Initialize histograms
    int nBins = ptBins.size() - 1;
    yieldsHists.Yields_uds = new TH1D("Yields_uds", "uds Yields", nBins, ptBins.data());
    yieldsHists.Yields_full = new TH1D("Yields_full", "Full Yields", nBins, ptBins.data());
    yieldsHists.Yields_hf = new TH1D("Yields_hf", "Heavy Flavor Yields", nBins, ptBins.data());
    yieldsHists.HF_Fraction = new TH1D("HF_Fraction", "HF Fraction", nBins, ptBins.data());
    yieldsHists.Chi2ndf = new TH1D("Chi2ndf", "Chi^{2}/ndf", nBins, ptBins.data());

    // CREATE PLOTS DIRECTORY
    TDirectory* plotDir = nullptr;
    if(makeplots) {
        plotDir = outputFile->mkdir("plots");
    }

    for(int i = 0; i < ptBins.size() - 1; i++) {
        float ptMin = ptBins[i];
        float ptMax = ptBins[i+1];

        vector<const char*> variables_loop = {variables_str[0].c_str(), variables_str[1].c_str()};
        vector<double> fitrange_min_loop = {fitrange_min[0], fitrange_min[1]};
        vector<double> fitrange_max_loop = {fitrange_max[0], fitrange_max[1]};
        vector<double> kdes_loop = {kdes[0], kdes[1]};

        FitResult result = Yields(input, templatesFile, variables_loop, fitrange_min_loop, fitrange_max_loop, kdes_loop, ptMin, ptMax, chargesel, plotDir);
        float binwidth = ptMax - ptMin;

        // Fill histograms
        FillHists(result, yieldsHists, i + 1, binwidth);

    }

    // WRITE TO FILE
    outputFile->cd();
    yieldsHists.Yields_uds->Write("Yields_uds");
    yieldsHists.Yields_full->Write("Yields_full");
    yieldsHists.Yields_hf->Write("Yields_hf");
    yieldsHists.HF_Fraction->Write("HF_Fraction");
    yieldsHists.Chi2ndf->Write("Chi2ndf");

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
        gPad->SetLogy();
        
        yieldsHists.Yields_uds->SetLineColor(kBlack);
        yieldsHists.Yields_uds->SetMarkerColor(kBlack);
        yieldsHists.Yields_uds->SetMarkerStyle(20);
        yieldsHists.Yields_uds->SetTitle("Light Flavor Yields (Simultaneous Fit)");
        yieldsHists.Yields_uds->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        yieldsHists.Yields_uds->GetYaxis()->SetTitle("dN / dp_{T}");
        yieldsHists.Yields_uds->Draw("E1");
        c_uds->Write();
        c_uds->SaveAs("plots/Yields_uds.pdf");
        delete c_uds;

        // FULL PLOT
        TCanvas* c_full = new TCanvas("Yields_full", "", 800, 600);
        c_full->cd();
        gPad->SetLogy();
        yieldsHists.Yields_full->SetLineColor(kBlack);
        yieldsHists.Yields_full->SetMarkerColor(kBlack);
        yieldsHists.Yields_full->SetMarkerStyle(20);
        yieldsHists.Yields_full->SetTitle("Full Yields (Simultaneous Fit)");
        yieldsHists.Yields_full->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        yieldsHists.Yields_full->GetYaxis()->SetTitle("dN / dp_{T}");
        yieldsHists.Yields_full->Draw("E1");
        c_full->Write();
        c_full->SaveAs("plots/Yields_full.pdf");
        delete c_full;  
        
        // HF PLOT
        TCanvas* c_hf = new TCanvas("Yields_hf", "", 800, 600);
        c_hf->cd();
        gPad->SetLogy();
        yieldsHists.Yields_hf->SetLineColor(kBlack);
        yieldsHists.Yields_hf->SetMarkerColor(kBlack);
        yieldsHists.Yields_hf->SetMarkerStyle(20);
        yieldsHists.Yields_hf->SetTitle("Heavy Flavor Yields (Simultaneous Fit)");
        yieldsHists.Yields_hf->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        yieldsHists.Yields_hf->GetYaxis()->SetTitle("dN / dp_{T}");
        yieldsHists.Yields_hf->Draw("E1");
        c_hf->Write();
        c_hf->SaveAs("plots/Yields_hf.pdf");
        delete c_hf;
        
        // HF FRACTION PLOT
        TCanvas* c_hf_frac = new TCanvas("HF_Fraction", "", 800, 600);
        c_hf_frac->cd();
        yieldsHists.HF_Fraction->SetLineColor(kBlack);
        yieldsHists.HF_Fraction->SetMarkerColor(kBlack);
        yieldsHists.HF_Fraction->SetMarkerStyle(20);
        yieldsHists.HF_Fraction->SetTitle("Heavy Flavor Fraction (Simultaneous Fit)");
        yieldsHists.HF_Fraction->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        yieldsHists.HF_Fraction->GetYaxis()->SetTitle("HF Fraction");
        yieldsHists.HF_Fraction->SetMinimum(0);
        yieldsHists.HF_Fraction->SetMaximum(1);
        yieldsHists.HF_Fraction->Draw("E1");
        c_hf_frac->Write();
        c_hf_frac->SaveAs("plots/HF_Fraction.pdf");
        delete c_hf_frac;
        
        // Chi2/ndf PLOT
        TCanvas* c_chi2 = new TCanvas("Chi2ndf", "", 800, 600);
        c_chi2->cd();
        yieldsHists.Chi2ndf->SetLineColor(kBlack);
        yieldsHists.Chi2ndf->SetMarkerColor(kBlack);
        yieldsHists.Chi2ndf->SetMarkerStyle(20);
        yieldsHists.Chi2ndf->SetTitle("Chi^{2}/ndf (Simultaneous Fit)");
        yieldsHists.Chi2ndf->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
        yieldsHists.Chi2ndf->GetYaxis()->SetTitle("Chi^{2}/ndf");
        yieldsHists.Chi2ndf->SetMinimum(0);
        //yieldsHists.Chi2ndf->SetMaximum(5);
        yieldsHists.Chi2ndf->Draw("E1");
        c_chi2->Write();
        c_chi2->SaveAs("plots/Chi2ndf.pdf");
        delete c_chi2;  

    
    }

    outputFile->Close();
    input->Close();
    templatesFile->Close();

    return 0;

}