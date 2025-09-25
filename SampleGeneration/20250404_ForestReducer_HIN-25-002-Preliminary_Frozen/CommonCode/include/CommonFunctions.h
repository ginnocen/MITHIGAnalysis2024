#ifndef CommonFunctions_h_25551
#define CommonFunctions_h_25551

#include <cmath>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#ifdef USE_FJ
#include "fastjet/contrib/ConstituentSubtractor.hh"
#endif

#define M_MU 0.1056583755

// Structs

struct PseudoParticle;

struct PseudoParticle
{
   double Eta;
   double Phi;
   double PT;
   PseudoParticle(double eta, double phi, double pt)
   {
      Eta = eta;
      Phi = phi;
      PT = pt;
   }
   bool operator <(const PseudoParticle &other) const
   {
      if(Eta < other.Eta)   return true;
      if(Eta > other.Eta)   return false;
      if(Phi < other.Phi)   return true;
      if(Phi > other.Phi)   return false;
      if(PT < other.PT)     return true;
      if(PT > other.PT)     return false;
      return false;
   }
};

// Function declarations

double DeltaPhi(double Phi1, double Phi2);
double PhiRangePositive(double Phi);
double PhiRangeSymmetric(double Phi);
double PhiRangeCorrelation(double Phi);
double FindNColl(int hiBin);
double FindNPart(int hiBin);
double FindNCollAverage(int hiBin);
double FindNPartAverage(int hiBin);
std::pair<double, double> WTAAxis(std::vector<double> &Eta, std::vector<double> &Phi, std::vector<double> &PT);
std::pair<double, double> WTAAxisCA(std::vector<double> Eta, std::vector<double> Phi, std::vector<double> PT);
std::pair<double, double> WTAAxisTable(std::vector<double> Eta, std::vector<double> Phi, std::vector<double> PT);
void ConstituentSubtraction(std::vector<double> &Eta, std::vector<double> &Phi, std::vector<double> &PT,
   std::vector<double> *EtaMin = nullptr, std::vector<double> *EtaMax = nullptr, std::vector<double> *Rho = nullptr,
   double MaxR = 1.0, double MaxAbsEta = -1);
void DoCSBruteForce(std::vector<double> &Eta, std::vector<double> &Phi, std::vector<double> &PT,
   std::vector<double> &GhostEta, std::vector<double> &GhostPhi, std::vector<double> &GhostPT,
   double MaxR2);
#ifdef USE_FJ
void DoCSFastJet(std::vector<double> &Eta, std::vector<double> &Phi, std::vector<double> &PT,
   std::vector<double> &GhostEta, std::vector<double> &GhostPhi, std::vector<double> &GhostPT,
   double MaxR2);
#endif
std::string InfoString(std::string Info);
std::string InfoString(char *Info);
std::string InfoString(int Info);
std::string InfoString(float Info);
std::string InfoString(double Info);
std::string InfoString(bool Info);
std::string InfoString(std::vector<std::string> Info);
double GetZWeightPbPb(double PT, double Y, double HiBin);
double GetZWeightPbPbMC(double PT, double Y, double HiBin);
double GetZWeightPbPbData(double PT, double Y, double HiBin);
double GetZWeightPbPbDataTrigger(double PT, double Y, double HiBin);
double GetZWeightPPMC(double PT, double Y);
double GetZWeightPPData(double PT, double Y);
double GetZWeightPPDataTrigger(double PT, double Y);
double GetVZWeightPbPb(double VZ);
double GetVZWeightPP(double VZ);
int GetHiBin(double hiHF, int Variation);
double GetInterSampleZWeight(double PT);

// Function implementations

double DeltaPhi(double Phi1, double Phi2)
{
   return PhiRangeSymmetric(Phi1 - Phi2);
}

double PhiRangePositive(double Phi)
{
   while(Phi < 0)   Phi = Phi + 2 * M_PI;
   return Phi;
}

double PhiRangeSymmetric(double Phi)
{
   if(Phi < -M_PI)   Phi = Phi + 2 * M_PI;
   if(Phi > +M_PI)   Phi = Phi - 2 * M_PI;
   return Phi;
}

double PhiRangeCorrelation(double Phi)
{
   if(Phi < -M_PI * 0.5)   Phi = Phi + 2 * M_PI;
   if(Phi > +M_PI * 1.5)   Phi = Phi - 2 * M_PI;
   return Phi;
}

double FindNColl(int hiBin)
{
   static const int nbins = 200;
   static const double Ncoll[nbins] = {1976.95, 1944.02, 1927.29, 1891.9, 1845.3, 1807.2, 1760.45, 1729.18, 1674.8, 1630.3, 1590.52, 1561.72, 1516.1, 1486.5, 1444.68, 1410.88, 1376.4, 1347.32, 1309.71, 1279.98, 1255.31, 1219.89, 1195.13, 1165.96, 1138.92, 1113.37, 1082.26, 1062.42, 1030.6, 1009.96, 980.229, 955.443, 936.501, 915.97, 892.063, 871.289, 847.364, 825.127, 806.584, 789.163, 765.42, 751.187, 733.001, 708.31, 690.972, 677.711, 660.682, 640.431, 623.839, 607.456, 593.307, 576.364, 560.967, 548.909, 530.475, 519.575, 505.105, 490.027, 478.133, 462.372, 451.115, 442.642, 425.76, 416.364, 405.154, 392.688, 380.565, 371.167, 360.28, 348.239, 340.587, 328.746, 320.268, 311.752, 300.742, 292.172, 281.361, 274.249, 267.025, 258.625, 249.931, 240.497, 235.423, 228.63, 219.854, 214.004, 205.425, 199.114, 193.618, 185.644, 180.923, 174.289, 169.641, 161.016, 157.398, 152.151, 147.425, 140.933, 135.924, 132.365, 127.017, 122.127, 117.817, 113.076, 109.055, 105.16, 101.323, 98.098, 95.0548, 90.729, 87.6495, 84.0899, 80.2237, 77.2201, 74.8848, 71.3554, 68.7745, 65.9911, 63.4136, 61.3859, 58.1903, 56.4155, 53.8486, 52.0196, 49.2921, 47.0735, 45.4345, 43.8434, 41.7181, 39.8988, 38.2262, 36.4435, 34.8984, 33.4664, 31.8056, 30.351, 29.2074, 27.6924, 26.7754, 25.4965, 24.2802, 22.9651, 22.0059, 21.0915, 19.9129, 19.1041, 18.1487, 17.3218, 16.5957, 15.5323, 14.8035, 14.2514, 13.3782, 12.8667, 12.2891, 11.61, 11.0026, 10.3747, 9.90294, 9.42648, 8.85324, 8.50121, 7.89834, 7.65197, 7.22768, 6.7755, 6.34855, 5.98336, 5.76555, 5.38056, 5.11024, 4.7748, 4.59117, 4.23247, 4.00814, 3.79607, 3.68702, 3.3767, 3.16309, 2.98282, 2.8095, 2.65875, 2.50561, 2.32516, 2.16357, 2.03235, 1.84061, 1.72628, 1.62305, 1.48916, 1.38784, 1.28366, 1.24693, 1.18552, 1.16085, 1.12596, 1.09298, 1.07402, 1.06105, 1.02954};
   return Ncoll[hiBin];
}

