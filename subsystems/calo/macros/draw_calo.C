#include <emcnoisytowerfinder/emcNoisyTowerFinder.h>
#include <qahtml/QADrawClient.h>
#include <calo/CaloDraw.h>

R__LOAD_LIBRARY(libemcNoisyTowerFinder.so)
R__LOAD_LIBRARY(libqadrawcalo.so)

void draw_calo(const std::string &rootfile) {
  
  // Produce the dead/hot tower file
  emcNoisyTowerFinder *calo = new emcNoisyTowerFinder("noisyTowerFinder","");
  std::string infile = rootfile;
  // next line is needed because QAHtml needs a valid run # string in the histogram filename
  std::string rootfile_base = infile.substr(infile.find_last_of("/") + 1);
  std::string mapsfile = "deadHotTowers_" + rootfile_base;
  // can only store one hot/cold/dead map per file --> need 3 files
  std::string outfile_cemc = "deadHotTowersCemc.root";
  std::string outfile_ihcal = "deadHotTowersIhcal.root";
  std::string outfile_ohcal = "deadHotTowersOhcal.root";
  calo->FindHot(infile, outfile_cemc, "h_CaloValid_cemc_etaphi_wQA"); 
  calo->set_hcal();
  calo->FindHot(infile, outfile_ihcal, "h_CaloValid_ihcal_etaphi_wQA"); 
  calo->FindHot(infile, outfile_ohcal, "h_CaloValid_ohcal_etaphi_wQA"); 
  // consolidate the maps into one file -- need to rename the histograms
  TFile* fcemc = new TFile(outfile_cemc.c_str(), "READ");
  TFile* fihcal = new TFile(outfile_ihcal.c_str(), "READ");
  TFile* fohcal = new TFile(outfile_ohcal.c_str(), "READ");
  TH2* hcemc = (TH2*)fcemc->Get("h_hot");
  TH2* hihcal = (TH2*)fihcal->Get("h_hot");
  TH2* hohcal = (TH2*)fohcal->Get("h_hot");
  /* std::string outfile = "deadHotTowersCemc_" + rootfile_base; */
  TFile* fmaps = new TFile(mapsfile.c_str(), "RECREATE");
  fmaps->cd();
  hcemc->Write("cemc_hotmap");
  hihcal->Write("ihcal_hotmap");
  hohcal->Write("ohcal_hotmap");
  fmaps->Close(); fcemc->Close(); fihcal->Close(); fohcal->Close();
  delete fmaps; delete fcemc; delete fihcal; delete fohcal;

  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  QADraw *ex = new CaloDraw();
  cl->registerDrawer(ex);

  cl->ReadHistogramsFromFile(rootfile);
  cl->ReadHistogramsFromFile(mapsfile.c_str());
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;

  // remove the extra files now that we're done
  gSystem->Exec(Form("rm %s", outfile_cemc.c_str()));
  gSystem->Exec(Form("rm %s", outfile_ihcal.c_str()));
  gSystem->Exec(Form("rm %s", outfile_ohcal.c_str()));
  gSystem->Exec(Form("rm %s", mapsfile.c_str()));

  gSystem->Exit(0);
  return ;
}
