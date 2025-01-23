bool DmesonSelectionPrelim23(DzeroTreeMessenger &MDzero, int iD) {
  const int nPtBinsOpt = 3;
  const int nYBinsOpt = 4;

  float Dchi2clCutValue[nPtBinsOpt][nYBinsOpt] = {{0.1, 0.1, 0.1, 0.1}, {0.1, 0.1, 0.1, 0.1}, {0.1, 0.1, 0.1, 0.1}};
  float DalphaCutValue[nPtBinsOpt][nYBinsOpt] = {{0.2, 0.4, 0.4, 0.2}, {0.25, 0.35, 0.35, 0.25}, {0.25, 0.4, 0.4, 0.25}};
  float DdthetaCutValue[nPtBinsOpt][nYBinsOpt] = {{0.3, 0.5, 0.5, 0.3}, {0.3, 0.3, 0.3, 0.3}, {0.3, 0.3, 0.3, 0.3}};
  float DsvpvSigCutValue[nPtBinsOpt][nYBinsOpt] = {{2.5, 2.5, 2.5, 2.5}, {3.5, 3.5, 3.5, 3.5}, {3.5, 3.5, 3.5, 3.5}};
  float Dtrk1PtCutValue[nPtBinsOpt][nYBinsOpt] = {{1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}};
  float Dtrk2PtCutValue[nPtBinsOpt][nYBinsOpt] = {{1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}};

  const int nPtBins = 3;
  const int nYBins = 4;
  float ptBins[nPtBins + 1] = {2, 5, 8, 12};
  float yBins[nYBins + 1] = {-2.0, -1.0, 0.0, 1.0, 2.0};

  float pt = MDzero.Dpt[iD];
  float y = MDzero.Dy[iD];

  // Determine pt and y bin indices
  int ptBin = -1;
  int yBin = -1;
  for (int i = 0; i < nPtBins; ++i) {
    if (pt >= ptBins[i] && pt < ptBins[i + 1]) {
      ptBin = i;
      break;
    }
  }
  for (int j = 0; j < nYBins; ++j) {
    if (y >= yBins[j] && y < yBins[j + 1]) {
      yBin = j;
      break;
    }
  }

  // Check if the pt and y bins were found
  if (ptBin == -1 || yBin == -1) {
    // cerr << "Error! No pt bin and/or y bin found for D meson selection. "
    //  << "pt = " << pt << ", y = " << y
    //  << ". Please check if the pt or y values fall within the defined bin ranges." << endl;
    return 0;
  }

  // Apply cuts for the identified ptBin and yBin
  bool pass = (MDzero.Dtrk1Pt[iD] >= Dtrk1PtCutValue[ptBin][yBin]) &&
              (MDzero.Dtrk2Pt[iD] >= Dtrk2PtCutValue[ptBin][yBin]) &&
              (MDzero.Dchi2cl[iD] >= Dchi2clCutValue[ptBin][yBin]) &&
              (MDzero.Dalpha[iD] <= DalphaCutValue[ptBin][yBin]) &&
              (MDzero.Ddtheta[iD] <= DdthetaCutValue[ptBin][yBin]) &&
              (MDzero.DsvpvDistance[iD] / MDzero.DsvpvDisErr[iD] >= DsvpvSigCutValue[ptBin][yBin]);

  return pass;
}

