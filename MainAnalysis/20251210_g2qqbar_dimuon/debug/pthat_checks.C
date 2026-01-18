

void pthat_checks(){

    TFile*f = new TFile("/data00/g2ccbar/mc2018/skim_010526_soft_0/mergedfile.root");
    TTree*T = (TTree*)f->Get("Tree");

    float EventWeight = 0;
    float pthat = 0;
    int nCh = 0;
    int nBh = 0;
    bool isMuMuTagged = false;
    T->SetBranchAddress("pthat",&pthat);
    T->SetBranchAddress("EventWeight",&EventWeight);
    T->SetBranchAddress("ncHad",&nCh);
    T->SetBranchAddress("nbHad",&nBh);
    T->SetBranchAddress("IsMuMuTagged",&isMuMuTagged);

    TH1D*h_pthat = new TH1D("h_pthat","h_pthat",100,-5,1);
    TH1D*h_pthat_mumu = new TH1D("h_pthat_mumu","h_pthat_mumu",100,-5,1);
    TH1D*h_pthat_LF = new TH1D("h_pthat_LF","h_pthat_LF",100,-5,1);
    TH1D*h_pthat_cc = new TH1D("h_pthat_cc","h_pthat_cc",100,-5,1);
    TH1D*h_pthat_bb = new TH1D("h_pthat_bb","h_pthat_bb",100,-5,1);
    TH1D*h_pthat_c = new TH1D("h_pthat_c","h_pthat_c",100,-5,1);
    TH1D*h_pthat_b = new TH1D("h_pthat_b","h_pthat_b",100,-5,1);

    for(int i=0;i<T->GetEntries();i++){
        
        T->GetEntry(i);
       

        h_pthat->Fill(pthat);
        
        if(isMuMuTagged){

            h_pthat_mumu->Fill(pthat);


        }
        




    }


}