double FindNPart(int hiBin)
{
   static const int nbins = 200;
   static const double Npart[nbins] = {401.99, 398.783, 396.936, 392.71, 387.901, 383.593, 377.914, 374.546, 367.507, 361.252, 356.05, 352.43, 345.701, 341.584, 335.148, 330.581, 325.135, 320.777, 315.074, 310.679, 306.687, 301.189, 296.769, 291.795, 287.516, 283.163, 277.818, 274.293, 269.29, 265.911, 260.574, 256.586, 252.732, 249.194, 245.011, 241.292, 236.715, 232.55, 229.322, 225.328, 221.263, 218.604, 214.728, 210.554, 206.878, 203.924, 200.84, 196.572, 193.288, 189.969, 186.894, 183.232, 180.24, 177.36, 174.008, 171.222, 168.296, 165.319, 162.013, 158.495, 156.05, 154.218, 150.559, 148.455, 145.471, 142.496, 139.715, 137.395, 134.469, 131.926, 129.817, 127.045, 124.467, 122.427, 119.698, 117.607, 114.543, 112.662, 110.696, 108.294, 105.777, 103.544, 101.736, 99.943, 97.4951, 95.4291, 93.2148, 91.2133, 89.5108, 87.2103, 85.7498, 83.5134, 81.9687, 79.7456, 78.1684, 76.4873, 74.7635, 72.761, 71.0948, 69.6102, 67.7806, 66.2215, 64.5813, 63.0269, 61.4325, 59.8065, 58.2423, 57.2432, 55.8296, 54.2171, 52.8809, 51.3254, 49.9902, 48.6927, 47.5565, 46.136, 44.8382, 43.6345, 42.3964, 41.4211, 39.9681, 39.178, 37.9341, 36.9268, 35.5626, 34.5382, 33.6912, 32.8156, 31.6695, 30.6552, 29.7015, 28.8655, 27.9609, 27.0857, 26.105, 25.3163, 24.4872, 23.6394, 23.0484, 22.2774, 21.4877, 20.5556, 19.9736, 19.3296, 18.5628, 17.916, 17.2928, 16.6546, 16.1131, 15.4013, 14.8264, 14.3973, 13.7262, 13.2853, 12.8253, 12.2874, 11.7558, 11.2723, 10.8829, 10.4652, 9.96477, 9.6368, 9.09316, 8.84175, 8.48084, 8.05694, 7.64559, 7.29709, 7.07981, 6.70294, 6.45736, 6.10284, 5.91788, 5.5441, 5.33311, 5.06641, 4.96415, 4.6286, 4.38214, 4.2076, 4.01099, 3.81054, 3.63854, 3.43403, 3.23244, 3.08666, 2.86953, 2.74334, 2.62787, 2.48354, 2.38115, 2.26822, 2.23137, 2.1665, 2.14264, 2.10636, 2.07358, 2.05422, 2.04126, 2.00954};
   return Npart[hiBin];
}

double FindNCollAverage(int hiBinLow, int hiBinHigh)
{
   double w=0;
   static const int nbins = 200;
   static const double Ncoll[nbins] = {1976.95, 1944.02, 1927.29, 1891.9, 1845.3, 1807.2, 1760.45, 1729.18, 1674.8, 1630.3, 1590.52, 1561.72, 1516.1, 1486.5, 1444.68, 1410.88, 1376.4, 1347.32, 1309.71, 1279.98, 1255.31, 1219.89, 1195.13, 1165.96, 1138.92, 1113.37, 1082.26, 1062.42, 1030.6, 1009.96, 980.229, 955.443, 936.501, 915.97, 892.063, 871.289, 847.364, 825.127, 806.584, 789.163, 765.42, 751.187, 733.001, 708.31, 690.972, 677.711, 660.682, 640.431, 623.839, 607.456, 593.307, 576.364, 560.967, 548.909, 530.475, 519.575, 505.105, 490.027, 478.133, 462.372, 451.115, 442.642, 425.76, 416.364, 405.154, 392.688, 380.565, 371.167, 360.28, 348.239, 340.587, 328.746, 320.268, 311.752, 300.742, 292.172, 281.361, 274.249, 267.025, 258.625, 249.931, 240.497, 235.423, 228.63, 219.854, 214.004, 205.425, 199.114, 193.618, 185.644, 180.923, 174.289, 169.641, 161.016, 157.398, 152.151, 147.425, 140.933, 135.924, 132.365, 127.017, 122.127, 117.817, 113.076, 109.055, 105.16, 101.323, 98.098, 95.0548, 90.729, 87.6495, 84.0899, 80.2237, 77.2201, 74.8848, 71.3554, 68.7745, 65.9911, 63.4136, 61.3859, 58.1903, 56.4155, 53.8486, 52.0196, 49.2921, 47.0735, 45.4345, 43.8434, 41.7181, 39.8988, 38.2262, 36.4435, 34.8984, 33.4664, 31.8056, 30.351, 29.2074, 27.6924, 26.7754, 25.4965, 24.2802, 22.9651, 22.0059, 21.0915, 19.9129, 19.1041, 18.1487, 17.3218, 16.5957, 15.5323, 14.8035, 14.2514, 13.3782, 12.8667, 12.2891, 11.61, 11.0026, 10.3747, 9.90294, 9.42648, 8.85324, 8.50121, 7.89834, 7.65197, 7.22768, 6.7755, 6.34855, 5.98336, 5.76555, 5.38056, 5.11024, 4.7748, 4.59117, 4.23247, 4.00814, 3.79607, 3.68702, 3.3767, 3.16309, 2.98282, 2.8095, 2.65875, 2.50561, 2.32516, 2.16357, 2.03235, 1.84061, 1.72628, 1.62305, 1.48916, 1.38784, 1.28366, 1.24693, 1.18552, 1.16085, 1.12596, 1.09298, 1.07402, 1.06105, 1.02954};
   for(int i=hiBinLow; i<hiBinHigh; i++)  w+=Ncoll[i]/(hiBinHigh-hiBinLow);
   return w;
}

