#include "MITHIG_CMSStyle.h"
#include "scrape.C"
using namespace std;


void plot2(){

    TFile*f = TFile::Open("data/XeXeDifferential.root");
    TDirectoryFile* dir = (TDirectoryFile*)f->Get("Table 3");
    TH1D*bins = (TH1D*)dir->Get("Hist1D_y1");

    TFile*f2 = TFile::Open("data/c70_90.root");
    TDirectoryFile* dir2 = (TDirectoryFile*)f2->Get("Table 13");
    TH1D*bins2 = (TH1D*)dir2->Get("Hist1D_y1");

    vector<float> smallsystembinedges = {4.8,5.6,6.4,7.2,9.6,12,14.4,19.2,24.0,28.8,35.2,48.0,73.6,103.6};

    TGraphAsymmErrors* gOO = new TGraphAsymmErrors();
    vector<TBox*> boxesOO;
    for(int i = 0; i< 13; i++){
        GraphEntry entry = getSmallSystemSystematic_only(i, 1);
        GraphEntry entry2 = getSmallSystemStatistical(i, 1);
        // Use template histogram for bin width
        double x = (smallsystembinedges[i]+smallsystembinedges[i+1])/2.0;
        cout << x << endl;
        double y = entry.y;
        gOO->SetPoint(i, x, y);
        gOO->SetPointError(i, 0, 0, entry2.eyl, entry2.eyh);

        int binIdx = bins->FindBin(x);
        double ex = bins->GetBinWidth(binIdx) / 2.0;
        TBox* box = new TBox(x-ex, y-entry.eyl, x+ex, y+entry.eyh);
        box->SetFillColorAlpha(cmsBlue,0.19);
        box->SetLineColor(cmsBlue+2);
        box->SetLineWidth(1);
        box->SetLineStyle(1); // solid line
        boxesOO.push_back(box);
    }

    TGraphAsymmErrors* gNeNe = new TGraphAsymmErrors();
    vector<TBox*> boxesNeNe;
    for(int i = 0; i< 13; i++){
        GraphEntry entry = getSmallSystemSystematic_only(i, 2);
        GraphEntry entry2 = getSmallSystemStatistical(i, 2);
        // Use template histogram for bin width
        double x = (smallsystembinedges[i]+smallsystembinedges[i+1])/2.0;
        cout << x << endl;
        double y = entry.y;
        gNeNe->SetPoint(i, x, y);
        gNeNe->SetPointError(i, 0, 0, entry2.eyl, entry2.eyh);

        int binIdx = bins->FindBin(x);
        double ex = bins->GetBinWidth(binIdx) / 2.0;
        TBox* box = new TBox(x-ex, y-entry.eyl, x+ex, y+entry.eyh);
        box->SetFillColorAlpha(cmsViolet,0.19);
        box->SetLineColor(cmsViolet+2);
        box->SetLineWidth(1);
        box->SetLineStyle(1);
        boxesNeNe.push_back(box);
    }

    TGraphAsymmErrors* gXeXe = new TGraphAsymmErrors();
    vector<TBox*> boxesXeXe;
    for(int i = 0; i< 29; i++){
        GraphEntry entry = getXeXeSystematic_only(i, 5);
        GraphEntry entry2 = getXeXeStatistical(i, 5);
        if(entry.y < 0.05){continue;}
        gXeXe->SetPoint(i, entry.x, entry.y);
        gXeXe->SetPointError(i, 0, 0, entry2.eyl, entry2.eyh);
        double x = entry.x;
        if (x < 0.4) continue;
        double y = entry.y;
        int binIdx = bins->FindBin(x);
        double ex = bins->GetBinWidth(binIdx) / 2.0;

        TBox* box = new TBox(x-ex, y-entry.eyl, x+ex, y+entry.eyh);
        box->SetFillColorAlpha(cmsRed,0.19);
        box->SetLineColor(cmsRed+2);
        box->SetLineWidth(1);
        box->SetLineStyle(1);
        boxesXeXe.push_back(box);
    }

    TGraphAsymmErrors* gPbPb = new TGraphAsymmErrors();
    vector<TBox*> boxesPbPb;
    for(int i = 0; i< 29; i++){
        GraphEntry entry = getPbPbSystematic_only(i, 5);
        GraphEntry entry2 = getPbPbStatistical(i, 5);
        gPbPb->SetPoint(i, entry.x, entry.y);
        gPbPb->SetPointError(i, 0, 0, entry2.eyl, entry2.eyh);
        double x = entry.x;
        double y = entry.y;
        int binIdx = bins2->FindBin(x);
        double ex = bins2->GetBinWidth(binIdx) / 2.0;
        TBox* box = new TBox(x-ex, y-entry.eyl, x+ex, y+entry.eyh);
        box->SetFillColorAlpha(cmsYellow,0.19);
        box->SetLineColor(cmsYellow+2);
        box->SetLineWidth(1);
        box->SetLineStyle(1);
        boxesPbPb.push_back(box);
    }

    // Example: Draw boxes for one graph (repeat for others as needed)
    // for (auto box : boxesOO) box->Draw();
    // for (auto box : boxesNeNe) box->Draw();
    // for (auto box : boxesXeXe) box->Draw();
    // for (auto box : boxesPbPb) box->Draw();

    // Set axis labels and axis span globally
    const char* xLabel = "Track p_{T}";
    const char* yLabel = "R_{AA}";
    double xMin = 0.4, xMax = 200.0;
    double yMin = 0.0, yMax = 1.6;

    gOO->GetXaxis()->SetTitle(xLabel);
    gOO->GetYaxis()->SetTitle(yLabel);
    gOO->GetXaxis()->SetLimits(xMin, xMax);
    //gOO->GetYaxis()->SetRangeUser(yMin, yMax);
    gOO->SetMinimum(0.0);
    gOO->SetMaximum(1.6);
    gOO->GetXaxis()->CenterTitle(true);
    gOO->GetYaxis()->CenterTitle(true);
    gOO->GetXaxis()->SetTitleOffset(1.2);
    gOO->GetYaxis()->SetTitleOffset(1.2);
    gOO->GetXaxis()->SetTitleSize(0.052);
    gOO->GetYaxis()->SetTitleSize(0.052);

    gNeNe->GetXaxis()->SetTitle(xLabel);
    gNeNe->GetYaxis()->SetTitle(yLabel);
    gNeNe->GetXaxis()->SetRangeUser(xMin, xMax);
    gNeNe->GetYaxis()->SetRangeUser(yMin, yMax);
    gNeNe->GetXaxis()->CenterTitle(true);
    gNeNe->GetYaxis()->CenterTitle(true);
    gNeNe->GetXaxis()->SetTitleOffset(1.2);
    gNeNe->GetYaxis()->SetTitleOffset(1.2);
    gNeNe->GetXaxis()->SetTitleSize(0.052);
    gNeNe->GetYaxis()->SetTitleSize(0.052);

    gXeXe->GetXaxis()->SetTitle(xLabel);
    gXeXe->GetYaxis()->SetTitle(yLabel);
    gXeXe->GetXaxis()->SetRangeUser(xMin, xMax);
    gXeXe->GetYaxis()->SetRangeUser(yMin, yMax);
    gXeXe->GetXaxis()->CenterTitle(true);
    gXeXe->GetYaxis()->CenterTitle(true);
    gXeXe->GetXaxis()->SetTitleOffset(1.2);
    gXeXe->GetYaxis()->SetTitleOffset(1.2);
    gXeXe->GetXaxis()->SetTitleSize(0.052);
    gXeXe->GetYaxis()->SetTitleSize(0.052);

    gPbPb->GetXaxis()->SetTitle(xLabel);
    gPbPb->GetYaxis()->SetTitle(yLabel);
    gPbPb->GetXaxis()->SetRangeUser(xMin, xMax);
    gPbPb->GetYaxis()->SetRangeUser(yMin, yMax);
    gPbPb->GetXaxis()->CenterTitle(true);
    gPbPb->GetYaxis()->CenterTitle(true);
    gPbPb->GetXaxis()->SetTitleOffset(1.2);
    gPbPb->GetYaxis()->SetTitleOffset(1.2);
    gPbPb->GetXaxis()->SetTitleSize(0.052);
    gPbPb->GetYaxis()->SetTitleSize(0.052);

    TCanvas*c = new TCanvas("","", 900, 900);
    c->cd();
    c->SetLogx();
    c->SetLeftMargin(0.15);
    c->SetBottomMargin(0.15);
    c->SetTopMargin(0.08);
    c->SetRightMargin(0.05);

    // Draw the graphs and boxes
    
    gOO->Draw("AP");
    gOO->SetMarkerColor(cmsBlue);
    gOO->SetLineColor(cmsBlue);
    gOO->SetMarkerStyle(33);
    gOO->SetMarkerSize(1.5);
    for (auto box : boxesOO) box->Draw();

    gNeNe->Draw("P SAME");
    gNeNe->SetMarkerColor(cmsViolet);
    gNeNe->SetLineColor(cmsViolet);
    gNeNe->SetMarkerStyle(33);
    gNeNe->SetMarkerSize(1.5);
    for (auto box : boxesNeNe) box->Draw();

    gXeXe->Draw("P SAME");
    gXeXe->SetMarkerColor(cmsRed);
    gXeXe->SetLineColor(cmsRed);
    gXeXe->SetMarkerStyle(33);
    gXeXe->SetMarkerSize(1.5);
    for (auto box : boxesXeXe) box->Draw();

    gPbPb->Draw("P SAME");
    gPbPb->SetMarkerColor(cmsYellow);
    gPbPb->SetLineColor(cmsYellow);
    gPbPb->SetMarkerStyle(33);
    gPbPb->SetMarkerSize(1.5);
    for (auto box : boxesPbPb) box->Draw();

    // Draw horizontal dotted line at y=1
    TLine* line = new TLine(0.4, 1.0, 200.0, 1.0);
    line->SetLineColor(kGray+2);
    line->SetLineStyle(3); // dotted
    line->SetLineWidth(2);
    line->Draw();

    TH1D*hOO = new TH1D("h", "Histogram", 100, 0, 200);
    hOO->SetFillColorAlpha(cmsBlue, 0.19);
    hOO->SetMarkerStyle(33);
    hOO->SetMarkerColor(cmsBlue);
    hOO->SetMarkerSize(1.5);
    hOO->SetFillStyle(1001);
    hOO->SetLineWidth(0);
    hOO->Draw("SAME");

    TH1D*hNeNe = new TH1D("h", "Histogram", 100, 0, 200);
    hNeNe->SetFillColorAlpha(cmsViolet, 0.19);
    hNeNe->SetMarkerStyle(33);
    hNeNe->SetMarkerColor(cmsViolet);
    hNeNe->SetMarkerSize(1.5);
    hNeNe->SetFillStyle(1001);
    hNeNe->SetLineWidth(0);
    hNeNe->Draw("SAME");

    TH1D*hXeXe = new TH1D("h", "Histogram", 100, 0, 200);
    hXeXe->SetFillColorAlpha(cmsRed, 0.19);
    hXeXe->SetMarkerStyle(33);
    hXeXe->SetMarkerColor(cmsRed);
    hXeXe->SetMarkerSize(1.5);
    hXeXe->SetFillStyle(1001);
    hXeXe->SetLineWidth(0);
    hXeXe->Draw("SAME");

    TH1D*hPbPb = new TH1D("h", "Histogram", 100, 0, 200);
    hPbPb->SetFillColorAlpha(cmsYellow, 0.19);
    hPbPb->SetMarkerStyle(33);
    hPbPb->SetMarkerColor(cmsYellow);
    hPbPb->SetMarkerSize(1.5);
    hPbPb->SetFillStyle(1001);
    hPbPb->SetLineWidth(0);
    hPbPb->Draw("SAME");

    TLegend* L = new TLegend(0.32, 0.65, 0.89, 0.85);
    L->SetTextSize(0.023);
    L->AddEntry(hPbPb, "PbPb (5.02 TeV) 70 - 90%, 404  #mub^{-1}  #LT N_{Part} #GT = 11.1", "pf");
    L->AddEntry(hXeXe, "XeXe (5.44 TeV) 70 - 80%, 3.42  #mub^{-1} #LT N_{Part} #GT = 10.55", "pf");
    L->AddEntry(hNeNe, "NeNe (5.36 TeV) 0 - 100%, 0.8 nb^{-1}    #LT N_{Part} #GT = 12.7", "pf");
    L->AddEntry(hOO,   "OO    (5.36 TeV) 0 - 100%, 6.0 nb^{-1}    #LT N_{Part} #GT = 10.8", "pf");
    L->SetBorderSize(0);
    L->SetFillStyle(0);
    L->Draw();

     // Add TLatex label for pseudorapidity
    TLatex* latex_eta = new TLatex(0.55, 1.35, "Track |#eta| < 1");
    latex_eta->SetTextSize(0.027);
    latex_eta->SetTextFont(42);
    latex_eta->Draw();


    float XeXeLumi = 0.26;
    float PbPbLumi = 0.16;
    float NeNeLumi = 0.05;
    float OOLumi = 0.05;

    TBox* box_PbPblumi = new TBox(0.5, 1-PbPbLumi, 0.55, 1+PbPbLumi);
    box_PbPblumi->SetFillStyle(0);
    box_PbPblumi->SetLineColor(cmsYellow);
    box_PbPblumi->SetLineWidth(2);
    
    TBox* box_XeXelumi = new TBox(0.55, 1-XeXeLumi, 0.605, 1+XeXeLumi);
    box_XeXelumi->SetLineColor(cmsRed);
    box_XeXelumi->SetFillStyle(0);
    box_XeXelumi->SetLineWidth(2);

    TBox* box_NeNelumi = new TBox(0.605, 1-NeNeLumi, 0.6655, 1+NeNeLumi);
    box_NeNelumi->SetLineColor(cmsViolet);
    box_NeNelumi->SetFillStyle(0);
    box_NeNelumi->SetLineWidth(2);

    TBox* box_OOLumi = new TBox(0.6655, 1 - OOLumi, 0.73205, 1 + OOLumi);
    box_OOLumi->SetLineColor(cmsBlue);
    box_OOLumi->SetFillStyle(0);
    box_OOLumi->SetLineWidth(2);

    box_PbPblumi->Draw();
    box_XeXelumi->Draw();
    box_NeNelumi->Draw();
    box_OOLumi->Draw();


    gPad->Update();
    // Remove these frame calls that override margins
    // gPad->GetFrame()->SetX1(xMin);
    // gPad->GetFrame()->SetX2(xMax);
    // gPad->GetFrame()->SetY1(yMin);
    // gPad->GetFrame()->SetY2(yMax);
    // gPad->Update();
    AddCMSHeader(c, true);

    c->SaveAs("plot2.pdf");
}