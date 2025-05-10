#include <TCut.h>
#include <iostream>
#include <TString.h>
using namespace std;

const char* none(){return "";}
const char* incljets(){return "JetPT > 70";}
const char* basic(){return "JetPT > 70 && muPt1 > 4 && muPt2 > 4";}
const char* basic1(){return "JetPT > 70 && muPt1 > 4 && muPt2 > 4 && leadingjet_PTHat < 2.85";}
const char* basic2(){return "JetPT > 70 && muPt1 > 4 && muPt2 > 4 && leadingjet_PTHat < 2.85 && TotalWeight < 2";}
const char* basic3(){return "JetPT > 70 && muPt1 > 4 && muPt2 > 4 && leadingjet_PTHat < 2.85 && muDiDxy1 > 0.01 && muDiDxy1 > 0.01 && TotalWeight < 2";}
const char* data1(){return "JetPT > 70 && muPt1 > 4 && muPt2 > 4 && muDiDxy1 > 0.01 && muDiDxy1 > 0.01";}

/*
const char* cutoff1(){return "JetPT > 70 && muPt1 > 4 && muPt2 > 4 && leadingjet_PTHat < 2.85 && TotalWeight < 10";}
const char* cutoff2(){return "JetPT > 70 && muPt1 > 4 && muPt2 > 4 && leadingjet_PTHat < 2.85 && TotalWeight < 5";}
const char* cutoff3(){return "JetPT > 70 && muPt1 > 4 && muPt2 > 4 && leadingjet_PTHat < 2.85 && TotalWeight < 1";}
const char* cutoff4(){return "JetPT > 70 && muPt1 > 4 && muPt2 > 4 && leadingjet_PTHat < 2.85 && TotalWeight < 0.5";}
const char* cutoff5(){return "JetPT > 70 && muPt1 > 4 && muPt2 > 4 && leadingjet_PTHat < 2.85 && TotalWeight < 0.1";}
const char* cutoff6(){return "JetPT > 70 && muPt1 > 4 && muPt2 > 4 && leadingjet_PTHat < 2.85 && TotalWeight < 0.005";}

//const char* cutoff1(){return "JetPT > 70 && leadingjet_PTHat < 2.85 && TotalWeight < 10";}
//const char* cutoff2(){return "JetPT > 70 && leadingjet_PTHat < 2.85 && TotalWeight < 5";}
//const char* cutoff3(){return "JetPT > 70 && leadingjet_PTHat < 2.85 && TotalWeight < 1";}
//const char* cutoff4(){return "JetPT > 70 && leadingjet_PTHat < 2.85 && TotalWeight < 0.5";}
//const char* cutoff5(){return "JetPT > 70 && leadingjet_PTHat < 2.85 && TotalWeight < 0.1";}
//const char* cutoff6(){return "JetPT > 70 && leadingjet_PTHat < 2.85 && TotalWeight < 0.005";}*/





string flavorselect(const char* m, int clow, int chigh, int blow, int bhigh) {
    // Create a std::string to handle string manipulations
    std::stringstream combinedCut;
    
    // Add the base filter
    combinedCut << m;
    
    // Add the additional conditions
    combinedCut << " && MJTNcHad >= " << clow
                << " && MJTNcHad <= " << chigh
                << " && MJTNbHad >= " << blow
                << " && MJTNbHad <= " << bhigh;
    
    // Return a C-style string (const char*)
    return combinedCut.str();  // This converts std::string to const char*
}

string binselect(const char* m, int pTbin, int centbin){

    // pTbin can be 0 - 6, centbin can be 0 - 1
    stringstream combinedCut;
    combinedCut << m;
    vector<string> pTs  = {
        " && JetPT < 75", 
        " && JetPT > 75 && JetPT < 90", 
        " && JetPT > 90 && JetPT < 105", 
        " && JetPT > 105 && JetPT < 125", 
        " && JetPT > 125 && JetPT < 145", 
        " && JetPT > 145 && JetPT < 165", 
        " && JetPT > 165 && JetPT < 220"};

    vector<string> cents = {
        " && hiBin < 30",
        " && hiBin > 30 && hiBin < 90"
    };

    combinedCut << pTs[pTbin] << cents[centbin];

    return combinedCut.str();
    }




