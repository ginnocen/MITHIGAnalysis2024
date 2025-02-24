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

float safedivide(float a, float b){
    if(b == 0){
        return 0;
    }
    else{
        return a/b;
    }
}

vector<float> f_function(float xy11, float xy22, float z11, float z22){

    float xy1 = abs(xy11);
    float xy2 = abs(xy22);
    float z1 = abs(z11);
    float z2 = abs(z22);

    vector<float> f(36);

    // original
    f[0] = xy1;
    f[1] = xy2;
    f[2] = z1;
    f[3] = z2;

    // ands
    f[4] = min(xy1, xy2);
    f[5] = min(xy1, z1);
    f[6] = min(xy2, z2);
    f[7] = min(z1, z2);

    //ors
    f[8] = max(xy1, xy2);
    f[9] = max(xy1, z1);
    f[10] = max(xy2, z2);
    f[11] = max(z1, z2);

    // sqrts
    f[12] = sqrt(xy1*xy1 + xy2*xy2);
    f[13] = sqrt(xy1*xy1 + z1*z1);
    f[14] = sqrt(xy2*xy2 + z2*z2);
    f[15] = sqrt(z1*z1 + z2*z2);

    //prods
    f[16] = sqrt(xy1*xy2);
    f[17] = sqrt(xy1*z1);
    f[18] = sqrt(xy2*z2);
    f[19] = sqrt(z1*z2);

    // 2 pairs
    f[20] = min(min(xy1, xy2),min(z1, z2));
    f[21] = min(max(xy1, xy2),max(z1,z2));
    f[22] = max(min(xy1, xy2),min(z1, z2));
    f[23] = max(max(xy1, xy2),max(z1, z2));
    f[24] = min(max(xy1, z1),max(xy2,z2));
    f[25] = max(min(xy1, z1),min(xy2,z2));

    f[26] = min(sqrt(xy1*xy2),sqrt(z1*z2));
    f[27] = max(sqrt(xy1*xy2),sqrt(z1*z2));
    f[28] = min(sqrt(xy1*z1),sqrt(xy2*z2));
    f[29] = max(sqrt(xy1*z1),sqrt(xy2*z2));

    f[30] = min(sqrt(xy1*xy1 + xy2*xy2),sqrt(z1*z1 + z2*z2));
    f[31] = max(sqrt(xy1*xy1 + xy2*xy2),sqrt(z1*z1 + z2*z2));
    f[32] = min(sqrt(xy1*xy1 + z1*z1),sqrt(xy2*xy2 + z2*z2));
    f[33] = max(sqrt(xy1*xy1 + z1*z1),sqrt(xy2*xy2 + z2*z2));
    
    f[34] = sqrt(xy1*xy1 + xy2*xy2 + z1*z1 + z2*z2);
    f[35] = sqrt(sqrt(xy1*xy2*z1*z2));

    return f;
} 

vector<vector<float>> yields(TTree* T, float cutoff){

    cout << "yields" << endl;
    float ncollweight = 0;
    float mupt1 = 0;
    float mupt2 = 0;
    float jetpt = 0;
    float xy1 = 0;
    float xy2 = 0;
    float z1 = 0;
    float z2 = 0;
    float b = 0;
    float c = 0;
    vector<float> f(36,0);
    vector<vector<float>> yields(6, vector<float>(36,0));
    

    T->SetBranchAddress("muDiDxy1",&xy1);
    T->SetBranchAddress("muDiDxy2",&xy2);
    T->SetBranchAddress("muDiDz1",&z1);
    T->SetBranchAddress("muDiDz2",&z2);
    T->SetBranchAddress("MJTNbHad",&b);
    T->SetBranchAddress("MJTNcHad",&c);
    T->SetBranchAddress("muPt1",&mupt1);
    T->SetBranchAddress("muPt2",&mupt2);
    T->SetBranchAddress("JetPT",&jetpt);
    T->SetBranchAddress("NCollWeight",&ncollweight);
    //order is 
    // 0. everything
    // 1. ccbar 
    // 2. light
    // 3. 1C
    // 4. 1B
    // 5. 2B
    for(int i = 0; i<T->GetEntries(); i++){

        T->GetEntry(i);
        
        if((mupt1 < 4)||(mupt2 < 4)){
            continue;
        }
        if(jetpt < 100){
            continue;
        }
        //cout << xy1 << " " << xy2 << " " << z1 << " " << z2 << endl;
        f=f_function(xy1,xy2,z1,z2);
        
        for(int j = 0; j<36; j++){
            if(f[j] > cutoff){ 

                yields[0][j] += ncollweight;
                if((b == 0)&&(c == 2)){yields[1][j] +=ncollweight;}
                if((b == 0)&&(c == 0)){yields[2][j] +=ncollweight;}
                if((b == 0)&&(c == 1)){yields[3][j] +=ncollweight;}
                if(b == 1){yields[4][j] +=ncollweight;}
                if(b == 2){yields[5][j] +=ncollweight;}

            }            
        }
    }

    return yields;

}

