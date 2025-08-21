#include <fstream>
#include <string>
#include <vector>
#include <TChain.h>

void histfromtree(TTree* T, TCut t, TH1D* h, const char* branch){
     cout << T->GetEntries(t) << endl;
     T->Project(h->GetName(), branch, t);
     h->Scale(1.0/h->Integral());
}

TChain* createDataChain(const char* dataSource) {
    TChain* chain = new TChain("Tree");
    string source(dataSource);
    if (source.find(".txt") != string::npos) {
        ifstream infile(dataSource);
        string line;
        int fileCount = 0;
        while (getline(infile, line)) {
            if (!line.empty() && line[0] != '#') {
                fileCount++;
                int result = chain->Add(line.c_str());
            }
        }
    }
    return chain;
}

TCut parseVZcut(TCut cut) {

     ///// PARSE THE NECESSARY VARIABLES FROM USER SUPPLIED TCUT /////
     /*
      - PVFILTER
      - CCFILTER
      - HF FILTER(S) online + offline
      I.E. EVERYTHING EXCEPT THE VZ CUT ITSELF
     */

     TString cutString = cut.GetTitle();
     
     cutString.ReplaceAll("(VZ > -15 && VZ < 15) && ", "");
     cutString.ReplaceAll(" && (VZ > -15 && VZ < 15)", "");
     cutString.ReplaceAll("(VZ > -15 && VZ < 15)", "");
     cutString.ReplaceAll("     ", " ");  // Replace multiple spaces with single space
     cutString.ReplaceAll("    ", " ");   // Replace 4 spaces with single space
     cutString.ReplaceAll("   ", " ");    // Replace 3 spaces with single space
     cutString.ReplaceAll("  ", " ");     // Replace 2 spaces with single space
     cutString.ReplaceAll(" && &&", " &&");
     if (cutString.BeginsWith("&& ")) {
         cutString.Remove(0, 3);
     }
     if (cutString.EndsWith(" &&")) {
         cutString.Remove(cutString.Length() - 3, 3);
     }
     cutString = cutString.Strip(TString::kBoth);
     
     return TCut(cutString.Data());
}

