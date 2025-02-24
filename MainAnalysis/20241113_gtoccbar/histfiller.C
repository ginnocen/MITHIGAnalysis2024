#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>

using namespace std;

struct Parameter {
   string name;
    double lo;
    double hi;
};

vector<Parameter> selection(string selectionfile){
    
    ifstream infile(selectionfile);
    vector<Parameter> parameters;
   string line;
    while (getline(infile, line)) {
       string paramName;
        double value1, value2;
       stringstream ss(line);
       ss >> paramName >> value1 >> value2;
        parameters.push_back({paramName, value1, value2});
    }
    infile.close();
    return parameters;
}

class Hist{

    
    public:
    const char* feature;
    const char* infile;
    const char* selectionfile;
    const char* tree;
    const char* name;
    const char* xlabel;
    const char* ylabel;
    double bbins;
    double blow;
    double bhi;
    int color;
    const char* legend;
    double xlow;
    double xhi;
    double ylow;
    double yhi;
    int logx;
    float collweight;

    Hist(const char* f, const char* in, const char* s, const char* tr, const char* na, const char* xl, const char* yl, double bbi,double blo, double bhigh, int col, const char* leg, double xlo, double xhigh, double ylo, double yhigh, int lx, float ncweight) : feature(f), infile(in), selectionfile(s), tree(tr), name(na), xlabel(xl), ylabel(yl), bbins(bbi), blow(blo), bhi(bhigh), color(col), legend(leg), xlow(xlo), xhi(xhigh), ylow(ylo), yhi(yhigh), logx(lx), collweight(ncweight) {}


    TH1D* histogramfill(){

        // open files
        TFile* f = TFile::Open(infile, "READ");
        TTree* T;
        T = (TTree*)f->Get(tree);

        // necessary objects
        TH1D* h = new TH1D("name","name",bbins,blow,bhi);
        vector<Parameter> s = selection(selectionfile);
        Parameter limits;
        vector<float> values(s.size());
        float treedata = 0;
        int isgood = 1;
        int issamebranchasfeature = -1;
        float ncollweight = 0;
        float eventweight = 0;
        float leadingpt = 0;
        float badcount = 0;
       

        
        //cout << feature << endl;

        //Set Addresses 
        T->SetBranchAddress(feature,&treedata);
        T->SetBranchAddress("NCollWeight",&ncollweight);
        T->SetBranchAddress("EventWeight",&eventweight);
        T->SetBranchAddress("leadingPT",&leadingpt);
    

        for(int i = 0; i<s.size(); i++){
            //cout << (s[i].name).c_str() << endl;
            T->SetBranchAddress((s[i].name).c_str(),&values[i]);
            //overlap precaution
            if(s[i].name == feature){
                issamebranchasfeature = i;
                //cout << "same branch as feature: " << issamebranchasfeature << endl; 
            }
        }

        //main loop
        for(int i = 0; i< T->GetEntries(); i++){
            T->GetEntry(i);
            if(i%10000 == 0){
            cout << 1.0*i/T->GetEntries();
            cout.flush(); 
            cout << "\b\b\b\b\b\b\b\b\b\b\b        \b\b\b\b\b\b\b\b\b\b\b";
            }
            
            if((eventweight*ncollweight > 5)&&(collweight > 0)){
                badcount += 1;
                continue;
            }
            //cout << leadingpt << endl;
            //if((log10(eventweight) > 1.0 - 0.0015*leadingpt - log((leadingpt-100)/1.5))&&(collweight > 0)){
            //    cout << "ping" << endl;
            //    continue;
            //}
            //if(ncollweight > 800){
            //    continue;
            //}
        

            if(issamebranchasfeature >= 0){
                treedata = values[issamebranchasfeature];
            }

            //cout << treedata << endl;

            for(int j = 0; j<s.size(); j++){
                limits = s[j];
                if((values[j] > limits.hi)||(values[j] < limits.lo)){
                    isgood = 0;
                    break;
                };  
            }
            if(isgood == 0){
                isgood = 1;
                continue;
            }
            if(logx == 0){
            h->Fill(treedata,pow(ncollweight*eventweight,collweight));
            //h->Fill(treedata);
            }
            else{
                h->Fill(log10(abs(treedata)),pow(ncollweight*eventweight,collweight));
            }
        }        

        //cosmetic
        h->SetStats(0);
        h->SetLineWidth(3);
        h->SetLineColor(color);
        h->SetFillColorAlpha(color,0.1);
        h->SetTitle(name);
        h->GetXaxis()->SetTitle(xlabel);
        h->GetYaxis()->SetTitle(ylabel);
        //h->GetXaxis()->SetTitleOffset(1.555);
        if(xhi != xlow){h->GetXaxis()->SetRangeUser(xlow, xhi);}
        if(yhi != ylow){h->GetYaxis()->SetRangeUser(ylow, yhi);}
        cout << badcount << " " <<badcount/T->GetEntries() << endl;
        cout << endl;
        return h;  
    }


};

