

void Zchecks(){

    TFile* f = TFile::Open("smallskims.root");

    TTree* T_hi = (TTree*)f->Get("Tree_HighEG");
    TTree* T_lo = (TTree*)f->Get("Tree_LowEG");
    TTree* T_mc = (TTree*)f->Get("Tree_MC");


    



}