bool DmesonSelectionPrelim23SystDsvpvSig(DzeroTreeMessenger &MDzero, int iD) {
  const int nPtBinsOpt = 3;
  const int nYBinsOpt = 4;

  float Dchi2clCutValue[nPtBinsOpt][nYBinsOpt] = {{0.1, 0.1, 0.1, 0.1}, {0.1, 0.1, 0.1, 0.1}, {0.1, 0.1, 0.1, 0.1}};
  float DalphaCutValue[nPtBinsOpt][nYBinsOpt] = {{0.2, 0.4, 0.4, 0.2}, {0.25, 0.35, 0.35, 0.25}, {0.25, 0.4, 0.4, 0.25}};
  float DdthetaCutValue[nPtBinsOpt][nYBinsOpt] = {{0.3, 0.5, 0.5, 0.3}, {0.3, 0.3, 0.3, 0.3}, {0.3, 0.3, 0.3, 0.3}};
  float DsvpvSigCutValue[nPtBinsOpt][nYBinsOpt] = {{2.0, 2.0, 2.0, 2.0}, {2.5, 2.5, 2.5, 2.5}, {2.5, 2.5, 2.5, 2.5}};
  float Dtrk1PtCutValue[nPtBinsOpt][nYBinsOpt] = {{1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}};
  float Dtrk2PtCutValue[nPtBinsOpt][nYBinsOpt] = {{1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}};

  const int nPtBins = 3;
  const int nYBins = 4;
  float ptBins[nPtBins + 1] = {2, 5, 8, 12};
  float yBins[nYBins + 1] = {-2.0, -1.0, 0.0, 1.0, 2.0};

  float pt = MDzero.Dpt[iD];
  float y = MDzero.Dy[iD];

  // Determine pt and y bin indices
  int ptBin = -1;
  int yBin = -1;
  for (int i = 0; i < nPtBins; ++i) {
    if (pt >= ptBins[i] && pt < ptBins[i + 1]) {
      ptBin = i;
      break;
    }
  }
  for (int j = 0; j < nYBins; ++j) {
    if (y >= yBins[j] && y < yBins[j + 1]) {
      yBin = j;
      break;
    }
  }

  // Check if the pt and y bins were found
  if (ptBin == -1 || yBin == -1) {
    // cerr << "Error! No pt bin and/or y bin found for D meson selection. "
    //  << "pt = " << pt << ", y = " << y
    //  << ". Please check if the pt or y values fall within the defined bin ranges." << endl;
    return 0;
  }

  // Apply cuts for the identified ptBin and yBin
  bool pass = (MDzero.Dtrk1Pt[iD] >= Dtrk1PtCutValue[ptBin][yBin]) &&
              (MDzero.Dtrk2Pt[iD] >= Dtrk2PtCutValue[ptBin][yBin]) &&
              (MDzero.Dchi2cl[iD] >= Dchi2clCutValue[ptBin][yBin]) &&
              (MDzero.Dalpha[iD] <= DalphaCutValue[ptBin][yBin]) &&
              (MDzero.Ddtheta[iD] <= DdthetaCutValue[ptBin][yBin]) &&
              (MDzero.DsvpvDistance[iD] / MDzero.DsvpvDisErr[iD] >= DsvpvSigCutValue[ptBin][yBin]);

  return pass;
}

bool DmesonSelectionPrelim23SystDtrkPt(DzeroTreeMessenger &MDzero, int iD) {
  const int nPtBinsOpt = 3;
  const int nYBinsOpt = 4;

  float Dchi2clCutValue[nPtBinsOpt][nYBinsOpt] = {{0.1, 0.1, 0.1, 0.1}, {0.1, 0.1, 0.1, 0.1}, {0.1, 0.1, 0.1, 0.1}};
  float DalphaCutValue[nPtBinsOpt][nYBinsOpt] = {{0.2, 0.4, 0.4, 0.2}, {0.25, 0.35, 0.35, 0.25}, {0.25, 0.4, 0.4, 0.25}};
  float DdthetaCutValue[nPtBinsOpt][nYBinsOpt] = {{0.3, 0.5, 0.5, 0.3}, {0.3, 0.3, 0.3, 0.3}, {0.3, 0.3, 0.3, 0.3}};
  float DsvpvSigCutValue[nPtBinsOpt][nYBinsOpt] = {{2.5, 2.5, 2.5, 2.5}, {3.5, 3.5, 3.5, 3.5}, {3.5, 3.5, 3.5, 3.5}};
  float Dtrk1PtCutValue[nPtBinsOpt][nYBinsOpt] = {{0.7, 0.7, 0.7, 0.7}, {0.7, 0.7, 0.7, 0.7}, {0.7, 0.7, 0.7, 0.7}};
  float Dtrk2PtCutValue[nPtBinsOpt][nYBinsOpt] = {{0.7, 0.7, 0.7, 0.7}, {0.7, 0.7, 0.7, 0.7}, {0.7, 0.7, 0.7, 0.7}};

  const int nPtBins = 3;
  const int nYBins = 4;
  float ptBins[nPtBins + 1] = {2, 5, 8, 12};
  float yBins[nYBins + 1] = {-2.0, -1.0, 0.0, 1.0, 2.0};

  float pt = MDzero.Dpt[iD];
  float y = MDzero.Dy[iD];

  // Determine pt and y bin indices
  int ptBin = -1;
  int yBin = -1;
  for (int i = 0; i < nPtBins; ++i) {
    if (pt >= ptBins[i] && pt < ptBins[i + 1]) {
      ptBin = i;
      break;
    }
  }
  for (int j = 0; j < nYBins; ++j) {
    if (y >= yBins[j] && y < yBins[j + 1]) {
      yBin = j;
      break;
    }
  }

  // Check if the pt and y bins were found
  if (ptBin == -1 || yBin == -1) {
    // cerr << "Error! No pt bin and/or y bin found for D meson selection. "
    //  << "pt = " << pt << ", y = " << y
    //  << ". Please check if the pt or y values fall within the defined bin ranges." << endl;
    return 0;
  }

  // Apply cuts for the identified ptBin and yBin
  bool pass = (MDzero.Dtrk1Pt[iD] >= Dtrk1PtCutValue[ptBin][yBin]) &&
              (MDzero.Dtrk2Pt[iD] >= Dtrk2PtCutValue[ptBin][yBin]) &&
              (MDzero.Dchi2cl[iD] >= Dchi2clCutValue[ptBin][yBin]) &&
              (MDzero.Dalpha[iD] <= DalphaCutValue[ptBin][yBin]) &&
              (MDzero.Ddtheta[iD] <= DdthetaCutValue[ptBin][yBin]) &&
              (MDzero.DsvpvDistance[iD] / MDzero.DsvpvDisErr[iD] >= DsvpvSigCutValue[ptBin][yBin]);

  return pass;
}

