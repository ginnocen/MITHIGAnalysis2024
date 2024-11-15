/* + Descriptions:
 *		Macro to dump the new skim format into .txt and histograms in .root, for the validations btw two skims
 * + Output:
 * 		+ .txt:  printing of event-, reco-D- and gen-D-level quantities
 * 		+ .root: contains histograms defined in ValidateHist.h
 * + Todo:
 *		+ The current MC skim doesn't have the gammaN, Ngamma info, so I assume 0 for the current being
 */
#include <cstdio>
#include <iostream>
#include <string>
using namespace std;

#include "TFile.h"
#include "TTree.h"

#include "CommandLine.h"
#include "CommonFunctions.h"
#include "Messenger.h"
#include "ProgressBar.h"

#include "ValidateHist.h"

int main(int argc, char *argv[]) {
  CommandLine CL(argc, argv);

  string InputFileName = CL.Get("Input");
  string OutputName = CL.Get("Output");
  bool IsData = CL.GetBool("IsData");

  double Fraction = CL.GetDouble("Fraction", 1.00);
  bool ApplyEventSelection = CL.GetBool("ApplyEventSelection", true);
  double SkimDzeroPresenceAbovePTMin = CL.GetDouble("SkimDzeroPresenceAbovePTMin", 2.);
  bool ApplyDSelection = CL.GetBool("ApplyDSelection", false);

  TFile InputFile(InputFileName.c_str());

  DzeroUPCTreeMessenger MDzeroUPC(InputFile, "Tree", true);

  FILE *outfile = fopen((OutputName + ".txt").c_str(), "w");
  if (outfile == nullptr) {
    perror("Error opening outfile");
    exit(EXIT_FAILURE);
  }

  ValidateHist v((OutputName + ".root").c_str(), "recreate");

  int EntryCount = MDzeroUPC.GetEntries() * Fraction;
  ProgressBar Bar(cout, EntryCount);
  Bar.SetStyle(-1);

  for (int iE = 0; iE < EntryCount; iE++) {
    if (EntryCount < 300 || (iE % (EntryCount / 250)) == 0) {
      Bar.Update(iE);
      Bar.Print();
    }

    MDzeroUPC.GetEntry(iE);

    bool DRequirement = false;
    for (int iD = 0; iD < MDzeroUPC.Dpt->size(); ++iD) {
      if (MDzeroUPC.Dpt->at(iD) > SkimDzeroPresenceAbovePTMin) {
        DRequirement = true;
        break;
      }
    }

    bool passEventSelection = (IsData) ? (MDzeroUPC.isL1ZDCOr || MDzeroUPC.isL1ZDCXORJet8) &&
                                             MDzeroUPC.selectedBkgFilter && MDzeroUPC.selectedVtxFilter &&
                                             (MDzeroUPC.gammaN->at(4) || MDzeroUPC.Ngamma->at(4)) && DRequirement
                                       : true; // don't skim on MC
    if (ApplyEventSelection && !passEventSelection)
      continue;

    if (IsData) {
      fprintf(outfile,
              // "[Entry %d] "
              "Run %d, Lumi %d, Event %d\n"
              "isL1ZDCOr %d, isL1ZDCXORJet8 %d\n"
              "selectedBkgFilter %d, selectedVtxFilter %d\n"
              "gammaN %d, Ngamma %d\n"
              "VX %f VY %f VZ %f\n"
              "VXError %f VYError %f VZError %f\n"
              "Dsize %d\n", // Dpt->size() %d\n",
              // iE,
              MDzeroUPC.Run, MDzeroUPC.Lumi, MDzeroUPC.Event, (int)MDzeroUPC.isL1ZDCOr, (int)MDzeroUPC.isL1ZDCXORJet8,
              (int)MDzeroUPC.selectedBkgFilter, (int)MDzeroUPC.selectedVtxFilter, (int)(MDzeroUPC.gammaN->at(4)),
              (int)(MDzeroUPC.Ngamma->at(4)), MDzeroUPC.VX, MDzeroUPC.VY, MDzeroUPC.VZ, MDzeroUPC.VXError,
              MDzeroUPC.VYError, MDzeroUPC.VZError, MDzeroUPC.Dsize);

      v.FillEventInfo(MDzeroUPC.Run, MDzeroUPC.Lumi, MDzeroUPC.Event, MDzeroUPC.VX, MDzeroUPC.VY, MDzeroUPC.VZ,
                      MDzeroUPC.VXError, MDzeroUPC.VYError, MDzeroUPC.VZError, MDzeroUPC.Dsize);
    } else {
      fprintf(outfile,
              // "[Entry %d] "
              "Run %d, Lumi %d, Event %d\n"
              "VX %f VY %f VZ %f\n"
              "VXError %f VYError %f VZError %f\n"
              "Dsize %d Gsize %d\n", // Dpt->size() %d\n",
              // iE,
              MDzeroUPC.Run, MDzeroUPC.Lumi, MDzeroUPC.Event, MDzeroUPC.VX, MDzeroUPC.VY, MDzeroUPC.VZ,
              MDzeroUPC.VXError, MDzeroUPC.VYError, MDzeroUPC.VZError, MDzeroUPC.Dsize, MDzeroUPC.Gsize);

      v.FillEventInfo(MDzeroUPC.Run, MDzeroUPC.Lumi, MDzeroUPC.Event, MDzeroUPC.VX, MDzeroUPC.VY, MDzeroUPC.VZ,
                      MDzeroUPC.VXError, MDzeroUPC.VYError, MDzeroUPC.VZError, MDzeroUPC.Dsize, MDzeroUPC.Gsize);
    }

    v.FillEventSelectionInfo((int)MDzeroUPC.isL1ZDCOr, (int)MDzeroUPC.isL1ZDCXORJet8, (int)MDzeroUPC.selectedBkgFilter,
                             (int)MDzeroUPC.selectedVtxFilter, (IsData) ? (int)(MDzeroUPC.gammaN->at(4)) : 0,
                             (IsData) ? (int)(MDzeroUPC.Ngamma->at(4)) : 0);

    for (int iD = 0; iD < MDzeroUPC.Dpt->size(); ++iD) {
      bool passDSelection = MDzeroUPC.DpassCut->at(iD);
      if (ApplyDSelection && !passDSelection)
        continue;

      fprintf(outfile,
              "[D %d] "
              "Dpt %f Dy %f Dmass %f\n"
              "Dtrk1Pt %f Dtrk2Pt %f\n"
              "Dchi2cl %f DsvpvDistance %f DsvpvDisErr %f\n"
              "Dalpha %f DsvpvDistance_2D %f DsvpvDisErr_2D %f\n"
              "Ddtheta %f\n",
              iD, MDzeroUPC.Dpt->at(iD), MDzeroUPC.Dy->at(iD), MDzeroUPC.Dmass->at(iD), MDzeroUPC.Dtrk1Pt->at(iD),
              MDzeroUPC.Dtrk2Pt->at(iD), MDzeroUPC.Dchi2cl->at(iD), MDzeroUPC.DsvpvDistance->at(iD),
              MDzeroUPC.DsvpvDisErr->at(iD), MDzeroUPC.Dalpha->at(iD), MDzeroUPC.DsvpvDistance_2D->at(iD),
              MDzeroUPC.DsvpvDisErr_2D->at(iD), MDzeroUPC.Ddtheta->at(iD));

      v.FillRecoDInfo(MDzeroUPC.Dpt->at(iD), MDzeroUPC.Dy->at(iD), MDzeroUPC.Dmass->at(iD), MDzeroUPC.Dtrk1Pt->at(iD),
                      MDzeroUPC.Dtrk2Pt->at(iD), MDzeroUPC.Dchi2cl->at(iD), MDzeroUPC.DsvpvDistance->at(iD),
                      MDzeroUPC.DsvpvDisErr->at(iD), MDzeroUPC.Dalpha->at(iD), MDzeroUPC.DsvpvDistance_2D->at(iD),
                      MDzeroUPC.DsvpvDisErr_2D->at(iD), MDzeroUPC.Ddtheta->at(iD));

      if (!IsData) {
        fprintf(outfile, "Dgen %d DisSignalCalc %d DisSignalCalcPrompt %d DisSignalCalcFeeddown %d\n",
                MDzeroUPC.Dgen->at(iD), (int)(MDzeroUPC.DisSignalCalc->at(iD)),
                (int)(MDzeroUPC.DisSignalCalcPrompt->at(iD)), (int)(MDzeroUPC.DisSignalCalcFeeddown->at(iD)));

        v.FillRecoDGenMatchedInfo(MDzeroUPC.Dgen->at(iD), (int)(MDzeroUPC.DisSignalCalc->at(iD)),
                                  (int)(MDzeroUPC.DisSignalCalcPrompt->at(iD)),
                                  (int)(MDzeroUPC.DisSignalCalcFeeddown->at(iD)));
      }
    }

    if (!IsData) {
      for (int iG = 0; iG < MDzeroUPC.Gpt->size(); ++iG) {
        fprintf(outfile,
                "[G %d] "
                "Gpt %f Gy %f\n"
                "GisSignalCalc %d GisSignalCalcPrompt %d GisSignalCalcFeeddown %d\n",
                iG, MDzeroUPC.Gpt->at(iG), MDzeroUPC.Gy->at(iG), (int)(MDzeroUPC.GisSignalCalc->at(iG)),
                (int)(MDzeroUPC.GisSignalCalcPrompt->at(iG)), (int)(MDzeroUPC.GisSignalCalcFeeddown->at(iG)));

        v.FillGenDInfo(MDzeroUPC.Gpt->at(iG), MDzeroUPC.Gy->at(iG), (int)(MDzeroUPC.GisSignalCalc->at(iG)),
                       (int)(MDzeroUPC.GisSignalCalcPrompt->at(iG)), (int)(MDzeroUPC.GisSignalCalcFeeddown->at(iG)));
      }
    }
  }
  v.Write();

  fclose(outfile);

  return 0;
}