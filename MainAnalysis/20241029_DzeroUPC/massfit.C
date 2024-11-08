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

#include <iostream>
#include "CommandLine.h" // Yi's Commandline bundle


using namespace std;
using namespace RooFit;

void main_fit(TTree *datatree, string outputstring) {
    TCanvas* canvas = new TCanvas("canvas", "Fit Plot", 800, 600);
    // Define the mass range and variables
    RooRealVar m("Dmass", "Mass [GeV]", 1.68, 2.05);

    // Define the signal model: double Gaussian
    RooRealVar mean("mean", "mean", 1.86484, 1.85, 1.88);
    RooRealVar sigma1("sigma1", "width of first Gaussian", 0.03, 0.01, 0.1);
    RooRealVar sigma2("sigma2", "width of second Gaussian", 0.01, 0.005, 0.03);
    RooGaussian gauss1("gauss1", "first Gaussian", m, mean, sigma1);
    RooGaussian gauss2("gauss2", "second Gaussian", m, mean, sigma2);
    RooRealVar sigFrac("sigFrac", "fraction of first Gaussian", 0.5, 0.0, 1.0);
    RooAddPdf signal("signal", "signal model", RooArgList(gauss1, gauss2), sigFrac);

    // Define the background model: Chebychev polynomial
    RooRealVar a0("a0", "constant", 0.5, -1.0, 1.0);
    RooRealVar a1("a1", "slope", -0.2, -1.0, 1.0);
    RooChebychev background("background", "background", m, RooArgList(a0, a1));

    // Define the combined model
    RooRealVar nsig("nsig", "number of signal events", 500, 0, 10000);
    RooRealVar nbkg("nbkg", "number of background events", 500, 0, 10000);
    RooAddPdf model("model", "signal + background", RooArgList(signal, background), RooArgList(nsig, nbkg));

    // Import data
    RooDataSet data("data", "dataset", RooArgSet(m), Import(*datatree));

    // Fit the model to data
    RooFitResult* result = model.fitTo(data, Save());

    // Plot the data and the fit result
    RooPlot* frame = m.frame();
    data.plotOn(frame);
    model.plotOn(frame);
    model.plotOn(frame, Components(background), LineStyle(kDashed), LineColor(kRed));
    frame->Draw();
    canvas->SaveAs(Form("fit_result%s.pdf", outputstring.c_str()));
    
}


int main(int argc, char *argv[]) {
  std::cout << "Starting mass fit" << std::endl;
  CommandLine CL(argc, argv);
  string input         = CL.Get      ("Input",   "output.root"); // Input file
  string output        = CL.Get      ("Output",  "fit.root");    // Output file
  string outputstring  = CL.Get      ("Label",   "label");       // Label for output file
  TFile *inf  = new TFile(input.c_str());
  TFile *outf = new TFile(output.c_str());
  TTree *t = (TTree*) inf->Get("nt");
  main_fit(t, outputstring);

}