bool DmesonSelectionPrelim23SystDalpha(DzeroTreeMessenger &MDzero, int iD) {
  const int nPtBinsOpt = 3;
  const int nYBinsOpt = 4;

  float Dchi2clCutValue[nPtBinsOpt][nYBinsOpt] = {{0.1, 0.1, 0.1, 0.1}, {0.1, 0.1, 0.1, 0.1}, {0.1, 0.1, 0.1, 0.1}};
  float DalphaCutValue[nPtBinsOpt][nYBinsOpt] = {{0.3, 0.6, 0.6, 0.3}, {0.45, 0.55, 0.55, 0.45}, {0.45, 0.6, 0.6, 0.45}};
  float DdthetaCutValue[nPtBinsOpt][nYBinsOpt] = {{0.3, 0.6, 0.6, 0.3}, {0.45, 0.55, 0.55, 0.45}, {0.45, 0.6, 0.6, 0.45}};
  float DsvpvSigCutValue[nPtBinsOpt][nYBinsOpt] = {{2.5, 2.5, 2.5, 2.5}, {3.5, 3.5, 3.5, 3.5}, {3.5, 3.5, 3.5, 3.5}};
  float Dtrk1PtCutValue[nPtBinsOpt][nYBinsOpt] = {{1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}};
  float Dtrk2PtCutValue[nPtBinsOpt][nYBinsOpt] = {{1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}};

  const int nPtBins = 3;
  const int nYBins = 4;
  float ptBins[nPtBins + 1] = {2, 5, 8, 12};
  float yBins[nYBins + 1] = {-2.0, -1.0, 0.0, 1.0, 2.0};

  float pt = MDzero.Dpt[iD];
  float y = MDzero.Dy[iD];

  // Determine pt and y bin indices
  int ptBin = -1;
  int yBin = -1;
  for (int i = 0; i < nPtBins; ++i) {
    if (pt >= ptBins[i] && pt < ptBins[i + 1]) {
      ptBin = i;
      break;
    }
  }
  for (int j = 0; j < nYBins; ++j) {
    if (y >= yBins[j] && y < yBins[j + 1]) {
      yBin = j;
      break;
    }
  }

  // Check if the pt and y bins were found
  if (ptBin == -1 || yBin == -1) {
    // cerr << "Error! No pt bin and/or y bin found for D meson selection. "
    //  << "pt = " << pt << ", y = " << y
    //  << ". Please check if the pt or y values fall within the defined bin ranges." << endl;
    return 0;
  }

  // Apply cuts for the identified ptBin and yBin
  bool pass = (MDzero.Dtrk1Pt[iD] >= Dtrk1PtCutValue[ptBin][yBin]) &&
              (MDzero.Dtrk2Pt[iD] >= Dtrk2PtCutValue[ptBin][yBin]) &&
              (MDzero.Dchi2cl[iD] >= Dchi2clCutValue[ptBin][yBin]) &&
              (MDzero.Dalpha[iD] <= DalphaCutValue[ptBin][yBin]) &&
              (MDzero.Ddtheta[iD] <= DdthetaCutValue[ptBin][yBin]) &&
              (MDzero.DsvpvDistance[iD] / MDzero.DsvpvDisErr[iD] >= DsvpvSigCutValue[ptBin][yBin]);

  return pass;
}

