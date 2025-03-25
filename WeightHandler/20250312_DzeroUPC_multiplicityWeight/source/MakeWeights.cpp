#include <string>
#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"


#include "WeightHandler2D.h"
#include "WeightHandler1D.h"
#include "CommandLine.h" // Yi's Commandline bundle

using namespace std;

/* + Description:
 *    Macro to create the 1D (mult) weight file
 */

int main(int argc, char *argv[])
{
  CommandLine CL(argc, argv);
  string UnweightedFileName = CL.Get      ("UnweightedFileName", "MC.root");
  string TargetFileName     = CL.Get      ("TargetFileName", "Data.root");
  string TreeName           = CL.Get      ("TreeName", "ntReweighting");
  string WeightFileName     = CL.Get      ("WeightFileName", "Weights/testWeight.root");
  bool DoGptGyReweighting   = CL.GetBool  ("DoGptGyReweighting", "true");
  string GptGyWeightFileName= CL.Get      ("GptGyWeightFileName", "../20250305_DzeroUPC_GptGyWeight/Weights/testWeight.root");
  bool IsGammaN             = CL.GetBool  ("IsGammaN", true);

  /////// Loading the files ///////
  TFile *fUnweighted = TFile::Open(UnweightedFileName.c_str(), "READ");
  if (!fUnweighted || fUnweighted->IsZombie())
  {
    std::cerr << "Error: Could not open unweighted file!" << std::endl;
    return 1;
  }
  
  TTree *tUnweighted = (TTree*) fUnweighted->Get(TreeName.c_str());
  if (!tUnweighted)
  {
    std::cerr << "Error: Could not retrieve tree!" << std::endl;
    return 1;
  }

  int nTrackInAcceptanceHP;
  float leadingGpt, leadingGy;
  tUnweighted->SetBranchAddress("nTrackInAcceptanceHP", &nTrackInAcceptanceHP);
  tUnweighted->SetBranchAddress("leadingGpt", &leadingGpt);
  tUnweighted->SetBranchAddress("leadingGy", &leadingGy);


  TFile *fTarget = TFile::Open(TargetFileName.c_str(), "READ");
  if (!fTarget || fTarget->IsZombie())
  {
    std::cerr << "Error: Could not open Target file!" << std::endl;
    return 1;
  }

  TTree *tTarget = (TTree*) fTarget->Get(TreeName.c_str());
  if (!tTarget)
  {
    std::cerr << "Error: Could not retrieve tree!" << std::endl;
    return 1;
  }

  tTarget->SetBranchAddress("nTrackInAcceptanceHP", &nTrackInAcceptanceHP);

  /////// Getting the multiplicity histogram ///////
  WeightHandler2D GptGyWH;
  if (DoGptGyReweighting)
  {
    GptGyWH.LoadFromFile(GptGyWeightFileName);
  }

  // Create histogram by using the same histogram binnings
  double _multBins[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                        10,11,12,13,14,16,18,22,80 };
  const int _nMultBins = sizeof(_multBins)/sizeof(double) - 1;
  TH1D *h_unweighted_EvtMult  = new TH1D("h_unweighted_EvtMult", ";Charged hadron multiplicity;", _nMultBins, _multBins); // 80, 0, 80);
  TH1D *h_target_EvtMult      = new TH1D("h_target_EvtMult", ";Charged hadron multiplicity;", _nMultBins, _multBins); // 80, 0, 80);

  Long64_t nEntries = tUnweighted->GetEntries();
  for (Long64_t i = 0; i < nEntries; i++)
  {
    tUnweighted->GetEntry(i);

    double GptGyWeight = 1.0;
    if (DoGptGyReweighting) GptGyWeight = GptGyWH.GetWeight(leadingGpt, leadingGy,
                                                           (!IsGammaN));

    h_unweighted_EvtMult->Fill(nTrackInAcceptanceHP, GptGyWeight);
  }

  nEntries = tTarget->GetEntries();
  for (Long64_t i = 0; i < nEntries; i++)
  {
    tTarget->GetEntry(i);
    h_target_EvtMult->Fill(nTrackInAcceptanceHP);
  }

  WeightHandler1D MultWH(h_target_EvtMult, h_unweighted_EvtMult);
  MultWH.WriteToFile(WeightFileName.c_str());

	return 0;
}