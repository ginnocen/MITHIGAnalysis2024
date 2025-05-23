using namespace std;
#include <vector>

bool CheckCut(
  vector<vector<float>> DcutValue,
  float Dvalue,
  float Dpt,
  float Dy
) {
  bool pass = false;
  for (int iCut = 0; iCut < DcutValue.size(); iCut++) {
    float cutVal =  DcutValue[iCut][0];
    float cutSign = DcutValue[iCut][1];
    float Dptmin =  DcutValue[iCut][2];
    float Dptmax =  DcutValue[iCut][3];
    float Dymin =   DcutValue[iCut][4];
    float Dymax =   DcutValue[iCut][5];
    if (
      Dpt >= Dptmin && Dpt <= Dptmax &&
      Dy >= Dymin && Dy <= Dymax &&
      (Dvalue - cutVal) * cutSign >= 0.
    ) {
      pass = true;
      break;
    }
  }
  return pass;
}

bool DCutSelection(
  DzeroTreeMessenger &MDzero,
  int iD,
  vector<vector<float>> Dchi2clCutValue,
  vector<vector<float>> DalphaCutValue,
  vector<vector<float>> DdthetaCutValue,
  vector<vector<float>> DsvpvSigCutValue,
  vector<vector<float>> Dtrk1PtCutValue,
  vector<vector<float>> Dtrk2PtCutValue
) {
  float pt = MDzero.Dpt[iD];
  float y = MDzero.Dy[iD];
  bool pass = false;
  if (
    CheckCut(Dchi2clCutValue,   MDzero.Dchi2cl[iD],   pt, y) &&
    CheckCut(DalphaCutValue,    MDzero.Dalpha[iD],    pt, y) &&
    CheckCut(DdthetaCutValue,   MDzero.Ddtheta[iD],   pt, y) &&
    CheckCut(DsvpvSigCutValue,  MDzero.DsvpvDistance[iD] / MDzero.DsvpvDisErr[iD],  pt, y) &&
    CheckCut(Dtrk1PtCutValue,   MDzero.Dtrk1Pt[iD],   pt, y) &&
    CheckCut(Dtrk2PtCutValue,   MDzero.Dtrk2Pt[iD],   pt, y)
  ) {
    pass = true;
  }
  return pass;
}

// DEFAULT CUTS
// Define cuts using {cut_value, pass_above/below, ptmin, ptmax, ymin, ymax}
// pass_above/below indicates if values above cut pass (1) or below pass (-1)
vector<vector<float>> Dchi2clCutValue_def = {
  {0.1, 1,   0,  2,  -3, 3},
  {0.1, 1,   2, 12,  -3, 3}
};
vector<vector<float>> DalphaCutValue_def = {
  {0.2,  -1,  0,  2,  -3,  3},
  {0.2,  -1,  2,  5,  -3, -1},
  {0.4,  -1,  2,  5,  -1,  1},
  {0.2,  -1,  2,  5,   1,  3},
  {0.25, -1,  5,  8,  -3, -1},
  {0.35, -1,  5,  8,  -1,  1},
  {0.25, -1,  5,  8,   1,  3},
  {0.25, -1,  8, 12,  -3, -1},
  {0.4,  -1,  8, 12,  -1,  1},
  {0.25, -1,  8, 12,   1,  3}
};
vector<vector<float>> DdthetaCutValue_def = {
  {0.3, -1,  0,  2, -3,  3},
  {0.3, -1,  2,  5, -3, -1},
  {0.5, -1,  2,  5, -1,  1},
  {0.3, -1,  2,  5,  1,  3},
  {0.3, -1,  5, 12, -3,  3}
};
vector<vector<float>> DsvpvSigCutValue_def = {
  {2.5, 1,  0,  2, -3, 3},
  {2.5, 1,  2,  5, -3, 3},
  {3.5, 1,  5, 12, -3, 3}
};
vector<vector<float>> Dtrk1PtCutValue_def = {
  {0.5, 1,  0,  2, -3, 3},
  {1.0, 1,  2, 12, -3, 3}
};
vector<vector<float>> Dtrk2PtCutValue_def = {
  {0.5, 1,  0,  2, -3, 3},
  {1.0, 1,  2, 12, -3, 3}
};

