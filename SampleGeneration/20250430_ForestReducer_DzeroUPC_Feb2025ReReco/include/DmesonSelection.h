using namespace std;
#include <vector>

// ============================================================================
// CUT FUNCTIONS --------------------------------------------------------------

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

// ============================================================================
// CUT SETTINGS ---------------------------------------------------------------
// Define cuts using:
//   {cut_value, pass_above/below, ptmin, ptmax, ymin, ymax}
//
// Notes:
// - pass_above/below indicates if values above the cut value are accepted (1)
//   or if values below the cut value are accepted (-1).
// - "_nom" is the nominal selection.
// - "_syst" are systematic settings. These should be looser than nominal".
// - "_loose" are looser-than-systematic settings, for cut validation
//   and/or BDT training

// Dchi2cl: Secondary vertex fit probability.
vector<vector<float>> Dchi2clCutValue_nom = {
  {0.1,  1,  0, 12,  -2.4, 2.4}
};
vector<vector<float>> Dchi2clCutValue_syst = {
  {0.05, 1,  0, 12,  -2.4, 2.4}
};
vector<vector<float>> Dchi2clCutValue_loose = {
  {0.05, 1,  0, 12,  -2.4, 2.4} // Minimum Dchi2cl (forest level cut)
};

// Dalpha: Angle between D0 momentum vector and decay path vector.
vector<vector<float>> DalphaCutValue_nom = {
  {1.2,  -1,  0,  1,  -2.4,  -1},
  {1.2,  -1,  0,  1,    -1,   1}, // Set before background peak
  {1.2,  -1,  0,  1,     1, 2.4},
  {1.0,  -1,  1,  2,  -2.4,  -1},
  {1.0,  -1,  1,  2,    -1,   1}, // Set at start of background plateau
  {1.0,  -1,  1,  2,     1, 2.4},
  {0.2,  -1,  2,  5,  -2.4,  -1},
  {0.5,  -1,  2,  5,    -1,   1}, // Set at start of background plateau
  {0.2,  -1,  2,  5,     1, 2.4},
  // Same as April 2025 PAS:
  {0.25, -1,  5,  8,  -2.4,  -1},
  {0.35, -1,  5,  8,    -1,   1},
  {0.25, -1,  5,  8,     1, 2.4},
  {0.25, -1,  8, 12,  -2.4,  -1},
  {0.4,  -1,  8, 12,    -1,   1},
  {0.25, -1,  8, 12,     1, 2.4}
};
vector<vector<float>> DalphaCutValue_syst = {
  {2.0,  -1,  0,  1,  -2.4,  -1},
  {2.0,  -1,  0,  1,    -1,   1}, // Set at start of signal plateau
  {2.0,  -1,  0,  1,     1, 2.4},
  {1.5,  -1,  1,  2,  -2.4,  -1},
  {1.5,  -1,  1,  2,    -1,   1}, // Set at start of signal plateau
  {1.5,  -1,  1,  2,     1, 2.4},
  {1.0,  -1,  2,  5,  -2.4,  -1},
  {1.0,  -1,  2,  5,    -1,   1}, // Set at start of signal plateau
  {1.0,  -1,  2,  5,     1, 2.4},
  // Same as April 2025 PAS:
  {0.45, -1,  5,  8,  -2.4,  -1},
  {0.55, -1,  5,  8,    -1,   1},
  {0.45, -1,  5,  8,     1, 2.4},
  {0.45, -1,  8, 12,  -2.4,  -1},
  {0.6,  -1,  8, 12,    -1,   1},
  {0.45, -1,  8, 12,     1, 2.4}
};
vector<vector<float>> DalphaCutValue_loose = {
  {3.2,  -1,  0,  1,  -2.4,  2.4}, // Covers full signal
  {2.3,  -1,  1,  2,  -2.4,  2.4}, // Set at end of signal plateau
  {2.3,  -1,  2,  5,  -2.4,  2.4}, // Set at end of signal plateau
  {1.0,  -1,  5, 12,  -2.4,  2.4}
};

