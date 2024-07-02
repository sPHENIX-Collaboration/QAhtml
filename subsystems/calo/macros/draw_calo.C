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
  CaloGoodRunChecker* ch = new CaloGoodRunChecker();
  ex->SetCemcChecker(ch);
  ch->SetHistfile(rootfile);
  std::string mapsfile = ch->MakeHotColdDeadMaps();
  bool cemc_isgood = ch->CemcGoodRun();
  TCanvas* cemc_summ = ch->CemcMakeSummary(cemc_isgood);
  ex->SetCemcSummary(cemc_summ);

  cl->ReadHistogramsFromFile(mapsfile.c_str());
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  ch->DeleteHotColdDeadMaps();

  // Write good/bad run status to triage database
  // (don't actually do this yet -- 7/1/24)
  // ch->CemcWriteDB(cemc_isgood);

  gSystem->Exit(0);
  return ;
}
