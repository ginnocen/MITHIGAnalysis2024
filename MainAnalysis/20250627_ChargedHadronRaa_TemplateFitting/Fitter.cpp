#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <THStack.h>
#include <TLegend.h>
#include <TColor.h>
#include <TTree.h>
#include "CommandLine.h"

#include <RooRealVar.h>
#include <RooPlot.h>
#include <RooHist.h> 
#include <RooHistPdf.h>
#include <RooDataHist.h>
#include <RooKeysPdf.h> 
#include <RooAddPdf.h>
#include <RooFitResult.h>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace RooFit;

TH1D* gethist(const char* name, const char* hname) {

    TFile* f = TFile::Open(name);
    if(!f || f->IsZombie()){
        cerr << "Error opening file: " << name << endl;
        return nullptr;
    }
    TH1D* hist = dynamic_cast<TH1D*>(f->Get(hname));
    if(!hist){
        cerr << "Error retrieving histogram 'hist' from file: " << name << endl;
        return nullptr;
    }
    return hist;

}

class fit{
    public:
    vector<TH1D*> templates;
    vector<string> template_names;
    TH1D* data;
    vector<double> fractions;
    string varname = "";
    double xmin = 0;
    double xmax = 0;

    // RooFit
    RooRealVar* x = nullptr;
    RooDataHist* dh_data = nullptr;
    vector<RooDataHist*> dh_templates;
    vector<RooHistPdf*> template_pdfs;
    vector<RooRealVar*> fractions_vars;
    RooAddPdf* model = nullptr;
    RooFitResult* result = nullptr;
    

fit(vector<TH1D*> templates, vector<string> template_names, TH1D* data, vector<double> fractions, string varname, double xmin = 0, double xmax = 0) {
        this->templates = templates;
        this->template_names = template_names;
        this->data = data;
        this->fractions = fractions;
        this->varname = varname;
        this->xmin = xmin;
        this->xmax = xmax;        
    }


    /////////////////////////////
    /////                  //////
    /////      Fitting     //////
    /////                  //////
    /////////////////////////////
    
    void templatefit(){
        
        // variable
        x = new RooRealVar("x", varname.c_str(), data->GetXaxis()->GetXmin(), data->GetXaxis()->GetXmax());
        if(xmin != xmax){x->setRange("fitRange", xmin, xmax);}
        
        // obtain datahist and template histogram PDFs
        dh_data = new RooDataHist("dh_data", "Data", RooArgList(*x), data);

        for(int i = 0; i < templates.size(); i++){
            string dh_name = "template_" + template_names[i]; string dh_title = "template " + template_names[i];
            RooDataHist* dh_template = new RooDataHist(dh_name.c_str(), dh_title.c_str(), RooArgList(*x), templates[i]);
            
            string pdf_name = "template_pdf_" + template_names[i]; string pdf_title = "PDF " + template_names[i];
            RooHistPdf* pdf = new RooHistPdf(pdf_name.c_str(), pdf_title.c_str(), *x, *dh_template);
            
            dh_templates.push_back(dh_template);
            template_pdfs.push_back(pdf);
        }

        // define fractions
        for(int i = 0; i < fractions.size(); i++){
            std::string frac_name = "f_" + template_names[i]; std::string frac_title = "Fraction " + template_names[i];
            RooRealVar* f = new RooRealVar(frac_name.c_str(), frac_title.c_str(), fractions[i], 0, 1.0);
            fractions_vars.push_back(f);
        }
            
        // Define model and fit  
        RooArgList pdfList;
        for (auto* pdf : template_pdfs) pdfList.add(*pdf);
        RooArgList fracList;
        for (auto* f : fractions_vars) fracList.add(*f);
        model = new RooAddPdf("model", "Model", pdfList, fracList, false);
        result = model->fitTo(*dh_data, 
            Save(), 
            Range("fitRange"), 
            Verbose(), 
            PrintLevel(1),
            SumW2Error(true),
            Minimizer("Minuit2","Migrad"),
            Strategy(2),
            MaxCalls(1000000));
        
        // obtain correct fractions
        double sum_fractions = 0;
        fractions.clear();
        for (size_t i = 0; i < fractions_vars.size(); ++i) {
        fractions.push_back(fractions_vars[i]->getVal());
        sum_fractions += fractions_vars[i]->getVal();
        }  

        for(int i = 0; i < fractions_vars.size(); i++){
            fractions[i] /= sum_fractions; // Normalize fractions
            //cout << "Unnormalized fractions " << template_names[i] << ": " << fractions_vars[i]->getVal() << endl;
            cout << "Normalized fractions " << template_names[i] << ": " << fractions[i] << endl;
        }

        cout << "done" << endl;
    }

