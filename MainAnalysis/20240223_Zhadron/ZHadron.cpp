#include <iostream>
#include <vector>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TH2D.h"
#include "TMath.h"
#include "TLorentzVector.h"

#include "Messenger.h"
#include "CommandLine.h"

#define M_MU 0.1056583755

double DPhi(double phi1, double phi2);
class ZhadronData;
void Zhadron(vector<string> infname, string outfname);

double DPhi(double phi1, double phi2)
{
   double dphi = phi1 - phi2;
   while(dphi < -M_PI)  dphi += 2 * M_PI;
   while(dphi > +M_PI)  dphi -= 2 * M_PI;
   return dphi;
}

class ZhadronData
{
public:
   vector<double> zMass;
   vector<double> zEta;
   vector<double> zPhi;
   vector<double> zPt;
   vector<double> genZMass;
   vector<double> genZEta;
   vector<double> genZPhi;
   vector<double> genZPt;
   vector<double> trackDphi;
   vector<double> trackPt;
   vector<double> trackDeta;
   int hiBin;

   ZhadronData(){};
   ~ZhadronData(){};
   void setBranch(TTree *t)
   {
      t->Branch("zMass",&zMass);
      t->Branch("zEta",&zEta);
      t->Branch("zPhi",&zPhi);
      t->Branch("zPt",&zPt);
      t->Branch("genZMass",&genZMass);
      t->Branch("genZEta",&genZEta);
      t->Branch("genZPhi",&genZPhi);
      t->Branch("genZPt",&genZPt);
      t->Branch("trackDphi",&trackDphi);
      t->Branch("trackDeta",&trackDeta);
      t->Branch("trackPt",&trackPt);
      t->Branch("hiBin",&hiBin);
   };

   void clear()
   {
      zMass.clear();
      zEta.clear();
      zPhi.clear();
      zPt.clear();
      genZMass.clear();
      genZEta.clear();
      genZPhi.clear();
      genZPt.clear();
      trackDphi.clear();
      trackPt.clear();
      trackDeta.clear();
   }
};

int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   string Default = "DYJetsToLL_MLL-50_TuneCP5_HydjetDrumMB_5p02TeV-amcatnloFXFX-pythia8_merged.root";
   vector<string> Input = CL.GetStringVector("Input", {Default});
   string Output        = CL.Get("Output", "output.root");

   Zhadron(Input, Output);
}

