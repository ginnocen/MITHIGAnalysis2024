#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <iostream>
#include "MITHIG_CMSStyle.h"

const int ccolors[13] = {
    kBlue+2,     // deep blue
    kAzure+7,    // cyan
    kTeal+3,     // teal
    kSpring+7,   // turquoise-green
    kOrange+7,   // orange (avoiding yellow)
    kRed,        // red
    kPink+7,     // pink
    kMagenta+2,  // magenta
    kViolet+7,   // purple
    kGray+2,     // gray
    kBlack,      // black
    kOrange+2,   // extra orange
    kCyan+2      // extra cyan
};

const int scolors[4] = {
    kAzure+7,    // cyan
    kOrange+7,   // orange (avoiding yellow)
    kTeal+3,     // teal
    kPink+7,     // pink
};

void divideByWidth(TH1* input) {
    if (!input) {
        std::cerr << "Error: Null histogram pointer passed to divideByWidth function." << std::endl;
        return;
    }

    int nBins = input->GetNbinsX();
    for (int i = 1; i <= nBins; ++i) {
        double binContent = input->GetBinContent(i);
        double binError = input->GetBinError(i);
        double binWidth = input->GetBinWidth(i);

        if (binWidth != 0) {
            input->SetBinContent(i, binContent / binWidth);
            input->SetBinError(i, binError / binWidth);
        } else {
            std::cerr << "Warning: Bin width is zero for bin " << i << ". Skipping division for this bin." << std::endl;
        }
    }
}

void setErrors(TH1* hRatio, TH1* num, TH1* denom){
    for (int bin = 1; bin <= hRatio->GetNbinsX(); bin++) {
        double content1 = num->GetBinContent(bin);
        double error1 = num->GetBinError(bin);
        double content2 = denom->GetBinContent(bin);
        double error2 = denom->GetBinError(bin);

        double ratioError = 0;
        if (content2 > 0) {
            ratioError = sqrt((error1 * error1) / (content2 * content2) +
                              (content1 * content1 * error2 * error2) / (content2 * content2 * content2 * content2));
        }
        hRatio->SetBinError(bin, ratioError);
    }
}

