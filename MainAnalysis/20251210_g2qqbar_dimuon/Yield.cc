///////////////////////////////////////////////////////////////////////
//        EXTRACT DIMUON YIELD WITHOUT LF COMPONENT + J/PSI          //
///////////////////////////////////////////////////////////////////////

#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TTreeFormula.h>

#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooGaussian.h>
#include <RooPlot.h>

#include <iostream>
#include <vector>

using namespace std;
#include "CommandLine.h" 
#include "DimuonMessenger.h"
#include "Messenger.h"   
#include "ProgressBar.h" 

using namespace std;

float JpsiYield(TNtuple* nt, float Jetptmin, float Jetptmax){

    //ESTIMATE JPSI USING SIDEBANDS
    RooRealVar mass("mumuMass", "Dimuon mass [GeV]", 0, 10);
    RooDataSet data("data", "data", nt, mass, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");

    mass.setRange("signal", 2.95, 3.25);
    mass.setRange("leftSB", 2.5, 2.90);
    mass.setRange("rightSB", 3.3, 3.7);

    double nSignal = data.sumEntries("", "signal");
    double nLeftSB = data.sumEntries("", "leftSB");
    double nRightSB = data.sumEntries("", "rightSB");
    double nBkgInSignal = (nLeftSB + nRightSB) * (0.375);

    //GAUSSIAN JPSI PEAK
    RooRealVar mean("mean", "J/psi mass", 3.1, 2.95, 3.25);
    RooRealVar sigma("sigma", "resolution", 0.09, 0.01, 0.2);
    RooRealVar nJpsi("nJpsi", "N J/psi", nSignal - nBkgInSignal, 0, nSignal);
    RooRealVar nBkg("nBkg", "N bkg", nBkgInSignal);
    nBkg.setConstant(true);  // Fixed from sideband estimate

    RooRealVar c0("c0", "constant", 1);
    RooPolynomial bkgPdf("bkgPdf", "background", mass, c0);
    RooGaussian jpsiPdf("jpsiPdf", "J/psi", mass, mean, sigma);
    RooAddPdf model("model", "jpsi+bkg", RooArgList(jpsiPdf, bkgPdf), RooArgList(nJpsi, nBkg));

    RooFitResult* result = model.fitTo(data, RooFit::Range("signal"), RooFit::Save());
    cout << "J/psi yield: " << nJpsi.getVal() << " +/- " << nJpsi.getError() << endl;

    return nJpsi.getVal();
}

float LFYield(){
    

    
}

int main(int argc, char *argv[]) {

    // INPUTS
    cout << "Beginning Acceptance x Efficiency" << endl;
    CommandLine CL(argc, argv);
    string file = CL.Get("Input"); // DISTRIBUTIONS TO BE FITTED (MC OR DATA)
    string output = CL.Get("Output"); 
    string templates = CL.Get("Templates"); // TEMPLATES TO HELP THE FITTING (MC)
    vector<double> ptBins = CL.GetDoubleVector("ptBins");
    bool doJpsi = CL.GetBool("doJpsi");
    bool doLF = CL.GetBool("doLF");
    bool makeplots = CL.GetBool("makeplots");

    // IMPORT TREE
    TFile* input = TFile::Open(file.c_str());
    TNtuple* nt = (TNtuple*)input->Get("ntDimuon");

    TFile* templatesFile = TFile::Open(templates.c_str());
    TNtuple* nt_o = (TNtuple*)templatesFile->Get("nt_other");
    TNtuple* nt_uds = (TNtuple*)templatesFile->Get("nt_uds");
    TNtuple* nt_c = (TNtuple*)templatesFile->Get("nt_c");
    TNtuple* nt_b = (TNtuple*)templatesFile->Get("nt_b");
    TNtuple* nt_cc = (TNtuple*)templatesFile->Get("nt_cc");
    TNtuple* nt_bb = (TNtuple*)templatesFile->Get("nt_bb");
    vector<TNtuple*> vflavors = {nt_o, nt_uds, nt_c, nt_b, nt_cc, nt_bb};

    // DECLARE HISTOGRAMS
    TH1D* JpsiYields = new TH1D("JpsiYields", "J/psi Yields", ptBins.size()-1, &ptBins[0]);
    TH1D* LightYields = new TH1D("LightYields", "Light Flavor Yields", ptBins.size()-1, &ptBins[0]);
    TH1D* SplittingYields = new TH1D("SplittingYields", "Splitting Yields", ptBins.size()-1, &ptBins[0]);

    float JspiYield = 0;
    float LightYield = 0;
    float SplittingYield = 0;
    for(int i = 0; i < ptBins.size()-1; i++){


        

    }

    TFile

    
}