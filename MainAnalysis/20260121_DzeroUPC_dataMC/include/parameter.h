#include <filesystem>
namespace fs = std::filesystem;

//============================================================//
// Define analysis parameters
//============================================================//
class Parameters {
public:
    Parameters( float MinDzeroPT, float MaxDzeroPT, float MinDzeroY, float MaxDzeroY, bool IsGammaN,
                int TriggerChoice, int BkgFilterChoice, bool IsData, int Year = 0, float scaleFactor = 1.0,
                int in_DoSystRapGap = 0, int in_DoSystD = 0,
                bool in_DoGptGyReweighting = false, string in_GptGyWeightFileName = "",
                bool in_DoMultReweighting = false, string in_MultWeightFileName = "" ,
                float in_HFMaxRapDefn = -1, bool in_DoTrkPtErrFilter = true, bool in_DoTrkHitFilter = true,
                bool in_DoBDTD = false, float in_BDTCutValue = 0.f,
                bool in_MCInclusiveOnlyBaseCuts = false,
                int in_DextraSelectionMask = 0)
	: MinDzeroPT(MinDzeroPT), MaxDzeroPT(MaxDzeroPT), MinDzeroY(MinDzeroY), MaxDzeroY(MaxDzeroY), IsGammaN(IsGammaN), TriggerChoice(TriggerChoice), BkgFilterChoice(BkgFilterChoice),  IsData(IsData), Year(Year), scaleFactor(scaleFactor)
    {
        // Check DoSystRapGap
        if (in_DoSystRapGap > 9) {
            // Custom HF energy threshold will be set to in_DoSystRapGap/10.
            printf("[INFO] Using custom rapidity gap energy threshold! (option DoSystRapGap > 9)");
            DoSystRapGap = in_DoSystRapGap;
        }
        else if (in_DoSystRapGap!=0 && in_DoSystRapGap != 1 && in_DoSystRapGap != -1)
        {
            printf("[Error] Couldn't recognize the option DoSystRapGap=%d (should be 0 = nominal, 1 = tight, -1: loose). Exiting...\n", in_DoSystRapGap);
            exit(1);
        }
        else { DoSystRapGap = in_DoSystRapGap; }

        // Check DoSystD (-1 = BDT working-point mode; requires DoBDTD)
        if (in_DoSystD != 0 && in_DoSystD != 1 && in_DoSystD != 2 \
                          && in_DoSystD != 3 && in_DoSystD != 4 && in_DoSystD != -1)
        {
            printf("[Error] Couldn't recognize the option DoSystD=%d (should be 0 = nominal, 1 = Dsvpv variation, 2: DtrkPt variation, 3: Dalpha variation, 4: Dchi2cl variation, -1: BDT mode). Exiting...\n", in_DoSystD);
            exit(1);
        }
        if (in_DoSystD == -1 && !in_DoBDTD)
        {
            printf("[Error] DoSystD=-1 requires DoBDTD true. Exiting...\n");
            exit(1);
        }
        DoSystD = in_DoSystD;
        DoBDTD = in_DoBDTD;
        BDTCutValue = in_BDTCutValue;

        // Check DoGptGyReweighting
        if (in_DoGptGyReweighting && fs::exists(in_GptGyWeightFileName))
        {
            DoGptGyReweighting  = true;
            GptGyWeightFileName = in_GptGyWeightFileName;
        } else {
            DoGptGyReweighting  = false;
            GptGyWeightFileName = "";
        }

        // Check DoMultReweighting
        if (in_DoMultReweighting && fs::exists(in_MultWeightFileName))
        {
            DoMultReweighting  = true;
            MultWeightFileName = in_MultWeightFileName;
        } else {
            DoMultReweighting  = false;
            MultWeightFileName = "";
        }
        
        // Check HFMaxRapDefn: 5 = eta5 branches, 6 = forest branches, 5.2 = default HFEMaxPlus/Minus, -1 = unspecified
        if (in_HFMaxRapDefn == 5 || in_HFMaxRapDefn == 5.2f || in_HFMaxRapDefn == 6)
        {
            HFMaxRapDefn = in_HFMaxRapDefn;
        } else if (in_HFMaxRapDefn == -1) {
            cout << "[Warning] Using unspecified definition for HFEMax. Max rapidity of HF was likely determined by forest settings." << endl;
            HFMaxRapDefn = in_HFMaxRapDefn;
        } else {
            printf("[Error] Couldn't recognize the option HFMaxRapDefn=%g (should be 5 = eta5, 6 = forest, 5.2 = default branches, or -1). Exiting...\n", (double)in_HFMaxRapDefn);
            exit(1);
        }
        
        DoTrkPtErrFilter = in_DoTrkPtErrFilter;
        DoTrkHitFilter = in_DoTrkHitFilter;
        MCInclusiveOnlyBaseCuts = in_MCInclusiveOnlyBaseCuts && !IsData;
        if (in_MCInclusiveOnlyBaseCuts && IsData) {
            cout << "[Warning] MCInclusiveOnlyBaseCuts applies only to MC; ignored for data." << endl;
        }
        if (in_DextraSelectionMask < 0 || in_DextraSelectionMask > 15) {
            printf("[Error] DextraSelectionMask must be in [0,15] (bit0=chi2>0.1, bit1=open_angle, bit2=DLS>2.5, bit3=trk pT>1). Got %d. Exiting...\n", in_DextraSelectionMask);
            exit(1);
        }
        DextraSelectionMask = in_DextraSelectionMask;
    }
    Parameters() {}
   string input;          // Input file name
   string output;         // Output file name
   int Year;              // Year of sample data
   float MinDzeroPT;      // Lower limit of Dzero pt
   float MaxDzeroPT;      // Upper limit of Dzero pt
   float MinDzeroY;       // Lower limit of Dzero rapidity
   float MaxDzeroY;       // Upper limit of Dzero rapidity
   bool IsGammaN;         // GammaN analysis (or NGamma)
   int TriggerChoice;     // 0 = no trigger sel, 1 = isL1ZDCOr, 2 = isL1ZDCXORJet8, 3 = isL1ZDCXORJet12, 4 = isL1ZDCXORJet16
   int BkgFilterChoice;   // 1 = CCF + halo, 2 = halo only
   bool IsData;           // Data or MC
   float scaleFactor;     // Scale factor
   int DoSystRapGap;      // Systematic study: apply the alternative event selections
                          // 0 = nominal, 1 = tight, -1: loose
                          // 9 < DoSystRapGap: use custom HF energy threshold, the threshold value will be DoSystRapGap/10.
   int DoSystD;           // Systematic study: apply the alternative D selections
                          // 0 = nominal, 1 = Dsvpv variation, 2: DtrkPt variation
                          // 3 = Dalpha variation, 4: Dchi2cl variation
                          // -1 = use BDT cut (DoBDTD); no DpassCut23* syst variation
   bool DoBDTD;           // Apply Dmva_BDT cut at BDTCutValue (sets DoSystD to -1)
   float BDTCutValue;     // BDT classifier cut when DoBDTD is true
   bool DoGptGyReweighting;      // MC reweighting:: Gpt, Gy
   string GptGyWeightFileName;   // MC reweighting:: Gpt, Gy correction factor
   bool DoMultReweighting;       // MC reweighting:: Mult
   string MultWeightFileName;    // MC reweighting:: Mult correction factor
   float HFMaxRapDefn;    // HF E_max branches: 5 = eta5, 6 = forest, 5.2 = HFEMaxPlus/Minus, -1 = unspecified
   bool DoTrkPtErrFilter; // Filter on DtrkPtErr / DtrkPt
   bool DoTrkHitFilter;   // Filter on track pixel and strip hits
   /** When true and IsData is false (MC_inclusive jobs): skip passesBaseCutEvent and passesBaseCutCandidate.
    *  Still apply eventSelection, then pt/y binning, then DpassCut23/BDT/extra trk cuts as configured. */
   bool MCInclusiveOnlyBaseCuts;
   /** Optional extra D cuts after base candidate (bitwise OR): 1=chi2>0.1, 2=open_angle(|Dy| vs Dalpha/Ddtheta), 4=DLS>2.5, 8=Dtrk1/2 pT>1 GeV. 0=off. */
   int DextraSelectionMask;