double FindNPartAverage(int hiBinLow, int hiBinHigh)
{
   double w=0;
   static const int nbins = 200;
   static const double Npart[nbins] = {401.99, 398.783, 396.936, 392.71, 387.901, 383.593, 377.914, 374.546, 367.507, 361.252, 356.05, 352.43, 345.701, 341.584, 335.148, 330.581, 325.135, 320.777, 315.074, 310.679, 306.687, 301.189, 296.769, 291.795, 287.516, 283.163, 277.818, 274.293, 269.29, 265.911, 260.574, 256.586, 252.732, 249.194, 245.011, 241.292, 236.715, 232.55, 229.322, 225.328, 221.263, 218.604, 214.728, 210.554, 206.878, 203.924, 200.84, 196.572, 193.288, 189.969, 186.894, 183.232, 180.24, 177.36, 174.008, 171.222, 168.296, 165.319, 162.013, 158.495, 156.05, 154.218, 150.559, 148.455, 145.471, 142.496, 139.715, 137.395, 134.469, 131.926, 129.817, 127.045, 124.467, 122.427, 119.698, 117.607, 114.543, 112.662, 110.696, 108.294, 105.777, 103.544, 101.736, 99.943, 97.4951, 95.4291, 93.2148, 91.2133, 89.5108, 87.2103, 85.7498, 83.5134, 81.9687, 79.7456, 78.1684, 76.4873, 74.7635, 72.761, 71.0948, 69.6102, 67.7806, 66.2215, 64.5813, 63.0269, 61.4325, 59.8065, 58.2423, 57.2432, 55.8296, 54.2171, 52.8809, 51.3254, 49.9902, 48.6927, 47.5565, 46.136, 44.8382, 43.6345, 42.3964, 41.4211, 39.9681, 39.178, 37.9341, 36.9268, 35.5626, 34.5382, 33.6912, 32.8156, 31.6695, 30.6552, 29.7015, 28.8655, 27.9609, 27.0857, 26.105, 25.3163, 24.4872, 23.6394, 23.0484, 22.2774, 21.4877, 20.5556, 19.9736, 19.3296, 18.5628, 17.916, 17.2928, 16.6546, 16.1131, 15.4013, 14.8264, 14.3973, 13.7262, 13.2853, 12.8253, 12.2874, 11.7558, 11.2723, 10.8829, 10.4652, 9.96477, 9.6368, 9.09316, 8.84175, 8.48084, 8.05694, 7.64559, 7.29709, 7.07981, 6.70294, 6.45736, 6.10284, 5.91788, 5.5441, 5.33311, 5.06641, 4.96415, 4.6286, 4.38214, 4.2076, 4.01099, 3.81054, 3.63854, 3.43403, 3.23244, 3.08666, 2.86953, 2.74334, 2.62787, 2.48354, 2.38115, 2.26822, 2.23137, 2.1665, 2.14264, 2.10636, 2.07358, 2.05422, 2.04126, 2.00954};
   for(int i=hiBinLow; i<hiBinHigh; i++)  w+=Npart[i]/(hiBinHigh-hiBinLow);
   return w;
}

std::pair<double, double> WTAAxis(std::vector<double> &Eta, std::vector<double> &Phi, std::vector<double> &PT)
{
   return WTAAxisTable(Eta, Phi, PT);
}

std::pair<double, double> WTAAxisCA(std::vector<double> Eta, std::vector<double> Phi, std::vector<double> PT)
{
   std::pair<double, double> Result;

   if(Eta.size() == 0)
      return Result;

   std::vector<PseudoParticle> Particles;
   for(int i = 0; i < (int)Eta.size(); i++)
      Particles.push_back(PseudoParticle(Eta[i], Phi[i], PT[i]));
   std::sort(Particles.begin(), Particles.end());

   while(Particles.size() >= 2)
   {
      int N = Particles.size();

      int BestI = -1, BestJ = -1;
      double BestD2 = -1;

      // Loop over particles
      for(int i = 0; i < N; i++)
      {
         for(int j = i + 1; j < N; j++)
         {
            double DEta = Particles[i].Eta - Particles[j].Eta;
            double DPhi = DeltaPhi(Particles[i].Phi, Particles[j].Phi);
            double PTMin = std::min(Particles[i].PT, Particles[j].PT);
            // double D2 = (DEta * DEta + DPhi * DPhi) / (PTMin * PTMin);
            double D2 = DEta * DEta + DPhi * DPhi;

            if(BestD2 < 0)
            {
               BestI = i;
               BestJ = j;
               BestD2 = D2;
               continue;
            }

            // Since things are eta-sorted, if we hit something larger than best already, we skip the rest
            // This is true only for C/A case
            if(DEta * DEta > BestD2)   
               break;

            if(D2 < BestD2)
            {
               BestI = i;
               BestJ = j;
               BestD2 = D2;
            }
         }
      }

      // Now we found the closest pair, we merge particles
      if(Particles[BestI].PT > Particles[BestJ].PT)
      {
         Particles[BestI].PT = Particles[BestI].PT + Particles[BestJ].PT;
         Particles.erase(Particles.begin() + BestJ);
      }
      else
      {
         Particles[BestJ].PT = Particles[BestI].PT + Particles[BestJ].PT;
         Particles.erase(Particles.begin() + BestI);
      }
   }

   Result.first = Particles[0].Eta;
   Result.second = Particles[0].Phi;

   return Result;
}

