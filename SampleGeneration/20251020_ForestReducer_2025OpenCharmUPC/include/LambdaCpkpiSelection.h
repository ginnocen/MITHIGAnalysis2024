using namespace std;
#include <vector>

// ============================================================================
// CUT FUNCTIONS --------------------------------------------------------------

namespace LambdaCpkpiSelection {

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
  LambdaCpkpiTreeMessenger &MLambdaC,
  int iD,
  vector<vector<float>> Dchi2clCutValue,
  vector<vector<float>> DalphaCutValue,
  vector<vector<float>> DdthetaCutValue,
  vector<vector<float>> DsvpvSigCutValue,
  vector<vector<float>> Dtrk1PtCutValue,
  vector<vector<float>> Dtrk2PtCutValue,
  vector<vector<float>> Dtrk3PtCutValue
) {
  float pt = MLambdaC.Dpt[iD];
  float y = MLambdaC.Dy[iD];
  bool pass = false;
  if (
    CheckCut(Dchi2clCutValue,   MLambdaC.Dchi2cl[iD],   pt, y) &&
    CheckCut(DalphaCutValue,    MLambdaC.Dalpha[iD],    pt, y) &&
    CheckCut(DdthetaCutValue,   MLambdaC.Ddtheta[iD],   pt, y) &&
    CheckCut(DsvpvSigCutValue,  MLambdaC.DsvpvDistance[iD] / MLambdaC.DsvpvDisErr[iD],  pt, y) &&
    CheckCut(Dtrk1PtCutValue,   MLambdaC.Dtrk1Pt[iD],   pt, y) &&
    CheckCut(Dtrk2PtCutValue,   MLambdaC.Dtrk2Pt[iD],   pt, y) &&
    CheckCut(Dtrk3PtCutValue,   MLambdaC.Dtrk3Pt[iD],   pt, y)
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
// Use same settings for Dtrk2 & Dtrk3 as Dtrk1:
vector<vector<float>> Dtrk2PtCutValue_nom = Dtrk1PtCutValue_nom;
vector<vector<float>> Dtrk2PtCutValue_syst = Dtrk1PtCutValue_syst;
vector<vector<float>> Dtrk2PtCutValue_loose = Dtrk1PtCutValue_loose;
vector<vector<float>> Dtrk3PtCutValue_nom = Dtrk1PtCutValue_nom;
vector<vector<float>> Dtrk3PtCutValue_syst = Dtrk1PtCutValue_syst;
vector<vector<float>> Dtrk3PtCutValue_loose = Dtrk1PtCutValue_loose;

bool DpassCutNominal(LambdaCpkpiTreeMessenger &MLambdaC, int iD) {
  return DCutSelection(
    MLambdaC,
    iD,
    Dchi2clCutValue_nom,
    DalphaCutValue_nom,
    DdthetaCutValue_nom,
    DsvpvSigCutValue_nom,
    Dtrk1PtCutValue_nom,
    Dtrk2PtCutValue_nom,
    Dtrk3PtCutValue_nom
  );
}

bool DpassCutLoose(LambdaCpkpiTreeMessenger &MLambdaC, int iD) {
  return DCutSelection(
    MLambdaC,
    iD,
    Dchi2clCutValue_loose,
    DalphaCutValue_loose,
    DdthetaCutValue_loose,
    DsvpvSigCutValue_loose,
    Dtrk1PtCutValue_loose,
    Dtrk2PtCutValue_loose,
    Dtrk3PtCutValue_loose
  );
}

bool DpassCutSystDsvpvSig(LambdaCpkpiTreeMessenger &MLambdaC, int iD) {
  return DCutSelection(
    MLambdaC,
    iD,
    Dchi2clCutValue_nom,
    DalphaCutValue_nom,
    DdthetaCutValue_nom,
    DsvpvSigCutValue_syst,
    Dtrk1PtCutValue_nom,
    Dtrk2PtCutValue_nom,
    Dtrk3PtCutValue_nom
  );
}

bool DpassCutSystDtrkPt(LambdaCpkpiTreeMessenger &MLambdaC, int iD) {
  return DCutSelection(
    MLambdaC,
    iD,
    Dchi2clCutValue_nom,
    DalphaCutValue_nom,
    DdthetaCutValue_nom,
    DsvpvSigCutValue_nom,
    Dtrk1PtCutValue_syst,
    Dtrk2PtCutValue_syst,
    Dtrk3PtCutValue_syst
  );
}

bool DpassCutSystDalpha(LambdaCpkpiTreeMessenger &MLambdaC, int iD) {
  return DCutSelection(
    MLambdaC,
    iD,
    Dchi2clCutValue_nom,
    DalphaCutValue_syst,
    DdthetaCutValue_nom,
    DsvpvSigCutValue_nom,
    Dtrk1PtCutValue_nom,
    Dtrk2PtCutValue_nom,
    Dtrk3PtCutValue_nom
  );
}

// Added to apply systematics for Dalpha and Ddtheta indendently:
bool DpassCutSystDdtheta(LambdaCpkpiTreeMessenger &MLambdaC, int iD) {
  return DCutSelection(
    MLambdaC,
    iD,
    Dchi2clCutValue_nom,
    DalphaCutValue_nom,
    DdthetaCutValue_syst,
    DsvpvSigCutValue_nom,
    Dtrk1PtCutValue_nom,
    Dtrk2PtCutValue_nom,
    Dtrk3PtCutValue_nom
  );
}

// Added to apply systematics for Dalpha and Ddtheta indendently:
bool DpassCutSystDalphaDdtheta(LambdaCpkpiTreeMessenger &MLambdaC, int iD) {
  return DCutSelection(
    MLambdaC,
    iD,
    Dchi2clCutValue_nom,
    DalphaCutValue_syst,
    DdthetaCutValue_syst,
    DsvpvSigCutValue_nom,
    Dtrk1PtCutValue_nom,
    Dtrk2PtCutValue_nom,
    Dtrk3PtCutValue_nom
  );
}

bool DpassCutSystDchi2cl(LambdaCpkpiTreeMessenger &MLambdaC, int iD) {
  return DCutSelection(
    MLambdaC,
    iD,
    Dchi2clCutValue_syst,
    DalphaCutValue_nom,
    DdthetaCutValue_nom,
    DsvpvSigCutValue_nom,
    Dtrk1PtCutValue_nom,
    Dtrk2PtCutValue_nom,
    Dtrk3PtCutValue_nom
  );
}

} // End namespace
