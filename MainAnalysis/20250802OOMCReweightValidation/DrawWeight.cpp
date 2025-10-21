
#include <TH1D.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TGraph.h>
#include <TLegend.h>
#include <iostream>
#include <vector>
#include <string>


void PlotTogether(
    const std::vector<TH1D*>& hists,
    std::string histname,
    const std::vector<std::string>& labels,
    const std::string& outDir,
    const std::string& outname,
    bool xlog = false,
    bool ylog = false,
    bool normalize = true,
    double legendx = 0.65,
    double xmin = -999, // default: do not set,
    double xmax = -999,
    double ymin = -999,
    double ymax = -999,
    const std::string& xtitle = "",
    const std::string& ytitle = ""){
    if (hists.size() != labels.size() || hists.empty()) {
        std::cerr << "Mismatch between histograms and labels!" << std::endl;
        return;
    }

    std::vector<int> colors = {
        kBlack,           // neutral
        kRed+1,           // red
        kOrange+1,        // orange
        kGreen+2,         // green
        kAzure+1,         // blue-ish
        kMagenta+2        // purple-pink
    };

    std::vector<int> styles = {
        1,                // solid
        2,                // dashed
        3,                // dotted
        4,                // dot-dash
        5                 // long dash
    };

    TCanvas* c = new TCanvas("c", "Comparison", 800, 600);

    double maxY = 0;
    std::vector<TH1D*> clones;
    std::vector<double> integrals;
    for (size_t i = 0; i < hists.size(); ++i) {
        if (!hists[i]) continue;
        TH1D* h = (TH1D*)hists[i]->Clone(("hclone" + std::to_string(i)).c_str());
        double integral = h->Integral();
        integrals.push_back(integral);
        if (normalize && integral > 0) h->Scale(1.0 / integral);
        h->SetLineColor(colors[i % colors.size()]);
       // h->SetLineStyle(styles[i % styles.size()]); // Different line style for each histogram
        h->SetLineWidth(2);
        double localMax = h->GetMaximum();
        if (localMax > maxY) maxY = localMax;
        clones.push_back(h);
    }
    // Print out integrals for the histograms actually plotted
    std::cout << "Integrals for plotted histograms:" << std::endl;
    for (size_t i = 0; i < clones.size() && i < integrals.size(); ++i) {
        std::cout << "  " << labels[i] << ": " << integrals[i] << std::endl;
    }

    // Set y axis range to 1.2 * maxY on the first histogram
    if (!clones.empty()) clones[0]->SetMaximum(maxY * 1.2);
    // Set xmin/xmax if requested
    if (!clones.empty()) {
        // X axis range
        double xaxis_min = clones[0]->GetXaxis()->GetXmin();
        double xaxis_max = clones[0]->GetXaxis()->GetXmax();
        if (xmin != -999){
            xaxis_min = xmin;
        }
        if (xmax != -999){
            xaxis_max = xmax;
        }
        clones[0]->GetXaxis()->SetRangeUser(xmin, xmax);

        // Y axis range
        if (ymin != -999 && ymax != -999) {
            clones[0]->SetMinimum(ymin);
            clones[0]->SetMaximum(ymax);
        } else if (ymin != -999) {
            clones[0]->SetMinimum(ymin);
        } else if (ymax != -999) {
            clones[0]->SetMaximum(ymax);
        }
    }

    // Set axis labels from histogram if available
    if (!clones.empty()) {
        // Use provided xtitle/ytitle if not empty, otherwise use from histogram
        const char* xlab = xtitle.empty() ? clones[0]->GetXaxis()->GetTitle() : xtitle.c_str();
        const char* ylab = ytitle.empty() ? clones[0]->GetYaxis()->GetTitle() : ytitle.c_str();
        clones[0]->GetXaxis()->SetTitle(xlab);
        clones[0]->GetYaxis()->SetTitle(ylab);
        clones[0]->SetName(histname.c_str());
    }


    // Draw all histograms
    for (size_t i = 0; i < clones.size(); ++i) {
        if (i == 0) { // Data: draw as points with markers
            clones[i]->SetMarkerStyle(20 + i); // Different marker for each, 20 is a good default for data
            clones[i]->SetMarkerColor(colors[i % colors.size()]);
            clones[i]->SetLineColor(colors[i % colors.size()]);
            clones[i]->SetLineWidth(2);
            clones[i]->Draw("E1"); // Draw data with error bars and markers
        } else { // MC: draw as lines
            clones[i]->SetLineColor(colors[i % colors.size()]);
        //    clones[i]->SetLineStyle(styles[i % styles.size()]);
            clones[i]->SetLineWidth(2);
            clones[i]->Draw("HIST SAME");
        }
    }
    gStyle->SetOptStat(0); // Disable statistics box

    TLegend* leg = new TLegend(legendx, 0.7, legendx+0.2, 0.88);
    for (size_t i = 0; i < labels.size(); ++i)
        leg->AddEntry(clones[i], labels[i].c_str(), "l");
    leg->Draw();

    if (xlog) {
        c->SetLogx();
    }
    if (ylog) {
        clones[0]->SetMinimum(1e-8); // Ensure plot is visible on log scale
        c->SetLogy();

    }
    
    c->SaveAs((outDir + "/" + outname).c_str());
    delete c;
}

