#include <qahtml/QADrawClient.h>
#include <bco/BCODraw.h>
#include <sPhenixStyle.C>
R__LOAD_LIBRARY(libqadrawbco.so)

void draw_packets(const std::string &rootfile, const std::string& what) {
  SetsPhenixStyle();
  QADrawClient *cl = QADrawClient::instance();
  cl->Verbosity(0); 
  QADraw *ex = new BCODraw();
  cl->registerDrawer(ex);

  cl->ReadHistogramsFromFile(rootfile);
  /* cl->Print("ALL"); */
  cl->MakeHtml(what, "BCOQA");
  delete cl;
  std::cout << "Done" << std::endl;
  gSystem->Exit(0);
  return ;
}
