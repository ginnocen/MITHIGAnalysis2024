#include <TTree.h>
#include <TFile.h>

#include <RooAddPdf.h>
#include <RooGaussian.h>
#include <RooCBShape.h>
#include <RooChebychev.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooFitResult.h>
#include <RooPlot.h>
#include <TCanvas.h>
#include <TLatex.h>

#include <iostream>
#include "CommandLine.h" // Yi's Commandline bundle


using namespace std;
using namespace RooFit;
#include <RooRealVar.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>

using namespace std;

struct ParamsBase {
  std::map<std::string, RooRealVar*> params; // Store RooRealVar objects

  ParamsBase() = default;

  virtual ~ParamsBase() = default; // Virtual destructor for proper cleanup

  // Assignment operator
  ParamsBase& operator=(const ParamsBase& other) {
    // Check for self-assignment
    if (this == &other) {
      return *this;
    }

    // Copy only the values and errors from other.params to this.params
    for (const auto& [name, var] : other.params) {
      if (params.find(name) != params.end()) {
        params[name]->setVal(var->getVal());
        params[name]->setAsymError(var->getErrorLo(), var->getErrorHi());
      } else {
        std::cerr << "Warning: Parameter " << name << " not found in params.\n";
      }
    }

    return *this; // Return the current object
  }

  // Method to print all parameters
  void print() const {
    for (const auto& [name, var] : params) {
      std::cout << name << " (" << var->GetTitle() << "): "
                << "Value = " << var->getVal()
                << ", Error = " << var->getError() << "\n";
    }
  }



  // Write parameters to .dat file
  void writeToDat(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
      std::cerr << "Error: Could not open file " << filename << " for writing.\n";
      return;
    }

    // Write header
    file << "name,title,value,lower_error,upper_error\n";

    // Write parameter data
    for (const auto& [name, var] : params) {
      file << name << ","
           << var->GetTitle() << ","
           << var->getVal() << ","
           << var->getErrorLo() << ","
           << var->getErrorHi() << "\n";
    }

    file.close();
    std::cout << "Parameters written to " << filename << "\n";
  }

  // Read parameters from .dat file
  void readFromDat(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cerr << "Error: Could not open file " << filename << " for reading.\n";
      return;
    }

    std::string line;
    bool isHeader = true;

    // this->print();

    while (std::getline(file, line)) {
      if (isHeader) {
        isHeader = false; // Skip the header line
        continue;
      }

      std::istringstream stream(line);
      std::string name, title, valueStr, lowerErrorStr, upperErrorStr;

      // Parse line
      if (std::getline(stream, name, ',') &&
          std::getline(stream, title, ',') &&
          std::getline(stream, valueStr, ',') &&
          std::getline(stream, lowerErrorStr, ',') &&
          std::getline(stream, upperErrorStr, ','))
      {
        double value = std::stod(valueStr);
        double lowerError = std::stod(lowerErrorStr);
        double upperError = std::stod(upperErrorStr);

        // Update parameter if it exists
        if (params.find(name) != params.end()) {
          auto& var = params[name];
          var->setVal(value);                                 // Set value
          var->setAsymError(lowerError, upperError);          // Asymmetric error
          var->setError((- lowerError + upperError)/2.);      // Approximate symmetric error
          var->setRange(value+lowerError, value+upperError);  // Range set to the +/- 1 sigma range
          var->setConstant(true);                             // [Warning!] Now the strategy is to fixed all the prefit parameters
        } else {
          std::cerr << "Warning: Parameter " << name << " not found in params map. Skipping.\n";
        }
      } else {
        std::cerr << "Error: Malformed line in " << filename << ": " << line << "\n";
      }
    }

    file.close();

    // this->print();

  }
};

struct SignalParams : public ParamsBase {
  RooRealVar mean;
  RooRealVar sigma1;
  RooRealVar sigma2;
  RooRealVar frac1;

