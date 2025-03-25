#include <string>
#include <iostream>

#include "TFile.h"
#include "TH2D.h"


#include "WeightHandler2D.h"
#include "CommandLine.h" // Yi's Commandline bundle

using namespace std;

/* + Description:
 *    Macro to create the 2D (Gpt, Gy) weight file
 */

int main(int argc, char *argv[])
{
  CommandLine CL(argc, argv);
  string NumFileName     = CL.Get      ("NumFileName",    "num.root");
  string NumHistName     = CL.Get      ("NumHistName",    "h_num");
  string DenFileName     = CL.Get      ("DenFileName",    "den.root");
  string DenHistName     = CL.Get      ("DenHistName",    "h_den");
  string WeightFileName  = CL.Get      ("WeightFileName", "Weights/testWeight.root");

  /////////// Input
  TFile numFile(NumFileName.c_str(), "READ");
  
  if (!numFile.IsOpen())
  {
    std::cerr << "Error: Could not open file " << NumFileName << std::endl;
    return 1;
  }

  TH2D* h_num = dynamic_cast<TH2D*>(numFile.Get(NumHistName.c_str()));


  TFile denFile(DenFileName.c_str(), "READ");
  
  if (!denFile.IsOpen())
  {
    std::cerr << "Error: Could not open file " << DenFileName << std::endl;
    return 1;
  }

  TH2D* h_den = dynamic_cast<TH2D*>(denFile.Get(DenHistName.c_str()));


  /////////// Checking
  if (!h_num || !h_den) 
  {
    std::cerr << "Error: Could not retrieve histograms." << std::endl;
    return 1;
  }

  // Check if the histograms have the same binning
  if (h_num->GetNbinsX() != h_den->GetNbinsX() || 
      h_num->GetNbinsY() != h_den->GetNbinsY()) 
  {
    std::cerr << "Error: Histograms have different bin numbers." << std::endl;
    return 1;
  }

  for (int i = 0; i <= h_num->GetNbinsX(); ++i) 
  {
    if (h_num->GetXaxis()->GetBinLowEdge(i) != h_den->GetXaxis()->GetBinLowEdge(i)) 
    {
      std::cerr << "Error: X-axis bin edges do not match." << std::endl;
      return 1;
    }
  }

  for (int j = 0; j <= h_num->GetNbinsY(); ++j) 
  {
    if (h_num->GetYaxis()->GetBinLowEdge(j) != h_den->GetYaxis()->GetBinLowEdge(j)) 
    {
      std::cerr << "Error: Y-axis bin edges do not match." << std::endl;
      return 1;
    }
  }

  /////////// Output
  WeightHandler2D w(h_num, h_den);
  w.WriteToFile(WeightFileName.c_str());

	return 0;
}