   int nThread;           // Number of Threads
   int nChunk;            // Process the Nth chunk
   void printParameters() const {
       cout << "Input file: " << input << endl;
       cout << "Output file: " << output << endl;
       cout << "Year: " << Year << endl;
       cout << "MinDzeroPT: " << MinDzeroPT << endl;
       cout << "MaxDzeroPT: " << MaxDzeroPT << endl;
       cout << "MinDzeroY: " << MinDzeroY << endl;
       cout << "MaxDzeroY: " << MaxDzeroY << endl;
       cout << "IsGammaN: " << IsGammaN << endl; 
       cout << "TriggerChoice: " << ((TriggerChoice==0)? "None" :
                                     (TriggerChoice==1)? "isL1ZDCOr" :
                                     (TriggerChoice==2)? "isL1ZDCXORJet8" :
                                     (TriggerChoice==3)? "isL1ZDCXORJet12" :
                                     (TriggerChoice==4)? "isL1ZDCXORJet16" : "")
                                 << endl;
       cout << "BkgFilterChoice: " << ((TriggerChoice==1)? "ClusterCompatibilityFilter + cscTightHalo2015Filter" :
                                       (TriggerChoice==2)? "cscTightHalo2015Filter only" : "")
                                   << endl;
       cout << "IsData: " << IsData << endl;
       cout << "Scale factor: " << scaleFactor << endl;
       cout << "DoSystRapGap: " << ((DoSystRapGap==0)? "No" :
                                    (DoSystRapGap==1)? "Tight" :
                                    (DoSystRapGap > 9)? Form("Custom threshold (%.1f)", ((float) DoSystRapGap)/10.) : "Loose")
                                << endl;
       cout << "DoSystD: "      << ((DoSystD==0)? "No" :
                                    (DoSystD==1)? "Dsvpv variation" :
                                    (DoSystD==2)? "DtrkPt variation" :
                                    (DoSystD==3)? "Dalpha variation" :
                                    (DoSystD==4)? "Dchi2cl variation" :
                                    (DoSystD==-1)? "BDT mode (no DpassCut23 syst)" : "unknown")
                                << endl;
       cout << "DoBDTD: " << DoBDTD << endl;
       cout << "BDTCutValue: " << BDTCutValue << endl;
       cout << "DoGptGyReweighting: " << DoGptGyReweighting << endl;
       cout << "GptGyWeightFileName: " << GptGyWeightFileName << endl;
       cout << "DoMultReweighting: " << DoMultReweighting << endl;
       cout << "MultWeightFileName: " << MultWeightFileName << endl;
       cout << "HFMaxRapDefn: " << HFMaxRapDefn << endl;
       cout << "DoTrkPtErrFilter: " << DoTrkPtErrFilter << endl;
       cout << "DoTrkHitFilter: " << DoTrkHitFilter << endl;
       cout << "MCInclusiveOnlyBaseCuts: " << MCInclusiveOnlyBaseCuts << endl;
       cout << "DextraSelectionMask: " << DextraSelectionMask
            << " (1=chi2>0.1, 2=open_angle, 4=DLS>2.5, 8=trkPt>1)" << endl;
       cout << "Number of Threads: " << nThread << endl;
       cout << "Process the Nth chunk: " << nChunk << endl;

   }
};