  SignalParams() :
    mean("sig_mean", "[signal] mean", 1.86484, 1.85, 1.88),
    sigma1("sig_sigma1", "[signal] width of first Gaussian", 0.03, 0.01, 0.1),
    sigma2("sig_sigma2", "[signal] width of second Gaussian", 0.01, 0.005, 0.03),
    frac1("sig_frac1", "[signal] fraction of first Gaussian", 0.1, 0.001, 0.5) 
  {
    // cout << "signal default" << endl;
    params[mean.GetName()] = &mean;
    params[sigma1.GetName()] = &sigma1;
    params[sigma2.GetName()] = &sigma2;
    params[frac1 .GetName()] = &frac1;
  }

  SignalParams(string dat) : SignalParams() { readFromDat(dat); }
};

struct CombinatoricsBkgParams : public ParamsBase {
  RooRealVar a0;
  RooRealVar a1;

  CombinatoricsBkgParams() :
    a0("a0", "constant", 0.5, -1.0, 1.0),
    a1("a1", "slope", -0.2, -1.0, 1.0)
  {
    params[a0.GetName()] = &a0;
    params[a1.GetName()] = &a1;
  }

  CombinatoricsBkgParams(string dat) : CombinatoricsBkgParams() { readFromDat(dat); }
};

struct SwapParams : public ParamsBase {
  RooRealVar mean;
  RooRealVar sigma;

  SwapParams() :
    mean("swap_mean", "[swap] mean", 1.86484, 1.85, 1.88),
    sigma("swap_sigma", "[swap] width of first Gaussian", 0.1, 0.08, 0.15) 
  {
    params[mean.GetName()] = &mean;
    params[sigma.GetName()] = &sigma;
  }

  SwapParams(string dat) : SwapParams() { readFromDat(dat); }
};

struct PeakingKKParams : public ParamsBase {
  RooRealVar mean;
  RooRealVar sigma;
  RooRealVar alpha;
  RooRealVar n;

  PeakingKKParams() :
    mean("kk_mean", "[kk] mean", 1.73, 1.8),
    sigma("kk_sigma", "[kk] width of Crystal Ball", 0.01, 0.081),
    alpha("kk_alpha", "[kk] alpha of Crystal Ball", 1e-3, 2),
    n("kk_n", "[kk] n of Crystal Ball", 1e-2, 10) 
  {
    params[mean.GetName()] = &mean;
    params[sigma.GetName()] = &sigma;
    params[alpha.GetName()] = &alpha;
    params[n .GetName()] = &n;
  }

  PeakingKKParams(string dat) : PeakingKKParams() { readFromDat(dat); }
};

struct PeakingPiPiParams : public ParamsBase {
  RooRealVar mean;
  RooRealVar sigma;
  RooRealVar alpha;
  RooRealVar n;

  PeakingPiPiParams() :
    mean("pipi_mean", "[pipi] mean", 1.89, 1.99),
    sigma("pipi_sigma", "[pipi] width of Crystal Ball", 0.01, 0.08),
    alpha("pipi_alpha", "[pipi] alpha of Crystal Ball", -3, 1),
    n("pipi_n", "[pipi] n of Crystal Ball", 1e-2, 40) 
  {
    params[mean.GetName()] = &mean;
    params[sigma.GetName()] = &sigma;
    params[alpha.GetName()] = &alpha;
    params[n .GetName()] = &n;
  }

  PeakingPiPiParams(string dat) : PeakingPiPiParams() { readFromDat(dat); }
};

struct EventParams {
    RooRealVar nsig;
    RooRealVar nbkg;

    EventParams()
        : nsig("nsig", "number of signal events", 500, 0, 10000),
          nbkg("nbkg", "number of combinatorics background events", 500, 0, 10000) {}
};

struct TotFitParams {
  SignalParams sigl;
  CombinatoricsBkgParams comb;
  SwapParams swap;
  PeakingKKParams pkkk;
  PeakingPiPiParams pkpp;
  EventParams events;