    /////////////////////////////
    /////                  //////
    /////   Output Plots   //////
    /////                  //////
    /////////////////////////////

    // 1. Plot templates and data with fill color
    void plot_distributions(const std::string& filename = "templates_vs_data.pdf") {
        RooPlot* frame = x->frame();
        dh_data->plotOn(frame, Name("data"), MarkerStyle(20), LineColor(kBlack));

        std::vector<int> colors = {kRed+1, kBlue+1, kGreen+2, kMagenta+1, kCyan+1, kOrange+7, kViolet+1, kPink+1};
        for (size_t i = 0; i < template_pdfs.size(); ++i) {
            int color = colors[i % colors.size()];
            int fillColorWithAlpha = TColor::GetColorTransparent(color, 0.4);
            template_pdfs[i]->plotOn(
                frame,
                LineColor(color),
                FillColor(fillColorWithAlpha),
                FillStyle(1001),
                Name(("template_pdf_" + template_names[i]).c_str()),
                Normalization(1.0, RooAbsReal::Relative),
                DrawOption("F")
            );
        }

        TCanvas* c = new TCanvas("c_templates", "Templates vs Data", 2000, 2000);
        c->SetMargin(0.13,0.1,0.13,0.1);
        frame->GetYaxis()->SetTitle("Events (Normalized)");
        frame->SetMinimum(1e-6);
        frame->Draw();
        auto leg = new TLegend(0.75, 0.7, 0.88, 0.88);
        leg->AddEntry(frame->findObject("data"), "Data", "lep");
        for (size_t i = 0; i < template_pdfs.size(); ++i) {
            leg->AddEntry(frame->findObject(("template_pdf_" + template_names[i]).c_str()), template_names[i].c_str(), "f");
        }
        leg->SetLineWidth(0);
        leg->Draw();
        c->SetLogy();
        c->SaveAs(filename.c_str());
        delete c;
    }