std::pair<double, double> WTAAxisTable(std::vector<double> Eta, std::vector<double> Phi, std::vector<double> PT)
{
   int N = Eta.size();

   std::vector<bool> Alive(N, true);
   std::vector<int> ClosestIndex(N);
   std::vector<double> ClosestD2(N);

   // First we initialize the tables
   for(int i = 0; i < N; i++)
   {
      int BestIndex = -1;
      double BestD2 = -1;

      for(int j = 0; j < N; j++)
      {
         if(i == j)
            continue;
            
         double DEta = Eta[i] - Eta[j];
         double DPhi = DeltaPhi(Phi[i], Phi[j]);
         double PTMin = std::min(PT[i], PT[j]);
         double D2 = (DEta * DEta + DPhi * DPhi) / (PTMin * PTMin);
         // double D2 = DEta * DEta + DPhi * DPhi;

         if(BestD2 < 0 || D2 < BestD2)
         {
            BestIndex = j;
            BestD2 = D2;
         }
      }

      ClosestIndex[i] = BestIndex;
      ClosestD2[i] = BestD2;
   }

   // Now we loop.  If there are N particles we need to recombine N - 1 times
   for(int iS = 0; iS < N - 1; iS++)
   {
      // First find the best pair from the table
      int BestI = -1;
      for(int i = 0; i < N; i++)
      {
         if(Alive[i] == false)
            continue;
         if(BestI < 0 || ClosestD2[i] <= ClosestD2[BestI])
            BestI = i;
      }

      // Now merge the two
      int BestJ = ClosestIndex[BestI];
      int NewI = BestI;
      
      // cout << Eta[BestI] << " " << Eta[BestJ] << endl;
      // cout << ClosestD2[BestI] << endl;

      if(PT[BestI] > PT[BestJ])
      {
         PT[BestI] = PT[BestI] + PT[BestJ];
         Alive[BestJ] = false;
         NewI = BestI;
      }
      else
      {
         PT[BestJ] = PT[BestI] + PT[BestJ];
         Alive[BestI] = false;
         NewI = BestJ;
      }

      // Finally update the tables
      // First we update ones where closest is the old NewI
      for(int i = 0; i < N; i++)
      {
         if(Alive[i] == false)
            continue;

         if(ClosestIndex[i] == BestI || ClosestIndex[i] == BestJ)   // need full reevaluation (?)
         {
            int BestIndex = 0;
            double BestD2 = -1;
            for(int j = 0; j < N; j++)
            {
               if(Alive[j] == false)   continue;
               if(i == j)              continue;
         
               double DEta = Eta[i] - Eta[j];
               double DPhi = DeltaPhi(Phi[i], Phi[j]);
               double PTMin = std::min(PT[i], PT[j]);
               double D2 = (DEta * DEta + DPhi * DPhi) / (PTMin * PTMin);
               // double D2 = DEta * DEta + DPhi * DPhi;
               
               if(BestD2 < 0 || D2 < BestD2)
               {
                  BestIndex = j;
                  BestD2 = D2;
               }
            }

            ClosestIndex[i] = BestIndex;
            ClosestD2[i] = BestD2;
         }
      }

      // Then we update the NewI with everyone else
      ClosestIndex[NewI] = -1;
      ClosestD2[NewI] = -1;
      for(int j = 0; j < N; j++)
      {
         if(Alive[j] == false)
            continue;
         if(j == NewI)
            continue;

         double DEta = Eta[NewI] - Eta[j];
         double DPhi = DeltaPhi(Phi[NewI], Phi[j]);
         double PTMin = std::min(PT[NewI], PT[j]);
         double D2 = (DEta * DEta + DPhi * DPhi) / (PTMin * PTMin);
         // double D2 = DEta * DEta + DPhi * DPhi;

         if(ClosestD2[NewI] < 0 || D2 < ClosestD2[NewI])
         {
            ClosestIndex[NewI] = j;
            ClosestD2[NewI] = D2;
         }
         if(D2 < ClosestD2[j])
         {
            ClosestIndex[j] = NewI;
            ClosestD2[j] = D2;
         }
      }
   }

   std::pair<double, double> Result;
   
   for(int i = 0; i < N; i++)
   {
      if(Alive[i] == false)
         continue;

      Result.first = Eta[i];
      Result.second = Phi[i];
   }
   
   return Result;
}

void ConstituentSubtraction(std::vector<double> &Eta, std::vector<double> &Phi, std::vector<double> &PT,
   std::vector<double> *EtaMin, std::vector<double> *EtaMax, std::vector<double> *Rho,
   double MaxR, double MaxAbsEta)
{
   if(EtaMin == nullptr || EtaMax == nullptr || Rho == nullptr)
      return;

   double GhostA = 0.005;
   double GhostDPhi = sqrt(GhostA);
   double GhostDEta = sqrt(GhostA);

   // Setup ghosts
   std::vector<double> GhostEta;
   std::vector<double> GhostPhi;
   std::vector<double> GhostPT;

   int NBin = EtaMin->size();
   for(int iBin = 0; iBin < NBin; iBin++)
   {
      double Min = (*EtaMin)[iBin];
      double Max = (*EtaMax)[iBin];

      if(MaxAbsEta > 0 && Max < -MaxAbsEta)
         continue;
      if(MaxAbsEta > 0 && Min > MaxAbsEta)
         continue;

      if(Min < -MaxAbsEta)
         Min = -MaxAbsEta;
      if(Max > MaxAbsEta)
         Max = MaxAbsEta;

      double NPhi = std::ceil(2 * M_PI / GhostDPhi);
      double NEta = std::ceil((Max - Min) / GhostDEta);
      double A = (Max - Min) / NEta * (2 * M_PI) / NPhi;

      for(int iEta = 0; iEta < NEta; iEta++)
      {
         for(int iPhi = 0; iPhi < NPhi; iPhi++)
         {
            GhostEta.push_back(Min + (Max - Min) / NEta * (iBin + 0.5));
            GhostPhi.push_back(-M_PI + 2 * M_PI / NPhi * (iPhi + 0.5));
            GhostPT.push_back(A * (*Rho)[iBin]);
         }
      }
   }

   if(NBin == 0)
      return;

   // Run CS
#ifdef USE_FJ
   DoCSFastJet(Eta, Phi, PT, GhostEta, GhostPhi, GhostPT, MaxR > 0 ? (MaxR * MaxR) : -1);
#else
   DoCSBruteForce(Eta, Phi, PT, GhostEta, GhostPhi, GhostPT, MaxR * MaxR);
#endif
}

