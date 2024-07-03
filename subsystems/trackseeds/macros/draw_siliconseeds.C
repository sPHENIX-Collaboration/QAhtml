#include <qahtml/QADrawClient.h>
#include "trackseeds/SiliconSeedsDraw.h"
#include "trackseeds/SiSeedsGoodRunChecker.h"

R__LOAD_LIBRARY(libqadrawsiliconseeds.so)

void draw_siliconseeds(const std::string &rootfile) {

  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  SiliconSeedsDraw *ex = new SiliconSeedsDraw();
  cl->registerDrawer(ex);

  SiSeedsGoodRunChecker* ch = new SiSeedsGoodRunChecker();
  TCanvas* siseeds_summ = ch->SiSeedsMakeSummary(true); // TESTING
  ex->SetSiSeedsSummary(siseeds_summ);

  cl->ReadHistogramsFromFile(rootfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  std::cout << "Done" << std::endl;
  gSystem->Exit(0);
  return ;
}