    // 2. Plot fit result and ratio panel (fit/data)
    void plot_fit_result(const std::string& filename = "fit_result_with_ratio.pdf") {
        // Upper pad: fit result
        TCanvas* cfit = new TCanvas("c_fit", "Fit Result with Ratio", 2000, 2000);
        //cfit->SetMargin(0.13,0.15,0.13,0.15);
        cfit->Divide(1,2,0,0);

        // --- Fit result (stacked) ---
        cfit->cd(1);
        gPad->SetPad(0,0.305,1,1);
        gPad->SetLeftMargin(0.15);
        gPad->SetRightMargin(0.05);
        gPad->SetBottomMargin(0);
        gPad->SetTopMargin(0.1);

        std::vector<int> colors = {kRed+1, kBlue+1, kGreen+2, kMagenta+1, kCyan+1, kOrange+7, kViolet+1, kPink+1};

        // Clone and scale templates by fitted fractions, add to THStack
        std::vector<std::pair<TH1D*, std::string>> stacked_hists;
        double total = data->Integral();
        TH1D* hsum = nullptr;
        THStack* hstack = new THStack("hstack", "Stacked Fit Templates");
        for (int i = templates.size() - 1; i >= 0; --i) {
            TH1D* h = (TH1D*)templates[i]->Clone(("stacked_"+template_names[i]).c_str());
            double frac = (i < fractions.size()) ? fractions[i] : 0.0;
            h->Scale(frac * total / h->Integral());
            h->SetFillColor(colors[i % colors.size()]);
            h->SetLineColor(colors[i % colors.size()]);
            hstack->Add(h);
            stacked_hists.push_back({h, template_names[i]});
            if (i == templates.size() - 1) {
                hsum = (TH1D*)h->Clone("hsum");
            } else {
                hsum->Add(h);
            }
        }

        // Draw the stack
        hstack->Draw("HIST");
        hstack->GetYaxis()->SetTitle("Events(Normalized)");
        hstack->GetXaxis()->SetTitle(varname.c_str());
        hstack->SetMinimum(1e-6);
        hstack->SetMaximum(data->GetMaximum()*50);

        // Draw data points on top
        data->SetMarkerStyle(20);
        data->SetMarkerColor(kBlack);
        data->SetLineColor(kBlack);
        data->Draw("E1 SAME");

        // Legend
        auto legfit = new TLegend(0.6, 0.6, 0.88, 0.88);
        legfit->AddEntry(data, "Data", "lep");
        for (int i = stacked_hists.size() - 1; i >= 0; --i) {
            legfit->AddEntry(stacked_hists[i].first, stacked_hists[i].second.c_str(), "f");
        }
        legfit->Draw();
        legfit->SetLineWidth(0);

        gPad->SetLogy();
        //gPad->SetLogx();

        // --- Ratio panel ---
        cfit->cd(2);
        gPad->SetPad(0,0,1,0.295);
        //gPad->SetLogx();
        gPad->SetLeftMargin(0.15);
        gPad->SetRightMargin(0.05);
        gPad->SetBottomMargin(0.28);
        gPad->SetTopMargin(0.05);

        int nbins = data->GetNbinsX();
        std::vector<double> xvals, ratios;
        for (int i = 1; i <= nbins; ++i) {
            double xval = data->GetBinCenter(i);
            double dataval = data->GetBinContent(i);
            double fitval = hsum->GetBinContent(i);
            xvals.push_back(xval);
            if (dataval != 0)
                ratios.push_back(fitval / dataval);
            else
                ratios.push_back(0);
        }
        TGraph* gratio = new TGraph(nbins);
        for (int i = 0; i < nbins; ++i)
            gratio->SetPoint(i, xvals[i], ratios[i]);
        gratio->GetYaxis()->SetTitle("Fit / Data");
        gratio->GetXaxis()->SetTitle(varname.c_str());
        gratio->SetTitle("");
        gratio->SetMarkerStyle(20);
        gratio->SetMarkerColor(kRed);
        gratio->SetMarkerSize(1);
        gratio->SetMinimum(0.9);
        gratio->SetMaximum(1.1);
        gratio->Draw("AP");

        gratio->GetXaxis()->SetLabelSize(0.08);
        gratio->GetXaxis()->SetTitleSize(0.10);
        gratio->GetYaxis()->SetLabelSize(0.08);
        gratio->GetYaxis()->SetTitleSize(0.10);
        gratio->GetYaxis()->SetTitleOffset(0.45);


        cfit->SaveAs(filename.c_str());

        // Clean up
        for (auto& h : stacked_hists) delete h.first;
        delete hsum;
        delete cfit;
    }

    // 3. Plot pull distribution
    void plot_pulls(const std::string& filename = "fit_result_pulls.pdf") {
        TCanvas* cpull = new TCanvas("cpull", "Pulls", 1000, 1000);
        RooPlot* fitframe = x->frame();
        dh_data->plotOn(fitframe, Name("data"), MarkerStyle(20), LineColor(kBlack));
        model->plotOn(fitframe, Name("fit"), LineColor(kBlue+2), LineWidth(2));
        RooHist* hpull = fitframe->pullHist("data", "fit");
        RooPlot* pullframe = x->frame();
        pullframe->addPlotable(hpull, "P");
        pullframe->SetTitle("Pulls (Data - Fit) / Error");
        pullframe->GetYaxis()->SetTitle("Pull");
        pullframe->GetYaxis()->SetTitleOffset(0.8);
        pullframe->Draw();
        cpull->SaveAs(filename.c_str());
        delete cpull;
    }