  void print() const {
    std::cout << "Signal Parameters:\n";
    sigl.print();

    std::cout << "Combinatorics Background Parameters:\n";
    comb.print();

    std::cout << "Swap Parameters:\n";
    swap.print();

    std::cout << "KK Parameters:\n";
    pkkk.print();

    std::cout << "pipi Parameters:\n";
    pkpp.print();

    std::cout << "Event Parameters:\n";
    // printVar(events.nsig);
    // printVar(events.nbkg);
  }
};

void sigswpmc_fit(TTree *mctree, string rstDir,
                string& sigldat, string& swapdat) 
{
  std::cout << "=======================================================" << std::endl;
  std::cout << "=    Doing signal prefit ......" << std::endl;
  std::cout << "=======================================================" << std::endl;

  TCanvas* canvas = new TCanvas("canvas", "Fit Plot", 800, 600);
  // Define the mass range and variables
  RooRealVar m("Dmass", "Mass [GeV]", 1.68, 2.05);
  RooRealVar Dgen("Dgen", "Dgen", 0, 30000);

  SignalParams sigl;
  SwapParams swap;

  // Define the signal model: double Gaussian
  RooGaussian gauss1("gauss1", "first Gaussian", m, sigl.mean, sigl.sigma1);
  RooGaussian gauss2("gauss2", "second Gaussian", m, sigl.mean, sigl.sigma2);
  RooAddPdf siglPDF("signal", "signal model", RooArgList(gauss1, gauss2), sigl.frac1);

  // Define the swap model: single Gaussian
  RooGaussian swapPDF("swap", "swap model", m, swap.mean, swap.sigma);

  // Define the combined model
  RooRealVar nsig("nsig", "number of signal events", 500, 0, 10000);
  RooRealVar nswp("nswp", "number of swap events", 500, 0, 10000);
  RooAddPdf model("model", "signal + swap", RooArgList(siglPDF, swapPDF), RooArgList(nsig, nswp));

  // Import data
  RooDataSet _data("_data", "dataset", RooArgSet(m, Dgen), Import(*mctree));
  RooDataSet& data = *( (RooDataSet*) _data.reduce( "(Dgen == 23333 || Dgen == 41022 || Dgen == 41044) || "
                                                    "(Dgen == 23344 || Dgen == 41122 || Dgen == 41144)") );

  std::cout << "[Info] Number of entries: " << data.sumEntries() << std::endl;

  // Fit the model to data
  RooFitResult* result = model.fitTo(data, Save());

  // Plot the data and the fit result
  RooPlot* frame = m.frame();
  data.plotOn(frame);
  model.plotOn(frame);
  model.plotOn(frame, Components(siglPDF), LineStyle(kSolid), LineColor(kRed));
  model.plotOn(frame, Components(gauss1), LineStyle(2), LineWidth(2), LineColor(kRed));
  model.plotOn(frame, Components(gauss2), LineStyle(3), LineWidth(2), LineColor(kRed));
  model.plotOn(frame, Components(swapPDF), LineStyle(kSolid), LineColor(kOrange+1));
  frame->Draw();

  // Add fitted values and errors using TLatex
  TLatex latex;
  latex.SetNDC(); // Use normalized coordinates (0 to 1)
  latex.SetTextSize(0.03);
  latex.SetTextAlign(13); // Align at top-left corner

  double xpos = 0.60, ypos = 0.85, ypos_step = 0.05; // Starting position and step for annotations

  // Draw each parameter value explicitly using ypos - n*ypos_step
  latex.DrawLatex(xpos, ypos - 0 * ypos_step, Form("Mean (Signal): %.3f #pm %.3f", sigl.mean.getVal(), sigl.mean.getError()));
  latex.DrawLatex(xpos, ypos - 1 * ypos_step, Form("Sigma1 (Signal): %.3f #pm %.3f", sigl.sigma1.getVal(), sigl.sigma1.getError()));
  latex.DrawLatex(xpos, ypos - 2 * ypos_step, Form("Sigma2 (Signal): %.3f #pm %.3f", sigl.sigma2.getVal(), sigl.sigma2.getError()));
  latex.DrawLatex(xpos, ypos - 3 * ypos_step, Form("Frac1 (Signal): %.3f #pm %.3f", sigl.frac1.getVal(), sigl.frac1.getError()));
  latex.DrawLatex(xpos, ypos - 4 * ypos_step, Form("Mean (Swap): %.3f #pm %.3f", swap.mean.getVal(), swap.mean.getError()));
  latex.DrawLatex(xpos, ypos - 5 * ypos_step, Form("Sigma (Swap): %.3f #pm %.3f", swap.sigma.getVal(), swap.sigma.getError()));
  latex.DrawLatex(xpos, ypos - 6 * ypos_step, Form("N_{Sig}: %.3f #pm %.3f", nsig.getVal(), nsig.getError()));
  latex.DrawLatex(xpos, ypos - 7 * ypos_step, Form("N_{Swap}: %.3f #pm %.3f", nswp.getVal(), nswp.getError()));

  canvas->SaveAs(Form("%s/fit_result_signal_and_swap.pdf", rstDir.c_str()));
  
  sigl.print();
  swap.print();

  sigldat=Form("%s/sigl.dat", rstDir.c_str());
  swapdat=Form("%s/swap.dat", rstDir.c_str());
  sigl.writeToDat(sigldat.c_str());
  swap.writeToDat(swapdat.c_str());

  delete canvas;
}