void Zhadron(vector<string> infname, string outfname)
{
   TFile BackgroundFile("hydjet.root");

   HiEventTreeMessenger   MBackgroundEvent(BackgroundFile);
   PbPbTrackTreeMessenger MBackgroundTrack(BackgroundFile);
   MuTreeMessenger        MBackgroundMu(BackgroundFile);

   TFile outfile(outfname.c_str(),"recreate");

   TNtuple *nt = new TNtuple("ntZ","Z tree","mass:pt:eta");
   TTree *t = new TTree("t","tree");

   TH2D *h2D = new TH2D("h2D","",100,-6,6,100,-3.14159,3.14159);
   TH2D *h2Dmix = new TH2D("h2Dmix","",100,-6,6,100,-3.14159,3.14159);

   ZhadronData data;
   data.setBranch(t);
   data.clear();

   TLorentzVector LgenMu1;
   TLorentzVector LgenMu2;
   TLorentzVector LgenZ;

   for(string inputfilename : infname)
   {
      TFile SignalFile(inputfilename.c_str());

      HiEventTreeMessenger   MSignalEvent(SignalFile);
      PbPbTrackTreeMessenger MSignalTrack(SignalFile);
      MuTreeMessenger        MSignalMu(SignalFile);

      for(int i = 0; i < MSignalEvent.GetEntries(); i++)
      {
         MSignalEvent.GetEntry(i);
         MSignalTrack.GetEntry(i);
         MSignalMu.GetEntry(i);

         // display the progress
         if(i % 1000 == 0)
            cout << i << "/" << MSignalEvent.GetEntries() << endl;

         // Loop over Gen information (single muons)
         if(MSignalMu.NGen > 1)
         {
            for(int igen1 = 0; igen1 < MSignalMu.NGen; igen1++)
            {
               if(MSignalMu.GenMom[igen1] != 23)
                  continue;

               LgenMu1.SetPtEtaPhiM(MSignalMu.GenPT[igen1],
                     MSignalMu.GenEta[igen1],
                     MSignalMu.GenPhi[igen1],
                     M_MU);

               for(int igen2 = igen1 + 1; igen2 < MSignalMu.NGen; igen2++)
               {
                  if(MSignalMu.GenMom[igen2] != 23)
                     continue;

                  LgenMu2.SetPtEtaPhiM(MSignalMu.GenPT[igen2],
                        MSignalMu.GenEta[igen2],
                        MSignalMu.GenPhi[igen2],
                        M_MU);

                  LgenZ = LgenMu1 + LgenMu2;
                  data.genZMass.push_back(LgenZ.M());
                  data.genZPt.push_back  (LgenZ.Pt());
                  data.genZPhi.push_back (LgenZ.Phi());
                  data.genZEta.push_back (LgenZ.Eta());
               }
            }
         }

         // loop over RECO information (dimuons)
         for(int ipair = 0; ipair < MSignalMu.NDi; ipair++)
         {
            //cout <<ipair<<" "<<MSignalMu.Di_mass[ipair]<<endl;
            if(MSignalMu.DiCharge1[ipair] == MSignalMu.DiCharge2[ipair])   continue;
            if(fabs(MSignalMu.DiEta1[ipair]) > 2.4)                        continue;
            if(fabs(MSignalMu.DiEta2[ipair]) > 2.4)                        continue;
            if(fabs(MSignalMu.DiPT1[ipair]) < 20)                          continue;
            if(fabs(MSignalMu.DiPT2[ipair]) < 20)                          continue;

            data.zMass.push_back(MSignalMu.DiMass[ipair]);
            data.zEta.push_back(MSignalMu.DiEta[ipair]);
            data.zPhi.push_back(MSignalMu.DiPhi[ipair]);
            data.zPt.push_back(MSignalMu.DiPT[ipair]);
            data.hiBin = MSignalEvent.hiBin;

            nt->Fill(MSignalMu.DiMass[ipair],MSignalMu.DiPT[ipair], MSignalMu.DiEta[ipair], MSignalMu.DiPhi[ipair]);
         }

         if(data.zMass.size() > 0 && data.zPt.at(0) > 30)
         {
            for(int itrack=0;itrack<MSignalTrack.TrackPT->size();itrack++)
            {
               if(MSignalTrack.TrackHighPurity->at(itrack) == false)
                  continue;

               double deltaPhi = DPhi(data.zPhi.at(0), MSignalTrack.TrackPhi->at(itrack) - M_PI);
               double deltaEta = fabs(data.zEta.at(0) - MSignalTrack.TrackEta->at(itrack));

               h2D->Fill(deltaEta, deltaPhi, 0.25);
               h2D->Fill(-deltaEta, deltaPhi, 0.25);
               h2D->Fill(-deltaEta, -deltaPhi, 0.25);
               h2D->Fill(deltaEta, -deltaPhi, 0.25);

               data.trackDphi.push_back(deltaPhi);
               data.trackDeta.push_back(deltaEta);
               data.trackPt.push_back(MSignalTrack.TrackPT->at(itrack));
            }

            int BackgroundEntry = (i + 1) % MBackgroundEvent.GetEntries();
            if(i + 1 == MBackgroundEvent.GetEntries())
               BackgroundEntry = 0;

            MBackgroundEvent.GetEntry(BackgroundEntry);
            MBackgroundTrack.GetEntry(BackgroundEntry);
            MBackgroundMu.GetEntry(BackgroundEntry);

            for(int itrack = 0; itrack < MBackgroundTrack.TrackPT->size(); itrack++)
            {
               if(MBackgroundTrack.TrackHighPurity->at(itrack) == false)
                  continue;

               double deltaPhi = DPhi(data.zPhi.at(0), MBackgroundTrack.TrackPhi->at(itrack) - M_PI);
               double deltaEta = fabs(data.zEta.at(0) - MBackgroundTrack.TrackEta->at(itrack));

               h2Dmix->Fill(deltaEta, deltaPhi, 0.25);
               h2Dmix->Fill(-deltaEta, deltaPhi, 0.25);
               h2Dmix->Fill(-deltaEta, -deltaPhi, 0.25);
               h2Dmix->Fill(deltaEta, -deltaPhi, 0.25);

               // data.trackDphi.push_back(deltaPhi);
               // data.trackDeta.push_back(deltaEta);
               // data.trackPt.push_back(MSignalTrack.TrackPT->at(itrack));
            }
         }

         t->Fill();
         data.clear();
      }
   
      SignalFile.Close();
   }

   h2D->Write();   
   nt->Write();
   outfile.Write();
   outfile.Close();      

   BackgroundFile.Close();
}