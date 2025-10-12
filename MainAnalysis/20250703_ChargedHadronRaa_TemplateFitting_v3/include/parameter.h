#include <filesystem>
namespace fs = std::filesystem;

//============================================================//
// Define analysis parameters
//============================================================//
class Parameters {
public:
  Parameters(float VzMax = 15.0, int CCFilter = 1, int PVFilter = 1, float MinTrackPt = 0.3, float HFE_min1 = 4.0, float HFE_min2 = 4.0, int ADC_min1 = 14, int ADC_min2 = 0)            
      :  VzMax(VzMax), CCFilter(CCFilter), PVFilter(PVFilter), MinTrackPt(MinTrackPt), HFE_min1(HFE_min1), HFE_min2(HFE_min2), ADC_min1(ADC_min1), ADC_min2(ADC_min2) {
  }
  Parameters() {}
  string input;      // Input file name
  string output;     // Output file name 
  float VzMax;       // Z Vertex
  int CCFilter;      // Cluster Compatibility Filter
  int PVFilter;      // Primary vertex filter
  float MinTrackPt; // Minimum track pT
  float HFE_min1;    // Minimum energy for HF
  float HFE_min2;    // Minimum subleading energy for the HF's opposite side
  int ADC_min1;     // Minimum leading ADC for online HF
  int ADC_min2;     // Minimum subleading ADC for the online HF's opposite side

  void printParameters() const {

    cout << "Parameters:" << endl;
    cout << "  Input file: " << input << endl;
    cout << "  Output file: " << output << endl;
    cout << "  VzMax: " << VzMax << endl;
    cout << "  CCFilter: " << CCFilter << endl;
    cout << "  PVFilter: " << PVFilter << endl;
    cout << "  MinTrackPt: " << MinTrackPt << endl;
    cout << "  HFE_min1: " << HFE_min1 << endl;
    cout << "  HFE_min2: " << HFE_min2 << endl;
    cout << "  ADC_min1: " << ADC_min1 << endl;
    cout << "  ADC_min2: " << ADC_min2 << endl;
  }
};

void saveParametersToHistograms(const Parameters &par, TFile *outf) {
  outf->cd();         // Navigate to the output file directory
  outf->mkdir("par"); // Create a directory named "par"
  outf->cd("par");    // Change to the "par" directory

  // Create and fill histograms for each parameter
  TH1D *hVzMax = new TH1D("hVzMax", "VzMax", 1, 0, 1);
  hVzMax->SetBinContent(1, par.VzMax);  
  TH1D *hCCFilter = new TH1D("hCCFilter", "CCFilter", 1, 0, 1);
  hCCFilter->SetBinContent(1, par.CCFilter);
  TH1D *hPVFilter = new TH1D("hPVFilter", "PVFilter", 1, 0, 1);
  hPVFilter->SetBinContent(1, par.PVFilter);
  TH1D *hMinTrackPt = new TH1D("hMinTrackPt", "MinTrackPt", 1, 0, 1);
  hMinTrackPt->SetBinContent(1, par.MinTrackPt);
  TH1D *hHFE_min1 = new TH1D("hHFE_min1", "HFE_min1", 1, 0, 1);
  hHFE_min1->SetBinContent(1, par.HFE_min1);
  TH1D *hHFE_min2 = new TH1D("hHFE_min2", "HFE_min2", 1, 0, 1);
  hHFE_min2->SetBinContent(1, par.HFE_min2);
  TH1D *hADC_min1 = new TH1D("hADC_min1", "ADC_min1", 1, 0, 1);
  hADC_min1->SetBinContent(1, par.ADC_min1);
  TH1D *hADC_min2 = new TH1D("hADC_min2", "ADC_min2", 1, 0, 1);
  hADC_min2->SetBinContent(1, par.ADC_min2);  
  

  // Write histograms to the output file
  hVzMax->Write();
  hCCFilter->Write();
  hPVFilter->Write();
  hMinTrackPt->Write();
  hHFE_min1->Write();
  hHFE_min2->Write();
  hADC_min1->Write();
  hADC_min2->Write();

  // Clean up
  delete hVzMax;
  delete hCCFilter;
  delete hPVFilter;
  delete hMinTrackPt;
  delete hHFE_min1;
  delete hHFE_min2;
  delete hADC_min1;
  delete hADC_min2;
}