void kkmc_fit(TTree *mctree, string rstDir,
              string& pkkkdat)
{
  std::cout << "=======================================================" << std::endl;
  std::cout << "=    Doing kk prefit ......" << std::endl;
  std::cout << "=======================================================" << std::endl;

  TCanvas* canvas = new TCanvas("canvas", "Fit Plot", 800, 600);
  // Define the mass range and variables
  RooRealVar m("Dmass", "Mass [GeV]", 1.68, 2.05);
  RooRealVar Dgen("Dgen", "Dgen", 0, 30000);

  PeakingKKParams pkkk;

  // Define the KK model: Crystal Ball
  RooCBShape pkkkPDF("peaking_kk", "peaking background KK state", m, pkkk.mean, pkkk.sigma, pkkk.alpha, pkkk.n );
  // Define the combined model
  RooRealVar npkkk("npkkk", "number of signal events", 500, 0, 10000);
  RooAddPdf model("model", "kk", RooArgList(pkkkPDF), RooArgList(npkkk));

  // Import data
  RooDataSet _data("_data", "dataset", RooArgSet(m, Dgen), Import(*mctree));
  RooDataSet& data = *( (RooDataSet*) _data.reduce( "Dgen == 333 && Dmass < 1.8648" ) );

  std::cout << "[Info] Number of entries: " << data.sumEntries() << std::endl;

  // Fit the model to data
  RooFitResult* result = model.fitTo(data, Save());

  // Plot the data and the fit result
  RooPlot* frame = m.frame();
  data.plotOn(frame);
  model.plotOn(frame);
  frame->Draw();

  // Add fitted parameter values and errors using TLatex
  double xpos = 0.60, ypos = 0.85, ypos_step = 0.05; // Starting position and step for annotations

  TLatex latex;
  latex.SetTextSize(0.03);
  latex.SetNDC();

  latex.DrawLatex(xpos, ypos - 0 * ypos_step, Form("#mu = %.3f #pm %.3f", pkkk.mean.getVal(), pkkk.mean.getError()));
  latex.DrawLatex(xpos, ypos - 1 * ypos_step, Form("#sigma = %.3f #pm %.3f", pkkk.sigma.getVal(), pkkk.sigma.getError()));
  latex.DrawLatex(xpos, ypos - 2 * ypos_step, Form("#alpha = %.3f #pm %.3f", pkkk.alpha.getVal(), pkkk.alpha.getError()));
  latex.DrawLatex(xpos, ypos - 3 * ypos_step, Form("n = %.3f #pm %.3f", pkkk.n.getVal(), pkkk.n.getError()));
  latex.DrawLatex(xpos, ypos - 4 * ypos_step, Form("N_{KK} = %.3f #pm %.3f", npkkk.getVal(), npkkk.getError()));

  canvas->SaveAs(Form("%s/fit_result_peaking_kk.pdf", rstDir.c_str()));
  
  pkkk.print();

  pkkkdat=Form("%s/pkkk.dat", rstDir.c_str());
  pkkk.writeToDat(pkkkdat.c_str());

  delete canvas;
}

