#include "SiSeedsGoodRunChecker.h"
#include <qahtml/QADrawClient.h>

#include <odbc++/connection.h>
#include <odbc++/drivermanager.h>
#include <odbc++/errorhandler.h>
#include <odbc++/preparedstatement.h>
#include <odbc++/resultset.h>
#include <odbc++/types.h>

#include <TCanvas.h>
#include <TPad.h>
#include <TH1.h>
#include <TH1F.h>
#include <TH2.h>
#include <TProfile2D.h>
#include <TLatex.h>
#include <TString.h>
#include <TSystem.h>
#include <TFile.h>
#include <TLine.h>
#include <TStyle.h>

#include <iostream>
#include <sstream>

bool SiSeedsGoodRunChecker::SiSeedsGoodRun()
{
  // check how many bins in the histogram are above a certain threshold
  int n_bins = h_KSSummary->GetNbinsX();
  int n_over_threshold = 0;
  float sumscore = 0.;
  for (int i = 1; i <= n_bins; i++)
  {
    sumscore += h_KSSummary->GetBinContent(i);
    if (h_KSSummary->GetBinContent(i) > 0.5)
    {
      n_over_threshold++;
    }
  }
  float avg_score = sumscore / n_bins;
  bool goodrun_check = (avg_score > 0.5) && (n_over_threshold > 0.5 * n_bins);

  if (goodrun_check) 
    return true;
  else 
    return false;
}

TCanvas* SiSeedsGoodRunChecker::SiSeedsMakeSummary(int runnum, bool siseeds_goodrun)
{
  gStyle->SetPaintTextFormat(".3g");

  QADrawClient *cl = QADrawClient::instance();
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();
  // xpos (-1) negative: do not draw menu bar
  TCanvas* canvas = new TCanvas("siseedssummary", "", -1, 0, (int) (xsize / 1.2), (int) (ysize / 1.2));
  canvas->SetBottomMargin(0.4);
  canvas->cd();
  h_KSSummary->SetStats(0);
  h_KSSummary->GetYaxis()->SetRangeUser(0, 1.5);
  h_KSSummary->Draw("histtext");
  TLine *line = new TLine(h_KSSummary->GetXaxis()->GetXmin(), 0.5, h_KSSummary->GetXaxis()->GetXmax(), 0.5);
  line->SetLineColor(kRed);
  line->SetLineStyle(2);
  line->SetLineWidth(2);
  line->Draw("same");

  if (siseeds_goodrun)
  {
    myText(0.5, 0.9, kGreen, Form("Run %d Overall Status: Good Run", runnum), 0.06);
  }
  else
  {
    myText(0.5, 0.9, kRed, Form("Run %d Overall Status: Bad Run", runnum), 0.06);
  }

  canvas->Update();
  return canvas;
}

void SiSeedsGoodRunChecker::myText(double x, double y, int color, const char *text, double tsize)
{
  TLatex l;
  l.SetTextAlign(22);
  l.SetTextSize(tsize);
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x, y, text);
}