void plotRatioLogy(vector<TH1*> hists, const char* title, vector<string> labels,
    const char* xTitle, double xmin, double xmax,
    const char* yTitle, double ymin, double ymax,
    const char* rTitle, double rmin, double rmax,
    int baseline = 0, bool logy = true,
    bool errorBars = true) {

    TLegend *leg = new TLegend(0.65, 0.65, 0.85, 0.85);
    leg->SetBorderSize(0); // Remove legend box
    leg->SetTextSize(0.04); // Reduce font size

    TPad *pad1 = new TPad(title, title, 0, 0.3, 1, 1);
    pad1->SetBottomMargin(0);
    logy ? pad1->SetLogy() : pad1->SetLogy(0);
    pad1->Draw();
    TPad *pad2 = new TPad(title, title, 0, 0, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.2);
    pad2->Draw();

    // Find global min/max for all hists in the visible x range
    double global_min = 1e30, global_max = -1e30;
    for (size_t ih = 0; ih < hists.size(); ++ih) {
        TH1* hist = hists[ih];
        double x1 = (xmin < xmax) ? xmin : hist->GetXaxis()->GetXmin();
        double x2 = (xmin < xmax) ? xmax : hist->GetXaxis()->GetXmax();
        int binmin = hist->GetXaxis()->FindBin(x1);
        int binmax = hist->GetXaxis()->FindBin(x2);
        for (int i = binmin; i <= binmax; ++i) {
            double val = hist->GetBinContent(i);
            if (val < global_min) global_min = val;
            if (val > global_max) global_max = val;
        }
    }
    // Add some margin
    double margin;
    if (logy) {
        margin = exp((log(global_max) - log((global_min > 0) ? global_min : 1)) * 1.2);
    } else {
        margin = 0.2 * (global_max - global_min);
    }

    for (int i = 0; i < hists.size(); i++) {
        pad1->cd();

        TH1* hist = hists[i];

        hist->SetStats(0);
        hist->SetTitle(title);
        hist->GetXaxis()->SetTitle(xTitle);
        hist->GetXaxis()->SetRangeUser(xmin, xmax);
        hist->GetYaxis()->SetTitle(yTitle);
        hist->GetYaxis()->SetTitleSize(0.05);
        hist->SetLineColor(ccolors[i]);

        // Auto-scale y axis if ymin < ymax, otherwise use global min/max
        if (ymin < ymax) {
            // If logy, ensure ymin > 0
            if (logy && ymin <= 0) {
            hist->GetYaxis()->SetRangeUser(1, ymax);
            } else {
            hist->GetYaxis()->SetRangeUser(ymin, ymax);
            }
        } else {
            if (logy && global_min-margin <= 0) {
                hist->GetYaxis()->SetRangeUser(1, global_max + margin);
            } else {
                hist->GetYaxis()->SetRangeUser(global_min - margin, global_max + margin);
            }
        }

        if (i == baseline) {
            hist->Draw("E SAME");
        } else {
            hist->Draw("HIST SAME");
        }
        leg->AddEntry(hist, Form("%s", labels[i].c_str()), "l");

        pad2->cd();
        if (i != baseline) {
            TH1* hRatio = (TH1*)hist->Clone(Form("ratio_TrkPt_%d", i));
            hRatio->Divide(hists[baseline]);
            hRatio->SetTitle("");
            hRatio->SetStats(0);
            hRatio->GetXaxis()->SetTitle(xTitle);
            hRatio->GetXaxis()->SetTitleSize(0.1);
            hRatio->GetXaxis()->SetLabelSize(0.08);
            hRatio->GetXaxis()->SetTitleOffset(0.4);
            hRatio->GetYaxis()->SetTitle(rTitle);
            hRatio->GetYaxis()->SetTitleSize(0.1);
            hRatio->GetYaxis()->SetLabelSize(0.08);
            hRatio->GetYaxis()->SetTitleOffset(0.4);
            hRatio->GetYaxis()->SetRangeUser(rmin, rmax);
            hRatio->SetLineColor(ccolors[i]);

            // Calculate statistical errors for the ratio
            if (errorBars) {
                setErrors(hRatio, hist, hists.at(baseline));
                hRatio->Draw("E SAME");
            } else {
                hRatio->Draw("HIST SAME");
            }

            cout << title << " " << labels[i] << " " << hRatio->Integral() << endl;

            // Print (ratio y value, ratio y value * bin width) for each bin
            for (int bin = 1; bin <= hRatio->GetNbinsX(); ++bin) {
                double y = (1 - hRatio->GetBinContent(bin)) * hists[baseline]->GetBinContent(bin);
                double w = hRatio->GetBinWidth(bin);
                cout << y << " " << y * w << endl;
            }

            TLine *line = new TLine(xmin, 1, xmax, 1);
            line->SetLineColor(kGray+2);
            line->SetLineStyle(2);
            line->Draw("SAME");
        }
    }
    pad1->cd();
    leg->Draw("SAME");
}

void plot2D(TH2* hist, const char* title,
    const char* xTitle, double xmin, double xmax,
    const char* yTitle, double ymin, double ymax,
    bool logz = true) {

    TPad *pad1 = new TPad(title, title, 0, 0, 1, 1);
    pad1->SetBottomMargin(0.18); // Increase bottom margin for x-axis labels/ticks
    pad1->SetLogx(0);
    pad1->SetLogy(0);
    pad1->SetLogz(logz);
    pad1->Draw();
    pad1->cd();

    hist->SetStats(0);
    hist->SetTitle(title);
    hist->SetTitleSize(0.03, "XYZ"); // Decrease title size
    hist->GetXaxis()->SetTitle(xTitle);
    hist->GetXaxis()->SetTitleSize(0.05);
    hist->GetXaxis()->SetLabelSize(0.045);
    hist->GetXaxis()->SetRangeUser(xmin, xmax);
    hist->GetYaxis()->SetTitle(yTitle);
    hist->GetYaxis()->SetTitleSize(0.05);
    hist->GetYaxis()->SetLabelSize(0.045);
    hist->GetYaxis()->SetRangeUser(ymin, ymax);

    hist->Draw("COLZ");
}

