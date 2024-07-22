#include <qahtml/QADrawClient.h>
#include <mvtx/MVTXDraw.h>
#include <intt/INTTDraw.h>
#include <tpc/TPCDraw.h>
#include <micromegas/MicromegasDraw.h>

R__LOAD_LIBRARY(libqadrawmvtx.so)
R__LOAD_LIBRARY(libqadrawintt.so)
R__LOAD_LIBRARY(libqadrawtpc.so)
R__LOAD_LIBRARY(libqadrawmicromegas.so)
void draw_clusters(const std::string &rootfile) {

  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */

  cl->registerDrawer(new MVTXDraw);
  cl->registerDrawer(new INTTDraw);
  cl->registerDrawer(new TPCDraw);
  cl->registerDrawer(new MicromegasDraw);

  cl->ReadHistogramsFromFile(rootfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  std::cout << "Done" << std::endl;
  gSystem->Exit(0);
  return ;
}
