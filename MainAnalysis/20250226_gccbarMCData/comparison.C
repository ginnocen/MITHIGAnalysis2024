#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>
#include "tdrStyle.C"
#include "cuts.C"

using namespace std;


class Hist{

    public:
    const char* feature;
    const char* infile;
    const char* cut;
    const char* tree;
    const char* name;
    const char* xlabel;
    const char* ylabel;
    double bbins;
    double blow;
    double bhi;
    int color;
    int linestyle;
    const char* legend;
    double xlow;
    double xhi;
    double ylow;
    double yhi;
    int logx;
    int collweight;

    Hist(const char* f, const char* in, const char* ct, const char* tr, const char* na, const char* xl, const char* yl, double bbi,double blo, double bhigh, int col, int ls, const char* leg, double xlo, double xhigh, double ylo, double yhigh, int lx, int ncweight) : feature(f), infile(in), cut(ct), tree(tr), name(na), xlabel(xl), ylabel(yl), bbins(bbi), blow(blo), bhi(bhigh), color(col), linestyle(ls), legend(leg), xlow(xlo), xhi(xhigh), ylow(ylo), yhi(yhigh), logx(lx), collweight(ncweight) {}


    TH1D* histogramfill(){
        
        // open files
        TFile* f = TFile::Open(infile, "READ");
        TTree* T;
        T = (TTree*)f->Get(tree);

        // necessary objects
        TH1D* h = new TH1D(name,name,bbins,blow,bhi);
        
       
        //Set Addresses 
        TString feature_log = TString::Format("log10(abs(%s))", feature);
        TString weightExpression = TString::Format("(%s) * pow(%s, %d)", cut, "TotalWeight", collweight);
        if(logx > 1){
            T->Project(name, feature_log, weightExpression.Data()); 
        }
        else{
            T->Project(name, feature, weightExpression.Data()); 
        }         

        if(logx > 1){
            cout << legend << endl;
            cout << "Histogram filled with: " << feature_log << " from " << infile << ", " << tree << endl;
            cout << cut << endl;
            cout << "Integral: " << h->Integral() << " Entries: " << h->GetEntries() << endl;
            cout << endl;
        }
        else{
            cout << legend << endl;
            cout << "Histogram filled with: " << feature << " from " << infile << ", " << tree << endl;
            cout << cut << endl;
            cout << "Integral: " << h->Integral() << " Entries: " << h->GetEntries() << endl;
            cout << endl;
    
        }
        
        //cosmetic
        h->SetStats(0);
        h->SetLineWidth(3);
        h->SetLineColor(color);
        h->SetFillColorAlpha(color,0.1);
        h->SetMarkerColorAlpha(color,1);
        h->SetMarkerSize(1);
        h->SetLineStyle(linestyle);
        h->SetTitle(name);
        h->GetXaxis()->SetTitle(xlabel);
        h->GetYaxis()->SetTitle(ylabel);
        //h->GetXaxis()->SetTitleOffset(1.555);
        if(xhi != xlow){h->GetXaxis()->SetRangeUser(xlow, xhi);}
        if(yhi != ylow){h->GetYaxis()->SetRangeUser(ylow, yhi);}
        cout << endl;
        return h;  
    }


};

class Hist2d{

    public:
    const char* feature1;
    const char* feature2;
    const char* infile;
    const char* cut;
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
    int collweight;

    Hist2d(const char* f1, const char* f2, const char* in, const char* ct, const char* tr, const char* na, const char* xl, const char* yl, const char* zl, double bbi,double blo, double bhigh, double hbi,double hlo, double hhigh, double xlo, double xhigh, double ylo, double yhigh, double zlo, double zhigh, int lx, int ly, int ncw) 
    : feature1(f1), feature2(f2), infile(in), cut(ct), tree(tr), name(na), xlabel(xl), ylabel(yl), zlabel(zl), bbins(bbi), blow(blo), bhi(bhigh), hbins(hbi), hlow(hlo), hhi(hhigh), xlow(xlo), xhi(xhigh), ylow(ylo), yhi(yhigh),zlow(zlo), zhi(zhigh), logx(lx), logy(ly), collweight(ncw) {}


