#include <qahtml/QADrawClient.h>
#include <qahtml/tpclasers/TPCLasersDraw.h>
#include <sPhenixStyle.C>

R__LOAD_LIBRARY(libqadrawtpclasers.so)

void draw_tpclasers(const std::string &rootfile) {
  SetsPhenixStyle();
  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  QADraw *ex = new TPCLasersDraw();
  cl->registerDrawer(ex);

  cl->ReadHistogramsFromFile(rootfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  std::cout << "Done" << std::endl;
  gSystem->Exit(0);
  return ;
}
