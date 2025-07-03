#include "GraphingUtils.h"
#include "EfficiencyResults.h"
#include "Plot.h"
#include "Color.h"
#include <TCanvas.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TMarker.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TLegend.h>
#include <set>
#include <TH1F.h>
#include <map>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

void drawROCmultiShapes(
    const vector<EfficiencyResults>& resultsVec,
    const vector<vector<float>>& effVecs,
    const vector<vector<float>>& purityVecs,
    const vector<string>& labels,
    const HistVar1D& hvar) 
{
    vector<int> markerStyles = {20, 21, 22, 23, 29, 33, 34, 39, 41, 43, 45, 47};

    // 1. Collect all unique HFEmax_Offline_min1 values for color mapping
    set<int> uniqueMin1;
    for (const auto& res : resultsVec) {
        for (float min1 : res.HFEmax_Offline_min1) {
            uniqueMin1.insert(static_cast<int>(min1));
        }
    }

    // 2. Generate rainbow palette for unique min1 values
    int colorOffset = 4000 + 1;
    vector<int> colors = getRainbow(uniqueMin1.size(), colorOffset);

    // 3. Map each unique min1 value to a color
    map<int, int> min1ToColor;
    size_t idx = 0;
    for (int min1 : uniqueMin1) min1ToColor[min1] = colors[idx++];

    PlotUtils::setgstyle();
    TCanvas* c = new TCanvas("cROCmulti", "", 800, 800);
    TMultiGraph* mg = new TMultiGraph();

    cout << "------- Drawing ROC Curves -------" << endl;
    cout << "Number of efficiency vectors: " <<  effVecs.size() << endl;
    cout << "Number of unique min1 values: " << uniqueMin1.size() << endl;
    cout << "Number of results: " << resultsVec.size() << endl;
    cout << "Number of Purity vectors: " << purityVecs.size() << endl;

    // 4. Plot, assigning color by HFEmax_Offline_min1 and marker style by curve index
    vector<pair<TObject*, string>> legendEntries;
    for (int i = 0; i < resultsVec.size(); ++i) {
        const auto& res = resultsVec[i];
        for (size_t j = 0; j < res.HFEmax_Offline_min1.size(); ++j) {
            int min1 = static_cast<int>(res.HFEmax_Offline_min1[j]);
            int min2 = static_cast<int>(res.HFEmax_Offline_min2[j]);
            int color = min1ToColor[min1];

            TGraph* ptGraph = new TGraph(1, &effVecs[i][j], &purityVecs[i][j]);
            cout << "Plotting point for min1 = " << min1 << ", min2 = " << min2
                 << ", color = " << color << ", marker style = " << markerStyles[i % markerStyles.size()] << endl;
            ptGraph->SetMarkerColor(color);
            ptGraph->SetMarkerStyle(markerStyles[i % markerStyles.size()]);
            ptGraph->SetMarkerSize(1.5);
            mg->Add(ptGraph, "P");

            // Label logic
            std::string label;
            if (min2 == 0)
                label = Form("%.1f OR", res.HFEmax_Offline_min1[j]);
            else if (min1 == min2)
                label = Form("%.1f AND", res.HFEmax_Offline_min1[j]);
            else
                label = Form("%.1f", res.HFEmax_Offline_min1[j]);
            legendEntries.push_back({ptGraph, label});
        }
    }

    mg->SetTitle(hvar.histTitle.c_str());
    mg->Draw("A");
    mg->GetXaxis()->SetTitle(hvar.xLabel.c_str());
    mg->GetYaxis()->SetTitle(hvar.yLabel.c_str());
    mg->GetXaxis()->SetLimits(hvar.xmin, hvar.xmax);
    mg->SetMinimum(hvar.ymin);
    mg->SetMaximum(hvar.ymax);
    c->SetGrid();

    // Color legend for min1 values
    vector<pair<TObject*, string>> colorLegendEntries;
    for (const auto& kv : min1ToColor) {
        TMarker* m = new TMarker(0, 0, 20);
        m->SetMarkerColor(kv.second);
        m->SetMarkerSize(1.3);
        colorLegendEntries.push_back({m, Form("Offline min1 = %d", kv.first)});
    }
    PlotUtils::GraphLegend(colorLegendEntries, 0.65, 0.25, 0.95, 0.45);

    // Marker style legend for each curve label
    vector<pair<TObject*, string>> markerLegendEntries;
    for (size_t i = 0; i < labels.size(); ++i) {
        TMarker* m = new TMarker(0, 0, markerStyles[i % markerStyles.size()]);
        m->SetMarkerColor(kBlack);
        m->SetMarkerSize(1.5);
        markerLegendEntries.push_back({m, labels[i]});
    }
    PlotUtils::GraphLegend(markerLegendEntries, 0.25, 0.15, 0.45, 0.35);
    // Draw AND/OR legend for each point
    PlotUtils::GraphLegend(legendEntries, 0.5, 0.15, 0.8, 0.45);

    // Optional: Add text annotations
    const auto& res1 = resultsVec[0];
    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.035);
    latex.DrawLatex(0.6, 0.85, Form("HIJING (%.1f b)", res1.xsec_had));
    latex.DrawLatex(0.6, 0.82, Form("Starlight SD (%.1f b)", res1.xsec_SD));
    latex.DrawLatex(0.6, 0.79, Form("Starlight DD (%.5f b)", res1.xsec_DD));
    latex.DrawLatex(0.23, 0.55, "PVFilter == 1");
    latex.DrawLatex(0.23, 0.52, "ClusterCompatibilityFilter == 1");
    latex.DrawLatex(0.23, 0.49, "abs(V_{z}) < 15");
    latex.DrawLatex(0.23, 0.46, "npart > 1 for HIJING");

    c->SaveAs((hvar.outFolderName + "Puritycurve_AllTrackptcuts_shapes_" + hvar.outFileName +  ".png").c_str());
}