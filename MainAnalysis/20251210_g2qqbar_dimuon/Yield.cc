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

float LFYield(TNtuple* data_nt, TNtuple* nt_uds, TNtuple* nt_other, TNtuple* nt_c, TNtuple* nt_cc, TNtuple* nt_b, TNtuple* nt_bb, float Jetptmin, float Jetptmax){
    
    RooRealVar fitVar("muDiDxy1Dxy2Sig", "muDiDxy1Dxy2Sig", -3, 4);
    fitVar.setRange("fitRange", -2, -0.8);
    fitVar.setRange("normRange", -3, 4);
    
    // DATA
    RooDataSet data("data", "data", data_nt, fitVar, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    
    // LIGHT FLAVOR TEMPLATE
    RooDataSet templateLF("templateLF", "Light flavor", nt_uds, fitVar, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    
    // HEAVY FLAVOR TEMPLATE 
    RooDataSet templateHF_other("tmp_other", "", nt_other, fitVar, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet templateHF_c("tmp_c", "", nt_c, fitVar, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet templateHF_cc("tmp_cc", "", nt_cc, fitVar, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet templateHF_b("tmp_b", "", nt_b, fitVar, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    RooDataSet templateHF_bb("tmp_bb", "", nt_bb, fitVar, Form("JetPT < %f && JetPT >= %f", Jetptmax, Jetptmin), "weight");
    
    // APPEND HF TEMPLATES
    RooDataSet templateHF("templateHF", "Heavy flavor", fitVar);
    templateHF.append(templateHF_other);
    templateHF.append(templateHF_c);
    templateHF.append(templateHF_cc);
    templateHF.append(templateHF_b);
    templateHF.append(templateHF_bb);
    
    // PDFs
    RooKeysPdf lfPdf("lfPdf", "Light flavor PDF", fitVar, templateLF, RooKeysPdf::MirrorLeft, 1.0);
    RooKeysPdf hfPdf("hfPdf", "Heavy flavor PDF", fitVar, templateHF, RooKeysPdf::MirrorLeft, 1.0);
    
    // YIELDs
    double nTotal = data.sumEntries();
    RooRealVar nLF("nLF", "N light flavor", nTotal/2, 0, nTotal);
    RooRealVar nHF("nHF", "N heavy flavor", nTotal/2, 0, nTotal);
    RooAddPdf model("model", "LF+HF", RooArgList(lfPdf, hfPdf), RooArgList(nLF, nHF));
    
    // Fit to data in restricted range, but normalize over full range
    RooFitResult* result = model.fitTo(data, 
                                        RooFit::Range("fitRange"),      // Fit only here
                                       RooFit::NormRange("normRange"),  // But yields are for full range
                                       RooFit::Save());
    
    cout << "Light flavor yield: " << nLF.getVal() << " +/- " << nLF.getError() << endl;
    cout << "Heavy flavor yield: " << nHF.getVal() << " +/- " << nHF.getError() << endl;
    
    return nLF.getVal();
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
    TNtuple* nt_other = (TNtuple*)templatesFile->Get("nt_other");
    TNtuple* nt_uds = (TNtuple*)templatesFile->Get("nt_uds");
    TNtuple* nt_c = (TNtuple*)templatesFile->Get("nt_c");
    TNtuple* nt_b = (TNtuple*)templatesFile->Get("nt_b");
    TNtuple* nt_cc = (TNtuple*)templatesFile->Get("nt_cc");
    TNtuple* nt_bb = (TNtuple*)templatesFile->Get("nt_bb");

    // DECLARE HISTOGRAMS
    TFile* outputFile = new TFile(output.c_str(), "RECREATE");
    outputFile->cd();
    TH1D* JpsiYields = new TH1D("JpsiYields", "J/psi Yields", ptBins.size()-1, &ptBins[0]);
    TH1D* LightYields = new TH1D("LightYields", "Light Flavor Yields", ptBins.size()-1, &ptBins[0]);
    TH1D* SplittingYields = new TH1D("SplittingYields", "Splitting Yields", ptBins.size()-1, &ptBins[0]);

    float JspiYield = 0;
    float LightYield = 0;
    float SplittingYield = 0;
    for(int i = 0; i < ptBins.size()-1; i++){
        
        float ptMin = ptBins[i];
        float ptMax = ptBins[i+1];
        
        // TOTAL YIELD
        RooRealVar mass("mumuMass", "mass", 0, 10);
        RooDataSet dataBin("dataBin", "data", nt, mass, 
                          Form("JetPT < %f && JetPT >= %f", ptMax, ptMin), "weight");
        float totalYield = dataBin.sumEntries();
        
        // JPSI
        if(doJpsi) {
            JspiYield = JpsiYield(nt, ptMin, ptMax);
            JpsiYields->SetBinContent(i+1, JspiYield);
        }
        
        // LF
        if(doLF) {
            LightYield = LFYield(nt, nt_uds, nt_other, nt_c, nt_cc, nt_b, nt_bb, ptMin, ptMax);
            LightYields->SetBinContent(i+1, LightYield);
        }
        
        // REMAINING YIELD
        SplittingYield = totalYield - JspiYield - LightYield;
        SplittingYields->SetBinContent(i+1, SplittingYield);
        
    }

    // WRITE TO FILE
    outputFile->cd();
    if(doJpsi) JpsiYields->Write();
    if(doLF) LightYields->Write();
    SplittingYields->Write();

    // SAVE COMMAND LINE PARAMS
    TNamed paramFile("InputFile", file.c_str());
    paramFile.Write();
    TNamed paramTemplates("Templates", templates.c_str());
    paramTemplates.Write();
    TNamed paramDoJpsi("doJpsi", doJpsi ? "true" : "false");
    paramDoJpsi.Write();
    TNamed paramDoLF("doLF", doLF ? "true" : "false");
    paramDoLF.Write();
    TNamed paramMakePlots("makeplots", makeplots ? "true" : "false");
    paramMakePlots.Write();

    outputFile->Close();

    // TODO: MAKE PLOTTING INFRASTRUCTURE

}