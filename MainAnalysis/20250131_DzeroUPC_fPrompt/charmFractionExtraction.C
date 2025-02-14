#include <TTree.h>
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TChain.h>
#include <TH1D.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TMath.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TLegend.h>

#include <RooAddPdf.h>
#include <RooAbsPdf.h>
#include <RooGaussian.h>
#include <RooCBShape.h>
#include <RooExponential.h>
#include <RooChebychev.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooFitResult.h>
#include <RooPlot.h>
#include <RooWorkspace.h>

#include "CommandLine.h" // Yi's Commandline bundle
#include "utilities.h"

using namespace std;
using namespace RooFit;
#include <RooArgSet.h>
#include <RooRealVar.h>
#include <RooConstVar.h>
#include <RooFormulaVar.h>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <iomanip> // For setting precision

int charmFractionExtraction(string dataInput, string promptTemplInput, string nonPromptTemplInput,
				string fitting,
        string plotPrefix, string outputMD)
{
  TFile* dataFile 					= TFile::Open(dataInput.c_str());
  TFile* promptTemplFile 		= TFile::Open(promptTemplInput.c_str());
  TFile* nonPromptTemplFile = TFile::Open(nonPromptTemplInput.c_str());

  // 1. Draw normalized distributions for shape comparisons
  TH1D *data_hDCA_SR_sub 								= (TH1D*) dataFile->Get("hDCA_SR_sub");
  TH1D *data_hDCA_SR_sub_norm 					= (TH1D*) dataFile->Get("details/hDCA_SR_sub_norm");
  TH1D *genMatchedPrompt_hDCA_SR_norm 	= (TH1D*) promptTemplFile->Get("hDCA_SR_norm");
  TH1D *genMatchedNonPrompt_hDCA_SR_norm= (TH1D*) nonPromptTemplFile->Get("hDCA_SR_norm");

  // 2. Histogram PDF fit
  TCanvas *c2 = new TCanvas("c2","c2",600,600);
  gPad->SetLeftMargin(0.13);
  gPad->SetRightMargin(0.05);
  c2->Divide(1,2,0,0);
  TPad* c2_sonpad1 = (TPad*) c2->GetPad(1);
  TPad* c2_sonpad2 = (TPad*) c2->GetPad(2);
  c2_sonpad1->SetPad( 0.0, 0.30, 1.0, 1.0 );
  c2_sonpad2->SetPad( 0.0, 0.0, 1.0, 0.30 );
  c2_sonpad1->SetMargin( 0.18, 0.12, 0.07, 0.12 );
  c2_sonpad2->SetMargin( 0.18, 0.12, 0.40, 0.00 );
  c2_sonpad2->SetGridy(1);
  c2_sonpad1->Draw();
  c2_sonpad2->Draw();

  c2_sonpad1->cd();
  data_hDCA_SR_sub->SetLineWidth(2);
  data_hDCA_SR_sub->SetLineColor(kBlack);

  // Define the custom fit function as a linear combination of the two templates
  TF1* fitFunc = new TF1("fitFunc", [&](double* x, double* par) {
      int bin = genMatchedPrompt_hDCA_SR_norm->FindBin(x[0]);
      double template1Value = genMatchedPrompt_hDCA_SR_norm->GetBinContent(bin);
      double template2Value = genMatchedNonPrompt_hDCA_SR_norm->GetBinContent(bin);
      return par[0] * (par[1] * template1Value + (1-par[1]) * template2Value);
  }, genMatchedPrompt_hDCA_SR_norm->GetXaxis()->GetXmin(), genMatchedPrompt_hDCA_SR_norm->GetXaxis()->GetXmax(), 2);

  // Set initial parameter values and names
  fitFunc->SetParameters(1, 1);
  fitFunc->SetParLimits(0, 0, data_hDCA_SR_sub->Integral()*1.1);
  fitFunc->SetParLimits(1, 0, 2);
  fitFunc->SetParNames("yield", "promptFraction");

  TFitResultPtr fitResult = data_hDCA_SR_sub->Fit(fitFunc, (fitting=="unbinned")? "RS0L": "RS0");
  double yield          = fitFunc->GetParameter(0);
  double promptFraction = fitFunc->GetParameter(1);
  double yieldErr          = fitFunc->GetParError(0);
  double promptFractionErr = fitFunc->GetParError(1);

  TH1D* data_hDCA_SR_sub_pull = (TH1D*) data_hDCA_SR_sub->Clone(Form("%s_pull",data_hDCA_SR_sub->GetName()));
  data_hDCA_SR_sub_pull->Reset();
  for (int i = 1; i <= data_hDCA_SR_sub->GetNbinsX(); ++i) 
  {
    double dataValue = data_hDCA_SR_sub->GetBinContent(i);
    double dataError = data_hDCA_SR_sub->GetBinError(i);
    double fitValue = fitFunc->Eval(data_hDCA_SR_sub->GetBinCenter(i));

    if (dataError != 0) 
    {
        double pull    = (dataValue - fitValue) / dataError;
        double pullErr = dataError / dataError;
        data_hDCA_SR_sub_pull->SetBinContent(i, pull);
        data_hDCA_SR_sub_pull->SetBinError(i, pull);
    } else 
    {
        data_hDCA_SR_sub_pull->SetBinContent(i, 0);
    }
  }

	gPad->SetLogy();
  gStyle->SetOptStat(0);
  // data_hDCA_SR_sub->GetYaxis()->SetRangeUser(0.9, data_hDCA_SR_sub->GetMaximum()*4);
  // data_hDCA_SR_sub->GetXaxis()->SetRangeUser(data_hDCA_SR_sub->GetXaxis()->GetXmin(), data_hDCA_SR_sub->GetXaxis()->GetXmax()-0.01);
  data_hDCA_SR_sub->Draw();
  fitFunc->SetLineColor(kBlue);
  fitFunc->DrawClone("same");

  // plot prompt
  fitFunc->SetParameters(yield*promptFraction , 1);
  fitFunc->SetLineStyle(2);
  fitFunc->SetLineWidth(2);
  fitFunc->SetLineColor(kRed);
  fitFunc->DrawClone("same");

  // plot non-prompt
  fitFunc->SetParameters(yield*(1-promptFraction) , 0);
  fitFunc->SetLineStyle(2);
  fitFunc->SetLineWidth(2);
  fitFunc->SetLineColor(kOrange+1);
  fitFunc->DrawClone("same");

  TLegend *legend2 = new TLegend(0.3, 0.67, 0.8, 0.87);
  legend2->AddEntry((TObject*) 0, Form("Prompt fraction: %.3f #pm %.3f", promptFraction, promptFractionErr), "");
  legend2->AddEntry((TObject*) 0, Form("#chi^{2}/NDF=%.3f/%d=%.3f", fitResult->Chi2(), fitResult->Ndf(), fitResult->Chi2()/((float) fitResult->Ndf())), "");
  formatLegend(legend2,18);
  legend2->Draw();

  c2_sonpad2->cd();
  data_hDCA_SR_sub_pull->SetTitle("");
  data_hDCA_SR_sub_pull->GetYaxis()->SetTitle("pull");
  data_hDCA_SR_sub_pull->SetMaximum(5);
  data_hDCA_SR_sub_pull->SetMinimum(-5);
  data_hDCA_SR_sub_pull->GetXaxis()->SetTitleSize(data_hDCA_SR_sub_pull->GetXaxis()->GetTitleSize() / 0.4);
  data_hDCA_SR_sub_pull->GetXaxis()->SetLabelSize(data_hDCA_SR_sub_pull->GetXaxis()->GetLabelSize() / 0.4);
  data_hDCA_SR_sub_pull->GetYaxis()->SetTitleSize(data_hDCA_SR_sub_pull->GetYaxis()->GetTitleSize() / 0.4);
  data_hDCA_SR_sub_pull->GetYaxis()->SetLabelSize(data_hDCA_SR_sub_pull->GetYaxis()->GetLabelSize() / 0.4);
  data_hDCA_SR_sub_pull->GetXaxis()->SetTitleOffset(data_hDCA_SR_sub_pull->GetXaxis()->GetTitleOffset());
  data_hDCA_SR_sub_pull->GetYaxis()->SetTitleOffset(data_hDCA_SR_sub_pull->GetYaxis()->GetTitleOffset() * 0.42);
  data_hDCA_SR_sub_pull->GetYaxis()->SetNdivisions(205);
  data_hDCA_SR_sub_pull->SetMarkerStyle(20);
  data_hDCA_SR_sub_pull->SetMarkerSize(1.0);
  data_hDCA_SR_sub_pull->SetMarkerColor(kBlack);
  data_hDCA_SR_sub_pull->DrawClone("ep");

  c2->SaveAs(Form("%sDCAFit.pdf",plotPrefix.c_str()));

  // this error calculation ignores efficiency errors!!
  // std::cout << "pt: " << parMinDzeroPT << "," << parMaxDzeroPT
  //           << ", y: " << parMinDzeroY << "," << parMaxDzeroY;
  std::cout << ", promptFraction: " << promptFraction << " +/- " << promptFractionErr;
  std::cout << ", yield: " << yield << " +/- " << yieldErr;

  // Open the file in overwrite mode (std::ios::out)
  std::ofstream outFile(outputMD.c_str(), std::ios::out);
  if (!outFile) {
    std::cerr << "Error: Cannot open output.md for writing!" << std::endl;
    return 1;
  }

  // Write headers if the file is empty
  if (outFile.tellp() == 0) {
    outFile << "| ptmin | ptmax | ymin | ymax |" << std::endl;
    outFile << "|-------|-------|------|------|" << std::endl;
  }

  // outFile << "| " << std::fixed << std::setprecision(4)
  //         << parMinDzeroPT << " | " << parMaxDzeroPT
  //         << " | " << parMinDzeroY << " | " << parMaxDzeroY
  //         << " |" << std::endl;

  outFile << "| yield | promptFraction | - | - |" << std::endl;

  outFile << "| " << std::fixed << std::setprecision(4)
          << yield << " +/- " << yieldErr
          << " | " << promptFraction << " +/- " << promptFractionErr
          << " | "
          << " | "
          << " |" << std::endl;

  // Close the file
  outFile.close();
  return 0;
}



int main(int argc, char *argv[])
{
  CommandLine CL(argc, argv);
  string dataInput     				= CL.Get      ("dataInput",    "output.root"); // Input data file
  string promptTemplInput  		= CL.Get      ("promptTemplInput",  "fit.root");    // Input data file
  string nonPromptTemplInput  = CL.Get      ("nonPromptTemplInput",  "fit.root");    // Input data file
  string fitting        = CL.Get      ("fitting", "binfit");               // fitting (string): binfit, unbinned
  string outputMD         = CL.Get      ("Output", "promptFraction.md");     // Output file
  string plotPrefix      = CL.Get      ("plotPrefix", "./");              // Plot prefix

  int retVal = charmFractionExtraction( dataInput, promptTemplInput, nonPromptTemplInput,
                                    fitting,
                                    plotPrefix, outputMD );

  return retVal;
}