void plotSimple(vector<TH1*> hists, const char* title, vector<string> labels,
    const char* xTitle, double xmin, double xmax,
    const char* yTitle, double ymin, double ymax,
    bool logx = false, bool logy = false, bool binnums = false) {

    TPad *pad1 = new TPad(title, title, 0, 0, 1, 1);
    pad1->SetBottomMargin(0.18); // Increase bottom margin for x-axis labels/ticks
    logx ? pad1->SetLogx() : pad1->SetLogx(0);
    logy ? pad1->SetLogy() : pad1->SetLogy(0);
    pad1->Draw();
    pad1->cd();

    TLegend *leg = new TLegend(0.65, 0.7, 0.85, 0.87);
    leg->SetBorderSize(0); // Remove legend box
    leg->SetTextSize(0.025); // Reduce font size

    double global_min = 1e30, global_max = -1e30;

    // Find global min/max for all hists in the visible x range
    for (size_t ih = 0; ih < hists.size(); ++ih) {
        TH1* hist = hists[ih];
        double x1 = (xmin < xmax) ? xmin : hist->GetXaxis()->GetXmin();
        double x2 = (xmin < xmax) ? xmax : hist->GetXaxis()->GetXmax();
        int binmin = hist->GetXaxis()->FindBin(x1);
        int binmax = hist->GetXaxis()->FindBin(x2);
        for (int i = binmin; i <= binmax; ++i) {
            double val = hist->GetBinContent(i);
            if (val < global_min) global_min = val;
            if (val > global_max) global_max = val;
        }
    }
    // Add some margin
    double margin;
    if (logy) {
        margin = exp((log(global_max) - log((global_min > 0) ? global_min : 1)) * 1.2);
        cout<<"Global min: " << global_min << ", Global max: " << global_max << ", Margin: " << margin << endl;
    } else {
        margin = 0.2 * (global_max - global_min);
    }

    for (size_t ih = 0; ih < hists.size(); ++ih) {
        TH1* hist = hists[ih];
        hist->SetStats(0);
        hist->SetTitle(title);
        hist->GetXaxis()->SetTitle(xTitle);
        hist->GetXaxis()->SetTitleSize(0.05);
        hist->GetXaxis()->SetLabelSize(0.045);

        // Auto-scale x axis if xmin >= xmax
        if (xmin < xmax) {
            hist->GetXaxis()->SetRangeUser(xmin, xmax);
        } else {
            hist->GetXaxis()->SetRangeUser(hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
        }

        hist->GetYaxis()->SetTitle(yTitle);
        hist->GetYaxis()->SetTitleSize(0.05);
        hist->GetYaxis()->SetLabelSize(0.03);
        hist->GetYaxis()->SetTitleOffset(1);

        // Auto-scale y axis if ymin < ymax, otherwise use global min/max
        if (ymin < ymax) {
            // If logy, ensure ymin > 0
            if (logy && ymin <= 0) {
            hist->GetYaxis()->SetRangeUser(1, ymax);
            } else {
            hist->GetYaxis()->SetRangeUser(ymin, ymax);
            }
        } else {
            if (logy && global_min <= 0) {
                hist->GetYaxis()->SetRangeUser(1, global_max + margin);
            } else {
                hist->GetYaxis()->SetRangeUser(global_min - margin, global_max + margin);
            }
        }

        hist->SetLineColor(scolors[ih]);
        hist->SetLineWidth(2);

        // Set dashed line style for the second curve (ih == 1)
        if (ih == 1) {
            hist->SetLineStyle(2); // dashed
        } else {
            hist->SetLineStyle(1); // solid
        }

        // Draw histogram as usual
        if (ih == 0) {
            hist->Draw("HIST");
        } else {
            hist->Draw("HIST SAME");
        }

        // Old way: draw bin content above each bin using "TEXT0" option
        if (binnums) {
            hist->Draw("SAME TEXT0");
        }

        /*
        if (binnums) {
            for (int bin = 1; bin <= hist->GetNbinsX(); ++bin) {
            double x = hist->GetBinCenter(bin);
            double y = hist->GetBinContent(bin);
            // Place 'a' slightly above the bin content
            double y_offset = (y > 0) ? y + 0.03 * (hist->GetMaximum() - hist->GetMinimum()) : 0.03 * (hist->GetMaximum() - hist->GetMinimum());
            TLatex latex;
            latex.SetTextAlign(22);
            latex.SetTextSize(0.027);
            latex.DrawLatex(x, y_offset, Form("%.0f", y));
            }
        }
        */

        if (labels.size() > ih)
            leg->AddEntry(hist, labels[ih].c_str(), "l");
    }

    leg->Draw("SAME");
}

void LabelBinContent(TH1* hist) {
  for (int bin = 1; bin <= hist->GetNbinsX(); ++bin) {
    double x = hist->GetBinCenter(bin);
    double y = hist->GetBinContent(bin);
    double y_offset = (y > 0) ? y + 0.03 * (hist->GetMaximum() - hist->GetMinimum()) : 0.03 * (hist->GetMaximum() - hist->GetMinimum());
    TLatex latex;
    latex.SetTextAlign(22);
    latex.SetTextSize(0.027);
    latex.DrawLatex(x, y_offset, Form("%.0f", y));
  }
}

TPad* plotCMSSimple(TCanvas* c, vector<TH1*> hists, const char* title, vector<string> labels,
    vector<Int_t> linecolors, vector<Int_t> linestyles, vector<Int_t> markercolors, vector<Int_t> markerstyles,
    const char* xTitle, double xmin, double xmax,
    const char* yTitle, double ymin, double ymax,
    bool logx = false, bool logy = false,
    bool binnums = false) {

    // Get the canvas pad to pass to other functions
    TPad* pad1 = (TPad*) c->GetPad(0);
    pad1->cd();
    logy ? pad1->SetLogy() : pad1->SetLogy(0);
    logx ? pad1->SetLogx() : pad1->SetLogx(0);
    
    // >>> Apply the CMS TDR style <<<
    SetTDRStyle();

    TLegend* leg = new TLegend(0.25, 0.7, 0.68, 0.85);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.035);

    // Find global min/max for all hists in the visible x range
    double global_min = 1e30, global_max = -1e30;
    for (size_t ih = 0; ih < hists.size(); ++ih) {
        TH1* hist = hists[ih];
        double x1 = (xmin < xmax) ? xmin : hist->GetXaxis()->GetXmin();
        double x2 = (xmin < xmax) ? xmax : hist->GetXaxis()->GetXmax();
        int binmin = hist->GetXaxis()->FindBin(x1);
        int binmax = hist->GetXaxis()->FindBin(x2);
        for (int i = binmin; i <= binmax; ++i) {
            double val = hist->GetBinContent(i);
            if (val < global_min) global_min = val;
            if (val > global_max) global_max = val;
        }
    }
    // Add some margin
    double margin;
    if (logy) {
        margin = exp((log(global_max) - log((global_min > 0) ? global_min : 1)) * 1.2);
    } else {
        margin = 0.2 * (global_max - global_min);
    }

    for (int i = 0; i < hists.size(); i++) {

        TH1* hist = hists[i];

        hist->SetLineColor(linecolors[i]);
        if (linestyles[i] == 0) hist->SetLineStyle(1);
        else if (linestyles[i] > 0) hist->SetLineStyle(linestyles[i]);
        hist->SetMarkerColor(markercolors[i]);
        hist->SetMarkerStyle(markerstyles[i]);
        if (linestyles[i] == 0) hist->SetLineWidth(3);
        else if (linestyles[i] > 0) hist->SetLineWidth(2);

        hist->GetXaxis()->SetTitle(xTitle);
        hist->GetXaxis()->SetRangeUser(xmin, xmax);
        hist->GetYaxis()->SetTitle(yTitle);
        hist->GetYaxis()->SetTitleSize(0.04);

        // Auto-scale y axis if ymin < ymax, otherwise use global min/max
        if (ymin < ymax) {
            // If logy, ensure ymin > 0
            if (logy && ymin <= 0) {
            hist->GetYaxis()->SetRangeUser(1, ymax);
            } else {
            hist->GetYaxis()->SetRangeUser(ymin, ymax);
            }
        } else {
            if (logy && global_min-margin <= 0) {
                hist->GetYaxis()->SetRangeUser(1, global_max + margin);
            } else {
                hist->GetYaxis()->SetRangeUser(global_min - margin, global_max + margin);
            }
        }

        if (binnums) hist->Draw("SAME TEXT0");
        else if (linestyles[i] == 0) hist->Draw("SAME");
        else if (linestyles[i] == -1) hist->Draw("P SAME");
        else hist->Draw("HIST SAME");
        
        if (linestyles[i] == 0) leg->AddEntry(hist, Form("%s", labels[i].c_str()), "pl");
        else if (linestyles[i] == -1) leg->AddEntry(hist, Form("%s", labels[i].c_str()), "p");
        else leg->AddEntry(hist, Form("%s", labels[i].c_str()), "l");
    }
    leg->Draw("SAME");

    return pad1; // Return the pad for further customization if needed
}