void saveParametersToHistograms(const Parameters& par, TFile* outf) {
    outf->cd();  // Navigate to the output file directory
    outf->mkdir("par"); // Create a directory named "par"
    outf->cd("par"); // Change to the "par" directory

    // Create and fill histograms for each parameter
    TH1D* hYear = new TH1D("parYear", "parYear", 1, 0, 1);
    hYear->SetBinContent(1, par.Year);
    TH1D* hMinDzeroPT = new TH1D("parMinDzeroPT", "parMinDzeroPT", 1, 0, 1);
    hMinDzeroPT->SetBinContent(1, par.MinDzeroPT);
    TH1D* hMaxDzeroPT = new TH1D("parMaxDzeroPT", "parMaxDzeroPT", 1, 0, 1);
    hMaxDzeroPT->SetBinContent(1, par.MaxDzeroPT);
    TH1D* hMinDzeroY = new TH1D("parMinDzeroY", "parMinDzeroY", 1, 0, 1);
    hMinDzeroY->SetBinContent(1, par.MinDzeroY);
    TH1D* hMaxDzeroY = new TH1D("parMaxDzeroY", "parMaxDzeroY", 1, 0, 1);
    hMaxDzeroY->SetBinContent(1, par.MaxDzeroY);
    TH1D* hIsGammaN = new TH1D("parIsGammaN", "parIsGammaN", 1, 0, 1);
    hIsGammaN->SetBinContent(1, par.IsGammaN);
    TH1D* hTriggerChoice = new TH1D("parTriggerChoice", "parTriggerChoice", 1, 0, 1);
    hTriggerChoice->SetBinContent(1, par.TriggerChoice);
    TH1D* hBkgFilterChoice = new TH1D("parBkgFilterChoice", "parBkgFilterChoice", 1, 0, 1);
    hBkgFilterChoice->SetBinContent(1, par.BkgFilterChoice);
    TH1D* hIsData = new TH1D("parIsData", "parIsData", 1, 0, 1);
    hIsData->SetBinContent(1, par.IsData);
    TH1D* hScaleFactor = new TH1D("parScaleFactor", "parScaleFactor", 1, 0, 1);
    hScaleFactor->SetBinContent(1, par.scaleFactor);
    TH1D* hDoSystRapGap = new TH1D("parDoSystRapGap", "parDoSystRapGap", 1, 0, 1);
    hDoSystRapGap->SetBinContent(1, par.DoSystRapGap);
    TH1D* hDoSystD = new TH1D("parDoSystD", "parDoSystD", 1, 0, 1);
    hDoSystD->SetBinContent(1, par.DoSystD);
    TH1D* hDoBDTD = new TH1D("parDoBDTD", "parDoBDTD", 1, 0, 1);
    hDoBDTD->SetBinContent(1, par.DoBDTD);
    TH1D* hBDTCutValue = new TH1D("parBDTCutValue", "parBDTCutValue", 1, 0, 1);
    hBDTCutValue->SetBinContent(1, par.BDTCutValue);
    TH1D* hDoGptGyReweighting = new TH1D("parDoGptGyReweighting", "parDoGptGyReweighting", 1, 0, 1);
    hDoGptGyReweighting->SetBinContent(1, par.DoGptGyReweighting);
    TH1D* hDoMultReweighting = new TH1D("parDoMultReweighting", "parDoMultReweighting", 1, 0, 1);
    hDoMultReweighting->SetBinContent(1, par.DoMultReweighting);
    TH1D* hHFMaxRapDefn = new TH1D("parHFMaxRapDefn", "parHFMaxRapDefn", 1, 0, 1);
    hHFMaxRapDefn->SetBinContent(1, par.HFMaxRapDefn);
    TH1D* hDoTrkPtErrFilter = new TH1D("parDoTrkPtErrFilter", "parDoTrkPtErrFilter", 1, 0, 1);
    hDoTrkPtErrFilter->SetBinContent(1, par.DoTrkPtErrFilter);
    TH1D* hDoTrkHitFilter = new TH1D("parDoTrkHitFilter", "parDoTrkHitFilter", 1, 0, 1);
    hDoTrkHitFilter->SetBinContent(1, par.DoTrkHitFilter);
    TH1D* hMCInclusiveOnlyBaseCuts = new TH1D("parMCInclusiveOnlyBaseCuts", "parMCInclusiveOnlyBaseCuts", 1, 0, 1);
    hMCInclusiveOnlyBaseCuts->SetBinContent(1, par.MCInclusiveOnlyBaseCuts);
    TH1D* hDextraSelectionMask = new TH1D("parDextraSelectionMask", "parDextraSelectionMask", 1, 0, 1);
    hDextraSelectionMask->SetBinContent(1, par.DextraSelectionMask);

    // Write histograms to the output file
    hYear->Write();
    hMinDzeroPT->Write();
    hMaxDzeroPT->Write();
    hMinDzeroY->Write();
    hMaxDzeroY->Write();
    hIsGammaN->Write();
    hTriggerChoice->Write();
    hBkgFilterChoice->Write();
    hIsData->Write();
    hScaleFactor->Write();
    hDoSystRapGap->Write();
    hDoSystD->Write();
    hDoBDTD->Write();
    hBDTCutValue->Write();
    hDoGptGyReweighting->Write();
    hDoMultReweighting->Write();
    hHFMaxRapDefn->Write();
    hDoTrkPtErrFilter->Write();
    hDoTrkHitFilter->Write();
    hMCInclusiveOnlyBaseCuts->Write();
    hDextraSelectionMask->Write();

    // Clean up
    delete hYear;
    delete hMinDzeroPT;
    delete hMaxDzeroPT;
    delete hMinDzeroY;
    delete hMaxDzeroY;
    delete hIsGammaN;
    delete hTriggerChoice;
    delete hBkgFilterChoice;
    delete hIsData;
    delete hScaleFactor;
    delete hDoSystRapGap;
    delete hDoSystD;
    delete hDoBDTD;
    delete hBDTCutValue;
    delete hDoGptGyReweighting;
    delete hDoMultReweighting;
    delete hHFMaxRapDefn;
    delete hDoTrkPtErrFilter;
    delete hDoTrkHitFilter;
    delete hMCInclusiveOnlyBaseCuts;
    delete hDextraSelectionMask;
}
