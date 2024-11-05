bool DmesonSelection(DzeroTreeMessenger& MDzero, int iD) {
  const int nPtBinsOpt = 3;
  const int nYBinsOpt = 4;

  float Dchi2clCutValue[nPtBinsOpt][nYBinsOpt] = {
    {0.1, 0.1, 0.1, 0.1}, {0.1, 0.1, 0.1, 0.1}, {0.1, 0.1, 0.1, 0.1}};
  float DalphaCutValue[nPtBinsOpt][nYBinsOpt] = {
    {0.2, 0.4, 0.4, 0.2}, {0.25, 0.35, 0.35, 0.25}, {0.25, 0.4, 0.4, 0.25}};
  float DdthetaCutValue[nPtBinsOpt][nYBinsOpt] = {
    {0.3, 0.5, 0.5, 0.3}, {0.3, 0.3, 0.3, 0.3}, {0.3, 0.3, 0.3, 0.3}};
  float DsvpvSigCutValue[nPtBinsOpt][nYBinsOpt] = {
    {2.5, 2.5, 2.5, 2.5}, {3.5, 3.5, 3.5, 3.5}, {3.5, 3.5, 3.5, 3.5}};

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
    cerr << "Error! No pt bin and/or y bin found for D meson selection. "
     << "pt = " << pt << ", y = " << y 
     << ". Please check if the pt or y values fall within the defined bin ranges." << endl;
    return 0;
  }

  // Apply cuts for the identified ptBin and yBin
  bool pass = (MDzero.Dchi2cl[iD] >= Dchi2clCutValue[ptBin][yBin]) &&
              (MDzero.Dalpha[iD] <= DalphaCutValue[ptBin][yBin]) &&
              (MDzero.Ddtheta[iD] <= DdthetaCutValue[ptBin][yBin]) &&
              (MDzero.DsvpvDistance[iD] / MDzero.DsvpvDisErr[iD] >= DsvpvSigCutValue[ptBin][yBin]);

  return pass;
}
