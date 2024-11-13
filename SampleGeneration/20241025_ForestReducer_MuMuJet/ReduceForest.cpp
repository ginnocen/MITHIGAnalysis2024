#include <iostream>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"

#include "CommonFunctions.h"
#include "CommandLine.h"
#include "ProgressBar.h"
#include "Messenger.h"

#include "tnp_weight.h"
#include "trackingEfficiency2017pp.h"
#include "trackingEfficiency2018PbPb.h"
#include "trackingEfficiency2023PbPb.h"
#include "TrackResidualCorrector.h"

int main(int argc, char *argv[]);
double GetHFSum(PFTreeMessenger *M);
double GetGenHFSum(GenParticleTreeMessenger *M, int SubEvent = -1);

int main(int argc, char *argv[])
{
   string VersionString = "V8";
   
   CommandLine CL(argc, argv);

   vector<string> InputFileNames = CL.GetStringVector("Input");
   string OutputFileName = CL.Get("Output");

   bool IsData                        = CL.GetBool("IsData", false);
   bool IsPP                          = CL.GetBool("IsPP", false);
   int Year                           = CL.GetInt("Year", 2018);
   double Fraction                    = CL.GetDouble("Fraction", 1.00);
   double MinJetPT                    = CL.GetDouble("MinJetPT", 40);

   string PFTreeName                  = IsPP ? "pfcandAnalyzer/pfTree" : "particleFlowAnalyser/pftree";
   PFTreeName                         = CL.Get("PFTree", PFTreeName);

   TFile OutputFile(OutputFileName.c_str(), "RECREATE");
   TTree Tree("Tree", Form("Tree for MuMu tagged jet analysis (%s)", VersionString.c_str()));
   TTree InfoTree("InfoTree", "Information");

   MuMuJetMessenger MMuMuJet;
   MMuMuJet.SetBranch(&Tree);

   for(string InputFileName : InputFileNames)
   {
      TFile InputFile(InputFileName.c_str());

      HiEventTreeMessenger     MEvent(InputFile);
      TrackTreeMessenger       MTrackPP(InputFile);
      PbPbTrackTreeMessenger   MTrack(InputFile);
      GenParticleTreeMessenger MGen(InputFile);
      PFTreeMessenger          MPF(InputFile, PFTreeName);
      MuTreeMessenger          MMu(InputFile);
      SkimTreeMessenger        MSkim(InputFile);
      TriggerTreeMessenger     MTrigger(InputFile);
      
      int EntryCount = MEvent.GetEntries() * Fraction;
      ProgressBar Bar(cout, EntryCount);
      Bar.SetStyle(-1);
      
      /////////////////////////////////
      //////// Main Event Loop ////////
      /////////////////////////////////   

      for(int iE = 0; iE < EntryCount; iE++)
      {
         if(EntryCount < 300 || (iE % (EntryCount / 250)) == 0)
         {
            Bar.Update(iE);
            Bar.Print();
         }
         
         MEvent.GetEntry(iE);
         MGen.GetEntry(iE);
         if(IsPP == true)         MTrackPP.GetEntry(iE);
         else                     MTrack.GetEntry(iE);
         MPF.GetEntry(iE);
         MMu.GetEntry(iE);
         MSkim.GetEntry(iE);
         MTrigger.GetEntry(iE);

         MMuMuJet.Clear();

         ////////////////////////////////////////
         ////////// Global event stuff //////////
         ////////////////////////////////////////
         
         MMuMuJet.Run   = MEvent.Run;
         MMuMuJet.Lumi  = MEvent.Lumi;
         MMuMuJet.Event = MEvent.Event;
         MMuMuJet.hiBin = MEvent.hiBin;
         MMuMuJet.hiHF  = MEvent.hiHF;
         MMuMuJet.NPU   = 0;
         if(MEvent.npus->size() == 9)
            MMuMuJet.NPU = MEvent.npus->at(5);
         else if(MEvent.npus->size() > 1)
            MMuMuJet.NPU = MEvent.npus->at(0);
         else
            MMuMuJet.NPU = 0;

         ////////////////////////////
         ////////// Vertex //////////
         ////////////////////////////

         MMuMuJet.NVertex = 0;
         int BestVertex = -1;
         for(int i = 0; i < (IsPP ? MTrackPP.nVtx : MTrack.VX->size()); i++)
         {
            if(IsPP == true && (BestVertex < 0 || MTrackPP.sumPtVtx[i] > MTrackPP.sumPtVtx[BestVertex]))
               BestVertex = i;
            if(IsPP == false && (BestVertex < 0 || MTrack.VPTSum->at(i) > MTrack.VPTSum->at(BestVertex)))
               BestVertex = i;

            MMuMuJet.NVertex = MMuMuJet.NVertex + 1;
         }

         if(BestVertex >= 0)
         {
            MMuMuJet.VX      = IsPP ? MTrackPP.xVtx[BestVertex] : MTrack.VX->at(BestVertex);
            MMuMuJet.VY      = IsPP ? MTrackPP.yVtx[BestVertex] : MTrack.VY->at(BestVertex);
            MMuMuJet.VZ      = IsPP ? MTrackPP.zVtx[BestVertex] : MTrack.VZ->at(BestVertex);
            MMuMuJet.VXError = IsPP ? MTrackPP.xVtxErr[BestVertex] : MTrack.VXError->at(BestVertex);
            MMuMuJet.VYError = IsPP ? MTrackPP.yVtxErr[BestVertex] : MTrack.VYError->at(BestVertex);
            MMuMuJet.VZError = IsPP ? MTrackPP.zVtxErr[BestVertex] : MTrack.VZError->at(BestVertex);
         }

         /////////////////////////////////////
         ////////// Event selection //////////
         /////////////////////////////////////

         if(IsPP == true)
         {
            if(IsData == true)
            {
               int pprimaryVertexFilter = MSkim.PVFilter;
               int beamScrapingFilter = MSkim.BeamScrapingFilter;

               // Event selection criteria
               //    see https://twiki.cern.ch/twiki/bin/viewauth/CMS/HIPhotonJe5TeVpp2017PbPb2018
               if(pprimaryVertexFilter == 0 || beamScrapingFilter == 0)
                  continue;

               //HLT trigger to select dimuon events, see Kaya's note: AN2019_143_v12, p.5
               // FIXME: need to be replaced with the actual pp triggers
               /*
               int HLT_HIL1DoubleMuOpen_2018 = MTrigger.CheckTriggerStartWith("HLT_HIL1DoubleMu");
               int HLT_HIL2DoubleMuOpen_2018 = MTrigger.CheckTriggerStartWith("HLT_HIL2DoubleMu");
               int HLT_HIL3DoubleMuOpen_2018 = MTrigger.CheckTriggerStartWith("HLT_HIL3DoubleMu");
               if(HLT_HIL1DoubleMuOpen_2018 == 0 && HLT_HIL2DoubleMuOpen_2018 == 0 && HLT_HIL3DoubleMuOpen_2018 == 0)
                  continue;
               */
            }  
         }
         else
         {
            if(IsData == true)
            {
               int pprimaryVertexFilter = MSkim.PVFilter;
               int phfCoincFilter2Th4 = MSkim.HFCoincidenceFilter2Th4;
               int pclusterCompatibilityFilter = MSkim.ClusterCompatibilityFilter;

               // Event selection criteria
               //    see https://twiki.cern.ch/twiki/bin/viewauth/CMS/HIPhotonJe5TeVpp2017PbPb2018
               if(pprimaryVertexFilter == 0 || phfCoincFilter2Th4 == 0 || pclusterCompatibilityFilter == 0)
                  continue;

               //HLT trigger to select dimuon events, see Kaya's note: AN2019_143_v12, p.5
               // FIXME: need to be replaced with the actual PbPb triggers
               int HLT_HIL1DoubleMuOpen_2018 = MTrigger.CheckTriggerStartWith("HLT_HIL1DoubleMu");
               int HLT_HIL2DoubleMuOpen_2018 = MTrigger.CheckTriggerStartWith("HLT_HIL2DoubleMu");
               int HLT_HIL3DoubleMuOpen_2018 = MTrigger.CheckTriggerStartWith("HLT_HIL3DoubleMu");
               if(HLT_HIL1DoubleMuOpen_2018 == 0 && HLT_HIL2DoubleMuOpen_2018 == 0 && HLT_HIL3DoubleMuOpen_2018 == 0)
                  continue;
            }
         }

         for(int ipair = 0; ipair < MMu.NDi; ipair++)
         {
            // We want opposite-charge muons with some basic kinematic cuts
            if(MMu.DiCharge1[ipair] == MMu.DiCharge2[ipair])        continue;
            if(fabs(MMu.DiEta1[ipair]) > 2.4)                       continue;
            if(fabs(MMu.DiEta2[ipair]) > 2.4)                       continue;
            if(fabs(MMu.DiPT1[ipair]) < 5)                          continue;
            if(fabs(MMu.DiPT2[ipair]) < 5)                          continue;
            //FIXME: this is meant at removing dimuons from Z0 decays
            if(MMu.DiMass[ipair] > 50)                              continue;

            TLorentzVector Mu1, Mu2;
            Mu1.SetPtEtaPhiM(MMu.DiPT1[ipair], MMu.DiEta1[ipair], MMu.DiPhi1[ipair], M_MU);
            Mu2.SetPtEtaPhiM(MMu.DiPT2[ipair], MMu.DiEta2[ipair], MMu.DiPhi2[ipair], M_MU);
            TLorentzVector MuMu = Mu1 + Mu2;
            if(fabs(MuMu.Rapidity()) > 2.4)
               continue;

            MMuMuJet.MuMuMass->push_back(MMu.DiMass[ipair]);
            MMuMuJet.MuMuEta->push_back(MMu.DiEta[ipair]);
            MMuMuJet.MuMuY->push_back(MuMu.Rapidity());
            MMuMuJet.MuMuPhi->push_back(MMu.DiPhi[ipair]);
            MMuMuJet.MuMuPt->push_back(MMu.DiPT[ipair]);
            MMuMuJet.muEta1->push_back(MMu.DiEta1[ipair]);
            MMuMuJet.muEta2->push_back(MMu.DiEta2[ipair]);
            MMuMuJet.muPhi1->push_back(MMu.DiPhi1[ipair]);
            MMuMuJet.muPhi2->push_back(MMu.DiPhi2[ipair]);
            MMuMuJet.muPt1->push_back(MMu.DiPT1[ipair]);
            MMuMuJet.muPt2->push_back(MMu.DiPT2[ipair]);

            double deltaMuEta = MMu.DiEta1[ipair] - MMu.DiEta2[ipair];
            double deltaMuPhi = PhiRangePositive(DeltaPhi(MMu.DiPhi1[ipair], MMu.DiPhi2[ipair]));
            // dimuon properties
            MMuMuJet.muDeta->push_back(deltaMuEta);
            MMuMuJet.muDphi->push_back(deltaMuPhi);
            MMuMuJet.muDR->push_back(sqrt(deltaMuEta * deltaMuEta + deltaMuPhi * deltaMuPhi));
            double deltaPhiStar = tan((M_PI - deltaMuPhi) / 2) * sqrt(1 - tanh(deltaMuEta / 2) * tanh(deltaMuEta / 2));
            MMuMuJet.muDphiS->push_back(deltaPhiStar);
         }
         MMuMuJet.FillEntry();
      }
   
      Bar.Update(EntryCount);
      Bar.Print();
      Bar.PrintLine();
   
      InputFile.Close();
   }

   OutputFile.cd();
   Tree.Write();
   InfoTree.Write();

   OutputFile.Close();

   return 0;
}