    // 4. Plot normalized residuals
    void plot_residuals(const std::string& filename = "fit_result_residuals.pdf") {
    int nbins = data->GetNbinsX();
    // Build the fitted sum histogram as in plot_fit_result
    std::vector<TH1D*> stacked_hists;
    double total = data->Integral();
    std::vector<int> colors = {kRed+1, kBlue+1, kGreen+2, kMagenta+1, kCyan+1, kOrange+7, kViolet+1, kPink+1};
    for (size_t i = 0; i < templates.size(); ++i) {
        TH1D* h = (TH1D*)templates[i]->Clone(("stacked_"+template_names[i]).c_str());
        double frac = (i < fractions.size()) ? fractions[i] : 0.0;
        h->Scale(frac * total / h->Integral());
        stacked_hists.push_back(h);
    }
    TH1D* hsum = (TH1D*)stacked_hists[0]->Clone("hsum");
    hsum->Reset();
    for (size_t i = 0; i < stacked_hists.size(); ++i) {
        hsum->Add(stacked_hists[i]);
    }

    std::vector<double> xvals, normresiduals;
    for (int i = 1; i <= nbins; ++i) {
        double xval = data->GetBinCenter(i);
        double dataval = data->GetBinContent(i);
        double fitval = hsum->GetBinContent(i);
        xvals.push_back(xval);
        if (dataval != 0)
            normresiduals.push_back((dataval - fitval) / dataval);
        else
            normresiduals.push_back(0.0);
    }
    TGraph* gnormres = new TGraph(nbins);
    for (int i = 0; i < nbins; ++i)
        gnormres->SetPoint(i, xvals[i], normresiduals[i]);
    //gnormres->SetTitle("Normalized Residuals;;(Data - Fit)/Data");
    gnormres->GetYaxis()->SetTitle("Normalized Residuals (Data - Fit) / Data");
    gnormres->GetXaxis()->SetTitle(varname.c_str());
    gnormres->SetMarkerStyle(20);

    TCanvas* cres = new TCanvas("cres", "Fit with Residuals", 2000, 1000);
    cres->SetMargin(0.15, 0.05, 0.15, 0.15);
    gnormres->Draw("AP");
    cres->SaveAs(filename.c_str());
    // Clean up
    for (auto h : stacked_hists) delete h;
    delete hsum;
    delete cres;
}

    // 5. Fit diagnostics
    void fit_diagnostics(const std::string& filename = "fit_result.log") {
        std::ofstream log(filename);
        if (!log.is_open()) {
            std::cerr << "Could not open log file: " << filename << std::endl;
            return;
        }

        log << "Fit variable: " << varname << std::endl;
        log << "Fit range: [" << xmin << ", " << xmax << "]" << std::endl;
        log << "Number of bins: " << (data ? data->GetNbinsX() : -1) << std::endl;

        if (result) {
            log << "Fit status: " << result->status() << std::endl;           // 1
            log << "Covariance quality: " << result->covQual() << std::endl;  // 2
            log << "Min NLL: " << result->minNll() << std::endl;              // 3
            log << "EDM: " << result->edm() << std::endl;                     // 4
            log << "Number of invalid NLL: " << result->numInvalidNLL() << std::endl; // 6

            log << std::fixed << std::setprecision(5);
            log << "\nFractions (fit values):\n";
            for (size_t i = 0; i < fractions_vars.size(); ++i) {
                double val = fractions_vars[i]->getVal();
                double err = fractions_vars[i]->getError();
                log << "  " << template_names[i]
                    << ": " << val
                    << " +/- " << err
                    << " (normalized: " << fractions[i] << ")\n";
            }

            log << "\nParameter correlations:\n";
            for (size_t i = 0; i < fractions_vars.size(); ++i) {
                for (size_t j = i + 1; j < fractions_vars.size(); ++j) {
                    log << "  Corr(" << fractions_vars[i]->GetName() << ", " << fractions_vars[j]->GetName() << "): "
                        << result->correlation(fractions_vars[i]->GetName(), fractions_vars[j]->GetName()) << "\n";
                }
            }
        } else {
            log << "No fit result available.\n";
        }

        log.close();
        }