void DoCSBruteForce(std::vector<double> &Eta, std::vector<double> &Phi, std::vector<double> &PT,
   std::vector<double> &GhostEta, std::vector<double> &GhostPhi, std::vector<double> &GhostPT,
   double MaxR2)
{
   std::vector<PseudoParticle> Particles;
   for(int i = 0; i < (int)Eta.size(); i++)
      Particles.push_back(PseudoParticle(Eta[i], Phi[i], PT[i]));
   std::sort(Particles.begin(), Particles.end());
   std::vector<bool> ParticlesAlive(Particles.size(), true);

   std::vector<PseudoParticle> Ghosts;
   for(int i = 0; i < (int)GhostEta.size(); i++)
      Ghosts.push_back(PseudoParticle(GhostEta[i], GhostPhi[i], GhostPT[i]));
   std::sort(Ghosts.begin(), Ghosts.end());
   std::vector<bool> GhostsAlive(Ghosts.size(), true);

   int IterationCount = 0;
   bool Changed = true;
   while(Changed == true)
   {
      Changed = false;
      IterationCount = IterationCount + 1;

      int OverallBestI = -1;
      int OverallBestJ = -1;
      double OverallBestR2 = -1;
      for(int i = 0; i < (int)Particles.size(); i++)
      {
         if(ParticlesAlive[i] == false)
            continue;

         int StartJ = 0;
         if(OverallBestR2 > 0)
         {
            double Limit = Particles[i].Eta - sqrt(OverallBestR2);
            int MinJ = 0;
            int MaxJ = Ghosts.size();

            if(Ghosts[MinJ].Eta > Limit)   // min already above limit.  Stop.
               StartJ = MinJ;
            else if(Ghosts[MaxJ].Eta < Limit)   // max still below limit.  Stop.
               StartJ = MaxJ;
            else   // min and max sandwich the limit.  Binary search.
            {
               while(MaxJ - MinJ > 1)
               {
                  int Middle = (MinJ + MaxJ) / 2;
                  if(Ghosts[Middle].Eta < Limit)
                     MinJ = Middle;
                  else
                     MaxJ = Middle;
               }

               StartJ = MinJ;
            }
         }

         int BestJ = -1;
         double BestR2 = -1;
         for(int j = StartJ; j < (int)Ghosts.size(); j++)
         {
            if(GhostsAlive[j] == false)
               continue;

            double DEta = Particles[i].Eta - Ghosts[j].Eta;
            double DPhi = DeltaPhi(Particles[i].Phi, Ghosts[j].Phi);
            double DR2 = DEta * DEta + DPhi * DPhi;
            
            if(OverallBestR2 > 0 && j != StartJ && DEta * DEta > OverallBestR2)
               break;

            if(BestR2 < 0 || DR2 < BestR2)
            {
               BestJ = j;
               BestR2 = DR2;
            }
         }

         // cout << "StartJ, BestJ, BestR2 = " << StartJ << " " << BestJ << " " << BestR2 << endl;

         if(BestR2 > 0)
         {
            if(OverallBestR2 < 0 || OverallBestR2 > BestR2)
            {
               OverallBestI = i;
               OverallBestJ = BestJ;
               OverallBestR2 = BestR2;
            }
         }
      }

      // if(IterationCount % 1000 == 0)
      // {
      //    cout << "Iteration " << IterationCount << endl;
      //    cout << Ghosts.size() << " " << Particles.size() << endl;
      //    cout << "OverallBest I,J,R2 = " << OverallBestI << " " << OverallBestJ << " " << OverallBestR2 << endl;
      // }

      if(OverallBestR2 >= 0 && (MaxR2 < 0 || OverallBestR2 < MaxR2))   // found a pair!
      {
         Changed = true;

         if(Particles[OverallBestI].PT > Ghosts[OverallBestJ].PT)
         {
            Particles[OverallBestI].PT = Particles[OverallBestI].PT - Ghosts[OverallBestJ].PT;
            Ghosts[OverallBestJ].PT = 0;
            GhostsAlive[OverallBestJ] = false;
         }
         else
         {
            Ghosts[OverallBestJ].PT = Ghosts[OverallBestJ].PT - Particles[OverallBestI].PT;
            Particles[OverallBestI].PT = 0;
            ParticlesAlive[OverallBestI] = false;
         }
      }
   }

   // cout << Particles.size() << " " << PT.size() << ", " << Ghosts.size() << " " << GhostPT.size() << endl;

   PT.clear();
   Eta.clear();
   Phi.clear();

   for(int i = 0; i < (int)Particles.size(); i++)
   {
      PT.push_back(Particles[i].PT);
      Eta.push_back(Particles[i].Eta);
      Phi.push_back(Particles[i].Phi);
   }

   return;
}

#ifdef USE_FJ
void DoCSFastJet(std::vector<double> &Eta, std::vector<double> &Phi, std::vector<double> &PT,
   std::vector<double> &GhostEta, std::vector<double> &GhostPhi, std::vector<double> &GhostPT,
   double MaxR2)
{
   std::vector<fastjet::PseudoJet> particles, ghosts;
   for(int i = 0; i < Eta.size(); i++)
   {
      fastjet::PseudoJet J;
      J.reset_PtYPhiM(PT[i], Eta[i], Phi[i], 0.0);
      particles.push_back(J);
   }
   for(int i = 0; i < GhostEta.size(); i++)
   {
      fastjet::PseudoJet J;
      J.reset_PtYPhiM(GhostPT[i], GhostEta[i], GhostPhi[i], 0.0);
      ghosts.push_back(J);
   }

   fastjet::contrib::ConstituentSubtractor subtractor;
   subtractor.set_distance_type(fastjet::contrib::ConstituentSubtractor::deltaR);
   subtractor.set_max_distance(MaxR2 > 0 ? sqrt(MaxR2) : -1);
   subtractor.set_alpha(2);
   subtractor.set_remove_all_zero_pt_particles(true);

   std::vector<fastjet::PseudoJet> subtracted_particles = subtractor.do_subtraction(particles, ghosts);

   PT.clear();
   Eta.clear();
   Phi.clear();

   for(int i = 0; i < (int)subtracted_particles.size(); i++)
   {
      PT.push_back(subtracted_particles[i].perp());
      Eta.push_back(subtracted_particles[i].rap());
      Phi.push_back(subtracted_particles[i].phi());
   }
}
#endif

std::string InfoString(std::string Info)
{
   return "\"" + Info + "\"";
}

std::string InfoString(char *Info)
{
   if(Info == nullptr)
      return "";
   return string(Info);
}

std::string InfoString(int Info)
{
   return to_string(Info);
}

std::string InfoString(double Info)
{
   return to_string(Info);
}

std::string InfoString(float Info)
{
   return to_string(Info);
}

std::string InfoString(bool Info)
{
   if(Info == false)
      return "false";
   if(Info == true)
      return "true";
   return "";
}

std::string InfoString(std::vector<std::string> Info)
{
   if(Info.size() == 0)
      return "{}";

   std::string Result = "{" + InfoString(Info[0]);
   for(int i = 1; i < (int)Info.size(); i++)
      Result = Result + "," + InfoString(Info[i]);
   Result = Result + "}";

   return Result;
}

