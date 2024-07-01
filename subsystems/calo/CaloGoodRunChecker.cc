#include "CaloGoodRunChecker.h"
#include <emcnoisytowerfinder/emcNoisyTowerFinder.h>
#include <qahtml/QADrawClient.h>

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

std::string CaloGoodRunChecker::MakeHotColdDeadMaps()
{
  emcNoisyTowerFinder *calo = new emcNoisyTowerFinder("noisyTowerFinder","");
  // next line is needed because QAHtml needs a valid run # string in the histogram filename
  std::string histfile_base = histfile.substr(histfile.find_last_of("/") + 1);
  mapsfile = mapsfile_prefix + histfile_base;
  // can only store one hot/cold/dead map per file --> need 3 files
  std::string outfile_cemc = "deadHotTowersCemc" + histfile_base;
  std::string outfile_ihcal = "deadHotTowersIhcal" + histfile_base;
  std::string outfile_ohcal = "deadHotTowersOhcal" + histfile_base;
  calo->FindHot(histfile, outfile_cemc, "h_CaloValid_cemc_etaphi_wQA"); 
  calo->set_hcal();
  calo->FindHot(histfile, outfile_ihcal, "h_CaloValid_ihcal_etaphi_wQA"); 
  calo->FindHot(histfile, outfile_ohcal, "h_CaloValid_ohcal_etaphi_wQA"); 
  // consolidate the maps into one file -- need to rename the histograms
  TFile* fcemc = new TFile(outfile_cemc.c_str(), "READ");
  TFile* fihcal = new TFile(outfile_ihcal.c_str(), "READ");
  TFile* fohcal = new TFile(outfile_ohcal.c_str(), "READ");
  cemc_hcdmap = (TH2*)fcemc->Get("h_hot");
  ihcal_hcdmap = (TH2*)fihcal->Get("h_hot");
  ohcal_hcdmap = (TH2*)fohcal->Get("h_hot");
  TFile* fmaps = new TFile(mapsfile.c_str(), "RECREATE");
  fmaps->cd();
  cemc_hcdmap->Write("cemc_hotmap");
  ihcal_hcdmap->Write("ihcal_hotmap");
  ohcal_hcdmap->Write("ohcal_hotmap");

  // clean up
  fmaps->Close(); fcemc->Close(); fihcal->Close(); fohcal->Close();
  delete fmaps; delete fcemc; delete fihcal; delete fohcal;
  // remove the extra files now that we're done
  gSystem->Exec(Form("rm %s", outfile_cemc.c_str()));
  gSystem->Exec(Form("rm %s", outfile_ihcal.c_str()));
  gSystem->Exec(Form("rm %s", outfile_ohcal.c_str()));

  return mapsfile;
}

void CaloGoodRunChecker::DeleteHotColdDeadMaps()
{
  gSystem->Exec(Form("rm %s", mapsfile.c_str()));
}


bool CaloGoodRunChecker::CemcGoodRun()
{
  TFile* hfile = new TFile(histfile.c_str(), "READ");
  TFile* mapfile = new TFile(mapsfile.c_str(), "READ");

  TH1 *zdcNorthcalib = dynamic_cast<TH1 *>(hfile->Get(Form("%szdcNorthcalib", histprefix.c_str())));
  TH2 *cemc_etaphi_time = dynamic_cast<TH2 *>(hfile->Get(Form("%scemc_etaphi_time", histprefix.c_str())));
  TH1 *vtx_z = dynamic_cast<TH1 *>(hfile->Get(Form("%svtx_z_raw", histprefix.c_str())));
  cemc_hcdmap = dynamic_cast<TH2 *>(mapfile->Get("cemc_hotmap"));

  // Number of events
  if (zdcNorthcalib)
  {
    n_events = zdcNorthcalib->GetEntries();
  }
  int MINEVENTS = 100000;
  if (n_events < MINEVENTS)
  {
    cemc_fails_events = true;
  }

  // Bad towers
  if (cemc_hcdmap)
  {
    cemc_dead_towers = 0;
    cemc_hot_towers = 0;
    cemc_cold_towers = 0;
    int nbins = cemc_hcdmap->GetNcells();
    for (int i=0; i<=nbins; i++)
    {
      if (cemc_hcdmap->GetBinContent(i) == 1) cemc_dead_towers++;
      if (cemc_hcdmap->GetBinContent(i) == 2) cemc_hot_towers++;
      if (cemc_hcdmap->GetBinContent(i) == 3) cemc_cold_towers++;
    }
  }
  int MAXHOTTOWERS = 100;
  if (cemc_hot_towers > MAXHOTTOWERS)
  {
    cemc_fails_badtowers = true;
  }
  int MAXCOLDDEADTOWERS = 500;
  if ((cemc_cold_towers + cemc_dead_towers) > MAXCOLDDEADTOWERS)
  {
    cemc_fails_badtowers = true;
  }

  // Hit timing
  if (cemc_etaphi_time)
  {
    int nx = cemc_etaphi_time->GetNbinsX();
    int ny = cemc_etaphi_time->GetNbinsY();
    float t_sum = 0;
    float t2_sum = 0;
    for (int ieta=0; ieta<nx; ieta++)
    {
      for (int iphi=0; iphi<ny; iphi++)
      {
	float t = cemc_etaphi_time->GetBinContent(ieta+1, iphi+1);
	t_sum += t;
	t2_sum += t*t;
      }
    }
    cemc_time_mean = t_sum/(nx*ny);
    cemc_time_sigma = sqrt((t2_sum/(nx*ny)) - (cemc_time_mean*cemc_time_mean));
  }
  float MINTIMEMEAN = -1.0;
  float MAXTIMEMEAN = 1.0;
  if ((cemc_time_mean < MINTIMEMEAN) || (cemc_time_mean > MAXTIMEMEAN))
  {
    cemc_fails_timing = true;
  }
  float MAXTIMESIGMA = 2.0;
  if (cemc_time_sigma > MAXTIMESIGMA)
  {
    cemc_fails_timing = true;
  }

  // MBD vertex
  if (vtx_z)
  {
    vtxz_mean = vtx_z->GetMean();
    vtxz_sigma = vtx_z->GetStdDev();
  }
  float MAXABSVTXZ = 5.0;
  if (abs(vtxz_mean) > MAXABSVTXZ)
  {
    cemc_fails_vertex = true;
  }
  // remove vertex width cut -- not actually useful
  /*
  float MAXVTXZSIGMA = 20.0;
  if (vtxz_sigma > MAXVTXZSIGMA)
  {
    cemc_fails_vertex = true;
  }
  */

  bool failed_check = (cemc_fails_events || cemc_fails_badtowers || cemc_fails_timing || cemc_fails_vertex);
  if (failed_check) return false;
  else return true;
}

