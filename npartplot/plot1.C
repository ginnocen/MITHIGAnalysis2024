#include "MITHIG_CMSStyle.h"
#include "scrape.C"
using namespace std;

void plot1(){

    vector<float> nparts_XeXe = {236.1,206.3,141.2,68.5,27.2,10.55};
    vector<float> nparts_XeXe_errorsup = {1.3,1.7,1.8,2.2,1.6,0.78};
    vector<float> nparts_XeXe_errorsdown = {1.3,1.7,1.8,2.2,1.6,0.78};

    vector<float> npartx_PbPb = {384.3,333.3,226.7,109.2,42.2,11.1};
    vector<float> npartx_PbPb_errorsup = {1.8,3.0,5.2,4.3,3.0,1.3};
    vector<float> npartx_PbPb_errorsdown = {2.0,3.2,5.3,4.2,2.9,1.2};


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

    TGraphAsymmErrors* pPbpointpt1 = new TGraphAsymmErrors(1);
    TGraphAsymmErrors* pPbpointpt2 = new TGraphAsymmErrors(1);
    TGraphAsymmErrors* pPbpointpt3 = new TGraphAsymmErrors(1);

    pPbpointpt1->SetPoint(0, 7.7230000, getpPbSystematic(17).y);
    pPbpointpt2->SetPoint(0, 7.7230000, getpPbSystematic(21).y);
    pPbpointpt3->SetPoint(0, 7.7230000, getpPbSystematic(26).y);
    pPbpointpt1->SetPointError(0, 0, 0, getpPbStatistical(17).eyl, getpPbStatistical(17).eyh);
    pPbpointpt2->SetPointError(0, 0, 0, getpPbStatistical(21).eyl, getpPbStatistical(21).eyh);
    pPbpointpt3->SetPointError(0, 0, 0, getpPbStatistical(26).eyl, getpPbStatistical(26).eyh);
    cout << "pPb: " << getpPbSystematic(17).y << " " << getpPbSystematic(21).y << " " << getpPbSystematic(26).y << endl;

    OOpointpt1->SetPoint(0, 10.8, getSmallSystemSystematic(0,1).y);
    OOpointpt2->SetPoint(0, 10.8, getSmallSystemSystematic(4,1).y);
    OOpointpt3->SetPoint(0, 10.8, getSmallSystemSystematic(9,1).y);
    OOpointpt1->SetPointError(0, 0, 0, getSmallSystemStatistical(0,1).eyl, getSmallSystemStatistical(0,1).eyh);
    OOpointpt2->SetPointError(0, 0, 0, getSmallSystemStatistical(4,1).eyl, getSmallSystemStatistical(4,1).eyh);
    OOpointpt3->SetPointError(0, 0, 0, getSmallSystemStatistical(9,1).eyl, getSmallSystemStatistical(9,1).eyh);

    NeNepointpt1->SetPoint(0, 12.7, getSmallSystemSystematic(0,2).y);
    NeNepointpt2->SetPoint(0, 12.7, getSmallSystemSystematic(4,2).y);
    NeNepointpt3->SetPoint(0, 12.7, getSmallSystemSystematic(9,2).y);
    NeNepointpt1->SetPointError(0, 0, 0, getSmallSystemStatistical(0,2).eyl, getSmallSystemStatistical(0,2).eyh);
    NeNepointpt2->SetPointError(0, 0, 0, getSmallSystemStatistical(4,2).eyl, getSmallSystemStatistical(4,2).eyh);
    NeNepointpt3->SetPointError(0, 0, 0, getSmallSystemStatistical(9,2).eyl, getSmallSystemStatistical(9,2).eyh);

    TBox* OObox_pt1 = new TBox(10.8-0.5, getSmallSystemSystematic(0,1).y - getSmallSystemSystematic(0,1).eyl, 10.8+0.5,  getSmallSystemSystematic(0,1).y + getSmallSystemSystematic(0,1).eyh);
    TBox* OObox_pt2 = new TBox(10.8-0.5, getSmallSystemSystematic(4,1).y - getSmallSystemSystematic(4,1).eyl, 10.8+0.5,  getSmallSystemSystematic(4,1).y + getSmallSystemSystematic(4,1).eyh);
    TBox* OObox_pt3 = new TBox(10.8-0.5, getSmallSystemSystematic(9,1).y - getSmallSystemSystematic(9,1).eyl, 10.8+0.5,  getSmallSystemSystematic(9,1).y + getSmallSystemSystematic(9,1).eyh);
    TBox* NeNebox_pt1 = new TBox(12.7-0.5, getSmallSystemSystematic(0,2).y - getSmallSystemSystematic(0,2).eyl, 12.7+0.5,  getSmallSystemSystematic(0,2).y + getSmallSystemSystematic(0,2).eyh);
    TBox* NeNebox_pt2 = new TBox(12.7-0.5, getSmallSystemSystematic(4,2).y - getSmallSystemSystematic(4,2).eyl, 12.7+0.5,  getSmallSystemSystematic(4,2).y + getSmallSystemSystematic(4,2).eyh);
    TBox* NeNebox_pt3 = new TBox(12.7-0.5, getSmallSystemSystematic(9,2).y - getSmallSystemSystematic(9,2).eyl, 12.7+0.5,  getSmallSystemSystematic(9,2).y + getSmallSystemSystematic(9,2).eyh);

    TBox* pPbbox_pt1 = new TBox(7.7230000-0.5, getpPbSystematic(17).y - getpPbSystematic(17).eyl, 7.7230000+0.5,  getpPbSystematic(17).y + getpPbSystematic(17).eyh);
    TBox* pPbbox_pt2 = new TBox(7.7230000-0.5, getpPbSystematic(21).y - getpPbSystematic(21).eyl, 7.7230000+0.5,  getpPbSystematic(21).y + getpPbSystematic(21).eyh);
    TBox* pPbbox_pt3 = new TBox(7.7230000-0.5, getpPbSystematic(26).y - getpPbSystematic(26).eyl, 7.7230000+0.5,  getpPbSystematic(26).y + getpPbSystematic(26).eyh);
    cout << " " << getpPbSystematic(17).y << " " << getpPbSystematic(21).y << " " << getpPbSystematic(26).y << endl;

    vector<TBox*> XeXeboxes_pt1(6);
    vector<TBox*> XeXeboxes_pt2(6);
    vector<TBox*> XeXeboxes_pt3(6);
    vector<TBox*> PbPbboxes_pt1(6);
    vector<TBox*> PbPbboxes_pt2(6);
    vector<TBox*> PbPbboxes_pt3(6);


    for(int i = 0; i<6; i++){

        gXeXe_pt1->SetPoint(i, nparts_XeXe[i], getXeXeSystematic(17,i).y);
        gXeXe_pt1->SetPointError(i, 0, 0, getXeXeStatistical(17,i).eyl, getXeXeStatistical(17,i).eyh);
        XeXeboxes_pt1[i] = new TBox(nparts_XeXe[i]-nparts_XeXe_errorsdown[i], getXeXeSystematic(17,i).y - getXeXeSystematic(17,i).eyl, nparts_XeXe[i]+nparts_XeXe_errorsup[i], getXeXeSystematic(17,i).y + getXeXeSystematic(17,i).eyh);

        gPbPb_pt1->SetPoint(i, npartx_PbPb[i], getPbPbSystematic(15,i).y);
        gPbPb_pt1->SetPointError(i, 0, 0, getPbPbStatistical(15,i).eyl, getPbPbStatistical(15,i).eyh);
        PbPbboxes_pt1[i] = new TBox(npartx_PbPb[i]-npartx_PbPb_errorsdown[i], getPbPbSystematic(15,i).y - getPbPbSystematic(15,i).eyl, npartx_PbPb[i]+npartx_PbPb_errorsup[i], getPbPbSystematic(15,i).y + getPbPbSystematic(15,i).eyh);

        gXeXe_pt2->SetPoint(i, nparts_XeXe[i], getXeXeSystematic(21,i).y);
        gXeXe_pt2->SetPointError(i, 0 , 0, getXeXeStatistical(21,i).eyl, getXeXeStatistical(21,i).eyh);
        XeXeboxes_pt2[i] = new TBox(nparts_XeXe[i]-nparts_XeXe_errorsdown[i], getXeXeSystematic(21,i).y - getXeXeSystematic(21,i).eyl, nparts_XeXe[i]+nparts_XeXe_errorsup[i], getXeXeSystematic(21,i).y + getXeXeSystematic(21,i).eyh);

        gPbPb_pt2->SetPoint(i, npartx_PbPb[i], getPbPbSystematic(19,i).y);
        gPbPb_pt2->SetPointError(i, 0, 0, getPbPbStatistical(19,i).eyl, getPbPbStatistical(19,i).eyh);
        PbPbboxes_pt2[i] = new TBox(npartx_PbPb[i]-npartx_PbPb_errorsdown[i], getPbPbSystematic(19,i).y - getPbPbSystematic(19,i).eyl, npartx_PbPb[i]+npartx_PbPb_errorsup[i], getPbPbSystematic(19,i).y + getPbPbSystematic(19,i).eyh);

        gXeXe_pt3->SetPoint(i, nparts_XeXe[i], getXeXeSystematic(26,i).y);
        gXeXe_pt3->SetPointError(i, 0, 0, getXeXeStatistical(26,i).eyl, getXeXeStatistical(26,i).eyh);
        XeXeboxes_pt3[i] = new TBox(nparts_XeXe[i]-nparts_XeXe_errorsdown[i], getXeXeSystematic(26,i).y - getXeXeSystematic(26,i).eyl, nparts_XeXe[i]+nparts_XeXe_errorsup[i], getXeXeSystematic(26,i).y + getXeXeSystematic(26,i).eyh);

        gPbPb_pt3->SetPoint(i, npartx_PbPb[i], getPbPbSystematic(24,i).y);
        gPbPb_pt3->SetPointError(i, 0, 0, getPbPbStatistical(24,i).eyl, getPbPbStatistical(24,i).eyh);
        PbPbboxes_pt3[i] = new TBox(npartx_PbPb[i]-npartx_PbPb_errorsdown[i], getPbPbSystematic(24,i).y - getPbPbSystematic(24,i).eyl, npartx_PbPb[i]+npartx_PbPb_errorsup[i], getPbPbSystematic(24,i).y + getPbPbSystematic(24,i).eyh);

    }

    TH1D*hPbPb = new TH1D("h", "Histogram", 100, 0, 200);
    hPbPb->SetFillColorAlpha(cmsYellow, 0.19);
    hPbPb->SetMarkerStyle(33);
    hPbPb->SetMarkerColor(cmsYellow);
    hPbPb->SetFillStyle(1001);
    hPbPb->SetLineWidth(0);
    hPbPb->SetMarkerSize(1.3);

    TH1D*hXeXe = new TH1D("h", "Histogram", 100, 0, 200);
    hXeXe->SetFillColorAlpha(cmsRed, 0.19);
    hXeXe->SetMarkerStyle(33);
    hXeXe->SetMarkerColor(cmsRed);
    hXeXe->SetFillStyle(1001);
    hXeXe->SetLineWidth(0);
    hXeXe->SetMarkerSize(1.3);

    TH1D* hOO = new TH1D("h", "Histogram", 100, 0, 200);
    hOO->SetFillColorAlpha(cmsBlue, 0.19);
    hOO->SetMarkerStyle(33);
    hOO->SetMarkerColor(cmsBlue);
    hOO->SetFillStyle(1001);
    hOO->SetLineWidth(0);
    hOO->SetMarkerSize(1.3);

    TH1D* hNeNe = new TH1D("h", "Histogram", 100, 0, 200);
    hNeNe->SetFillColorAlpha(cmsViolet, 0.19);
    hNeNe->SetMarkerStyle(33);
    hNeNe->SetMarkerColor(cmsViolet);
    hNeNe->SetFillStyle(1001);
    hNeNe->SetLineWidth(0);
    hNeNe->SetMarkerSize(1.3);

    TH1D* hpPb = new TH1D("h", "Histogram", 100, 0, 200);
    hpPb->SetFillColorAlpha(cmsTeal, 0.19);
    hpPb->SetMarkerStyle(33);
    hpPb->SetMarkerColor(cmsTeal);
    hpPb->SetFillStyle(1001);
    hpPb->SetLineWidth(0);
    hpPb->SetMarkerSize(1.3);

    TLegend* L = new TLegend(0.41,0.64,0.86,0.88);
    L->SetTextSize(0.035);
    /*L->AddEntry(gPbPb_pt2, "PbPb (5.02 TeV) 404 #mub^{-1}");
    L->AddEntry(gXeXe_pt2, "XeXe (5.44 TeV) 3.42 #mub^{-1}");
    L->AddEntry(OOpointpt2, "OO (5.02 TeV) 6.0 nb^{-1}");
    L->AddEntry(NeNepointpt2, "NeNe (5.02 TeV) 0.8 nb^{-1}");
    L->AddEntry(pPbpointpt2, "pPb (5.02 TeV) 35 nb^{-1}");*/
    L->AddEntry(hPbPb, "PbPb (5.02 TeV)");
    L->AddEntry(hXeXe, "XeXe (5.44 TeV)");
    L->AddEntry(hNeNe, "NeNe (5.36 TeV) Min-Bias");
    L->AddEntry(hOO, "OO (5.36 TeV) Min-Bias");
    L->AddEntry(hpPb, "pPb (5.02 TeV) Min-Bias");
    L->SetBorderSize(0);
    L->SetFillColor(0);

    gPbPb_pt1->SetTitle("");
    gPbPb_pt1->GetXaxis()->SetTitle("#LT N_{part} #GT");
    gPbPb_pt1->GetYaxis()->SetTitle("Charged particle R_{AA}");
    gPbPb_pt1->GetYaxis()->SetRangeUser(0.0, 1.6);
    gPbPb_pt1->GetXaxis()->SetRangeUser(6, 500.0);
    gPbPb_pt1->GetXaxis()->SetTitleSize(0.050);
    gPbPb_pt1->GetYaxis()->SetTitleSize(0.050);
    gPbPb_pt1->GetXaxis()->SetTitleOffset(1.2);
    gPbPb_pt1->GetXaxis()->CenterTitle(true);
    gPbPb_pt1->GetYaxis()->SetTitleOffset(1.2);
    gPbPb_pt1->GetYaxis()->CenterTitle(true);
    gPbPb_pt1->SetMarkerStyle(33);
    gPbPb_pt1->SetMarkerSize(1.3);
    gPbPb_pt1->SetMarkerColor(cmsYellow);
    gPbPb_pt1->SetLineColor(cmsYellow);
    gPbPb_pt1->SetMinimum(0.0);
    gPbPb_pt1->SetMaximum(1.6);
    //gPbPb_pt1->Draw("AP");


    gPbPb_pt2->SetTitle("");
    gPbPb_pt2->GetXaxis()->SetTitle("#LT N_{part} #GT");
    gPbPb_pt2->GetYaxis()->SetTitle("Charged particle R_{AA}");
    gPbPb_pt2->GetYaxis()->SetRangeUser(0.0, 1.6);
    gPbPb_pt2->GetXaxis()->SetRangeUser(6, 500.0);
    gPbPb_pt2->GetXaxis()->SetTitleSize(0.050);
    gPbPb_pt2->GetYaxis()->SetTitleSize(0.050);
    gPbPb_pt2->GetXaxis()->SetTitleOffset(1.2);
    gPbPb_pt2->GetXaxis()->CenterTitle(true);
    gPbPb_pt2->GetYaxis()->SetTitleOffset(1.2);
    gPbPb_pt2->GetYaxis()->CenterTitle(true);
    gPbPb_pt2->SetMarkerStyle(33);
    gPbPb_pt2->SetMarkerSize(1.3);
    gPbPb_pt2->SetMarkerColor(cmsYellow);
    gPbPb_pt2->SetLineColor(cmsYellow);
    gPbPb_pt2->SetMinimum(0.0);
    gPbPb_pt2->SetMaximum(1.6);


    gPbPb_pt3->SetTitle("");
    gPbPb_pt3->GetXaxis()->SetTitle("#LT N_{part} #GT");
    gPbPb_pt3->GetYaxis()->SetTitle("Charged particle R_{AA}");
    gPbPb_pt3->GetYaxis()->SetRangeUser(0.0, 1.6);
    gPbPb_pt3->GetXaxis()->SetRangeUser(6, 500.0);
    gPbPb_pt3->GetXaxis()->SetTitleSize(0.050);
    gPbPb_pt3->GetYaxis()->SetTitleSize(0.050);
    gPbPb_pt3->GetXaxis()->SetTitleOffset(1.2);
    gPbPb_pt3->GetXaxis()->CenterTitle(true);
    gPbPb_pt3->GetYaxis()->SetTitleOffset(1.2);
    gPbPb_pt3->GetYaxis()->CenterTitle(true);
    gPbPb_pt3->SetMarkerStyle(33);
    gPbPb_pt3->SetMarkerSize(1.3);
    gPbPb_pt3->SetMarkerColor(cmsYellow);
    gPbPb_pt3->SetLineColor(cmsYellow);
    gPbPb_pt3->SetMinimum(0.0);
    gPbPb_pt3->SetMaximum(1.6);


    gXeXe_pt1->SetMarkerStyle(33);
    gXeXe_pt1->SetMarkerSize(1.3);
    gXeXe_pt1->SetMarkerColor(cmsRed);
    gXeXe_pt1->SetLineColor(cmsRed);
    //gXeXe_pt1->Draw("P SAME");

    gXeXe_pt2->SetMarkerStyle(33);
    gXeXe_pt2->SetMarkerSize(1.3);
    //gXeXe_pt2->SetLineStyle(9);
    gXeXe_pt2->SetMarkerColor(cmsRed);
    gXeXe_pt2->SetLineColor(cmsRed);


    gXeXe_pt3->SetMarkerStyle(33);
    gXeXe_pt3->SetMarkerSize(1.3);
    //gXeXe_pt3->SetLineStyle(10);
    gXeXe_pt3->SetMarkerColor(cmsRed);
    gXeXe_pt3->SetLineColor(cmsRed);
    //gXeXe_pt3->Draw("P SAME");

    OOpointpt1->SetMarkerStyle(33);
    OOpointpt1->SetMarkerSize(1.3);
    OOpointpt1->SetMarkerColor(cmsBlue);
    OOpointpt1->SetLineColor(cmsBlue);
    //OOpointpt1->Draw("P SAME");

    OOpointpt2->SetMarkerStyle(33);
    OOpointpt2->SetMarkerSize(1.3);
    OOpointpt2->SetMarkerColor(cmsBlue);
    OOpointpt2->SetLineColor(cmsBlue);
    OOpointpt2->Draw("P SAME");

    OOpointpt3->SetMarkerStyle(33);
    OOpointpt3->SetMarkerSize(1.3);
    OOpointpt3->SetMarkerColor(cmsBlue);
    OOpointpt3->SetLineColor(cmsBlue);
    //OOpointpt3->Draw("P SAME");

    NeNepointpt1->SetMarkerStyle(33);
    NeNepointpt1->SetMarkerSize(1.3);
    NeNepointpt1->SetMarkerColor(cmsViolet);
    NeNepointpt1->SetLineColor(cmsViolet);
    //NeNepointpt1->Draw("P SAME");

    NeNepointpt2->SetMarkerStyle(33);
    NeNepointpt2->SetMarkerSize(1.3);
    NeNepointpt2->SetMarkerColor(cmsViolet);
    NeNepointpt2->SetLineColor(cmsViolet);
    //NeNepointpt2->Draw("P SAME");

    NeNepointpt3->SetMarkerStyle(33);
    NeNepointpt3->SetMarkerSize(1.3);
    NeNepointpt3->SetMarkerColor(cmsViolet);
    NeNepointpt3->SetLineColor(cmsViolet);
    //NeNepointpt3->Draw("P SAME");

    pPbpointpt1->SetMarkerStyle(33);
    pPbpointpt1->SetMarkerSize(1.3);
    pPbpointpt1->SetMarkerColor(cmsTeal);
    pPbpointpt1->SetLineColor(cmsTeal);

    pPbpointpt2->SetMarkerStyle(33);
    pPbpointpt2->SetMarkerSize(1.3);
    pPbpointpt2->SetMarkerColor(cmsTeal);
    pPbpointpt2->SetLineColor(cmsTeal);

    pPbpointpt3->SetMarkerStyle(33);
    pPbpointpt3->SetMarkerSize(1.3);
    pPbpointpt3->SetMarkerColor(cmsTeal);
    pPbpointpt3->SetLineColor(cmsTeal);
    //pPbpointpt1->Draw("P SAME");

    for(int i = 0; i<6; i++){
        PbPbboxes_pt1[i]->SetFillColorAlpha(cmsYellow, 0.19);
        PbPbboxes_pt1[i]->SetLineColor(cmsYellow+2);
        PbPbboxes_pt1[i]->SetLineWidth(1);
        PbPbboxes_pt1[i]->SetLineStyle(1);

        PbPbboxes_pt2[i]->SetFillColorAlpha(cmsYellow, 0.19);
        PbPbboxes_pt2[i]->SetLineColor(cmsYellow+2);
        PbPbboxes_pt2[i]->SetLineWidth(1);
        PbPbboxes_pt2[i]->SetLineStyle(1);
        
        PbPbboxes_pt3[i]->SetFillColorAlpha(cmsYellow, 0.19);
        PbPbboxes_pt3[i]->SetLineColor(cmsYellow+2);
        PbPbboxes_pt3[i]->SetLineWidth(1);
        PbPbboxes_pt3[i]->SetLineStyle(1);

        XeXeboxes_pt1[i]->SetFillColorAlpha(cmsRed, 0.19);
        XeXeboxes_pt1[i]->SetLineColor(cmsRed+2);
        XeXeboxes_pt1[i]->SetLineWidth(1);
        XeXeboxes_pt1[i]->SetLineStyle(1);

        XeXeboxes_pt2[i]->SetFillColorAlpha(cmsRed, 0.19);
        XeXeboxes_pt2[i]->SetLineColor(cmsRed+2);
        XeXeboxes_pt2[i]->SetLineWidth(1);
        XeXeboxes_pt2[i]->SetLineStyle(1);

        XeXeboxes_pt3[i]->SetFillColorAlpha(cmsRed, 0.19);
        XeXeboxes_pt3[i]->SetLineColor(cmsRed+2);
        XeXeboxes_pt3[i]->SetLineWidth(1);
        XeXeboxes_pt3[i]->SetLineStyle(1);

    }

    NeNebox_pt1->SetFillColorAlpha(cmsViolet, 0.19);
    NeNebox_pt1->SetLineColor(cmsViolet+2);
    NeNebox_pt1->SetLineWidth(1);
    NeNebox_pt1->SetLineStyle(1);

    NeNebox_pt2->SetFillColorAlpha(cmsViolet, 0.19);
    NeNebox_pt2->SetLineColor(cmsViolet+2);
    NeNebox_pt2->SetLineWidth(1);
    NeNebox_pt2->SetLineStyle(1);

    NeNebox_pt3->SetFillColorAlpha(cmsViolet, 0.19);
    NeNebox_pt3->SetLineColor(cmsViolet+2);
    NeNebox_pt3->SetLineWidth(1);
    NeNebox_pt3->SetLineStyle(1);

    OObox_pt1->SetFillColorAlpha(cmsBlue, 0.19);
    OObox_pt1->SetLineColor(cmsBlue+2);
    OObox_pt1->SetLineWidth(1);
    OObox_pt1->SetLineStyle(1);

    OObox_pt2->SetFillColorAlpha(cmsBlue, 0.19);
    OObox_pt2->SetLineColor(cmsBlue+2);
    OObox_pt2->SetLineWidth(1);
    OObox_pt2->SetLineStyle(1);

    OObox_pt3->SetFillColorAlpha(cmsBlue, 0.19);
    OObox_pt3->SetLineColor(cmsBlue+2);
    OObox_pt3->SetLineWidth(1);
    OObox_pt3->SetLineStyle(1);

    pPbbox_pt1->SetFillColorAlpha(cmsTeal, 0.19);
    pPbbox_pt1->SetLineColor(cmsTeal+2);
    pPbbox_pt1->SetLineWidth(1);
    pPbbox_pt1->SetLineStyle(1);

    pPbbox_pt2->SetFillColorAlpha(cmsTeal, 0.19);
    pPbbox_pt2->SetLineColor(cmsTeal+2);
    pPbbox_pt2->SetLineWidth(1);
    pPbbox_pt2->SetLineStyle(1);

    pPbbox_pt3->SetFillColorAlpha(cmsTeal, 0.19);
    pPbbox_pt3->SetLineColor(cmsTeal+2);
    pPbbox_pt3->SetLineWidth(1);
    pPbbox_pt3->SetLineStyle(1);

    // Set axis ranges for all pT classes to match pT2
    gPbPb_pt1->GetXaxis()->SetRangeUser(6, 500.0);
    gPbPb_pt1->GetYaxis()->SetRangeUser(0.0, 1.6);

    gPbPb_pt3->GetXaxis()->SetRangeUser(6, 500.0);
    gPbPb_pt3->GetYaxis()->SetRangeUser(0.0, 1.6);

    gXeXe_pt1->GetXaxis()->SetRangeUser(6, 500.0);
    gXeXe_pt1->GetYaxis()->SetRangeUser(0.0, 1.6);

    gXeXe_pt3->GetXaxis()->SetRangeUser(6, 500.0);
    gXeXe_pt3->GetYaxis()->SetRangeUser(0.0, 1.6);

    OOpointpt1->GetXaxis()->SetRangeUser(6, 500.0);
    OOpointpt1->GetYaxis()->SetRangeUser(0.0, 1.6);

    OOpointpt3->GetXaxis()->SetRangeUser(6, 500.0);
    OOpointpt3->GetYaxis()->SetRangeUser(0.0, 1.6);

    NeNepointpt1->GetXaxis()->SetRangeUser(6, 500.0);
    NeNepointpt1->GetYaxis()->SetRangeUser(0.0, 1.6);

    NeNepointpt3->GetXaxis()->SetRangeUser(6, 500.0);
    NeNepointpt3->GetYaxis()->SetRangeUser(0.0, 1.6);


    // Update legend for all pT ranges
    //L->AddEntry(gPbPb_pt1, "PbPb 5.02 TeV");
    //L->AddEntry(gXeXe_pt1, "XeXe 5.44 TeV");
    //L->AddEntry(OOpointpt1, "OO 5.02 TeV");
    //L->AddEntry(NeNepointpt1, "NeNe 5.02 TeV");

    TCanvas* c = new TCanvas("c", "c", 800, 800);
    c->cd();
    c->SetLogx();
    c->SetLeftMargin(0.15);
    c->SetBottomMargin(0.15);


    L->Draw();

    // Draw flat dotted grey line at y=1
    TLine* line = new TLine(6.0, 1.0, 400.0, 1.0);
    line->SetLineColor(kGray+2);
    line->SetLineStyle(3); // dotted
    line->SetLineWidth(2);
    line->Draw();

    
    TLatex* latex_eta = new TLatex(60, 0.86, "|#eta| < 1");
    latex_eta->SetTextSize(0.032); // smallish font
    latex_eta->SetTextFont(42);
    latex_eta->Draw();
    TLatex* latex_pt1 = new TLatex(60, 0.93, "4.8 < p_{T} < 5.6 GeV");
    latex_pt1->SetTextSize(0.032); // smallish font
    latex_pt1->SetTextFont(42);
    latex_pt1->Draw();
    TLatex* latex_pt2 = new TLatex(60, 0.93, "9.6 < p_{T} < 12.0 GeV");
    latex_pt2->SetTextSize(0.032); // smallish font
    latex_pt2->SetTextFont(42);
    latex_pt2->Draw();
    TLatex* latex_pt3 = new TLatex(60, 0.93, "28.8 < p_{T} < 35.2 GeV");
    latex_pt3->SetTextSize(0.032); // smallish font
    latex_pt3->SetTextFont(42);
    latex_pt3->Draw();

    // Save separate PDFs for each pT class
    c->Clear();
    // Debugging: Print TBox coordinates to verify visibility
    for(int i = 0; i<6; i++){
        cout << "PbPbboxes_pt1[" << i << "]: x1=" << PbPbboxes_pt1[i]->GetX1() << ", y1=" << PbPbboxes_pt1[i]->GetY1()
             << ", x2=" << PbPbboxes_pt1[i]->GetX2() << ", y2=" << PbPbboxes_pt1[i]->GetY2() << endl;
        cout << "XeXeboxes_pt1[" << i << "]: x1=" << XeXeboxes_pt1[i]->GetX1() << ", y1=" << XeXeboxes_pt1[i]->GetY1()
             << ", x2=" << XeXeboxes_pt1[i]->GetX2() << ", y2=" << XeXeboxes_pt1[i]->GetY2() << endl;
    }

    // Ensure TBoxes are drawn first with proper settings
    c->Clear();
    
    // Draw other elements after TBoxes
    gPbPb_pt1->GetXaxis()->SetRangeUser(6, 500.0);
    gPbPb_pt1->Draw("AP SAME");
    gXeXe_pt1->Draw("P SAME");
    OOpointpt1->Draw("P SAME");
    NeNepointpt1->Draw("P SAME");
    pPbpointpt1->Draw("P SAME");
    latex_pt1->Draw();
    latex_eta->Draw();
    L->Draw();
    for(int i = 0; i<6; i++){
        PbPbboxes_pt1[i]->SetFillStyle(1001); // Solid fill
        PbPbboxes_pt1[i]->Draw("SAME");

        XeXeboxes_pt1[i]->SetFillStyle(1001); // Solid fill
        XeXeboxes_pt1[i]->Draw("SAME");
    }
    OObox_pt1->Draw("SAME");
    NeNebox_pt1->Draw("SAME");
    pPbbox_pt1->Draw("SAME");
    line->Draw();
    AddCMSHeader(c, "Preliminary", true);
    c->Update(); // Ensure canvas updates
    c->SaveAs("npart_plot_pt1.pdf");

    c->Clear();

    gPbPb_pt2->GetXaxis()->SetRangeUser(6, 500.0);
    gPbPb_pt2->Draw("AP");
    gXeXe_pt2->Draw("P SAME");
    OOpointpt2->Draw("P SAME");
    NeNepointpt2->Draw("P SAME");
    pPbpointpt2->Draw("P SAME");
    latex_pt2->Draw();
    latex_eta->Draw();
    L->Draw();
    for(int i = 0; i<6; i++){
        PbPbboxes_pt2[i]->SetFillStyle(1001); // Solid fill
        PbPbboxes_pt2[i]->Draw("SAME");
        XeXeboxes_pt2[i]->SetFillStyle(1001); // Solid fill
        XeXeboxes_pt2[i]->Draw("SAME");
    }
    OObox_pt2->Draw("SAME");
    NeNebox_pt2->Draw("SAME");
    pPbbox_pt2->Draw("SAME");
    line->Draw();
    AddCMSHeader(c, "Preliminary", true);
    c->Update(); // Ensure canvas updates
    c->SaveAs("npart_plot_pt2.pdf");

    c->Clear();
    gPbPb_pt3->GetXaxis()->SetRangeUser(6, 500.0);
    gPbPb_pt3->Draw("AP");
    gXeXe_pt3->Draw("P SAME");
    OOpointpt3->Draw("P SAME");
    NeNepointpt3->Draw("P SAME");
    pPbpointpt3->Draw("P SAME");
    latex_pt3->Draw();
    latex_eta->Draw();
    L->Draw();
    for(int i = 0; i<6; i++){
        PbPbboxes_pt3[i]->SetFillStyle(1001); // Solid fill
        PbPbboxes_pt3[i]->Draw("SAME");
        XeXeboxes_pt3[i]->SetFillStyle(1001); // Solid fill
        XeXeboxes_pt3[i]->Draw("SAME");
    }
    OObox_pt3->Draw("SAME");
    NeNebox_pt3->Draw("SAME");
    pPbbox_pt3->Draw("SAME");
    line->Draw();
    AddCMSHeader(c, "Preliminary", true);
    c->Update(); // Ensure canvas updates
    c->SaveAs("npart_plot_pt3.pdf");

    

}

