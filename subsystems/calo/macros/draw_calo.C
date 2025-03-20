#include <qahtml/QADrawClient.h>
#include <calo/CaloDraw.h>
#include <calo/CaloGoodRunChecker.h>
#include <sPhenixStyle.C>

R__LOAD_LIBRARY(libqadrawcalo.so)

void draw_calo(const std::string &rootfile) {
  SetsPhenixStyle();
  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  CaloDraw *ex = new CaloDraw();
  cl->registerDrawer(ex);
  cl->ReadHistogramsFromFile(rootfile);

  // Tower masking & good/bad run determination
  CaloGoodRunChecker* ch = new CaloGoodRunChecker();
  ex->SetCaloChecker(ch);
  ch->SetHistfile(rootfile);
  std::string mapsfile = ch->MakeHotColdDeadMaps();

  // EMCal
  ch->CemcCheckGoodRun();
  TCanvas* cemc_summ = ch->CemcMakeSummary();
  ex->SetCemcSummary(cemc_summ);

  // iHCal
  ch->ihcalCheckGoodRun();
  TCanvas* ihcal_summ = ch->ihcalMakeSummary();
  ex->SetihcalSummary(ihcal_summ);

  // oHCal
  ch->ohcalCheckGoodRun();
  TCanvas* ohcal_summ = ch->ohcalMakeSummary();
  ex->SetohcalSummary(ohcal_summ);

  // Debug output before reading histograms
  // std::cout << "Reading histograms from maps files..." << std::endl;

  cl->ReadHistogramsFromFile(mapsfile.c_str());
  /* cl->Print("ALL"); */

  // Debug output before making HTML
  // std::cout << "Making HTML and saving PNG files..." << std::endl;

  cl->MakeHtml();

  // Debug output after making HTML
  // std::cout << "HTML and PNG files should be saved now." << std::endl;

  // Debug output before deleting maps
  // std::cout << "Deleting hot/cold/dead maps..." << std::endl;

  ch->DeleteHotColdDeadMaps();

  // Debug output after deleting maps
  // std::cout << "Hot/cold/dead maps deleted." << std::endl;

  // Write good/bad run status to triage database
  //  std::cout << "Writing emcal_auto to run triage DB... ";
  //ch->CaloWriteDB("emcal");
  //std::cout << "Done!!" << std::endl;
  //std::cout << "Writing ihcal_auto to run triage DB... ";
  //ch->CaloWriteDB("ihcal");
  //std::cout << "Done!" << std::endl;
  //std::cout << "Writing ohcal_auto to run triage DB... ";
  //ch->CaloWriteDB("ohcal");
  //std::cout << "Done!" << std::endl;

  delete cl;

  gSystem->Exit(0);
  return ;
}