void VZReweight(
                TCut Datacut,
                TCut MCcut,
                const char* dataSource = "datafiles.txt",
                const char* ooskim = "/data00/OOsamples/Skims20250704/skim_HiForest_250520_Hijing_MinimumBias_b015_OO_5362GeV_250518.root",
                const char* ooskim_arg = "/data00/OOsamples/Skims20250704/20250704_HiForest_250520_Pythia_Angantyr_OO_OO_5362GeV_250626.root",
                int doreweight = 1
               ){
    
    cout << "STARTING VZ REWEIGHT" << endl;
    
    // Enable ROOT multithreading for faster processing
    ROOT::EnableImplicitMT();

    ///// READ FILES /////
    TChain* chainData = createDataChain(dataSource);
    //chainData->GetListOfBranches()->Print();
    TFile* fOO = TFile::Open(ooskim); 
    TFile* fOO_Arg = TFile::Open(ooskim_arg); 
    
    cout << "PROCESSING FILES:" << endl;
    cout << "Data: " << dataSource << endl;
    cout << "OO: " << ooskim << endl;
    cout << "OO Arg: " << ooskim_arg << endl;

    TTree* tOO = (TTree*)fOO->Get("Tree");
    TTree* tOO_Arg = (TTree*)fOO_Arg->Get("Tree");

    /////// GET SELECTION CUTS FOR REWEIGHT HISTOGRAMS PRE-RATIO //////
     TCut cut_MC = parseVZcut(MCcut);
     TCut cut_DATA = parseVZcut(Datacut);

     cout << "SELECTION FOR DATA VZ HIST: " << endl;
     cout << cut_DATA.GetTitle() << endl;
     cout << "SELECTION FOR MC VZ HIST: " << endl;
     cout << cut_MC.GetTitle() << endl;

    //////// OBTAIN HISTOGRAMS FOR VZ INFO ////////
    // Z vertex shift correction for data
    const double vzShift = +0.366387;
    cout << "Applying VZ shift correction to data: " << vzShift << " cm" << endl;
    
    TH1D* hDataVZ = new TH1D("hDataVZ", "VZ Distribution Data (shift corrected)", 50, -30, 30);
    histfromtree(chainData, cut_DATA, hDataVZ, Form("VZ + %f", vzShift));

    TH1D* hMCVZ = new TH1D("hMCVZ", "VZ Distribution MC", 50, -30, 30);
    histfromtree(tOO, cut_MC, hMCVZ, "VZ");

    TH1D* hMCARGVZ = new TH1D("hMCARGVZ", "VZ Distribution MC Arg", 50, -30, 30);
    histfromtree(tOO_Arg, cut_MC, hMCARGVZ, "VZ");

    //////// DIVIDE-- WANT TO SCALE MC TO DATA ////////
    TH1D* reweight = (TH1D*)hDataVZ->Clone("VZReweight");
    reweight->SetTitle("VZ Reweight Factor (Data/MC)");
    reweight->Divide(hMCVZ);

    TH1D* reweight_Arg = (TH1D*)hDataVZ->Clone("VZReweight_Arg");
    reweight_Arg->SetTitle("VZ Reweight Factor (Data/MC Arg)");
    reweight_Arg->Divide(hMCARGVZ);

    ///////// DRAW REWEIGHT HISTOGRAM ///////
    gStyle->SetOptStat(0);
    TCanvas* c = new TCanvas("c", "VZ Reweight Factor", 800, 600);
    reweight->GetXaxis()->SetTitle("VZ (cm)");
    reweight->GetYaxis()->SetTitle("Reweight Factor (Data/MC)");
    reweight->SetLineColor(kBlue);
    reweight->SetLineWidth(2);
    reweight->Draw("HIST");

    reweight_Arg->SetLineColor(kOrange+7);
    reweight_Arg->SetLineWidth(2);
    reweight_Arg->Draw("HIST SAME");

    TLegend* leg = new TLegend(0.3, 0.7, 0.7, 0.9);
    leg->AddEntry(reweight, "Data/MC VZ HIJING", "l");
    leg->AddEntry(reweight_Arg, "Data/MC VZ Angantyr", "l");
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->Draw();
    
    // Add horizontal line at y=1
    TLine* line = new TLine(-30, 1, 30, 1);
    line->SetLineColor(kRed);
    line->SetLineStyle(2);
    line->Draw("same");

    ///////// DRAW VZ DISTRIBUTIONS ///////
    TCanvas* c2 = new TCanvas("c2", "VZ Distributions", 800, 600);
    gPad->SetLogy();
    
    // Set colors and styles for the distributions
    hDataVZ->SetLineColor(kBlack);
    hDataVZ->SetLineWidth(2);
    hDataVZ->SetMarkerStyle(20);
    hDataVZ->SetMarkerSize(0.8);
    hDataVZ->GetXaxis()->SetTitle("VZ (cm)");
    hDataVZ->GetYaxis()->SetTitle("Normalized Events");
    hDataVZ->SetTitle("VZ Distributions");
    
    hMCVZ->SetLineColor(kBlue);
    hMCVZ->SetLineWidth(2);
    
    hMCARGVZ->SetLineColor(kOrange);
    hMCARGVZ->SetLineWidth(2);
    
    // Find maximum for proper scaling
    double maxVal = hMCVZ->GetMaximum();
    hDataVZ->SetMaximum(maxVal * 1.5);
    
    hDataVZ->Draw("PE");
    hMCVZ->Draw("HIST SAME");
    hMCARGVZ->Draw("HIST SAME");
    
    TLegend* leg2 = new TLegend(0.65, 0.7, 0.85, 0.9);
    leg2->AddEntry(hDataVZ, "Data", "pe");
    leg2->AddEntry(hMCVZ, "MC HIJING", "l");
    leg2->AddEntry(hMCARGVZ, "MC Angantyr", "l");
    leg2->SetBorderSize(0);
    leg2->SetFillStyle(0);
    leg2->Draw();

    // Save the reweight histogram and canvas
    TFile* fout = TFile::Open("VZReweight.root", "RECREATE");
    reweight->Write();
    reweight_Arg->Write();
    c->Write();
    c2->Write();
    fout->Close();
    c->SaveAs("VZReweight.pdf");
    c2->SaveAs("VZDistributions.pdf");

    cout << "DONE WITH VZ REWEIGHT" << endl;
}