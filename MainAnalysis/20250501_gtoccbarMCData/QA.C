#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>
//#include "tdrStyle.C"
#include "comparison.C"

using namespace std;

void QA_plots(const char* filename, 
const char* directoryname,
const char* branchname,
const char* xlabelname,
int nbins,
double xlow,
double xhi,
int logx,
int logy,
double yscale){

    cout << directoryname <<" //////////////// START ////////////" << endl;

    //TFile*f = new TFile(filename,"RECREATE");
    //f->cd();

   string c1 = flavorselect(basic3(),1,1,0,0); 
   string c2 = flavorselect(basic3(),2,2,0,0);
   string b1 = flavorselect(basic3(),0,200,1,1);
   string b2 = flavorselect(basic3(),0,200,2,2);
   string light = flavorselect(basic3(),0,0,0,0);

    //inclusive flavor plot
    stringstream flavors;
    flavors << "plots/" << directoryname << "_flavors.pdf";
    Hist m1(branchname, "omc.root", basic3(), "mumutree", "other", xlabelname, "counts",nbins,xlow,xhi,1,1,"Other",xlow,xhi,0,0,logx,1);
    Hist m2(branchname, "omc.root", c2.c_str(), "mumutree", "cc", xlabelname, "counts",nbins,xlow,xhi,2,1,"Double C",xlow,xhi,0,0,logx,1);
    Hist m3(branchname, "omc.root", b2.c_str(), "mumutree", "bb", xlabelname, "counts",nbins,xlow,xhi,6,1,"Double B",xlow,xhi,0,0,logx,1);
    Hist m4(branchname, "omc.root", c1.c_str(), "mumutree", "c", xlabelname, "counts",nbins,xlow,xhi,3,1,"Single C",xlow,xhi,0,0,logx,1);
    Hist m5(branchname, "omc.root", b1.c_str(), "mumutree", "b", xlabelname, "counts",nbins,xlow,xhi,4,1,"Single B",xlow,xhi,0,0,logx,1);
    Hist m6(branchname, "omc.root", light.c_str(), "mumutree", "light", xlabelname, "counts",nbins,xlow,xhi,7,1,"Light Flavor",xlow,xhi,0,0,logx,1);
   
    vector<Hist> m_flavors = {m1,m2,m3,m4,m5,m6};
    finishplots(m_flavors,flavors.str(),0,1,yscale,logy,1);
    
    //for(int i = 0; i<m_flavors.size(); i++){
    //    f->WriteObject(m_flavors[i].histogramfill(),m_flavors[i].histogramfill()->GetName());
    //}

    //overall plot
    stringstream overall;
    overall << "plots/" << directoryname << "_overall.pdf";
    Hist mmc(branchname, "omc.root", basic3(), "mumutree", "mc", xlabelname, "counts",nbins,xlow,xhi,2,1,"MC",xlow,xhi,0,0,logx,1);
    Hist mdata(branchname, "odata.root", data1(), "mumutree", "data", xlabelname, "counts",nbins,xlow,xhi,1,1,"Data",xlow,xhi,0,0,logx,0);


    vector<Hist> m_overall = {mmc,mdata};
    cout << "check " << overall.str() << endl;
    finishplots(m_overall,overall.str(),1,0,yscale,logy,1);
    //for(int i = 0; i<m_overall.size(); i++){
    //    f->WriteObject(m_overall[i].histogramfill(),m_overall[i].histogramfill()->GetName());
    //}


    stringstream flavorsdifferential;
    stringstream overalldifferential;

    stringstream othername;
    stringstream ccname;
    stringstream bbname;
    stringstream cname;
    stringstream bname;
    stringstream lightname;
    stringstream mcname;
    stringstream dataname;

    for(int i = 0; i<7; i++){
        for(int j = 0; j<2; j++){

            cout << directoryname << "  " << i << " " << j << " ///////////// CONTINUE ////" << endl;

            string datab = binselect(data1(),i,j);
            string mcb = binselect(basic3(),i,j);
            string c1b = binselect(flavorselect(basic3(),1,1,0,0).c_str(),i,j); 
            string c2b = binselect(flavorselect(basic3(),2,2,0,0).c_str(),i,j);
            string b1b = binselect(flavorselect(basic3(),0,200,1,1).c_str(),i,j);
            string b2b = binselect(flavorselect(basic3(),0,200,2,2).c_str(),i,j);
            string lightb = binselect(flavorselect(basic3(),0,0,0,0).c_str(),i,j);

            
            othername << "other_" << i << "_" <<j;
            ccname << "cc_" << i << "_" <<j;
            bbname << "bb_" << i << "_" << j;
            cname << "c_" << i << "_" <<j;
            bname << "b_" << i << "_" << j;
            lightname << "light_" << i << "_" << j;
            mcname << "mc_" << i << "_" << j;
            dataname << "data_" << i << "_" << j;
            

            flavorsdifferential << "plots/" << directoryname << "_flavors_pTbin_" << i << "_centbin_" << j << ".pdf";
            overalldifferential  << "plots/" << directoryname << "_overall_pTbin_" << i << "_centbin_" << j << ".pdf";

            Hist mb1(branchname, "omc.root", mcb.c_str(), "mumutree", othername.str().c_str(), xlabelname, "counts",nbins,xlow,xhi,1,1,"Other",xlow,xhi,0,0,logx,1);
            Hist mb2(branchname, "omc.root", c2b.c_str(), "mumutree", ccname.str().c_str(), xlabelname, "counts",nbins,xlow,xhi,2,1,"Double C",xlow,xhi,0,0,logx,1);
            Hist mb3(branchname, "omc.root", b2b.c_str(), "mumutree", bbname.str().c_str(), xlabelname, "counts",nbins,xlow,xhi,6,1,"Double B",xlow,xhi,0,0,logx,1);
            Hist mb4(branchname, "omc.root", c1b.c_str(), "mumutree", cname.str().c_str(), xlabelname, "counts",nbins,xlow,xhi,3,1,"Single C",xlow,xhi,0,0,logx,1);
            Hist mb5(branchname, "omc.root", b1b.c_str(), "mumutree", bname.str().c_str(), xlabelname, "counts",nbins,xlow,xhi,4,1,"Single B",xlow,xhi,0,0,logx,1);
            Hist mb6(branchname, "omc.root", lightb.c_str(), "mumutree", lightname.str().c_str(), xlabelname, "counts",nbins,xlow,xhi,7,1,"Light Flavor",xlow,xhi,0,0,logx,1);

            vector<Hist> mb_flavors = {mb1,mb2,mb3,mb4,mb5,mb6};
            finishplots(mb_flavors,flavorsdifferential.str(),0,1,yscale,logy,1);

            //for(int i = 0; i<mb_flavors.size(); i++){
            //f->WriteObject(mb_flavors[i].histogramfill(),mb_flavors[i].histogramfill()->GetName());
            //}


            Hist mmcb(branchname, "omc.root", mcb.c_str(), "mumutree", mcname.str().c_str(), xlabelname, "counts",nbins,xlow,xhi,2,1,"MC",xlow,xhi,0,0,logx,1);
            Hist mdatab(branchname, "odata.root", datab.c_str(), "mumutree", dataname.str().c_str(), xlabelname, "counts",nbins,xlow,xhi,1,1,"Data",xlow,xhi,0,0,logx,0);

            vector<Hist> mb_overall = {mmcb,mdatab};
            finishplots(mb_overall,overalldifferential.str(),1,0,yscale,logy,1);

            //for(int i = 0; i<mb_overall.size(); i++){
            //    cout << othername.str() << endl;
            //    f->WriteObject(mb_overall[i].histogramfill(),mb_overall[i].histogramfill()->GetName());
            //}

            
            othername.str("");
            ccname.str("");
            bbname.str("");
            cname.str("");
            bname.str("");
            lightname.str("");
            mcname.str("");
            dataname.str("");
            flavorsdifferential.str("");
            overalldifferential.str("");
            othername.clear();
            ccname.clear();
            bbname.clear();
            cname.clear();
            bname.clear();
            lightname.clear();
            mcname.clear();
            dataname.clear();
            flavorsdifferential.clear();
            overalldifferential.clear();


        }
    }
    //f->Close();

    cout << "DONE!!!!" << endl;
}

