#include "MITHIG_CMSStyle.h"

void plot3(){

    gStyle->SetOptStat(0);

    /// AS

    int A_OO = 16;
    int A_NeNe = 20;
    int A_XeXe = 129;
    int A_PbPb = 208;

    /// NPARTS

    vector<float> nparts_XeXe = {236.1,206.3,141.2,68.5,27.2,10.55};
    vector<float> nparts_XeXe_errorsup = {1.3,1.7,1.8,2.2,1.6,0.78};
    vector<float> nparts_XeXe_errorsdown = {1.3,1.7,1.8,2.2,1.6,0.78};

    vector<float> nparts_PbPb = {384.3,333.3,226.7,109.2,42.2,11.1};
    vector<float> nparts_PbPb_errorsup = {1.8,3.0,5.2,4.3,3.0,1.3};
    vector<float> nparts_PbPb_errorsdown = {2.0,3.2,5.3,4.2,2.9,1.2};

    float npart_OO = 10.8;
    float npart_NeNe = 12.7;
    float npart_pPb = 7.723;

    /// NCOLLS

    vector<float> ncolls_XeXe = {930, 732, 407, 135, 35.3, 9.8, };
    vector<float> ncolls_XeXe_errorsup = {51, 44, 30, 15, 4.8, 1.4};
    vector<float> ncolls_XeXe_errorsdown = {51, 44, 30, 15, 4.8, 1.4};

    vector<float> ncolls_PbPb = {  1820,  1430,  805,  267,  65.4,  10.7};
    vector<float> ncolls_PbPb_errorsup = { 130, 100, 55, 20, 7, 1.7, };
    vector<float> ncolls_PbPb_errorsdown = {140,110, 58, 20, 6.6, 1.5};

    float ncolls_OO = 12.8;
    float ncolls_NeNe = 15.7;
    float ncolls_pPb = 6.7230000;


    TGraphAsymmErrors* gXeXe = new TGraphAsymmErrors();
    TGraphAsymmErrors* gPbPb = new TGraphAsymmErrors();    
    TGraphAsymmErrors* gOO = new TGraphAsymmErrors();
    TGraphAsymmErrors* gNeNe = new TGraphAsymmErrors();
    TGraphAsymmErrors* gpPb = new TGraphAsymmErrors();

    for(int i = 0; i<nparts_XeXe.size(); i++){
        gXeXe->SetPoint(i, nparts_XeXe[i], ncolls_XeXe[i]);
        gXeXe->SetPointError(i, nparts_XeXe_errorsdown[i], nparts_XeXe_errorsup[i], ncolls_XeXe_errorsdown[i], ncolls_XeXe_errorsup[i]);
    }

    for(int i = 0; i< nparts_PbPb.size(); i++){
        gPbPb->SetPoint(i, nparts_PbPb[i], ncolls_PbPb[i]);
        gPbPb->SetPointError(i, nparts_PbPb_errorsdown[i], nparts_PbPb_errorsup[i], ncolls_PbPb_errorsdown[i], ncolls_PbPb_errorsup[i]);
    }

    gOO->SetPoint(0, npart_OO, ncolls_OO);
    gNeNe->SetPoint(0, npart_NeNe, ncolls_NeNe);
    gpPb->SetPoint(0, npart_pPb, ncolls_pPb);

    gXeXe->SetMarkerColor(cmsRed);
    gPbPb->SetMarkerColor(cmsYellow);
    gOO->SetMarkerColor(cmsBlue);
    gNeNe->SetMarkerColor(cmsViolet);
    gpPb->SetMarkerColor(cmsTeal);

    gXeXe->SetLineColor(cmsRed);
    gPbPb->SetLineColor(cmsYellow);
    gOO->SetLineColor(cmsBlue);
    gNeNe->SetLineColor(cmsViolet);
    gpPb->SetLineColor(cmsTeal);

    gXeXe->SetMarkerStyle(33);
    gPbPb->SetMarkerStyle(33);
    gOO->SetMarkerStyle(33);
    gNeNe->SetMarkerStyle(33);
    gpPb->SetMarkerStyle(33);

    gXeXe->SetMarkerSize(1.5);
    gPbPb->SetMarkerSize(1.5);
    gOO->SetMarkerSize(1.5);
    gNeNe->SetMarkerSize(1.5);
    gpPb->SetMarkerSize(1.5);

    TCanvas*c = new TCanvas("c","c",800,800);
    c->cd();
    gPad->SetLogx();
    gPad->SetLogy();
    
    // Set margins
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);

    // Set axis ranges BEFORE setting titles and other properties
    gXeXe->GetXaxis()->SetLimits(2, 1000);
    gXeXe->GetYaxis()->SetRangeUser(2, 10000);
    
    gXeXe->GetXaxis()->SetTitle(" #LT N_{part} #GT");
    gXeXe->GetYaxis()->SetTitle(" #LT N_{coll} #GT");
    
    // Add space between x-axis title and axis
    gXeXe->GetXaxis()->SetTitleOffset(1.4);

    gXeXe->Draw("AP");
    gPbPb->Draw("P same");
    gOO->Draw("P same");
    gNeNe->Draw("P same");
    gpPb->Draw("P same");

    // Center axis titles after drawing
    gXeXe->GetXaxis()->CenterTitle(true);
    gXeXe->GetYaxis()->CenterTitle(true);

    // Center axis labels after drawing
    gXeXe->GetXaxis()->CenterLabels(true);
    gXeXe->GetYaxis()->CenterLabels(true);

    // Create a combined graph with all data points for fitting in log space
    TGraph* gCombined = new TGraph();
    int pointIndex = 0;
    
    // Add XeXe points (using log values, no errors)
    for(int i = 0; i < nparts_XeXe.size(); i++){
        gCombined->SetPoint(pointIndex, log(nparts_XeXe[i]), log(ncolls_XeXe[i]));
        pointIndex++;
    }
    
    // Add PbPb points (using log values, no errors)
    for(int i = 0; i < nparts_PbPb.size(); i++){
        gCombined->SetPoint(pointIndex, log(nparts_PbPb[i]), log(ncolls_PbPb[i]));
        pointIndex++;
    }
    
    // Add OO point (using log values, no errors)
    gCombined->SetPoint(pointIndex, log(npart_OO), log(ncolls_OO));
    pointIndex++;
    
    // Add NeNe point (using log values, no errors)
    gCombined->SetPoint(pointIndex, log(npart_NeNe), log(ncolls_NeNe));
    pointIndex++;

    gCombined->SetPoint(pointIndex, log(npart_pPb), log(ncolls_pPb));
    
    // Create and perform linear fit on log-log data
    TF1* fitCombined = new TF1("fitCombined", "pol1", log(5), log(1000));
    fitCombined->SetLineColor(kBlack);
    fitCombined->SetLineWidth(3);
    fitCombined->SetLineStyle(2); // Dashed line
    
    gCombined->Fit(fitCombined, "RQ"); // R=restrict range, Q=quiet
    
    // Convert fit back to original coordinates for drawing
    TF1* fitDisplay = new TF1("fitDisplay", "[0]*pow(x,[1])", 7, 600);
    // If log(y) = a + b*log(x), then y = exp(a) * x^b
    fitDisplay->SetParameter(0, exp(fitCombined->GetParameter(0)));
    fitDisplay->SetParameter(1, fitCombined->GetParameter(1));
    fitDisplay->SetLineColorAlpha(kBlack, 0.2);
    fitDisplay->SetLineWidth(3);
    fitDisplay->SetLineStyle(2);
    //fitDisplay->Draw("same");

    TLegend* L = new TLegend(0.65,0.12,0.88,0.47);
    L->SetTextSize(0.035);
    L->AddEntry(fitDisplay, "Loglinear Fit", "l");
    L->SetBorderSize(0);
    L->SetFillStyle(0);
    //L->Draw();


    // Add TLatex with fit parameters
    /*
    TLatex* fitText = new TLatex();
    fitText->SetNDC();
    fitText->SetTextFont(42);
    fitText->SetTextSize(0.027);
    
    // Get fit parameters
    double a = fitCombined->GetParameter(0);
    double b = fitCombined->GetParameter(1);
    double chi2 = fitCombined->GetChisquare();
    int ndf = fitCombined->GetNDF();
    
    // Format the fit equation: N_coll = exp(a) * N_part^b
    double coeff = exp(a);
    
    fitText->DrawLatex(0.63, 0.24, Form("Fit: N_{coll} = %.2f #times N_{part}^{%.3f}", coeff, b));
    fitText->DrawLatex(0.63, 0.20, Form("#chi^{2}/ndf = %.2f/%d = %.2f", chi2, ndf, chi2/ndf));
    */


    TLegend*L2 = new TLegend(0.15,0.7,0.6,0.88);
    L2->AddEntry(gPbPb, "PbPb (5.02 TeV)", "p");
    L2->AddEntry(gXeXe, "XeXe (5.44 TeV)", "p");
    L2->AddEntry(gNeNe, "NeNe (5.36 TeV)", "p");
    L2->AddEntry(gOO, "OO (5.36 TeV)", "p");
    L2->AddEntry(gpPb, "pPb (5.02 TeV)", "p");
    L2->SetBorderSize(0);
    L2->SetFillStyle(0);
    L2->Draw();

    TLatex* latex1 = new TLatex(0.75, 0.68, "XeXe 0-5%");
    latex1->SetNDC();
    latex1->SetTextFont(42);
    latex1->SetTextSize(0.027);
    latex1->SetTextColor(cmsRed);
    latex1->Draw();

    TLatex* latex2 = new TLatex(0.73, 0.655, "XeXe 5-10%");
    latex2->SetNDC();
    latex2->SetTextFont(42);
    latex2->SetTextSize(0.027);
    latex2->SetTextColor(cmsRed);
    latex2->Draw();

    TLatex* latex3 = new TLatex(0.68, 0.605, "XeXe 10-30%");
    latex3->SetNDC();
    latex3->SetTextFont(42);
    latex3->SetTextSize(0.027);
    latex3->SetTextColor(cmsRed);
    latex3->Draw();

    TLatex* latex4 = new TLatex(0.59, 0.51, "XeXe 30-50%");
    latex4->SetNDC();
    latex4->SetTextFont(42);
    latex4->SetTextSize(0.027);
    latex4->SetTextColor(cmsRed);
    latex4->Draw();

    TLatex* latex5 = new TLatex(0.485, 0.39, "XeXe 50-70%");
    latex5->SetNDC();
    latex5->SetTextFont(42);
    latex5->SetTextSize(0.027);
    latex5->SetTextColor(cmsRed);
    latex5->Draw();

    TLatex* latex6 = new TLatex(0.38, 0.28, "XeXe 70-80%");
    latex6->SetNDC();
    latex6->SetTextFont(42);
    latex6->SetTextSize(0.027);
    latex6->SetTextColor(cmsRed);
    latex6->Draw();

    TLatex* latex7 = new TLatex(0.22, 0.31, "OO 0-100%");
    latex7->SetNDC();
    latex7->SetTextFont(42);
    latex7->SetTextSize(0.027);
    latex7->SetTextColor(cmsBlue);
    latex7->Draw();

    TLatex* latex8 = new TLatex(0.40, 0.32, "NeNe 0-100%");
    latex8->SetNDC();
    latex8->SetTextFont(42);
    latex8->SetTextSize(0.027);
    latex8->SetTextColor(cmsViolet);
    latex8->Draw();

    TLatex* latex9 = new TLatex(0.65, 0.75, "PbPb 0-5%");
    latex9->SetNDC();
    latex9->SetTextFont(42);
    latex9->SetTextSize(0.027);
    latex9->SetTextColor(cmsYellow);
    latex9->Draw();

    TLatex* latex10 = new TLatex(0.62, 0.72, "PbPb 5-10%");
    latex10->SetNDC();
    latex10->SetTextFont(42);
    latex10->SetTextSize(0.027);
    latex10->SetTextColor(cmsYellow);
    latex10->Draw();

    TLatex* latex11 = new TLatex(0.555, 0.67, "PbPb 10-30%");
    latex11->SetNDC();
    latex11->SetTextFont(42);
    latex11->SetTextSize(0.027);
    latex11->SetTextColor(cmsYellow);
    latex11->Draw();

    TLatex* latex12 = new TLatex(0.46, 0.574, "PbPb 30-50%");
    latex12->SetNDC();
    latex12->SetTextFont(42);
    latex12->SetTextSize(0.027);
    latex12->SetTextColor(cmsYellow);
    latex12->Draw();

    TLatex* latex13 = new TLatex(0.34, 0.45, "PbPb 50-70%");
    latex13->SetNDC();
    latex13->SetTextFont(42);
    latex13->SetTextSize(0.027);
    latex13->SetTextColor(cmsYellow);
    latex13->Draw();

    TLatex* latex14 = new TLatex(0.19, 0.28, "PbPb 70-90%");
    latex14->SetNDC();
    latex14->SetTextFont(42);
    latex14->SetTextSize(0.027);
    latex14->SetTextColor(cmsYellow);
    latex14->Draw();

    TLatex* latex15 = new TLatex(0.25, 0.20, "pPb 0-100%");
    latex15->SetNDC();
    latex15->SetTextFont(42);
    latex15->SetTextSize(0.027);
    latex15->SetTextColor(cmsTeal);
    latex15->Draw();

    //AddCMSHeader(c, true);
    TLatex* preliminary = new TLatex(0.315, 0.82, "#it{preliminary}");
    preliminary->SetNDC();
    preliminary->SetTextFont(42);
    preliminary->SetTextSize(0.035);
    preliminary->SetTextColor(kBlack);
    //preliminary->Draw();

    c->SaveAs("plot3.pdf");


}