bool DpassCutDefault(DzeroTreeMessenger &MDzero, int iD) {
  bool pass = DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_def,
    DalphaCutValue_def,
    DdthetaCutValue_def,
    DsvpvSigCutValue_def,
    Dtrk1PtCutValue_def,
    Dtrk2PtCutValue_def
  );
  return pass;
}

bool DpassCutSystDsvpvSig(DzeroTreeMessenger &MDzero, int iD) {
  vector<vector<float>> DsvpvSigCutValue_syst = {
    {2.0, 1,  0,  2, -3, 3},
    {2.0, 1,  2,  5, -3, 3},
    {2.5, 1,  5, 12, -3, 3}
  };
  bool pass = DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_def,
    DalphaCutValue_def,
    DdthetaCutValue_def,
    DsvpvSigCutValue_syst,
    Dtrk1PtCutValue_def,
    Dtrk2PtCutValue_def
  );
  return pass;
}

bool DpassCutSystDtrkPt(DzeroTreeMessenger &MDzero, int iD) {
  vector<vector<float>> Dtrk1PtCutValue_syst = {
    {0.3, 1,  0,  2, -3, 3},
    {0.7, 1,  2, 12, -3, 3}
  };
  vector<vector<float>> Dtrk2PtCutValue_syst = {
    {0.3, 1,  0,  2, -3, 3},
    {0.7, 1,  2, 12, -3, 3}
  };
  bool pass = DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_def,
    DalphaCutValue_def,
    DdthetaCutValue_def,
    DsvpvSigCutValue_def,
    Dtrk1PtCutValue_syst,
    Dtrk2PtCutValue_syst
  );
  return pass;
}

bool DpassCutSystDalpha(DzeroTreeMessenger &MDzero, int iD) {
  vector<vector<float>> DalphaCutValue_syst = {
    {0.3,  -1,  0,  2,  -3,  3},
    {0.3,  -1,  2,  5,  -3, -1},
    {0.6,  -1,  2,  5,  -1,  1},
    {0.3,  -1,  2,  5,   1,  3},
    {0.45, -1,  5,  8,  -3, -1},
    {0.55, -1,  5,  8,  -1,  1},
    {0.45, -1,  5,  8,   1,  3},
    {0.45, -1,  8, 12,  -3, -1},
    {0.6,  -1,  8, 12,  -1,  1},
    {0.45, -1,  8, 12,   1,  3}
  };
  vector<vector<float>> DdthetaCutValue_syst = {
    {0.3,  -1,  0,  2,  -3,  3},
    {0.3,  -1,  2,  5,  -3, -1},
    {0.6,  -1,  2,  5,  -1,  1},
    {0.3,  -1,  2,  5,   1,  3},
    {0.45, -1,  5,  8,  -3, -1},
    {0.55, -1,  5,  8,  -1,  1},
    {0.45, -1,  5,  8,   1,  3},
    {0.45, -1,  8, 12,  -3, -1},
    {0.6,  -1,  8, 12,  -1,  1},
    {0.45, -1,  8, 12,   1,  3}
  };
  bool pass = DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_def,
    DalphaCutValue_syst,
    DdthetaCutValue_syst,
    DsvpvSigCutValue_def,
    Dtrk1PtCutValue_def,
    Dtrk2PtCutValue_def
  );
  return pass;
}

bool DpassCutSystDchi2cl(DzeroTreeMessenger &MDzero, int iD) {
  vector<vector<float>> Dchi2clCutValue_syst = {
    {0.05, 1,   0,  2,  -3, 3},
    {0.05, 1,   2, 12,  -3, 3}
  };
  bool pass = DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_syst,
    DalphaCutValue_def,
    DdthetaCutValue_def,
    DsvpvSigCutValue_def,
    Dtrk1PtCutValue_def,
    Dtrk2PtCutValue_def
  );
  return pass;
}

// ============================================================================
// DO NOT CHANGE BELOW THIS POINT! --------------------------------------------
// ============================================================================