class Hist2d{

    public:
    const char* feature1;
    const char* feature2;
    const char* infile;
    const char* selectionfile;
    const char* tree;
    const char* name;
    const char* xlabel;
    const char* ylabel;
    const char* zlabel;
    double bbins;
    double blow;
    double bhi;
    double hbins;
    double hlow;
    double hhi;
    double xlow;
    double xhi;
    double ylow;
    double yhi;
    double zlow;
    double zhi;
    int logx;
    int logy;
    float collweight;

    Hist2d(const char* f1, const char* f2, const char* in, const char* s, const char* tr, const char* na, const char* xl, const char* yl, const char* zl, double bbi,double blo, double bhigh, double hbi,double hlo, double hhigh, double xlo, double xhigh, double ylo, double yhigh, double zlo, double zhigh, int lx, int ly, float ncw) 
    : feature1(f1), feature2(f2), infile(in), selectionfile(s), tree(tr), name(na), xlabel(xl), ylabel(yl), zlabel(zl), bbins(bbi), blow(blo), bhi(bhigh), hbins(hbi), hlow(hlo), hhi(hhigh), xlow(xlo), xhi(xhigh), ylow(ylo), yhi(yhigh),zlow(zlo), zhi(zhigh), logx(lx), logy(ly), collweight(ncw) {}


