#include <qahtml/QADrawClient.h>
#include <bco/BCODraw.h>
#include <sPhenixStyle.C>
R__LOAD_LIBRARY(libqadrawbco.so)

void draw_packets(const std::string &rootfile) {
  SetsPhenixStyle();
  QADrawClient *cl = QADrawClient::instance();
  /* cl->Verbosity(1); */
  QADraw *ex = new BCODraw();
  cl->registerDrawer(ex);

  cl->ReadHistogramsFromFile(rootfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml();
  delete cl;
  std::cout << "Done" << std::endl;
  gSystem->Exit(0);
  return ;
}
