struct clusComp {
  int nPixHits;
  std::vector<float>* z0;
  std::vector<int>* nHit;
  std::vector<float>* chi;
};

// https://github.com/CmsHI/cmssw/blob/forest_CMSSW_15_1_X/HeavyIonsAnalysis/EventAnalysis/plugins/HIClusterCompatibilityFilter.cc
// https://github.com/CmsHI/cmssw/blob/forest_CMSSW_15_1_X/HeavyIonsAnalysis/EventAnalysis/python/clusterCompatibilityFilter_cfi.py
double determineQuality(const clusComp& cc, double minZ = -20.0, double maxZ = 20.05) {
  // will compare cluster compatibility at a determined best
  // z position to + and - 10 cm from the best position

  float best_z = 0.;
  int best_n = 0., low_n = 0., high_n = 0.;

  // look for best vertex z position within zMin to zMax range
  // best position is determined by maximum nHit with
  // chi used for breaking a tie
  int nhits_max = 0;
  double chi_max = 1e+9;
  for (int i = 0; i < cc.z0->size(); i++) {
    if (cc.z0->at(i) > maxZ || cc.z0->at(i) < minZ)
      continue;
    if (cc.nHit->at(i) == 0)
      continue;
    if (cc.nHit->at(i) > nhits_max) {
      chi_max = 1e+9;
      nhits_max = cc.nHit->at(i);
    }
    if (cc.nHit->at(i) >= nhits_max && cc.chi->at(i) < chi_max) {
      chi_max = cc.chi->at(i);
      best_z = cc.z0->at(i);
      best_n = cc.nHit->at(i);
    }
  }

  // find compatible clusters at + or - 10 cm of the best,
  // or get as close as possible in terms of z position.
  double low_target = best_z - 10.0;
  double high_target = best_z + 10.0;
  double low_match = 1000., high_match = 1000.;
  for (int i = 0; i < cc.z0->size(); i++) {
    if (fabs(cc.z0->at(i) - low_target) < low_match) {
      low_n = cc.nHit->at(i);
      low_match = fabs(cc.z0->at(i) - low_target);
    }
    if (fabs(cc.z0->at(i) - high_target) < high_match) {
      high_n = cc.nHit->at(i);
      high_match = fabs(cc.z0->at(i) - high_target);
    }
  }

  // determine vertex compatibility quality score
  double clusVtxQual = 0.0;
  if ((low_n + high_n) > 0)
    clusVtxQual = (2.0 * best_n) / (low_n + high_n);  // A/B
  else if (best_n > 0)
    clusVtxQual = 1000.0;  // A/0 (set to arbitrarily large number)
  else
    clusVtxQual = 0;

  return clusVtxQual;
}