void dca_optimize(){

    TFile* f = TFile::Open("o12.root", "READ");
    TTree* T;
    T = (TTree*)f->Get("mumutree");
    float cutoff;
    float charmfraction;
    float lightfraction;
    float signalfraction;

    vector<vector<float>> y(6,vector<float>(36,0));
    std::vector<TGraph*> graphs(36);
    for(int k = 0; k<36; k++){
        graphs[k] = new TGraph(40);
    }

    for(int i = 0; i<40; i++){

        cutoff = pow(10,(-5 + (5.0/40)*i));
        cout << "cutoff: " << cutoff << endl;

        y = yields(T,cutoff);

        for(int j = 0; j<y.size(); j++){
            for(int k = 0; k<y[0].size(); k++){
                cout << y[j][k] << " ";
            }
            cout << endl;
        }

        for(int j = 0; j<36; j++){

            charmfraction = safedivide((1.0*y[1][j]),1.09668e+06);
            lightfraction = 1 - safedivide((1.0*y[2][j]),(1.0*y[0][j]));
            signalfraction = safedivide((1.0*y[1][j]),(1.0*y[0][j]));
            //cout << charmfraction*lightfraction << endl;

            graphs[j]->SetPoint(i,log10(cutoff),pow(charmfraction,1));
            //graphs[j]->SetPoint(i,log10(cutoff),pow(lightfraction,1)*pow(charmfraction,0));

        }
       cout << endl;
       cout << endl;

    }
    

    vector<float> maxes(36,0);
    vector<float> maxes2(36,0);
    //vector<float> maxcutoffs(36,0);
    for(int k = 0; k<36; k++){
        graphs[k]->SetLineWidth(3);
        
        //graphs[k]->GetXaxis()->SetLabelOffset(0.02);
        //graphs[k]->GetXaxis()->SetLabelOffset(0.02);
        //cout <<"color: " << (k/4)+1 << " style " << k%4+1 << endl;
        graphs[k]->SetLineColorAlpha((k/4)+1,1.0);
        graphs[k]->SetLineStyle(k%4+1);
        
        graphs[k]->SetTitle("; C; L");
        double* yValues = graphs[k]->GetY(); 
        double maxY = *max_element(yValues, yValues + 36);
        
            //cout << k << " maxentry: " <<  maxY << endl;
        maxes[k] = maxY;
        maxes2[k] = maxY;
        //if(maxY < 0.982082){
        //    graphs[k]->SetLineColor(17);
        //}
    }

    sort(maxes.begin(), maxes.end());
    for(int q = maxes.size(); q >= 0; q--){
        for(int w = 0; w< maxes.size(); w++){
            if(maxes[q] == maxes2[w]){
                cout << " function: " << w << " maximum value: " << maxes[q] << " rank: " << 36-q <<  endl;
                //if((36-q) > 5){
                //    graphs[w]->SetLineColor(17);
                //}
            }
        }
    }

    TLegend*bogus = new TLegend(0.1,0.55,0.4,0.9);
    //bogus->AddEntry(graphs[34],"#sqrt{dxy1 #times dxy1 + dxy2 #times dxy2 + dz1 #times dz1 + dz2 #times dz2}","lp");
    //bogus->AddEntry(graphs[33],"max(#sqrt{dxy1 #times dxy1 + dz1 #times dz1},#sqrt{dxy2 #times dxy2 + dz2 #times dz2})","lp");
    //bogus->AddEntry(graphs[31],"max(#sqrt{dxy1 #times dxy1 + xy2 #times xy2},#sqrt{dz1 #times z1 + z2 #times z2})","lp");
    //bogus->AddEntry(graphs[23],"max(max(dxy1, dxy2),max(dz1, dz2))","lp");
    //bogus->AddEntry(graphs[29],"max(#sqrt{dxy1 #times dz1}, #sqrt{dxy2 #times dz2})","lp"); 

    /*bogus->AddEntry(graphs[30],"min(#sqrt{xy1 #times xy1 + xy2 #times xy2}, #sqrt{z1 #times z1 + z2 #times z2)}","lp");
    bogus->AddEntry(graphs[21],"min(max(xy1, xy2),max(z1,z2))","lp");
    bogus->AddEntry(graphs[29],"max(#sqrt{dxy1 #times dz1}, #sqrt{dxy2 #times dz2})","lp");
    bogus->AddEntry(graphs[25],"max(min(dxy1, dz1), min(dxy2, dz2))","lp");
    bogus->AddEntry(graphs[34],"#sqrt{dxy1 #times dxy1 + dxy2 #times dxy2 + dz1 #times dz1 + dz2 #times dz2}","lp");*/

    //bogus->AddEntry(graphs[20],"min(min(dxy1, dxy2),min(dz1, dz2))","lp");
    //bogus->AddEntry(graphs[28],"min(#sqrt{xy1 #times z1},#sqrt{xy2 #times z2})","lp");
    //bogus->AddEntry(graphs[4], "min(dxy1, dxy2)","lp");
    //bogus->AddEntry(graphs[32],"min(#sqrt{dxy1 #times dxy1 + dz1 #times dz1},#sqrt{dxy2 #times dxy2 + dz2 #times dz2})","lp");
    //bogus->AddEntry(graphs[24],"min(max(dxy1, dz1),max(dxy2,dz2))","lp");


    
    bogus->SetBorderSize(0); // Removes border
    bogus->SetFillStyle(0);


    TCanvas*C = new TCanvas("","",100,100);
    //C->SetLogx();
   // C->SetLogy();
    graphs[0]->Draw("alp");
    graphs[1]->Draw("lp same");
    graphs[2]->Draw("lp same");
    graphs[3]->Draw("lp same");
    graphs[4]->Draw("lp same");
    graphs[5]->Draw("lp same");
    graphs[6]->Draw("lp same");
    graphs[7]->Draw("lp same");
    graphs[8]->Draw("lp same");
    graphs[9]->Draw("lp same");
    graphs[10]->Draw("lp same");
    graphs[11]->Draw("lp same");
    graphs[12]->Draw("lp same");
    graphs[13]->Draw("lp same");
    graphs[14]->Draw("lp same");
    graphs[15]->Draw("lp same");
    graphs[16]->Draw("lp same");
    graphs[17]->Draw("lp same");
    graphs[18]->Draw("lp same");
    graphs[19]->Draw("lp same");
    graphs[20]->Draw("lp same");
    graphs[21]->Draw("lp same");
    graphs[22]->Draw("lp same");
    graphs[23]->Draw("lp same");
    graphs[24]->Draw("lp same");
    graphs[25]->Draw("lp same");
    graphs[26]->Draw("lp same");
    graphs[27]->Draw("lp same");
    graphs[28]->Draw("lp same");
    graphs[29]->Draw("lp same");
    graphs[30]->Draw("lp same");
    graphs[31]->Draw("lp same");
    graphs[32]->Draw("lp same");
    graphs[33]->Draw("lp same");
    graphs[34]->Draw("lp same");
    graphs[35]->Draw("lp same");

    bogus->Draw();


    graphs[0]->GetYaxis()->SetRangeUser(0.0,0.4);
    C->SetGridx();
    C->SetGridy();
    C->SaveAs("q.pdf");

}

