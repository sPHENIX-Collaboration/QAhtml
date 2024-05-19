#include <qahtml/QADrawClient.h>
#include <calo/CaloDraw.h>

R__LOAD_LIBRARY(libqadrawcalo.so)

void draw_calo(const std::string &rootfile) {

  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  QADraw *ex = new CaloDraw();
  cl->registerDrawer(ex);

  cl->ReadHistogramsFromFile(rootfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  gSystem->Exit(0);
  return ;
}
