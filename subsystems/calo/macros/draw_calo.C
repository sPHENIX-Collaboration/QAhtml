#include <qahtml/QADrawClient.h>
#include <calo/CaloDraw.h>
#include <calo/CaloGoodRunChecker.h>

R__LOAD_LIBRARY(libqadrawcalo.so)

void draw_calo(const std::string &rootfile) {
  
  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  CaloDraw *ex = new CaloDraw();
  cl->registerDrawer(ex);
  cl->ReadHistogramsFromFile(rootfile);

  // Tower masking & good/bad run determination
  CaloGoodRunChecker* ch_cemc = new CaloGoodRunChecker();
  ex->SetCemcChecker(ch_cemc);
  ch_cemc->SetHistfile(rootfile);
  std::string mapsfile_cemc = ch_cemc->MakeHotColdDeadMaps();
  bool cemc_isgood = ch_cemc->CemcGoodRun();
  TCanvas* cemc_summ = ch_cemc->CemcMakeSummary(cemc_isgood);
  ex->SetCemcSummary(cemc_summ);

  CaloGoodRunChecker* ch_ihcal = new CaloGoodRunChecker();
  ex->SetihcalChecker(ch_ihcal);
  ch_ihcal->SetHistfile(rootfile);
  std::string mapsfile_ihcal = ch_ihcal->MakeHotColdDeadMaps();
  bool ihcal_isgood = ch_ihcal->ihcalGoodRun();
  TCanvas* ihcal_summ = ch_ihcal->ihcalMakeSummary(ihcal_isgood);
  ex->SetihcalSummary(ihcal_summ);

  CaloGoodRunChecker* ch_ohcal = new CaloGoodRunChecker();
  ex->SetohcalChecker(ch_ohcal);
  ch_ohcal->SetHistfile(rootfile);
  std::string mapsfile_ohcal = ch_ohcal->MakeHotColdDeadMaps();
  bool ohcal_isgood = ch_ohcal->ohcalGoodRun();
  TCanvas* ohcal_summ = ch_ohcal->ohcalMakeSummary(ohcal_isgood);
  ex->SetohcalSummary(ohcal_summ);

  // Debug output before reading histograms
  std::cout << "Reading histograms from maps files..." << std::endl;

  //cl->ReadHistogramsFromFile(mapsfile.c_str());
  /* cl->Print("ALL"); */
  cl->ReadHistogramsFromFile(mapsfile_cemc.c_str());
  cl->ReadHistogramsFromFile(mapsfile_ihcal.c_str());
  cl->ReadHistogramsFromFile(mapsfile_ohcal.c_str());

  // Debug output before making HTML
  std::cout << "Making HTML and saving PNG files..." << std::endl;

  cl->MakeHtml();

  // Debug output after making HTML
  std::cout << "HTML and PNG files should be saved now." << std::endl;

  delete cl;

  // Debug output before deleting maps
  std::cout << "Deleting hot/cold/dead maps..." << std::endl;


  //  ch->DeleteHotColdDeadMaps();
  ch_cemc->DeleteHotColdDeadMaps();
  ch_ihcal->DeleteHotColdDeadMaps();
  ch_ohcal->DeleteHotColdDeadMaps();

  // Debug output after deleting maps
  std::cout << "Hot/cold/dead maps deleted." << std::endl;

  gSystem->Exit(0);
  return ;
}