    // 6. Save relevant hists to root file
    void save_all_histograms(const std::string& filename = "fit_outputs.root") {
        TFile* fout = new TFile(filename.c_str(), "RECREATE");

        // --- Save stacked fit result histograms ---
        double total = data->Integral();
        std::vector<int> colors = {kRed+1, kBlue+1, kGreen+2, kMagenta+1, kCyan+1, kOrange+7, kViolet+1, kPink+1};
        std::vector<TH1D*> stacked_hists;
        for (size_t i = 0; i < templates.size(); ++i) {
            TH1D* h = (TH1D*)templates[i]->Clone(("stacked_"+template_names[i]).c_str());
            double frac = (i < fractions.size()) ? fractions[i] : 0.0;
            h->Scale(frac * total / h->Integral());
            h->SetFillColor(colors[i % colors.size()]);
            h->SetLineColor(colors[i % colors.size()]);
            h->Write();
            stacked_hists.push_back(h);
        }
        // Save the sum histogram
        TH1D* hsum = (TH1D*)stacked_hists[0]->Clone("hsum");
        hsum->Reset();
        for (auto h : stacked_hists) hsum->Add(h);
        hsum->Write();

        // --- Save data histogram ---
        if (data) data->Write("data");

        // --- Save pulls as TH1D ---
        if (model && dh_data) {
            RooPlot* fitframe = x->frame();
            dh_data->plotOn(fitframe, Name("data"), MarkerStyle(20), LineColor(kBlack));
            model->plotOn(fitframe, Name("fit"), LineColor(kBlue+2), LineWidth(2));
            RooHist* hpull = fitframe->pullHist("data", "fit");
            int nbins = hpull->GetN();
            TH1D* hpull_hist = new TH1D("pulls", "Pulls (Data - Fit) / Error", nbins, data->GetXaxis()->GetXmin(), data->GetXaxis()->GetXmax());
            for (int i = 0; i < nbins; ++i) {
                double x, y;
                hpull->GetPoint(i, x, y);
                hpull_hist->SetBinContent(i+1, y);
            }
            hpull_hist->Write();
            delete fitframe;
            delete hpull_hist;
        }

        // --- Save residuals as TGraph ---
        int nbins = data->GetNbinsX();
        std::vector<double> xvals, normresiduals;
        for (int i = 1; i <= nbins; ++i) {
            double xval = data->GetBinCenter(i);
            double dataval = data->GetBinContent(i);
            double fitval = hsum->GetBinContent(i);
            xvals.push_back(xval);
            if (dataval != 0)
                normresiduals.push_back((dataval - fitval) / dataval);
            else
                normresiduals.push_back(0.0);
        }
        TGraph* gnormres = new TGraph(nbins);
        for (int i = 0; i < nbins; ++i)
            gnormres->SetPoint(i, xvals[i], normresiduals[i]);
        gnormres->SetName("normalized_residuals");
        gnormres->Write();
        delete gnormres;

        // --- Save ratio as TGraph ---
        std::vector<double> ratios;
        for (int i = 1; i <= nbins; ++i) {
            double dataval = data->GetBinContent(i);
            double fitval = hsum->GetBinContent(i);
            if (dataval != 0)
                ratios.push_back(fitval / dataval);
            else
                ratios.push_back(0);
        }
        TGraph* gratio = new TGraph(nbins);
        for (int i = 0; i < nbins; ++i)
            gratio->SetPoint(i, xvals[i], ratios[i]);
        gratio->SetName("fit_over_data_ratio");
        gratio->Write();
        delete gratio;

        // Clean up
        for (auto h : stacked_hists) delete h;
        delete hsum;
        fout->Close();
        delete fout;
    }

    void plot_fit(const std::string& basename = "fit_result") {
        
        plot_distributions(basename + "_distributions.pdf");
        plot_fit_result(basename + "_results.pdf");
        plot_pulls(basename + "_pulls.pdf");
        plot_residuals(basename + "_residuals.pdf");
        fit_diagnostics(basename + "_diagnostics.log");
        save_all_histograms(basename + "_histograms.root");
    }


};


int main(int argc, char *argv[]) {
    CommandLine CL(argc, argv);

    string dataFile = CL.Get("Data");
    vector<string> templateFile = CL.GetStringVector("Templates");
    vector<string> templateNames = CL.GetStringVector("TemplateNames");
    vector<double> fractions = CL.GetDoubleVector("Fractions");
    string branch = CL.Get("Branch");
    string varname = CL.Get("VarName");
    double xmin = CL.GetDouble("Xmin", 0);
    double xmax = CL.GetDouble("Xmax", 0);

    vector<TH1D*> templates;
    for(int i = 0; i < templateFile.size(); i++){
        TH1D* hist = gethist(templateFile[i].c_str(), branch.c_str());
        templates.push_back(hist);
    }
    TH1D* data = gethist(dataFile.c_str(), branch.c_str());

    const string of = "fit_result_" + branch;

    fit a(templates, templateNames, data, fractions, varname, xmin, xmax);
    a.templatefit();
    a.plot_fit(of);

    return 1;
}