// Ddtheta: Opening angle between daughter tracks.
vector<vector<float>> DdthetaCutValue_nom = {
  {1.4,  -1,  0,  1,  -2.4,  -1},
  {1.4,  -1,  0,  1,    -1,   1}, // Set at start of background plateau
  {1.4,  -1,  0,  1,     1, 2.4},
  {1.0,  -1,  1,  2,  -2.4,  -1},
  {1.0,  -1,  1,  2,    -1,   1}, // Set at start of background plateau
  {1.0,  -1,  1,  2,     1, 2.4},
  {0.5,  -1,  2,  5,  -2.4,  -1},
  {0.5,  -1,  2,  5,    -1,   1}, // Set at start of background plateau
  {0.5,  -1,  2,  5,     1, 2.4},
  // Same as April 2025 PAS:
  {0.3,  -1,  5, 12,  -2.4, 2.4}
};
vector<vector<float>> DdthetaCutValue_syst = {
  {1.8,  -1,  0,  1,  -2.4,  -1},
  {1.8,  -1,  0,  1,    -1,   1}, // Set at start of signal plateau
  {1.8,  -1,  0,  1,     1, 2.4},
  {1.5,  -1,  1,  2,  -2.4,  -1},
  {1.5,  -1,  1,  2,    -1,   1}, // Set at start of signal plateau
  {1.5,  -1,  1,  2,     1, 2.4},
  {0.8,  -1,  2,  5,  -2.4,  -1},
  {0.8,  -1,  2,  5,    -1,   1}, // Set at start of signal plateau
  {0.8,  -1,  2,  5,     1, 2.4},
  // Same as April 2025 PAS:
  {0.45, -1,  5,  8,  -2.4,  -1},
  {0.55, -1,  5,  8,    -1,   1},
  {0.45, -1,  5,  8,     1, 2.4},
  {0.45, -1,  8, 12,  -2.4,  -1},
  {0.6,  -1,  8, 12,    -1,   1},
  {0.45, -1,  8, 12,     1, 2.4}
};
vector<vector<float>> DdthetaCutValue_loose = {
  {3.2,  -1,  0,  1,  -2.4,  2.4}, // Covers full signal
  {2.5,  -1,  1,  2,  -2.4,  2.4}, // Set at end of signal plateau
  {2.5,  -1,  2,  5,  -2.4,  2.4}, // Set at end of signal plateau
  {1.0,  -1,  5, 12,  -2.4,  2.4}
};

// DsvpvSig: Decay length significance (DsvpvDistance/DsvpvDisErr)
vector<vector<float>> DsvpvSigCutValue_nom = {
  // Same as April 2025 PAS:
  {2.5, 1,  0,  1, -2.4, 2.4},
  {2.5, 1,  1,  2, -2.4, 2.4},
  {2.5, 1,  2,  5, -2.4, 2.4},
  {3.5, 1,  5, 12, -2.4, 2.4}
};
vector<vector<float>> DsvpvSigCutValue_syst = {
  // Same as April 2025 PAS:
  {2.0, 1,  0,  1, -2.4, 2.4},
  {2.0, 1,  1,  2, -2.4, 2.4},
  {2.0, 1,  2,  5, -2.4, 2.4},
  {2.5, 1,  5, 12, -2.4, 2.4}
};
vector<vector<float>> DsvpvSigCutValue_loose = {
  {1.5, 1,  0,  1, -2.4, 2.4},
  {1.5, 1,  1,  2, -2.4, 2.4},
  {1.5, 1,  2,  5, -2.4, 2.4},
  {2.0, 1,  5, 12, -2.4, 2.4},
};

// Dtrk(1/2)Pt: Daughter track tranverse momentum
vector<vector<float>> Dtrk1PtCutValue_nom = {
  {0.2, 1,  0,  1, -2.4, 2.4}, // Minimum track pT (forest level cut)
  {0.3, 1,  1,  2, -2.4, 2.4}, // Removes part of background peak
  {0.7, 1,  2,  5, -2.4, 2.4}, // Removes large portion of background peak
  // Same as April 2025 PAS:
  {1.0, 1,  5, 12, -2.4, 2.4}
};
vector<vector<float>> Dtrk1PtCutValue_syst = {
  {0.2, 1,  0,  1, -2.4, 2.4}, // Minimum track pT (forest level cut)
  {0.2, 1,  1,  2, -2.4, 2.4}, // Minimum track pT (forest level cut)
  {0.5, 1,  2,  5, -2.4, 2.4},
  // Same as April 2025 PAS:
  {0.7, 1,  5, 12, -2.4, 2.4}
};
vector<vector<float>> Dtrk1PtCutValue_loose = {
  {0.2, 1,  0,  1, -2.4, 2.4}, // Minimum track pT (forest level cut)
  {0.2, 1,  1,  2, -2.4, 2.4}, // Minimum track pT (forest level cut)
  {0.2, 1,  2,  5, -2.4, 2.4}, // Minimum track pT (forest level cut)
  {0.7, 1,  2, 12, -2.4, 2.4}
};
// Use same settings for Dtrk2 as Dtrk1:
vector<vector<float>> Dtrk2PtCutValue_nom = Dtrk1PtCutValue_nom;
vector<vector<float>> Dtrk2PtCutValue_syst = Dtrk1PtCutValue_syst;
vector<vector<float>> Dtrk2PtCutValue_loose = Dtrk1PtCutValue_loose;

