#include <TTree.h>
#include <TFile.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TNtuple.h>

#include <RooAddPdf.h>
#include <RooGaussian.h>
#include <RooChebychev.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooDataHist.h>
#include <RooFitResult.h>
#include <RooPlot.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <iostream>
#include "CommandLine.h" // Yi's Commandline bundle
#include <TStyle.h>

using namespace std;
using namespace RooFit;

void main_fit(TNtuple *datatuple, string outputstring, double &signalYield_, double &signalError_, float jetptmin, float jetptmax) {
    gStyle->SetOptStat(0);
    TCanvas* canvas = new TCanvas("canvas", "Fit Plot", 800, 800);
    
    // Define the mass range and variables for the ntuple data
    RooRealVar m("mumuMass", "Mass [GeV]", 0.0, 7.0);  // Match your histogram range
    RooRealVar muDiDxy1Dxy2("muDiDxy1Dxy2", "DCA", -10, 2);
    RooRealVar mumuPt("mumuPt", "Dimuon pT [GeV]", 0, 50);
    RooRealVar JetPT("JetPT", "Jet pT [GeV]", 0, 500);

    // Define the signal model: double Gaussian
    RooRealVar mean("mean", "mean", 1.5, 1.0, 2.0);
    RooRealVar sigma1("sigma1", "width of first Gaussian", 0.5, 0.0, 1.0);
    RooRealVar sigma2("sigma2", "width of second Gaussian", 1.0, 0.0, 2.0); 
    RooGaussian gauss1("gauss1", "first Gaussian", m, mean, sigma1);
    RooGaussian gauss2("gauss2", "second Gaussian", m, mean, sigma2);
    RooRealVar sigFrac("sigFrac", "fraction of first Gaussian", 0.5, 0.0, 1.0);
    RooAddPdf signal("signal", "signal model", RooArgList(gauss1, gauss2), sigFrac);

    // Define the Jpsi signal model
    RooRealVar meanJpsi("meanJpsi", "mean of Jpsi", 3.0969, 3.0, 3.2);
    RooRealVar sigmaJpsi("sigmaJpsi", "width of Jpsi", 0.05, 0.03, 0.07);
    RooGaussian jpsi("jpsi", "Jpsi signal", m, meanJpsi, sigmaJpsi);
    
    RooRealVar meanUpsilon("meanUpsilon", "mean of Upsilon", 9.5, 9.0, 10.0);
    RooRealVar sigmaUpsilon("sigmaUpsilon", "width of Upsilon", 0.1, 0.0, 1.0);
    RooGaussian upsilon("upsilon", "Upsilon signal", m, meanUpsilon, sigmaUpsilon);
    RooRealVar oniaFrac("oniaFrac", "fraction of Jpsi and Upsilon", 0.5, 0.0, 1.0);
    RooAddPdf background("background", "background", RooArgList(jpsi, upsilon), oniaFrac);

    // Define the combined model
    RooRealVar nsig("nsig", "number of signal events", 500, 0, 10000);
    RooRealVar nbkg("nbkg", "number of background events", 500, 0, 10000);
    RooAddPdf model("model", "signal + background", RooArgList(signal, background), RooArgList(nsig, nbkg));

    // Import ntuple data - we only need the mumuMass for the fit
    RooDataSet data("data", "dataset", RooArgSet(m, muDiDxy1Dxy2, mumuPt, JetPT), Import(*datatuple));

    // Fit the model to data
    RooFitResult* result = model.fitTo(data, Save());
    // Extract the signal yield and its uncertainty
    double signalYield = nsig.getVal();  // Get signal yield
    double signalError = nsig.getError();  // Get uncertainty on signal yield
    signalYield_ = signalYield;
    signalError_ = signalError;
    // Print the results
    std::cout << "Signal Yield (nsig): " << signalYield << " ± " << signalError << std::endl;
    std::cout << "Background Yield (nbkg): " << nbkg.getVal() << " ± " << nbkg.getError() << std::endl;
    
    // Plot the data and the fit result
    RooPlot* frame = m.frame(Title(Form("(PbPb) #mu^{+}#mu^{-} mass fit, %.0f < p_{T}^{jet} < %.0f GeV", jetptmin, jetptmax)));
    frame->SetTitleSize(0.03);
    frame->GetYaxis()->SetTitleOffset(1.3);
    frame->GetXaxis()->SetTitle("m_{#mu#mu} [GeV/c^{2}]");
    frame->GetYaxis()->SetTitle("Events / 0.1 GeV/c^{2}");
    
    // Plot data points
    data.plotOn(frame, MarkerStyle(20), MarkerSize(0.8), Name("data"));
    
    // Plot total fit
    model.plotOn(frame, LineColor(kBlue), LineWidth(2), Name("total"));
    
    // Plot individual components
    model.plotOn(frame, Components(signal), LineStyle(kSolid), LineColor(kGreen+2), LineWidth(2), Name("signal"));
    model.plotOn(frame, Components(background), LineStyle(kDashed), LineColor(kRed), LineWidth(2), Name("background"));
    model.plotOn(frame, Components(jpsi), LineStyle(kDotted), LineColor(kMagenta), LineWidth(2), Name("jpsi"));
    model.plotOn(frame, Components(upsilon), LineStyle(kDashDotted), LineColor(kOrange), LineWidth(2), Name("upsilon"));
    
    // Draw the plot
    frame->Draw();
    
    // Add legend
    TLegend* legend = new TLegend(0.65, 0.65, 0.88, 0.88);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);
    legend->AddEntry(frame->findObject("data"), "Data", "p");
    legend->AddEntry(frame->findObject("total"), "Total Fit", "l");
    legend->AddEntry(frame->findObject("signal"), "Signal (double Gauss)", "l");
    legend->AddEntry(frame->findObject("background"), "Background (J/#psi + #Upsilon)", "l");
    legend->AddEntry(frame->findObject("jpsi"), "J/#psi component", "l");
    legend->AddEntry(frame->findObject("upsilon"), "#Upsilon component", "l");
    legend->Draw();
    
    // Add text box with fit results
    TLatex* latex = new TLatex();
    latex->SetNDC();
    latex->SetTextSize(0.03);
    latex->DrawLatex(0.15, 0.85, Form("Signal yield: %.0f #pm %.0f", signalYield, signalError));
    latex->DrawLatex(0.15, 0.81, Form("Background yield: %.0f #pm %.0f", nbkg.getVal(), nbkg.getError()));
    latex->DrawLatex(0.15, 0.77, Form("#chi^{2}/ndf: %.2f", frame->chiSquare()));
    
    canvas->SaveAs(Form("fit_result%s.pdf", outputstring.c_str()));

}


