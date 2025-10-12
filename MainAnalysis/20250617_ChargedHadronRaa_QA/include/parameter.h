#include <filesystem>
namespace fs = std::filesystem;

//============================================================//
// Define analysis parameters
//============================================================//
class Parameters {
public:
  Parameters(int TriggerChoice, bool IsData, float scaleFactor = 1.0, float xSection = 1, float VzMax = 15.0, 
             int CCFilter = 1, int PVFilter = 1, int IsHijing = 0, float HFE_min1 = 4.0, float HFE_min2 = 4.0, bool useOnlineHFE = 0, bool doQA = 0)
      : TriggerChoice(TriggerChoice), IsData(IsData), scaleFactor(scaleFactor), xSection(xSection), VzMax(VzMax),
      CCFilter(CCFilter), PVFilter(PVFilter), IsHijing(IsHijing), HFE_min1(HFE_min1), HFE_min2(HFE_min2), useOnlineHFE(useOnlineHFE), doQA(doQA) {
  }
  Parameters() {}
  string input;      // Input file name
  string output;     // Output file name
  int TriggerChoice; // FIXME: option to be defined
  bool IsData;       // Data or MC
  float scaleFactor; // Scale factor

  float xSection;    // Cross section in b 
  float VzMax;       // Z Vertex
  int CCFilter;      // Cluster Compatibility Filter
  int PVFilter;      // Primary vertex filter
  int IsHijing;      // Flags sample as Hijing, and imposes Npart > 1 cut
  float HFE_min1;    // Minimum energy for HF
  float HFE_min2;    // Minimum subleading energy for the HF's opposite side
  bool useOnlineHFE; // Use online HFE cuts
  bool doQA;          // Do QA or not

  void printParameters() const {

    cout << "Input file: " << input << endl;
    cout << "Output file: " << output << endl;
    cout << "TriggerChoice: " << TriggerChoice << endl;
    cout << "IsData: " << IsData << endl;
    cout << "Scale factor: " << scaleFactor << endl;
    cout << "Cross section: " << xSection << endl;
    cout << "VzMax: " << VzMax << endl;
    cout << "CCFilter: " << CCFilter << endl;
    cout << "PVFilter: " << PVFilter << endl;
    cout << "IsHijing: " << IsHijing << endl;
    cout << "HFE_min1: " << HFE_min1 << endl;
    cout << "HFE_min2: " << HFE_min2 << endl;
    cout << "useOnlineHFE: " << useOnlineHFE << endl;
    cout << "doQA: " << doQA << endl;
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

  TH1D *hXSection = new TH1D("parXSection", "parXSection", 1, 0, 1);
  hXSection->SetBinContent(1, par.xSection);
  TH1D *hVzMax = new TH1D("parVzMax", "parVzMax", 1, 0, 1);
  hVzMax->SetBinContent(1, par.VzMax);
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
  TH1D *hUseOnlineHFE = new TH1D("parUseOnlineHFE", "parUseOnlineHFE", 1, 0, 1);
  hUseOnlineHFE->SetBinContent(1, par.useOnlineHFE);

  TH1D *hDoQA = new TH1D("parDoQA", "parDoQA", 1, 0, 1);
  hDoQA->SetBinContent(1, par.doQA);
  

  // Write histograms to the output file
  hIsData->Write();
  hTriggerChoice->Write();
  hScaleFactor->Write();

  hXSection->Write();
  hVzMax->Write();
  hCCFilter->Write();
  hPVFilter->Write();
  hIsHijing->Write();
  hHFE_min1->Write();
  hHFE_min2->Write();
  hUseOnlineHFE->Write();
  hDoQA->Write();

  // Clean up
  delete hTriggerChoice;
  delete hIsData;
  delete hScaleFactor;
  delete hXSection;
  delete hVzMax;
  delete hCCFilter;
  delete hPVFilter;
  delete hIsHijing;
  delete hHFE_min1;
  delete hHFE_min2;
  delete hUseOnlineHFE;
  delete hDoQA;
}
