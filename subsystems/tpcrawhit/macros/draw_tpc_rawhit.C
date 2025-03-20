#include <qahtml/QADrawClient.h>
#include <tpc/TPCRawHitDraw.h>
#include <sPhenixStyle.C>
R__LOAD_LIBRARY(libqadrawtpcrawhit.so)

void draw_tpc_rawhit(const std::string &rootfile) {
  SetsPhenixStyle();
  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  QADraw *ex = new TPCRawHitDraw();
  cl->registerDrawer(ex);

  cl->ReadHistogramsFromFile(rootfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  std::cout << "Done" << std::endl;
  gSystem->Exit(0);
  return ;
}
