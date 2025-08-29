#include "MITHIG_CMSStyle.h"
#include "scrape.C"
using namespace std;


void plot2(){

    TFile*f = TFile::Open("data/XeXeDifferential.root");
    TDirectoryFile* dir = (TDirectoryFile*)f->Get("Table 3");
    TH1D*bins = (TH1D*)dir->Get("Hist1D_y1");

    TFile*f2 = TFile::Open("data/c30_50.root");
    TDirectoryFile* dir2 = (TDirectoryFile*)f2->Get("Table 11");
    TH1D*bins2 = (TH1D*)dir2->Get("Hist1D_y1");

    vector<float> smallsystembinedges = {4.8,5.6,6.4,7.2,9.6,12,14.4,19.2,24.0,28.8,35.2,48.0,73.6,103.6};

    TGraphErrors* gOO = new TGraphErrors();
    vector<TBox*> boxesOO;
    for(int i = 0; i< 13; i++){
        GraphEntry entry = getSmallSystemEntry(i, 1);
        // Use template histogram for bin width
        double x = (smallsystembinedges[i]+smallsystembinedges[i+1])/2.0;
        cout << x << endl;
        double y = entry.y;
        gOO->SetPoint(i, x, y);
        gOO->SetPointError(i, 0,0);

        int binIdx = bins->FindBin(x);
        double ex = bins->GetBinWidth(binIdx) / 2.0;
        TBox* box = new TBox(x-ex, y-entry.eyl, x+ex, y+entry.eyh);
        box->SetFillColorAlpha(cmsBlue,0.35);
        box->SetLineColor(cmsBlue+2);
    box->SetLineWidth(1);
    box->SetLineStyle(1); // solid line
        boxesOO.push_back(box);
    }

    TGraphErrors* gNeNe = new TGraphErrors();
    vector<TBox*> boxesNeNe;
    for(int i = 0; i< 13; i++){
        GraphEntry entry = getSmallSystemEntry(i, 2);
        // Use template histogram for bin width
        double x = (smallsystembinedges[i]+smallsystembinedges[i+1])/2.0;
        cout << x << endl;
        double y = entry.y;
        gNeNe->SetPoint(i, x, y);
        gNeNe->SetPointError(i, 0,0);

        int binIdx = bins->FindBin(x);
        double ex = bins->GetBinWidth(binIdx) / 2.0;
        TBox* box = new TBox(x-ex, y-entry.eyl, x+ex, y+entry.eyh);
        box->SetFillColorAlpha(cmsViolet,0.35);
        box->SetLineColor(cmsViolet+2);
        box->SetLineWidth(1);
        box->SetLineStyle(1);
        boxesNeNe.push_back(box);
    }

    TGraphErrors* gXeXe = new TGraphErrors();
    vector<TBox*> boxesXeXe;
    for(int i = 0; i< 29; i++){
        GraphEntry entry = getXeXeEntry(i, 5);
        gXeXe->SetPoint(i, entry.x, entry.y);
        gXeXe->SetPointError(i, 0,0);
    double x = entry.x;
    if (x < 0.4) continue;
    double y = entry.y;
     int binIdx = bins->FindBin(x);
        double ex = bins->GetBinWidth(binIdx) / 2.0;
    TBox* box = new TBox(x-ex, y-entry.eyl, x+ex, y+entry.eyh);
        box->SetFillColorAlpha(cmsRed,0.35);
        box->SetLineColor(cmsRed+2);
    box->SetLineWidth(1);
    box->SetLineStyle(1);
        boxesXeXe.push_back(box);
    }

    TGraphErrors* gPbPb = new TGraphErrors();
    vector<TBox*> boxesPbPb;
    for(int i = 0; i< 29; i++){
        GraphEntry entry = getPbPbEntry(i, 5);
        gPbPb->SetPoint(i, entry.x, entry.y);
        gPbPb->SetPointError(i, 0,0);
    double x = entry.x;
    double y = entry.y;
    int binIdx = bins2->FindBin(x);
    double ex = bins2->GetBinWidth(binIdx) / 2.0;
    TBox* box = new TBox(x-ex, y-entry.eyl, x+ex, y+entry.eyh);
        box->SetFillColorAlpha(cmsYellow,0.35);
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

    gNeNe->GetXaxis()->SetTitle(xLabel);
    gNeNe->GetYaxis()->SetTitle(yLabel);
    gNeNe->GetXaxis()->SetRangeUser(xMin, xMax);
    gNeNe->GetYaxis()->SetRangeUser(yMin, yMax);
    gNeNe->GetXaxis()->CenterTitle(true);
    gNeNe->GetYaxis()->CenterTitle(true);
    gNeNe->GetXaxis()->SetTitleOffset(1.2);
    gNeNe->GetYaxis()->SetTitleOffset(1.2);

    gXeXe->GetXaxis()->SetTitle(xLabel);
    gXeXe->GetYaxis()->SetTitle(yLabel);
    gXeXe->GetXaxis()->SetRangeUser(xMin, xMax);
    gXeXe->GetYaxis()->SetRangeUser(yMin, yMax);
    gXeXe->GetXaxis()->CenterTitle(true);
    gXeXe->GetYaxis()->CenterTitle(true);
    gXeXe->GetXaxis()->SetTitleOffset(1.2);
    gXeXe->GetYaxis()->SetTitleOffset(1.2);

    gPbPb->GetXaxis()->SetTitle(xLabel);
    gPbPb->GetYaxis()->SetTitle(yLabel);
    gPbPb->GetXaxis()->SetRangeUser(xMin, xMax);
    gPbPb->GetYaxis()->SetRangeUser(yMin, yMax);
    gPbPb->GetXaxis()->CenterTitle(true);
    gPbPb->GetYaxis()->CenterTitle(true);
    gPbPb->GetXaxis()->SetTitleOffset(1.2);
    gPbPb->GetYaxis()->SetTitleOffset(1.2);

    TCanvas*c = new TCanvas();
    c->cd();
    c->SetLogx();

    // Draw the graphs and boxes
    
    gOO->Draw("AP");
    gOO->SetMarkerColor(cmsBlue);
    gOO->SetMarkerStyle(33);
    for (auto box : boxesOO) box->Draw();

    gNeNe->Draw("P SAME");
    gNeNe->SetMarkerColor(cmsViolet);
    gNeNe->SetMarkerStyle(33);
    for (auto box : boxesNeNe) box->Draw();

    gXeXe->Draw("P SAME");
    gXeXe->SetMarkerColor(cmsRed);
    gXeXe->SetMarkerStyle(33);
    for (auto box : boxesXeXe) box->Draw();

    gPbPb->Draw("P SAME");
    gPbPb->SetMarkerColor(cmsYellow);
    gPbPb->SetMarkerStyle(33);
    for (auto box : boxesPbPb) box->Draw();

    // Draw horizontal dotted line at y=1
    TLine* line = new TLine(0.4, 1.0, 200.0, 1.0);
    line->SetLineColor(kGray+2);
    line->SetLineStyle(3); // dotted
    line->SetLineWidth(2);
    line->Draw();

    TH1D*hOO = new TH1D("h", "Histogram", 100, 0, 200);
    hOO->SetFillColorAlpha(cmsBlue, 0.35);
    hOO->SetMarkerStyle(33);
    hOO->SetMarkerColor(cmsBlue);
    hOO->SetFillStyle(1001);
    hOO->SetLineWidth(0);
    hOO->Draw("SAME");

    TH1D*hNeNe = new TH1D("h", "Histogram", 100, 0, 200);
    hNeNe->SetFillColorAlpha(cmsViolet, 0.35);
    hNeNe->SetMarkerStyle(33);
    hNeNe->SetMarkerColor(cmsViolet);
    hNeNe->SetFillStyle(1001);
    hNeNe->SetLineWidth(0);
    hNeNe->Draw("SAME");

    TH1D*hXeXe = new TH1D("h", "Histogram", 100, 0, 200);
    hXeXe->SetFillColorAlpha(cmsRed, 0.35);
    hXeXe->SetMarkerStyle(33);
    hXeXe->SetMarkerColor(cmsRed);
    hXeXe->SetFillStyle(1001);
    hXeXe->SetLineWidth(0);
    hXeXe->Draw("SAME");

    TH1D*hPbPb = new TH1D("h", "Histogram", 100, 0, 200);
    hPbPb->SetFillColorAlpha(cmsYellow, 0.35);
    hPbPb->SetMarkerStyle(33);
    hPbPb->SetMarkerColor(cmsYellow);
    hPbPb->SetFillStyle(1001);
    hPbPb->SetLineWidth(0);
    hPbPb->Draw("SAME");

    TLegend* L = new TLegend(0.5, 0.65, 0.89, 0.89);
    L->AddEntry(hOO, "OO 5.02 TeV:   #LT N_{Part} #GT = 10.8", "pf");
    L->AddEntry(hNeNe, "NeNe 5.02 TeV:   #LT N_{Part} #GT = 12.7", "pf");
    L->AddEntry(hXeXe, "XeXe 5.44 TeV (70 - 80%):    #LT N_{Part} #GT = 10.55", "pf");
    L->AddEntry(hPbPb, "PbPb 5.02 TeV (70 - 90%):    #LT N_{Part} #GT = 11.1", "pf");
    L->SetBorderSize(0);
    L->SetFillStyle(0);
    L->Draw();

     // Add TLatex label for pseudorapidity
    TLatex* latex_eta = new TLatex(4, 1.5, "|#eta| < 1");
    latex_eta->SetTextSize(0.03);
    latex_eta->SetTextFont(42);
    latex_eta->Draw();



    

    gPad->Update();
    gPad->GetFrame()->SetX1(xMin);
    gPad->GetFrame()->SetX2(xMax);
    gPad->GetFrame()->SetY1(yMin);
    gPad->GetFrame()->SetY2(yMax);
    gPad->Update();

    c->SaveAs("plot2.pdf");
}