void PlotTogetherRatio(
    const std::vector<TH1D*>& hists,
    std::string histname,
    const std::vector<std::string>& labels,
    const std::string& outDir,
    const std::string& outname,
    bool xlog = false,
    bool ylog = false,
    bool normalize = true,
    double legendx = 0.65,
    double xmin = -999, // default: do not set,
    double xmax = -999,
    double ymin = -999,
    double ymax = -999,
    const std::string& xtitle = "",
    const std::string& ytitle = ""){
    if (hists.size() != labels.size() || hists.empty()) {
        std::cerr << "Mismatch between histograms and labels!" << std::endl;
        return;
    }

    std::vector<int> colors = {
        kRed+1,           // red
        kOrange+1,        // orange
        kGreen+2,         // green
        kAzure+1,         // blue-ish
        kMagenta+2        // purple-pink
    };
    
    std::vector<int> styles = {
        2,                // dashed
        3,                // dotted
        4,                // dot-dash
        5                 // long dash
    };
    TCanvas* c = new TCanvas("c", "Comparison", 800, 600);

    double maxY = 0;
    std::vector<TH1D*> clones;
    std::vector<double> integrals;
    for (size_t i = 0; i < hists.size(); ++i) {
        if (!hists[i]) continue;
        TH1D* h = (TH1D*)hists[i]->Clone(("hclone" + std::to_string(i)).c_str());
        double integral = h->Integral();
        integrals.push_back(integral);
        if (normalize && integral > 0) h->Scale(1.0 / integral);
        h->SetLineColor(colors[i % colors.size()]);
      //  h->SetLineStyle(styles[i % styles.size()]); // Different line style for each histogram
        h->SetLineWidth(2);
        double localMax = h->GetMaximum();
        if (localMax > maxY) maxY = localMax;
        clones.push_back(h);
    }
    // Print out integrals for the histograms actually plotted
    std::cout << "Integrals for plotted histograms:" << std::endl;
    for (size_t i = 0; i < clones.size() && i < integrals.size(); ++i) {
        std::cout << "  " << labels[i] << ": " << integrals[i] << std::endl;
    }

    // Set y axis range to 1.2 * maxY on the first histogram
    if (!clones.empty()) clones[0]->SetMaximum(maxY * 1.2);
    // Set xmin/xmax if requested
    if (!clones.empty()) {
        // X axis range
        if (xmin != -999 && xmax != -999) {
            clones[0]->GetXaxis()->SetRangeUser(xmin, xmax);
        } else if (xmin != -999) {
            clones[0]->GetXaxis()->SetRangeUser(xmin, clones[0]->GetXaxis()->GetXmax());
        } else if (xmax != -999) {
            clones[0]->GetXaxis()->SetRangeUser(clones[0]->GetXaxis()->GetXmin(), xmax);
        }
        // Y axis range
        if (ymin != -999 && ymax != -999) {
            clones[0]->SetMinimum(ymin);
            clones[0]->SetMaximum(ymax);
        } else if (ymin != -999) {
            clones[0]->SetMinimum(ymin);
        } else if (ymax != -999) {
            clones[0]->SetMaximum(ymax);
        }
    }

    // Set axis labels from histogram if available
    if (!clones.empty()) {
        // Use provided xtitle/ytitle if not empty, otherwise use from histogram
        const char* xlab = xtitle.empty() ? clones[0]->GetXaxis()->GetTitle() : xtitle.c_str();
        const char* ylab = ytitle.empty() ? clones[0]->GetYaxis()->GetTitle() : ytitle.c_str();
        clones[0]->GetXaxis()->SetTitle(xlab);
        clones[0]->GetYaxis()->SetTitle(ylab);
        clones[0]->SetName(histname.c_str());
    }


    // Draw all histograms
    for (size_t i = 0; i < clones.size(); ++i) {
        if (i == 0) { // Data: draw as points with markers
          // clones[i]->SetMarkerStyle(20 + i); // Different marker for each, 20 is a good default for data
          //  clones[i]->SetMarkerColor(colors[i % colors.size()]);
            clones[i]->SetLineColor(colors[i % colors.size()]);
            clones[i]->SetLineWidth(2);
            clones[i]->Draw("HIST"); // Draw data with error bars and markers
        } else { // MC: draw as lines
            clones[i]->SetLineColor(colors[i % colors.size()]);
            clones[i]->SetLineWidth(2);
            clones[i]->Draw("HIST SAME");
        }
    }
    gStyle->SetOptStat(0); // Disable statistics box

    TLegend* leg = new TLegend(legendx, 0.7, legendx+0.2, 0.88);
    for (size_t i = 0; i < labels.size(); ++i)
        leg->AddEntry(clones[i], labels[i].c_str(), "l");
    leg->Draw();

    if (xlog) {
        c->SetLogx();
    }
    if (ylog) {
        clones[0]->SetMinimum(1e-3); // Ensure plot is visible on log scale
        c->SetLogy();
    }
    
    c->SaveAs((outDir + "/" + outname).c_str());
    delete c;
}