int main(int argc, char *argv[]) {
  std::cout << "Starting mass fit" << std::endl;
  CommandLine CL(argc, argv);
  string input         = CL.Get      ("Input",   "output.root"); // Input file
  string output        = CL.Get      ("Output",  "fit.root");    // Output file
  string outputstring  = CL.Get      ("Label",   "label");       // Label for output file
  float MinJetPT       = CL.GetDouble("MinJetPT", 30);          // Minimum Jet PT
  float MaxJetPT       = CL.GetDouble("MaxJetPT", 40);          // Maximum Jet PT
  TFile *inf  = new TFile(input.c_str());
  TFile *outf = new TFile(output.c_str(), "RECREATE");
  TNtuple *nt = (TNtuple*) inf->Get("nt");

  
  double signalMuMu, sigMuMuError;
  main_fit(nt, outputstring, signalMuMu, sigMuMuError, MinJetPT, MaxJetPT);
  TH1F *hYieldMuMuJetPt = new TH1F("hYieldMuMuJetPt", "Yield MuMu", 1, 0, 1);
  hYieldMuMuJetPt->SetBinContent(1, signalMuMu);
  TH1F *hInclusivejetPT = (TH1F*) inf->Get("hInclusivejetPT");
  TH1F *hYieldInclusiveJetPt = new TH1F("hYieldInclusiveJetPt", "Yield Inclusive", 1, 0, 1);
  hYieldInclusiveJetPt->SetBinContent(1, hInclusivejetPT->GetEntries());
  outf->cd();
  hYieldMuMuJetPt->Write();
  hYieldInclusiveJetPt->Write();
  outf->Close();
}

