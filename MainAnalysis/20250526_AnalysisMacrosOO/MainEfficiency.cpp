#include <iostream>
#include <TFile.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLegend.h>
#include <utility>
#include <tuple>
#include <string>
#include "CommandLine.h"
#include "Messenger.h"
#include "Parameters.h"
#include "EventSelection.h"
#include "EfficiencyCounting.h"
#include "EfficiencyWrite.h"
#include "EfficiencyResults.h"
#include "ReturnEfficiency.h"
#include "Plot.h"
#include <chrono> // Add this at the top


using namespace std;

void getEfficiencyParameters(CommandLine CL, Parameters &par) {
  par.VzMax = CL.GetDouble("Vzmax", 15.0); // Maximum Z vertex position. Set to very high to disable the cut
  par.NVtxMin = CL.GetInt("NVtxMin", 1); // Minimum number of vertices. Set to 0 to disable the cut
  par.CCFilter = CL.GetInt("CCFilter", 1); // Cluster Compatibility Filter. Set to 0 to disable the cut
  par.PVFilter = CL.GetInt("PVFilter", 1); // Primary vertex filter. Set to 0 to disable the cut
  par.HFEmax_Online_min1 = CL.GetDouble("HFEmax_Online_min1", 4.0); // Minimum energy for HF
  par.HFEmax_Online_min2 = CL.GetDouble("HFEmax_Online_min2", 4.0); // Minimum subleading energy for the HF's opposite side. 
  par.HFEmax_Offline_min1 = CL.GetDouble("HFEmax_Offline_min1", 4.0); // Minimum energy for HF
  par.HFEmax_Offline_min2 = CL.GetDouble("HFEmax_Offline_min2", 4.0); // Minimum subleading energy for the HF's opposite side. 
  par.xsec_SD = CL.GetDouble("xsec_SD", 0.3); // Cross section for Starlight SD
  par.xsec_DD = CL.GetDouble("xsec_DD", 0.0003); // Cross section for Starlight DD
  par.xsec_had = CL.GetDouble("xsec_had", 1.3); // Cross section for HIJING
  par.xsec_alphaO = CL.GetDouble("xsec_alphaO", 0.0); // Cross section for Starlight AlphaO
}