// Selection settings used for HIN-25-002:
// April 2024 PAS for Quark Matter 2025
vector<vector<float>> Dchi2clCutValue_PAS = {
  {0.1, 1,   2, 12,  -2, 2}
};
vector<vector<float>> DalphaCutValue_PAS = {
  {0.2,  -1,  2,  5,  -2, -1},
  {0.4,  -1,  2,  5,  -1,  1},
  {0.2,  -1,  2,  5,   1,  2},
  {0.25, -1,  5,  8,  -2, -1},
  {0.35, -1,  5,  8,  -1,  1},
  {0.25, -1,  5,  8,   1,  2},
  {0.25, -1,  8, 12,  -2, -1},
  {0.4,  -1,  8, 12,  -1,  1},
  {0.25, -1,  8, 12,   1,  2}
};
vector<vector<float>> DdthetaCutValue_PAS = {
  {0.3, -1,  2,  5, -2, -1},
  {0.5, -1,  2,  5, -1,  1},
  {0.3, -1,  2,  5,  1,  2},
  {0.3, -1,  5, 12, -2,  2}
};
vector<vector<float>> DsvpvSigCutValue_PAS = {
  {2.5, 1,  2,  5, -2, 2},
  {3.5, 1,  5, 12, -2, 2}
};
vector<vector<float>> Dtrk1PtCutValue_PAS = {
  {1.0, 1,  2, 12, -2, 2}
};
vector<vector<float>> Dtrk2PtCutValue_PAS = {
  {1.0, 1,  2, 12, -2, 2}
};
  
bool DpassCut23PAS(DzeroTreeMessenger &MDzero, int iD) {
  bool pass = DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_PAS,
    DalphaCutValue_PAS,
    DdthetaCutValue_PAS,
    DsvpvSigCutValue_PAS,
    Dtrk1PtCutValue_PAS,
    Dtrk2PtCutValue_PAS
  );
  return pass;
}

bool DpassCut23PASSystDsvpvSig(DzeroTreeMessenger &MDzero, int iD) {
  vector<vector<float>> DsvpvSigCutValue_syst = {
    {2.0, 1,  2,  5, -2, 2},
    {2.5, 1,  5, 12, -2, 2}
  };
  bool pass = DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_PAS,
    DalphaCutValue_PAS,
    DdthetaCutValue_PAS,
    DsvpvSigCutValue_syst,
    Dtrk1PtCutValue_PAS,
    Dtrk2PtCutValue_PAS
  );
  return pass;
}

bool DpassCut23PASSystDtrkPt(DzeroTreeMessenger &MDzero, int iD) {
  vector<vector<float>> Dtrk1PtCutValue_syst = {
    {0.7, 1,  2, 12, -2, 2}
  };
  vector<vector<float>> Dtrk2PtCutValue_syst = {
    {0.7, 1,  2, 12, -2, 2}
  };
  bool pass = DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_PAS,
    DalphaCutValue_PAS,
    DdthetaCutValue_PAS,
    DsvpvSigCutValue_PAS,
    Dtrk1PtCutValue_syst,
    Dtrk2PtCutValue_syst
  );
  return pass;
}

bool DpassCut23PASSystDalpha(DzeroTreeMessenger &MDzero, int iD) {
  vector<vector<float>> DalphaCutValue_syst = {
    {0.3,  -1,  2,  5,  -2, -1},
    {0.6,  -1,  2,  5,  -1,  1},
    {0.3,  -1,  2,  5,   1,  2},
    {0.45, -1,  5,  8,  -2, -1},
    {0.55, -1,  5,  8,  -1,  1},
    {0.45, -1,  5,  8,   1,  2},
    {0.45, -1,  8, 12,  -2, -1},
    {0.6,  -1,  8, 12,  -1,  1},
    {0.45, -1,  8, 12,   1,  2}
  };
  vector<vector<float>> DdthetaCutValue_syst = {
    {0.3,  -1,  2,  5,  -2, -1},
    {0.6,  -1,  2,  5,  -1,  1},
    {0.3,  -1,  2,  5,   1,  2},
    {0.45, -1,  5,  8,  -2, -1},
    {0.55, -1,  5,  8,  -1,  1},
    {0.45, -1,  5,  8,   1,  2},
    {0.45, -1,  8, 12,  -2, -1},
    {0.6,  -1,  8, 12,  -1,  1},
    {0.45, -1,  8, 12,   1,  2}
  };
  bool pass = DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_PAS,
    DalphaCutValue_syst,
    DdthetaCutValue_syst,
    DsvpvSigCutValue_PAS,
    Dtrk1PtCutValue_PAS,
    Dtrk2PtCutValue_PAS
  );
  return pass;
}

