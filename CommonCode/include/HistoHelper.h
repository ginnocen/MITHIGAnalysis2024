#ifndef _HISTOHELPER_H_
#define _HISTOHELPER_H_

#include "TH1D.h"
#include "TLegend.h"

void normalizeHistoBinWidth(TH1D* histo)
{
  for (int i = 1; i <= histo->GetNbinsX(); ++i)
  {
    double binWidth = histo->GetXaxis()->GetBinWidth(i);
    histo->SetBinContent(i, histo->GetBinContent(i) / binWidth);
    histo->SetBinError(i, histo->GetBinError(i) / binWidth);
  }
}

void formatLegend(TLegend* leg, double textsize=27)
{
  leg->SetBorderSize(0);
  leg->SetTextFont(43);
  leg->SetTextSize(textsize);
  leg->SetFillStyle(0);
  leg->SetFillColor(0);
  leg->SetLineColor(0);
}

#endif