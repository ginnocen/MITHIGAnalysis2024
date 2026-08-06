#ifndef EFFICIENCYCOUNTING_H
#define EFFICIENCYCOUNTING_H

#include "Parameters.h"
#include "Messenger.h"

std::pair<double,int> calculateEfficiency(ChargedHadronRAATreeMessenger *ch,
        const Parameters &par,
        bool IsHijing,
        int trkPtCut);

std::pair<double, int> countingTrkptAsymmVariable(const char* inFileName, 
    const char* variableName,
    float cutplus = 0, 
    float cutminus = 0,
    bool cutBool = false, 
    bool HijingBool = false,
    int trkptcut = 0);

std::pair<double, int> countingTrkptVariable(const char* inFileName, 
    const char* varNamePlus,
    const char* varNameMinus,
    float cut = 0, 
    bool cutBool = false, 
    bool HijingBool = false,
    int trkptcut = 0);
#endif // EFFICIENCYCOUNTING_H