    TH2D* histogramfill(){
        // open files
        TFile* f = TFile::Open(infile, "READ");
        TTree* T;
        T = (TTree*)f->Get(tree);

        // necessary objects
        TH2D* h = new TH2D(name,name,bbins,blow,bhi,hbins,hlow,hhi);
 
        TString feature1_log = TString::Format("log10(abs(%s))", feature1);
        TString feature2_log = TString::Format("log10(abs(%s))", feature2);
        TString weightExpression = TString::Format("(%s) * pow(%s, %d)", cut, "TotalWeight", collweight);
        if(logx > 0){
            if(logy > 0){
                TString concatenatedString = TString(feature1_log.Data()) + ":" + TString(feature2_log.Data());
                T->Project(name, concatenatedString.Data(), weightExpression.Data());

            }
            else{
                TString concatenatedString = TString(feature1_log.Data()) + ":" + TString(feature2);
                T->Project(name, concatenatedString.Data(), weightExpression.Data());
            }
        }
        else{
            if(logy > 0){
                TString concatenatedString = TString(feature1) + ":" + TString(feature2_log.Data());
                T->Project(name, concatenatedString.Data(), weightExpression.Data());
            }
            else{
                TString concatenatedString = TString(feature1) + ":" + TString(feature2);
                T->Project(name, concatenatedString.Data(), weightExpression.Data());
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

void finishplots(vector<Hist> vec = {},string name = "", int isnormalized = 0, int subtractfromfirst = 0, float yscale = 1.5, int plotlogy = 0, int errors = 0){

    int N = vec.size();
    vector<TH1D*> th1dvec;
    for(int i = 0; i<N; i++){
        th1dvec.push_back(vec[i].histogramfill());
    }

    // normalize histograms (flag)
    if(isnormalized == 1){
        for(int i = 0; i<N; i++){
        th1dvec[i]->Scale(1/th1dvec[i]->Integral());       
        }
    }
    
    if(subtractfromfirst == 1){
    for(int i = 1; i<N; i++){
        th1dvec[0]->Add(th1dvec[i],-1.0);
    }
    }

    //regularize y values + print integral information
    float total = 0;
    double u = unifscale(th1dvec);
    cout << "=================="<< endl;
    for(int i = 0; i<N; i++){
        //cout << vec[i].legend << ": " << th1dvec[i]->Integral() << endl;
        total+= th1dvec[i]->Integral();
        th1dvec[i]->GetYaxis()->SetRangeUser(1e-6, u*yscale);
    }
    //cout << "Total: " << total << endl;
    cout << "=================="<< endl;

    // make legend
    TLegend*G = new TLegend(0.60,0.65,0.90,0.85);
    for(int i = 0; i<N; i++){
        G->AddEntry(th1dvec[i],vec[i].legend,"plf");
    }
    G->SetBorderSize(0);
    G->SetFillStyle(0);  
    //draw on canvas

    TCanvas*q = new TCanvas("","",600,500);
    q->cd();
    if(plotlogy == 1){
        q->SetLogy();
    }
    for(int i = 0; i<N; i++){
        if(errors == 1){
            th1dvec[i]->Draw("colz same");
        }
        else{th1dvec[i]->Draw("hist same");}
    }
    G->Draw();
    drawHeader();
    q->SaveAs(name.c_str());
}




void comparison(){
    setTDRStyle();
    //histogram objects  
    

   string c1 = flavorselect(basic(),1,1,0,0);
   string c2 = flavorselect(basic(),2,2,0,0);
   string b1 = flavorselect(basic(),0,100,1,1);
   string b2 = flavorselect(basic(),0,100,2,2);
   string light = flavorselect(basic(),0,0,0,0);
   
    


    Hist t1("mumuMass", "omc.root", basic2(), "mumutree", "1", "Dimuon Invariant Mass (GeV)", "counts",30,0,5,1,1,"Other",0,5,0,0,0,1);
    Hist t2("mumuMass", "omc.root", c2.c_str(), "mumutree", "2", "Dimuon Invariant Mass (GeV)", "counts",30,0,5,2,1,"Double C",0,5,0,0,0,1);
    Hist t3("mumuMass", "omc.root", b2.c_str(), "mumutree", "3", "Dimuon Invariant Mass (GeV)", "counts",30,0,5,6,1,"Double B",0,5,0,0,0,1);
    Hist t4("mumuMass", "omc.root", c1.c_str(), "mumutree", "4", "Dimuon Invariant Mass (GeV)", "counts",30,0,5,3,1,"Single C",0,5,0,0,0,1);
    Hist t5("mumuMass", "omc.root", b1.c_str(), "mumutree", "5", "Dimuon Invariant Mass (GeV)", "counts",30,0,5,4,1,"Single B",0,5,0,0,0,1);
    Hist t6("mumuMass", "omc.root", light.c_str(), "mumutree", "6", "Dimuon Invariant Mass (GeV)", "counts",30,0,5,7,1,"Light Flavor",0,5,0,0,0,1);
   
   vector<Hist> Q = {t1,t2,t3,t4,t5,t6};
   finishplots(Q,"tt.pdf",0,1,1.5,0,1);
}