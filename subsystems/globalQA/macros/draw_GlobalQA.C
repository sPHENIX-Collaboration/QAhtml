#include <qahtml/QADrawClient.h>
#include <globalqa/GlobalQADraw.h>
#include <sPhenixStyle.C>
R__LOAD_LIBRARY(libqadrawglobalqa.so)
R__LOAD_LIBRARY(libRooFit.so)
R__LOAD_LIBRARY(libRooFitCore.so)

void draw_GlobalQA(const char *rootfile) {
  SetsPhenixStyle();
  QADrawClient *cl = QADrawClient::instance();
  GlobalQADraw *ex = new GlobalQADraw();
  cl->registerDrawer(ex);

  cl->ReadHistogramsFromFile(rootfile);
  cl->Verbosity(1);
  cl->MakeHtml();
  delete cl;
  gSystem->Exit(0);
  return ;
}