bool DmesonSelectionPrelim23SystDchi2cl(DzeroTreeMessenger &MDzero, int iD) {
  const int nPtBinsOpt = 3;
  const int nYBinsOpt = 4;

  float Dchi2clCutValue[nPtBinsOpt][nYBinsOpt] = {{0.05, 0.05, 0.05, 0.05}, {0.05, 0.05, 0.05, 0.05}, {0.05, 0.05, 0.05, 0.05}};
  float DalphaCutValue[nPtBinsOpt][nYBinsOpt] = {{0.2, 0.4, 0.4, 0.2}, {0.25, 0.35, 0.35, 0.25}, {0.25, 0.4, 0.4, 0.25}};
  float DdthetaCutValue[nPtBinsOpt][nYBinsOpt] = {{0.3, 0.5, 0.5, 0.3}, {0.3, 0.3, 0.3, 0.3}, {0.3, 0.3, 0.3, 0.3}};
  float DsvpvSigCutValue[nPtBinsOpt][nYBinsOpt] = {{2.5, 2.5, 2.5, 2.5}, {3.5, 3.5, 3.5, 3.5}, {3.5, 3.5, 3.5, 3.5}};
  float Dtrk1PtCutValue[nPtBinsOpt][nYBinsOpt] = {{1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}};
  float Dtrk2PtCutValue[nPtBinsOpt][nYBinsOpt] = {{1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}};

  const int nPtBins = 3;
  const int nYBins = 4;
  float ptBins[nPtBins + 1] = {2, 5, 8, 12};
  float yBins[nYBins + 1] = {-2.0, -1.0, 0.0, 1.0, 2.0};

  float pt = MDzero.Dpt[iD];
  float y = MDzero.Dy[iD];

  // Determine pt and y bin indices
  int ptBin = -1;
  int yBin = -1;
  for (int i = 0; i < nPtBins; ++i) {
    if (pt >= ptBins[i] && pt < ptBins[i + 1]) {
      ptBin = i;
      break;
    }
  }
  for (int j = 0; j < nYBins; ++j) {
    if (y >= yBins[j] && y < yBins[j + 1]) {
      yBin = j;
      break;
    }
  }

  // Check if the pt and y bins were found
  if (ptBin == -1 || yBin == -1) {
    // cerr << "Error! No pt bin and/or y bin found for D meson selection. "
    //  << "pt = " << pt << ", y = " << y
    //  << ". Please check if the pt or y values fall within the defined bin ranges." << endl;
    return 0;
  }

  // Apply cuts for the identified ptBin and yBin
  bool pass = (MDzero.Dtrk1Pt[iD] >= Dtrk1PtCutValue[ptBin][yBin]) &&
              (MDzero.Dtrk2Pt[iD] >= Dtrk2PtCutValue[ptBin][yBin]) &&
              (MDzero.Dchi2cl[iD] >= Dchi2clCutValue[ptBin][yBin]) &&
              (MDzero.Dalpha[iD] <= DalphaCutValue[ptBin][yBin]) &&
              (MDzero.Ddtheta[iD] <= DdthetaCutValue[ptBin][yBin]) &&
              (MDzero.DsvpvDistance[iD] / MDzero.DsvpvDisErr[iD] >= DsvpvSigCutValue[ptBin][yBin]);

  return pass;
}

