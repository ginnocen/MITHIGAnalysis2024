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
  int TriggerChoice; // FIXME: option to be defined
  bool IsData;       // Data or MC
  float scaleFactor; // Scale factor
  void printParameters() const {

    cout << "Input file: " << input << endl;
    cout << "Output file: " << output << endl;
    cout << "MinTrackPt: " << MinTrackPt << endl;
    cout << "TriggerChoice: " << TriggerChoice << endl;
    cout << "IsData: " << IsData << endl;
    cout << "Scale factor: " << scaleFactor << endl;
  }
};

void saveParametersToHistograms(const Parameters &par, TFile *outf) {
  outf->cd();         // Navigate to the output file directory
  outf->mkdir("par"); // Create a directory named "par"
  outf->cd("par");    // Change to the "par" directory

  // Create and fill histograms for each parameter
  TH1D *hMinTrackPt = new TH1D("parMinTrackPt", "parMinTrackPt", 1, 0, 1);
  hMinTrackPt->SetBinContent(1, par.MinTrackPt);
  TH1D *hTriggerChoice = new TH1D("parTriggerChoice", "parTriggerChoice", 1, 0, 1);
  hTriggerChoice->SetBinContent(1, par.TriggerChoice);
  TH1D *hIsData = new TH1D("parIsData", "parIsData", 1, 0, 1);
  hIsData->SetBinContent(1, par.IsData);
  TH1D *hScaleFactor = new TH1D("parScaleFactor", "parScaleFactor", 1, 0, 1);
  hScaleFactor->SetBinContent(1, par.scaleFactor);

  // Write histograms to the output file
  hMinTrackPt->Write();
  hTriggerChoice->Write();
  hIsData->Write();
  hScaleFactor->Write();

  // Clean up
  delete hMinTrackPt;
  delete hTriggerChoice;
  delete hIsData;
  delete hScaleFactor;
}
