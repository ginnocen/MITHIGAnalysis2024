


void cutdownskims(){

    // Open input file
    TFile* f_higheg = TFile::Open("/data00/g2ccbar/data2018/highEGfull.root");
    TFile* f_loweg = TFile::Open("/data00/g2ccbar/data2018/lowEGfull.root");
    TFile* f_mc = TFile::Open("/data00/g2ccbar/mc2018/skim_011226_full.root");


    TTree* tree = (TTree*)f_higheg->Get("Tree");
    TTree* tree2 = (TTree*)f_loweg->Get("Tree");
    TTree* tree_mc = (TTree*)f_mc->Get("Tree");

    // Create output file
    TFile* f_output = new TFile("output.root", "RECREATE");

    // Define your cut condition
    const char* cut_higheg = "HLT_HIAK4PFJet80_v1 && IsMuMuTagged";
    const char* cut_loweg = "HLT_HIAK4PFJet60_v1 && IsMuMuTagged";
    const char* cut_mc = "IsMuMuTagged || GenIsMuMuTagged";

    // Copy only events passing the cut
    TTree* tree_filtered = tree->CopyTree(cut_higheg);
    cout << "good"  << endl;
    TTree* tree2_filtered = tree2->CopyTree(cut_loweg);
    cout << "good2"  << endl;
    TTree* tree_mc_filtered = tree_mc->CopyTree(cut_mc);
    cout << "good3"  << endl;

    cout << "Original entries: " << tree->GetEntries() << endl;
    cout << "Filtered entries: " << tree_filtered->GetEntries() << endl;
    cout << "Original entries (low EG): " << tree2->GetEntries() << endl;
    cout << "Filtered entries (low EG): " << tree2_filtered->GetEntries() << endl;
    cout << "Original entries (MC): " << tree_mc->GetEntries() << endl;
    cout << "Filtered entries (MC): " << tree_mc_filtered->GetEntries() << endl;

    // Write to output file
    tree_filtered->Write("Tree_HighEG");
    tree2_filtered->Write("Tree_LowEG");
    tree_mc_filtered->Write("Tree_MC");
    f_output->Close();

    cout << "Filtered tree saved to output.root" << endl;

}