double GetZWeightPbPb(double PT, double Y, double HiBin)
{
   // Tree->SetAlias("YWeight", "(1/(0.907863-0.00478487*Y+0.0190346*Y*Y+0.000807052*Y*Y*Y-0.00919791*Y*Y*Y*Y+0.000399488*Y*Y*Y*Y*Y+0.00319896*Y*Y*Y*Y*Y*Y-0.0000754237*Y*Y*Y*Y*Y*Y*Y-0.000386165*Y*Y*Y*Y*Y*Y*Y*Y))")
   double PY[9] = {0.907863, -0.00478487, 0.0190346, 0.000807052, -0.00919791, 0.000399488, 0.00319896, -0.0000754237, -0.000386165};
   double YWeight = 0;
   for(int i = 8; i >= 0; i--)
      YWeight = YWeight * Y + PY[i];

   // Tree->SetAlias("CWeight", "(1/(1.02931+2.27613*exp(-0.0162159*HiBin)-2.52141*exp(-0.0173972*HiBin)))")
   double PC[5] = {1.02931, -2.27613, 0.0162159, 2.52141, 0.0173972};
   double CWeight = PC[0] - PC[1] * exp(-PC[2] * (HiBin + 3)) - PC[3] * exp(-PC[4] * (HiBin + 3));

   // Tree->SetAlias("CWeight2", "(1/((HiBin>=45)*1+(HiBin<45)*(1.0082-0.000508604*HiBin)))")
   double CWeight2 = 1;
   if(HiBin + 3 < 45)
      CWeight2 = 1.0082 - 0.000508604 * (HiBin + 3);

   double PTWeight = 1.00407 - 0.00346508 * log(PT) / log(10);
   // cout << YWeight << " " << CWeight << " " << CWeight2 << endl;

   return 1 / (YWeight * CWeight * CWeight2 * PTWeight);
}

double GetZWeightPbPbMC(double PT, double Y, double HiBin)
{
   return GetZWeightPbPb(PT, Y, HiBin);
}

double GetZWeightPbPbData(double PT, double Y, double HiBin)
{
   double BaseWeight = GetZWeightPbPb(PT, Y, HiBin);

   double CWeight = 1;
   if(HiBin < 20)        CWeight = 0.9055;
   else if(HiBin < 40)   CWeight = 0.9242;
   else if(HiBin < 80)   CWeight = 0.9613;
   else                  CWeight = 0.9714;

   double YWeight = 1.002 + 0.002775 * Y - 0.001617 * Y * Y - 0.0003243 * Y * Y * Y - 0.00005662 * Y * Y * Y * Y;

   return BaseWeight / CWeight / YWeight;
}

double GetZWeightPbPbDataTrigger(double PT, double Y, double HiBin)
{
   double BaseWeight = GetZWeightPbPb(PT, Y, HiBin);

   double CWeight = 1;
   if(HiBin < 20)        CWeight = 9.66820e-01;
   else if(HiBin < 40)   CWeight = 9.66820e-01;
   else if(HiBin < 80)   CWeight = 9.77292e-01;
   else                  CWeight = 9.87452e-01;

   return BaseWeight / CWeight;
}

double GetZWeightPPMC(double PT, double Y)
{
   double PY[7] = {0.946307, -0.000706671, 0.0173304, 0.000445317, -0.00192675, -0.0000253594, -0.000132195};
   double YWeight = 0;
   for(int i = 6; i >= 0; i--)
      YWeight = YWeight * Y + PY[i];

   double PTWeight = 1.00301 - 0.00287347 * log(PT) / log(10);

   return 1 / (YWeight * PTWeight);
}

double GetZWeightPPData(double PT, double Y)
{
   double PY[7] = {0.957442, -0.00770481, -0.00381821, 0.00254708, 0.000737472, -0.000274649, 0.000160242};
   double YWeight = 0;
   for(int i = 6; i >= 0; i--)
      YWeight = YWeight * Y + PY[i];
   
   return GetZWeightPPMC(PT, Y) / YWeight;
}

double GetZWeightPPDataTrigger(double PT, double Y)
{
   double PY[7] = {9.87569e-01, -1.13884e-03, 6.51882e-05, 3.75163e-04, 7.16973e-05, -9.64381e-05, -1.04866e-04};
   double YWeight = 0;
   for(int i = 6; i >= 0; i--)
      YWeight = YWeight * Y + PY[i];
   
   return GetZWeightPPMC(PT, Y) / YWeight;
}

double GetVZWeightPbPb(double VZ)
{
   double P[5] = {0.99095, 0.017396, -0.000292688, 0.00000591932, 0.00000440875};
   double Weight = 0;
   for(int i = 4; i >= 0; i--)
      Weight = Weight * VZ + P[i];
   return 1 / Weight;
}

double GetVZWeightPP(double VZ)
{
   double P[5] = {1.00646, -0.00269911, -0.000138962, 0.0000118646, -0.000000400376};
   double Weight = 0;
   for(int i = 4; i >= 0; i--)
      Weight = Weight * VZ + P[i];
   return 1 / Weight;
}

