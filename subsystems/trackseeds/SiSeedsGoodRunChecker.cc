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

#include <iostream>
#include <sstream>

bool SiSeedsGoodRunChecker::SiSeedsGoodRun()
{
  // get the reference histograms
  reffile = new TFile(Form("%s/HIST_DST_TRKR_SEED_run2pp_new_%s-000%d-9000.root", refhistbasedir.c_str(), prodtag.c_str(), refrunnumber));
  if (!reffile)
  {
    std::cout << "Cannot open reference file " << Form("%s/HIST_DST_TRKR_SEED_run2pp_new_%s-000%d-9000.root", refhistbasedir.c_str(), prodtag.c_str(), refrunnumber) << std::endl;
    return false;
  }
  refh_ntrack1d = dynamic_cast<TH1F *>(reffile->Get(Form("%snrecotracks1d", histprefix.c_str())));
  refh_ntrack = dynamic_cast<TH2F *>(reffile->Get(Form("%snrecotracks", histprefix.c_str())));
  refh_nmaps_nintt = dynamic_cast<TH2F *>(reffile->Get(Form("%snmaps_nintt", histprefix.c_str())));
  refh_avgnclus_eta_phi = dynamic_cast<TProfile2D *>(reffile->Get(Form("%savgnclus_eta_phi", histprefix.c_str())));
  refh_trackcrossing = dynamic_cast<TH1F *>(reffile->Get(Form("%strackcrossing", histprefix.c_str())));
  refh_trackchi2ndf = dynamic_cast<TH1F *>(reffile->Get(Form("%strackchi2ndf", histprefix.c_str())));

  checkfile = new TFile(histfile.c_str(), "READ");
  if (!checkfile)
  {
    std::cout << "Cannot open check file " << histfile << std::endl;
    return false;
  }
  h_ntrack1d = dynamic_cast<TH1F *>(checkfile->Get(Form("%snrecotracks1d", histprefix.c_str())));
  h_ntrack = dynamic_cast<TH2F *>(checkfile->Get(Form("%snrecotracks", histprefix.c_str())));
  h_nmaps_nintt = dynamic_cast<TH2F *>(checkfile->Get(Form("%snmaps_nintt", histprefix.c_str())));
  h_avgnclus_eta_phi = dynamic_cast<TProfile2D *>(checkfile->Get(Form("%savgnclus_eta_phi", histprefix.c_str())));
  h_trackcrossing = dynamic_cast<TH1F *>(checkfile->Get(Form("%strackcrossing", histprefix.c_str())));
  h_trackchi2ndf = dynamic_cast<TH1F *>(checkfile->Get(Form("%strackchi2ndf", histprefix.c_str())));

  // KS test
  Double_t KSscore_ntrack1d = h_ntrack1d->KolmogorovTest(refh_ntrack1d, "D");
  Double_t KSscore_ntrack = h_ntrack->KolmogorovTest(refh_ntrack, "D");
  Double_t KSscore_nmaps_nintt = h_nmaps_nintt->KolmogorovTest(refh_nmaps_nintt, "D");
  Double_t KSscore_avgnclus_eta_phi = h_avgnclus_eta_phi->KolmogorovTest(refh_avgnclus_eta_phi, "D");
  Double_t KSscore_trackcrossing = h_trackcrossing->KolmogorovTest(refh_trackcrossing, "D");
  Double_t KSscore_trackchi2ndf = h_trackchi2ndf->KolmogorovTest(refh_trackchi2ndf, "D");

  // Fill the KS test results
  h_KSSummary = dynamic_cast<TH1F *>(new TH1F("h_KSSummary", "KS Test Summary", 6, 0, 6));
  h_KSSummary->SetBinContent(1, KSscore_ntrack1d);
  h_KSSummary->SetBinContent(2, KSscore_ntrack);
  h_KSSummary->SetBinContent(3, KSscore_nmaps_nintt);
  h_KSSummary->SetBinContent(4, KSscore_avgnclus_eta_phi);
  h_KSSummary->SetBinContent(5, KSscore_trackcrossing);
  h_KSSummary->SetBinContent(6, KSscore_trackchi2ndf);

  bool goodrun_check = true;
  if (goodrun_check) 
    return true;
  else 
    return false;
}

TCanvas* SiSeedsGoodRunChecker::SiSeedsMakeSummary(bool siseeds_goodrun)
{
  QADrawClient *cl = QADrawClient::instance();
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();
  // xpos (-1) negative: do not draw menu bar
  TCanvas* canvas = new TCanvas("siseedssummary", "", -1, 0, (int) (xsize / 1.2), (int) (ysize / 1.2));
  canvas->cd();
  h_KSSummary->SetStats(0);
  h_KSSummary->Draw("histtext");

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