bool DpassCutNominal(DzeroTreeMessenger &MDzero, int iD) {
  return DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_nom,
    DalphaCutValue_nom,
    DdthetaCutValue_nom,
    DsvpvSigCutValue_nom,
    Dtrk1PtCutValue_nom,
    Dtrk2PtCutValue_nom
  );
}

bool DpassCutLoose(DzeroTreeMessenger &MDzero, int iD) {
  return DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_loose,
    DalphaCutValue_loose,
    DdthetaCutValue_loose,
    DsvpvSigCutValue_loose,
    Dtrk1PtCutValue_loose,
    Dtrk2PtCutValue_loose
  );
}

bool DpassCutSystDsvpvSig(DzeroTreeMessenger &MDzero, int iD) {
  return DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_nom,
    DalphaCutValue_nom,
    DdthetaCutValue_nom,
    DsvpvSigCutValue_syst,
    Dtrk1PtCutValue_nom,
    Dtrk2PtCutValue_nom
  );
}

bool DpassCutSystDtrkPt(DzeroTreeMessenger &MDzero, int iD) {
  return DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_nom,
    DalphaCutValue_nom,
    DdthetaCutValue_nom,
    DsvpvSigCutValue_nom,
    Dtrk1PtCutValue_syst,
    Dtrk2PtCutValue_syst
  );
}

bool DpassCutSystDalpha(DzeroTreeMessenger &MDzero, int iD) {
  return DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_nom,
    DalphaCutValue_syst,
    DdthetaCutValue_nom,
    DsvpvSigCutValue_nom,
    Dtrk1PtCutValue_nom,
    Dtrk2PtCutValue_nom
  );
}

// Added to apply systematics for Dalpha and Ddtheta indendently:
bool DpassCutSystDdtheta(DzeroTreeMessenger &MDzero, int iD) {
  return DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_nom,
    DalphaCutValue_nom,
    DdthetaCutValue_syst,
    DsvpvSigCutValue_nom,
    Dtrk1PtCutValue_nom,
    Dtrk2PtCutValue_nom
  );
}

// Added to apply systematics for Dalpha and Ddtheta indendently:
bool DpassCutSystDalphaDdtheta(DzeroTreeMessenger &MDzero, int iD) {
  return DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_nom,
    DalphaCutValue_syst,
    DdthetaCutValue_syst,
    DsvpvSigCutValue_nom,
    Dtrk1PtCutValue_nom,
    Dtrk2PtCutValue_nom
  );
}

bool DpassCutSystDchi2cl(DzeroTreeMessenger &MDzero, int iD) {
  return DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_syst,
    DalphaCutValue_nom,
    DdthetaCutValue_nom,
    DsvpvSigCutValue_nom,
    Dtrk1PtCutValue_nom,
    Dtrk2PtCutValue_nom
  );
}



// ============================================================================
// DO NOT EDIT BELOW THIS LINE! -----------------------------------------------
// HIN-25-002 2025-04 PAS SETTINGS --------------------------------------------
// ============================================================================
// Used for April 2025 PAS for Quark Matter 2025
// 2025-09-15:
//    - Added bins for Dpt 0-2 GeV with same settings as Dpt 2-5 GeV bins.
// 2025-10-20:
//    - Rearranged bins to match formatting structure above.
//    - Added "_PAS" label to systematic cuts for clarity.

vector<vector<float>> Dchi2clCutValue_PAS = {
  {0.1, 1,   0,  2,  -2, 2},
  {0.1, 1,   2, 12,  -2, 2}
};
vector<vector<float>> Dchi2clCutValue_PAS_syst = {
  {0.05, 1,   0,  2,  -2, 2},
  {0.05, 1,   2, 12,  -2, 2}
};

