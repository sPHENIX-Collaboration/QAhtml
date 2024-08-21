#include <qahtml/QADrawClient.h>
#include <qahtml/tpcseeds/TpcSeedsDraw.h>
#include <sPhenixStyle.C>
R__LOAD_LIBRARY(libqadrawtpcseeds.so)

void draw_tpcseeds(const std::string &rootfile) {
  SetsPhenixStyle();
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
