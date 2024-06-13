#include <qahtml/QADrawClient.h>
#include <mvtx/MVTXDraw.h>

R__LOAD_LIBRARY(libqadrawmvtx.so)

void draw_mvtx(const std::string &rootfile) {

  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  QADraw *ex = new MVTXDraw();
  cl->registerDrawer(ex);

  cl->ReadHistogramsFromFile(rootfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  std::cout << "Done" << std::endl;
  gSystem->Exit(0);
  return ;
}