vector<vector<float>> DalphaCutValue_PAS = {
  {0.2,  -1,  0,  2,  -2, -1},
  {0.4,  -1,  0,  2,  -1,  1},
  {0.2,  -1,  0,  2,   1,  2},
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
vector<vector<float>> DalphaCutValue_PAS_syst = {
  {0.3,  -1,  0,  2,  -2, -1},
  {0.6,  -1,  0,  2,  -1,  1},
  {0.3,  -1,  0,  2,   1,  2},
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

vector<vector<float>> DdthetaCutValue_PAS = {
  {0.3, -1,  0,  2, -2, -1},
  {0.5, -1,  0,  2, -1,  1},
  {0.3, -1,  0,  2,  1,  2},
  {0.3, -1,  2,  5, -2, -1},
  {0.5, -1,  2,  5, -1,  1},
  {0.3, -1,  2,  5,  1,  2},
  {0.3, -1,  5, 12, -2,  2}
};
vector<vector<float>> DdthetaCutValue_PAS_syst = {
  {0.3,  -1,  0,  2,  -2, -1},
  {0.6,  -1,  0,  2,  -1,  1},
  {0.3,  -1,  0,  2,   1,  2},
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

vector<vector<float>> DsvpvSigCutValue_PAS = {
  {2.5, 1,  0,  2, -2, 2},
  {2.5, 1,  2,  5, -2, 2},
  {3.5, 1,  5, 12, -2, 2}
};
vector<vector<float>> DsvpvSigCutValue_PAS_syst = {
  {2.0, 1,  0,  2, -2, 2},
  {2.0, 1,  2,  5, -2, 2},
  {2.5, 1,  5, 12, -2, 2}
};

vector<vector<float>> Dtrk1PtCutValue_PAS = {
  {1.0, 1,  0,  2, -2, 2},
  {1.0, 1,  2, 12, -2, 2}
};
vector<vector<float>> Dtrk1PtCutValue_PAS_syst = {
  {0.7, 1,  0,  2, -2, 2},
  {0.7, 1,  2, 12, -2, 2}
};

vector<vector<float>> Dtrk2PtCutValue_PAS = {
  {1.0, 1,  0,  2, -2, 2},
  {1.0, 1,  2, 12, -2, 2}
};
vector<vector<float>> Dtrk2PtCutValue_PAS_syst = {
  {0.7, 1,  0,  2, -2, 2},
  {0.7, 1,  2, 12, -2, 2}
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
  bool pass = DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_PAS,
    DalphaCutValue_PAS,
    DdthetaCutValue_PAS,
    DsvpvSigCutValue_PAS_syst,
    Dtrk1PtCutValue_PAS,
    Dtrk2PtCutValue_PAS
  );
  return pass;
}

bool DpassCut23PASSystDtrkPt(DzeroTreeMessenger &MDzero, int iD) {
  bool pass = DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_PAS,
    DalphaCutValue_PAS,
    DdthetaCutValue_PAS,
    DsvpvSigCutValue_PAS,
    Dtrk1PtCutValue_PAS_syst,
    Dtrk2PtCutValue_PAS_syst
  );
  return pass;
}

bool DpassCut23PASSystDalpha(DzeroTreeMessenger &MDzero, int iD) {
  bool pass = DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_PAS,
    DalphaCutValue_PAS_syst,
    DdthetaCutValue_PAS_syst,
    DsvpvSigCutValue_PAS,
    Dtrk1PtCutValue_PAS,
    Dtrk2PtCutValue_PAS
  );
  return pass;
}

bool DpassCut23PASSystDchi2cl(DzeroTreeMessenger &MDzero, int iD) {
  bool pass = DCutSelection(
    MDzero,
    iD,
    Dchi2clCutValue_PAS_syst,
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
    {0.1, 1,  0, 12,  -2,  2}
  };
  vector<vector<float>> DalphaCutValue_LowPt23 = {
    {0.3,  -1,  0,  2,  -2,  2},
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
    {0.3, -1,  0,  2, -2,  2},
    {0.3, -1,  2,  5, -2, -1},
    {0.5, -1,  2,  5, -1,  1},
    {0.3, -1,  2,  5,  1,  2},
    {0.3, -1,  5, 12, -2,  2}
  };
  vector<vector<float>> DsvpvSigCutValue_LowPt23 = {
    {1.5, 1,  0,  2, -2, 2},
    {2.5, 1,  2,  5, -2, 2},
    {3.5, 1,  5, 12, -2, 2}
  };
  vector<vector<float>> Dtrk1PtCutValue_LowPt23 = {
    {0.6, 1,  0,  2, -2, 2},
    {0.5, 1,  2,  5, -2, 2},
    {1.0, 1,  5, 12, -2, 2}
  };
  vector<vector<float>> Dtrk2PtCutValue_LowPt23 = {
    {0.6, 1,  0,  2, -2, 2},
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
