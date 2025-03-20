#include <qahtml/QADrawClient.h>
#include <tpc/TPCDraw.h>
#include <sPhenixStyle.C>
R__LOAD_LIBRARY(libqadrawtpc.so)

void draw_tpc(const std::string &rootfile) {
  SetsPhenixStyle();
  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  QADraw *ex = new TPCDraw();
  cl->registerDrawer(ex);

  cl->ReadHistogramsFromFile(rootfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  std::cout << "Done" << std::endl;
  gSystem->Exit(0);
  return ;
}