TCanvas* CaloGoodRunChecker::CemcMakeSummary(bool cemc_goodrun)
{
  QADrawClient *cl = QADrawClient::instance();
  int xsize = cl->GetDisplaySizeX();
  int ysize = cl->GetDisplaySizeY();
  // xpos (-1) negative: do not draw menu bar
  TCanvas* canvas = new TCanvas("cemcsummary", "", -1, 0, (int) (xsize / 1.2), (int) (ysize / 1.2));
  canvas->cd();

  int runno = cl->RunNumber();
  std::string runtime = cl->RunTime();
  int n_events_db = cl->EventsInRun();
  myText(0.5, 0.85, kBlack, Form("EMCal Summary - Run %d", runno), 0.08);
  
  if (cemc_goodrun)
  {
    myText(0.5, 0.70, kGreen, "Overall status: Good Run", 0.06);
  }
  else
  {
    myText(0.5, 0.70, kRed, "Overall status: Bad Run", 0.06);
  }
  myText(0.5, 0.60, kBlack, Form("Start time: %s", runtime.c_str()));
  myText(0.5, 0.55, kBlack, Form("Total events: %d CaloValid / %d from DB", n_events, n_events_db));
  if (cemc_fails_events)
  {
    myText(0.8, 0.55, kRed, "(bad)");
  }
  else
  {
    myText(0.8, 0.55, kGreen, "(good)");
  }
  myText(0.5, 0.50, kBlack, Form("Bad towers: %d dead, %d hot, %d cold", cemc_dead_towers, cemc_hot_towers, cemc_cold_towers));
  if (cemc_fails_badtowers)
  {
    myText(0.8, 0.50, kRed, "(bad)");
  }
  else
  {
    myText(0.8, 0.50, kGreen, "(good)");
  }
  myText(0.5, 0.45, kBlack, Form("Hit timing: mean = %.3f, sigma = %.3f", cemc_time_mean, cemc_time_sigma));
  if (cemc_fails_timing)
  {
    myText(0.8, 0.45, kRed, "(bad)");
  }
  else
  {
    myText(0.8, 0.45, kGreen, "(good)");
  }
  myText(0.5, 0.40, kBlack, Form("MBD vertex: mean = %.3f, sigma = %.3f", vtxz_mean, vtxz_sigma));
  if (cemc_fails_vertex)
  {
    myText(0.8, 0.40, kRed, "(bad)");
  }
  else
  {
    myText(0.8, 0.40, kGreen, "(good)");
  }

  // add the run number title
  TPad* tr = new TPad("transparent_cemc", "", 0, 0, 1, 1);
  tr->SetFillStyle(4000);
  tr->Draw();
  TText PrintRun;
  PrintRun.SetTextFont(62);
  PrintRun.SetTextSize(0.04);
  PrintRun.SetNDC();          // set to normalized coordinates
  PrintRun.SetTextAlign(23);  // center/top alignment
  std::ostringstream runnostream;
  std::string runstring;
  runnostream << "CaloQA_cemc_summary Run " << cl->RunNumber();
  runstring = runnostream.str();
  tr->cd();
  PrintRun.DrawText(0.5, 1., runstring.c_str());

  canvas->Update();
  return canvas;
}

void CaloGoodRunChecker::myText(double x, double y, int color, const char *text, double tsize)
{
  TLatex l;
  l.SetTextAlign(22);
  l.SetTextSize(tsize);
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x, y, text);
}

