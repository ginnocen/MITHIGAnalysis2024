#include <filesystem>
namespace fs = std::filesystem;

//============================================================//
// Define analysis parameters
//============================================================//
class Parameters {
public:
  Parameters(int TriggerChoice, bool IsData, float scaleFactor = 1.0)
      : TriggerChoice(TriggerChoice), IsData(IsData), scaleFactor(scaleFactor) {
  }
  Parameters() {}
  string input;      // Input file name
  string output;     // Output file name
  int TriggerChoice; // FIXME: option to be defined
  bool IsData;       // Data or MC
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
    cout << "IsData: " << IsData << endl;
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
  TH1D *hIsData = new TH1D("parIsData", "parIsData", 1, 0, 1);
  hIsData->SetBinContent(1, par.IsData);
  TH1D *hScaleFactor = new TH1D("parScaleFactor", "parScaleFactor", 1, 0, 1);
  hScaleFactor->SetBinContent(1, par.scaleFactor);
  TH1D *hTriggerChoice = new TH1D("parTriggerChoice", "parTriggerChoice", 1, 0, 1);
  hTriggerChoice->SetBinContent(1, par.TriggerChoice);

  // Write histograms to the output file
  hIsData->Write();
  hTriggerChoice->Write();
  hScaleFactor->Write();

  // Clean up
  delete hTriggerChoice;
  delete hIsData;
  delete hScaleFactor;
}