TPad* plotCMSRatio(vector<TH1*> hists, const char* title, vector<string> labels,
    vector<Int_t> linecolors, vector<Int_t> linestyles, vector<Int_t> markercolors, vector<Int_t> markerstyles,
    const char* xTitle, double xmin, double xmax,
    const char* yTitle, double ymin, double ymax,
    const char* rTitle, double rmin, double rmax,
    int baseline = 0, bool logx = false, bool logy = false,
    bool errorBars = true) {

    // linestyle options
    // -1: no line, only markers [homemade]
    // 0: solid histogram with point in center
    // 1: solid histogram
    // 2: dashed histogram

    // Get the canvas pad to pass to other functions
    // Leave a 50% larger border around the figure within the canvas
    double border = 0.06; // 7.5% border on all sides (50% larger than default 5%)
    TPad *pad1 = new TPad(title, title, border, 0.25 + border, 1.0 - border, 1.0 - border);
    pad1->SetBottomMargin(0);
    logy ? pad1->SetLogy() : pad1->SetLogy(0);
    logx ? pad1->SetLogx() : pad1->SetLogx(0);
    pad1->Draw();
    TPad *pad2 = new TPad(title, title, border, border, 1.0 - border, 0.25 + border);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.2);
    logx ? pad2->SetLogx() : pad2->SetLogx(0);
    pad2->Draw();
    
    // >>> Apply the CMS TDR style <<<
    SetTDRStyle();

    TLegend* leg = new TLegend(0.55, 0.7, 0.78, 0.85);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.035);

    // Find global min/max for all hists in the visible x range
    double global_min = 1e30, global_max = -1e30;
    for (size_t ih = 0; ih < hists.size(); ++ih) {
        TH1* hist = hists[ih];
        double x1 = (xmin < xmax) ? xmin : hist->GetXaxis()->GetXmin();
        double x2 = (xmin < xmax) ? xmax : hist->GetXaxis()->GetXmax();
        int binmin = hist->GetXaxis()->FindBin(x1);
        int binmax = hist->GetXaxis()->FindBin(x2);
        for (int i = binmin; i <= binmax; ++i) {
            double val = hist->GetBinContent(i);
            if (val < global_min) global_min = val;
            if (val > global_max) global_max = val;
        }
    }
    // Add some margin
    double margin;
    if (logy) {
        margin = exp((log(global_max) - log((global_min > 0) ? global_min : 1)) * 1.2);
    } else {
        margin = 0.2 * (global_max - global_min);
    }

    for (int i = 0; i < hists.size(); i++) {
        pad1->cd();

        TH1* hist = hists[i];

        if (linestyles[i] == -1) hist->SetLineColorAlpha(0, 0);
        else hist->SetLineColor(linecolors[i]);
        if (linestyles[i] == 0) hist->SetLineStyle(1);
        else if (linestyles[i] > 0) hist->SetLineStyle(linestyles[i]);
        hist->SetMarkerColor(markercolors[i]);
        hist->SetMarkerStyle(markerstyles[i]);
        if (linestyles[i] == 0) hist->SetLineWidth(3);
        else if (linestyles[i] > 0) hist->SetLineWidth(2);

        hist->GetXaxis()->SetTitle(xTitle);
        hist->GetXaxis()->SetRangeUser(xmin, xmax);
        hist->GetYaxis()->SetTitle(yTitle);
        hist->GetYaxis()->SetTitleSize(0.05);
        hist->GetYaxis()->SetTitleOffset(0.7);

        // Auto-scale y axis if ymin < ymax, otherwise use global min/max
        if (ymin < ymax) {
            // If logy, ensure ymin > 0
            if (logy && ymin <= 0) {
            hist->GetYaxis()->SetRangeUser(1, ymax);
            } else {
            hist->GetYaxis()->SetRangeUser(ymin, ymax);
            }
        } else {
            if (logy && global_min-margin <= 0) {
                hist->GetYaxis()->SetRangeUser(1, global_max + margin);
            } else {
                hist->GetYaxis()->SetRangeUser(global_min - margin, global_max + margin);
            }
        }

        if (linestyles[i] == 0) hist->Draw("SAME");
        else if (linestyles[i] == -1) hist->Draw("P SAME");
        else hist->Draw("HIST SAME");
        
        if (linestyles[i] == 0) leg->AddEntry(hist, Form("%s", labels[i].c_str()), "pl");
        else if (linestyles[i] == -1) leg->AddEntry(hist, Form("%s", labels[i].c_str()), "p");
        else leg->AddEntry(hist, Form("%s", labels[i].c_str()), "l");

        pad2->cd();
        if (i != baseline) {
            TH1* hRatio = (TH1*)hist->Clone(Form("ratio_%s_%d", title, i));
            hRatio->Divide(hists[baseline]);
            hRatio->GetXaxis()->SetTitle(xTitle);
            hRatio->GetXaxis()->SetTitleSize(0.1);
            hRatio->GetXaxis()->SetLabelSize(0.08);
            hRatio->GetXaxis()->SetTitleOffset(1);
            hRatio->GetYaxis()->SetTitle(rTitle);
            hRatio->GetYaxis()->SetRangeUser(rmin, rmax);
            hRatio->GetYaxis()->SetTitleSize(0.08);
            hRatio->GetYaxis()->SetLabelSize(0.06);
            hRatio->GetYaxis()->SetTitleOffset(0.5);
            hRatio->SetLineColor(linecolors[i]);
            hRatio->SetLineStyle(linestyles[i]);
            hRatio->SetLineWidth(2);
            hRatio->Draw("HIST SAME");

            // Draw error bars on the ratio plot if requested
            if (errorBars) {
                setErrors(hRatio, hist, hists.at(baseline));
                hRatio->Draw("E SAME");
            } else {
                hRatio->Draw("HIST SAME");
            }

            double xlow = hRatio->GetXaxis()->GetBinLowEdge(hRatio->GetXaxis()->GetFirst());
            double xhigh = hRatio->GetXaxis()->GetBinUpEdge(hRatio->GetXaxis()->GetLast());
            TLine *line = new TLine(xlow, 1, xhigh, 1);
            line->SetLineColor(kGray+2);
            line->SetLineStyle(2);
            line->Draw("SAME");
        }
    }
    pad1->cd();
    leg->Draw("SAME");

    return pad1; // Return the main pad for further customization if needed
}