    TH2D* histogramfill(){
        // open files
        TFile* f = TFile::Open(infile, "READ");
        TTree* T;
        T = (TTree*)f->Get(tree);

        // necessary objects
        TH2D* h = new TH2D("name","name",bbins,blow,bhi,hbins,hlow,hhi);
        vector<Parameter> s = selection(selectionfile);
        Parameter limits;
        vector<float> values(s.size());
        float treedata1 = 0;
        float treedata2 = 0;
        int isgood = 1;
        int issamebranchasfeature1 = -1;
        int issamebranchasfeature2 = -1;
        float ncollweight;
        float eventweight;
    

        //Set Addresses 
        T->SetBranchAddress(feature1,&treedata1);
        T->SetBranchAddress(feature2,&treedata2);
        T->SetBranchAddress("NCollWeight",&ncollweight);
        T->SetBranchAddress("EventWeight",&eventweight);
        

        for(int i = 0; i<s.size(); i++){
            //cout << (s[i].name).c_str() << endl;
            T->SetBranchAddress((s[i].name).c_str(),&values[i]);
            //overlap precaution
            if(s[i].name == feature1){
                issamebranchasfeature1 = i;
                //cout << "same branch as feature: " << issamebranchasfeature << endl; 
            }
            if(s[i].name == feature2){
                issamebranchasfeature2 = i;
                //cout << "same branch as feature: " << issamebranchasfeature << endl; 
            }
        }

        //main loop
        for(int i = 0; i< T->GetEntries(); i++){
            T->GetEntry(i);
            
            //if(eventweight > 100){
            //    continue;
            //}
            if(issamebranchasfeature1 >= 0){
                treedata1 = values[issamebranchasfeature1];
            }
            if(issamebranchasfeature2 >= 0){
                treedata2 = values[issamebranchasfeature2];
            }

            //cout << treedata << endl;

            for(int j = 0; j<s.size(); j++){
                limits = s[j];
                if((values[j] > limits.hi)||(values[j] < limits.lo)){
                    isgood = 0;
                    break;
                };  
            }
            if(isgood == 0){
                isgood = 1;
                continue;
            }
            if((logx == 0) && (logy == 0)){
            h->Fill(treedata1,treedata2,pow(ncollweight*eventweight,collweight));
            }
            else if((logx == 1) && (logy == 0)){
                h->Fill(log10(abs(treedata1)),treedata2,pow(ncollweight*eventweight,collweight));
            }
            else if((logx == 0) && (logy == 1)){
                h->Fill(treedata1,log10(abs(treedata2)),pow(ncollweight*eventweight,collweight));
            }
            else if((logx == 1) && (logy == 1)){
                h->Fill(log10(abs(treedata1)),log10(abs(treedata2)),pow(ncollweight*eventweight,collweight));
                //cout << "filled: " <<  log10(abs(treedata1)) << " " << log10(abs(treedata2)) << endl;
            }
        }        

        //cosmetic
        h->SetStats(0);
        //h->SetLineWidth(3);
        //h->SetLineColor(color);
        //h->SetFillColorAlpha(color,0.1);
        h->SetTitle(name);
        h->GetXaxis()->SetTitle(xlabel);
        h->GetYaxis()->SetTitle(ylabel);
        h->GetZaxis()->SetTitle(zlabel);
        if(xhi != xlow){h->GetXaxis()->SetRangeUser(xlow, xhi);}
        if(yhi != ylow){h->GetYaxis()->SetRangeUser(ylow, yhi);}
        if(zhi != zlow){h->GetYaxis()->SetRangeUser(zlow, zhi);}

        return h;  
    }


};

double unifscale(vector<TH1D*> vec){

    double maxbinheight = 0;
    for(int i =0; i<vec.size(); i++){
        if(vec[i]->GetMaximum() > maxbinheight){
        maxbinheight = vec[i]->GetMaximum();
        }
    }
    return maxbinheight;
}

