#include <qahtml/QADrawClient.h>
#include <qahtml/trackseeds/TpcSeedsDraw.h>

R__LOAD_LIBRARY(libqadrawtpcseeds.so)

void draw_tpcseeds(const std::string &rootfile) {

  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  QADraw *ex = new TpcSeedsDraw();
  cl->registerDrawer(ex);

  cl->ReadHistogramsFromFile(rootfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  std::cout << "Done" << std::endl;
  gSystem->Exit(0);
  return ;
}