int GetHiBin(double hiHF, int Variation)
{
   static int BinCount = 200;
   static double BinEdge[201] = {0, 10.5072, 11.2099, 11.8364, 12.478, 13.1194, 13.7623, 14.4081, 15.0709, 15.7532, 16.4673, 17.1881, 17.923, 18.673, 19.4865, 20.3033, 21.1536, 22.0086, 22.9046, 23.8196, 24.7924, 25.8082, 26.8714, 27.9481, 29.0828, 30.2757, 31.5043, 32.8044, 34.1572, 35.6142, 37.1211, 38.6798, 40.3116, 42.0398, 43.8572, 45.6977, 47.6312, 49.6899, 51.815, 54.028, 56.3037, 58.7091, 61.2024, 63.8353, 66.5926, 69.3617, 72.2068, 75.2459, 78.3873, 81.5916, 84.9419, 88.498, 92.1789, 95.9582, 99.8431, 103.739, 107.78, 111.97, 116.312, 120.806, 125.46, 130.269, 135.247, 140.389, 145.713, 151.212, 156.871, 162.729, 168.762, 174.998, 181.424, 188.063, 194.907, 201.942, 209.19, 216.683, 224.37, 232.291, 240.43, 248.807, 257.416, 266.256, 275.348, 284.668, 294.216, 304.053, 314.142, 324.488, 335.101, 345.974, 357.116, 368.547, 380.283, 392.29, 404.564, 417.122, 429.968, 443.116, 456.577, 470.357, 484.422, 498.78, 513.473, 528.479, 543.813, 559.445, 575.411, 591.724, 608.352, 625.344, 642.686, 660.361, 678.371, 696.749, 715.485, 734.608, 754.068, 773.846, 794.046, 814.649, 835.608, 856.972, 878.719, 900.887, 923.409, 946.374, 969.674, 993.435, 1017.62, 1042.21, 1067.28, 1092.72, 1118.64, 1144.96, 1171.71, 1198.98, 1226.67, 1254.82, 1283.46, 1312.65, 1342.21, 1372.27, 1402.85, 1433.93, 1465.49, 1497.62, 1530.29, 1563.49, 1597.22, 1631.49, 1666.37, 1701.8, 1737.75, 1774.35, 1811.51, 1849.29, 1887.75, 1926.79, 1966.6, 2006.97, 2047.99, 2089.71, 2132.1, 2175.23, 2219.17, 2263.72, 2309.2, 2355.43, 2402.47, 2450.33, 2499.05, 2548.66, 2599.16, 2650.59, 2703.03, 2756.32, 2810.75, 2866.27, 2922.91, 2980.54, 3039.47, 3099.53, 3160.98, 3223.66, 3287.71, 3353.18, 3420.34, 3489.13, 3559.72, 3632.06, 3706.18, 3782.42, 3860.78, 3941.42, 4024.52, 4110.27, 4199.4, 4292.8, 4394.49, 4519.52, 5199.95};
   static double BinEdgeDown[201] = {0, 10.5071, 11.2094, 11.8357, 12.4763, 13.117, 13.7597, 14.4049, 15.0671, 15.7491, 16.4622, 17.1812, 17.9144, 18.6674, 19.4797, 20.2963, 21.1435, 21.9974, 22.8928, 23.8068, 24.7805, 25.7931, 26.8556, 27.9308, 29.0638, 30.2582, 31.4795, 32.7816, 34.1349, 35.5834, 37.0941, 38.6474, 40.2782, 42.0035, 43.8112, 45.6576, 47.5758, 49.6381, 51.6667, 53.7353, 55.8903, 58.1259, 60.4528, 62.8712, 65.3859, 67.9968, 70.7065, 73.5231, 76.4519, 79.4922, 82.6461, 85.9264, 89.3269, 92.8562, 96.5212, 100.322, 104.262, 108.344, 112.585, 116.971, 121.521, 126.225, 131.09, 136.127, 141.328, 146.721, 152.284, 158.014, 163.935, 170.054, 176.372, 182.878, 189.602, 196.532, 203.653, 211.017, 218.599, 226.387, 234.418, 242.667, 251.16, 259.886, 268.852, 278.071, 287.498, 297.2, 307.184, 317.409, 327.894, 338.66, 349.686, 360.996, 372.607, 384.508, 396.669, 409.133, 421.86, 434.906, 448.258, 461.916, 475.906, 490.16, 504.74, 519.663, 534.911, 550.453, 566.322, 582.525, 599.08, 615.968, 633.211, 650.805, 668.76, 687.048, 705.707, 724.774, 744.163, 763.9, 783.999, 804.528, 825.432, 846.746, 868.429, 890.523, 913.007, 935.952, 959.211, 982.919, 1007.08, 1031.63, 1056.62, 1082.08, 1107.96, 1134.24, 1160.99, 1188.22, 1215.91, 1244.06, 1272.69, 1301.85, 1331.45, 1361.51, 1392.07, 1423.18, 1454.77, 1486.93, 1519.57, 1552.81, 1586.55, 1620.87, 1655.79, 1691.26, 1727.27, 1763.93, 1801.12, 1838.97, 1877.47, 1916.61, 1956.45, 1996.89, 2038.04, 2079.84, 2122.35, 2165.52, 2209.53, 2254.24, 2299.83, 2346.19, 2393.31, 2441.28, 2490.16, 2539.86, 2590.57, 2642.16, 2694.74, 2748.23, 2802.81, 2858.47, 2915.33, 2973.2, 3032.28, 3092.56, 3154.24, 3217.19, 3281.45, 3347.18, 3414.6, 3483.65, 3554.56, 3627.2, 3701.66, 3778.25, 3856.97, 3937.98, 4021.48, 4107.62, 4197.21, 4291.05, 4393.19, 4518.6, 5199.95};
   static double BinEdgeUp[201] = {0, 10.5075, 11.2107, 11.838, 12.4797, 13.1213, 13.7641, 14.4124, 15.0745, 15.7577, 16.473, 17.1939, 17.9297, 18.6812, 19.4958, 20.3143, 21.1648, 22.0218, 22.9159, 23.8328, 24.8059, 25.8204, 26.89, 27.9702, 29.1042, 30.3022, 31.528, 32.8347, 34.1896, 35.6439, 37.1542, 38.7172, 40.3518, 42.091, 43.9053, 45.7415, 47.6853, 49.7457, 51.8755, 54.0983, 56.3594, 58.7848, 61.2861, 63.9228, 66.6825, 69.4421, 72.297, 75.3547, 78.4967, 81.6977, 85.0755, 88.6211, 92.3058, 96.1071, 99.9975, 104.065, 108.272, 112.512, 116.906, 121.601, 126.465, 131.482, 136.866, 142.229, 147.786, 153.546, 159.571, 165.586, 171.902, 178.419, 185.063, 191.856, 199.055, 206.261, 213.999, 221.719, 229.671, 237.84, 246.088, 254.828, 263.883, 272.907, 282.236, 291.925, 301.519, 311.477, 321.691, 332.153, 342.892, 353.878, 365.161, 376.742, 388.577, 400.684, 413.075, 425.746, 438.711, 451.989, 465.556, 479.45, 493.608, 508.077, 522.891, 538.003, 553.415, 569.151, 585.216, 601.601, 618.354, 635.422, 652.84, 670.599, 688.699, 707.161, 726.014, 745.185, 764.687, 784.557, 804.838, 825.489, 846.537, 867.951, 889.752, 911.955, 934.588, 957.52, 980.912, 1004.73, 1028.94, 1053.57, 1078.67, 1104.17, 1130.07, 1156.39, 1183.2, 1210.47, 1238.17, 1266.38, 1295.02, 1324.16, 1353.71, 1383.77, 1414.35, 1445.41, 1477, 1509.09, 1541.74, 1574.88, 1608.59, 1642.83, 1677.66, 1713.07, 1748.98, 1785.47, 1822.63, 1860.33, 1898.72, 1937.73, 1977.42, 2017.71, 2058.62, 2100.25, 2142.57, 2185.56, 2229.38, 2273.91, 2319.2, 2365.33, 2412.22, 2459.94, 2508.52, 2557.98, 2608.35, 2659.61, 2711.86, 2765, 2819.23, 2874.58, 2930.97, 2988.46, 3047.12, 3106.95, 3168.15, 3230.6, 3294.37, 3359.58, 3426.47, 3494.95, 3565.21, 3637.21, 3711.03, 3786.91, 3864.85, 3945.11, 4027.8, 4113.06, 4201.73, 4294.72, 4395.9, 4520.5, 5199.95};
   static double MCBinEdge[201] = {0.000000000e+00,1.221866608e+01,1.303708744e+01,1.376741886e+01,1.451291370e+01,1.526025295e+01,1.600864601e+01,1.676231384e+01,1.753350639e+01,1.832830048e+01,1.915963364e+01,1.999889565e+01,2.085316849e+01,2.172974205e+01,2.267727470e+01,2.363133430e+01,2.462081909e+01,2.561552811e+01,2.665853500e+01,2.772228050e+01,2.886320305e+01,3.004099464e+01,3.128650284e+01,3.254307938e+01,3.386550522e+01,3.525390244e+01,3.669124985e+01,3.820637894e+01,3.978758240e+01,4.148179626e+01,4.324157333e+01,4.506051636e+01,4.696524811e+01,4.899179459e+01,5.109997177e+01,5.324172974e+01,5.550939560e+01,5.792085266e+01,6.038174057e+01,6.297782898e+01,6.560985565e+01,6.843524933e+01,7.135434723e+01,7.441539764e+01,7.762516785e+01,8.084247589e+01,8.416105652e+01,8.773950195e+01,9.139731598e+01,9.512862396e+01,9.905710602e+01,1.031847458e+02,1.074815903e+02,1.119287720e+02,1.164504089e+02,1.211777344e+02,1.260805893e+02,1.309948730e+02,1.361708069e+02,1.416123962e+02,1.472980804e+02,1.531386566e+02,1.594185638e+02,1.656330414e+02,1.721137543e+02,1.788811798e+02,1.858440247e+02,1.928451691e+02,2.002444916e+02,2.078301239e+02,2.155285797e+02,2.234885254e+02,2.318784180e+02,2.402543335e+02,2.493193817e+02,2.583033752e+02,2.675081482e+02,2.770369873e+02,2.867287598e+02,2.968448181e+02,3.074582214e+02,3.178819885e+02,3.287866211e+02,3.400741882e+02,3.512948608e+02,3.629790039e+02,3.751250610e+02,3.871965332e+02,3.996041565e+02,4.125161438e+02,4.256829224e+02,4.390009766e+02,4.526673584e+02,4.668159485e+02,4.810065918e+02,4.956786194e+02,5.105879822e+02,5.261378784e+02,5.417820435e+02,5.576412964e+02,5.741410522e+02,5.910710449e+02,6.083785400e+02,6.260684814e+02,6.436163330e+02,6.618848267e+02,6.802882690e+02,6.994486084e+02,7.189249878e+02,7.389677734e+02,7.589831543e+02,7.794594727e+02,8.003760986e+02,8.216380005e+02,8.435552368e+02,8.657709351e+02,8.883392334e+02,9.110311279e+02,9.349785156e+02,9.585595093e+02,9.825820312e+02,1.007016602e+03,1.031899536e+03,1.057812256e+03,1.084011597e+03,1.111714233e+03,1.138206909e+03,1.165716187e+03,1.193733154e+03,1.221653320e+03,1.251507690e+03,1.281227417e+03,1.311011963e+03,1.341104126e+03,1.372396362e+03,1.404285767e+03,1.436516602e+03,1.468652344e+03,1.501910034e+03,1.535562012e+03,1.569694702e+03,1.604690063e+03,1.640646484e+03,1.676045776e+03,1.712617920e+03,1.749282837e+03,1.787432129e+03,1.825893921e+03,1.866071289e+03,1.906581055e+03,1.947844116e+03,1.989663940e+03,2.031399536e+03,2.072800781e+03,2.115318115e+03,2.159495117e+03,2.205225830e+03,2.252677734e+03,2.298583740e+03,2.345652588e+03,2.393358154e+03,2.442872314e+03,2.491453369e+03,2.541042969e+03,2.592810303e+03,2.645517822e+03,2.699104248e+03,2.753293457e+03,2.807929688e+03,2.864374023e+03,2.922601807e+03,2.979420898e+03,3.038675049e+03,3.098716309e+03,3.159294434e+03,3.221664062e+03,3.285901611e+03,3.350948486e+03,3.415805664e+03,3.482691406e+03,3.552617432e+03,3.623614502e+03,3.694631104e+03,3.767252930e+03,3.840279297e+03,3.917036621e+03,3.993660400e+03,4.073357666e+03,4.154333008e+03,4.238127930e+03,4.322208496e+03,4.409825195e+03,4.498888672e+03,4.589717773e+03,4.681564941e+03,4.777086914e+03,4.877952637e+03,4.987053711e+03,5.113039062e+03,5.279584473e+03,9.999999999e+03};

   for(int i = 0; i < BinCount; i++)
   {
      if(Variation == 100 && hiHF >= MCBinEdge[i] && hiHF < MCBinEdge[i+1])
         return BinCount - 1 - i;
      if(Variation == 0 && hiHF >= BinEdge[i] && hiHF < BinEdge[i+1])
         return BinCount - 1 - i;
      if(Variation == 1 && hiHF >= BinEdgeUp[i] && hiHF < BinEdgeUp[i+1])
         return BinCount - 1 - i;
      if(Variation == -1 && hiHF >= BinEdgeDown[i] && hiHF < BinEdgeDown[i+1])
         return BinCount - 1 - i;
   }

   return -1;
}

double GetInterSampleZWeight(double PT)
{
   // This function applies on pp DYLL to get embedded DYLL based on Z PT

   if(PT < 1)
      PT = 1;
   if(PT > 150)
      PT = 150;

   double P0 =     0.985164;
   double P1 =  -0.00116944;
   double P2 =  5.26231e-05;
   double P3 = -2.21173e-07;

   double Weight = P0 + P1 * PT + P2 * PT * PT + P3 * PT * PT * PT;

   return Weight;
}



#endif
