#include <qahtml/QADrawClient.h>
#include <calo/CaloDraw.h>
#include <calo/CaloGoodRunChecker.h>

R__LOAD_LIBRARY(libqadrawcalo.so)

void draw_calo(const std::string &rootfile) {
  
  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  CaloDraw *ex = new CaloDraw();
  cl->registerDrawer(ex);

  // Tower masking & good/bad run determination
  CaloGoodRunChecker* ch = new CaloGoodRunChecker();
  ex->SetCemcChecker(ch);
  ch->SetHistfile(rootfile);
  std::string mapsfile = ch->MakeHotColdDeadMaps();
  bool cemc_isgood = ch->CemcGoodRun();
  TCanvas* cemc_summ = ch->CemcMakeSummary(cemc_isgood);
  ex->SetCemcSummary(cemc_summ);

  cl->ReadHistogramsFromFile(rootfile);
  cl->ReadHistogramsFromFile(mapsfile.c_str());
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  ch->DeleteHotColdDeadMaps();

  gSystem->Exit(0);
  return ;
}
