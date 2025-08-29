#include "MITHIG_CMSStyle.h"
#include "scrape.C"
using namespace std;

void plot1(){

    vector<float> nparts_XeXe = {236.1,206.3,141.2,68.5,27.2,10.55};
    vector<float> nparts_XeXe_errorsup = {1.3,1.7,1.8,2.2,1.6,0.78};
    vector<float> nparts_XeXe_errorsdown = {1.3,1.7,1.8,2.2,1.6,0.78};

    vector<float> npartx_PbPb = {384.3,333.3,226.7,109.2,42.2,11.1};
    vector<float> npartx_PbPb_errorsup = {1.8,3.0,5.2,4.3,3.0,1.3};
    vector<float> npartx_PbPb_errorsdown = {-2.0,-3.2,-5.3,-4.2,-2.9,-1.2};


    TGraphAsymmErrors* gXeXe_pt1 = new TGraphAsymmErrors(6);
    TGraphAsymmErrors* gXeXe_pt2 = new TGraphAsymmErrors(6);
    TGraphAsymmErrors* gXeXe_pt3 = new TGraphAsymmErrors(6);

    TGraphAsymmErrors* gPbPb_pt1 = new TGraphAsymmErrors(6);
    TGraphAsymmErrors* gPbPb_pt2 = new TGraphAsymmErrors(6);
    TGraphAsymmErrors* gPbPb_pt3 = new TGraphAsymmErrors(6);

    TGraphAsymmErrors* OOpointpt1 = new TGraphAsymmErrors(1);
    TGraphAsymmErrors* OOpointpt2 = new TGraphAsymmErrors(1);
    TGraphAsymmErrors* OOpointpt3 = new TGraphAsymmErrors(1);

    TGraphAsymmErrors* NeNepointpt1 = new TGraphAsymmErrors(1);
    TGraphAsymmErrors* NeNepointpt2 = new TGraphAsymmErrors(1);
    TGraphAsymmErrors* NeNepointpt3 = new TGraphAsymmErrors(1);

    OOpointpt1->SetPoint(0, 10.8, getSmallSystemEntry(0,1).y);
    OOpointpt2->SetPoint(0, 10.8, getSmallSystemEntry(4,1).y);
    OOpointpt3->SetPoint(0, 10.8, getSmallSystemEntry(9,1).y);
    OOpointpt1->SetPointError(0, 0, 0, getSmallSystemEntry(0,1).eyl, getSmallSystemEntry(0,1).eyh);
    OOpointpt2->SetPointError(0, 0, 0, getSmallSystemEntry(4,1).eyl, getSmallSystemEntry(4,1).eyh);
    OOpointpt3->SetPointError(0, 0, 0, getSmallSystemEntry(9,1).eyl, getSmallSystemEntry(9,1).eyh);

    NeNepointpt1->SetPoint(0, 12.7, getSmallSystemEntry(0,2).y);
    NeNepointpt2->SetPoint(0, 12.7, getSmallSystemEntry(4,2).y);
    NeNepointpt3->SetPoint(0, 12.7, getSmallSystemEntry(9,2).y);
    NeNepointpt1->SetPointError(0, 0, 0, getSmallSystemEntry(0,2).eyl, getSmallSystemEntry(0,2).eyh);
    NeNepointpt2->SetPointError(0, 0, 0, getSmallSystemEntry(4,2).eyl, getSmallSystemEntry(4,2).eyh);
    NeNepointpt3->SetPointError(0, 0, 0, getSmallSystemEntry(9,2).eyl, getSmallSystemEntry(9,2).eyh);

    for(int i = 0; i<6; i++){

        gXeXe_pt1->SetPoint(i, nparts_XeXe[i], getXeXeEntry(14,i).y);
        gXeXe_pt1->SetPointError(i, nparts_XeXe_errorsdown[i], nparts_XeXe_errorsup[i], getXeXeEntry(14,i).eyl, getXeXeEntry(14,i).eyh);

        gPbPb_pt1->SetPoint(i, npartx_PbPb[i], getPbPbEntry(14,i).y);
        gPbPb_pt1->SetPointError(i, npartx_PbPb_errorsdown[i], npartx_PbPb_errorsup[i], getPbPbEntry(14,i).eyl, getPbPbEntry(14,i).eyh);

        gXeXe_pt2->SetPoint(i, nparts_XeXe[i], getXeXeEntry(18,i).y);
        gXeXe_pt2->SetPointError(i, nparts_XeXe_errorsdown[i], nparts_XeXe_errorsup[i], getXeXeEntry(18,i).eyl, getXeXeEntry(18,i).eyh);

        gPbPb_pt2->SetPoint(i, npartx_PbPb[i], getPbPbEntry(18,i).y);
        gPbPb_pt2->SetPointError(i, npartx_PbPb_errorsdown[i], npartx_PbPb_errorsup[i], getPbPbEntry(18,i).eyl, getPbPbEntry(18,i).eyh);

        gXeXe_pt3->SetPoint(i, nparts_XeXe[i], getXeXeEntry(23,i).y);
        gXeXe_pt3->SetPointError(i, nparts_XeXe_errorsdown[i], nparts_XeXe_errorsup[i], getXeXeEntry(23,i).eyl, getXeXeEntry(23,i).eyh);

        gPbPb_pt3->SetPoint(i, npartx_PbPb[i], getPbPbEntry(23,i).y);
        gPbPb_pt3->SetPointError(i, npartx_PbPb_errorsdown[i], npartx_PbPb_errorsup[i], getPbPbEntry(23,i).eyl, getPbPbEntry(23,i).eyh);

    }

    TCanvas* c = new TCanvas();
    c->cd();
    c->SetLogx();


    TLegend* L = new TLegend(0.6,0.6,0.88,0.88);
    L->SetTextSize(0.04);
    L->AddEntry(gPbPb_pt2, "PbPb 5.02 TeV");
    L->AddEntry(gXeXe_pt2, "XeXe 5.44 TeV");
    L->AddEntry(OOpointpt2, "OO 5.02 TeV");
    L->AddEntry(NeNepointpt2, "NeNe 5.02 TeV");
    L->SetBorderSize(0);
    L->SetFillColor(0);

    gPbPb_pt2->SetTitle("");
    gPbPb_pt2->GetXaxis()->SetTitle("#LT N_{part} #GT");
    gPbPb_pt2->GetYaxis()->SetTitle("R_{AA}");
    gPbPb_pt2->GetYaxis()->SetRangeUser(0.0, 1.6);
    gPbPb_pt2->GetXaxis()->SetRangeUser(6, 500.0);

    // Centralize axis labels
    gPbPb_pt2->GetXaxis()->SetTitleOffset(1.2);
    gPbPb_pt2->GetXaxis()->CenterTitle(true);
    gPbPb_pt2->GetYaxis()->SetTitleOffset(1.2);
    gPbPb_pt2->GetYaxis()->CenterTitle(true);

    gPbPb_pt1->SetMarkerStyle(20);
    gPbPb_pt1->SetMarkerColor(cmsYellow);
    gPbPb_pt1->SetLineColor(cmsYellow);
    //gPbPb_pt1->Draw("AP");

    gPbPb_pt2->SetMarkerStyle(21);
    gPbPb_pt2->SetLineStyle(9);
    gPbPb_pt2->SetMarkerColor(cmsYellow);
    gPbPb_pt2->SetLineColor(cmsYellow);
    gPbPb_pt2->Draw("AP SAME");

    gPbPb_pt3->SetMarkerStyle(22);
    gPbPb_pt3->SetLineStyle(10);
    gPbPb_pt3->SetMarkerColor(cmsYellow);
    gPbPb_pt3->SetLineColor(cmsYellow);
    //gPbPb_pt3->Draw("AP SAME");

    gXeXe_pt1->SetMarkerStyle(20);
    gXeXe_pt1->SetMarkerColor(cmsRed);
    gXeXe_pt1->SetLineColor(cmsRed);
    //gXeXe_pt1->Draw("P SAME");

    gXeXe_pt2->SetMarkerStyle(21);
    gXeXe_pt2->SetLineStyle(9);
    gXeXe_pt2->SetMarkerColor(cmsRed);
    gXeXe_pt2->SetLineColor(cmsRed);
    gXeXe_pt2->Draw("P SAME");

    gXeXe_pt3->SetMarkerStyle(22);
    gXeXe_pt3->SetLineStyle(10);
    gXeXe_pt3->SetMarkerColor(cmsRed);
    gXeXe_pt3->SetLineColor(cmsRed);
    //gXeXe_pt3->Draw("P SAME");

    OOpointpt1->SetMarkerStyle(20);
    OOpointpt1->SetMarkerColor(cmsBlue);
    OOpointpt1->SetLineColor(cmsBlue);
    //OOpointpt1->Draw("P SAME");

    OOpointpt2->SetMarkerStyle(21);
    OOpointpt2->SetMarkerColor(cmsBlue);
    OOpointpt2->SetLineColor(cmsBlue);
    OOpointpt2->Draw("P SAME");

    OOpointpt3->SetMarkerStyle(22);
    OOpointpt3->SetMarkerColor(cmsBlue);
    OOpointpt3->SetLineColor(cmsBlue);
    //OOpointpt3->Draw("P SAME");

    NeNepointpt1->SetMarkerStyle(20);
    NeNepointpt1->SetMarkerColor(cmsViolet);
    NeNepointpt1->SetLineColor(cmsViolet);
    //NeNepointpt1->Draw("P SAME");

    NeNepointpt2->SetMarkerStyle(21);
    NeNepointpt2->SetMarkerColor(cmsViolet);
    NeNepointpt2->SetLineColor(cmsViolet);
    NeNepointpt2->Draw("P SAME");

    NeNepointpt3->SetMarkerStyle(22);
    NeNepointpt3->SetMarkerColor(cmsViolet);
    NeNepointpt3->SetLineColor(cmsViolet);
    //NeNepointpt3->Draw("P SAME");

    L->Draw();

    // Draw flat dotted grey line at y=1
    TLine* line = new TLine(6.0, 1.0, 400.0, 1.0);
    line->SetLineColor(kGray+2);
    line->SetLineStyle(3); // dotted
    line->SetLineWidth(2);
    line->Draw();

    
    TLatex* latex_eta = new TLatex(20, 1.5, "|#eta| < 1");
    latex_eta->SetTextSize(0.04); // smallish font
    latex_eta->SetTextFont(42);
    latex_eta->Draw();
    TLatex* latex_pt = new TLatex(20, 1.4, "9.6 < p_{T} < 12.0 GeV");
    latex_pt->SetTextSize(0.04); // smallish font
    latex_pt->SetTextFont(42);
    latex_pt->Draw();

    c->SaveAs("npart_plot_pt2.pdf");

}


