#include <string>
#include <iostream>

#include "TFile.h"
#include "TH2D.h"


#include "WeightHandler.h"
#include "CommandLine.h" // Yi's Commandline bundle

using namespace std;

/* + Description:
 *    Macro to create the 2D (Dpt, Dy) weight file
 */

int main(int argc, char *argv[])
{
  CommandLine CL(argc, argv);
  string numFileName     = CL.Get      ("numFileName",    "num.root");
  string numHistName     = CL.Get      ("numHistName",    "h_num");
  string denFileName     = CL.Get      ("denFileName",    "den.root");
  string denHistName     = CL.Get      ("denHistName",    "h_den");
  string weightFileName  = CL.Get      ("weightFileName", "Weights/testWeight.root");

  /////////// Input
  TFile numFile(numFileName.c_str(), "READ");
  
  if (!numFile.IsOpen())
  {
    std::cerr << "Error: Could not open file " << numFileName << std::endl;
    return 1;
  }

  TH2D* h_num = dynamic_cast<TH2D*>(numFile.Get(numHistName.c_str()));


  TFile denFile(denFileName.c_str(), "READ");
  
  if (!denFile.IsOpen())
  {
    std::cerr << "Error: Could not open file " << denFileName << std::endl;
    return 1;
  }

  TH2D* h_den = dynamic_cast<TH2D*>(denFile.Get(denHistName.c_str()));


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
  WeightHandler w(h_num, h_den);
  w.WriteToFile(weightFileName.c_str());

	return 0;
}