void histfiller(){
    
    //histogram objects  
    
    Hist c1("hiBin","omc.root","selections/none.csv","mumutree","","Centrality %","counts",10,0,100,1,"Other",0,100,0,0,0,1);
    Hist c2("hiBin","omc.root","selections/none_cc.csv","mumutree","","Centrality %","counts",10,0,100,2,"C Hadrons = 2, B Hadrons = 0",0,100,0,0,0,1);
    Hist c3("hiBin","omc.root","selections/none_c.csv","mumutree","","Centrality %","counts",10,0,100,3,"C Hadrons = 1, B Hadrons = 0",0,100,0,0,0,1);
    Hist c4("hiBin","omc.root","selections/none_b.csv","mumutree","","Centrality %","counts",10,0,100,4,"B Hadrons = 1",0,100,0,0,0,1);
    Hist c5("hiBin","omc.root","selections/none_bb.csv","mumutree","","Centrality %","counts",10,0,100,6,"B Hadrons = 2",0,100,0,0,0,1);
    Hist c6("hiBin","omc.root","selections/none_light.csv","mumutree","","Centrality %","counts",10,0,100,7,"C Hadrons = 0, B Hadrons = 0",0,100,0,0,0,1);
   

    
    vector<Hist> Histvec = {c1,c2,c3,c4,c5,c6};
    
    int N = Histvec.size();

    //fill histogram
    vector<TH1D*> th1dvec;
    for(int i = 0; i<N; i++){
        th1dvec.push_back(Histvec[i].histogramfill());
    }


    //for(int i = 0; i<N; i++){
    //    th1dvec[i]->Scale(1/th1dvec[i]->Integral());       
    //}
    
    for(int i = 1; i<N; i++){
        th1dvec[0]->Add(th1dvec[i],-1.0);
    }
   

    //regularize y values
    float total = 0;
    double u = unifscale(th1dvec);
    for(int i = 0; i<N; i++){
        cout << Histvec[i].legend << ": " << th1dvec[i]->Integral() << endl;
        total+= th1dvec[i]->Integral();
        th1dvec[i]->GetYaxis()->SetRangeUser(0, u*1.55);
    }
    cout << "Total: " << total << endl;


    // make legend
    TLegend*G = new TLegend(0.52,0.250,0.82,0.90);
    for(int i = 0; i<N; i++){
        G->AddEntry(th1dvec[i],Histvec[i].legend,"lf");
    }
    G->SetBorderSize(0); // Removes border
    G->SetFillStyle(0);  // Removes background fill
    
    //draw on canvas
    TCanvas*q = new TCanvas("","",100,100.0);
    q->cd();
    //q->SetLogx();
    //q->SetLogy();
    q->SetMargin(0.14, 0.1, 0.1, 0.1);
    for(int i = 0; i<N; i++){
        //th1dvec[i]->Sumw2();
        th1dvec[i]->Draw("colz same");
    }
    G->Draw();
    q->SaveAs("t.pdf");
    

    cout << endl;
    cout << "none" << endl;
    cout << "S: " << ((th1dvec[1]->Integral()*1.0)/total) << endl; 
    cout << "C: " << ((th1dvec[1]->Integral()*1.0)/1096440) << endl;
    cout << "L: " << 1-((th1dvec[5]->Integral()*1.0)/total) << endl;
    

    
    TCanvas*m = new TCanvas("","",100,100);
    m->SetMargin(0.14, 0.14, 0.14, 0.14);
    m->SetLogz();
    Hist2d gat("hiBin","hiBin","odata.root","selections/none.csv","mumutree","","Dimuon pT","pT Asymmetry","counts",10,0,100,1,0,1,0,1,0,100,0,0,1,1,0);
    TH2D*cc1 = gat.histogramfill();
    cc1->Draw("colz");
    m->SaveAs("tt.pdf");
    

   Hist mcdata("hiBin","omc.root","selections/none.csv","mumutree","","Centrality %","counts (Normalized)",10,0,100,2,"Other",0,100,0,0,0,1);
   Hist data("hiBin","odata.root","selections/none.csv","mumutree","","Centrality %","counts (Normalized)",10,0,100,1,"Other",0,100,0,0,0,0);
   TH1D*mhist = mcdata.histogramfill();
   TH1D*dhist = data.histogramfill();
   cout << "DATA: " << dhist->Integral() << endl;
   dhist->Scale(1/dhist->Integral());
   mhist->Scale(1/mhist->Integral());

   vector<TH1D*> vlist = {dhist,mhist};
   double uu = unifscale(vlist);
   dhist->GetYaxis()->SetRangeUser(0, uu*1.55);
   mhist->GetYaxis()->SetRangeUser(0, uu*1.55);
   
   TLegend*G2 = new TLegend(0.252,0.75,0.92,0.90);
   G2->AddEntry(dhist,"Data","lf");
   G2->AddEntry(mhist,"Aggregate Monte Carlo","lf");
   G2->SetBorderSize(0); // Removes border
   G2->SetFillStyle(0);  // Removes background fill

   TCanvas*qq = new TCanvas("","",100,100);

   //qq->SetLogy();
    qq->SetMargin(0.14, 0.1, 0.1, 0.1);
    qq->cd();

   dhist->Draw("colz same");
   mhist->Draw("colz same");
   G2->Draw();

    qq->SaveAs("t2.pdf");
    
   
}