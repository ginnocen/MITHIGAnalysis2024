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
#include "TauHelperFunctions3.h"
//----------------------------------------------------------------------------
#define PI 3.14159265358979323846264338327950288479716939937510
//----------------------------------------------------------------------------
FourVector::FourVector()
{
   P[0] = 0;
   P[1] = 0;
   P[2] = 0;
   P[3] = 0;

   CalculateInnerQuantities();
}
//----------------------------------------------------------------------------
FourVector::FourVector(double p[4])
{
   P[0] = p[0];
   P[1] = p[1];
   P[2] = p[2];
   P[3] = p[3];

   CalculateInnerQuantities();
}
//----------------------------------------------------------------------------
FourVector::FourVector(double e, double px, double py, double pz)
{
   P[0] = e;
   P[1] = px;
   P[2] = py;
   P[3] = pz;

   CalculateInnerQuantities();
}
//----------------------------------------------------------------------------
FourVector::~FourVector()
{
}
//----------------------------------------------------------------------------
void FourVector::SetPtEtaPhi(double pt, double eta, double phi)
{
   SetPtEtaPhiMass(pt, eta, phi, 0);
}
//----------------------------------------------------------------------------
void FourVector::SetPtEtaPhiMass(double pt, double eta, double phi, double mass)
{
   P[1] = pt * cos(phi);
   P[2] = pt * sin(phi);
   P[3] = pt * sinh(eta);

   P[0] = sqrt(mass * mass + SpatialDot(*this));

   CalculateInnerQuantities();
}
//----------------------------------------------------------------------------
void FourVector::SetPtYPhi(double pt, double y, double phi)
{
   SetPtYPhiMass(pt, y, phi, 0);
}
//----------------------------------------------------------------------------
void FourVector::SetPtYPhiMass(double pt, double y, double phi, double mass)
{
   P[1] = pt * cos(phi);
   P[2] = pt * sin(phi);
   P[0] = sqrt(pt * pt +  mass * mass) * cosh(y);
   P[3] = P[0] * tanh(y);

   CalculateInnerQuantities();
}
//----------------------------------------------------------------------------
void FourVector::SetSizeEtaPhi(double size, double eta, double phi)
{
   SetSizeEtaPhiMass(size, eta, phi, 0);
}
//----------------------------------------------------------------------------
void FourVector::SetSizeEtaPhiMass(double size, double eta, double phi, double mass)
{
   P[0] = sqrt(size * size + mass * mass);
   P[1] = size / cosh(eta) * cos(phi);
   P[2] = size / cosh(eta) * sin(phi);
   P[3] = size * tanh(eta);

   CalculateInnerQuantities();
}
//----------------------------------------------------------------------------
void FourVector::SetSizeEtaPhiEnergy(double size, double eta, double phi, double energy)
{
   P[0] = energy;
   P[1] = size / cosh(eta) * cos(phi);
   P[2] = size / cosh(eta) * sin(phi);
   P[3] = size * tanh(eta);

   CalculateInnerQuantities();
}
//----------------------------------------------------------------------------
void FourVector::SetSizeThetaPhi(double size, double theta, double phi)
{
   SetSizeThetaPhiMass(size, theta, phi, 0);
}
//----------------------------------------------------------------------------
void FourVector::SetSizeThetaPhiMass(double size, double theta, double phi, double mass)
{
   P[0] = sqrt(size * size + mass * mass);
   P[1] = size * sin(theta) * cos(phi);
   P[2] = size * sin(theta) * sin(phi);
   P[3] = size * cos(theta);

   CalculateInnerQuantities();
}
//----------------------------------------------------------------------------
double &FourVector::operator [](int index)
{
   if(index >= 0 && index <= 3)
      return P[index];
   return P[0];
}
//----------------------------------------------------------------------------
double FourVector::operator [](int index) const
{
   if(index >= 0 && index <= 3)
      return P[index];
   return 0;
}
//----------------------------------------------------------------------------
FourVector &FourVector::operator =(const FourVector &Other)
{
   P[0] = Other.P[0];
   P[1] = Other.P[1];
   P[2] = Other.P[2];
   P[3] = Other.P[3];

   CalculateInnerQuantities();

   return *this;
}
//----------------------------------------------------------------------------
FourVector FourVector::operator +(const FourVector &Other) const
{
   FourVector Out;
   Out.P[0] = P[0] + Other.P[0];
   Out.P[1] = P[1] + Other.P[1];
   Out.P[2] = P[2] + Other.P[2];
   Out.P[3] = P[3] + Other.P[3];
   return Out;
}
//----------------------------------------------------------------------------
FourVector FourVector::operator -() const
{
   FourVector Out;
   Out.P[0] = -P[0];
   Out.P[1] = -P[1];
   Out.P[2] = -P[2];
   Out.P[3] = -P[3];
   return Out;
}
//----------------------------------------------------------------------------
FourVector FourVector::operator -(const FourVector &Other) const
{
   FourVector Out;
   Out.P[0] = P[0] - Other.P[0];
   Out.P[1] = P[1] - Other.P[1];
   Out.P[2] = P[2] - Other.P[2];
   Out.P[3] = P[3] - Other.P[3];
   return Out;
}
//----------------------------------------------------------------------------
double FourVector::operator *(FourVector other)
{
   return MetricDot(other);
}
//----------------------------------------------------------------------------
FourVector FourVector::operator *(double Scale) const
{
   FourVector Out;
   Out.P[0] = P[0] * Scale;
   Out.P[1] = P[1] * Scale;
   Out.P[2] = P[2] * Scale;
   Out.P[3] = P[3] * Scale;
   return Out;
}
//----------------------------------------------------------------------------
FourVector FourVector::operator /(double Scale) const
{
   FourVector Out;
   Out.P[0] = P[0] / Scale;
   Out.P[1] = P[1] / Scale;
   Out.P[2] = P[2] / Scale;
   Out.P[3] = P[3] / Scale;
   return Out;
}
//----------------------------------------------------------------------------
bool FourVector::operator <(const FourVector &Other) const
{
   if(P[0] < Other.P[0])   return true;
   if(P[0] > Other.P[0])   return false;
   if(P[1] < Other.P[1])   return true;
   if(P[1] > Other.P[1])   return false;
   if(P[2] < Other.P[2])   return true;
   if(P[2] > Other.P[2])   return false;
   if(P[3] < Other.P[3])   return true;
   if(P[3] > Other.P[3])   return false;

   return false;
}
//----------------------------------------------------------------------------
void FourVector::CalculateInnerQuantities()
{
   InnerP[0] = P[0];
   InnerP[1] = P[1];
   InnerP[2] = P[2];
   InnerP[3] = P[3];

   InnerEta = InnerGetEta();
   InnerPhi = InnerGetPhi();
   InnerMomentum = InnerGetP();
   InnerPT = InnerGetPT();
   InnerET = InnerGetET();
   InnerMass2 = InnerGetMass2();
   InnerY = InnerGetY();
   InnerTheta = InnerGetTheta();
}
//----------------------------------------------------------------------------
bool FourVector::IsModified()
{
   if(P[0] != InnerP[0])   return true;
   if(P[1] != InnerP[1])   return true;
   if(P[2] != InnerP[2])   return true;
   if(P[3] != InnerP[3])   return true;

   return false;
}
//----------------------------------------------------------------------------
void FourVector::CheckModified()
{
   if(IsModified() == true)
      CalculateInnerQuantities();
}
//----------------------------------------------------------------------------
double FourVector::InnerGetEta() const
{
   double Momentum = InnerGetP();
   return 0.5 * log((Momentum + InnerP[3]) / (Momentum - InnerP[3]));
}
//----------------------------------------------------------------------------
double FourVector::InnerGetPhi() const
{
   double PT = InnerGetPT();

   double Angle = acos(InnerP[1] / PT);
   if(InnerP[2] < 0)
      Angle = -Angle;

   return Angle;
}
//----------------------------------------------------------------------------
double FourVector::InnerGetP() const
{
   return sqrt(InnerP[1] * InnerP[1] + InnerP[2] * InnerP[2] + InnerP[3] * InnerP[3]);
}
//----------------------------------------------------------------------------
double FourVector::InnerGetPT() const
{
   return sqrt(InnerP[1] * InnerP[1] + InnerP[2] * InnerP[2]);
}
//----------------------------------------------------------------------------
double FourVector::InnerGetET() const
{
   return InnerGetPT() / InnerGetP() * InnerP[0];
}
//----------------------------------------------------------------------------
double FourVector::InnerGetMass2() const
{
   return InnerP[0] * InnerP[0] - InnerP[1] * InnerP[1] - InnerP[2] * InnerP[2] - InnerP[3] * InnerP[3];
}
//----------------------------------------------------------------------------
double FourVector::InnerGetY() const
{
   return 0.5 * log((InnerP[0] + InnerP[3]) / (InnerP[0] - InnerP[3]));
}
//----------------------------------------------------------------------------
double FourVector::InnerGetTheta() const
{
   return acos(InnerP[3] / InnerGetP());
}
//----------------------------------------------------------------------------
double FourVector::GetMass()
{
   CheckModified();
   
   if(InnerMass2 >= 0)
      return sqrt(InnerMass2);
   return 0;
}
//----------------------------------------------------------------------------
double FourVector::GetMass2()
{
   CheckModified();

   return InnerMass2;
}
//----------------------------------------------------------------------------
double FourVector::GetP()
{
   CheckModified();
   
   return InnerMomentum;
}
//----------------------------------------------------------------------------
double FourVector::GetP2()
{
   CheckModified();

   return InnerMomentum * InnerMomentum;
}
//----------------------------------------------------------------------------
double FourVector::GetPT()
{
   CheckModified();

   return InnerPT;
}
//----------------------------------------------------------------------------
double FourVector::GetPT2()
{
   CheckModified();

   return InnerPT * InnerPT;
}
//----------------------------------------------------------------------------
double FourVector::GetET()
{
   CheckModified();

   return InnerET;
}
//----------------------------------------------------------------------------
double FourVector::GetET2()
{
   CheckModified();

   return InnerET * InnerET;
}
//----------------------------------------------------------------------------
double FourVector::GetEta()
{
   CheckModified();

   return InnerEta;
}
//----------------------------------------------------------------------------
double FourVector::GetAbsEta()
{
   CheckModified();

   double Eta = InnerEta;

   if(Eta < 0)
      Eta = -Eta;

   return Eta;
}
//----------------------------------------------------------------------------
double FourVector::GetRapidity()
{
   CheckModified();

   return InnerY;
}
//----------------------------------------------------------------------------
double FourVector::GetY()
{
   return GetRapidity();
}
//----------------------------------------------------------------------------
double FourVector::GetPhi()
{
   CheckModified();

   return InnerPhi;
}
//----------------------------------------------------------------------------
double FourVector::GetTheta()
{
   CheckModified();

   return InnerTheta;
}
//----------------------------------------------------------------------------
double FourVector::GetBeta()
{
   double Gamma = GetGamma();
   return sqrt(1 - 1 / (Gamma * Gamma));
}
//----------------------------------------------------------------------------
double FourVector::GetGamma()
{
   return P[0] / GetMass();
}
//----------------------------------------------------------------------------
FourVector FourVector::RotateX(double Angle) const
{
   FourVector Out;
   Out.P[0] = P[0];
   Out.P[1] = P[1];
   Out.P[2] = cos(Angle) * P[2] - sin(Angle) * P[3];
   Out.P[3] = sin(Angle) * P[2] + cos(Angle) * P[3];
   return Out;
}
//----------------------------------------------------------------------------
FourVector FourVector::RotateY(double Angle) const
{
   FourVector Out;
   Out.P[0] = P[0];
   Out.P[1] = sin(Angle) * P[3] + cos(Angle) * P[1];
   Out.P[2] = P[2];
   Out.P[3] = cos(Angle) * P[3] - sin(Angle) * P[1];
   return Out;
}
//----------------------------------------------------------------------------
FourVector FourVector::RotateZ(double Angle) const
{
   FourVector Out;
   Out.P[0] = P[0];
   Out.P[1] = cos(Angle) * P[1] - sin(Angle) * P[2];
   Out.P[2] = sin(Angle) * P[1] + cos(Angle) * P[2];
   Out.P[3] = P[3];
   return Out;
}
//----------------------------------------------------------------------------
FourVector FourVector::Rotate(FourVector Axis, double Angle)
{
   // rotate "axis" and input to y-z plane, then rotate "axis" to z axis,
   //    rotate input with respect to z axis
   //    and then rotate back

   double Psi = PI / 2 - Axis.GetPhi();
   double Theta = acos(Axis[3] / Axis.GetP());

   return RotateZ(Psi).RotateX(Theta).RotateZ(Angle).RotateX(-Theta).RotateZ(-Psi);
}
//----------------------------------------------------------------------------
FourVector FourVector::BoostX(double Beta) const
{
   double Gamma = BetaToGamma(Beta);

   FourVector Out;
   Out.P[0] = Gamma * P[0] + Beta * Gamma * P[1];
   Out.P[1] = Beta * Gamma * P[0] + Gamma * P[1];
   Out.P[2] = P[2];
   Out.P[3] = P[3];
   return Out;
}
//----------------------------------------------------------------------------
FourVector FourVector::BoostY(double Beta) const
{
   double Gamma = BetaToGamma(Beta);

   FourVector Out;
   Out.P[0] = Gamma * P[0] + Beta * Gamma * P[2];
   Out.P[1] = P[1];
   Out.P[2] = Beta * Gamma * P[0] + Gamma * P[2];
   Out.P[3] = P[3];
   return Out;
}
//----------------------------------------------------------------------------
FourVector FourVector::BoostZ(double Beta) const
{
   double Gamma = BetaToGamma(Beta);

   FourVector Out;
   Out.P[0] = Gamma * P[0] + Beta * Gamma * P[3];
   Out.P[1] = P[1];
   Out.P[2] = P[2];
   Out.P[3] = Beta * Gamma * P[0] + Gamma * P[3];
   return Out;
}
//----------------------------------------------------------------------------
FourVector FourVector::Boost(FourVector Axis, double Beta) const
{
   if(Axis.GetPT() < 1e-8)   // axis along z direction
   {
      if(Axis[3] > 0)
         return BoostZ(Beta);
      else
         return BoostZ(-Beta);
   }

   double Psi = PI / 2 - Axis.GetPhi();
   double Theta = acos(Axis[3] / Axis.GetP());

   return RotateZ(Psi).RotateX(Theta).BoostZ(Beta).RotateX(-Theta).RotateZ(-Psi);
}
//----------------------------------------------------------------------------
FourVector FourVector::GammaBoostX(double Gamma) const
{
   double Beta = GammaToBeta(Gamma);
   if(Gamma < 0)
   {
      Gamma = -Gamma;
      Beta = -Beta;
   }

   FourVector Out;
   Out.P[0] = Gamma * P[0] + Beta * Gamma * P[1];
   Out.P[1] = Beta * Gamma * P[0] + Gamma * P[1];
   Out.P[2] = P[2];
   Out.P[3] = P[3];
   return Out;
}
//----------------------------------------------------------------------------
FourVector FourVector::GammaBoostY(double Gamma) const
{
   double Beta = GammaToBeta(Gamma);
   if(Gamma < 0)
   {
      Gamma = -Gamma;
      Beta = -Beta;
   }

   FourVector Out;
   Out.P[0] = Gamma * P[0] + Beta * Gamma * P[2];
   Out.P[1] = P[1];
   Out.P[2] = Beta * Gamma * P[0] + Gamma * P[2];
   Out.P[3] = P[3];
   return Out;
}
//----------------------------------------------------------------------------
FourVector FourVector::GammaBoostZ(double Gamma) const
{
   double Beta = GammaToBeta(Gamma);
   if(Gamma < 0)
   {
      Gamma = -Gamma;
      Beta = -Beta;
   }

   FourVector Out;
   Out.P[0] = Gamma * P[0] + Beta * Gamma * P[3];
   Out.P[1] = P[1];
   Out.P[2] = P[2];
   Out.P[3] = Beta * Gamma * P[0] + Gamma * P[3];
   return Out;
}
//----------------------------------------------------------------------------
FourVector FourVector::GammaBoost(FourVector Axis, double Gamma) const
{
   if(Axis.GetPT() < 1e-8)   // axis along z direction
   {
      if(Axis[3] > 0)
         return GammaBoostZ(Gamma);
      else
         return GammaBoostZ(-Gamma);
   }

   double Psi = PI / 2 - Axis.GetPhi();
   double Theta = acos(Axis[3] / Axis.GetP());

   return RotateZ(Psi).RotateX(Theta).GammaBoostZ(Gamma).RotateX(-Theta).RotateZ(-Psi);
}
//----------------------------------------------------------------------------
FourVector FourVector::SmearAngle(double Angle)
{
   FourVector Reference(0, 1, 0, 0);
   if(fabs(P[2]) < 1e-6 && fabs(P[3]) < 1e-6)
      Reference[2] = 1;

   FourVector Axis = SpatialCross(Reference);   // so that axis is perpendicular to input momentum

   FourVector RealAxis;   // pick a random rotation axis perpendicular to input momentum
   // double AxisRotation = DrawRandom(0, 2 * PI);
   double AxisRotation = 0;
   RealAxis = Axis.Rotate(*this, AxisRotation);

   // double SmearAngle = DrawGaussian(Angle);
   double SmearAngle = 0;
   return Rotate(RealAxis, SmearAngle);
}
//----------------------------------------------------------------------------
FourVector FourVector::SmearMomentum(double Scale) const
{
   // double Factor = 1 + DrawGaussian(Scale);
   double Factor = 1;

   return (*this) * Factor;
}
//----------------------------------------------------------------------------
FourVector FourVector::SpatialCross(FourVector Other) const
{
   FourVector Out;
   Out.P[0] = P[0];
   Out.P[1] = P[2] * Other.P[3] - P[3] * Other.P[2];
   Out.P[2] = P[3] * Other.P[1] - P[1] * Other.P[3];
   Out.P[3] = P[1] * Other.P[2] - P[2] * Other.P[1];
   return Out;
}
//----------------------------------------------------------------------------
FourVector FourVector::SpatialNormalize()
{
   FourVector Out;
   Out = (*this) / GetP();
   return Out;
}
//----------------------------------------------------------------------------
double FourVector::SpatialDot(FourVector Other) const
{
   return P[1] * Other.P[1] + P[2] * Other.P[2] + P[3] * Other.P[3];
}
//----------------------------------------------------------------------------
double FourVector::MetricDot(FourVector Other) const
{
   return P[0] * Other.P[0] - SpatialDot(Other);
}
//----------------------------------------------------------------------------
std::ostream &operator <<(std::ostream &out, FourVector P)
{
   out << "(" << P[0] << ", " << P[1] << ", " << P[2] << ", " << P[3] << ")";
   return out;
}
//----------------------------------------------------------------------------
FourVector operator *(double Scale, FourVector P)
{
   return P * Scale;
}
//----------------------------------------------------------------------------
double GetAngle(FourVector P1, FourVector P2)
{
   double V = P1.SpatialDot(P2) / P1.GetP() / P2.GetP();
   if(V >= 1)
      return 0;
   if(V <= -1)
      return M_PI;
   return acos(V);
}
//----------------------------------------------------------------------------
double GetDR(FourVector P1, FourVector P2)
{
   double DEta = P1.GetEta() - P2.GetEta();
   double DPhi = GetDPhi(P1, P2);

   return sqrt(DPhi * DPhi + DEta * DEta);
}
//----------------------------------------------------------------------------
double GetDPhi(FourVector P1, FourVector P2)
{
   double DPhi = P1.GetPhi() - P2.GetPhi();

   if(DPhi > PI)
      DPhi = 2 * PI - DPhi;
   if(DPhi < -PI)
      DPhi = DPhi + 2 * PI;

   return DPhi;
}
//----------------------------------------------------------------------------
double EPS(FourVector A, FourVector B, FourVector C, FourVector D)
{
   double Result = 0;

   Result = Result + A[1] * B[3] * C[2] * D[0];
   Result = Result - A[1] * B[2] * C[3] * D[0];
   Result = Result - A[0] * B[3] * C[2] * D[1];
   Result = Result + A[0] * B[2] * C[3] * D[1];
   Result = Result - A[1] * B[3] * C[0] * D[2];
   Result = Result + A[0] * B[3] * C[1] * D[2];
   Result = Result + A[1] * B[0] * C[3] * D[2];
   Result = Result - A[0] * B[1] * C[3] * D[2];
   
   Result = Result + A[3] * B[2] * C[1] * D[0];
   Result = Result - A[3] * B[1] * C[2] * D[0];
   Result = Result - A[3] * B[2] * C[0] * D[1];
   Result = Result + A[3] * B[0] * C[2] * D[1];
   Result = Result + A[3] * B[1] * C[0] * D[2];
   Result = Result - A[3] * B[0] * C[1] * D[2];
  
   Result = Result + A[1] * B[2] * C[0] * D[3];
   Result = Result - A[0] * B[2] * C[1] * D[3];
   Result = Result - A[1] * B[0] * C[2] * D[3];
   Result = Result + A[0] * B[1] * C[2] * D[3];

   Result = Result - A[2] * B[3] * C[1] * D[0];
   Result = Result + A[2] * B[1] * C[3] * D[0];
   Result = Result + A[2] * B[3] * C[0] * D[1];
   Result = Result - A[2] * B[0] * C[3] * D[1];
   Result = Result - A[2] * B[1] * C[0] * D[3];
   Result = Result + A[2] * B[0] * C[1] * D[3];

   /*
   // Reordered 
   Result = Result + A[0] * B[1] * C[2] * D[3];
   Result = Result - A[0] * B[1] * C[3] * D[2];
   Result = Result - A[0] * B[2] * C[1] * D[3];
   Result = Result + A[0] * B[2] * C[3] * D[1];
   Result = Result + A[0] * B[3] * C[1] * D[2];
   Result = Result - A[0] * B[3] * C[2] * D[1];

   Result = Result - A[1] * B[0] * C[2] * D[3];
   Result = Result + A[1] * B[0] * C[3] * D[2];
   Result = Result + A[1] * B[2] * C[0] * D[3];
   Result = Result - A[1] * B[2] * C[3] * D[0];
   Result = Result - A[1] * B[3] * C[0] * D[2];
   Result = Result + A[1] * B[3] * C[2] * D[0];
   
   Result = Result + A[2] * B[0] * C[1] * D[3];
   Result = Result - A[2] * B[0] * C[3] * D[1];
   Result = Result - A[2] * B[1] * C[0] * D[3];
   Result = Result + A[2] * B[1] * C[3] * D[0];
   Result = Result + A[2] * B[3] * C[0] * D[1];
   Result = Result - A[2] * B[3] * C[1] * D[0];
   
   Result = Result - A[3] * B[0] * C[1] * D[2];
   Result = Result + A[3] * B[0] * C[2] * D[1];
   Result = Result + A[3] * B[1] * C[0] * D[2];
   Result = Result - A[3] * B[1] * C[2] * D[0];
   Result = Result - A[3] * B[2] * C[0] * D[1];
   Result = Result + A[3] * B[2] * C[1] * D[0];
   */

   return Result;
}
//----------------------------------------------------------------------------
double GetMT(FourVector P1, FourVector P2)
{
   double PT1 = P1.GetPT();
   double PT2 = P2.GetPT();

   return sqrt(2 * (PT1 * PT2 - P1[1] * P2[1] - P1[2] * P2[2]));
}
//----------------------------------------------------------------------------
double GetMinRadius(FourVector P1, FourVector P2, FourVector P3)   // in eta-phi space
{
   double Eta1 = P1.GetEta();
   double Phi1 = P1.GetPhi();
   double Eta2 = P2.GetEta();
   double Phi2 = P2.GetPhi();
   double Eta3 = P3.GetEta();
   double Phi3 = P3.GetPhi();

   double BestResidual2 = 9999999;
   double Best1 = 0;
   double Best2 = 0;
   double Best3 = 0;
   for(int i1 = 0; i1 <= 1; i1++)
   {
      for(int i2 = 0; i2 <= 1; i2++)
      {
         for(int i3 = 0; i3 <= 1; i3++)
         {
            double AveragePhi = (Phi1 + Phi2 + Phi3 + (i1 + i2 + i3) * 2 * PI) / 3;

            double Residual2 = (AveragePhi - Phi1 - i1 * 2 * PI) * (AveragePhi - Phi1 - i1 * 2 * PI)
               + (AveragePhi - Phi2 - i2 * 2 * PI) * (AveragePhi - Phi2 - i2 * 2 * PI)
               + (AveragePhi - Phi3 - i3 * 2 * PI) * (AveragePhi - Phi3 - i3 * 2 * PI);

            if(Residual2 < BestResidual2)
            {
               Best1 = i1;
               Best2 = i2;
               Best3 = i3;

               BestResidual2 = Residual2;
            }
         }
      }
   }

   return GetMinRadius(Eta1, Phi1 + Best1 * 2 * PI, Eta2, Phi2 + Best2 * 2 * PI, Eta3, Phi3 + Best3 * 2 * PI);
}
//----------------------------------------------------------------------------
double GetMinRadius(double X1, double Y1, double X2, double Y2,
   double X3, double Y3)
{
   // compare two radii:
   //    - common circle radius
   //    - maximum of edge length (divided by two)
   // return the smaller of the two

   // calculate common circle radius
   double C1X0 = 2 * (X1 - X2);
   double C1Y0 = 2 * (Y1 - Y2);
   double C1 = X1 * X1 + Y1 * Y1 - X2 * X2 - Y2 * Y2;
   
   double C2X0 = 2 * (X1 - X3);
   double C2Y0 = 2 * (Y1 - Y3);
   double C2 = X1 * X1 + Y1 * Y1 - X3 * X3 - Y3 * Y3;

   double Distance2 = 99999999;

   if(fabs(C1X0 * C2Y0 - C2X0 * C1Y0) > 1e-8)   // Otherwise three points too close to a straight line
   {
      double M = C1X0 * C2Y0 - C2X0 * C1Y0;
      double MX = C1 * C2Y0 - C2 * C1Y0;
      double MY = C1X0 * C2 - C2X0 * C1;

      double X0 = MX / M;
      double Y0 = MY / M;

      Distance2 = (X1 - X0) * (X1 - X0) + (Y1 - Y0) * (Y1 - Y0);
   }

   // calculate max of edge
   double MaxEdge2 = 0;
   if(MaxEdge2 < (X1 - X2) * (X1 - X2) + (Y1 - Y2) * (Y1 - Y2))
      MaxEdge2 = (X1 - X2) * (X1 - X2) + (Y1 - Y2) * (Y1 - Y2);
   if(MaxEdge2 < (X2 - X3) * (X2 - X3) + (Y2 - Y3) * (Y2 - Y3))
      MaxEdge2 = (X2 - X3) * (X2 - X3) + (Y2 - Y3) * (Y2 - Y3);
   if(MaxEdge2 < (X3 - X1) * (X3 - X1) + (Y3 - Y1) * (Y3 - Y1))
      MaxEdge2 = (X3 - X1) * (X3 - X1) + (Y3 - Y1) * (Y3 - Y1);
   MaxEdge2 = MaxEdge2 / 4;

   // minimum of the two
   return sqrt(std::min(MaxEdge2, Distance2));
}
//----------------------------------------------------------------------------
double GetMR(FourVector P1, FourVector P2)
{
   double Temp1 = P1[0] * P2[3] - P1[3] * P2[0];
   double Temp2 = P1[3] - P2[3];
   double Temp3 = P1[0] - P2[0];
   return 2 * sqrt(Temp1 * Temp1 / (Temp2 * Temp2 - Temp3 * Temp3));
}
//----------------------------------------------------------------------------
double GetMRStar(FourVector P1, FourVector P2)
{
   double Temp1 = P1[0] + P2[0];
   double Temp2 = P1[3] + P2[3];
   double Temp3 = P1.GetPT() * P1.GetPT() - P2.GetPT() * P2.GetPT();
   double Temp4 = (P1 + P2).GetPT();
   return sqrt((Temp1 * Temp1) - (Temp2 * Temp2) - (Temp3 * Temp3) / (Temp4 * Temp4));
}
//----------------------------------------------------------------------------
double Get2011MR(FourVector P1, FourVector P2)
{
   return GetMRStar(P1, P2) * GetGammaRStar(P1, P2);
}
//----------------------------------------------------------------------------
double GetISRRemovedMR(FourVector P1, FourVector P2, FourVector POther, double ME3Assumption)
{
   FourVector ME = -(P1 + P2 + POther);
   ME[3] = ME3Assumption;
   ME[0] = ME.GetP();

   FourVector Total = P1 + P2 + ME;

   double Beta = Total.GetPT() / Total[0];
   FourVector Direction = -Total;

   FourVector NewP1 = P1.Boost(Direction, Beta);
   FourVector NewP2 = P2.Boost(Direction, Beta);

   return GetMR(NewP1, NewP2);
}
//----------------------------------------------------------------------------
double GetISRRemoved2011MR(FourVector P1, FourVector P2, FourVector POther, double ME3Assumption)
{
   FourVector ME = -(P1 + P2 + POther);
   ME[3] = ME3Assumption;
   ME[0] = ME.GetP();

   FourVector Total = P1 + P2 + ME;

   double Beta = Total.GetPT() / Total[0];
   FourVector Direction = -Total;

   FourVector NewP1 = P1.Boost(Direction, Beta);
   FourVector NewP2 = P2.Boost(Direction, Beta);

   return Get2011MR(NewP1, NewP2);
}
//----------------------------------------------------------------------------
double GetISR2011MR(FourVector P1, FourVector P2, FourVector ME, FourVector ISR, int Assumption)
{
   if(Assumption == 1)
   {
      FourVector ME1 = ME;
      ME1[3] = 0;
      ME1[0] = ME1.GetP();

      FourVector Total1 = P1 + P2 + ME1;
      double Beta1 = Total1.GetPT() / Total1[0];
      Total1[3] = 0;
      FourVector BP1 = P1.Boost(-Total1, Beta1);
      FourVector BP2 = P2.Boost(-Total1, Beta1);
      ME1 = ME1.Boost(-Total1, Beta1);

      return Get2011MR(BP1, BP2);
   }
   if(Assumption == 2)
   {
      FourVector ME2 = ME;
      ME2[3] = 0;
      double Bottom = P1.SpatialDot(P2) * P1.SpatialDot(P2) - P1.GetP2() * P2.GetP2();
      double Projection1Top = P2.SpatialDot(ME2) * P2.SpatialDot(P1) - P1.SpatialDot(ME2) * P2.GetP2();
      double Projection2Top = P1.SpatialDot(ME2) * P1.SpatialDot(P2) - P2.SpatialDot(ME2) * P1.GetP2();
      FourVector ME2P1rojection = P1 * Projection1Top / Bottom;
      FourVector ME2P2rojection = P2 * Projection2Top / Bottom;
      ME2[0] = ME2P1rojection.GetP() + ME2P2rojection.GetP();

      FourVector Total2 = P1 + P2 + ME2;
      double Beta2 = Total2.GetPT() / Total2[0];
      Total2[3] = 0;
      FourVector P12 = P1.Boost(-Total2, Beta2);
      FourVector P22 = P2.Boost(-Total2, Beta2);
      ME2 = ME2.Boost(-Total2, Beta2);

      return Get2011MR(P12, P22);
   }
   if(Assumption == 3)
   {
      FourVector ME3 = ME;
      ME3[3] = -(P1[3] + P2[3]);
      ME3[0] = ME3.GetP();

      FourVector Total3 = P1 + P2 + ME3;
      double Beta3 = Total3.GetPT() / Total3[0];
      Total3[3] = 0;
      FourVector P13 = P1.Boost(-Total3, Beta3);
      FourVector P23 = P2.Boost(-Total3, Beta3);
      ME3 = ME3.Boost(-Total3, Beta3);

      return Get2011MR(P13, P23);
   }
   if(Assumption == 4)
   {
      FourVector ME4 = ME;
      ME4[3] = -(P1[3] + P2[3]);
      double Bottom4 = P1.SpatialDot(P2) * P1.SpatialDot(P2) - P1.GetP2() * P2.GetP2();
      double Projection1Top4 = P2.SpatialDot(ME4) * P2.SpatialDot(P1) - P1.SpatialDot(ME4) * P2.GetP2();
      double Projection2Top4 = P1.SpatialDot(ME4) * P1.SpatialDot(P2) - P2.SpatialDot(ME4) * P1.GetP2();
      FourVector ME2P1rojection4 = P1 * Projection1Top4 / Bottom4;
      FourVector ME2P2rojection4 = P2 * Projection2Top4 / Bottom4;
      ME4[0] = ME2P1rojection4.GetP() + ME2P2rojection4.GetP();

      FourVector Total4 = P1 + P2 + ME4;
      double Beta4 = Total4.GetPT() / Total4[0];
      Total4[3] = 0;
      FourVector P14 = P1.Boost(-Total4, Beta4);
      FourVector P24 = P2.Boost(-Total4, Beta4);
      ME4 = ME4.Boost(-Total4, Beta4);

      return Get2011MR(P14, P24);
   }
   if(Assumption == 5)
   {
      FourVector P1Temp5 = P1;
      FourVector P2Temp5 = P2;
      FourVector METemp5 = ME;
      FourVector Total5 = P1 + P2 + ME;

      P1Temp5[3] = 0;   P1Temp5[0] = P1Temp5.GetPT();
      P2Temp5[3] = 0;   P2Temp5[0] = P2Temp5.GetPT();
      METemp5[3] = 0;
      Total5[3] = 0;

      P1Temp5 = P1Temp5.RotateZ(-Total5.GetPhi());
      P2Temp5 = P2Temp5.RotateZ(-Total5.GetPhi());
      METemp5 = METemp5.RotateZ(-Total5.GetPhi());

      // visible: particle 1 (B1) and 3 (B2), partners: particle 2 and 4
      // rotated so that boost is in the x direction
      // y direction is then trivial
      double P2XPrime5 = (P1Temp5[1] + P2Temp5[1] + METemp5[1]) / 2 - P1Temp5[1];
      double P2YPrime5 = -P1Temp5[2];
      double P4XPrime5 = (P1Temp5[1] + P2Temp5[1] + METemp5[1]) / 2 - P2Temp5[1];
      double P4YPrime5 = -P2Temp5[2];
      double E2 = sqrt(P2XPrime5 * P2XPrime5 + P2YPrime5 * P2YPrime5);
      double E4 = sqrt(P4XPrime5 * P4XPrime5 + P4YPrime5 * P4YPrime5);
      double Beta5 = (P1Temp5[1] + P2XPrime5 + P2Temp5[1] + P4XPrime5) / (P1Temp5[0] + E2 + P2Temp5[0] + E4);
      METemp5[0] = E2 + E4;

      FourVector ME5 = ME;
      ME5[3] = 0;
      ME5[0] = sqrt((E2 + E4) * (E2 + E4) + ME5[3] * ME5[3]);

      FourVector P15 = P1.Boost(-Total5, Beta5);
      FourVector P25 = P2.Boost(-Total5, Beta5);
      ME5 = ME5.Boost(-Total5, Beta5);

      return Get2011MR(P15, P25);
   }
   if(Assumption == 6)
   {
      FourVector P1Temp6 = P1;
      FourVector P2Temp6 = P2;
      FourVector METemp6 = ME;
      FourVector Total6 = P1 + P2 + ME;

      P1Temp6[3] = 0;   P1Temp6[0] = P1Temp6.GetPT();
      P2Temp6[3] = 0;   P2Temp6[0] = P2Temp6.GetPT();
      METemp6[3] = 0;
      Total6[3] = 0;

      P1Temp6 = P1Temp6.RotateZ(-Total6.GetPhi());
      P2Temp6 = P2Temp6.RotateZ(-Total6.GetPhi());
      METemp6 = METemp6.RotateZ(-Total6.GetPhi());

      double E13_6 = P1Temp6[0] + P2Temp6[0];
      double P13x_6 = P1Temp6[1] + P2Temp6[1];
      double P24x_6 = METemp6[1];
      double P1234x_6 = P13x_6 + P24x_6;
      double Beta6 = (-E13_6 + sqrt(E13_6 * E13_6 + P24x_6 * P24x_6 - P13x_6 * P13x_6)) / P1234x_6;

      double E24_6 = (-P1234x_6 - Beta6 * E13_6) / Beta6;

      FourVector ME6 = ME;
      ME6[3] = 0;
      ME6[0] = sqrt(E24_6 * E24_6 + ME6[3] * ME6[3]);

      Total6 = P1Temp6 + P2Temp6 + ME6;
      FourVector P16 = P1.Boost(-Total6, fabs(Beta6));
      FourVector P26 = P2.Boost(-Total6, fabs(Beta6));
      ME6 = ME6.Boost(-Total6, fabs(Beta6));

      return Get2011MR(P16, P26);
   }
   if(Assumption == 7)
   {
      FourVector P1Temp7 = P1;
      FourVector P2Temp7 = P2;
      FourVector METemp7 = ME;
      FourVector Total7 = P1 + P2 + ME;

      P1Temp7[3] = 0;   P1Temp7[0] = P1Temp7.GetPT();
      P2Temp7[3] = 0;   P2Temp7[0] = P2Temp7.GetPT();
      METemp7[3] = 0;
      Total7[3] = 0;

      P1Temp7 = P1Temp7.RotateZ(-Total7.GetPhi());
      P2Temp7 = P2Temp7.RotateZ(-Total7.GetPhi());
      METemp7 = METemp7.RotateZ(-Total7.GetPhi());

      double PxJet7 = P1Temp7[1] + P2Temp7[1];
      double EJet7 = P1Temp7[0] + P2Temp7[0];

      double Beta7 = (EJet7 - sqrt(EJet7 * EJet7 - PxJet7 * PxJet7 + METemp7[1] * METemp7[1]))
         / (PxJet7 - METemp7[1]);

      METemp7[3] = 0;
      METemp7[0] = EJet7 + Beta7 * (METemp7[1] - PxJet7);

      Total7 = P1Temp7 + P2Temp7 + METemp7;
      FourVector P17 = P1Temp7.Boost(-Total7, -Beta7);
      FourVector P27 = P2Temp7.Boost(-Total7, -Beta7);
      FourVector ME7 = METemp7.Boost(-Total7, -Beta7);

      return Get2011MR(P17, P27);
   }
   if(Assumption == 8)
   {
      FourVector B1PTemp8 = P1;
      FourVector B2PTemp8 = P2;
      FourVector METemp8 = ME;
      FourVector Total8 = P1 + P2 + ME;

      B1PTemp8 = B1PTemp8.RotateZ(-Total8.GetPhi());
      B2PTemp8 = B2PTemp8.RotateZ(-Total8.GetPhi());
      METemp8 = METemp8.RotateZ(-Total8.GetPhi());

      double MinimumBetaX = 1000;
      double MinimumDifference = 999999;
      double SearchCenter = 0;
      double SearchStep = 0.1;
      for(int i = 0; i < 5; i++)
      {
         for(double BetaX = SearchCenter - SearchStep * 10; BetaX < SearchCenter + SearchStep * 10;
            BetaX = BetaX + SearchStep)
         {
            double Difference = GetDifference8(B1PTemp8, B2PTemp8, METemp8, BetaX);

            if(fabs(Difference) < MinimumDifference)
            {
               MinimumBetaX = BetaX;
               MinimumDifference = fabs(Difference);
            }
         }

         SearchCenter = MinimumBetaX;
         SearchStep = SearchStep / 10;
      }
         
      double Beta8 = MinimumBetaX;

      double GammaX8 = BetaToGamma(Beta8);
      double BetaZ8 = GammaX8 * ((B1PTemp8[0] - B2PTemp8[0]) - Beta8 * (B1PTemp8[1] - B2PTemp8[1]))
         / (P1[3] - P2[3]);
      double GammaZ8 = BetaToGamma(BetaZ8);
      // double Beta8E8 = METemp8[1] + B1PTemp8[1] + B2PTemp8[1] - Beta8 * (B1PTemp8[0] + B2PTemp8[0]);

      // double FinalMEx = GammaX8 * METemp8[1] - GammaX8 * Beta8E8;
      // double FinalMEy = METemp8[2];
      // double FinalMET = sqrt(FinalMEx * FinalMEx + FinalMEy * FinalMEy);

      // double FinalJx = GammaX8 * B1PTemp8[1] - GammaX8 * Beta8 * B1PTemp8[0]
      //    + GammaX8 * B2PTemp8[1] - GammaX8 * Beta8 * B2PTemp8[0];
      // double FinalJy = B1PTemp8[2] + B2PTemp8[2];
      // double FinalJT = sqrt(FinalJx * FinalJx + FinalJy * FinalJy);

      double MR8 = GammaZ8 * ((GammaX8 * (B1PTemp8[0] + B2PTemp8[0])
         - GammaX8 * Beta8 * (B1PTemp8[1] + B2PTemp8[1]))
         - BetaZ8 * (P1[3] + P2[3]));
      // double MT8 = sqrt(2 * (FinalMET * FinalJT - FinalMEx * FinalJx - FinalMEy * FinalJy));

      return MR8;
   }
   if(Assumption == 9)
   {
      FourVector B1PTemp9 = P1;
      FourVector B2PTemp9 = P2;
      FourVector METemp9 = ME;
      FourVector Total9 = P1 + P2 + ME;

      B1PTemp9 = B1PTemp9.RotateZ(-Total9.GetPhi());
      B2PTemp9 = B2PTemp9.RotateZ(-Total9.GetPhi());
      METemp9 = METemp9.RotateZ(-Total9.GetPhi());

      double MinimumBetaZ = 1000;
      double MinimumDifference9 = 99999999;
      double SearchCenter9 = 0;
      double SearchStep9 = 0.05;
      for(int i = 0; i < 5; i++)
      {
         for(double BetaZ = SearchCenter9 - SearchStep9 * 20; BetaZ < SearchCenter9 + SearchStep9 * 20;
            BetaZ = BetaZ + SearchStep9)
         {
            double Difference = GetDifference9(B1PTemp9, B2PTemp9, METemp9, BetaZ);

            if(fabs(Difference) < MinimumDifference9)
            {
               MinimumBetaZ = BetaZ;
               MinimumDifference9 = fabs(Difference);
            }
         }

         SearchCenter9 = MinimumBetaZ;
         SearchStep9 = SearchStep9 / 10;
      }

      double GammaZ9 = BetaToGamma(MinimumBetaZ);
      double BetaX9 = (GammaZ9 * (B1PTemp9[0] - B2PTemp9[0])
         - GammaZ9 * MinimumBetaZ * (B1PTemp9[3] - B2PTemp9[3]))
         / (B1PTemp9[1] - B2PTemp9[1]);
      double GammaX9 = BetaToGamma(BetaX9);

      double FinalJE = GammaX9 * (GammaZ9 * (B1PTemp9[0] + B2PTemp9[0])
         - GammaZ9 * MinimumBetaZ * (B1PTemp9[3] - B2PTemp9[3]))
         - GammaX9 * BetaX9 * (B1PTemp9[1] + B2PTemp9[0]);
      double FinalJx9 = GammaX9 * (B1PTemp9[1] + B2PTemp9[1]) - GammaX9 * BetaX9
         * (GammaZ9 * (B1PTemp9[0] + B2PTemp9[0]) - GammaZ9 * MinimumBetaZ * (B1PTemp9[3] - B2PTemp9[3]));
      double FinalJy9 = B1PTemp9[2] + B2PTemp9[2];
      // double FinalJT9 = sqrt(FinalJx9 * FinalJx9 + FinalJy9 * FinalJy9);

      double MR9 = FinalJE;
      // double MT9 = 2 * FinalJT9;
      // double R9 = MT9 / MR9 / 2;

      return MR9;
   }
   if(Assumption == 11)
   {
      double JJMass2 = (P1 + P2).GetMass2();
      FourVector METemp11 = ME;

      METemp11[3] = FindMR11MinimumPz(P1, P2, METemp11, ISR);
      METemp11[0] = sqrt(JJMass2 + METemp11.GetP2());

      return EstimateMass11(P1, P2, METemp11, ISR);
   }
   if(Assumption == -11)
   {
      double JJMass2 = (P1 + P2).GetMass2();
      FourVector METemp11 = ME;

      METemp11[3] = FindMR11MinimumPz(P1, P2, METemp11, ISR);
      METemp11[0] = sqrt(JJMass2 + METemp11.GetP2());

      return EstimateMass11(P1, P2, METemp11, ISR);
   }

   return 0;
}
//----------------------------------------------------------------------------
double GetMRT(FourVector P1, FourVector P2, FourVector ME)
{
   double Temp1 = ME.GetPT() * (P1.GetPT() + P2.GetPT());
   double Temp2 = ME[1] * (P1[1] + P2[1]) + ME[2] * (P1[2] + P2[2]);
   return sqrt((Temp1 - Temp2) / 2);
}
//----------------------------------------------------------------------------
double GetR(FourVector P1, FourVector P2, FourVector ME)
{
   return GetMRT(P1, P2, ME) / GetMR(P1, P2);
}
//----------------------------------------------------------------------------
double GetRStar(FourVector P1, FourVector P2, FourVector ME)
{
   return GetMRT(P1, P2, ME) / GetMRStar(P1, P2) / GetGammaRStar(P1, P2);
}
//----------------------------------------------------------------------------
double Get2011R(FourVector P1, FourVector P2, FourVector ME)
{
   return GetMRT(P1, P2, ME) / GetMRStar(P1, P2) / GetGammaRStar(P1, P2);
}
//----------------------------------------------------------------------------
double GetISRRemovedR(FourVector P1, FourVector P2, FourVector POther, double ME3Assumption)
{
   FourVector ME = -(P1 + P2 + POther);
   ME[3] = ME3Assumption;
   ME[0] = ME.GetP();

   FourVector Total = P1 + P2 + ME;

   double Beta = Total.GetPT() / Total[0];
   FourVector Direction = -Total;

   FourVector NewP1 = P1.Boost(Direction, Beta);
   FourVector NewP2 = P2.Boost(Direction, Beta);
   FourVector NewME = ME.Boost(Direction, Beta);

   return GetR(NewP1, NewP2, NewME);
}
//----------------------------------------------------------------------------
double GetISRRemoved2011R(FourVector P1, FourVector P2, FourVector POther, double ME3Assumption)
{
   FourVector ME = -(P1 + P2 + POther);
   ME[3] = ME3Assumption;
   ME[0] = ME.GetP();

   FourVector Total = P1 + P2 + ME;

   double Beta = Total.GetPT() / Total[0];
   FourVector Direction = -Total;

   FourVector NewP1 = P1.Boost(Direction, Beta);
   FourVector NewP2 = P2.Boost(Direction, Beta);
   FourVector NewME = ME.Boost(Direction, Beta);

   return Get2011R(NewP1, NewP2, NewME);
}
//----------------------------------------------------------------------------
double GetISR2011R(FourVector P1, FourVector P2, FourVector ME, FourVector ISR, int Assumption, char AdditionalVariant)
{
   if(Assumption == 1)
   {
      FourVector ME1 = ME;
      ME1[3] = 0;
      ME1[0] = ME1.GetP();

      FourVector Total1 = P1 + P2 + ME1;
      double Beta1 = Total1.GetPT() / Total1[0];
      Total1[3] = 0;
      FourVector BP1 = P1.Boost(-Total1, Beta1);
      FourVector BP2 = P2.Boost(-Total1, Beta1);
      ME1 = ME1.Boost(-Total1, Beta1);

      return Get2011R(BP1, BP2, ME1);
   }
   if(Assumption == 2)
   {
      FourVector ME2 = ME;
      ME2[3] = 0;
      double Bottom = P1.SpatialDot(P2) * P1.SpatialDot(P2) - P1.GetP2() * P2.GetP2();
      double Projection1Top = P2.SpatialDot(ME2) * P2.SpatialDot(P1) - P1.SpatialDot(ME2) * P2.GetP2();
      double Projection2Top = P1.SpatialDot(ME2) * P1.SpatialDot(P2) - P2.SpatialDot(ME2) * P1.GetP2();
      FourVector ME2P1rojection = P1 * Projection1Top / Bottom;
      FourVector ME2P2rojection = P2 * Projection2Top / Bottom;
      ME2[0] = ME2P1rojection.GetP() + ME2P2rojection.GetP();

      FourVector Total2 = P1 + P2 + ME2;
      double Beta2 = Total2.GetPT() / Total2[0];
      Total2[3] = 0;
      FourVector P12 = P1.Boost(-Total2, Beta2);
      FourVector P22 = P2.Boost(-Total2, Beta2);
      ME2 = ME2.Boost(-Total2, Beta2);

      return Get2011R(P12, P22, ME2);
   }
   if(Assumption == 3)
   {
      FourVector ME3 = ME;
      ME3[3] = -(P1[3] + P2[3]);
      ME3[0] = ME3.GetP();

      FourVector Total3 = P1 + P2 + ME3;
      double Beta3 = Total3.GetPT() / Total3[0];
      Total3[3] = 0;
      FourVector P13 = P1.Boost(-Total3, Beta3);
      FourVector P23 = P2.Boost(-Total3, Beta3);
      ME3 = ME3.Boost(-Total3, Beta3);

      return Get2011R(P13, P23, ME3);
   }
   if(Assumption == 4)
   {
      FourVector ME4 = ME;
      ME4[3] = -(P1[3] + P2[3]);
      double Bottom4 = P1.SpatialDot(P2) * P1.SpatialDot(P2) - P1.GetP2() * P2.GetP2();
      double Projection1Top4 = P2.SpatialDot(ME4) * P2.SpatialDot(P1) - P1.SpatialDot(ME4) * P2.GetP2();
      double Projection2Top4 = P1.SpatialDot(ME4) * P1.SpatialDot(P2) - P2.SpatialDot(ME4) * P1.GetP2();
      FourVector ME2P1rojection4 = P1 * Projection1Top4 / Bottom4;
      FourVector ME2P2rojection4 = P2 * Projection2Top4 / Bottom4;
      ME4[0] = ME2P1rojection4.GetP() + ME2P2rojection4.GetP();

      FourVector Total4 = P1 + P2 + ME4;
      double Beta4 = Total4.GetPT() / Total4[0];
      Total4[3] = 0;
      FourVector P14 = P1.Boost(-Total4, Beta4);
      FourVector P24 = P2.Boost(-Total4, Beta4);
      ME4 = ME4.Boost(-Total4, Beta4);

      return Get2011R(P14, P24, ME4);
   }
   if(Assumption == 5)
   {
      FourVector P1Temp5 = P1;
      FourVector P2Temp5 = P2;
      FourVector METemp5 = ME;
      FourVector Total5 = P1 + P2 + ME;

      P1Temp5[3] = 0;   P1Temp5[0] = P1Temp5.GetPT();
      P2Temp5[3] = 0;   P2Temp5[0] = P2Temp5.GetPT();
      METemp5[3] = 0;
      Total5[3] = 0;

      P1Temp5 = P1Temp5.RotateZ(-Total5.GetPhi());
      P2Temp5 = P2Temp5.RotateZ(-Total5.GetPhi());
      METemp5 = METemp5.RotateZ(-Total5.GetPhi());

      // visible: particle 1 (B1) and 3 (B2), partners: particle 2 and 4
      // rotated so that boost is in the x direction
      // y direction is then trivial
      double P2XPrime5 = (P1Temp5[1] + P2Temp5[1] + METemp5[1]) / 2 - P1Temp5[1];
      double P2YPrime5 = -P1Temp5[2];
      double P4XPrime5 = (P1Temp5[1] + P2Temp5[1] + METemp5[1]) / 2 - P2Temp5[1];
      double P4YPrime5 = -P2Temp5[2];
      double E2 = sqrt(P2XPrime5 * P2XPrime5 + P2YPrime5 * P2YPrime5);
      double E4 = sqrt(P4XPrime5 * P4XPrime5 + P4YPrime5 * P4YPrime5);
      double Beta5 = (P1Temp5[1] + P2XPrime5 + P2Temp5[1] + P4XPrime5) / (P1Temp5[0] + E2 + P2Temp5[0] + E4);
      METemp5[0] = E2 + E4;

      FourVector ME5 = ME;
      ME5[3] = 0;
      ME5[0] = sqrt((E2 + E4) * (E2 + E4) + ME5[3] * ME5[3]);

      FourVector P15 = P1.Boost(-Total5, Beta5);
      FourVector P25 = P2.Boost(-Total5, Beta5);
      ME5 = ME5.Boost(-Total5, Beta5);

      return Get2011R(P15, P25, ME5);
   }
   if(Assumption == 6)
   {
      FourVector P1Temp6 = P1;
      FourVector P2Temp6 = P2;
      FourVector METemp6 = ME;
      FourVector Total6 = P1 + P2 + ME;

      P1Temp6[3] = 0;   P1Temp6[0] = P1Temp6.GetPT();
      P2Temp6[3] = 0;   P2Temp6[0] = P2Temp6.GetPT();
      METemp6[3] = 0;
      Total6[3] = 0;

      P1Temp6 = P1Temp6.RotateZ(-Total6.GetPhi());
      P2Temp6 = P2Temp6.RotateZ(-Total6.GetPhi());
      METemp6 = METemp6.RotateZ(-Total6.GetPhi());

      double E13_6 = P1Temp6[0] + P2Temp6[0];
      double P13x_6 = P1Temp6[1] + P2Temp6[1];
      double P24x_6 = METemp6[1];
      double P1234x_6 = P13x_6 + P24x_6;
      double Beta6 = (-E13_6 + sqrt(E13_6 * E13_6 + P24x_6 * P24x_6 - P13x_6 * P13x_6)) / P1234x_6;

      double E24_6 = (-P1234x_6 - Beta6 * E13_6) / Beta6;

      FourVector ME6 = ME;
      ME6[3] = 0;
      ME6[0] = sqrt(E24_6 * E24_6 + ME6[3] * ME6[3]);

      Total6 = P1Temp6 + P2Temp6 + ME6;
      FourVector P16 = P1.Boost(-Total6, fabs(Beta6));
      FourVector P26 = P2.Boost(-Total6, fabs(Beta6));
      ME6 = ME6.Boost(-Total6, fabs(Beta6));

      return Get2011R(P16, P26, ME6);
   }
   if(Assumption == 7)
   {
      FourVector P1Temp7 = P1;
      FourVector P2Temp7 = P2;
      FourVector METemp7 = ME;
      FourVector Total7 = P1 + P2 + ME;

      P1Temp7[3] = 0;   P1Temp7[0] = P1Temp7.GetPT();
      P2Temp7[3] = 0;   P2Temp7[0] = P2Temp7.GetPT();
      METemp7[3] = 0;
      Total7[3] = 0;

      P1Temp7 = P1Temp7.RotateZ(-Total7.GetPhi());
      P2Temp7 = P2Temp7.RotateZ(-Total7.GetPhi());
      METemp7 = METemp7.RotateZ(-Total7.GetPhi());

      double PxJet7 = P1Temp7[1] + P2Temp7[1];
      double EJet7 = P1Temp7[0] + P2Temp7[0];

      double Beta7 = (EJet7 - sqrt(EJet7 * EJet7 - PxJet7 * PxJet7 + METemp7[1] * METemp7[1]))
         / (PxJet7 - METemp7[1]);

      METemp7[3] = 0;
      METemp7[0] = EJet7 + Beta7 * (METemp7[1] - PxJet7);

      Total7 = P1Temp7 + P2Temp7 + METemp7;
      FourVector P17 = P1Temp7.Boost(-Total7, -Beta7);
      FourVector P27 = P2Temp7.Boost(-Total7, -Beta7);
      FourVector ME7 = METemp7.Boost(-Total7, -Beta7);

      return Get2011R(P17, P27, ME7);
   }
   if(Assumption == 8)
   {
      FourVector B1PTemp8 = P1;
      FourVector B2PTemp8 = P2;
      FourVector METemp8 = ME;
      FourVector Total8 = P1 + P2 + ME;

      B1PTemp8 = B1PTemp8.RotateZ(-Total8.GetPhi());
      B2PTemp8 = B2PTemp8.RotateZ(-Total8.GetPhi());
      METemp8 = METemp8.RotateZ(-Total8.GetPhi());

      double MinimumBetaX = 1000;
      double MinimumDifference = 999999;
      double SearchCenter = 0;
      double SearchStep = 0.1;

      for(int i = 0; i < 5; i++)
      {
         for(double BetaX = SearchCenter - SearchStep * 10; BetaX < SearchCenter + SearchStep * 10;
            BetaX = BetaX + SearchStep)
         {
            double Difference = GetDifference8(B1PTemp8, B2PTemp8, METemp8, BetaX);

            if(fabs(Difference) < MinimumDifference)
            {
               MinimumBetaX = BetaX;
               MinimumDifference = fabs(Difference);
            }
         }

         SearchCenter = MinimumBetaX;
         SearchStep = SearchStep / 10;
      }
         
      double Beta8 = MinimumBetaX;

      double GammaX8 = BetaToGamma(Beta8);
      double BetaZ8 = GammaX8 * ((B1PTemp8[0] - B2PTemp8[0]) - Beta8 * (B1PTemp8[1] - B2PTemp8[1]))
         / (P1[3] - P2[3]);
      double GammaZ8 = BetaToGamma(BetaZ8);
      double Beta8E8 = METemp8[1] + B1PTemp8[1] + B2PTemp8[1] - Beta8 * (B1PTemp8[0] + B2PTemp8[0]);

      double FinalMEx = GammaX8 * METemp8[1] - GammaX8 * Beta8E8;
      double FinalMEy = METemp8[2];
      double FinalMET = sqrt(FinalMEx * FinalMEx + FinalMEy * FinalMEy);
      double FinalJx = GammaX8 * B1PTemp8[1] - GammaX8 * Beta8 * B1PTemp8[0]
         + GammaX8 * B2PTemp8[1] - GammaX8 * Beta8 * B2PTemp8[0];
      double FinalJy = B1PTemp8[2] + B2PTemp8[2];
      double FinalJT = sqrt(FinalJx * FinalJx + FinalJy * FinalJy);

      double MR8 = GammaZ8 * ((GammaX8 * (B1PTemp8[0] + B2PTemp8[0])
         - GammaX8 * Beta8 * (B1PTemp8[1] + B2PTemp8[1]))
         - BetaZ8 * (P1[3] + P2[3]));
      double MT8 = sqrt(2 * (FinalMET * FinalJT - FinalMEx * FinalJx - FinalMEy * FinalJy));

      return MT8 / MR8 / 2;
   }
   if(Assumption == 9)
   {
      FourVector B1PTemp9 = P1;
      FourVector B2PTemp9 = P2;
      FourVector METemp9 = ME;
      FourVector Total9 = P1 + P2 + ME;

      B1PTemp9 = B1PTemp9.RotateZ(-Total9.GetPhi());
      B2PTemp9 = B2PTemp9.RotateZ(-Total9.GetPhi());
      METemp9 = METemp9.RotateZ(-Total9.GetPhi());

      double MinimumBetaZ = 1000;
      double MinimumDifference9 = 99999999;
      double SearchCenter9 = 0;
      double SearchStep9 = 0.05;
      for(int i = 0; i < 5; i++)
      {
         for(double BetaZ = SearchCenter9 - SearchStep9 * 20; BetaZ < SearchCenter9 + SearchStep9 * 20;
            BetaZ = BetaZ + SearchStep9)
         {
            double Difference = GetDifference9(B1PTemp9, B2PTemp9, METemp9, BetaZ);

            if(fabs(Difference) < MinimumDifference9)
            {
               MinimumBetaZ = BetaZ;
               MinimumDifference9 = fabs(Difference);
            }         }

         SearchCenter9 = MinimumBetaZ;
         SearchStep9 = SearchStep9 / 10;
      }

      double GammaZ9 = BetaToGamma(MinimumBetaZ);
      double BetaX9 = (GammaZ9 * (B1PTemp9[0] - B2PTemp9[0])
         - GammaZ9 * MinimumBetaZ * (B1PTemp9[3] - B2PTemp9[3]))
         / (B1PTemp9[1] - B2PTemp9[1]);
      double GammaX9 = BetaToGamma(BetaX9);

      double FinalJE = GammaX9 * (GammaZ9 * (B1PTemp9[0] + B2PTemp9[0])
         - GammaZ9 * MinimumBetaZ * (B1PTemp9[3] - B2PTemp9[3]))
         - GammaX9 * BetaX9 * (B1PTemp9[1] + B2PTemp9[0]);
      double FinalJx9 = GammaX9 * (B1PTemp9[1] + B2PTemp9[1]) - GammaX9 * BetaX9
         * (GammaZ9 * (B1PTemp9[0] + B2PTemp9[0]) - GammaZ9 * MinimumBetaZ * (B1PTemp9[3] - B2PTemp9[3]));
      double FinalJy9 = B1PTemp9[2] + B2PTemp9[2];
      double FinalJT9 = sqrt(FinalJx9 * FinalJx9 + FinalJy9 * FinalJy9);

      double MR9 = FinalJE;
      double MT9 = 2 * FinalJT9;
      double R9 = MT9 / MR9 / 2;

      return R9;
   }
   if(Assumption == 11)
   {
      double JJMass2 = (P1 + P2).GetMass2();
      FourVector METemp11 = ME;

      METemp11[3] = FindMR11MinimumPz(P1, P2, METemp11, ISR);
      METemp11[0] = sqrt(JJMass2 + METemp11.GetP2());

      double MR11 = EstimateMass11(P1, P2, METemp11, ISR);
      double MRT11 = EstimateTransverseMass11(P1, P2, METemp11, ISR, AdditionalVariant);

      return MRT11 / MR11;
   }

   return 0;
}
//----------------------------------------------------------------------------
double GetGammaRStar(FourVector P1, FourVector P2)
{
   double Temp1 = P1[0] + P2[0];
   double Temp2 = P1[3] + P2[3];
   double Temp3 = P1.GetPT() * P1.GetPT() - P2.GetPT() * P2.GetPT();
   double Temp4 = (P1 + P2).GetPT();

   double Upper = Temp1 * Temp1 - Temp2 * Temp2;
   double Lower = Temp1 * Temp1 - Temp2 * Temp2 - Temp3 * Temp3 / Temp4 / Temp4;
   return sqrt(Upper / Lower);

   /*
   double Beta2 = (Temp3 * Temp3) / (Temp4 * Temp4 * (Temp1 * Temp1 - Temp2 * Temp2));
   double Gamma = 1.0 / sqrt(1.0 - Beta2);
   return Gamma;
   */
}
//----------------------------------------------------------------------------
double BetaToGamma(double Beta)
{
   return 1 / sqrt(1 - Beta * Beta);
}
//----------------------------------------------------------------------------
double GammaToBeta(double Gamma)
{
   return sqrt(1 - 1 / (Gamma * Gamma));
}
//----------------------------------------------------------------------------
std::vector<FourVector> SplitIntoGroups(std::vector<FourVector> &Input, bool ZeroMass)
{
   std::vector<FourVector> Result;
   
   if(Input.size() == 0)
   {
      Result.push_back(FourVector());
      Result.push_back(FourVector());
      return Result;
   }
   if(Input.size() == 1)
   {
      Result.push_back(Input[0]);
      Result.push_back(FourVector());
      return Result;
   }
   if(Input.size() == 2)
      return Input;

   // let's start with easy (potentially slow) way: try out all possibilities
   // if speed becomes a problem then we'll come up with something else

   int InputSize = Input.size();

   std::vector<int> Groups(InputSize);
   for(int i = 0; i < InputSize; i++)
      Groups[i] = 0;
   Groups[0] = 1;

   FourVector Group1;
   FourVector Group2;
   double MinMass2 = -1;

   while(Groups[InputSize-1] == 0)   // last one is always in group "0"
   {
      FourVector Vector1Temp;
      FourVector Vector2Temp;

      for(int i = 0; i < InputSize; i++)
      {
         if(Groups[i] == 0)
            Vector1Temp = Vector1Temp + Input[i];
         else
            Vector2Temp = Vector2Temp + Input[i];
      }

      double MinMass2Temp = Vector1Temp.GetMass2() + Vector2Temp.GetMass2();
      if(MinMass2 < 0 || MinMass2Temp < MinMass2)
      {
         MinMass2 = MinMass2Temp;
         Group1 = Vector1Temp;
         Group2 = Vector2Temp;
      }

      Groups[0] = Groups[0] + 1;
      for(int i = 0; i < InputSize - 1; i++)
      {
         while(Groups[i] >= 2)   // just in case something went wrong....an if statement should be enough
         {
            Groups[i] = Groups[i] - 2;
            Groups[i+1] = Groups[i+1] + 1;
         }
      }
   }

   if(ZeroMass == true)
   {
      Group1[0] = Group1.GetP();
      Group2[0] = Group2.GetP();
   }

   Result.push_back(Group1);
   Result.push_back(Group2);

   return Result;
}
//----------------------------------------------------------------------------
double GetDifference8(FourVector &P1, FourVector &P2, FourVector &ME, double BetaX)
{
   double DeltaPx = P1[1] - P2[1];
   double Pxj = P1[1] + P2[1];
   double Px = ME[1];
   double DeltaPz = P1[3] - P2[3];
   double Pzj = P1[3] + P2[3];
   double DeltaE = P1[0] - P2[0];
   double Ej = P1[0] + P2[0];

   double GammaX = BetaToGamma(BetaX);
   double DeltaPzBetaZ = GammaX * DeltaE - GammaX * BetaX * DeltaPx;
   double BetaXE = Px + Pxj - BetaX * Ej;
   double BetaXDeltaPzPz = -BetaX * DeltaPz * Pzj
      + DeltaPzBetaZ * GammaX * (BetaXE + BetaX * Ej - BetaX * BetaX * (Px + Pxj));
   double Left = DeltaPz * DeltaPz * BetaX * GammaX * Ej
      - GammaX * BetaX * BetaX * DeltaPz * Pxj * DeltaPz
      - Pzj * BetaX * DeltaPzBetaZ * DeltaPz;
   double Right = GammaX * BetaXE * DeltaPz * DeltaPz
      - GammaX * BetaX * BetaX * Px * DeltaPz * DeltaPz
      - DeltaPzBetaZ * BetaXDeltaPzPz;

   return Left - Right;
}
//----------------------------------------------------------------------------
double GetDifference9(FourVector &P1, FourVector &P2, FourVector &ME, double BetaZ)
{
   double DeltaPx = P1[1] - P2[1];
   double Pxj = P1[1] + P2[1];
   double Px = ME[1];
   double DeltaPz = P1[3] - P2[3];
   double Pzj = P1[3] + P2[3];
   double DeltaE = P1[0] - P2[0];
   double Ej = P1[0] + P2[0];

   double GammaZ = BetaToGamma(BetaZ);
   double InvGammaZ = sqrt(1 - BetaZ * BetaZ);
   double DeltaPxBetaX = GammaZ * DeltaE - GammaZ * BetaZ * DeltaPz;
   double DeltaPxInvGammaE = GammaZ * Ej * DeltaPx * (1 + BetaZ * BetaZ)
      - 2 * GammaZ * BetaZ * Pzj * DeltaPx
      + DeltaPxBetaX * (Px - Pxj);
   double DeltaPxInvGammaPz = DeltaPx * InvGammaZ * Ej * BetaZ
      - DeltaPx * InvGammaZ * Pzj + BetaZ * DeltaPxInvGammaE;

   double Left = DeltaPx * DeltaPx * InvGammaZ * (Pxj + Px);
   double Right = DeltaPxBetaX * (GammaZ * (DeltaPxInvGammaE + DeltaPx * InvGammaZ * Ej)
         - GammaZ * BetaZ * (DeltaPxInvGammaPz + DeltaPx * InvGammaZ * Pzj));

   return fabs(Left - Right);
}
//----------------------------------------------------------------------------
double FindMR11MinimumPz(FourVector J1, FourVector J2, FourVector ME, FourVector ISR)
{
   // do some basic caching to save time repeating search for MR and R
   static FourVector PreviousJ1 = FourVector(0, 0, 0, 0);
   static FourVector PreviousJ2 = FourVector(0, 0, 0, 0);
   static FourVector PreviousME = FourVector(0, 0, 0, 0);
   static FourVector PreviousISR = FourVector(0, 0, 0, 0);
   static double PreviousPz = 0;

   if((J1 - PreviousJ1).GetP() < 0.1
      && (J2 - PreviousJ2).GetP() < 0.1
      && (ME - PreviousME).GetP() < 0.1
      && (ISR - PreviousISR).GetP() < 0.1)
      return PreviousPz;

   // start calculation
   double JJMass2 = (J1 + J2).GetMass2();

   int InitialStep = 400;
   double InitialStepSize = 5;
   double InitialCenter = 0;
   int SearchStep = 10;
   double SearchStepSize = 1;

   std::vector<double> Masses;
   for(int i = 0; i <= InitialStep; i++)
   {
      ME[3] = InitialCenter - InitialStep / 2 * InitialStepSize + i * InitialStepSize;
      ME[0] = sqrt(JJMass2 + ME.GetP2());

      double Mass = EstimateMass11(J1, J2, ME, ISR);
      Masses.push_back(Mass);
   }

   std::vector<double> LocalMinima;
   for(int i = 1; i < InitialStep; i++)
      if(Masses[i] <= Masses[i-1] && Masses[i] <= Masses[i+1])
         LocalMinima.push_back(InitialCenter - InitialStep / 2 * InitialStepSize + i * InitialStepSize);
   if(Masses[0] <= Masses[1])
      LocalMinima.push_back(InitialCenter - InitialStep / 2 * InitialStepSize);
   if(Masses[InitialStep] <= Masses[InitialStep-1])
      LocalMinima.push_back(InitialCenter + InitialStep / 2 * InitialStepSize);

   for(int i = 0; i <= 5; i++)
   {
      std::vector<double> NewMinima;
      for(int j = 0; j < (int)LocalMinima.size(); j++)
      {
         double SearchCenter = LocalMinima[j];

         Masses.clear();
         for(int k = 0; k <= SearchStep; k++)
         {
            ME[3] = SearchCenter - SearchStep / 2 * SearchStepSize + k * SearchStepSize;
            ME[0] = sqrt(JJMass2 + ME.GetP2());
            
            double Mass = EstimateMass11(J1, J2, ME, ISR);
            Masses.push_back(Mass);
         }

         for(int k = 1; k < SearchStep; k++)
            if(Masses[k] <= Masses[k-1] && Masses[k] <= Masses[k+1])
               NewMinima.push_back(SearchCenter - SearchStep / 2 * SearchStepSize + k * SearchStepSize);
         if(Masses[0] <= Masses[1])
            NewMinima.push_back(SearchCenter - SearchStep / 2 * SearchStepSize);
         if(Masses[InitialStep] <= Masses[InitialStep-1])
            NewMinima.push_back(SearchCenter + SearchStep / 2 * SearchStepSize);
      }

      SearchStepSize = SearchStepSize / 5;
      LocalMinima = NewMinima;
   }

   double BestPz = -1;
   double BestMass = -1;
   for(int i = 0; i < (int)LocalMinima.size(); i++)
   {
      ME[3] = LocalMinima[i];
      ME[0] = sqrt(JJMass2 + ME.GetP2());

      double Mass = EstimateMass11(J1, J2, ME, ISR);

      if(Mass < BestMass || BestMass < 0)
      {
         BestPz = LocalMinima[i];
         BestMass = Mass;
      }
   }

   PreviousJ1 = J1;
   PreviousJ2 = J2;
   PreviousME = ME;
   PreviousISR = ISR;
   PreviousPz = BestPz;

   return BestPz;
}
//----------------------------------------------------------------------------
double EstimateMass11(FourVector J1, FourVector J2, FourVector ME, FourVector ISR, bool Reversal)
{
   Reversal = false;

   FourVector TempTotal = ME + J1 + J2 + ISR;
   double TempBetaZ = TempTotal[3] / TempTotal[0];

   FourVector TempJ1 = J1.Boost(FourVector(1, 0, 0, 1), TempBetaZ);
   FourVector TempJ2 = J2.Boost(FourVector(1, 0, 0, 1), TempBetaZ);
   FourVector TempME = ME.Boost(FourVector(1, 0, 0, 1), TempBetaZ);
   FourVector TempISR = ISR.Boost(FourVector(1, 0, 0, 1), TempBetaZ);

   TempTotal = TempJ1 + TempJ2 + TempME;
   double TempBeta = TempTotal.GetP() / TempTotal[0];

   TempJ1 = TempJ1.Boost(TempTotal, TempBeta);
   TempJ2 = TempJ2.Boost(TempTotal, TempBeta);
   TempME = TempME.Boost(TempTotal, TempBeta);

   double EMET = TempME[0];
   double SumE = TempJ1[0] + TempJ2[0];
   double DeltaE = TempJ1[0] - TempJ2[0];
   double ES = (TempJ1 + TempJ2).GetP();
   double ED = (TempJ1 - TempJ2).GetP();
   double A = SumE * DeltaE / ES;
   double B = sqrt(ED * ED - A * A);

   double m0 = (SumE - EMET) / (2 * DeltaE) - SumE * DeltaE / ES / ES;
   double m1 = DeltaE * (SumE + EMET) / 2;

   double EQA = m0 * m0 + B * B / ES / ES;
   double EQB = 2 * m0 * m1 - B * B;
   double EQC = m1 * m1;

   double X2Max = (-EQB + sqrt(EQB * EQB - 4 * EQA * EQC)) / (2 * EQA);
   double X2Min = (-EQB - sqrt(EQB * EQB - 4 * EQA * EQC)) / (2 * EQA);

   if(X2Min > ES * ES)
      return -1;
   if(X2Max < 0)
      return -1;

   if(X2Max > ES * ES)
      X2Max = ES * ES;

   double X = sqrt(X2Max);
   double Y = ((X * X + DeltaE * DeltaE) * SumE - (X * X - DeltaE * DeltaE) * EMET) / (2 * X * DeltaE);

   double M2 = (SumE * X - DeltaE * Y) * (SumE * X - DeltaE * Y) / (X * X - DeltaE * DeltaE) / 4;

   if(M2 < 0)
      return -1;

   return sqrt(M2) * 2;
}
//----------------------------------------------------------------------------
double EstimateTransverseMass11(FourVector J1, FourVector J2, FourVector ME, FourVector ISR, char Variant, bool Reversal)
{
   Reversal = false;

   FourVector TempTotal = ME + J1 + J2 + ISR;
   double TempBetaZ = TempTotal[3] / TempTotal[0];

   FourVector TempJ1 = J1.Boost(FourVector(1, 0, 0, 1), TempBetaZ);
   FourVector TempJ2 = J2.Boost(FourVector(1, 0, 0, 1), TempBetaZ);
   FourVector TempME = ME.Boost(FourVector(1, 0, 0, 1), TempBetaZ);
   FourVector TempISR = ISR.Boost(FourVector(1, 0, 0, 1), TempBetaZ);

   TempTotal = TempJ1 + TempJ2 + TempME;
   double TempBeta = TempTotal.GetP() / TempTotal[0];

   TempJ1 = TempJ1.Boost(TempTotal, TempBeta);
   TempJ2 = TempJ2.Boost(TempTotal, TempBeta);
   TempME = TempME.Boost(TempTotal, TempBeta);

   double MT = 0;

   double EMET = TempME[0];
   double SumE = TempJ1[0] + TempJ2[0];
   double DeltaE = TempJ1[0] - TempJ2[0];
   double ES = (TempJ1 + TempJ2).GetP();
   double ED = (TempJ1 - TempJ2).GetP();
   double A = SumE * DeltaE / ES;
   double B = sqrt(ED * ED - A * A);

   double P1P1 = TempJ1.SpatialDot(TempJ1);
   double P1P2 = TempJ1.SpatialDot(TempJ2);

   double m0 = (SumE - EMET) / (2 * DeltaE) - SumE * DeltaE / ES / ES;
   double m1 = DeltaE * (SumE + EMET) / 2;

   double EQA = m0 * m0 + B * B / ES / ES;
   double EQB = 2 * m0 * m1 - B * B;
   double EQC = m1 * m1;

   double X2Max = (-EQB + sqrt(EQB * EQB - 4 * EQA * EQC)) / (2 * EQA);
   // double X2Min = (-EQB - sqrt(EQB * EQB - 4 * EQA * EQC)) / (2 * EQA);

   double X = sqrt(X2Max);
   double Y = ((X * X + DeltaE * DeltaE) * SumE - (X * X - DeltaE * DeltaE) * EMET) / (2 * X * DeltaE);
   double M2 = (SumE * X - DeltaE * Y) * (SumE * X - DeltaE * Y) / (X * X - DeltaE * DeltaE) / 4;

   double A0 = X / ES;
   double B0 = (Y - A * A0) / B;
   double C0 = sqrt(1 - A0 * A0 - B0 * B0);
   if(A0 * A0 + B0 * B0 >= 1)
      C0 = 0;

   FourVector DirectionA = (TempJ1 + TempJ2).SpatialNormalize();
   FourVector DirectionB = ((TempJ1 - TempJ2) - DirectionA * A).SpatialNormalize();
   FourVector DirectionC = DirectionA.SpatialCross(DirectionB);   // ambiguity in direction...

   double BetaCMSize = DeltaE / X;
   FourVector BetaCM = (DirectionA * A0 + DirectionB * B0 + DirectionC * C0).SpatialNormalize() * BetaCMSize;
      
   FourVector J1Boosted = TempJ1.Boost(BetaCM, -BetaCMSize);
   FourVector J2Boosted = TempJ2.Boost(BetaCM, BetaCMSize);

   if(fabs(J1Boosted.GetP() - J2Boosted.GetP()) > 0.1)   // wrong boost direction!
   {
      J1Boosted = TempJ1.Boost(BetaCM, BetaCMSize);
      J2Boosted = TempJ2.Boost(BetaCM, -BetaCMSize);
   }

   if(Variant == 'a')
      MT = GetMRT(TempJ1, TempJ2, TempME);
   if(Variant == 'b')
   {
      FourVector TransverseVector = (TempJ1 - (TempJ1 + TempJ2) * (P1P1 + P1P2) / (ES * ES)) * 2;
      MT = sqrt(M2 * 4 - TransverseVector.GetP2());
   }
   if(Variant == 'c')
   {
      double MT1 = J1Boosted.GetPT();
      double MT2 = J2Boosted.GetPT();
      MT = sqrt(M2) * 2 - (MT1 + MT2);
      J2Boosted = TempJ2.Boost(BetaCM, -BetaCMSize);
   }

   if(Variant == 'a')
      MT = GetMRT(TempJ1, TempJ2, TempME);
   if(Variant == 'b')
   {
      FourVector TransverseVector = (TempJ1 - (TempJ1 + TempJ2) * (P1P1 + P1P2) / (ES * ES)) * 2;
      MT = sqrt(M2 * 4 - TransverseVector.GetP2());
   }
   if(Variant == 'c')
   {
      double MT1 = J1Boosted.GetPT();
      double MT2 = J2Boosted.GetPT();
      MT = sqrt(M2) * 2 - (MT1 + MT2);
   }
   if(Variant == 'd')
   {
      double MT1 = J1Boosted.GetPT();
      double MT2 = J2Boosted.GetPT();
      MT = sqrt(M2) * 2 - sqrt(MT1 * MT1 + MT2 * MT2) * sqrt(2);
   }
   if(Variant == 'e')
      MT = (J1Boosted + J2Boosted).GetPT();
   if(Variant == 'f')
   {
      double MT1 = J1Boosted.GetPT();
      double MT2 = J2Boosted.GetPT();
      MT = sqrt(2 * M2 - MT1 * MT1 - MT2 * MT2);
   }
   if(Variant == 'g')
      MT = GetMRT(TempJ1, TempJ2, TempME) / BetaToGamma(BetaCMSize);

   return MT;
}
//----------------------------------------------------------------------------
/*
int FindCategory(GenParticleTree &Tree, int index)
{
   std::vector<int> Daughters = Tree[index].Daughters;

   int PiZeroCount = 0;
   int PiPlusCount = 0;
   int PiMinusCount = 0;

   for(int i = 0; i < (int)Daughters.size(); i++)
   {
      if(Tree[Daughters[i]].PDGID == 13 || Tree[Daughters[i]].PDGID == -13)   // detect muon
         return 1;
      if(Tree[Daughters[i]].PDGID == 11 || Tree[Daughters[i]].PDGID == -11)   // detect electron
         return 2;
      if(Tree[Daughters[i]].PDGID == 15 || Tree[Daughters[i]].PDGID == -15)   // detect tau
         return -1;

      if(Tree[Daughters[i]].PDGID == 111)   // pi0
         PiZeroCount = PiZeroCount + 1;
      if(Tree[Daughters[i]].PDGID == 211)   // pi+
         PiPlusCount = PiPlusCount + 1;
      if(Tree[Daughters[i]].PDGID == -211)   // pi-
         PiMinusCount = PiMinusCount + 1;

      // rho, omega => look at their decays
      if(Tree[Daughters[i]].PDGID == 223 || Tree[Daughters[i]].PDGID == 213
         || Tree[Daughters[i]].PDGID == 113 || Tree[Daughters[i]].PDGID == -213)
      {
         std::vector<int> DaughterList = Tree[Daughters[i]].Daughters;

         for(int j = 0; j < (int)DaughterList.size(); j++)
         {
            if(Tree[DaughterList[j]].PDGID == 111)   // pi0
               PiZeroCount = PiZeroCount + 1;
            if(Tree[DaughterList[j]].PDGID == 211)   // pi+
               PiPlusCount = PiPlusCount + 1;
            if(Tree[DaughterList[j]].PDGID == -211)   // pi-
               PiMinusCount = PiMinusCount + 1;
         }
      }
   }

   if(PiMinusCount == 1 && PiZeroCount == 0 && PiPlusCount == 0)
      return 3;
   if(PiMinusCount == 1 && PiZeroCount == 1 && PiPlusCount == 0)
      return 4;
   if(PiMinusCount == 1 && PiZeroCount == 2 && PiPlusCount == 0)
      return 5;
   if(PiMinusCount == 2 && PiZeroCount == 0 && PiPlusCount == 1)
      return 6;
   if(PiMinusCount == 2 && PiZeroCount == 1 && PiPlusCount == 1)
      return 7;
   
   if(PiMinusCount == 0 && PiZeroCount == 0 && PiPlusCount == 1)
      return 3;
   if(PiMinusCount == 0 && PiZeroCount == 1 && PiPlusCount == 1)
      return 4;
   if(PiMinusCount == 0 && PiZeroCount == 2 && PiPlusCount == 1)
      return 5;
   if(PiMinusCount == 1 && PiZeroCount == 0 && PiPlusCount == 2)
      return 6;
   if(PiMinusCount == 1 && PiZeroCount == 1 && PiPlusCount == 2)
      return 7;

   return 8;
}
*/
//----------------------------------------------------------------------------