bool DmesonSelectionLowPt23(DzeroTreeMessenger &MDzero, int iD) {
  const int nPtBinsOpt = 4;
  const int nYBinsOpt = 4;

  float Dchi2clCutValue[nPtBinsOpt][nYBinsOpt] = {{0.1, 0.1, 0.1, 0.1}, {0.1, 0.1, 0.1, 0.1}, {0.1, 0.1, 0.1, 0.1}, {0.1, 0.1, 0.1, 0.1}};
  float DalphaCutValue[nPtBinsOpt][nYBinsOpt] = {{0.3, 0.3, 0.3, 0.3}, {0.2, 0.4, 0.4, 0.2}, {0.25, 0.35, 0.35, 0.25}, {0.25, 0.4, 0.4, 0.25}};
  float DdthetaCutValue[nPtBinsOpt][nYBinsOpt] = {{0.3, 0.3, 0.3, 0.3}, {0.3, 0.5, 0.5, 0.3}, {0.3, 0.3, 0.3, 0.3}, {0.3, 0.3, 0.3, 0.3}};
  float DsvpvSigCutValue[nPtBinsOpt][nYBinsOpt] = {{1.5, 1.5, 1.5, 1.5}, {2.5, 2.5, 2.5, 2.5}, {3.5, 3.5, 3.5, 3.5}, {3.5, 3.5, 3.5, 3.5}};
  float Dtrk1PtCutValue[nPtBinsOpt][nYBinsOpt] = {{0.6, 0.6, 0.6, 0.6}, {0.5, 0.5, 0.5, 0.5}, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}};
  float Dtrk2PtCutValue[nPtBinsOpt][nYBinsOpt] = {{0.6, 0.6, 0.6, 0.6}, {0.5, 0.5, 0.5, 0.5}, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}};

  const int nPtBins = 4;
  const int nYBins = 4;
  float ptBins[nPtBins + 1] = {1, 2, 5, 8, 12};
  float yBins[nYBins + 1] = {-2.0, -1.0, 0.0, 1.0, 2.0};

  float pt = MDzero.Dpt[iD];
  float y = MDzero.Dy[iD];

  // Determine pt and y bin indices
  int ptBin = -1;
  int yBin = -1;
  for (int i = 0; i < nPtBins; ++i) {
    if (pt >= ptBins[i] && pt < ptBins[i + 1]) {
      ptBin = i;
      break;
    }
  }
  for (int j = 0; j < nYBins; ++j) {
    if (y >= yBins[j] && y < yBins[j + 1]) {
      yBin = j;
      break;
    }
  }

  // Check if the pt and y bins were found
  if (ptBin == -1 || yBin == -1) {
    // cerr << "Error! No pt bin and/or y bin found for D meson selection. "
    //  << "pt = " << pt << ", y = " << y
    //  << ". Please check if the pt or y values fall within the defined bin ranges." << endl;
    return 0;
  }

  // Apply cuts for the identified ptBin and yBin
  bool pass = (MDzero.Dtrk1Pt[iD] >= Dtrk1PtCutValue[ptBin][yBin]) &&
              (MDzero.Dtrk2Pt[iD] >= Dtrk2PtCutValue[ptBin][yBin]) &&
              (MDzero.Dchi2cl[iD] >= Dchi2clCutValue[ptBin][yBin]) &&
              (MDzero.Dalpha[iD] <= DalphaCutValue[ptBin][yBin]) &&
              (MDzero.Ddtheta[iD] <= DdthetaCutValue[ptBin][yBin]) &&
              (MDzero.DsvpvDistance[iD] / MDzero.DsvpvDisErr[iD] >= DsvpvSigCutValue[ptBin][yBin]);

  return pass;
}

bool DmesonSelectionSkimLowPt23(DzeroTreeMessenger &MDzero, int iD) {

  float DptCutMin = 1.;
  float DyCutAbsMax = 2.;
  float DalphaCutMax = 0.5;
  float DdthetaCutMax = 0.5;
  float DsvpvSigCutMin = 1.5;
  float Dchi2clCutMin = 0.1;
  float Dtrk1PtCutMin = 0.5;
  float Dtrk2PtCutMin = 0.5;

  bool pass = (MDzero.Dtrk1Pt[iD] >= Dtrk1PtCutMin && MDzero.Dtrk2Pt[iD] >= Dtrk2PtCutMin && MDzero.Dpt[iD] >= DptCutMin &&
               fabs(MDzero.Dy[iD]) <= DyCutAbsMax && MDzero.Dalpha[iD] <= DalphaCutMax && MDzero.Ddtheta[iD] <= DdthetaCutMax &&
               MDzero.DsvpvDistance[iD] / MDzero.DsvpvDisErr[iD] >= DsvpvSigCutMin && MDzero.Dchi2cl[iD] >= Dchi2clCutMin);

  return true;
}