int main(int argc, char *argv[]) {

  CommandLine CL(argc, argv);
  Parameters par;
  FileNames filename;

  filename.SignalFile = CL.Get("SignalFile"); 
  filename.BackgroundFiles.push_back(CL.Get("StarlightSD")); 
  filename.BackgroundFiles.push_back(CL.Get("StarlightDD"));
  filename.BackgroundFiles.push_back(CL.Get("AlphaO"));
  
  cout << "Signal File: " << filename.SignalFile << endl;
  cout << "Background File SD: " << filename.BackgroundFiles[0] << endl;
  cout << "Background File DD: " << filename.BackgroundFiles[1] << endl;
  cout << "Background File AlphaO: " << filename.BackgroundFiles[2] << endl;
  
  getEfficiencyParameters(CL, par);

  vector<double> min1_vector = CL.GetDoubleVector("HFEmax_Offline_min1_vector");
  vector<double> min2_vector = CL.GetDoubleVector("HFEmax_Offline_min2_vector");

  vector<Parameters> par_vector = {};

   for (int i = 0; i < min1_vector.size(); i++) {
    par.HFEmax_Offline_min1 = min1_vector[i];
    par.HFEmax_Offline_min2 = min2_vector[i];
    par_vector.push_back(par);
  }

  bool IsHijing = true; 

  int HFEMmax_Online_min1 = static_cast<int>(par.HFEmax_Online_min1);
  int HFEMmax_Online_min2 = static_cast<int>(par.HFEmax_Online_min2);

  // 1) With Given Inputs, the efficiency will automatically be calculated
  TFile *file_signal = TFile::Open(filename.SignalFile.c_str());
  ChargedHadronRAATreeMessenger *ch = new ChargedHadronRAATreeMessenger(file_signal, "Tree");
  cout << "/////// IMPORTANT EFFICIENCY INFORMATION HERE ///////" << endl;
  for (Parameters &par : par_vector) {
    par.HFEmax_Online_min1 = HFEMmax_Online_min1;
    par.HFEmax_Online_min2 = HFEMmax_Online_min2;
    calculateEfficiency(ch,par,IsHijing,-1);
  }
  cout << "/////// IMPORTANT EFFICIENCY INFORMATION DONE ///////" << endl;
  cout << "/////// CALCULATING PURITY  ///////" << endl;

  // 2)The code would scan save it in a root file named


  // Choose 5-7 values centered around the initial HFEmax_Offline_min1, but not below 0
  /*int nPoints = 7; // Number of points (odd for symmetry)
  int center = static_cast<int>(par.HFEmax_Offline_min1);
  int step = 1; // Step size between points

  int start = std::max(0, center - (nPoints / 2) * step);
  int end = center + (nPoints / 2) * step;

  for (int i = start; i <= end; i += step) {
    par.HFEmax_Offline_min1 = i;
    par.HFEmax_Offline_min2 = i;
    par_vector.push_back(par);
    par.HFEmax_Offline_min1 = i;
    par.HFEmax_Offline_min2 = -2;
    par_vector.push_back(par);
  }*/
  std::string dataoutfolder = "Data/";
  std::string rootfilename = WriteEfficiencyRoot(filename, par_vector, -1, dataoutfolder, Form("%iOR_"));
 // std::string rootfilename = "Data/EfficiencyData/EfficiencyPurityData_N49_trkpt-1_OfflineAND.root";
  EfficiencyResults res = ReturnEfficiency_root(rootfilename.c_str());

  cout << "/////// CALCULATING PURITY DONE ///////" << endl;

  // 3) The code would plot the efficiency and purity
  HistVar1D hvar = {
      "ROC curve with HFEMax selection",   // histTitle
      "HF Energy ADC",         // xLabel
      "Events rejected from selection",         // yLabel
      100,                    // nbin
      0.8,                    // xmin
      1,                    // xmax
      0.8,                    // ymin
      1.1,                    // ymax
      "Plots/",             // outFolderName
      "TESTINGTESTING"            // outFileName
  };

  vector<EfficiencyResults> resultsVec = {res};
  vector<vector<float>> effVecs = {res.EfficiencyHijing};
  vector<vector<float>> purityVecs;
  vector<float> vPurity;

  res.xsec_SD = par.xsec_SD;
  res.xsec_DD = par.xsec_DD;
  res.xsec_had = par.xsec_had;
  res.xsec_alphaO = par.xsec_alphaO;

  for (int j = 0; j < resultsVec.size(); j++){
    EfficiencyResults res = resultsVec[j];
    for (int i = 0; i < effVecs[j].size(); i++) {
        float Purity = CalculatePurity(
            {par.xsec_had, par.xsec_SD, par.xsec_DD, par.xsec_alphaO},
            {res.EfficiencyHijing[i], res.EfficiencySD[i], res.EfficiencyDD[i], res.EfficiencyAO[i]},
            {1, 1, 1, 0}); // Assuming background fraction is 1 for all
        vPurity.push_back(Purity);
        cout << "" << i << endl;
        cout << "HFEmax_Online_min1: " << res.HFEmax_Online_min1[i] << endl;
        cout << "HFEmax_Online_min2: " << res.HFEmax_Online_min2[i] << endl;
        cout << "HFEmax_Offline_min1: " << res.HFEmax_Offline_min1[i] << endl;
        cout << "HFEmax_Offline_min2: " << res.HFEmax_Offline_min2[i] << endl;
        cout << "EfficiencyHijing: " << res.EfficiencyHijing[i] << endl;
        cout << "EfficiencySD: " << res.EfficiencySD[i] << endl;
        cout << "EfficiencyDD: " << res.EfficiencyDD[i] << endl;
        cout << "EfficiencyAO: " << res.EfficiencyAO[i] << endl;
        cout << "Purity for" << ": " << Purity << endl;
    }
    purityVecs.push_back(vPurity);
}

  vector<string> labels = {"Efficiency and Purity"};
  drawROCmultiShapes(resultsVec, effVecs, purityVecs, labels, hvar);
  return 0;
}