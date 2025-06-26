#include <iostream>
#include <vector>
#include <cmath>
#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TF1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TPad.h"
#include "TLatex.h"


std::vector<std::vector<double>> PID_fit(double min, double max, TH1D* hist){
    //helper function for pid() that performs the actual fit
    TH1D* histCopy = (TH1D*) hist->Clone("htmp");
    std::vector<std::pair<double, double>> peaks; // (mean, amplitude pairs)

    //  All these values were individually and manually tested for fit accuracy (see Notes + histTesting function)
    double removeWindow = .5;

    std::vector<std::vector<double>> windows =
    {
        {.30, .5}, {.35, .5}, {.40, .5}, {.45, .5}, {.50, .5},
        {.55, .3}, {.60, .3}, {.65, .3}, {.70, .2}, {.75, .2},
        {.80, .2}, {.85, .2}, {.90, .5}, {.95, .5}, {1.0, .5},

        {1.05, .3}, {1.10, .3}, {1.15, .3}, {1.20, .2}, {1.25, .2},
        {1.30, .2}, {1.35, .2}, {1.40, .2}, {1.45, .2}, {1.50, .2},
        {1.55, .2}, {1.60, .3}, {1.65, .2}, {1.70, .2}, {1.75, .5},
        {1.80, .5}
    };

    for(int i = 0; i < windows.size(); i++)
    {
        double p_lo = windows[i][0];
        if(min > p_lo - .001 && min < p_lo + .001)
        {
            removeWindow = windows[i][1];
        }
    }

    if(min < .3) return {{0, 0}, {0, 0}, {0, 0}};

    //3 gaussian fit if min < 1.1 (pion-kaon distinguishability)
    else if(min < .9 && min >= .299) {
    for(int i=0; i < 3; i++) { //loops through and finds 3 maximum bins for initial guesses
        int maxBin = histCopy->GetMaximumBin();
        double mu = histCopy->GetBinCenter(maxBin);
        double A = histCopy->GetBinContent(maxBin);
        peaks.emplace_back(mu, A);
        int bLow = histCopy->GetXaxis()->FindBin(mu - removeWindow);
        int bHigh = histCopy->GetXaxis()->FindBin(mu + removeWindow);
        for(int b=bLow; b <= bHigh; b++){
            histCopy->SetBinContent(b, 0);
        }
    }
    delete histCopy;

    std::sort(peaks.begin(), peaks.end(), //puts into correct order
                [](auto &a, auto &b){ return a.first < b.first; });

    TF1 *fit = new TF1( "fit",
                        "gaus(0) + gaus(3) + gaus(6)",
                        hist->GetXaxis()->GetXmin(),
                        hist->GetXaxis()->GetXmax());

    for(int i=0; i<3; i++) {
        double mu = peaks[i].first;
        double A = peaks[i].second;
        fit->SetParameter(0+3*i, A);
        fit->SetParameter(1+3*i, mu);
        fit->SetParLimits(1+3*i, mu - removeWindow, mu + removeWindow);
        fit->SetParameter(2+3*i, .1);
        fit->SetParLimits(2+3*i, .01, .5);
    }

    hist->Fit(fit, "RQ0");
    double  mean_pion = fit->GetParameter(1), sigma_pion = fit->GetParameter(2),
            mean_kaon = fit->GetParameter(4), sigma_kaon = fit->GetParameter(5),
            mean_proton = fit->GetParameter(7), sigma_proton = fit->GetParameter(8);

    std::vector<std::vector<double>> output = {{mean_pion, sigma_pion}, {mean_kaon, sigma_kaon}, {mean_proton, sigma_proton}};
    return output;}

    //2 gaussian fit after 1.1 due to kaon-pion indistinguishability
    else if(min >= .9 && min < 1.7){
        for(int i=0; i < 2; i++) { //loops through and finds 2 maximum bins for initial guesses
        int maxBin = histCopy->GetMaximumBin();
        double mu = histCopy->GetBinCenter(maxBin);
        double A = histCopy->GetBinContent(maxBin);
        peaks.emplace_back(mu, A);
        int bLow = histCopy->GetXaxis()->FindBin(mu - removeWindow);
        int bHigh = histCopy->GetXaxis()->FindBin(mu + removeWindow);
        for(int b=bLow; b <= bHigh; b++){
            histCopy->SetBinContent(b, 0);
        }
    }
    delete histCopy;

    std::sort(peaks.begin(), peaks.end(), //puts into correct order
                [](auto &a, auto &b){ return a.first < b.first; });

    TF1 *fit = new TF1( "fit",
                        "gaus(0) + gaus(3)",
                        hist->GetXaxis()->GetXmin(),
                        hist->GetXaxis()->GetXmax());

    for(int i=0; i<2; i++) {
        double mu = peaks[i].first;
        double A = peaks[i].second;
        fit->SetParameter(0+3*i, A);
        fit->SetParameter(1+3*i, mu);
        fit->SetParLimits(1+3*i, mu - removeWindow, mu + removeWindow);
        fit->SetParameter(2+3*i, .1);
        fit->SetParLimits(2+3*i, .01, .5);
    }

    hist->Fit(fit, "RQ0");
    double  mean_pion = fit->GetParameter(1), sigma_pion = fit->GetParameter(2),
            mean_kaon = fit->GetParameter(1), sigma_kaon = fit->GetParameter(2),
            mean_proton = fit->GetParameter(4), sigma_proton = fit->GetParameter(5);

    std::vector<std::vector<double>> output = {{mean_pion, sigma_pion}, {mean_kaon, sigma_kaon}, {mean_proton, sigma_proton}};
    return output;
    }

    //1 gaussian fit after 1.8 due to kaon-pion-proton merging
    else{
        TF1 *fit = new TF1( "fit",
                            "gaus",
                            hist->GetXaxis()->GetXmin(),
                            hist->GetXaxis()->GetXmax());


        hist->Fit(fit, "RQ0");
        double  mean_pion = fit->GetParameter(1), sigma_pion = fit->GetParameter(2),
                mean_kaon = fit->GetParameter(1), sigma_kaon = fit->GetParameter(2),
                mean_proton = fit->GetParameter(1), sigma_proton = fit->GetParameter(2);

        std::vector<std::vector<double>> output = {{mean_pion, sigma_pion}, {mean_kaon, sigma_kaon}, {mean_proton, sigma_proton}};
        return output;
    }
}
