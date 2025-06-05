//----------------------------------------------------------------------------
#ifndef TauHelperFunctions_15763_AJSDKGIRIKANSVCGKISNCGIKNHKZSG
#define TauHelperFunctions_15763_AJSDKGIRIKANSVCGKISNCGIKNHKZSG
//----------------------------------------------------------------------------
// FourVector class - with a lot of extra setters and getter
// Author: Yi Chen (original version on 6624)
//----------------------------------------------------------------------------
#include <vector>
#include <cmath>
#include <ostream>
//----------------------------------------------------------------------------
#include "DrawRandom.h"
//----------------------------------------------------------------------------
// Categories:
//    -1. not final tau (goes into tau gamma)
//    1. mu nu nu
//    2. e nu nu
//    3. nu pi-
//    4. nu pi0 pi-
//    5. nu pi0 pi0 pi-
//    6. nu pi+ pi- pi-
//    7. nu pi+ pi- pi- pi0
//    8. Otherwise
//----------------------------------------------------------------------------
#define PI 3.14159265358979323846264338327950288479716939937510
//----------------------------------------------------------------------------
class FourVector;
std::ostream &operator <<(std::ostream &out, FourVector P);
FourVector operator *(double Scale, FourVector P);
double GetAngle(FourVector P1, FourVector P2);
double GetDR(FourVector P1, FourVector P2);
double GetDPhi(FourVector P1, FourVector P2);
double EPS(FourVector A, FourVector B, FourVector C, FourVector D);
double GetMT(FourVector P1, FourVector P2);
double GetMinRadius(FourVector P1, FourVector P2, FourVector P3);   // in eta-phi space
double GetMinRadius(double X1, double Y1, double X2, double Y2, double X3, double Y3);
double GetMR(FourVector P1, FourVector P2);
double GetMRStar(FourVector P1, FourVector P2);
double Get2011MR(FourVector P1, FourVector P2);
double GetISRRemovedMR(FourVector P1, FourVector P2, FourVector POther, double ME3Assumption = 0);
double GetISRRemoved2011MR(FourVector P1, FourVector P2, FourVector POther, double ME3Assumption = 0);
double GetISR2011MR(FourVector P1, FourVector P2, FourVector ME, int Assumption = 0);
double GetMRT(FourVector P1, FourVector P2, FourVector ME);
double GetR(FourVector P1, FourVector P2, FourVector ME);
double GetRStar(FourVector P1, FourVector P2, FourVector ME);
double Get2011R(FourVector P1, FourVector P2, FourVector ME);
double GetISRRemovedR(FourVector P1, FourVector P2, FourVector POther, double ME3Assumption = 0);
double GetISRRemoved2011R(FourVector P1, FourVector P2, FourVector POther, double ME3Assumption = 0);
double GetISR2011R(FourVector P1, FourVector P2, FourVector ME, int Assumption = 0, char AdditionalVariant = 'g');
double GetGammaRStar(FourVector P1, FourVector P2);
double BetaToGamma(double Beta);
double GammaToBeta(double Gamma);
std::vector<FourVector> SplitIntoGroups(std::vector<FourVector> &Input, bool ZeroMass = true);
double GetDifference8(FourVector &P1, FourVector &P2, FourVector &ME, double BetaX);
double GetDifference9(FourVector &P1, FourVector &P2, FourVector &ME, double BetaZ);
double FindMR11MinimumPz(FourVector J1, FourVector J2, FourVector ME, FourVector ISR);
double EstimateMass11(FourVector J1, FourVector J2, FourVector ME, FourVector ISR, bool Reversal = false);
double EstimateTransverseMass11(FourVector J1, FourVector J2, FourVector ME, FourVector ISR, char Variant = 'g', bool Reversal = false);
// int FindCategory(GenParticleTree &Tree, int index);
//----------------------------------------------------------------------------
class FourVector
{
public:
   double P[4];
public:
   FourVector();
   FourVector(double p[4]);
   FourVector(double e, double px, double py, double pz);
   ~FourVector();
   void SetPtEtaPhi(double pt, double eta, double phi);   // massless
   void SetPtEtaPhiMass(double pt, double eta, double phi, double mass = 0);
   void SetPtYPhi(double pt, double y, double phi);   // massless
   void SetPtYPhiMass(double pt, double y, double phi, double mass = 0);
   void SetSizeEtaPhi(double size, double eta, double phi);
   void SetSizeEtaPhiMass(double size, double eta, double phi, double mass = 0);
   void SetSizeEtaPhiEnergy(double size, double eta, double phi, double energy);
   void SetSizeThetaPhi(double size, double theta, double phi);
   void SetSizeThetaPhiMass(double size, double theta, double phi, double mass = 0);
   double &operator [](int index);
   double operator [](int index) const;
   FourVector &operator =(const FourVector &Other);
   FourVector operator +(const FourVector &Other) const;
   FourVector operator -() const;
   FourVector operator -(const FourVector &Other) const;
   double operator *(FourVector other);
   FourVector operator *(double Scale) const;
   FourVector operator /(double Scale) const;
   bool operator <(const FourVector &other) const;
public:
   double InnerP[4];
   double InnerEta;
   double InnerPhi;
   double InnerMomentum;
   double InnerPT;
   double InnerET;
   double InnerMass2;
   double InnerY;
   double InnerTheta;
   void CalculateInnerQuantities();
   bool IsModified();
   void CheckModified();
   double InnerGetEta() const;
   double InnerGetPhi() const;
   double InnerGetP() const;
   double InnerGetPT() const;
   double InnerGetET() const;
   double InnerGetMass2() const;
   double InnerGetY() const;
   double InnerGetTheta() const;
public:
   double GetMass();
   double GetMass2();
   double GetP();
   double GetP2();
   double GetPT();
   double GetPT2();
   double GetET();
   double GetET2();
   double GetEta();
   double GetAbsEta();
   double GetRapidity();
   double GetY();
   double GetPhi();
   double GetTheta();
   double GetGamma();
   double GetBeta();
   FourVector RotateX(double Angle) const;
   FourVector RotateY(double Angle) const;
   FourVector RotateZ(double Angle) const;
   FourVector Rotate(FourVector Axis, double Angle);
   FourVector BoostX(double Beta) const;
   FourVector BoostY(double Beta) const;
   FourVector BoostZ(double Beta) const;
   FourVector Boost(FourVector Axis, double Beta) const;
   FourVector GammaBoostX(double Gamma) const;
   FourVector GammaBoostY(double Gamma) const;
   FourVector GammaBoostZ(double Gamma) const;
   FourVector GammaBoost(FourVector Axis, double Gamma) const;
   FourVector SmearAngle(double Angle);
   FourVector SmearMomentum(double Scale) const;
   FourVector SpatialCross(FourVector Other) const;
   FourVector SpatialNormalize();
   double SpatialDot(FourVector Other) const;
   double MetricDot(FourVector Other) const;
};
//----------------------------------------------------------------------------
#endif
