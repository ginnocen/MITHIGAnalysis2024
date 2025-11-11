#include <TTree.h>
#include <TFile.h>

#include <RooAddPdf.h>
#include <RooGaussian.h>
#include <RooChebychev.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooFitResult.h>
#include <RooPlot.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <iostream>
#include "CommandLine.h" // Yi's Commandline bundle
#include <TStyle.h>

using namespace std;
using namespace RooFit;

void main_fit(TTree *datatree, string outputstring, double &signalYield_, double &signalError_, float jetptmin, float jetptmax) {
    gStyle->SetOptStat(0);
    TCanvas* canvas = new TCanvas("canvas", "Fit Plot", 800, 800);
    // Define the mass range and variables
    RooRealVar m("mumuMass", "Mass [GeV]", 0.0, 10.0);

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

    // Import data
    RooDataSet data("data", "dataset", RooArgSet(m), Import(*datatree));

    // Fit the model to data
    RooFitResult* result = model.fitTo(data, Save());
    // Extract the signal yield and its uncertainty
    double signalYield = nsig.getVal();  // Get signal yield
    double signalError = nsig.getError();  // Get uncertainty on signal yield
    signalYield_ = signalYield;
    signalError_ = signalError;
    // Print the results
    std::cout << "Signal Yield (nsig): " << signalYield << " ± " << signalError << std::endl;
    // Plot the data and the fit result
    RooPlot* frame = m.frame();
    frame->SetTitleSize(0.03);
    frame->SetTitle(Form("(PbPb) #mu^{+}#mu^{-} mass fit, %.0f < p_{T}^{jet} < %.0f GeV", jetptmin, jetptmax));
    frame->GetYaxis()->SetTitleOffset(1.3);
    data.plotOn(frame);
    model.plotOn(frame);
    model.plotOn(frame, Components(background), LineStyle(kDashed), LineColor(kRed));
    frame->Draw();
    
    canvas->SaveAs(Form("output/fit_result%s.pdf", outputstring.c_str()));

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
  TTree *t = (TTree*) inf->Get("nt");
  double signalMuMu, sigMuMuError;
  main_fit(t, outputstring, signalMuMu, sigMuMuError, MinJetPT, MaxJetPT);
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

