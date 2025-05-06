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
        //h->SetDirectory(0);
        
       
        //Set Addresses 
        TString feature_log = TString::Format("log10(abs(%s))", feature);
        TString weightExpression = TString::Format("(%s) * pow(%s, %d)", cut, "TotalWeight", collweight);
        if(logx == 1){
            T->Project(name, feature_log, weightExpression.Data()); 
        }
        else{
            T->Project(name, feature, weightExpression.Data()); 
        }         
        
        if(logx == 1){
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
        
        h->SetDirectory(0);
        f->Close();
        delete f;
        
    

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
        else{
            h->SetMinimum(1e-6);
        }
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
        if(logy > 0){
            if(logx > 0){
                TString concatenatedString = TString(feature2_log.Data()) + ":" + TString(feature1_log.Data());
                T->Project(name, concatenatedString.Data(), weightExpression.Data());

            }
            else{
                TString concatenatedString = TString(feature2_log.Data()) + ":" + TString(feature1);
                T->Project(name, concatenatedString.Data(), weightExpression.Data());
            }
        }
        else{
            if(logx > 0){
                TString concatenatedString = TString(feature2) + ":" + TString(feature1_log.Data());
                T->Project(name, concatenatedString.Data(), weightExpression.Data());
            }
            else{
                TString concatenatedString = TString(feature2) + ":" + TString(feature1);
                T->Project(name, concatenatedString.Data(), weightExpression.Data());
            }
        }
        //T->Delete();
        f->Close();

        //cosmetic
        h->SetStats(0);
        //h->SetLineWidth(3);
        //h->SetLineColor(color);
        //h->SetFillColorAlpha(color,0.1);
        h->SetTitle(name);
        h->GetXaxis()->SetTitle(xlabel);
        h->GetYaxis()->SetTitle(ylabel);
        h->GetZaxis()->SetTitle(zlabel);
        cout << h->GetMean(2);
        
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

    gStyle->SetOptTitle(0);
    int N = vec.size();
    vector<TH1D*> th1dvec;
    for(int i = 0; i<N; i++){
        th1dvec.push_back(vec[i].histogramfill());
    }

    // normalize histograms (flag)
    if(isnormalized == 1){
        for(int i = 0; i<N; i++){
        if(th1dvec[i]->Integral() > 0){
        th1dvec[i]->Scale(1/th1dvec[i]->Integral()); }
             
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
        th1dvec[i]->GetYaxis()->SetRangeUser(1e-5, u*yscale);
    }
    //cout << "Total: " << total << endl;
    cout << "=================="<< endl;

    // make legend
    TLegend*G = new TLegend(0.66,0.60,0.91,0.85);
    for(int i = 0; i<N; i++){
        G->AddEntry(th1dvec[i],vec[i].legend,"plf");
    }
    G->SetBorderSize(0);
    G->SetFillStyle(0); 
    G->SetTextSize(0.04); 
    //draw on canvas

    TCanvas*q = new TCanvas("","",600,500);
    q->cd();
    if(plotlogy == 1){
        q->SetLogy();
    }
    for(int i = 0; i<N; i++){
        if(errors == 1){
            if(th1dvec[i]->Integral() > 0){
                //cout << "draw" << endl;
                th1dvec[i]->Draw("colz same");
                th1dvec[i]->Draw("hist same");
            }
        }
        else{
            if(th1dvec[i]->GetEntries() > 0){
                th1dvec[i]->Draw("hist same");}
        }
    }
    G->Draw();
    drawHeader();
    q->SaveAs(name.c_str());
    q->Clear();
    //q->Delete();
}




void comparison(){
    setTDRStyle();
    //histogram objects  


    string c1 = flavorselect(basic2(),1,1,0,0); 
    string c2 = flavorselect(basic2(),2,2,0,0);
    string b1 = flavorselect(basic2(),0,200,1,1);
    string b2 = flavorselect(basic2(),0,200,2,2);
    string light = flavorselect(basic2(),0,0,0,0);

    string b1c0 = flavorselect(basic2(),0,0,1,1);
    string b1c1 = flavorselect(basic2(),1,1,1,1);
    string b1c2 = flavorselect(basic2(),2,2,1,1);
    string b1c34 = flavorselect(basic2(),3,200,1,1);

    


    string xlabelname = "dR";
    string branchname = "muDR";
    int weight = 1;
    int nbins = 15;
    float xlow = 0;
    float xhi = 0.5;
    int logx = 0;

    //mcdata
    Hist d1(branchname.c_str(), "omc.root", basic2(), "mumutree", "MC", xlabelname.c_str(), "counts",nbins,xlow,xhi,2,1,"MC",xlow,xhi,0,0,logx,weight);
    Hist d2(branchname.c_str(), "odata.root", data1(), "mumutree", "Data", xlabelname.c_str(), "counts",nbins,xlow,xhi,1,1,"Data",xlow,xhi,0,0,logx,0);

    //inclusive flavor plot
    Hist m1(branchname.c_str(), "omc.root", basic2(), "mumutree", "other", xlabelname.c_str(), "counts",nbins,xlow,xhi,1,1,"Other",xlow,xhi,0,0,logx,weight);
    Hist m2(branchname.c_str(), "omc.root", c2.c_str(), "mumutree", "cc", xlabelname.c_str(), "counts",nbins,xlow,xhi,2,1,"Double C",xlow,xhi,0,0,logx,weight);
    Hist m3(branchname.c_str(), "omc.root", b2.c_str(), "mumutree", "bb", xlabelname.c_str(), "counts",nbins,xlow,xhi,6,1,"Double B",xlow,xhi,0,0,logx,weight);
    Hist m4(branchname.c_str(), "omc.root", c1.c_str(), "mumutree", "c", xlabelname.c_str(), "counts",nbins,xlow,xhi,3,1,"Single C",xlow,xhi,0,0,logx,weight);
    Hist m5(branchname.c_str(), "omc.root", b1.c_str(), "mumutree", "b", xlabelname.c_str(), "counts",nbins,xlow,xhi,4,1,"Single B",xlow,xhi,0,0,logx,weight);
    Hist m6(branchname.c_str(), "omc.root", light.c_str(), "mumutree", "light", xlabelname.c_str(), "counts",nbins,xlow,xhi,7,1,"Light Flavor",xlow,xhi,0,0,logx,weight);


    Hist fl0(branchname.c_str(), "omc.root", b1.c_str(), "mumutree", "other", xlabelname.c_str(), "counts",nbins,xlow,xhi,1,2,"Other 1B (check)",xlow,xhi,0,0,logx,weight);
    Hist fl1(branchname.c_str(), "omc.root", b1c0.c_str(), "mumutree", "other", xlabelname.c_str(), "counts",nbins,xlow,xhi,1,2,"1B 0C",xlow,xhi,0,0,logx,weight);
    Hist fl2(branchname.c_str(), "omc.root", b1c1.c_str(), "mumutree", "other", xlabelname.c_str(), "counts",nbins,xlow,xhi,28,2,"1B 1C",xlow,xhi,0,0,logx,weight);
    Hist fl3(branchname.c_str(), "omc.root", b1c2.c_str(), "mumutree", "other", xlabelname.c_str(), "counts",nbins,xlow,xhi,3,2,"1B 2C",xlow,xhi,0,0,logx,weight);
    Hist fl4(branchname.c_str(), "omc.root", b1c34.c_str(), "mumutree", "other", xlabelname.c_str(), "counts",nbins,xlow,xhi,4,2,"1B 3+C",xlow,xhi,0,0,logx,weight);

    vector<Hist> comp = {d1,d2};
    vector<Hist> m_flavors = {m1,m2,m3,m4,m5,m6};
    //vector<Hist> m_flavors_norm = {m1,m2,m3,m5};
    vector<Hist> m_flavors_norm = {fl2,fl3,m2,m3,m5};
    vector<Hist> f_flavors = {fl1,fl2,fl3,fl4,m6,m2,m3,m4};
    finishplots(comp,"angle.pdf",1,0,1.5,0,1);


}
