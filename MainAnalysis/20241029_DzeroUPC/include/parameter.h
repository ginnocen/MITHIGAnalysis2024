//============================================================//
// Define analysis parameters
//============================================================//
class Parameters {
public:
    Parameters( float MinDzeroPT, float MaxDzeroPT, float MinDzeroY, float MaxDzeroY, float MaxDalpha, float scaleFactor = 1.0)
    : MinDzeroPT(MinDzeroPT), MaxDzeroPT(MaxDzeroPT), MinDzeroY(MinDzeroY), MaxDzeroY(MaxDzeroY), MaxDalpha(MaxDalpha), scaleFactor(scaleFactor) {}
    string input;          // Input file name
    string output;         // Output file name
    float MinDzeroPT;      // Lower limit of Dzero pt
    float MaxDzeroPT;      // Upper limit of Dzero pt
    float MinDzeroY;       // Lower limit of Dzero rapidity
    float MaxDzeroY;       // Upper limit of Dzero rapidity
    float MaxDalpha;          // MaxDalpha
    float scaleFactor;     // Scale factor
    int nThread;           // Number of Threads
    int nChunk;            // Process the Nth chunk
   void printParameters() const {
       cout << "Input file: " << input << endl;
       cout << "Output file: " << output << endl;
       cout << "MinDzeroPT: " << MinDzeroPT << endl;
       cout << "MaxDzeroPT: " << MaxDzeroPT << endl;
       cout << "MinDzeroY: " << MinDzeroY << endl;
       cout << "MaxDzeroY: " << MaxDzeroY << endl;
       cout << "MaxDalpha: " << MaxDalpha << endl;
       cout << "Scale factor: " << scaleFactor << endl;
       cout << "Number of Threads: " << nThread << endl;
       cout << "Process the Nth chunk: " << nChunk << endl;

   }
};

void saveParametersToHistograms(const Parameters& par, TFile* outf) {
    outf->cd();  // Navigate to the output file directory
    outf->mkdir("par"); // Create a directory named "par"
    outf->cd("par"); // Change to the "par" directory

    // Create and fill histograms for each parameter
    TH1D* hMinDzeroPT = new TH1D("parMinDzeroPT", "parMinDzeroPT", 1, 0, 1);  
    hMinDzeroPT->SetBinContent(1, par.MinDzeroPT);
    TH1D* hMaxDzeroPT = new TH1D("parMaxDzeroPT", "parMaxDzeroPT", 1, 0, 1);
    hMaxDzeroPT->SetBinContent(1, par.MaxDzeroPT);
    TH1D* hMinDzeroY = new TH1D("parMinDzeroY", "parMinDzeroY", 1, 0, 1);
    hMinDzeroY->SetBinContent(1, par.MinDzeroY);
    TH1D* hMaxDzeroY = new TH1D("parMaxDzeroY", "parMaxDzeroY", 1, 0, 1);
    hMaxDzeroY->SetBinContent(1, par.MaxDzeroY);
    TH1D* hMaxDalpha = new TH1D("parMaxDalpha", "parMaxDalpha", 1, 0, 1);
    hMaxDalpha->SetBinContent(1, par.MaxDalpha);
    TH1D* hScaleFactor = new TH1D("parScaleFactor", "parScaleFactor", 1, 0, 1);
    hScaleFactor->SetBinContent(1, par.scaleFactor);
    TH1D* hNThread = new TH1D("parNThread", "parNThread", 1, 0, 1);
    hNThread->SetBinContent(1, par.nThread); 
    TH1D* hNChunk = new TH1D("parNChunk", "parNChunk", 1, 0, 1);
    hNChunk->SetBinContent(1, par.nChunk);
    
    // Write histograms to the output file
    hMinDzeroPT->Write();
    hMaxDzeroPT->Write();
    hMinDzeroY->Write();
    hMaxDzeroY->Write();
    hMaxDalpha->Write();
    hScaleFactor->Write();

    // Clean up
    delete hMinDzeroPT;
    delete hMaxDzeroPT;
    delete hMinDzeroY;
    delete hMaxDzeroY;
    delete hMaxDalpha;
    delete hScaleFactor;
}