int DrawWeight( 
    std::string MCoutputVZ,
    std::string MCoutputMult,
    std::string MCoutputPt,
    std::string MCoutput,
    std::string DataOutput,
    std::string outDir = "/home/xirong/OOAnalysis_2025/072925ReweightUpdatedCodes/Plots/080425ReweightNeNeHijingTest"
) {
    // Open ROOT files
    TFile* f_data = TFile::Open(DataOutput.c_str());
    TFile* f_mc   = TFile::Open(MCoutput.c_str());
    TFile* f_mc_vz = TFile::Open(MCoutputVZ.c_str());
    TFile* f_mc_mult = TFile::Open(MCoutputMult.c_str());
    TFile* f_mc_pt = TFile::Open(MCoutputPt.c_str());

    if (!f_data || f_data->IsZombie()) {
        std::cerr << "Could not open data file!" << std::endl;
        return 1;
    }
    if (!f_mc || f_mc->IsZombie()
    ||!f_mc_mult || !f_mc_vz || !f_mc_pt
    || f_mc_mult->IsZombie() || f_mc_vz->IsZombie() || f_mc_pt->IsZombie()) {
        std::cerr << "Could not open MC file!" << std::endl;
        return 1;
    }
    
    // List of histogram base names to process
    std::vector<std::string> histNames = {"VZ",
                                        "leadingPtEta1p0_sel",
                                        "HFEMaxPlus",
                                        "HFEMaxMinus",
                                        "hiHFMinus_pf",
                                        "hiHFPlus_pf",
                                        "hiHF_pf",
                                        "multiplicityEta2p4",
                                        "multiplicityEta1p0",
                                        "trkPt",
                                        "trkEta",
                                        "trkDxyAssociatedVtx",
                                        "trkDzAssociatedVtx"
                                    }; // Add more as needed
    std::vector<bool> xlog = {
        false, // VZ
        false, // leadingPtEta1p0_sel
        false, // HFEMaxPlus
        false, // HFEMaxMinus
        false, // hiHFMinus_pf
        false, // hiHFPlus_pf
        false, // hiHF_pf
        true, // multipicityEta2p4
        true,  // multipicityEta1p0
        false, // trkPt
        false,  // trkEta
        false, // trkDxyAssociatedVtx
        false // trkDzAssociatedVtx
    };

    std::vector<bool> ylog = {
        false, // VZ
        true, // leadingPtEta1p0_sel
        false, // HFEMaxPlus
        false, // HFEMaxMinus
        false, // hiHFMinus_pf
        false, // hiHFPlus_pf
        false, // hiHF_pf
        false, // multipicityEta2p4
        false,  // multipicityEta1p0
        true, // trkPt
        false,  // trkEta
        true, // trkDxyAssociatedVtx
        true // trkDzAssociatedVtx
    };
    

    std::vector<double> legendx = {
        0.65, // VZ
        0.65, // leadingPtEta1p0_sel
        0.65, // HFEMaxPlus
        0.65, // HFEMaxMinus
        0.65, // hiHFMinus_pf
        0.65, // hiHFPlus_pf
        0.65, // hiHF_pf
        0.65, // multipicityEta2p4
        0.65,  // multipicityEta1p0
        0.65, // trkPt
        0.65, // trkEta
        0.65, // trkDxyAssociatedVtx
        0.65 // trkDzAssociatedVtx
    };

   // std::vector<std::string> histNames = {"multipicityEta2p4"};

    for (size_t i = 0; i < histNames.size(); ++i) {
        const std::string& baseName = histNames[i];
        // Compose histogram names
        std::string dataName = "histDATA_" + baseName;
        std::string mcNoWeightName = "h_noWeight_" + baseName;
        std::string mcWeightedName = "h_weighted_" + baseName;

        // Retrieve histograms
        TH1D* h_data = (TH1D*)f_data->Get(dataName.c_str());
        TH1D* h_mc_nw = (TH1D*)f_mc->Get(mcNoWeightName.c_str());
        TH1D* h_mc_rw = (TH1D*)f_mc->Get(mcWeightedName.c_str());
        TH1D* h_mc_rw_vz = (TH1D*)f_mc_vz->Get(mcWeightedName.c_str());
        TH1D* h_mc_rw_mult = (TH1D*)f_mc_mult->Get(mcWeightedName.c_str());
        TH1D* h_mc_rw_pt = (TH1D*)f_mc_pt->Get(mcWeightedName.c_str());
        if(!h_mc_nw || !h_mc_rw || !h_mc_rw_vz || !h_mc_rw_mult || !h_data) {
            std::cout << "No weighted histogram found for " << baseName << ", skipping Pt weighted ratio." << std::endl;
            continue;
        }

        cout << "number of bins for MC branch " << baseName << " unweighted : " << h_mc_nw->GetNbinsX() << endl;
        cout << "number of bins for MC branch " << baseName << " weighted: " << h_mc_rw->GetNbinsX() << endl;
        cout << "number of bins for MC branch " << baseName << " weighted VZ: " << h_mc_rw_vz->GetNbinsX() << endl;
        cout << "number of bins for MC branch " << baseName << " weighted Mult: " << h_mc_rw_mult->GetNbinsX() << endl;
        cout << "number of bins for MC branch " << baseName << " weighted Pt: " << (h_mc_rw_pt ? h_mc_rw_pt->GetNbinsX() : 0) << endl;
        // Create ratio histograms: MC (no weight)/Data and MC (Vz weighted)/Data

        TH1D* h_ratio_nw = nullptr;
        TH1D* h_ratio_rw = nullptr;
        TH1D* h_ratio_rw_vz = nullptr;
        TH1D* h_ratio_rw_mult = nullptr;
        TH1D* h_ratio_rw_pt = nullptr;


        if (!h_data || !h_mc_nw || !h_mc_rw || !h_mc_rw_mult || !h_mc_rw_vz) {
            std::cerr << "One or more histograms not found for " << baseName << "!" << std::endl;
            continue;
        }

        h_ratio_nw = (TH1D*)h_data->Clone((baseName + "_ratio_nw").c_str());
        h_ratio_nw->Divide(h_mc_nw);

        h_ratio_rw = (TH1D*)h_data->Clone((baseName + "_ratio_rw").c_str());
        h_ratio_rw->Divide(h_mc_rw);

        h_ratio_rw_vz = (TH1D*)h_data->Clone((baseName + "_ratio_rw_vz").c_str());
        h_ratio_rw_vz->Divide(h_mc_rw_vz);

        h_ratio_rw_mult = (TH1D*)h_data->Clone((baseName + "_ratio_mult").c_str());
        h_ratio_rw_mult->Divide(h_mc_rw_mult);

        if (h_mc_rw_pt && h_data) {
            h_ratio_rw_pt = (TH1D*)h_data->Clone((baseName + "_ratio_pt").c_str());
            h_ratio_rw_pt->Divide(h_mc_rw_pt);
        }

        // Create vectors of histograms and labels
        std::vector<TH1D*> hists = {h_data, h_mc_nw, h_mc_rw, h_mc_rw_vz, h_mc_rw_mult};
        std::vector<TH1D*> ratios = {h_ratio_nw, h_ratio_rw, h_ratio_rw_vz, h_ratio_rw_mult};
        std::vector<std::string> labels = { "Data", "MC (no weight)", "MC (All weighted)", "MC (Vz weighted)","MC (mult weighted)"};
        std::vector<std::string> ratiolabels = {
            "Data/MC (no weight)",
            "Data/MC (All weighted)",
            "Data/MC (Vz weighted)",
            "Data/MC (Mult weighted)"
        };
        if (h_mc_rw_pt && h_ratio_rw_pt) {
            hists.push_back(h_mc_rw_pt);
            ratios.push_back(h_ratio_rw_pt);
            labels.push_back("MC (Pt weighted)");
            ratiolabels.push_back("Data/MC (Pt weighted)");
        }
        // Output file names
        std::string out_norm   = baseName + "_Comparison_norm.png";
        std::string out_ratio   = baseName + "_Ratio_norm.png";

        double xmin = -999;
        double xmax = -999;
        double ymin = -999;
        double ymax = -999; 
        double ratioymin = -999;
        double ratioymax = -999;
        if (baseName.find("multiplicity") != std::string::npos) {
            xmin = 1;
        }
        if (baseName.find("hiHF") != std::string::npos) {
            xmax = 300; 
        }

        if (baseName == "trkEta") {
            xmin = -2.4; 
            xmax = 2.4;  
        }

        if (baseName == "trkPt") {
            cout << "Processing trkPt histogram" << std::endl;
            xmin = 0; 
            xmax = 1000; 
            ratioymin = 0;
            ratioymax = 2;
        }

        if (baseName == "hiHF_pf") {
            ratioymin = 0; 
            ratioymax = 2; 
        }

        if (baseName.find("AssociatedVtx") != std::string::npos) {
            xmin = -1; 
            xmax = 1; 
            ratioymin = 0;
            ratioymax = 3;
        }

        // Call plotting function
        PlotTogether(hists, baseName, labels, outDir,  out_norm, xlog[i], ylog[i], true, legendx[i], xmin, xmax, ymin, ymax,
            histNames[i], "Counts");
        PlotTogetherRatio(ratios, baseName + " ratio", ratiolabels, outDir, out_ratio, xlog[i], false, false, legendx[i], xmin, xmax, ratioymin, ratioymax,
            histNames[i], "Data/MC Ratio");

        delete h_data;
        delete h_mc_nw;
        delete h_mc_rw;
        delete h_mc_rw_mult;
        delete h_mc_rw_vz;
    }


    // Clean up
    f_data->Close();
    f_mc->Close();
    f_mc_mult->Close();
    f_mc_vz->Close();
    delete f_data;
    delete f_mc;
    delete f_mc_mult;
    delete f_mc_vz;
    

    return 0;
}

