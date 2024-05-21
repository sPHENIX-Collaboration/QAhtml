#include <qahtml/OnlProdClient.h>
#include <qahtml/TPCDraw.h>

R__LOAD_LIBRARY(libonlprodcalo.so)

void draw_calo(const char *rootfile) {

  OnlProdClient *cl = OnlProdClient::instance();
  /* cl->Verbosity(1); */
  OnlProdDraw *ex = new TPCDraw();
  cl->registerDrawer(ex);

  cl->ReadHistogramsFromFile(rootfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  gSystem->Exit(0);
  return ;
}
