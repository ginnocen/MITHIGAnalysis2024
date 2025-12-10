//////////////////////////////////////////////////////////
//     ACCEPTANCE x EFFICIENCY MAP CALCULATION MACRO    //
////////////////////////////////////////////////////////// 

#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TTreeFormula.h>

#include <iostream>
#include <vector>

using namespace std;
#include "CommandLine.h" 
#include "DimuonMessenger.h"
#include "Messenger.h"   
#include "ProgressBar.h" 
#include "helpMessage.h"
#include "utilities.h"  

using namespace std;

bool isSelected(float ){  // ASKGM: EVENT + JET SELECTION HANDLED IN SKIMMER 
return true;} 

int main(int argc, char *argv[]) {

    // INPUTS
    CommandLine CL(argc, argv);
    const char* file = CL.getString("file");
    const char* output = CL.getString("output");
    vector<int> ptBins = CL.getIntVector("ptBins");
    int chargeSelection = CL.getInt("chargeSelection",0); // 1 for same sign, -1 for opposite sign, 0 for no selection
    float muPtSelection = CL.getDouble("muPt",3.5);

    // IMPORT TREE
    DimuonJetMessenger t(file);

    // DECLARE TREES + HISTOGRAMS
    TH3D* inclusiveJets = new TH3D("hInclusiveJets","hInclusiveJets",)







    unsigned long nentries = t.GetEntries();
    ProgressBar Bar(cout, nentries);
    for(int i = 0; i < t.GetEntries(); i++) { // LOOP OVER JETS


        if (i % 1000 == 0) {
            Bar.Update(i);
            Bar.Print();
        }
        
        t->GetEntry(i);







    }
    
    

  
}








