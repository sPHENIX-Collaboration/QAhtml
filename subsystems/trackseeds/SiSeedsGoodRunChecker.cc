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
#include <TH2.h>
#include <TLatex.h>
#include <TString.h>
#include <TSystem.h>
#include <TFile.h>

#include <iostream>
#include <sstream>

TCanvas* SiSeedsGoodRunChecker::SiSeedsMakeSummary(bool siseeds_goodrun)
{
  QADrawClient *cl = QADrawClient::instance();
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();
  // xpos (-1) negative: do not draw menu bar
  TCanvas* canvas = new TCanvas("siseedssummary", "", -1, 0, (int) (xsize / 1.2), (int) (ysize / 1.2));
  canvas->cd();

  int runno = cl->RunNumber();
  std::string runtime = cl->RunTime();
  myText(0.5, 0.85, kBlack, Form("Silicon Seeds QA Summary - Run %d", runno), 0.08);
  myText(0.5, 0.70, kBlack, "TESTING", 0.06);
  if (siseeds_goodrun)
  {
    myText(0.5, 0.60, kGreen, "Overall status: Good Run", 0.06);
  }
  else
  {
    myText(0.5, 0.60, kRed, "Overall status: Bad Run", 0.06);
  }

  // add the run number title
//   TPad* tr = new TPad("transparent_siseeds", "", 0, 0, 1, 1);
//   tr->SetFillStyle(4000);
//   tr->Draw();
//   TText PrintRun;
//   PrintRun.SetTextFont(62);
//   PrintRun.SetTextSize(0.04);
//   PrintRun.SetNDC();          // set to normalized coordinates
//   PrintRun.SetTextAlign(23);  // center/top alignment
//   std::ostringstream runnostream;
//   std::string runstring;
//   runnostream << "SiliconSeeds_summary Run " << cl->RunNumber();
//   runstring = runnostream.str();
//   tr->cd();
//   PrintRun.DrawText(0.5, 1., runstring.c_str());

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