bool DpassCut23PASSystDchi2cl(DzeroTreeMessenger &MDzero, int iD) {
  vector<vector<float>> Dchi2clCutValue_syst = {
    {0.05, 1,   2, 12,  -2, 2}
  };
  bool pass = DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_syst,
    DalphaCutValue_PAS,
    DdthetaCutValue_PAS,
    DsvpvSigCutValue_PAS,
    Dtrk1PtCutValue_PAS,
    Dtrk2PtCutValue_PAS
  );
  return pass;
}

bool DpassCut23LowPt(DzeroTreeMessenger &MDzero, int iD) {
  vector<vector<float>> Dchi2clCutValue_LowPt23 = {
    {0.1, 1,  1, 12,  -2,  2}
  };
  vector<vector<float>> DalphaCutValue_LowPt23 = {
    {0.3,  -1,  1,  2,  -2,  2},
    {0.2,  -1,  2,  5,  -2, -1},
    {0.4,  -1,  2,  5,  -1,  1},
    {0.2,  -1,  2,  5,   1,  2},
    {0.25, -1,  5,  8,  -2, -1},
    {0.35, -1,  5,  8,  -1,  1},
    {0.25, -1,  5,  8,   1,  2},
    {0.25, -1,  8, 12,  -2, -1},
    {0.4,  -1,  8, 12,  -1,  1},
    {0.25, -1,  8, 12,   1,  2}
  };
  vector<vector<float>> DdthetaCutValue_LowPt23 = {
    {0.3, -1,  1,  2, -2,  2},
    {0.3, -1,  2,  5, -2, -1},
    {0.5, -1,  2,  5, -1,  1},
    {0.3, -1,  2,  5,  1,  2},
    {0.3, -1,  5, 12, -2,  2}
  };
  vector<vector<float>> DsvpvSigCutValue_LowPt23 = {
    {1.5, 1,  1,  2, -2, 2},
    {2.5, 1,  2,  5, -2, 2},
    {3.5, 1,  5, 12, -2, 2}
  };
  vector<vector<float>> Dtrk1PtCutValue_LowPt23 = {
    {0.6, 1,  1,  2, -2, 2},
    {0.5, 1,  2,  5, -2, 2},
    {1.0, 1,  5, 12, -2, 2}
  };
  vector<vector<float>> Dtrk2PtCutValue_LowPt23 = {
    {0.6, 1,  1,  2, -2, 2},
    {0.5, 1,  2,  5, -2, 2},
    {1.0, 1,  5, 12, -2, 2}
  };
  bool pass = DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_LowPt23,
    DalphaCutValue_LowPt23,
    DdthetaCutValue_LowPt23,
    DsvpvSigCutValue_LowPt23,
    Dtrk1PtCutValue_LowPt23,
    Dtrk2PtCutValue_LowPt23
  );
  return pass;
}

bool DmesonSelectionSkimLowPt23(DzeroTreeMessenger &MDzero, int iD) {
  /*
  float Dtrk1PtCutMin = 0.5;
  float Dtrk2PtCutMin = 0.5;
  float DptCutMin = 1.;
  float DyCutAbsMax = 2.;
  float DalphaCutMax = 0.5;
  float DdthetaCutMax = 0.5;
  float DsvpvSigCutMin = 1.5;
  float Dchi2clCutMin = 0.1;

  bool pass = (
    MDzero.Dtrk1Pt[iD] >= Dtrk1PtCutMin &&
    MDzero.Dtrk2Pt[iD] >= Dtrk2PtCutMin &&
    MDzero.Dpt[iD] >= DptCutMin &&
    fabs(MDzero.Dy[iD]) <= DyCutAbsMax &&
    MDzero.Dalpha[iD] <= DalphaCutMax &&
    MDzero.Ddtheta[iD] <= DdthetaCutMax &&
    MDzero.DsvpvDistance[iD] / MDzero.DsvpvDisErr[iD] >= DsvpvSigCutMin &&
    MDzero.Dchi2cl[iD] >= Dchi2clCutMin
  );
  */
  return true;
}