void pipimc_fit(TTree *mctree, string rstDir,
                string& pkppdat)
{
  std::cout << "=======================================================" << std::endl;
  std::cout << "=    Doing pipi prefit ......" << std::endl;
  std::cout << "=======================================================" << std::endl;

  TCanvas* canvas = new TCanvas("canvas", "Fit Plot", 800, 600);
  // Define the mass range and variables
  RooRealVar m("Dmass", "Mass [GeV]", 1.68, 2.05);
  RooRealVar Dgen("Dgen", "Dgen", 0, 30000);

  PeakingPiPiParams pkpp;

  // Define the pipi model: Crystal Ball
  RooCBShape pkppPDF("peaking_pipi", "peaking background pipi state", m, pkpp.mean, pkpp.sigma, pkpp.alpha, pkpp.n );
  // Define the combined model
  RooRealVar npkpp("npkpp", "number of signal events", 500, 0, 10000);
  RooAddPdf model("model", "pipi", RooArgList(pkppPDF), RooArgList(npkpp));

  // Import data
  RooDataSet _data("_data", "dataset", RooArgSet(m, Dgen), Import(*mctree));
  RooDataSet& data = *( (RooDataSet*) _data.reduce( "Dgen == 333 && Dmass > 1.8648" ) );

  std::cout << "[Info] Number of entries: " << data.sumEntries() << std::endl;

  // Fit the model to data
  RooFitResult* result = model.fitTo(data, Save());

  // Plot the data and the fit result
  RooPlot* frame = m.frame();
  data.plotOn(frame);
  model.plotOn(frame);
  frame->Draw();

  // Add fitted parameter values and errors using TLatex
  double xpos = 0.15, ypos = 0.85, ypos_step = 0.05; // Starting position and step for annotations

  TLatex latex;
  latex.SetTextSize(0.03);
  latex.SetNDC();

  latex.DrawLatex(xpos, ypos - 0 * ypos_step, Form("#mu = %.3f #pm %.3f", pkpp.mean.getVal(), pkpp.mean.getError()));
  latex.DrawLatex(xpos, ypos - 1 * ypos_step, Form("#sigma = %.3f #pm %.3f", pkpp.sigma.getVal(), pkpp.sigma.getError()));
  latex.DrawLatex(xpos, ypos - 2 * ypos_step, Form("#alpha = %.3f #pm %.3f", pkpp.alpha.getVal(), pkpp.alpha.getError()));
  latex.DrawLatex(xpos, ypos - 3 * ypos_step, Form("n = %.3f #pm %.3f", pkpp.n.getVal(), pkpp.n.getError()));
  latex.DrawLatex(xpos, ypos - 4 * ypos_step, Form("N_{#pi#pi} = %.3f #pm %.3f", npkpp.getVal(), npkpp.getError()));

  canvas->SaveAs(Form("%s/fit_result_peaking_pipi.pdf", rstDir.c_str()));
  
  pkpp.print();

  pkppdat=Form("%s/pkpp.dat", rstDir.c_str());
  pkpp.writeToDat(pkppdat.c_str());

  delete canvas;
}

