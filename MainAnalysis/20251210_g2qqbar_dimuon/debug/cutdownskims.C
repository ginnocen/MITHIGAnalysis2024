


void cutdownskims(){

    // Open input file
    //TFile* f = TFile::Open("/data00/g2ccbar/data2018/highEGfull.root");
    //TFile* f = TFile::Open("/data00/g2ccbar/data2018/lowEGfull.root");
    TString inputFileName = "/data00/g2ccbar/mc2018/pythia_0210_softmvaID.root";
    TFile* f = TFile::Open(inputFileName);

    // Create output file
    TFile* f_output = new TFile("pythiamini_softmvaID.root", "RECREATE");
    bool isData = false;
    
    TTree* T = (TTree*)f->Get("Tree");

    string selection;
    if(isData){
        selection = "(HLT_HIAK4PFJet80 || HLT_HIAK4PFJet60 || HLT_HIAK4PFJet40) && IsMuMuTagged";
    }
    else{
        selection = "IsMuMuTagged || GenIsMuMuTagged";
    }

    // Copy only events passing the cut
    TTree* tree_filtered = T->CopyTree(selection.c_str());
    cout << "done filtering" << endl;

    // Write to output file
    tree_filtered->Write("Tree");
    
    // Save original filename as TNamed
    TNamed* originalFilename = new TNamed("OriginalFileName", inputFileName.Data());
    originalFilename->Write();
    
    f_output->Close();

    cout << "Filtered tree saved to output.root" << endl;

}