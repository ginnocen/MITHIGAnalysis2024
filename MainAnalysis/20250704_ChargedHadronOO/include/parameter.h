#include <filesystem>
namespace fs = std::filesystem;

//============================================================//
// Define analysis parameters
//============================================================//
class Parameters {
public:
  Parameters(float MinTrackPt, int TriggerChoice, bool IsData, float scaleFactor = 1.0)
      : MinTrackPt(MinTrackPt), TriggerChoice(TriggerChoice), IsData(IsData), scaleFactor(scaleFactor) {
    if (MinTrackPt < 0.1) {
      printf("[INFO] attention: you are performing the analysis with a very low pT track selection");
    }
  }
  Parameters() {}
  string input;      // Input file name
  string output;     // Output file name
  float MinTrackPt;  // Minimum track pt
  float MaxTrackPt;  // Maximum track pt
  int TriggerChoice; // FIXME: option to be defined
  bool IsData;       // Data or MC
  bool IsPP;
  float scaleFactor; // Scale factor
  bool UseTrackWeight; // Use track weight
  bool UseEventWeight; // Use event weight
  bool ApplyEventSelection; // Apply event selection criteria
  float OnlineHFAND; // Online HF AND condition
  float OfflineHFAND;  // Offline HF AND condition
  void printParameters() const {

    cout << "Input file: " << input << endl;
    cout << "Output file: " << output << endl;
    cout << "MinTrackPt: " << MinTrackPt << endl;
    cout << "MaxTrackPt: " << MaxTrackPt << endl;
    cout << "TriggerChoice: " << TriggerChoice << endl;
    cout << "IsData: " << IsData << endl;
    cout << "IsPP: " << IsPP << endl;
    cout << "Scale factor: " << scaleFactor << endl;
    cout << "UseTrackWeight: " << UseTrackWeight << endl;
    cout << "UseEventWeight: " << UseEventWeight << endl;
    cout << "ApplyEventSelection: " << ApplyEventSelection << endl;
    cout << "OnlineHFAND: " << OnlineHFAND << endl;
    cout << "OfflineHFAND: " << OfflineHFAND << endl;
  }
};

void saveParametersToHistograms(const Parameters &par, TFile *outf) {
  outf->cd();         // Navigate to the output file directory
  outf->mkdir("par"); // Create a directory named "par"
  outf->cd("par");    // Change to the "par" directory

  // Create and fill histograms for each parameter
  TH1D *hMinTrackPt = new TH1D("parMinTrackPt", "parMinTrackPt", 1, 0, 1);
  hMinTrackPt->SetBinContent(1, par.MinTrackPt);
  TH1D *hIsData = new TH1D("parIsData", "parIsData", 1, 0, 1);
  hIsData->SetBinContent(1, par.IsData);
  TH1D *hScaleFactor = new TH1D("parScaleFactor", "parScaleFactor", 1, 0, 1);
  hScaleFactor->SetBinContent(1, par.scaleFactor);
  TH1D *hTriggerChoice = new TH1D("parTriggerChoice", "parTriggerChoice", 1, 0, 1);
  hTriggerChoice->SetBinContent(1, par.TriggerChoice);

  // Write histograms to the output file
  hMinTrackPt->Write();
  hIsData->Write();
  hTriggerChoice->Write();
  hScaleFactor->Write();

  // Clean up
  delete hMinTrackPt;
  delete hTriggerChoice;
  delete hIsData;
  delete hScaleFactor;
}
