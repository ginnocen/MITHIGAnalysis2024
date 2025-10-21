#include <TStyle.h>
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

#include <RooFormulaVar.h>
#include <RooRealVar.h>
#include <RooPlot.h>
#include <RooHist.h> 
#include <RooHistPdf.h>
#include <RooCategory.h>
#include <RooSimultaneous.h>
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
    vector<vector<TH1D*>> templates;
    vector<string> template_names;
    vector<TH1D*> data;
    vector<double> fractions;
    vector<string> varname;
    vector<double> xmin;
    vector<double> xmax;

    vector<RooRealVar*> xs;
    vector<RooDataHist*> dh_data;
    vector<vector<RooDataHist*>> dh_templates;
    vector<vector<RooHistPdf*>> template_pdfs;
    vector<RooRealVar*> fractions_vars;
    vector<RooAddPdf*> model;
    vector<RooFitResult*> result;
    

    fit(vector<vector<TH1D*>> templates, vector<string> template_names, vector<TH1D*> data, vector<double> fractions, vector<string> varname, vector<double> xmin, vector<double> xmax) {
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

    void simultaneousFit() {
        // 1. Observables and category
        for (size_t i = 0; i < varname.size(); ++i)
            xs.push_back(new RooRealVar(varname[i].c_str(), varname[i].c_str(), xmin[i], xmax[i]));
        RooCategory cat("cat", "cat");
        for (const auto& n : varname) cat.defineType(n.c_str());

        // 2. Template PDFs for each channel
        vector<vector<RooHistPdf*>> pdfs(varname.size());
        for (size_t c = 0; c < varname.size(); ++c) {
            for (size_t t = 0; t < templates.size(); ++t) {
                if (!templates[t][c]) { cerr << "Missing template " << t << "," << c << endl; exit(1); }
                auto* dh = new RooDataHist(Form("dh_temp_%zu_%zu", t, c), "template", RooArgList(*xs[c]), templates[t][c]);
                pdfs[c].push_back(new RooHistPdf(Form("pdf_%zu_%zu", t, c), "pdf", *xs[c], *dh));
            }
        }

        // 3. Fractions (N-1 free, last constrained)
        RooArgList fracList;
        vector<RooRealVar*> fracVars;
        for (size_t i = 0; i < template_names.size() - 1; ++i) {
            auto* f = new RooRealVar(Form("f_%s", template_names[i].c_str()), "Fraction", fractions[i], 0, 1);
            fracVars.push_back(f);
            fracList.add(*f);
        }
        string formula = "1";
        for (size_t i = 0; i < template_names.size() - 1; ++i)
            formula += "-@" + to_string(i);
        auto* lastFrac = new RooFormulaVar(Form("f_%s", template_names.back().c_str()), formula.c_str(), fracList);
        fracList.add(*lastFrac);

        // 4. RooAddPdf for each channel
        vector<RooAddPdf*> models;
        for (size_t c = 0; c < varname.size(); ++c) {
            RooArgList pdfList;
            for (auto* p : pdfs[c]) pdfList.add(*p);
            auto* m = new RooAddPdf(Form("model_%zu", c), "model", pdfList, fracList, false);
            m->fixCoefNormalization(RooArgSet(*xs[c]));
            models.push_back(m);
        }

        // 5. RooSimultaneous
        RooSimultaneous simPdf("simPdf", "simPdf", cat);
        for (size_t c = 0; c < varname.size(); ++c)
            simPdf.addPdf(*models[c], varname[c].c_str());

        // 6. Build a combined RooDataHist for RooSimultaneous
        //    (category + correct observable for each channel)
        RooArgSet obsAndCat;
        obsAndCat.add(cat);
        for (auto* x : xs) obsAndCat.add(*x);
        RooDataHist simData("simData", "simData", obsAndCat);

        for (size_t c = 0; c < varname.size(); ++c) {
            for (int bin = 1; bin <= data[c]->GetNbinsX(); ++bin) {
                xs[c]->setVal(data[c]->GetXaxis()->GetBinCenter(bin));
                cat.setLabel(varname[c].c_str());
                double weight = data[c]->GetBinContent(bin);
                if (weight == 0) continue;
                simData.add(obsAndCat, weight);
            }
        }

        // 7. Fit
        RooFitResult* fitResult = simPdf.fitTo(simData, Save(true), PrintLevel(1));
        //RooFitResult* fitResult = simPdf.fitTo(simData,
        //    Save(), 
        //    Verbose(), 
        //    PrintLevel(1),
        //    SumW2Error(true),
        //    Minimizer("Minuit2","Migrad"),
        //    Strategy(2),
         //   MaxCalls(1000000));
        cout << "Fit status: " << fitResult->status() << endl;


        fractions.clear();
        vector<double> fraction_errors;
        for (int i = 0; i < fracList.getSize(); ++i) {
            RooAbsArg* arg = fracList.at(i);
            double val = 0, err = 0;
            if (auto* v = dynamic_cast<RooRealVar*>(arg)) {
                val = v->getVal();
                err = v->getError();
            } else if (auto* f = dynamic_cast<RooFormulaVar*>(arg)) {
                val = f->getVal();
                err = f->getPropagatedError(*fitResult);
            }
            fractions.push_back(val);
            fraction_errors.push_back(err);
        }
        cout << "Fitted fractions:" << endl;
        for (size_t i = 0; i < template_names.size(); ++i) {
            cout << "  " << template_names[i]
                    << ": " << fractions[i]
                    << " +/- " << fraction_errors[i] << endl;
        }

        this->xs = xs;
        this->model = models;                // RooAddPdf* per channel
        this->dh_data.clear();               // RooDataHist* per channel (data)
        for (size_t c = 0; c < varname.size(); ++c) {
            // Build RooDataHist for each channel's data
            auto* dh = new RooDataHist(Form("dh_data_%zu", c), "data", RooArgList(*xs[c]), data[c]);
            this->dh_data.push_back(dh);
        }
        this->dh_templates = dh_templates;   // RooDataHist* per template per channel
        this->template_pdfs = pdfs;          // RooHistPdf* per template per channel
        this->fractions_vars = fracVars;     // RooRealVar* for fractions
        this->result.push_back(fitResult);   // RooFitResult* 

    }

    /////////////////////////////
    /////                  //////
    /////   Output Plots   //////
    /////                  //////
    /////////////////////////////

   
    void plot_distributions(const string& basename = "templates_vs_data") {
        vector<int> colors = {kRed+1, kBlue+1, kGreen+2, kMagenta+1, kCyan+1, kOrange+7, kViolet+1, kPink+1};

        for (size_t c = 0; c < varname.size(); ++c) {
            TCanvas* c1 = new TCanvas(("c_templates_" + varname[c]).c_str(), ("Templates vs Data " + varname[c]).c_str(), 2000, 2000);

            // Prepare normalized templates and find the one with the highest maximum
            vector<TH1D*> htemplates;
            vector<double> maxvals;
            for (size_t t = 0; t < templates.size(); ++t) {
                TH1D* h = (TH1D*)templates[t][c]->Clone(("template_norm_" + template_names[t] + "_" + varname[c]).c_str());
                double tempInt = h->Integral();
                if (tempInt > 0) h->Scale(1.0 / tempInt);
                int color = colors[t % colors.size()];
                h->SetLineColor(color);
                h->SetFillColorAlpha(color, 0.2);
                h->SetFillStyle(1001);
                htemplates.push_back(h);
                maxvals.push_back(h->GetMaximum());
            }

            // Find index of template with highest maximum
            size_t maxidx = 0;
            for (size_t t = 1; t < maxvals.size(); ++t) {
                if (maxvals[t] > maxvals[maxidx]) maxidx = t;
            }

            // Prepare and scale data
            double dataInt = data[c]->Integral();
            TH1D* hdata = (TH1D*)data[c]->Clone(("data_norm_" + varname[c]).c_str());
            if (dataInt > 0) hdata->Scale(1 / dataInt); // scale by 10x
            hdata->SetMarkerStyle(20);
            hdata->SetMarkerColor(kBlack);
            hdata->SetLineColor(kBlack);

            // Set y-axis range so all peaks are visible
            double ymax = hdata->GetMaximum();
            for (auto h : htemplates) {
                if (h->GetMaximum() > ymax) ymax = h->GetMaximum();
            }
            hdata->SetMaximum(ymax * 1.5);

            // Draw the template with the highest maximum first
            htemplates[maxidx]->Draw("HIST");
            // Draw the rest of the templates
            for (size_t t = 0; t < htemplates.size(); ++t) {
                if (t == maxidx) continue;
                htemplates[t]->Draw("HIST SAME");
            }
            // Draw data last
            hdata->Draw("E1 SAME");

                // Draw legend
            auto leg = new TLegend(0.75, 0.7, 0.88, 0.88);
            leg->AddEntry(hdata, "Data", "lep");
            for (size_t t = 0; t < htemplates.size(); ++t)
                leg->AddEntry(htemplates[t], template_names[t].c_str(), "f");
            leg->SetLineWidth(0);
            leg->Draw();


            c1->SetLogy();
            c1->SaveAs((basename + "_" + varname[c] + "_distributions.pdf").c_str());

            delete hdata;
            for (auto h : htemplates) delete h;
            delete c1;
        }
    }

    void plot_fit_results(const string& basename = "fit_result_with_ratio") {
        vector<int> colors = {kRed+1, kBlue+1, kGreen+2, kMagenta+1, kCyan+1, kOrange+7, kViolet+1, kPink+1};

        for (size_t c = 0; c < varname.size(); ++c) {
            TCanvas* cfit = new TCanvas(("c_fit_" + varname[c]).c_str(), ("Fit Result with Ratio " + varname[c]).c_str(), 2000, 2000);
            cfit->Divide(1,2,0,0);

            // --- Fit result (stacked) ---
            cfit->cd(1);
            gPad->SetPad(0,0.305,1,1);
            gPad->SetLeftMargin(0.15);
            gPad->SetRightMargin(0.05);
            gPad->SetBottomMargin(0);
            gPad->SetTopMargin(0.1);

            // Stack templates by fitted fractions
            double total = data[c]->Integral();
            THStack* hstack = new THStack(("hstack_" + varname[c]).c_str(), "Stacked Fit Templates");
            vector<TH1D*> stacked_hists;
            for (int t = templates.size() - 1; t >= 0; --t) {
                TH1D* h = (TH1D*)templates[t][c]->Clone(("stacked_" + template_names[t] + "_" + varname[c]).c_str());
                double frac = (t < fractions.size()) ? fractions[t] : 0.0;
                if (h->Integral() > 0) h->Scale(frac * total / h->Integral());
                h->SetFillColor(colors[t % colors.size()]);
                h->SetLineColor(colors[t % colors.size()]);
                hstack->Add(h);
                stacked_hists.push_back(h);
            }

            hstack->Draw("HIST");
            hstack->GetYaxis()->SetTitle("Events");
            hstack->GetXaxis()->SetTitle(varname[c].c_str());
            hstack->SetMinimum(1e-6);
            hstack->SetMaximum(data[c]->GetMaximum() * 2);

            // Draw data points on top
            data[c]->SetMarkerStyle(20);
            data[c]->SetMarkerColor(kBlack);
            data[c]->SetLineColor(kBlack);
            data[c]->Draw("E1 SAME");

            // Legend
            auto legfit = new TLegend(0.7, 0.7, 0.93, 0.85);
            legfit->AddEntry(data[c], "Data", "lep");
            for (int t = stacked_hists.size() - 1; t >= 0; --t) {
                legfit->AddEntry(stacked_hists[t], template_names[t].c_str(), "f");
            }
            legfit->Draw();
            legfit->SetLineWidth(0);

            gPad->SetLogy();

            // --- Ratio panel ---
            cfit->cd(2);
            gPad->SetPad(0,0,1,0.295);
            gPad->SetLeftMargin(0.15);
            gPad->SetRightMargin(0.05);
            gPad->SetBottomMargin(0.28);
            gPad->SetTopMargin(0.05);

            // Build the fitted sum histogram
            TH1D* hsum = (TH1D*)stacked_hists[0]->Clone("hsum");
            hsum->Reset();
            for (auto h : stacked_hists) hsum->Add(h);

            int nbins = data[c]->GetNbinsX();
            vector<double> xvals, ratios;
            for (int i = 1; i <= nbins; ++i) {
                double xval = data[c]->GetBinCenter(i);
                double dataval = data[c]->GetBinContent(i);
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
            gratio->GetXaxis()->SetTitle(varname[c].c_str());
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

            cfit->SaveAs((basename + "_" + varname[c] + ".pdf").c_str());

            for (auto h : stacked_hists) delete h;
            delete hsum;
            delete cfit;

        }
    }


   void plot_pulls_and_residuals(const string& basename = "fit") {
        for (size_t c = 0; c < varname.size(); ++c) {
            // Safety check
            if (c >= dh_data.size() || c >= model.size() || c >= data.size()) continue;
            if (!dh_data[c] || !model[c] || !data[c]) continue;

            // Prepare pulls
            RooPlot* fitframe = xs[c]->frame();
            dh_data[c]->plotOn(fitframe, Name("data"), MarkerStyle(20), LineColor(kBlack));
            model[c]->plotOn(fitframe, Name("fit"), LineColor(kBlue+2), LineWidth(2));
            RooHist* hpull = fitframe->pullHist("data", "fit");

            int nbins = data[c]->GetNbinsX();
            vector<double> xvals, normresiduals;
            double maxabs = 0;
            for (int i = 1; i <= nbins; ++i) {
                double xval = data[c]->GetBinCenter(i);
                double dataval = data[c]->GetBinContent(i);

                xs[c]->setVal(xval);
                double fitval = model[c]->getVal(RooArgSet(*xs[c]));

                double resid = (dataval != 0) ? (dataval - fitval) / dataval : 0.0;
                xvals.push_back(xval);
                normresiduals.push_back(resid);
                if (abs(resid) > maxabs) maxabs = abs(resid);
            }

            // Compute scaling factor to make max residual ~3
            double scale = (maxabs > 0) ? 3.0 / maxabs : 1.0;

            // Scale residuals
            for (auto& r : normresiduals) r *= scale;

            // Plot
            TGraph* gnormres = new TGraph(nbins);
            for (int i = 0; i < nbins; ++i)
                gnormres->SetPoint(i, xvals[i], normresiduals[i]);
            gnormres->SetMarkerStyle(24);
            gnormres->SetMarkerColor(kRed);

            // Create canvas
            TCanvas* ccan = new TCanvas(Form("cpullres_%s", varname[c].c_str()), "Pulls and Residuals", 1200, 800);

            // Draw pulls
            hpull->SetMarkerStyle(20);
            hpull->SetMarkerColor(kBlue+2);
            hpull->SetTitle(Form("Pulls and Scaled Residuals for %s", varname[c].c_str()));
            hpull->GetYaxis()->SetTitle("Pull / Scaled Residual");
            hpull->GetXaxis()->SetTitle(varname[c].c_str());
            hpull->Draw("AP");

            // Draw scaled normalized residuals on same plot
            gnormres->Draw("P SAME");

            // Add legend with scaling factor
            auto leg = new TLegend(0.7, 0.75, 0.9, 0.9);
            leg->AddEntry(hpull, "Pull: (Data-Fit)/Uncert.", "p");
            leg->AddEntry(gnormres, Form("Residual: (Data-Fit)/Data x %.2f", scale), "p");
            leg->SetLineWidth(0);
            leg->Draw();

            ccan->SaveAs((basename + "_" + varname[c] + "_pulls_residuals.pdf").c_str());

            delete fitframe;
            delete gnormres;
            delete ccan;
        }

    }

    void write_fit_log(const string& logname = "fit_results.log") {
        ofstream log(logname);
        if (!log.is_open()) {
            cerr << "Could not open log file: " << logname << endl;
            return;
        }
        log << "Fit Results Log\n";
        log << "====================\n";
        for (size_t c = 0; c < varname.size(); ++c) {
            log << "Channel: " << varname[c] << "\n";
            if (c < result.size() && result[c]) {
                log << "  Fit status: " << result[c]->status() << "\n";
                log << "  EDM: " << result[c]->edm() << "\n";
                log << "  NLL: " << result[c]->minNll() << "\n";
                log << "  Global correlation coefficient: " << result[c]->globalCorr() << "\n";
            } else {
                log << "  No fit result for this channel.\n";
                continue;
            }
            log << "  Fractions:\n";
            for (size_t t = 0; t < template_names.size(); ++t) {
                double val = (t < fractions.size()) ? fractions[t] : 0.0;
                double err = 0.0;
                if (t < fractions_vars.size() && fractions_vars[t])
                    err = fractions_vars[t]->getError();
                log << "    " << template_names[t] << ": " << val << " +/- " << err << "\n";
            }

            // Covariance matrix
            log << "  Covariance matrix:\n";
            const TMatrixDSym& cov = result[c]->covarianceMatrix();
            int npar = cov.GetNrows();
            for (int i = 0; i < npar; ++i) {
                for (int j = 0; j < npar; ++j)
                    log << setw(12) << cov(i, j) << " ";
                log << "\n";
            }

            // Correlation matrix
            log << "  Correlation matrix:\n";
            const TMatrixDSym& corr = result[c]->correlationMatrix();
            for (int i = 0; i < npar; ++i) {
                for (int j = 0; j < npar; ++j)
                    log << setw(12) << corr(i, j) << " ";
                log << "\n";
            }

            // Chi2/NDF and pull stats
            double chi2 = 0, pull_sum = 0, pull_sum2 = 0;
            int ndf = 0, npull = 0;
            for (int i = 1; i <= data[c]->GetNbinsX(); ++i) {
                double obs = data[c]->GetBinContent(i);
                double err = data[c]->GetBinError(i);
                xs[c]->setVal(data[c]->GetBinCenter(i));
                double exp = model[c]->getVal(RooArgSet(*xs[c])) * data[c]->Integral();
                if (err > 0) {
                    double pull = (obs - exp) / err;
                    chi2 += pow((obs - exp) / err, 2);
                    pull_sum += pull;
                    pull_sum2 += pull * pull;
                    ndf++;
                    npull++;
                }
            }
            log << "  Chi2/NDF: " << chi2 << " / " << ndf << " = " << (ndf > 0 ? chi2/ndf : 0) << "\n";
            if (npull > 0) {
                double pull_mean = pull_sum / npull;
                double pull_rms = sqrt(pull_sum2 / npull - pull_mean * pull_mean);
                log << "  Pull mean: " << pull_mean << ", Pull RMS: " << pull_rms << "\n";
            }
            log << "\n";
        }
        log.close();
        cout << "Fit results written to " << logname << endl;
    }

    void write_histograms_to_root(const string& filename = "fit_histograms.root") {
        TFile fout(filename.c_str(), "RECREATE");
        if (!fout.IsOpen()) {
            cerr << "Could not open output file: " << filename << endl;
            return;
        }

        // Write data histograms
        for (size_t c = 0; c < data.size(); ++c) {
            if (data[c]) data[c]->Write(Form("data_%s", varname[c].c_str()));
        }

        // Write template histograms (input)
        for (size_t t = 0; t < templates.size(); ++t) {
            for (size_t c = 0; c < templates[t].size(); ++c) {
                if (templates[t][c])
                    templates[t][c]->Write(Form("template_%s_%s", template_names[t].c_str(), varname[c].c_str()));
            }
        }

        // Write fitted stacked histograms and pulls/residuals
        for (size_t c = 0; c < varname.size(); ++c) {
            double total = data[c]->Integral();
            vector<TH1D*> stacked_hists;
            for (int t = templates.size() - 1; t >= 0; --t) {
                TH1D* h = (TH1D*)templates[t][c]->Clone(Form("fitted_%s_%s", template_names[t].c_str(), varname[c].c_str()));
                double frac = (t < fractions.size()) ? fractions[t] : 0.0;
                if (h->Integral() > 0) h->Scale(frac * total / h->Integral());
                stacked_hists.push_back(h);
                h->Write(); // Write each fitted template
            }
            // Write the sum of fitted templates
            if (!stacked_hists.empty()) {
                TH1D* hsum = (TH1D*)stacked_hists[0]->Clone(Form("fitted_sum_%s", varname[c].c_str()));
                hsum->Reset();
                for (auto h : stacked_hists) hsum->Add(h);
                hsum->Write();

                // --- Pulls and residuals ---
                int nbins = data[c]->GetNbinsX();
                TH1D* hpull = new TH1D(Form("pull_%s", varname[c].c_str()), "Pulls", nbins, data[c]->GetXaxis()->GetXmin(), data[c]->GetXaxis()->GetXmax());
                TH1D* hresid = new TH1D(Form("residual_%s", varname[c].c_str()), "Scaled Residuals", nbins, data[c]->GetXaxis()->GetXmin(), data[c]->GetXaxis()->GetXmax());

                // Compute pulls and residuals
                double maxabs = 0;
                vector<double> residuals;
                for (int i = 1; i <= nbins; ++i) {
                    double obs = data[c]->GetBinContent(i);
                    double err = data[c]->GetBinError(i);
                    double fitval = hsum->GetBinContent(i);
                    double pull = (err > 0) ? (obs - fitval) / err : 0.0;
                    double resid = (obs != 0) ? (obs - fitval) / obs : 0.0;
                    hpull->SetBinContent(i, pull);
                    residuals.push_back(resid);
                    if (abs(resid) > maxabs) maxabs = abs(resid);
                }
                // Scale residuals so max is ~3
                double scale = (maxabs > 0) ? 3.0 / maxabs : 1.0;
                for (int i = 1; i <= nbins; ++i) {
                    hresid->SetBinContent(i, residuals[i-1] * scale);
                }
                hpull->Write();
                hresid->Write();

                for (auto h : stacked_hists) delete h;
                delete hsum;
                delete hpull;
                delete hresid;
            }
        }

        fout.Close();
        cout << "Histograms (including pulls and residuals) written to " << filename << endl;
    }

    void processfit(const string& name){

        write_fit_log(name + "_fit_results.log");
        write_histograms_to_root(name + "_fit_histograms.root");
        plot_distributions(name + "_templates_vs_data");        
        plot_fit_results(name + "_fit_result");
        plot_pulls_and_residuals(name + "_fit_pulls_residuals");
        cout << "Fit processing complete. Results saved." << endl;

    }
    
};
 

int main(int argc, char *argv[]) {
    gStyle->SetOptStat(0);
    CommandLine CL(argc, argv);

    string dataFile = CL.Get("Data");
    vector<string> templateFiles = CL.GetStringVector("Templates"); // one file per template
    vector<string> templateNames = CL.GetStringVector("TemplateNames");
    vector<double> fractions = CL.GetDoubleVector("Fractions");
    vector<string> branches = CL.GetStringVector("Branches"); // one per channel
    vector<string> channelNames = branches; // use branch names as channel names
    string varname = CL.Get("VarName");
    vector<double> xmin = CL.GetDoubleVector("Xmin");
    vector<double> xmax = CL.GetDoubleVector("Xmax");

    int M = branches.size();
    int N = templateNames.size();

    // Load data histograms for each channel from the same file
    vector<TH1D*> data(M);
    TFile* dataF = TFile::Open(dataFile.c_str());
    for (int c = 0; c < M; ++c) {
        TH1D* h = (TH1D*)dataF->Get(branches[c].c_str());
        if (h) data[c] = (TH1D*)h->Clone();
        else cerr << "ERROR: Could not find data histogram '" << branches[c] << "' in file " << dataFile << endl;
    }

    // Load templates: [template][channel]
    vector<vector<TH1D*>> templates(N, vector<TH1D*>(M));
    for (int t = 0; t < N; ++t) {
        TFile* f = TFile::Open(templateFiles[t].c_str());
        for (int c = 0; c < M; ++c) {
            TH1D* h = (TH1D*)f->Get(branches[c].c_str());
            if (h) templates[t][c] = (TH1D*)h->Clone();
            else cerr << "ERROR: Could not find template histogram '" << branches[c] << "' in file " << templateFiles[t] << endl;
        }
        //f->Close();
    }

    fit a(
        templates,
        templateNames,
        data,
        fractions,
        branches,
        xmin,
        xmax
    );
    a.simultaneousFit();
    a.processfit("fit_results");

    return 0;
}

