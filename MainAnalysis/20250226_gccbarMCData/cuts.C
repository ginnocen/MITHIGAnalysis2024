#include <TCut.h>
#include <iostream>
#include <TString.h>
using namespace std;

const char* none(){return "";}
const char* basic(){return "JetPT > 70 && muPt1 > 4 && muPt2 > 4";}
const char* basic1(){return "JetPT > 70 && muPt1 > 4 && muPt2 > 4 && leadingjet_PTHat < 2.85";}
const char* basic2(){return "JetPT > 70 && muPt1 > 4 && muPt2 > 4 && leadingjet_PTHat < 2.85 && TotalWeight < 5";}


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



