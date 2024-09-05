#include <qahtml/QADrawClient.h>
#include <mvtx/MVTXRawHitDraw.h>
#include <sPhenixStyle.C>
R__LOAD_LIBRARY(libqadrawmvtxrawhit.so)

void draw_mvtx_rawhit(const std::string &rootfile) {
  SetsPhenixStyle();
  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  QADraw *ex = new MVTXRawHitDraw();
  cl->registerDrawer(ex);

  cl->ReadHistogramsFromFile(rootfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  std::cout << "Done" << std::endl;
  gSystem->Exit(0);
  return ;
}
