#include <filesystem>
namespace fs = std::filesystem;

//============================================================//
// Define analysis parameters
//============================================================//
class Parameters {
public:
  Parameters(float MinTrackPt, int TriggerChoice, bool IsData, float scaleFactor = 1.0, float VzMax = 15.0, 
             int NVtxMin = 1, int CCFilter = 1, int PVFilter = 1, int IsHijing = 0, float HFE_min1 = 4.0, float HFE_min2 = 4.0)
      : MinTrackPt(MinTrackPt), TriggerChoice(TriggerChoice), IsData(IsData), scaleFactor(scaleFactor), 
        VzMax(VzMax), NVtxMin(NVtxMin), CCFilter(CCFilter), PVFilter(PVFilter), IsHijing(IsHijing), HFE_min1(HFE_min1), HFE_min2(HFE_min2) {
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

  float VzMax;       // Z Vertex
  int NVtxMin;       // Minimum number of vertices
  int CCFilter;      // Cluster Compatibility Filter
  int PVFilter;      // Primary vertex filter
  int IsHijing;      // Flags sample as Hijing, and imposes Npart > 1 cut
  float HFE_min1;    // Minimum energy for HF
  float HFE_min2;    // Minimum subleading energy for the HF's opposite side

  void printParameters() const {

    cout << "Input file: " << input << endl;
    cout << "Output file: " << output << endl;
    cout << "MinTrackPt: " << MinTrackPt << endl;
    cout << "TriggerChoice: " << TriggerChoice << endl;
    cout << "IsData: " << IsData << endl;
    cout << "Scale factor: " << scaleFactor << endl;
    cout << "VzMax: " << VzMax << endl;
    cout << "NVtxMin: " << NVtxMin << endl;
    cout << "CCFilter: " << CCFilter << endl;
    cout << "PVFilter: " << PVFilter << endl;
    cout << "IsHijing: " << IsHijing << endl;
    cout << "HFE_min1: " << HFE_min1 << endl;
    cout << "HFE_min2: " << HFE_min2 << endl;
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

  TH1D *hVzMax = new TH1D("parVzMax", "parVzMax", 1, 0, 1);
  hVzMax->SetBinContent(1, par.VzMax);
  TH1D *hNVtxMin = new TH1D("parNVtxMin", "parNVtxMin", 1, 0, 1);
  hNVtxMin->SetBinContent(1, par.NVtxMin);
  TH1D *hCCFilter = new TH1D("parCCFilter", "parCCFilter", 1, 0, 1);
  hCCFilter->SetBinContent(1, par.CCFilter);
  TH1D *hPVFilter = new TH1D("parPVFilter", "parPVFilter", 1, 0, 1);
  hPVFilter->SetBinContent(1, par.PVFilter);
  TH1D *hIsHijing = new TH1D("parIsHijing", "parIsHijing", 1, 0, 1);
  hIsHijing->SetBinContent(1, par.IsHijing);
  TH1D *hHFE_min1 = new TH1D("parHFE_min1", "parHFE_min1", 1, 0, 1);
  hHFE_min1->SetBinContent(1, par.HFE_min1);
  TH1D *hHFE_min2 = new TH1D("parHFE_min2", "parHFE_min2", 1, 0, 1);
  hHFE_min2->SetBinContent(1, par.HFE_min2);
  

  // Write histograms to the output file
  hMinTrackPt->Write();
  hIsData->Write();
  hTriggerChoice->Write();
  hScaleFactor->Write();

  hVzMax->Write();
  hNVtxMin->Write();
  hCCFilter->Write();
  hPVFilter->Write();
  hIsHijing->Write();
  hHFE_min1->Write();
  hHFE_min2->Write();

  // Clean up
  delete hMinTrackPt;
  delete hTriggerChoice;
  delete hIsData;
  delete hScaleFactor;
  delete hVzMax;
  delete hNVtxMin;
  delete hPVFilter;
  delete hIsHijing;
  delete hHFE_min1;
  delete hHFE_min2;
}
