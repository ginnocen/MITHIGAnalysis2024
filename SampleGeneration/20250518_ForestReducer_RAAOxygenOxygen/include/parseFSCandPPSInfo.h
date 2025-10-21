#ifndef CHARGEDHADRONRAAFSCANDPPSPARSEFUNCS
#define CHARGEDHADRONRAAFSCANDPPSPARSEFUNCS

void fillFSCInfo(ChargedHadronRAATreeMessenger &MCH, FSCTreeMessenger &MFSC, int n) {
    if (MFSC.zside[n] != -1 || MFSC.section[n] != 1) { return; }

    if (MFSC.channel[n] == 7) {
        MCH.FSC2topM_adc->push_back(MFSC.adcTs0[n]);
        MCH.FSC2topM_adc->push_back(MFSC.adcTs1[n]);
        MCH.FSC2topM_adc->push_back(MFSC.adcTs2[n]);
        MCH.FSC2topM_adc->push_back(MFSC.adcTs3[n]);
        MCH.FSC2topM_adc->push_back(MFSC.adcTs4[n]);
        MCH.FSC2topM_adc->push_back(MFSC.adcTs5[n]);

        MCH.FSC2topM_chargefC->push_back(MFSC.chargefCTs0[n]);
        MCH.FSC2topM_chargefC->push_back(MFSC.chargefCTs1[n]);
        MCH.FSC2topM_chargefC->push_back(MFSC.chargefCTs2[n]);
        MCH.FSC2topM_chargefC->push_back(MFSC.chargefCTs3[n]);
        MCH.FSC2topM_chargefC->push_back(MFSC.chargefCTs4[n]);
        MCH.FSC2topM_chargefC->push_back(MFSC.chargefCTs5[n]);
        
        MCH.FSC2topM_tdc->push_back(MFSC.tdcTs0[n]);
        MCH.FSC2topM_tdc->push_back(MFSC.tdcTs1[n]);
        MCH.FSC2topM_tdc->push_back(MFSC.tdcTs2[n]);
        MCH.FSC2topM_tdc->push_back(MFSC.tdcTs3[n]);
        MCH.FSC2topM_tdc->push_back(MFSC.tdcTs4[n]);
        MCH.FSC2topM_tdc->push_back(MFSC.tdcTs5[n]);
    } else if (MFSC.channel[n] == 8) {
        MCH.FSC2bottomM_adc->push_back(MFSC.adcTs0[n]);
        MCH.FSC2bottomM_adc->push_back(MFSC.adcTs1[n]);
        MCH.FSC2bottomM_adc->push_back(MFSC.adcTs2[n]);
        MCH.FSC2bottomM_adc->push_back(MFSC.adcTs3[n]);
        MCH.FSC2bottomM_adc->push_back(MFSC.adcTs4[n]);
        MCH.FSC2bottomM_adc->push_back(MFSC.adcTs5[n]);

        MCH.FSC2bottomM_chargefC->push_back(MFSC.chargefCTs0[n]);
        MCH.FSC2bottomM_chargefC->push_back(MFSC.chargefCTs1[n]);
        MCH.FSC2bottomM_chargefC->push_back(MFSC.chargefCTs2[n]);
        MCH.FSC2bottomM_chargefC->push_back(MFSC.chargefCTs3[n]);
        MCH.FSC2bottomM_chargefC->push_back(MFSC.chargefCTs4[n]);
        MCH.FSC2bottomM_chargefC->push_back(MFSC.chargefCTs5[n]);
        
        MCH.FSC2bottomM_tdc->push_back(MFSC.tdcTs0[n]);
        MCH.FSC2bottomM_tdc->push_back(MFSC.tdcTs1[n]);
        MCH.FSC2bottomM_tdc->push_back(MFSC.tdcTs2[n]);
        MCH.FSC2bottomM_tdc->push_back(MFSC.tdcTs3[n]);
        MCH.FSC2bottomM_tdc->push_back(MFSC.tdcTs4[n]);
        MCH.FSC2bottomM_tdc->push_back(MFSC.tdcTs5[n]);
    } else if (MFSC.channel[n] == 9) {
        MCH.FSC3bottomleftM_adc->push_back(MFSC.adcTs0[n]);
        MCH.FSC3bottomleftM_adc->push_back(MFSC.adcTs1[n]);
        MCH.FSC3bottomleftM_adc->push_back(MFSC.adcTs2[n]);
        MCH.FSC3bottomleftM_adc->push_back(MFSC.adcTs3[n]);
        MCH.FSC3bottomleftM_adc->push_back(MFSC.adcTs4[n]);
        MCH.FSC3bottomleftM_adc->push_back(MFSC.adcTs5[n]);

        MCH.FSC3bottomleftM_chargefC->push_back(MFSC.chargefCTs0[n]);
        MCH.FSC3bottomleftM_chargefC->push_back(MFSC.chargefCTs1[n]);
        MCH.FSC3bottomleftM_chargefC->push_back(MFSC.chargefCTs2[n]);
        MCH.FSC3bottomleftM_chargefC->push_back(MFSC.chargefCTs3[n]);
        MCH.FSC3bottomleftM_chargefC->push_back(MFSC.chargefCTs4[n]);
        MCH.FSC3bottomleftM_chargefC->push_back(MFSC.chargefCTs5[n]);
        
        MCH.FSC3bottomleftM_tdc->push_back(MFSC.tdcTs0[n]);
        MCH.FSC3bottomleftM_tdc->push_back(MFSC.tdcTs1[n]);
        MCH.FSC3bottomleftM_tdc->push_back(MFSC.tdcTs2[n]);
        MCH.FSC3bottomleftM_tdc->push_back(MFSC.tdcTs3[n]);
        MCH.FSC3bottomleftM_tdc->push_back(MFSC.tdcTs4[n]);
        MCH.FSC3bottomleftM_tdc->push_back(MFSC.tdcTs5[n]);
    } else if (MFSC.channel[n] == 10) {
        MCH.FSC3bottomrightM_adc->push_back(MFSC.adcTs0[n]);
        MCH.FSC3bottomrightM_adc->push_back(MFSC.adcTs1[n]);
        MCH.FSC3bottomrightM_adc->push_back(MFSC.adcTs2[n]);
        MCH.FSC3bottomrightM_adc->push_back(MFSC.adcTs3[n]);
        MCH.FSC3bottomrightM_adc->push_back(MFSC.adcTs4[n]);
        MCH.FSC3bottomrightM_adc->push_back(MFSC.adcTs5[n]);

        MCH.FSC3bottomrightM_chargefC->push_back(MFSC.chargefCTs0[n]);
        MCH.FSC3bottomrightM_chargefC->push_back(MFSC.chargefCTs1[n]);
        MCH.FSC3bottomrightM_chargefC->push_back(MFSC.chargefCTs2[n]);
        MCH.FSC3bottomrightM_chargefC->push_back(MFSC.chargefCTs3[n]);
        MCH.FSC3bottomrightM_chargefC->push_back(MFSC.chargefCTs4[n]);
        MCH.FSC3bottomrightM_chargefC->push_back(MFSC.chargefCTs5[n]);
        
        MCH.FSC3bottomrightM_tdc->push_back(MFSC.tdcTs0[n]);
        MCH.FSC3bottomrightM_tdc->push_back(MFSC.tdcTs1[n]);
        MCH.FSC3bottomrightM_tdc->push_back(MFSC.tdcTs2[n]);
        MCH.FSC3bottomrightM_tdc->push_back(MFSC.tdcTs3[n]);
        MCH.FSC3bottomrightM_tdc->push_back(MFSC.tdcTs4[n]);
        MCH.FSC3bottomrightM_tdc->push_back(MFSC.tdcTs5[n]);
    } else if (MFSC.channel[n] == 11) {
        MCH.FSC3topleftM_adc->push_back(MFSC.adcTs0[n]);
        MCH.FSC3topleftM_adc->push_back(MFSC.adcTs1[n]);
        MCH.FSC3topleftM_adc->push_back(MFSC.adcTs2[n]);
        MCH.FSC3topleftM_adc->push_back(MFSC.adcTs3[n]);
        MCH.FSC3topleftM_adc->push_back(MFSC.adcTs4[n]);
        MCH.FSC3topleftM_adc->push_back(MFSC.adcTs5[n]);

        MCH.FSC3topleftM_chargefC->push_back(MFSC.chargefCTs0[n]);
        MCH.FSC3topleftM_chargefC->push_back(MFSC.chargefCTs1[n]);
        MCH.FSC3topleftM_chargefC->push_back(MFSC.chargefCTs2[n]);
        MCH.FSC3topleftM_chargefC->push_back(MFSC.chargefCTs3[n]);
        MCH.FSC3topleftM_chargefC->push_back(MFSC.chargefCTs4[n]);
        MCH.FSC3topleftM_chargefC->push_back(MFSC.chargefCTs5[n]);
        
        MCH.FSC3topleftM_tdc->push_back(MFSC.tdcTs0[n]);
        MCH.FSC3topleftM_tdc->push_back(MFSC.tdcTs1[n]);
        MCH.FSC3topleftM_tdc->push_back(MFSC.tdcTs2[n]);
        MCH.FSC3topleftM_tdc->push_back(MFSC.tdcTs3[n]);
        MCH.FSC3topleftM_tdc->push_back(MFSC.tdcTs4[n]);
        MCH.FSC3topleftM_tdc->push_back(MFSC.tdcTs5[n]);
    } else if (MFSC.channel[n] == 12) {
        MCH.FSC3toprightM_adc->push_back(MFSC.adcTs0[n]);
        MCH.FSC3toprightM_adc->push_back(MFSC.adcTs1[n]);
        MCH.FSC3toprightM_adc->push_back(MFSC.adcTs2[n]);
        MCH.FSC3toprightM_adc->push_back(MFSC.adcTs3[n]);
        MCH.FSC3toprightM_adc->push_back(MFSC.adcTs4[n]);
        MCH.FSC3toprightM_adc->push_back(MFSC.adcTs5[n]);

        MCH.FSC3toprightM_chargefC->push_back(MFSC.chargefCTs0[n]);
        MCH.FSC3toprightM_chargefC->push_back(MFSC.chargefCTs1[n]);
        MCH.FSC3toprightM_chargefC->push_back(MFSC.chargefCTs2[n]);
        MCH.FSC3toprightM_chargefC->push_back(MFSC.chargefCTs3[n]);
        MCH.FSC3toprightM_chargefC->push_back(MFSC.chargefCTs4[n]);
        MCH.FSC3toprightM_chargefC->push_back(MFSC.chargefCTs5[n]);
        
        MCH.FSC3toprightM_tdc->push_back(MFSC.tdcTs0[n]);
        MCH.FSC3toprightM_tdc->push_back(MFSC.tdcTs1[n]);
        MCH.FSC3toprightM_tdc->push_back(MFSC.tdcTs2[n]);
        MCH.FSC3toprightM_tdc->push_back(MFSC.tdcTs3[n]);
        MCH.FSC3toprightM_tdc->push_back(MFSC.tdcTs4[n]);
        MCH.FSC3toprightM_tdc->push_back(MFSC.tdcTs5[n]);
    }
}

void fillPPSInfo(ChargedHadronRAATreeMessenger &MCH, PPSTreeMessenger &MPPS, int n) {
    if (MPPS.zside[n] != -1) { return; }

    if (MPPS.station[n] == 0) {
        MCH.PPSStation0M_x->push_back(MPPS.x[n]);
        MCH.PPSStation0M_y->push_back(MPPS.y[n]);
    } else if (MPPS.station[n] == 2) {
        MCH.PPSStation2M_x->push_back(MPPS.x[n]);
        MCH.PPSStation2M_y->push_back(MPPS.y[n]);
    }
}

#endif