void main_fit(TTree *datatree, string rstDir,
              string sigldat, string swapdat,
              string pkkkdat, string pkppdat)
{
  std::cout << "=======================================================" << std::endl;
  std::cout << "=    Doing main fit ......" << std::endl;
  std::cout << "=======================================================" << std::endl;

  TCanvas* canvas = new TCanvas("canvas", "Fit Plot", 800, 600);
  // Define the mass range and variables
  RooRealVar m("Dmass", "Mass [GeV]", 1.68, 2.05);

  TotFitParams p;
  p.sigl = SignalParams(sigldat.c_str());
  p.swap = SwapParams(swapdat.c_str());
  p.pkkk = PeakingKKParams(pkkkdat.c_str());
  p.pkpp = PeakingPiPiParams(pkppdat.c_str());

  // Define the signal model: double Gaussian
  RooGaussian gauss1("gauss1", "first Gaussian", m, p.sigl.mean, p.sigl.sigma1);
  RooGaussian gauss2("gauss2", "second Gaussian", m, p.sigl.mean, p.sigl.sigma2);
  RooAddPdf siglPDF("signal", "signal model", RooArgList(gauss1, gauss2), p.sigl.frac1);

  // Define the background model: Chebychev polynomial
  RooChebychev combPDF("combinatorics", "combinatorics model", m, RooArgList(p.comb.a0, p.comb.a1));

  // Define the swap model: single Gaussian
  RooGaussian swapPDF("swap", "swap model", m, p.swap.mean, p.swap.sigma);

  // Define the KK model: Crystal Ball
  RooCBShape pkkkPDF("peaking_kk", "peaking background KK state", m, p.pkkk.mean, p.pkkk.sigma, p.pkkk.alpha, p.pkkk.n );

  // Define the pipi model: Crystal Ball
  RooCBShape pkppPDF("peaking_pipi", "peaking background pipi state", m, p.pkpp.mean, p.pkpp.sigma, p.pkpp.alpha, p.pkpp.n );

  // Define the combined model
  RooRealVar nsig("nsig", "number of signal events", 500, 0, 10000);
  RooRealVar nswp("nswp", "number of swap events", 500, 0, 10000);
  RooRealVar nbkg("nbkg", "number of background events", 500, 0, 10000);
  RooRealVar npkkk("npkkk", "number of KK events", 500, 0, 10000);
  RooRealVar npkpp("npkpp", "number of pipi events", 500, 0, 10000);
  RooAddPdf model("model", "signal + background", 
                  RooArgList(siglPDF, swapPDF, pkkkPDF, pkppPDF, combPDF), 
                  RooArgList(nsig, nswp, npkkk, npkpp, nbkg));

  // Import data
  RooDataSet data("data", "dataset", RooArgSet(m), Import(*datatree));

  std::cout << "[Info] Number of entries: " << data.sumEntries() << std::endl;

  // Fit the model to data
  RooFitResult* result = model.fitTo(data, Save());

  // Plot the data and the fit result
  RooPlot* frame = m.frame();
  data.plotOn(frame);
  model.plotOn(frame);
  model.plotOn(frame, Components(siglPDF), LineStyle(kSolid), LineColor(kRed));
  model.plotOn(frame, Components(swapPDF), LineStyle(kSolid), LineColor(kOrange+1));
  model.plotOn(frame, Components(pkkkPDF), LineStyle(kSolid), LineColor(kTeal-7));
  model.plotOn(frame, Components(pkppPDF), LineStyle(kSolid), LineColor(kViolet-3));
  model.plotOn(frame, Components(combPDF), LineStyle(kDashed), LineColor(kGray));
  frame->Draw();

  // Add parameter annotations
  double xpos = 0.60, ypos = 0.85, ypos_step = 0.05; // Starting position and step for annotations

  TLatex latex;
  latex.SetTextSize(0.03);
  latex.SetNDC();

  latex.DrawLatex(xpos, ypos - 0 * ypos_step, Form("a_{0} = %.3f #pm %.3f", p.comb.a0.getVal(), p.comb.a0.getError()));
  latex.DrawLatex(xpos, ypos - 1 * ypos_step, Form("a_{1} = %.3f #pm %.3f", p.comb.a1.getVal(), p.comb.a1.getError()));
  latex.DrawLatex(xpos, ypos - 2 * ypos_step, Form("N_{Sig} = %.3f #pm %.3f", nsig.getVal(), nsig.getError()));
  latex.DrawLatex(xpos, ypos - 3 * ypos_step, Form("N_{Swap} = %.3f #pm %.3f", nswp.getVal(), nswp.getError()));
  latex.DrawLatex(xpos, ypos - 4 * ypos_step, Form("N_{KK} = %.3f #pm %.3f", npkkk.getVal(), npkkk.getError()));
  latex.DrawLatex(xpos, ypos - 5 * ypos_step, Form("N_{#pi#pi} = %.3f #pm %.3f", npkpp.getVal(), npkpp.getError()));
  latex.DrawLatex(xpos, ypos - 6 * ypos_step, Form("N_{Comb} = %.3f #pm %.3f", nbkg.getVal(), nbkg.getError()));

  canvas->SaveAs(Form("%s/fit_result.pdf", rstDir.c_str()));

  delete canvas;
}