void QA(){



    QA_plots("mass.root","Mass","mumuMass","Invariant Mass (GeV)",10,0,5.5,0,0,1.5);
    QA_plots("dimuon_pT.root","Dimuon_pT","mumuPt","Dimuon pT (GeV)",20,0,120.5,0,0,1.3);
    QA_plots("dPhi.root","dPhi","muDphi","d#phi(#mu_{1},#mu_{2})",15,0,0.5,0,0,2.0);
    QA_plots("dR.root","dR","muDR","dR(#mu_{1},#mu_{2})",15,0,0.5,0,0,1.2);
    QA_plots("dR_jetmumu.root","dRjetmumu","DRJetmumu","dR(Jet,#mu_{1}#mu_{2})",15,0,0.25,0,0,1.2);
    QA_plots("pTAsymmetry.root","Asymmetry","muDPT","pT Asymmetry",15,0,1,0,0,2.0);
    QA_plots("DCAtot.root","DCAtot","totaldca","DCA_{tot}",25,-2.5,0.5,1,0,1.3);
    QA_plots("z.root","z","mumuz","z_{#mu#mu}",15,0,1,0,0,1.3);
    QA_plots("angularity_11.root","angularity_11","mumuang11","#lambda_{1}^{1}",15,-2.5,0,1,0,1.3);
    QA_plots("angularity_12.root","angularity_12","mumuang12","#lambda_{2}^{1}",15,-4.5,0,1,0,1.3);
    QA_plots("angularity_112.root","angularity_112","mumuang112","#lambda_{0.5}^{1}",15,-2,0,1,0,1.3);


}