double GetHFSum(PFTreeMessenger *M)
{
   if(M == nullptr)
      return -1;
   if(M->Tree == nullptr)
      return -1;

   double Sum = 0;
   for(int iPF = 0; iPF < M->ID->size(); iPF++)
   {
      if(fabs(M->Eta->at(iPF)) < 3)
         continue;
      if(fabs(M->Eta->at(iPF)) > 5)
         continue;
      Sum = Sum + M->E->at(iPF);
   }

   // cout << Sum << endl;

   return Sum;
}

double GetGenHFSum(GenParticleTreeMessenger *M, int SubEvent)
{
   if(M == nullptr)
      return -1;
   if(M->Tree == nullptr)
      return -1;

   double Sum = 0;
   for(int iGen = 0; iGen < M->Mult; iGen++)
   {
      if(fabs(M->Eta->at(iGen)) < 3)
         continue;
      if(fabs(M->Eta->at(iGen)) > 5)
         continue;
      if(M->DaughterCount->at(iGen) > 0)
         continue;
      if(M->PT->at(iGen) < 0.4)   // for now...
         continue;

      if(SubEvent >= 0)   // if SubEvent >= 0, check subevent
      {
         if(M->SubEvent->at(iGen) != SubEvent)
            continue;
      }

      Sum = Sum + M->PT->at(iGen) * cosh(M->Eta->at(iGen));
   }

   return Sum;
}