int main(int argc, char *argv[]) {
  CommandLine CL(argc, argv);
  string dataInput     = CL.Get      ("dataInput",    "output.root"); // Input data file
  string mcInput       = CL.Get      ("mcInput",      "output.root"); // Input mc file
  
  ///// for the component modeling (optional: default is taking the MC sample to do the modeling)
  ///// if the user specify the prefitted shape parameters (.dat), then the main_fit will take those parameters without a prefit procedure directly
  string sigswpInput    = CL.Get     ("sigswpInput",   mcInput.c_str()); // Input mc file for signal and swap component
  string KKmcInput     = CL.Get      ("KKmcInput",    mcInput.c_str()); // Input mc file for D0 > K K component
  string pipimcInput   = CL.Get      ("pipimcInput",  mcInput.c_str()); // Input mc file for D0 > pi pi component
  
  string output        = CL.Get      ("Output",  "fit.root");    // Output file
  string rstDir  = CL.Get      ("RstDir","./");       // Label for output file
  
  TFile *in_data_f  = new TFile(dataInput.c_str());
  TTree *datatree = (TTree*) in_data_f->Get("nt");
  TFile *in_mc_f  = new TFile(mcInput.c_str());
  TTree *mctree = (TTree*) in_mc_f->Get("nt");
  // TFile *outf = new TFile(output.c_str());

  string sigldat, swapdat, pkkkdat, pkppdat;

  if (sigswpInput.find(".dat")==string::npos) {
    TFile *in_sigswp_f  = new TFile(sigswpInput.c_str());
    TTree *sigswptree = (TTree*) in_sigswp_f->Get("nt");
    sigswpmc_fit(sigswptree, rstDir, sigldat, swapdat);
  }
  if (KKmcInput.find(".dat")==string::npos) {
    TFile *in_KKmc_f  = new TFile(KKmcInput.c_str());
    TTree *KKmctree = (TTree*) in_KKmc_f->Get("nt");
    kkmc_fit(KKmctree, rstDir, pkkkdat);
  }
  if (pipimcInput.find(".dat")==string::npos) {
    TFile *in_pipimc_f  = new TFile(pipimcInput.c_str());
    TTree *pipimctree = (TTree*) in_pipimc_f->Get("nt");
    pipimc_fit(mctree, rstDir, pkppdat);
  }

  main_fit(datatree, rstDir, sigldat, swapdat, pkkkdat, pkppdat);

  return 0;
}
