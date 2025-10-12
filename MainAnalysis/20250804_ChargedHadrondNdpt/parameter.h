#include <filesystem>
namespace fs = std::filesystem;

//============================================================//
// Define analysis parameters
//============================================================//
class Parameters {
public:
  Parameters(int TriggerChoice, float scaleFactor = 1.0)
      : TriggerChoice(TriggerChoice), scaleFactor(scaleFactor) {
  }
  Parameters() {}
  string input;      // Input file name
  string output;     // Output file name
  int TriggerChoice; // FIXME: option to be defined
  bool CollisionType;
  float scaleFactor; // Scale factor
  bool UseSpeciesWeight; //Use species-by-species correction weight
  bool UseTrackWeight; // Use track weight
  bool UseEventWeight; // Use event weight
  bool ApplyEventSelection; // Apply event selection criteria
  int TrackSelectionOption; // Selection criteria for track weight
  int SpeciesCorrectionOption; //Part species correction option
  int EventSelectionOption;
  string EventCorrectionFile; // File for event selection efficiency
  bool HideProgressBar; // Hide progress bar in output
  void printParameters() const {

    cout << "Input file: " << input << endl;
    cout << "Output file: " << output << endl;
    cout << "TriggerChoice: " << TriggerChoice << endl;
    cout << "CollisionType: " << CollisionType << endl;
    cout << "Scale factor: " << scaleFactor << endl;
    cout << "UseSpeciesWeight" << UseSpeciesWeight << endl;
    cout << "UseTrackWeight: " << UseTrackWeight << endl;
    cout << "UseEventWeight: " << UseEventWeight << endl;
    cout << "ApplyEventSelection: " << ApplyEventSelection << endl;
    cout << "TrackSelectionOption: " << TrackSelectionOption << endl;
    cout << "EventCorrectionFile: " << EventCorrectionFile << endl;
    cout << "SpeciesCorrectionOption: " << SpeciesCorrectionOption << endl;
    cout << "HideProgressBar: " << HideProgressBar << endl;
  }
};

void saveParametersToHistograms(const Parameters &par, TFile *outf) {
  outf->cd();         // Navigate to the output file directory
  outf->mkdir("par"); // Create a directory named "par"
  outf->cd("par");    // Change to the "par" directory

  // Create and fill histograms for each parameter
  TH1D *hScaleFactor = new TH1D("parScaleFactor", "parScaleFactor", 1, 0, 1);
  hScaleFactor->SetBinContent(1, par.scaleFactor);
  TH1D *hTriggerChoice = new TH1D("parTriggerChoice", "parTriggerChoice", 1, 0, 1);
  hTriggerChoice->SetBinContent(1, par.TriggerChoice);
  TH1D *hCollisionType = new TH1D("parCollisionType", "parCollisionType", 1, 0, 1);
  hCollisionType->SetBinContent(1, par.CollisionType);
  TH1D *hUseSpeciesWeight = new TH1D("parUseSpeciesWeight", "parUseSpeciesWeight", 1, 0, 1);
  hUseSpeciesWeight->SetBinContent(1, par.UseSpeciesWeight);
  TH1D *hUseTrackWeight = new TH1D("parUseTrackWeight", "parUseTrackWeight", 1, 0, 1);
  hUseTrackWeight->SetBinContent(1, par.UseTrackWeight);
  TH1D *hUseEventWeight = new TH1D("parUseEventWeight", "parUseEventWeight", 1, 0, 1);
  hUseEventWeight->SetBinContent(1, par.UseEventWeight);
  TH1D *hApplyEventSelection = new TH1D("parApplyEventSelection", "parApplyEventSelection", 1, 0, 1);
  hApplyEventSelection->SetBinContent(1, par.ApplyEventSelection);
  TH1D *hTrackSelectionOption = new TH1D("parTrackSelectionOption", "parTrackSelectionOption", 1, 0, 1);
  hTrackSelectionOption->SetBinContent(1, par.TrackSelectionOption);
  TH1D *hSpeciesCorrectionOption = new TH1D("parSpeciesCorrectionOption", "parSpeciesCorrectionOption", 1, 0, 1);
  hSpeciesCorrectionOption->SetBinContent(1, par.SpeciesCorrectionOption);
  TH1D *hEventSelectionOption = new TH1D("parEventSelectionOption", "parEventSelectionOption", 1, 0, 1);
  hEventSelectionOption->SetBinContent(1, par.EventSelectionOption);
  

  // Write histograms to the output file
  hTriggerChoice->Write();
  hScaleFactor->Write();
  hCollisionType->Write();
  hUseSpeciesWeight->Write();
  hUseTrackWeight->Write();
  hUseEventWeight->Write();
  hApplyEventSelection->Write();
  hTrackSelectionOption->Write();
  hSpeciesCorrectionOption->Write();
  hEventSelectionOption->Write();

  // Clean up
  delete hTriggerChoice;
  delete hScaleFactor;
  delete hCollisionType;
  delete hUseSpeciesWeight;
  delete hUseTrackWeight;
  delete hUseEventWeight;
  delete hApplyEventSelection;
  delete hTrackSelectionOption;
  delete hSpeciesCorrectionOption;
  delete hEventSelectionOption;
}
