#pragma once
#include <TLegend.h>
#include <TGraph.h>
#include <TCanvas.h>
//Took a lot of these default setups from Jings /xjjlibs/xjjroot/xjjrootuti.h
namespace PlotUtils {

    const float margin_pad_left = 0.18;
    const float margin_pad_right = 0.043;
    const float margin_pad_bottom = 0.145;
    const float margin_pad_top = 0.1;

    inline void GraphLegend(const std::vector<std::pair<TObject*, std::string>>& entries, 
                            double x1=0.6, double y1=0.23, double x2=0.97, double y2=0.33) {
        TLegend* legend = new TLegend(x1, y1, x2, y2);
        legend->SetBorderSize(0);
        legend->SetFillStyle(0);
        legend->SetTextFont(42);
        legend->SetTextSize(0.03);
        legend->SetMargin(0.15);
        legend->SetEntrySeparation(0.12);
        for (const auto& entry : entries) {
            legend->AddEntry(entry.first, entry.second.c_str(), "p");
        }
        legend->Draw();
    }

    void setgstyle(Int_t padtick = 0, Width_t lwidth = 2 )
    {
        gStyle->SetOptStat(0);
        gStyle->SetEndErrorSize(0);
        gStyle->SetTextSize(0.05);
        gStyle->SetTextFont(42);
        gStyle->SetPadRightMargin(PlotUtils::margin_pad_right);
        gStyle->SetPadLeftMargin(PlotUtils::margin_pad_left);
        gStyle->SetPadTopMargin(PlotUtils::margin_pad_top);
        gStyle->SetPadBottomMargin(PlotUtils::margin_pad_bottom);
        gStyle->SetTitleX(0.5);
        gStyle->SetPadTickX(0);
        gStyle->SetPadTickY(0);
        gStyle->SetLineWidth(1);
        gStyle->SetFrameLineWidth(1);
        if(padtick==1 || padtick==3)
            {
            gStyle->SetPadTickX(1);
            gStyle->SetPadTickY(1);
            }
        if(padtick==2 || padtick==3) { gStyle->SetLineWidth(lwidth); gStyle->SetFrameLineWidth(lwidth);}
    }

    template <class T>
    void setgraphstyle(T* hempty, float xoffset=0, float yoffset=0) {
        hempty->GetXaxis()->CenterTitle();
        hempty->GetYaxis()->CenterTitle();
        hempty->GetXaxis()->SetTitleOffset(1.10 + xoffset);
        hempty->GetYaxis()->SetTitleOffset(1.30 + yoffset);
        hempty->GetXaxis()->SetTitleSize(0.05);
        hempty->GetYaxis()->SetTitleSize(0.05);
        hempty->GetXaxis()->SetTitleFont(42);
        hempty->GetYaxis()->SetTitleFont(42);
        hempty->GetXaxis()->SetLabelFont(42);
        hempty->GetYaxis()->SetLabelFont(42);
        hempty->GetXaxis()->SetLabelSize(0.045);
        hempty->GetYaxis()->SetLabelSize(0.045);
        hempty->SetStats(0);
        hempty->SetMarkerStyle(20);
        hempty->SetMarkerSize(1.1);
        hempty->SetMarkerColor(kBlack);
        hempty->SetLineColor(kBlack);
    }

}